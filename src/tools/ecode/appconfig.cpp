#include "appconfig.hpp"
#include <eepp/network/uri.hpp>
#include <eepp/system/filesystem.hpp>
#include <eepp/system/md5.hpp>
#include <eepp/system/sys.hpp>

using namespace EE::Network;

static PanelPosition panelPositionFromString( const std::string& str ) {
	if ( String::toLower( str ) == "right" )
		return PanelPosition::Right;
	return PanelPosition::Left;
}

static std::string panelPositionToString( const PanelPosition& pos ) {
	if ( pos == PanelPosition::Right )
		return "right";
	return "left";
}

static std::vector<std::string> urlEncode( const std::vector<std::string>& vec ) {
	std::vector<std::string> encoded;
	for ( const auto& item : vec )
		encoded.emplace_back( URI::encode( item ) );
	return encoded;
}

static std::vector<std::string> urlDecode( const std::vector<std::string>& vec ) {
	std::vector<std::string> decoded;
	for ( const auto& item : vec )
		decoded.emplace_back( URI::decode( item ) );
	return decoded;
}

void AppConfig::load( const std::string& confPath, std::string& keybindingsPath,
					  std::string& initColorScheme, std::vector<std::string>& recentFiles,
					  std::vector<std::string>& recentFolders, const std::string& resPath,
					  const Float& displayDPI ) {
	keybindingsPath = confPath + "keybindings.cfg";
	ini.loadFromFile( confPath + "config.cfg" );
	iniState.loadFromFile( confPath + "state.cfg" );
	std::string recent = iniState.getValue( "files", "recentfiles", "" );
	recentFiles = urlDecode( String::split( recent, ';' ) );
	std::string recentFol = iniState.getValue( "folders", "recentfolders", "" );
	recentFolders = urlDecode( String::split( recentFol, ';' ) );
	initColorScheme = editor.colorScheme = ini.getValue( "editor", "colorscheme", "eepp" );
	editor.fontSize = ini.getValue( "editor", "font_size", "11dp" );
	window.size.setWidth( iniState.getValueI( "window", "width", displayDPI > 105 ? 1920 : 1280 ) );
	window.size.setHeight(
		iniState.getValueI( "window", "height", displayDPI > 105 ? 1080 : 720 ) );
	window.maximized = iniState.getValueB( "window", "maximized", false );
	window.pixelDensity = iniState.getValueF( "window", "pixeldensity" );
	window.winIcon = ini.getValue( "window", "winicon", resPath + "icon/ee.png" );
	window.panelPartition = iniState.getValue( "window", "panel_partition", "15%" );
	window.displayIndex = iniState.getValueI( "window", "display_index", 0 );
	window.position.x = iniState.getValueI( "window", "x", -1 );
	window.position.y = iniState.getValueI( "window", "y", -1 );
	editor.showLineNumbers = ini.getValueB( "editor", "show_line_numbers", true );
	editor.showWhiteSpaces = ini.getValueB( "editor", "show_white_spaces", true );
	editor.highlightMatchingBracket =
		ini.getValueB( "editor", "highlight_matching_brackets", true );
	editor.highlightCurrentLine = ini.getValueB( "editor", "highlight_current_line", true );
	editor.verticalScrollbar = ini.getValueB( "editor", "vertical_scrollbar", true );
	editor.horizontalScrollbar = ini.getValueB( "editor", "horizontal_scrollbar", false );
	ui.fontSize = ini.getValue( "ui", "font_size", "11dp" );
	ui.showSidePanel = ini.getValueB( "ui", "show_side_panel", true );
	ui.panelPosition = panelPositionFromString( ini.getValue( "ui", "panel_position", "left" ) );
	ui.serifFont = ini.getValue( "ui", "serif_font", "fonts/NotoSans-Regular.ttf" );
	ui.monospaceFont = ini.getValue( "ui", "monospace_font", "fonts/DejaVuSansMono.ttf" );
	ui.colorScheme = ini.getValue( "ui", "ui_color_scheme", "dark" ) == "light"
						 ? ColorSchemePreference::Light
						 : ColorSchemePreference::Dark;
	doc.trimTrailingWhitespaces = ini.getValueB( "document", "trim_trailing_whitespaces", false );
	doc.forceNewLineAtEndOfFile =
		ini.getValueB( "document", "force_new_line_at_end_of_file", false );
	doc.autoDetectIndentType = ini.getValueB( "document", "auto_detect_indent_type", true );
	doc.writeUnicodeBOM = ini.getValueB( "document", "write_bom", false );
	doc.indentWidth = ini.getValueI( "document", "indent_width", 4 );
	doc.indentSpaces = ini.getValueB( "document", "indent_spaces", false );
	doc.windowsLineEndings = ini.getValueB( "document", "windows_line_endings", false );
	doc.tabWidth = eemax( 2, ini.getValueI( "document", "tab_width", 4 ) );
	doc.lineBreakingColumn = eemax( 0, ini.getValueI( "document", "line_breaking_column", 100 ) );
	editor.autoCloseBrackets = ini.getValue( "editor", "auto_close_brackets", "" );
	editor.highlightSelectionMatch = ini.getValueB( "editor", "highlight_selection_match", true );
	editor.colorPickerSelection = ini.getValueB( "editor", "color_picker_selection", true );
	editor.colorPreview = ini.getValueB( "editor", "color_preview", true );
	editor.minimap = ini.getValueB( "editor", "minimap", true );
	editor.autoComplete = ini.getValueB( "editor", "auto_complete", true );
	editor.linter = ini.getValueB( "editor", "linter", true );
	editor.formatter = ini.getValueB( "editor", "formatter", true );
	editor.showDocInfo = ini.getValueB( "editor", "show_doc_info", true );
	editor.hideTabBarOnSingleTab = ini.getValueB( "editor", "hide_tab_bar_on_single_tab", true );
	editor.singleClickTreeNavigation =
		ini.getValueB( "editor", "single_click_tree_navigation", false );
	editor.syncProjectTreeWithEditor =
		ini.getValueB( "editor", "sync_project_tree_with_editor", false );
	editor.autoCloseXMLTags = ini.getValueB( "editor", "auto_close_xml_tags", true );
	iniInfo = FileInfo( ini.path() );
}

