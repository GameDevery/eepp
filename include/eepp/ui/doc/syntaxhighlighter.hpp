#ifndef EE_UI_DOC_SYNTAXHIGHLIGHTER_HPP
#define EE_UI_DOC_SYNTAXHIGHLIGHTER_HPP

#include <eepp/ui/doc/syntaxtokenizer.hpp>
#include <eepp/ui/doc/textdocument.hpp>
#include <map>

namespace EE { namespace UI { namespace Doc {

struct TokenizedLine {
	Uint64 initState;
	String::HashType hash;
	std::vector<SyntaxToken> tokens;
	Uint64 state;
	Uint64 signature;
};

class EE_API SyntaxHighlighter {
  public:
	SyntaxHighlighter( TextDocument* doc );

	void changeDoc( TextDocument* doc );

	void reset();

	void invalidate( Int64 lineIndex );

	const TokenizedLine& getLine( const size_t& index ) const;

	Int64 getFirstInvalidLine() const;

	Int64 getMaxWantedLine() const;

	bool updateDirty( int visibleLinesCount = 40 );

	const SyntaxDefinition& getSyntaxDefinitionFromTextPosition( const TextPosition& position );

  protected:
	TextDocument* mDoc;
	mutable std::map<size_t, TokenizedLine> mLines;
	mutable Int64 mFirstInvalidLine;
	mutable Int64 mMaxWantedLine;
	TokenizedLine tokenizeLine( const size_t& line, const Uint64& state ) const;
};

}}} // namespace EE::UI::Doc

#endif // EE_UI_DOC_SYNTAXHIGHLIGHTER_HPP
