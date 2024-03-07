#include <eepp/ui/doc/languages/sb.hpp>
#include <eepp/ui/doc/syntaxdefinitionmanager.hpp>

namespace EE { namespace UI { namespace Doc { namespace Language {

void addSmallBASIC() {

	SyntaxDefinitionManager::instance()->add(

		{ "SmallBASIC",
		  { "%.bas$" },
		  {
			  { { "'.*" }, "comment" },
			  { { "REM%s.*" }, "comment" },
			  { { "\"", "\"" }, "string" },
			  { { "-?&H%x+" }, "number" },
			  { { "-?%d+[%d%.eE]*" }, "number" },
			  { { "-?%.?%d+f?" }, "number" },
			  { { "[=><%+%-%*%^&:%.,_%(%)]" }, "operator" },
			  { { "[%a_][%w_]*" }, "symbol" },

		  },
		  {

			  { "abs", "keyword" },
			  { "absmax", "keyword" },
			  { "absmin", "keyword" },
			  { "access", "keyword" },
			  { "acos", "keyword" },
			  { "acosh", "keyword" },
			  { "acot", "keyword" },
			  { "acoth", "keyword" },
			  { "acsc", "keyword" },
			  { "acsch", "keyword" },
			  { "and", "keyword" },
			  { "append", "keyword" },
			  { "arc", "keyword" },
			  { "array", "keyword" },
			  { "as", "keyword" },
			  { "asc", "keyword" },
			  { "asec", "keyword" },
			  { "asech", "keyword" },
			  { "asin", "keyword" },
			  { "asinh", "keyword" },
			  { "at", "keyword" },
			  { "atan", "keyword" },
			  { "atan2", "keyword" },
			  { "atanh", "keyword" },
			  { "atn", "keyword" },
			  { "band", "keyword" },
			  { "bcs", "keyword" },
			  { "beep", "keyword" },
			  { "bg", "keyword" },
			  { "bgetc", "keyword" },
			  { "bin", "keyword" },
			  { "bload", "keyword" },
			  { "bor", "keyword" },
			  { "bputc", "keyword" },
			  { "bsave", "keyword" },
			  { "byref", "keyword" },
			  { "call", "keyword" },
			  { "case", "keyword" },
			  { "cat", "keyword" },
			  { "catch", "keyword" },
			  { "cbs", "keyword" },
			  { "cdbl", "keyword" },
			  { "ceil", "keyword" },
			  { "chain", "keyword" },
			  { "chart", "keyword" },
			  { "chdir", "keyword" },
			  { "chmod", "keyword" },
			  { "chop", "keyword" },
			  { "chr", "keyword" },
			  { "cint", "keyword" },
			  { "circle", "keyword" },
			  { "close", "keyword" },
			  { "cls", "keyword" },
			  { "color", "keyword" },
			  { "command", "keyword" },
			  { "const", "keyword" },
			  { "copy", "keyword" },
			  { "cos", "keyword" },
			  { "cosh", "keyword" },
			  { "cot", "keyword" },
			  { "coth", "keyword" },
			  { "creal", "keyword" },
			  { "csc", "keyword" },
			  { "csch", "keyword" },
			  { "cwd", "keyword" },
			  { "data", "keyword" },
			  { "date", "keyword" },
			  { "datedmy", "keyword" },
			  { "datefmt", "keyword" },
			  { "declare", "keyword" },
			  { "def", "keyword" },
			  { "definekey", "keyword" },
			  { "deg", "keyword" },
			  { "delay", "keyword" },
			  { "delete", "keyword" },
			  { "deriv", "keyword" },
			  { "determ", "keyword" },
			  { "diffeqn", "keyword" },
			  { "dim", "keyword" },
			  { "dirwalk", "keyword" },
			  { "disclose", "keyword" },
			  { "do", "keyword" },
			  { "draw", "keyword" },
			  { "drawpoly", "keyword" },
			  { "elif", "keyword" },
			  { "else", "keyword" },
			  { "elseif", "keyword" },
			  { "empty", "keyword" },
			  { "enclose", "keyword" },
			  { "end", "keyword" },
			  { "end", "keyword" },
			  { "try", "keyword" },
			  { "endif", "keyword" },
			  { "env", "keyword" },
			  { "env", "keyword" },
			  { "eof", "keyword" },
			  { "eqv", "keyword" },
			  { "erase", "keyword" },
			  { "exec", "keyword" },
			  { "exist", "keyword" },
			  { "exit", "keyword" },
			  { "exp", "keyword" },
			  { "export", "keyword" },
			  { "exprseq", "keyword" },
			  { "false", "keyword" },
			  { "fi", "keyword" },
			  { "files", "keyword" },
			  { "fix", "keyword" },
			  { "floor", "keyword" },
			  { "for", "keyword" },
			  { "form", "keyword" },
			  { "format", "keyword" },
			  { "frac", "keyword" },
			  { "fre", "keyword" },
			  { "freefile", "keyword" },
			  { "func", "keyword" },
			  { "gosub", "keyword" },
			  { "goto", "keyword" },
			  { "hex", "keyword" },
			  { "home", "keyword" },
			  { "if", "keyword" },
			  { "iff", "keyword" },
			  { "image", "keyword" },
			  { "imp", "keyword" },
			  { "import", "keyword" },
			  { "in", "keyword" },
			  { "include", "keyword" },
			  { "inkey", "keyword" },
			  { "input", "keyword" },
			  { "input", "keyword" },
			  { "input", "keyword" },
			  { "insert", "keyword" },
			  { "instr", "keyword" },
			  { "int", "keyword" },
			  { "intersect", "keyword" },
			  { "inverse", "keyword" },
			  { "isarray", "keyword" },
			  { "isdir", "keyword" },
			  { "isfile", "keyword" },
			  { "islink", "keyword" },
			  { "ismap", "keyword" },
			  { "isnumber", "keyword" },
			  { "isstring", "keyword" },
			  { "join", "keyword" },
			  { "julian", "keyword" },
			  { "kill", "keyword" },
			  { "label", "keyword" },
			  { "lbound", "keyword" },
			  { "lcase", "keyword" },
			  { "left", "keyword" },
			  { "leftof", "keyword" },
			  { "leftoflast", "keyword" },
			  { "len", "keyword" },
			  { "let", "keyword" },
			  { "like", "keyword" },
			  { "line", "keyword" },
			  { "lineinput", "keyword" },
			  { "lineqn", "keyword" },
			  { "linput", "keyword" },
			  { "local", "keyword" },
			  { "locate", "keyword" },
			  { "lock", "keyword" },
			  { "lof", "keyword" },
			  { "log", "keyword" },
			  { "log10", "keyword" },
			  { "logprint", "keyword" },
			  { "lower", "keyword" },
			  { "lshift", "keyword" },
			  { "ltrim", "keyword" },
			  { "m3apply", "keyword" },
			  { "m3ident", "keyword" },
			  { "m3rotate", "keyword" },
			  { "m3scale", "keyword" },
			  { "m3trans", "keyword" },
			  { "max", "keyword" },
			  { "maxint", "keyword" },
			  { "mdl", "keyword" },
			  { "mid", "keyword" },
			  { "min", "keyword" },
			  { "mkdir", "keyword" },
			  { "mod", "keyword" },
			  { "nand", "keyword" },
			  { "next", "keyword" },
			  { "nil", "keyword" },
			  { "nor", "keyword" },
			  { "nosound", "keyword" },
			  { "not", "keyword" },
			  { "oct", "keyword" },
			  { "on", "keyword" },
			  { "open", "keyword" },
			  { "option", "keyword" },
			  { "or", "keyword" },
			  { "paint", "keyword" },
			  { "pause", "keyword" },
			  { "pen", "keyword" },
			  { "pen", "keyword" },
			  { "pi", "keyword" },
			  { "play", "keyword" },
			  { "plot", "keyword" },
			  { "point", "keyword" },
			  { "polyarea", "keyword" },
			  { "polycent", "keyword" },
			  { "polyext", "keyword" },
			  { "pow", "keyword" },
			  { "print", "keyword" },
			  { "progline", "keyword" },
			  { "pset", "keyword" },
			  { "ptdistln", "keyword" },
			  { "ptdistseg", "keyword" },
			  { "ptsign", "keyword" },
			  { "rad", "keyword" },
			  { "randomize", "keyword" },
			  { "read", "keyword" },
			  { "rect", "keyword" },
			  { "redim", "keyword" },
			  { "rem", "keyword" },
			  { "rename", "keyword" },
			  { "repeat", "keyword" },
			  { "replace", "keyword" },
			  { "restore", "keyword" },
			  { "return", "keyword" },
			  { "rgb", "keyword" },
			  { "rgbf", "keyword" },
			  { "right", "keyword" },
			  { "rightof", "keyword" },
			  { "rightoflast", "keyword" },
			  { "rinstr", "keyword" },
			  { "rmdir", "keyword" },
			  { "rnd", "keyword" },
			  { "root", "keyword" },
			  { "round", "keyword" },
			  { "rshift", "keyword" },
			  { "rtrim", "keyword" },
			  { "run", "keyword" },
			  { "sbver", "keyword" },
			  { "search", "keyword" },
			  { "sec", "keyword" },
			  { "sech", "keyword" },
			  { "seek", "keyword" },
			  { "seek", "keyword" },
			  { "segcos", "keyword" },
			  { "seglen", "keyword" },
			  { "segsin", "keyword" },
			  { "select", "keyword" },
			  { "self", "keyword" },
			  { "seq", "keyword" },
			  { "sgn", "keyword" },
			  { "showpage", "keyword" },
			  { "sin", "keyword" },
			  { "sinh", "keyword" },
			  { "sinput", "keyword" },
			  { "sort", "keyword" },
			  { "sound", "keyword" },
			  { "space", "keyword" },
			  { "spc", "keyword" },
			  { "split", "keyword" },
			  { "sprint", "keyword" },
			  { "sqr", "keyword" },
			  { "squeeze", "keyword" },
			  { "statmean", "keyword" },
			  { "statmeandev", "keyword" },
			  { "statspreadp", "keyword" },
			  { "statspreads", "keyword" },
			  { "step", "keyword" },
			  { "stkdump", "keyword" },
			  { "stop", "keyword" },
			  { "str", "keyword" },
			  { "string", "keyword" },
			  { "sub", "keyword" },
			  { "sum", "keyword" },
			  { "sumsq", "keyword" },
			  { "swap", "keyword" },
			  { "tab", "keyword" },
			  { "tan", "keyword" },
			  { "tanh", "keyword" },
			  { "textheight", "keyword" },
			  { "textwidth", "keyword" },
			  { "then", "keyword" },
			  { "throw", "keyword" },
			  { "ticks", "keyword" },
			  { "time", "keyword" },
			  { "timehms", "keyword" },
			  { "timer", "keyword" },
			  { "timestamp", "keyword" },
			  { "tload", "keyword" },
			  { "to", "keyword" },
			  { "translate", "keyword" },
			  { "trim", "keyword" },
			  { "troff", "keyword" },
			  { "tron", "keyword" },
			  { "true", "keyword" },
			  { "try", "keyword" },
			  { "tsave", "keyword" },
			  { "txth", "keyword" },
			  { "txtw", "keyword" },
			  { "ubound", "keyword" },
			  { "ucase", "keyword" },
			  { "unit", "keyword" },
			  { "until", "keyword" },
			  { "upper", "keyword" },
			  { "use", "keyword" },
			  { "usg", "keyword" },
			  { "using", "keyword" },
			  { "val", "keyword" },
			  { "view", "keyword" },
			  { "weekday", "keyword" },
			  { "wend", "keyword" },
			  { "while", "keyword" },
			  { "window", "keyword" },
			  { "write", "keyword" },
			  { "xmax", "keyword" },
			  { "xnor", "keyword" },
			  { "xor", "keyword" },
			  { "xpos", "keyword" },
			  { "ymax", "keyword" },
			  { "ypos", "keyword" },
			  { "include", "keyword2" } },
		  "'",
		  {}

		} );
}

}}}} // namespace EE::UI::Doc::Language