void AppConfig::save( const std::vector<std::string>& recentFiles,
					  const std::vector<std::string>& recentFolders,
					  const std::string& panelPartition, EE::Window::Window* win,
					  const std::string& colorSchemeName ) {

	FileInfo configInfo( ini.path() );
	if ( iniInfo.getModificationTime() != 0 &&
		 iniInfo.getModificationTime() != configInfo.getModificationTime() ) {
		ini.loadFromFile( ini.path() );
	}

	editor.colorScheme = colorSchemeName;
	window.size = win->getLastWindowedSize();
	window.maximized = win->isMaximized();
	window.displayIndex = win->getCurrentDisplayIndex();
	window.position = win->getPosition() - win->getBorderSize().getSize();
	ini.setValue( "editor", "colorscheme", editor.colorScheme );
	iniState.setValueI( "window", "width", window.size.getWidth() );
	iniState.setValueI( "window", "height", window.size.getHeight() );
	iniState.setValueB( "window", "maximized", window.maximized );
	iniState.setValueF( "window", "pixeldensity", window.pixelDensity );
	iniState.setValue( "window", "panel_partition", panelPartition );
	iniState.setValueI( "window", "display_index", window.displayIndex );
	iniState.setValueI( "window", "x", window.position.x );
	iniState.setValueI( "window", "y", window.position.y );
	iniState.setValue( "files", "recentfiles", String::join( urlEncode( recentFiles ), ';' ) );
	iniState.setValue( "folders", "recentfolders",
					   String::join( urlEncode( recentFolders ), ';' ) );
	ini.setValueB( "editor", "show_line_numbers", editor.showLineNumbers );
	ini.setValueB( "editor", "show_white_spaces", editor.showWhiteSpaces );
	ini.setValueB( "editor", "highlight_matching_brackets", editor.highlightMatchingBracket );
	ini.setValueB( "editor", "highlight_current_line", editor.highlightCurrentLine );
	ini.setValueB( "editor", "vertical_scrollbar", editor.verticalScrollbar );
	ini.setValueB( "editor", "horizontal_scrollbar", editor.horizontalScrollbar );
	ini.setValue( "editor", "font_size", editor.fontSize.toString() );
	ini.setValue( "ui", "font_size", ui.fontSize.toString() );
	ini.setValueB( "ui", "show_side_panel", ui.showSidePanel );
	ini.setValue( "ui", "panel_position", panelPositionToString( ui.panelPosition ) );
	ini.setValue( "ui", "serif_font", ui.serifFont );
	ini.setValue( "ui", "monospace_font", ui.monospaceFont );
	ini.setValue( "ui", "ui_color_scheme",
				  ui.colorScheme == ColorSchemePreference::Light ? "light" : "dark" );
	ini.setValueB( "document", "trim_trailing_whitespaces", doc.trimTrailingWhitespaces );
	ini.setValueB( "document", "force_new_line_at_end_of_file", doc.forceNewLineAtEndOfFile );
	ini.setValueB( "document", "auto_detect_indent_type", doc.autoDetectIndentType );
	ini.setValueB( "document", "write_bom", doc.writeUnicodeBOM );
	ini.setValueI( "document", "indent_width", doc.indentWidth );
	ini.setValueB( "document", "indent_spaces", doc.indentSpaces );
	ini.setValueB( "document", "windows_line_endings", doc.windowsLineEndings );
	ini.setValueI( "document", "tab_width", doc.tabWidth );
	ini.setValueI( "document", "line_breaking_column", doc.lineBreakingColumn );
	ini.setValue( "editor", "auto_close_brackets", editor.autoCloseBrackets );
	ini.setValueB( "editor", "highlight_selection_match", editor.highlightSelectionMatch );
	ini.setValueB( "editor", "color_picker_selection", editor.colorPickerSelection );
	ini.setValueB( "editor", "color_preview", editor.colorPreview );
	ini.setValueB( "editor", "minimap", editor.minimap );
	ini.setValueB( "editor", "auto_complete", editor.autoComplete );
	ini.setValueB( "editor", "linter", editor.linter );
	ini.setValueB( "editor", "formatter", editor.formatter );
	ini.setValueB( "editor", "show_doc_info", editor.showDocInfo );
	ini.setValueB( "editor", "hide_tab_bar_on_single_tab", editor.hideTabBarOnSingleTab );
	ini.setValueB( "editor", "single_click_tree_navigation", editor.singleClickTreeNavigation );
	ini.setValueB( "editor", "sync_project_tree_with_editor", editor.syncProjectTreeWithEditor );
	ini.setValueB( "editor", "auto_close_xml_tags", editor.autoCloseXMLTags );
	ini.writeFile();
	iniState.writeFile();
}

