#include <eepp/ui/uitextbox.hpp>
#include <eepp/ui/uimanager.hpp>
#include <eepp/ui/uithememanager.hpp>
#include <eepp/graphics/textcache.hpp>
#include <eepp/graphics/font.hpp>
#include <eepp/graphics/primitives.hpp>
#include <eepp/window/clipboard.hpp>

namespace EE { namespace UI {

UITextBox::UITextBox( const UITextBox::CreateParams& Params ) :
	UIComplexControl( Params ),
	mFontColor( Params.FontColor ),
	mFontShadowColor( Params.FontShadowColor ),
	mFontSelectionBackColor( Params.FontSelectionBackColor ),
	mAlignOffset( 0.f, 0.f ),
	mSelCurInit( -1 ),
	mSelCurEnd( -1 )
{
	mTextCache = eeNew( TextCache, () );
	mTextCache->font( Params.Font );
	mTextCache->color( mFontColor );
	mTextCache->shadowColor( mFontShadowColor );

	if ( NULL == Params.Font ) {
		if ( NULL != UIThemeManager::instance()->defaultFont() )
			mTextCache->font( UIThemeManager::instance()->defaultFont() );
		else
			eePRINTL( "UITextBox::UITextBox : Created a UI TextBox without a defined font." );
	}

	autoAlign();
}

UITextBox::~UITextBox() {
	eeSAFE_DELETE( mTextCache );
}

Uint32 UITextBox::getType() const {
	return UI_TYPE_TEXTBOX;
}

bool UITextBox::isType( const Uint32& type ) const {
	return UITextBox::getType() == type ? true : UIComplexControl::isType( type );
}

void UITextBox::draw() {
	if ( mVisible && 0.f != mAlpha ) {
		UIControlAnim::draw();

		drawSelection();

		if ( mTextCache->getTextWidth() ) {
			if ( mFlags & UI_CLIP_ENABLE ) {
				UIManager::instance()->clipEnable(
						mScreenPos.x + mPadding.Left,
						mScreenPos.y + mPadding.Top,
						mSize.width() - mPadding.Left - mPadding.Right,
						mSize.height() - mPadding.Top - mPadding.Bottom
				);
			}

			mTextCache->flags( flags() );
			mTextCache->draw( (Float)mScreenPos.x + mAlignOffset.x + (Float)mPadding.Left, (Float)mScreenPos.y + mAlignOffset.y + (Float)mPadding.Top, Vector2f::One, 0.f, blend() );

			if ( mFlags & UI_CLIP_ENABLE ) {
				UIManager::instance()->clipDisable();
			}
		}
	}
}

Graphics::Font * UITextBox::font() const {
	return mTextCache->font();
}

void UITextBox::font( Graphics::Font * font ) {
	if ( mTextCache->font() != font ) {
		mTextCache->font( font );
		autoShrink();
		autoSize();
		autoAlign();
		onFontChanged();
	}
}

const String& UITextBox::text() {
	if ( mFlags & UI_AUTO_SHRINK_TEXT )
		return mString;

	return mTextCache->text();
}

void UITextBox::text( const String& text ) {
	if ( mFlags & UI_AUTO_SHRINK_TEXT ) {
		mString = text;
		mTextCache->text( mString );
	} else {
		mTextCache->text( text );
	}

	autoShrink();
	autoSize();
	autoAlign();
	onTextChanged();
}

const ColorA& UITextBox::color() const {
	return mFontColor;
}

void UITextBox::color( const ColorA& color ) {
	mFontColor = color;
	mTextCache->color( color );

	alpha( color.a() );
}

const ColorA& UITextBox::shadowColor() const {
	return mFontShadowColor;
}

void UITextBox::shadowColor( const ColorA& color ) {
	mFontShadowColor = color;
	mTextCache->shadowColor( mFontColor );
}

const ColorA& UITextBox::selectionBackColor() const {
	return mFontSelectionBackColor;
}

void UITextBox::selectionBackColor( const ColorA& color ) {
	mFontSelectionBackColor = color;
}

void UITextBox::alpha( const Float& alpha ) {
	UIControlAnim::alpha( alpha );
	mFontColor.Alpha = (Uint8)alpha;
	mFontShadowColor.Alpha = (Uint8)alpha;

	mTextCache->alpha( mFontColor.Alpha );
}

void UITextBox::autoShrink() {
	if ( mFlags & UI_AUTO_SHRINK_TEXT ) {
		shrinkText( mSize.width() );
	}
}

void UITextBox::shrinkText( const Uint32& MaxWidth ) {
	if ( mFlags & UI_AUTO_SHRINK_TEXT ) {
		mTextCache->text( mString );
	}

	mTextCache->font()->shrinkText( mTextCache->text(), MaxWidth );
}

void UITextBox::autoSize() {
	if ( mFlags & UI_AUTO_SIZE ) {
		mSize.width( (int)mTextCache->getTextWidth() );
		mSize.height( (int)mTextCache->getTextHeight() );
	}
}

void UITextBox::autoAlign() {
	switch ( FontHAlignGet( flags() ) ) {
		case UI_HALIGN_CENTER:
			mAlignOffset.x = (Float)( (Int32)( mSize.x - mTextCache->getTextWidth() ) / 2 );
			break;
		case UI_HALIGN_RIGHT:
			mAlignOffset.x = ( (Float)mSize.x - (Float)mTextCache->getTextWidth() );
			break;
		case UI_HALIGN_LEFT:
			mAlignOffset.x = 0.f;
			break;
	}

	switch ( FontVAlignGet( flags() ) ) {
		case UI_VALIGN_CENTER:
			mAlignOffset.y = (Float)( ( (Int32)( mSize.y - mTextCache->getTextHeight() ) ) / 2 ) - 1;
			break;
		case UI_VALIGN_BOTTOM:
			mAlignOffset.y = ( (Float)mSize.y - (Float)mTextCache->getTextHeight() );
			break;
		case UI_VALIGN_TOP:
			mAlignOffset.y = 0.f;
			break;
	}
}

Uint32 UITextBox::onFocusLoss() {
	selCurInit( -1 );
	selCurEnd( -1 );

	return 1;
}

void UITextBox::onSizeChange() {
	autoShrink();
	autoSize();
	autoAlign();

	UIControlAnim::onSizeChange();

	mTextCache->cache();
}

void UITextBox::onTextChanged() {
	sendCommonEvent( UIEvent::EventOnTextChanged );
}

void UITextBox::onFontChanged() {
	sendCommonEvent( UIEvent::EventOnFontChanged );
}

void UITextBox::padding( const Recti& padding ) {
	mPadding = padding;
}

const Recti& UITextBox::padding() const {
	return mPadding;
}

void UITextBox::setTheme( UITheme * Theme ) {
	UIControlAnim::setTheme( Theme );

	if ( NULL == mTextCache->font() && NULL != Theme->font() ) {
		mTextCache->font( Theme->font() );
	}
}

TextCache * UITextBox::getTextCache() {
	return mTextCache;
}

Float UITextBox::getTextWidth() {
	return mTextCache->getTextWidth();
}

Float UITextBox::getTextHeight() {
	return mTextCache->getTextHeight();
}

const int& UITextBox::getNumLines() const {
	return mTextCache->getNumLines();
}

const Vector2f& UITextBox::alignOffset() const {
	return mAlignOffset;
}

Uint32 UITextBox::onMouseDoubleClick( const Vector2i& Pos, const Uint32 Flags ) {
	if ( isTextSelectionEnabled() && ( Flags & EE_BUTTON_LMASK ) ) {
		Vector2i controlPos( Pos );
		worldToControl( controlPos );

		Int32 curPos = mTextCache->font()->findClosestCursorPosFromPoint( mTextCache->text(), controlPos );

		if ( -1 != curPos ) {
			Int32 tSelCurInit, tSelCurEnd;

			mTextCache->font()->selectSubStringFromCursor( mTextCache->text(), curPos, tSelCurInit, tSelCurEnd );

			selCurInit( tSelCurInit );
			selCurEnd( tSelCurEnd );

			mControlFlags &= ~UI_CTRL_FLAG_SELECTING;
		}
	}

	return UIComplexControl::onMouseDoubleClick( Pos, Flags );
}

Uint32 UITextBox::onMouseClick( const Vector2i& Pos, const Uint32 Flags ) {
	if ( isTextSelectionEnabled() && ( Flags & EE_BUTTON_LMASK ) ) {
		if ( selCurInit() == selCurEnd() ) {
			selCurInit( -1 );
			selCurEnd( -1 );
		}

		mControlFlags &= ~UI_CTRL_FLAG_SELECTING;
	}

	return 1;
}

Uint32 UITextBox::onMouseDown( const Vector2i& Pos, const Uint32 Flags ) {
	if ( isTextSelectionEnabled() && ( Flags & EE_BUTTON_LMASK ) ) {
		Vector2i controlPos( Pos );
		worldToControl( controlPos );

		Int32 curPos = mTextCache->font()->findClosestCursorPosFromPoint( mTextCache->text(), controlPos );

		if ( -1 != curPos ) {
			if ( -1 == selCurInit() || !( mControlFlags & UI_CTRL_FLAG_SELECTING ) ) {
				selCurInit( curPos );
				selCurEnd( curPos );
			} else {
				selCurEnd( curPos );
			}
		}

		mControlFlags |= UI_CTRL_FLAG_SELECTING;
	}

	return UIComplexControl::onMouseDown( Pos, Flags );
}

void UITextBox::drawSelection() {
	if ( selCurInit() != selCurEnd() ) {
		Int32 init		= eemin( selCurInit(), selCurEnd() );
		Int32 end		= eemax( selCurInit(), selCurEnd() );

		if ( init < 0 && end > (Int32)mTextCache->text().size() ) {
			return;
		}

		Int32 lastEnd;
		Vector2i initPos, endPos;

		Primitives P;
		P.setColor( mFontSelectionBackColor );

		do {
			initPos	= mTextCache->font()->getCursorPos( mTextCache->text(), init );
			lastEnd = mTextCache->text().find_first_of( '\n', init );

			if ( lastEnd < end && -1 != lastEnd ) {
				endPos	= mTextCache->font()->getCursorPos( mTextCache->text(), lastEnd );
				init	= lastEnd + 1;
			} else {
				endPos	= mTextCache->font()->getCursorPos( mTextCache->text(), end );
				lastEnd = end;
			}

			P.drawRectangle( Rectf( mScreenPos.x + initPos.x + mAlignOffset.x + mPadding.Left,
									  mScreenPos.y + initPos.y - mTextCache->font()->getFontHeight() + mAlignOffset.y + mPadding.Top,
									  mScreenPos.x + endPos.x + mAlignOffset.x + mPadding.Left,
									  mScreenPos.y + endPos.y + mAlignOffset.y + mPadding.Top )
			);
		} while ( end != lastEnd );
	}
}

bool UITextBox::isTextSelectionEnabled() const {
	return 0 != ( mFlags & UI_TEXT_SELECTION_ENABLED );
}

void UITextBox::selCurInit( const Int32& init ) {
	mSelCurInit = init;
}

void UITextBox::selCurEnd( const Int32& end ) {
	mSelCurEnd = end;
}

Int32 UITextBox::selCurInit() {
	return mSelCurInit;
}

Int32 UITextBox::selCurEnd() {
	return mSelCurEnd;
}

}}