#ifndef EE_GRAPHICSCFONT_H
#define EE_GRAPHICSCFONT_H

#include <eepp/graphics/base.hpp>
#include <eepp/graphics/fonthelper.hpp>
#include <eepp/graphics/texturefactory.hpp>
#include <eepp/graphics/textcache.hpp>

namespace EE { namespace Graphics {

/** @brief Font interface class. */
class EE_API Font {
	public:
		virtual ~Font();

		/** Set a text to render
		 * @param Text The Text
		 */
		void setText( const String& Text );

		/** @return The width of the string rendered */
		Float getTextWidth();

		/** @return Assign a new text and then returns his width */
		Float getTextWidth( const String& Text );

		/** @return The current text height */
		Float getTextHeight();

		/** @return The number of lines of the current text */
		virtual int getNumLines();

		/** @return The Font Color */
		const ColorA& color() const;

		/** Set the color of the string rendered */
		void color(const ColorA& color);

		/** @return The Shadow Font Color */
		const ColorA& shadowColor() const;

		/** Set the shadow color of the string rendered */
		void shadowColor(const ColorA& color);

		/** @return The current font size */
		Uint32 getFontSize() const;

		/** @return The current font height */
		Uint32 getFontHeight() const;

		/** @return The recommended line spacing */
		Int32 getLineSkip() const;

		/** @return The font highest ascent (height above base) */
		Int32 getFontAscent() const;

		/** @return The font lowest descent (height below base) */
		Int32 getFontDescent() const;

		/** @return The current text */
		String getText();

		/** @return The last text rendered or setted lines width */
		const std::vector<Float>& getLinesWidth();

		/** Draw a String on the screen
		* @param Text The text to draw
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void draw( const String& Text, const Float& X, const Float& Y, const Uint32& Flags = FONT_DRAW_LEFT, const Vector2f& Scale = Vector2f::One, const Float& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Draw the string seted on the screen
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void draw( const Float& X, const Float& Y, const Uint32& Flags = FONT_DRAW_LEFT, const Vector2f& Scale = Vector2f::One, const Float& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Draw a string on the screen from a cached text
		* @param TextCache The cached text
		* @param X The start x position
		* @param Y The start y position
		* @param Flags Set some flags to the rendering ( for text align )
		* @param Scale The string rendered scale
		* @param Angle The angle of the string rendered
		* @param Effect Set the Blend Mode ( default ALPHA_NORMAL )
		*/
		void draw( TextCache& TextCache, const Float& X, const Float& Y, const Uint32& Flags = FONT_DRAW_LEFT, const Vector2f& Scale = Vector2f::One, const Float& Angle = 0, const EE_BLEND_MODE& Effect = ALPHA_NORMAL );

		/** Shrink the String to a max width
		* @param Str The string to shrink
		* @param MaxWidth The Max Width posible
		*/
		void shrinkText( String& Str, const Uint32& MaxWidth );

		/** Shrink the string to a max width
		* @param Str The string to shrink
		* @param MaxWidth The Max Width posible
		*/
		void shrinkText( std::string& Str, const Uint32& MaxWidth );

		/** Cache the with of the current text */
		void cacheWidth( const String& Text, std::vector<Float>& LinesWidth, Float& CachedWidth, int& NumLines, int& LargestLineCharCount );

		/** @return The font texture id */
		const Uint32& getTexId() const;

		/** @return The type of the instance of the font, can be FONT_TYPE_TTF ( true type font ) or FONT_TYPE_TEX ( texture font ) */
		const Uint32& getType() const;

		/** @return The font name */
		const std::string&	getName() const;

		/** Change the font name ( and id, because it's the font name hash ) */
		void setName( const std::string& setName );

		/** @return The font id */
		const Uint32& getId();

		/** Finds the closest cursor position to the point position */
		Int32 findClosestCursorPosFromPoint( const String & Text, const Vector2i& pos );

		/** Simulates a selection request and return the initial and end cursor position when the selection worked. Otherwise both parameters will be -1. */
		void selectSubStringFromCursor( const String& Text, const Int32& CurPos, Int32& InitCur, Int32& EndCur );

		/** @return The cursor position inside the string */
		Vector2i getCursorPos( const String& Text, const Uint32& Pos );
	protected:
		Uint32 						mType;
		std::string					mFontName;
		Uint32						mFontHash;
		Uint32 						mTexId;
		Uint32 						mHeight;
		Uint32 						mSize;
		Int32						mLineSkip;
		Int32						mAscent;
		Int32						mDescent;

		std::vector<eeGlyph> 		mGlyphs;
		std::vector<eeTexCoords> 	mTexCoords;

		TextCache					mTextCache;

		Font( const Uint32& Type, const std::string& setName );

		void cacheWidth();
};

}}

#endif