struct ProjectPath {
	std::string path;
	TextRange selection{ { 0, 0 }, { 0, 0 } };
	ProjectPath() {}
	ProjectPath( const std::string& path, const TextRange& selection ) :
		path( path ), selection( selection ) {}

	std::string toString() { return URI::encode( path ) + ";" + selection.toString(); }

	static ProjectPath fromString( const std::string& str ) {
		auto split = String::split( str, ';' );
		if ( !split.empty() ) {
			ProjectPath pp;
			pp.path = URI::decode( split[0] );
			pp.selection = split.size() >= 2
							   ? TextRange::fromString( split[1] )
							   : TextRange( TextPosition( 0, 0 ), TextPosition( 0, 0 ) );
			return pp;
		}
		return {};
	}
};

void AppConfig::saveProject( std::string projectFolder, UICodeEditorSplitter* editorSplitter,
							 const std::string& configPath,
							 const ProjectDocumentConfig& docConfig ) {
	FileSystem::dirAddSlashAtEnd( projectFolder );
	std::vector<UICodeEditor*> editors = editorSplitter->getAllEditors();
	std::vector<ProjectPath> paths;
	for ( auto editor : editors )
		if ( editor->getDocument().hasFilepath() )
			paths.emplace_back( ProjectPath{ editor->getDocument().getFilePath(),
											 editor->getDocument().getSelection() } );
	std::string projectsPath( configPath + "projects" + FileSystem::getOSSlash() );
	if ( !FileSystem::fileExists( projectsPath ) )
		FileSystem::makeDir( projectsPath );
	MD5::Result hash = MD5::fromString( projectFolder );
	std::string projectCfgPath( projectsPath + hash.toHexString() + ".cfg" );
	IniFile ini( projectCfgPath, false );
	ini.setValue( "path", "folder_path", projectFolder );
	for ( size_t i = 0; i < paths.size(); i++ )
		ini.setValue( "files", String::format( "file_name_%lu", i ), paths[i].toString() );
	ini.setValueI( "files", "current_page",
				   !editorSplitter->getTabWidgets().empty()
					   ? editorSplitter->getTabWidgets()[0]->getTabSelectedIndex()
					   : 0 );
	ini.setValueB( "document", "use_global_settings", docConfig.useGlobalSettings );
	ini.setValueB( "document", "trim_trailing_whitespaces", docConfig.doc.trimTrailingWhitespaces );
	ini.setValueB( "document", "force_new_line_at_end_of_file",
				   docConfig.doc.forceNewLineAtEndOfFile );
	ini.setValueB( "document", "auto_detect_indent_type", docConfig.doc.autoDetectIndentType );
	ini.setValueB( "document", "write_bom", docConfig.doc.writeUnicodeBOM );
	ini.setValueI( "document", "indent_width", docConfig.doc.indentWidth );
	ini.setValueB( "document", "indent_spaces", docConfig.doc.indentSpaces );
	ini.setValueB( "document", "windows_line_endings", docConfig.doc.windowsLineEndings );
	ini.setValueI( "document", "tab_width", docConfig.doc.tabWidth );
	ini.setValueI( "document", "line_breaking_column", docConfig.doc.lineBreakingColumn );
	ini.writeFile();
}

