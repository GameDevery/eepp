#include <eepp/ui/doc/languages/css.hpp>
#include <eepp/ui/doc/syntaxdefinitionmanager.hpp>

namespace EE { namespace UI { namespace Doc { namespace Language {

void addCSS() {

	SyntaxDefinitionManager::instance()->add(

		{ "CSS",
		  { "%.css$" },
		  {
			  { { "\\." }, "normal" },
			  { { "//.-\n" }, "comment" },
			  { { "/%*", "%*/" }, "comment" },
			  { { "\"", "\"", "\\" }, "string" },
			  { { "'", "'", "\\" }, "string" },
			  { { "^%s*#[%a%_%-][%w%_%-]*" }, "keyword2" },
			  { { ",%s*#[%a%_%-][%w%_%-]*" }, "keyword2" },
			  { { "#%x%x%x%x?%x?%x?%x?%x?" }, "string" },
			  { { "#[%a%_%-][%w%_%-]*" }, "keyword2" },
			  { { "%-%-%a[%w%-%_]*" }, "keyword2" },
			  { { "-?%d+[%d%.]*p[xt]" }, "number" },
			  { { "-?%d+[%d%.]*deg" }, "number" },
			  { { "-?%d+[%d%.]*" }, "number" },
			  { { "@[%a][%w%_%-]*" }, "keyword2" },
			  { { "%.[%a%_%-][%w%_%-]*" }, "keyword2" },
			  { { "(:)(hover)" }, { "normal", "operator", "literal" } },
			  { { "(:)(focus%-within)" }, { "normal", "operator", "literal" } },
			  { { "(:)(focus)" }, { "normal", "operator", "literal" } },
			  { { "(:)(selected)" }, { "normal", "operator", "literal" } },
			  { { "(:)(pressed)" }, { "normal", "operator", "literal" } },
			  { { "(:)(disabled)" }, { "normal", "operator", "literal" } },
			  { { "(:)(checked)" }, { "normal", "operator", "literal" } },
			  { { "(:)(root)" }, { "normal", "operator", "link" } },
			  { { "(%a+)(%()" }, { "normal", "function", "normal" } },
			  { { "[%a][%w-]*%s*%f[:]" }, "keyword" },
			  { { "[{}:>~!]" }, "operator" },
			  { { "[%a_][%w_]*" }, "symbol" },

		  },
		  {
			  { "orange", "literal" },
			  { "tabwidget", "keyword" },
			  { "olivedrab", "literal" },
			  { "olive", "literal" },
			  { "navajowhite", "literal" },
			  { "mistyrose", "literal" },
			  { "midnightblue", "literal" },
			  { "darkseagreen", "literal" },
			  { "mediumseagreen", "literal" },
			  { "lavenderblush", "literal" },
			  { "mediumaquamarine", "literal" },
			  { "limegreen", "literal" },
			  { "mediumspringgreen", "literal" },
			  { "lightgreen", "literal" },
			  { "lime", "literal" },
			  { "mediumorchid", "literal" },
			  { "gray", "literal" },
			  { "black", "literal" },
			  { "lightyellow", "literal" },
			  { "mediumblue", "literal" },
			  { "lightslategray", "literal" },
			  { "lightseagreen", "literal" },
			  { "tableview", "keyword" },
			  { "input", "keyword" },
			  { "lightpink", "literal" },
			  { "lightslategrey", "literal" },
			  { "lightgrey", "literal" },
			  { "lightgoldenrodyellow", "literal" },
			  { "lightblue", "literal" },
			  { "lemonchiffon", "literal" },
			  { "lawngreen", "literal" },
			  { "mintcream", "literal" },
			  { "pushbutton", "keyword" },
			  { "khaki", "literal" },
			  { "ivory", "literal" },
			  { "moccasin", "literal" },
			  { "indigo", "literal" },
			  { "transparent", "literal" },
			  { "oldlace", "literal" },
			  { "indianred", "literal" },
			  { "hotpink", "literal" },
			  { "sizenwse", "literal" },
			  { "honeydew", "literal" },
			  { "false", "literal" },
			  { "grey", "literal" },
			  { "green", "literal" },
			  { "cornflowerblue", "literal" },
			  { "steelblue", "literal" },
			  { "mediumturquoise", "literal" },
			  { "lightgray", "literal" },
			  { "goldenrod", "literal" },
			  { "greenyellow", "literal" },
			  { "saddlebrown", "literal" },
			  { "lightcyan", "literal" },
			  { "rlay", "keyword" },
			  { "gainsboro", "literal" },
			  { "hslider", "keyword" },
			  { "fuchsia", "literal" },
			  { "forestgreen", "literal" },
			  { "palegoldenrod", "literal" },
			  { "maroon", "literal" },
			  { "floralwhite", "literal" },
			  { "dodgerblue", "literal" },
			  { "mediumvioletred", "literal" },
			  { "dimgray", "literal" },
			  { "peru", "literal" },
			  { "deepskyblue", "literal" },
			  { "textureregion", "keyword" },
			  { "blue", "literal" },
			  { "bisque", "literal" },
			  { "slategray", "literal" },
			  { "treeview", "keyword" },
			  { "scrollbar", "keyword" },
			  { "sizewe", "literal" },
			  { "none", "literal" },
			  { "image", "keyword" },
			  { "checkbox", "keyword" },
			  { "lavender", "literal" },
			  { "cornsilk", "literal" },
			  { "textinput", "keyword" },
			  { "blueviolet", "literal" },
			  { "skyblue", "literal" },
			  { "darkslateblue", "literal" },
			  { "lightsalmon", "literal" },
			  { "a", "keyword" },
			  { "widgettable", "keyword" },
			  { "beige", "literal" },
			  { "darkgray", "literal" },
			  { "crimson", "literal" },
			  { "menucheckbox", "keyword" },
			  { "hscrollbar", "keyword" },
			  { "dimgrey", "literal" },
			  { "sizeall", "literal" },
			  { "aliceblue", "literal" },
			  { "progressbar", "keyword" },
			  { "lightskyblue", "literal" },
			  { "blanchedalmond", "literal" },
			  { "darkslategray", "literal" },
			  { "mediumpurple", "literal" },
			  { "tomato", "literal" },
			  { "paleturquoise", "literal" },
			  { "darkturquoise", "literal" },
			  { "powderblue", "literal" },
			  { "codeeditor", "keyword" },
			  { "wheat", "literal" },
			  { "seashell", "literal" },
			  { "hbox", "keyword" },
			  { "sienna", "literal" },
			  { "salmon", "literal" },
			  { "menubar", "keyword" },
			  { "silver", "literal" },
			  { "tan", "literal" },
			  { "thistle", "literal" },
			  { "sandybrown", "literal" },
			  { "turquoise", "literal" },
			  { "azure", "literal" },
			  { "plum", "literal" },
			  { "vslider", "keyword" },
			  { "waitarrow", "literal" },
			  { "textview", "keyword" },
			  { "springgreen", "literal" },
			  { "widgettablerow", "keyword" },
			  { "vscrollbar", "keyword" },
			  { "wait", "literal" },
			  { "relativelayout", "keyword" },
			  { "palevioletred", "literal" },
			  { "spinbox", "keyword" },
			  { "darkblue", "literal" },
			  { "linearlayout", "keyword" },
			  { "chartreuse", "literal" },
			  { "teal", "literal" },
			  { "darkorchid", "literal" },
			  { "seagreen", "literal" },
			  { "listview", "keyword" },
			  { "red", "literal" },
			  { "selectbutton", "keyword" },
			  { "viewpagerhorizontal", "keyword" },
			  { "vbox", "keyword" },
			  { "slategrey", "literal" },
			  { "gridlayout", "keyword" },
			  { "listbox", "keyword" },
			  { "peachpuff", "literal" },
			  { "coral", "literal" },
			  { "stacklayout", "keyword" },
			  { "center", "literal" },
			  { "lightcoral", "literal" },
			  { "rosybrown", "literal" },
			  { "viewpager", "keyword" },
			  { "tab", "keyword" },
			  { "inputpassword", "keyword" },
			  { "window", "keyword" },
			  { "tooltip", "keyword" },
			  { "scrollview", "keyword" },
			  { "stackwidget", "keyword" },
			  { "textedit", "keyword" },
			  { "loader", "keyword" },
			  { "combobox", "keyword" },
			  { "radiobutton", "keyword" },
			  { "dropdownlist", "keyword" },
			  { "splitter", "keyword" },
			  { "snow", "literal" },
			  { "palegreen", "literal" },
			  { "anchor", "keyword" },
			  { "royalblue", "literal" },
			  { "slider", "keyword" },
			  { "violet", "literal" },
			  { "whitesmoke", "literal" },
			  { "white", "literal" },
			  { "darksalmon", "literal" },
			  { "pink", "literal" },
			  { "slateblue", "literal" },
			  { "darkred", "literal" },
			  { "lightsteelblue", "literal" },
			  { "darkolivegreen", "literal" },
			  { "darkviolet", "literal" },
			  { "yellowgreen", "literal" },
			  { "widget", "keyword" },
			  { "linen", "literal" },
			  { "darkgrey", "literal" },
			  { "menuradiobutton", "keyword" },
			  { "crosshair", "literal" },
			  { "darkcyan", "literal" },
			  { "sprite", "keyword" },
			  { "navy", "literal" },
			  { "darkmagenta", "literal" },
			  { "burlywood", "literal" },
			  { "menuseparator", "keyword" },
			  { "firebrick", "literal" },
			  { "orchid", "literal" },
			  { "chocolate", "literal" },
			  { "darkgreen", "literal" },
			  { "aquamarine", "literal" },
			  { "ghostwhite", "literal" },
			  { "arrow", "literal" },
			  { "antiquewhite", "literal" },
			  { "deeppink", "literal" },
			  { "yellow", "literal" },
			  { "console", "keyword" },
			  { "cadetblue", "literal" },
			  { "touchdraggable", "keyword" },
			  { "gold", "literal" },
			  { "darkkhaki", "literal" },
			  { "viewpagervertical", "keyword" },
			  { "mediumslateblue", "literal" },
			  { "aqua", "literal" },
			  { "brown", "literal" },
			  { "cyan", "literal" },
			  { "darkgoldenrod", "literal" },
			  { "papayawhip", "literal" },
			  { "hand", "literal" },
			  { "purple", "literal" },
			  { "tv", "keyword" },
			  { "sizenesw", "literal" },
			  { "darkslategrey", "literal" },
			  { "button", "keyword" },
			  { "nocursor", "literal" },
			  { "orangered", "literal" },
			  { "ibeam", "literal" },
			  { "darkorange", "literal" },
			  { "true", "literal" },
			  { "sizens", "literal" },
			  { "magenta", "literal" },
			  { "textinputpassword", "keyword" },
			  { "important", "literal" },

		  },
		  "",
		  {}

		} );
}

}}}} // namespace EE::UI::Doc::Language
