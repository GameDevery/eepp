#include <eepp/ui/uidefaulttheme.hpp>
#include <eepp/ui/uiwindow.hpp>
#include <eepp/ui/uiwinmenu.hpp>
#include <eepp/ui/uipopupmenu.hpp>
#include <eepp/ui/uiprogressbar.hpp>
#include <eepp/ui/uicommondialog.hpp>
#include <eepp/ui/uimessagebox.hpp>
#include <eepp/ui/uitabwidget.hpp>

namespace EE { namespace UI {

UIdefaultTheme::UIdefaultTheme( const std::string& Name, const std::string& Abbr, Graphics::Font * defaultFont ) :
	UITheme( Name, Abbr, defaultFont )
{
	fontColor( ColorA( 230, 230, 230, 255 ) );
	fontOverColor( ColorA( 255, 255, 255, 255 ) );
	fontSelectedColor( ColorA( 255, 255, 255, 255 ) );
	fontShadowColor( ColorA( 50, 50, 50, 150 ) );
}

UIPopUpMenu * UIdefaultTheme::createPopUpMenu( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 RowHeight, Recti PaddingContainer, Uint32 MinWidth, Uint32 MinSpaceForIcons, Uint32 MinRightMargin ) {
	UIPopUpMenu::CreateParams MenuParams;
	MenuParams.setParent( Parent );
	MenuParams.setPos( Pos );
	MenuParams.setSize( Size );
	MenuParams.Flags = Flags;
	MenuParams.RowHeight = RowHeight;
	MenuParams.PaddingContainer = PaddingContainer;
	MenuParams.MinWidth = MinWidth;
	MenuParams.MinSpaceForIcons = MinSpaceForIcons;
	MenuParams.MinRightMargin = MinRightMargin;

	if ( useDefaultThemeValues() ) {
		MenuParams.MinWidth = 100;
		MenuParams.MinSpaceForIcons = 24;
		MenuParams.MinRightMargin = 8;
		MenuParams.FontColor = ColorA( 230, 230, 230, 255 );
	}

	return eeNew( UIPopUpMenu, ( MenuParams ) );
}

UIProgressBar * UIdefaultTheme::createProgressBar( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, bool DisplayPercent, bool VerticalExpand, Vector2f MovementSpeed, Rectf FillerMargin ) {
	UIProgressBar::CreateParams PBParams;
	PBParams.setParent( Parent );
	PBParams.setPos( Pos );
	PBParams.setSize( Size );
	PBParams.Flags = Flags;
	PBParams.DisplayPercent = DisplayPercent;
	PBParams.VerticalExpand = VerticalExpand;
	PBParams.MovementSpeed = MovementSpeed;
	PBParams.FillerMargin = FillerMargin;

	if ( useDefaultThemeValues() ) {
		PBParams.Flags |= UI_AUTO_SIZE;
		PBParams.DisplayPercent = true;
		PBParams.VerticalExpand = true;
		PBParams.FillerMargin = Rectf( 2, 2, 2, 2 );
		PBParams.MovementSpeed = Vector2f( 32, 0 );
	}

	UIProgressBar * Ctrl = eeNew( UIProgressBar, ( PBParams ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

UIWinMenu * UIdefaultTheme::createWinMenu( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MarginBetweenButtons, Uint32 ButtonMargin, Uint32 MenuHeight, Uint32 FirstButtonMargin ) {
	UIWinMenu::CreateParams WinMenuParams;
	WinMenuParams.setParent( Parent );
	WinMenuParams.setPos( Pos );
	WinMenuParams.setSize( Size );
	WinMenuParams.Flags = Flags;
	WinMenuParams.MarginBetweenButtons = MarginBetweenButtons;
	WinMenuParams.ButtonMargin = ButtonMargin;
	WinMenuParams.MenuHeight = MenuHeight;
	WinMenuParams.FirstButtonMargin = FirstButtonMargin;

	if ( useDefaultThemeValues() ) {
		WinMenuParams.ButtonMargin = 12;
	}

	UIWinMenu * Ctrl = eeNew( UIWinMenu, ( WinMenuParams ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

UIWindow * UIdefaultTheme::createWindow( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 WinFlags, Sizei MinWindowSize, Uint8 BaseAlpha ) {
	UIWindow::CreateParams WinParams;
	WinParams.setParent( Parent );
	WinParams.setPos( Pos );
	WinParams.setSize( Size );
	WinParams.Flags = Flags;
	WinParams.WinFlags = WinFlags;
	WinParams.MinWindowSize = MinWindowSize;
	WinParams.BaseAlpha = BaseAlpha;

	if ( useDefaultThemeValues() ) {
		WinParams.Flags |= UI_DRAW_SHADOW;
		WinParams.WinFlags |= UI_WIN_DRAW_SHADOW;
		WinParams.ButtonsPositionFixer.x = -2;
		WinParams.TitleFontColor = ColorA( 230, 230, 230, 255 );
	}

	return eeNew( UIWindow, ( WinParams ) );
}

UICommonDialog * UIdefaultTheme::createCommonDialog( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 WinFlags, Sizei MinWindowSize, Uint8 BaseAlpha, Uint32 CDLFlags, std::string DefaultFilePattern, std::string DefaultDirectory ) {
	UICommonDialog::CreateParams DLGParams;
	DLGParams.setParent( Parent );
	DLGParams.setPos( Pos );
	DLGParams.setSize( Size );
	DLGParams.Flags = Flags;
	DLGParams.WinFlags = WinFlags;
	DLGParams.MinWindowSize = MinWindowSize;
	DLGParams.BaseAlpha = BaseAlpha;
	DLGParams.DefaultDirectory = DefaultDirectory;
	DLGParams.DefaultFilePattern = DefaultFilePattern;
	DLGParams.CDLFlags = CDLFlags;

	if ( useDefaultThemeValues() ) {
		DLGParams.Flags |= UI_DRAW_SHADOW;
		DLGParams.WinFlags |= UI_WIN_DRAW_SHADOW;
		DLGParams.ButtonsPositionFixer.x = -2;
		DLGParams.TitleFontColor = ColorA( 230, 230, 230, 255 );
	}

	return eeNew( UICommonDialog, ( DLGParams ) );
}

UIMessageBox * UIdefaultTheme::createMessageBox( UI_MSGBOX_TYPE Type, const String& Message, Uint32 WinFlags, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Sizei MinWindowSize, Uint8 BaseAlpha ) {
	UIMessageBox::CreateParams MsgBoxParams;
	MsgBoxParams.setParent( Parent );
	MsgBoxParams.setPos( Pos );
	MsgBoxParams.setSize( Size );
	MsgBoxParams.Flags = Flags;
	MsgBoxParams.WinFlags = WinFlags;
	MsgBoxParams.MinWindowSize = MinWindowSize;
	MsgBoxParams.BaseAlpha = BaseAlpha;
	MsgBoxParams.Type = Type;
	MsgBoxParams.Message = Message;

	if ( useDefaultThemeValues() ) {
		MsgBoxParams.Flags |= UI_DRAW_SHADOW;
		MsgBoxParams.WinFlags |= UI_WIN_DRAW_SHADOW;
		MsgBoxParams.ButtonsPositionFixer.x = -2;
		MsgBoxParams.TitleFontColor = ColorA( 230, 230, 230, 255 );
	}

	return eeNew( UIMessageBox, ( MsgBoxParams ) );
}

UIComboBox * UIdefaultTheme::createComboBox( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MinNumVisibleItems, bool PopUpToMainControl, UIListBox * ListBox ) {
	UIComboBox::CreateParams ComboParams;
	ComboParams.setParent( Parent );
	ComboParams.setPos( Pos );
	ComboParams.setSize( Size );
	ComboParams.Flags = Flags;
	ComboParams.MinNumVisibleItems = MinNumVisibleItems;
	ComboParams.PopUpToMainControl = PopUpToMainControl;
	ComboParams.ListBox = ListBox;

	if ( useDefaultThemeValues() ) {
		ComboParams.Flags |= UI_AUTO_SIZE | UI_TEXT_SELECTION_ENABLED;
	}

	UIComboBox * Ctrl = eeNew( UIComboBox, ( ComboParams ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

UIDropDownList * UIdefaultTheme::createDropDownList( UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags, Uint32 MinNumVisibleItems, bool PopUpToMainControl, UIListBox * ListBox ) {
	UIDropDownList::CreateParams DDLParams;
	DDLParams.setParent( Parent );
	DDLParams.setPos( Pos );
	DDLParams.setSize( Size );
	DDLParams.Flags = Flags;
	DDLParams.MinNumVisibleItems = MinNumVisibleItems;
	DDLParams.PopUpToMainControl = PopUpToMainControl;
	DDLParams.ListBox = ListBox;

	if ( useDefaultThemeValues() ) {
		DDLParams.Flags |= UI_AUTO_SIZE;
	}

	UIDropDownList * Ctrl = eeNew( UIDropDownList, ( DDLParams ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

UITabWidget * UIdefaultTheme::createTabWidget( UIControl *Parent, const Sizei &Size, const Vector2i &Pos, const Uint32 &Flags, const bool &TabsClosable, const bool &SpecialBorderTabs, const Int32 &TabSeparation, const Uint32 &MaxTextLength, const Uint32 &TabWidgetHeight, const Uint32 &TabTextAlign, const Uint32 &MinTabWidth, const Uint32 &MaxTabWidth ) {
	UITabWidget::CreateParams TabWidgetParams;
	TabWidgetParams.setParent( Parent );
	TabWidgetParams.setPos( Pos );
	TabWidgetParams.setSize( Size );
	TabWidgetParams.Flags = Flags;
	TabWidgetParams.TabsClosable = TabsClosable;
	TabWidgetParams.SpecialBorderTabs = SpecialBorderTabs;
	TabWidgetParams.TabSeparation = TabSeparation;
	TabWidgetParams.MaxTextLength = MaxTextLength;
	TabWidgetParams.TabWidgetHeight = TabWidgetHeight;
	TabWidgetParams.TabTextAlign = TabTextAlign;
	TabWidgetParams.MinTabWidth = MinTabWidth;
	TabWidgetParams.MaxTabWidth = MaxTabWidth;

	if ( useDefaultThemeValues() ) {
		TabWidgetParams.TabSeparation = -1;
		TabWidgetParams.FontSelectedColor = ColorA( 255, 255, 255, 255 );
		TabWidgetParams.DrawLineBelowTabs = true;
		TabWidgetParams.LineBelowTabsColor = ColorA( 0, 0, 0, 255 );
		TabWidgetParams.LineBewowTabsYOffset = -1;
	}

	UITabWidget * Ctrl = eeNew( UITabWidget, ( TabWidgetParams ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

UITooltip * UIdefaultTheme::createTooltip( UIControl * TooltipOf, UIControl * Parent, const Sizei& Size, const Vector2i& Pos, const Uint32& Flags ) {
	UITooltip::CreateParams TooltipParams;
	TooltipParams.setParent( Parent );
	TooltipParams.setPos( Pos );
	TooltipParams.setSize( Size );
	TooltipParams.Flags = Flags;

	if ( useDefaultThemeValues() ) {
		TooltipParams.Flags &= ~UI_AUTO_PADDING;
		TooltipParams.FontColor = ColorA( 0, 0, 0, 255 );
		TooltipParams.Padding = Recti( 4, 6, 4, 6 );
	}

	UITooltip * Ctrl = eeNew( UITooltip, ( TooltipParams, TooltipOf ) );
	Ctrl->visible( true );
	Ctrl->enabled( true );
	return Ctrl;
}

}}