void AppConfig::loadProject( std::string projectFolder, UICodeEditorSplitter* editorSplitter,
							 const std::string& configPath, ProjectDocumentConfig& docConfig,
							 std::shared_ptr<ThreadPool> pool ) {
	FileSystem::dirAddSlashAtEnd( projectFolder );
	std::string projectsPath( configPath + "projects" + FileSystem::getOSSlash() );
	MD5::Result hash = MD5::fromString( projectFolder );
	std::string projectCfgPath( projectsPath + hash.toHexString() + ".cfg" );
	if ( !FileSystem::fileExists( projectCfgPath ) )
		return;
	IniFile ini( projectCfgPath );
	bool found;
	size_t i = 0;
	std::vector<ProjectPath> paths;
	do {
		std::string val( ini.getValue( "files", String::format( "file_name_%lu", i ) ) );
		found = !val.empty();
		if ( found ) {
			auto pp = ProjectPath::fromString( val );
			if ( FileSystem::fileExists( pp.path ) )
				paths.emplace_back( pp );
		}
		i++;
	} while ( found );

	Int64 currentPage = ini.getValueI( "files", "current_page" );
	size_t totalToLoad = paths.size();

	for ( auto& pp : paths ) {
		editorSplitter->loadAsyncFileFromPathInNewTab(
			pp.path, pool,
			[pp, editorSplitter, totalToLoad, currentPage]( UICodeEditor* editor,
															const std::string& ) {
				editor->getDocument().setSelection( pp.selection );
				editor->scrollToCursor();

				if ( !editorSplitter->getTabWidgets().empty() &&
					 editorSplitter->getTabWidgets()[0]->getTabCount() == totalToLoad )
					editorSplitter->switchToTab( currentPage );
			} );
	}

	docConfig.useGlobalSettings = ini.getValueB( "document", "use_global_settings", true );
	docConfig.doc.trimTrailingWhitespaces =
		ini.getValueB( "document", "trim_trailing_whitespaces", false );
	docConfig.doc.forceNewLineAtEndOfFile =
		ini.getValueB( "document", "force_new_line_at_end_of_file", false );
	docConfig.doc.autoDetectIndentType =
		ini.getValueB( "document", "auto_detect_indent_type", true );
	docConfig.doc.writeUnicodeBOM = ini.getValueB( "document", "write_bom", false );
	docConfig.doc.indentWidth = ini.getValueI( "document", "indent_width", 4 );
	docConfig.doc.indentSpaces = ini.getValueB( "document", "indent_spaces", false );
	docConfig.doc.windowsLineEndings = ini.getValueB( "document", "windows_line_endings", false );
	docConfig.doc.tabWidth = eemax( 2, ini.getValueI( "document", "tab_width", 4 ) );
	docConfig.doc.lineBreakingColumn =
		eemax( 0, ini.getValueI( "document", "line_breaking_column", 100 ) );
}