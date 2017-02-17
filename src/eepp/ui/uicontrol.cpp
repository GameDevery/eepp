#include <eepp/ui/uicontrol.hpp>
#include <eepp/ui/uitheme.hpp>
#include <eepp/ui/uimanager.hpp>
#include <eepp/graphics/primitives.hpp>
#include <eepp/graphics/subtexture.hpp>
#include <eepp/graphics/renderer/gl.hpp>

namespace EE { namespace UI {

UIControl::UIControl( const CreateParams& Params ) :
	mPos( Params.Pos ),
	mSize( Params.Size ),
	mFlags( Params.Flags ),
	mData( 0 ),
	mParentCtrl( Params.ParentCtrl ),
	mChild( NULL ),
	mChildLast( NULL ),
	mNext( NULL ),
	mPrev( NULL ),
	mSkinState( NULL ),
	mBackground( NULL ),
	mBorder( NULL ),
	mControlFlags( 0 ),
	mBlend( Params.Blend ),
	mNumCallBacks( 0 ),
	mVisible( false ),
	mEnabled( false )
{
	if ( NULL == mParentCtrl && NULL != UIManager::instance()->mainControl() ) {
		mParentCtrl = UIManager::instance()->mainControl();
	}

	if ( NULL != mParentCtrl )
		mParentCtrl->childAdd( this );

	if ( mFlags & UI_FILL_BACKGROUND )
		mBackground = eeNew( UIBackground, ( Params.Background ) );

	if ( mFlags & UI_BORDER )
		mBorder = eeNew( UIBorder, ( Params.Border ) );

	updateScreenPos();
	updateQuad();
}

UIControl::~UIControl() {
	safeDeleteSkinState();
	eeSAFE_DELETE( mBackground );
	eeSAFE_DELETE( mBorder );

	childDeleteAll();

	if ( NULL != mParentCtrl )
		mParentCtrl->childRemove( this );

	if ( UIManager::instance()->focusControl() == this && UIManager::instance()->mainControl() != this ) {
		UIManager::instance()->focusControl( UIManager::instance()->mainControl() );
	}

	if ( UIManager::instance()->overControl() == this && UIManager::instance()->mainControl() != this ) {
		UIManager::instance()->overControl( UIManager::instance()->mainControl() );
	}
}

void UIControl::screenToControl( Vector2i& Pos ) const {
	UIControl * ParentLoop = mParentCtrl;

	Pos.x -= mPos.x;
	Pos.y -= mPos.y;

	while ( NULL != ParentLoop ) {
		const Vector2i& ParentPos = ParentLoop->position();

		Pos.x -= ParentPos.x;
		Pos.y -= ParentPos.y;

		ParentLoop = ParentLoop->parent();
	}
}

void UIControl::controlToScreen( Vector2i& Pos ) const {
	UIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		const Vector2i& ParentPos = ParentLoop->position();

		Pos.x += ParentPos.x;
		Pos.y += ParentPos.y;

		ParentLoop = ParentLoop->parent();
	}
}

Uint32 UIControl::getType() const {
	return UI_TYPE_CONTROL;
}

bool UIControl::isType( const Uint32& type ) const {
	return UIControl::getType() == type;
}

void UIControl::messagePost( const UIMessage * Msg ) {
	UIControl * Ctrl = this;

	while( NULL != Ctrl ) {
		if ( Ctrl->onMessage( Msg ) )
			break;

		Ctrl = Ctrl->parent();
	}
}

Uint32 UIControl::onMessage( const UIMessage * Msg ) {
	return 0;
}

bool UIControl::isInside( const Vector2i& Pos ) const {
	return ( Pos.x >= 0 && Pos.y >= 0 && Pos.x < mSize.width() && Pos.y < mSize.height() );
}

void UIControl::position( const Vector2i& Pos ) {
	mPos = Pos;

	onPositionChange();
}

void UIControl::position( const Int32& x, const Int32& y ) {
	mPos = Vector2i( x, y );

	onPositionChange();
}

const Vector2i& UIControl::position() const {
	return mPos;
}

void UIControl::size( const Sizei& Size ) {
	if ( Size != mSize ) {
		Vector2i sizeChange( Size.x - mSize.x, Size.y - mSize.y );

		mSize = Size;

		onSizeChange();

		if ( mFlags & UI_REPORT_SIZE_CHANGE_TO_CHILDS ) {
			sendParentSizeChange( sizeChange );
		}
	}
}

void UIControl::size( const Int32& Width, const Int32& Height ) {
	size( Sizei( Width, Height ) );
}

Recti UIControl::getRect() const {
	return Recti( mPos, mSize );
}

const Sizei& UIControl::size() {
	return mSize;
}

void UIControl::visible( const bool& visible ) {
	mVisible = visible;
	onVisibleChange();
}

bool UIControl::visible() const {
	return mVisible;
}

bool UIControl::isHided() const {
	return !mVisible;
}

void UIControl::enabled( const bool& enabled ) {
	mEnabled = enabled;
	onEnabledChange();
}

bool UIControl::enabled() const {
	return mEnabled;
}

bool UIControl::isDisabled() const {
	return !mEnabled;
}

UIControl * UIControl::parent() const {
	return mParentCtrl;
}

void UIControl::parent( UIControl * parent ) {
	if ( parent == mParentCtrl )
		return;

	if ( NULL != mParentCtrl )
		mParentCtrl->childRemove( this );

	mParentCtrl = parent;

	if ( NULL != mParentCtrl )
		mParentCtrl->childAdd( this );
}

bool UIControl::isParentOf( UIControl * Ctrl ) {
	eeASSERT( NULL != Ctrl );

	UIControl * tParent = Ctrl->parent();

	while ( NULL != tParent ) {
		if ( this == tParent )
			return true;

		tParent = tParent->parent();
	}

	return false;
}

void UIControl::centerHorizontal() {
	UIControl * Ctrl = parent();

	if ( NULL != Ctrl )
		position( Vector2i( ( Ctrl->size().width() / 2 ) - ( mSize.width() / 2 ), mPos.y ) );
}

void UIControl::centerVertical(){
	UIControl * Ctrl = parent();

	if ( NULL != Ctrl )
		position( Vector2i( mPos.x, ( Ctrl->size().height() / 2 ) - ( mSize.height() / 2 ) ) );
}

void UIControl::center() {
	centerHorizontal();
	centerVertical();
}

void UIControl::close() {
	mControlFlags |= UI_CTRL_FLAG_CLOSE;

	UIManager::instance()->addToCloseQueue( this );
}

void UIControl::draw() {
	if ( mVisible ) {
		if ( mFlags & UI_FILL_BACKGROUND )
			backgroundDraw();

		if ( mFlags & UI_BORDER )
			borderDraw();

		if ( NULL != mSkinState )
			mSkinState->draw( mScreenPosf.x, mScreenPosf.y, (Float)mSize.width(), (Float)mSize.height(), 255 );

		if ( UIManager::instance()->highlightFocus() && UIManager::instance()->focusControl() == this ) {
			Primitives P;
			P.fillMode( DRAW_LINE );
			P.blendMode( blend() );
			P.setColor( UIManager::instance()->highlightFocusColor() );
			P.drawRectangle( getRectf() );
		}

		if ( UIManager::instance()->highlightOver() && UIManager::instance()->overControl() == this ) {
			Primitives P;
			P.fillMode( DRAW_LINE );
			P.blendMode( blend() );
			P.setColor( UIManager::instance()->highlightOverColor() );
			P.drawRectangle( getRectf() );
		}
	}
}

void UIControl::update() {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->update();
		ChildLoop = ChildLoop->mNext;
	}

	if ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD )
		writeCtrlFlag( UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD, 0 );
}

void UIControl::sendMouseEvent( const Uint32& Event, const Vector2i& Pos, const Uint32& Flags ) {
	UIEventMouse MouseEvent( this, Event, Pos, Flags );
	sendEvent( &MouseEvent );
}

void UIControl::sendCommonEvent( const Uint32& Event ) {
	UIEvent CommonEvent( this, Event );
	sendEvent( &CommonEvent );
}

Uint32 UIControl::onKeyDown( const UIEventKey& Event ) {
	sendEvent( &Event );
	return 0;
}

Uint32 UIControl::onKeyUp( const UIEventKey& Event ) {
	sendEvent( &Event );
	return 0;
}

Uint32 UIControl::onMouseMove( const Vector2i& Pos, const Uint32 Flags ) {
	sendMouseEvent( UIEvent::EventMouseMove, Pos, Flags );
	return 1;
}

Uint32 UIControl::onMouseDown( const Vector2i& Pos, const Uint32 Flags ) {
	sendMouseEvent( UIEvent::EventMouseDown, Pos, Flags );

	setSkinState( UISkinState::StateMouseDown );

	return 1;
}

Uint32 UIControl::onMouseUp( const Vector2i& Pos, const Uint32 Flags ) {
	sendMouseEvent( UIEvent::EventMouseUp, Pos, Flags );

	setPrevSkinState();

	return 1;
}

Uint32 UIControl::onMouseClick( const Vector2i& Pos, const Uint32 Flags ) {
	sendMouseEvent( UIEvent::EventMouseClick, Pos, Flags );
	return 1;
}

bool UIControl::isMouseOver() {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER );
}

bool UIControl::isMouseOverMeOrChilds() {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD );
}

Uint32 UIControl::onMouseDoubleClick( const Vector2i& Pos, const Uint32 Flags ) {
	sendMouseEvent( UIEvent::EventMouseDoubleClick, Pos, Flags );
	return 1;
}

Uint32 UIControl::onMouseEnter( const Vector2i& Pos, const Uint32 Flags ) {
	writeCtrlFlag( UI_CTRL_FLAG_MOUSEOVER, 1 );

	sendMouseEvent( UIEvent::EventMouseEnter, Pos, Flags );

	setSkinState( UISkinState::StateMouseEnter );

	return 1;
}

Uint32 UIControl::onMouseExit( const Vector2i& Pos, const Uint32 Flags ) {
	writeCtrlFlag( UI_CTRL_FLAG_MOUSEOVER, 0 );

	sendMouseEvent( UIEvent::EventMouseExit, Pos, Flags );

	setSkinState( UISkinState::StateMouseExit );

	return 1;
}

Uint32 UIControl::onFocus() {
	mControlFlags |= UI_CTRL_FLAG_HAS_FOCUS;

	sendCommonEvent( UIEvent::EventOnFocus );

	setSkinState( UISkinState::StateFocus );

	return 1;
}

Uint32 UIControl::onFocusLoss() {
	mControlFlags &= ~UI_CTRL_FLAG_HAS_FOCUS;

	sendCommonEvent( UIEvent::EventOnFocusLoss );

	return 1;
}

void UIControl::onComplexControlFocusLoss() {
	sendCommonEvent( UIEvent::EventOnComplexControlFocusLoss );
}

bool UIControl::hasFocus() const {
	return 0 != ( mControlFlags & UI_CTRL_FLAG_HAS_FOCUS );
}

Uint32 UIControl::onValueChange() {
	sendCommonEvent( UIEvent::EventOnValueChange );

	return 1;
}

void UIControl::onClose() {
	sendCommonEvent( UIEvent::EventOnClose );
}

Uint32 UIControl::hAlign() const {
	return mFlags & UI_HALIGN_MASK;
}

void UIControl::hAlign( Uint32 halign ) {
	mFlags |= halign & UI_HALIGN_MASK;
}

Uint32 UIControl::vAlign() const {
	return mFlags & UI_VALIGN_MASK;
}

void UIControl::vAlign( Uint32 valign ) {
	mFlags |= valign & UI_VALIGN_MASK;
}

void UIControl::fillBackground( bool enabled ) {
	writeFlag( UI_FILL_BACKGROUND, enabled ? 1 : 0 );

	if ( enabled && NULL == mBackground ) {
		mBackground = eeNew( UIBackground, () );
	}
}

void UIControl::border( bool enabled ) {
	writeFlag( UI_BORDER, enabled ? 1 : 0 );

	if ( enabled && NULL == mBorder ) {
		mBorder = eeNew( UIBorder, () );

		if ( NULL == mBackground ) {
			mBackground = eeNew( UIBackground, () );
		}
	}
}

UIControl * UIControl::nextGet() const {
	return mNext;
}

UIControl * UIControl::prevGet() const {
	return mPrev;
}

UIControl * UIControl::nextGetLoop() const {
	if ( NULL == mNext )
		return parent()->childGetFirst();
	else
		return mNext;
}

void UIControl::data(const UintPtr& data ) {
	mData = data;
}

const UintPtr& UIControl::data() const {
	return mData;
}

const Uint32& UIControl::flags() const {
	return mFlags;
}

void UIControl::flags( const Uint32& flags ) {
	mFlags |= flags;
}

void UIControl::blend( const EE_BLEND_MODE& blend ) {
	mBlend = static_cast<Uint16> ( blend );
}

EE_BLEND_MODE UIControl::blend() {
	return static_cast<EE_BLEND_MODE> ( mBlend );
}

void UIControl::toFront() {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->childRemove( this );
		mParentCtrl->childAdd( this );
	}
}

void UIControl::toBack() {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->childAddAt( this, 0 );
	}
}

void UIControl::toPos( const Uint32& Pos ) {
	if ( NULL != mParentCtrl ) {
		mParentCtrl->childAddAt( this, Pos );
	}
}

void UIControl::onVisibleChange() {
	sendCommonEvent( UIEvent::EventOnVisibleChange );
}

void UIControl::onEnabledChange() {
	if ( !enabled() && NULL != UIManager::instance()->focusControl() ) {
		if ( isChild( UIManager::instance()->focusControl() ) ) {
			UIManager::instance()->focusControl( NULL );
		}
	}

	sendCommonEvent( UIEvent::EventOnEnabledChange );
}

void UIControl::onPositionChange() {
	updateScreenPos();

	updateChildsScreenPos();

	sendCommonEvent( UIEvent::EventOnPosChange );
}

void UIControl::onSizeChange() {
	updateCenter();

	sendCommonEvent( UIEvent::EventOnSizeChange );
}

Rectf UIControl::getRectf() {
	return Rectf( mScreenPosf, Sizef( (Float)mSize.width(), (Float)mSize.height() ) );
}

void UIControl::backgroundDraw() {
	Primitives P;
	Rectf R = getRectf();
	P.blendMode( mBackground->blend() );
	P.setColor( mBackground->color() );

	if ( 4 == mBackground->colors().size() ) {
		if ( mBackground->corners() ) {
			P.drawRoundedRectangle( R, mBackground->colors()[0], mBackground->colors()[1], mBackground->colors()[2], mBackground->colors()[3], mBackground->corners() );
		} else {
			P.drawRectangle( R, mBackground->colors()[0], mBackground->colors()[1], mBackground->colors()[2], mBackground->colors()[3] );
		}
	} else {
		if ( mBackground->corners() ) {
			P.drawRoundedRectangle( R, 0.f, Vector2f::One, mBackground->corners() );
		} else {
			P.drawRectangle( R );
		}
	}
}

void UIControl::borderDraw() {
	Primitives P;
	P.fillMode( DRAW_LINE );
	P.blendMode( blend() );
	P.lineWidth( (Float)mBorder->width() );
	P.setColor( mBorder->color() );

	//! @TODO: Check why was this +0.1f -0.1f?
	if ( mFlags & UI_CLIP_ENABLE ) {
		Rectf R( Vector2f( mScreenPosf.x + 0.1f, mScreenPosf.y + 0.1f ), Sizef( (Float)mSize.width() - 0.1f, (Float)mSize.height() - 0.1f ) );

		if ( mBackground->corners() ) {
			P.drawRoundedRectangle( getRectf(), 0.f, Vector2f::One, mBackground->corners() );
		} else {
			P.drawRectangle( R );
		}
	} else {
		if ( mBackground->corners() ) {
			P.drawRoundedRectangle( getRectf(), 0.f, Vector2f::One, mBackground->corners() );
		} else {
			P.drawRectangle( getRectf() );
		}
	}
}

const Uint32& UIControl::controlFlags() const {
	return mControlFlags;
}

void UIControl::controlFlags( const Uint32& Flags ) {
	mControlFlags = Flags;
}

void UIControl::drawChilds() {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		if ( ChildLoop->mVisible ) {
			ChildLoop->internalDraw();
		}

		ChildLoop = ChildLoop->mNext;
	}
}

void UIControl::internalDraw() {
	if ( mVisible ) {
		matrixSet();

		clipMe();

		draw();

		drawChilds();

		clipDisable();

		matrixUnset();
	}
}

void UIControl::clipMe() {
	if ( mFlags & UI_CLIP_ENABLE ) {
		if ( mFlags & UI_BORDER )
			UIManager::instance()->clipEnable( mScreenPos.x, mScreenPos.y, mSize.width(), mSize.height() + 1 );
		else
			UIManager::instance()->clipEnable( mScreenPos.x, mScreenPos.y, mSize.width(), mSize.height() );
	}
}

void UIControl::clipDisable() {
	if ( mFlags & UI_CLIP_ENABLE )
		UIManager::instance()->clipDisable();
}

void UIControl::matrixSet() {
}

void UIControl::matrixUnset() {
}

void UIControl::childDeleteAll() {
	while( NULL != mChild ) {
		eeDelete( mChild );
	}
}

void UIControl::childAdd( UIControl * ChildCtrl ) {
	if ( NULL == mChild ) {
		mChild 		= ChildCtrl;
		mChildLast 	= ChildCtrl;
	} else {
		mChildLast->mNext 		= ChildCtrl;
		ChildCtrl->mPrev		= mChildLast;
		ChildCtrl->mNext		= NULL;
		mChildLast 				= ChildCtrl;
	}
}

void UIControl::childAddAt( UIControl * ChildCtrl, Uint32 Pos ) {
	eeASSERT( NULL != ChildCtrl );

	UIControl * ChildLoop = mChild;
	
	ChildCtrl->parent( this );

	childRemove( ChildCtrl );
	ChildCtrl->mParentCtrl = this;
	
	if ( ChildLoop == NULL ) {
		mChild 				= ChildCtrl;
		mChildLast			= ChildCtrl;
		ChildCtrl->mNext 	= NULL;
		ChildCtrl->mPrev 	= NULL;
	} else {
		if( Pos == 0 ) {
			if ( NULL != mChild ) {
				mChild->mPrev		= ChildCtrl;
			}

			ChildCtrl->mNext 	= mChild;
			ChildCtrl->mPrev	= NULL;
			mChild 				= ChildCtrl;
		} else {
			Uint32 i = 0;

			while ( NULL != ChildLoop->mNext && i < Pos ) {
				ChildLoop = ChildLoop->mNext;
				i++;
			}

			UIControl * ChildTmp = ChildLoop->mNext;
			ChildLoop->mNext 	= ChildCtrl;
			ChildCtrl->mPrev 	= ChildLoop;
			ChildCtrl->mNext 	= ChildTmp;

			if ( NULL != ChildTmp ) {
				ChildTmp->mPrev = ChildCtrl;
			} else {
				mChildLast		= ChildCtrl;
			}
		}
	}
}

void UIControl::childRemove( UIControl * ChildCtrl ) {
	if ( ChildCtrl == mChild ) {
		mChild 			= mChild->mNext;

		if ( NULL != mChild ) {
			mChild->mPrev 	= NULL;

			if ( ChildCtrl == mChildLast )
				mChildLast		= mChild;
		} else {
			mChildLast		= NULL;
		}
	} else {
		if ( mChildLast == ChildCtrl )
			mChildLast = mChildLast->mPrev;

		ChildCtrl->mPrev->mNext = ChildCtrl->mNext;

		if ( NULL != ChildCtrl->mNext ) {
			ChildCtrl->mNext->mPrev = ChildCtrl->mPrev;
			ChildCtrl->mNext = NULL;
		}

		ChildCtrl->mPrev = NULL;
	}
}

void UIControl::childsCloseAll() {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->close();
		ChildLoop = ChildLoop->mNext;
	}
}

bool UIControl::isChild( UIControl * ChildCtrl ) const {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		if ( ChildCtrl == ChildLoop )
			return true;

		ChildLoop = ChildLoop->mNext;
	}

	return false;
}

bool UIControl::inParentTreeOf( UIControl * Child ) const {
	UIControl * ParentLoop = Child->mParentCtrl;

	while ( NULL != ParentLoop ) {
		if ( ParentLoop == this )
			return true;

		ParentLoop = ParentLoop->mParentCtrl;
	}

	return false;
}

Uint32 UIControl::childCount() const {
	UIControl * ChildLoop = mChild;
	Uint32 Count = 0;

	while( NULL != ChildLoop ) {
		ChildLoop = ChildLoop->mNext;
		Count++;
	}

	return Count;
}

UIControl * UIControl::childAt( Uint32 Index ) const {
	UIControl * ChildLoop = mChild;

	while( NULL != ChildLoop && Index ) {
		ChildLoop = ChildLoop->mNext;
		Index--;
	}

	return ChildLoop;
}

UIControl * UIControl::childPrev( UIControl * Ctrl, bool Loop ) const {
	if ( Loop && Ctrl == mChild && NULL != mChild->mNext )
		return mChildLast;

	return Ctrl->mPrev;
}

UIControl * UIControl::childNext( UIControl * Ctrl, bool Loop ) const {
	if ( NULL == Ctrl->mNext && Loop )
		return mChild;

	return Ctrl->mNext;
}

UIControl * UIControl::childGetFirst() const {
	return mChild;
}

UIControl * UIControl::childGetLast() const {
	return mChildLast;
}

UIControl * UIControl::overFind( const Vector2f& Point ) {
	UIControl * pOver = NULL;

	if ( mEnabled && mVisible ) {
		updateQuad();

		if ( mPoly.pointInside( Point ) ) {
			writeCtrlFlag( UI_CTRL_FLAG_MOUSEOVER_ME_OR_CHILD, 1 );

			UIControl * ChildLoop = mChildLast;

			while ( NULL != ChildLoop ) {
				UIControl * ChildOver = ChildLoop->overFind( Point );

				if ( NULL != ChildOver ) {
					pOver = ChildOver;

					break; // Search from top to bottom, so the first over will be the topmost
				}

				ChildLoop = ChildLoop->mPrev;
			}


			if ( NULL == pOver )
				pOver = this;
		}
	}

	return pOver;
}

UIControl * UIControl::childGetAt( Vector2i CtrlPos, unsigned int RecursiveLevel ) {
	UIControl * Ctrl = NULL;

	for( UIControl * pLoop = mChild; NULL != pLoop && NULL == Ctrl; pLoop = pLoop->mNext )
	{
		if ( !pLoop->visible() )
			continue;

		if ( pLoop->getRect().contains( CtrlPos ) ) {
			if ( RecursiveLevel )
				Ctrl = childGetAt( CtrlPos - pLoop->position(), RecursiveLevel - 1 );

			if ( NULL == Ctrl )
				Ctrl = pLoop;
		}
	}

	return Ctrl;
}

Uint32 UIControl::isAnimated() {
	return mControlFlags & UI_CTRL_FLAG_ANIM;
}

Uint32 UIControl::isDragable() {
	return mControlFlags & UI_CTRL_FLAG_DRAGABLE;
}

Uint32 UIControl::isComplex() {
	return mControlFlags & UI_CTRL_FLAG_COMPLEX;
}

Uint32 UIControl::isClipped() {
	return mFlags & UI_CLIP_ENABLE;
}

Polygon2f& UIControl::getPolygon() {
	return mPoly;
}

const Vector2f& UIControl::getPolygonCenter() const {
	return mCenter;
}

void UIControl::updateQuad() {
	mPoly 	= Polygon2f( eeAABB( mScreenPosf.x, mScreenPosf.y, mScreenPosf.x + mSize.width(), mScreenPosf.y + mSize.height() ) );

	UIControl * tParent = parent();

	while ( tParent ) {
		if ( tParent->isAnimated() ) {
			UIControlAnim * tP = reinterpret_cast<UIControlAnim *> ( tParent );

			mPoly.rotate( tP->angle(), tP->rotationCenter() );
			mPoly.scale( tP->scale(), tP->scaleCenter() );
		}

		tParent = tParent->parent();
	};
}

void UIControl::updateCenter() {
	mCenter = Vector2f( mScreenPosf.x + (Float)mSize.width() * 0.5f, mScreenPosf.y + (Float)mSize.height() * 0.5f );
}

Time UIControl::elapsed() {
	return UIManager::instance()->elapsed();
}

Uint32 UIControl::addEventListener( const Uint32& EventType, const UIEventCallback& Callback ) {
	mNumCallBacks++;

	mEvents[ EventType ][ mNumCallBacks ] = Callback;

	return mNumCallBacks;
}

void UIControl::removeEventListener( const Uint32& CallbackId ) {
	UIEventsMap::iterator it;

	for ( it = mEvents.begin(); it != mEvents.end(); ++it ) {
		std::map<Uint32, UIEventCallback> event = it->second;

		if ( event.erase( CallbackId ) )
			break;
	}
}

void UIControl::sendEvent( const UIEvent * Event ) {
	if ( 0 != mEvents.count( Event->getEventType() ) ) {
		std::map<Uint32, UIEventCallback>			event = mEvents[ Event->getEventType() ];
		std::map<Uint32, UIEventCallback>::iterator it;

		if ( event.begin() != event.end() ) {
			for ( it = event.begin(); it != event.end(); ++it )
				it->second( Event );
		}
	}
}

UIBackground * UIControl::background() {
	return mBackground;
}

UIBorder * UIControl::border() {
	return mBorder;
}

void UIControl::setThemeByName( const std::string& Theme ) {
	setTheme( UIThemeManager::instance()->getByName( Theme ) );
}

void UIControl::setTheme( UITheme * Theme ) {
	setThemeControl( Theme, "control" );
}

void UIControl::safeDeleteSkinState() {
	if ( NULL != mSkinState && ( mControlFlags & UI_CTRL_FLAG_SKIN_OWNER ) ) {
		UISkin * tSkin = mSkinState->getSkin();

		eeSAFE_DELETE( tSkin );
	}

	eeSAFE_DELETE( mSkinState );
}

void UIControl::setThemeControl( UITheme * Theme, const std::string& ControlName ) {
	if ( NULL != Theme ) {
		UISkin * tSkin = Theme->getByName( Theme->abbr() + "_" + ControlName );

		if ( NULL != tSkin ) {
			Uint32 InitialState = UISkinState::StateNormal;

			if ( NULL != mSkinState ) {
				InitialState = mSkinState->getState();
			}

			safeDeleteSkinState();

			mSkinState = eeNew( UISkinState, ( tSkin ) );
			mSkinState->setState( InitialState );
		}
	}
}

void UIControl::setSkinFromTheme( UITheme * Theme, const std::string& ControlName ) {
	setThemeControl( Theme, ControlName );
}

void UIControl::setSkin( const UISkin& Skin ) {
	safeDeleteSkinState();

	writeCtrlFlag( UI_CTRL_FLAG_SKIN_OWNER, 1 );

	UISkin * SkinCopy = const_cast<UISkin*>( &Skin )->copy();

	mSkinState = eeNew( UISkinState, ( SkinCopy ) );

	doAftersetTheme();
}

void UIControl::onStateChange() {
}

void UIControl::setSkinState( const Uint32& State ) {
	if ( NULL != mSkinState ) {
		mSkinState->setState( State );

		onStateChange();
	}
}

void UIControl::setPrevSkinState() {
	if ( NULL != mSkinState ) {
		mSkinState->setPrevState();

		onStateChange();
	}
}

void UIControl::setThemeToChilds( UITheme * Theme ) {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->setThemeToChilds( Theme );
		ChildLoop->setTheme( Theme );	// First set the theme to childs to let the father override the childs forced themes

		ChildLoop = ChildLoop->mNext;
	}
}

void UIControl::updateChildsScreenPos() {
	UIControl * ChildLoop = mChild;

	while ( NULL != ChildLoop ) {
		ChildLoop->updateScreenPos();
		ChildLoop->updateChildsScreenPos();

		ChildLoop = ChildLoop->mNext;
	}
}

void UIControl::updateScreenPos() {
	Vector2i Pos( mPos );

	controlToScreen( Pos );

	mScreenPos = Pos;
	mScreenPosf = Vector2f( Pos.x, Pos.y );

	updateCenter();
}

UISkin * UIControl::getSkin() {
	if ( NULL != mSkinState )
		return mSkinState->getSkin();

	return NULL;
}

void UIControl::writeCtrlFlag( const Uint32& Flag, const Uint32& Val ) {
	BitOp::setBitFlagValue( &mControlFlags, Flag, Val );
}

void UIControl::writeFlag( const Uint32& Flag, const Uint32& Val ) {
	if ( Val )
		mFlags |= Flag;
	else {
		if ( mFlags & Flag )
			mFlags &= ~Flag;
	}
}

void UIControl::applyDefaultTheme() {
	UIThemeManager::instance()->applyDefaultTheme( this );
}

Recti UIControl::getScreenRect() {
	return Recti( mScreenPos, mSize );
}

Recti UIControl::makePadding( bool PadLeft, bool PadRight, bool PadTop, bool PadBottom, bool SkipFlags ) {
	Recti tPadding( 0, 0, 0, 0 );

	if ( mFlags & UI_AUTO_PADDING || SkipFlags ) {
		if ( NULL != mSkinState && NULL != mSkinState->getSkin() ) {
			if ( mSkinState->getSkin()->getType() == UISkin::SkinComplex ) {
				UISkinComplex * tComplex = reinterpret_cast<UISkinComplex*> ( mSkinState->getSkin() );

				SubTexture * tSubTexture = NULL;

				if ( PadLeft ) {
					tSubTexture = tComplex->getSubTextureSide( UISkinState::StateNormal, UISkinComplex::Left );

					if ( NULL != tSubTexture )
						tPadding.Left = tSubTexture->realSize().width();
				}

				if ( PadRight ) {
					tSubTexture = tComplex->getSubTextureSide( UISkinState::StateNormal, UISkinComplex::Right );

					if ( NULL != tSubTexture )
						tPadding.Right = tSubTexture->realSize().width();
				}

				if ( PadTop ) {
					tSubTexture = tComplex->getSubTextureSide( UISkinState::StateNormal, UISkinComplex::Up );

					if ( NULL != tSubTexture )
						tPadding.Top = tSubTexture->realSize().height();
				}

				if ( PadBottom ) {
					tSubTexture = tComplex->getSubTextureSide( UISkinState::StateNormal, UISkinComplex::Down );

					if ( NULL != tSubTexture )
						tPadding.Bottom = tSubTexture->realSize().height();
				}
			}
		}
	}

	return tPadding;
}

void UIControl::setFocus() {
	UIManager::instance()->focusControl( this );
}

void UIControl::sendParentSizeChange( const Vector2i& SizeChange ) {
	if ( mFlags & UI_REPORT_SIZE_CHANGE_TO_CHILDS )	{
		UIControl * ChildLoop = mChild;

		while( NULL != ChildLoop ) {
			ChildLoop->onParentSizeChange( SizeChange );
			ChildLoop = ChildLoop->mNext;
		}
	}
}

void UIControl::onParentSizeChange( const Vector2i& SizeChange ) {
	sendCommonEvent( UIEvent::EventOnParentSizeChange );
}

Sizei UIControl::getSkinSize( UISkin * Skin, const Uint32& State ) {
	Sizei		tSize;

	if ( NULL != Skin ) {
		SubTexture * tSubTexture = Skin->getSubTexture( State );

		if ( NULL != tSubTexture ) {
			tSize = tSubTexture->realSize();
		}

		if ( Skin->getType() == UISkin::SkinComplex ) {
			UISkinComplex * SkinC = reinterpret_cast<UISkinComplex*> ( Skin );

			tSubTexture = SkinC->getSubTextureSide( State, UISkinComplex::Up );

			if ( NULL != tSubTexture ) {
				tSize.y += tSubTexture->realSize().height();
			}

			tSubTexture = SkinC->getSubTextureSide( State, UISkinComplex::Down );

			if ( NULL != tSubTexture ) {
				tSize.y += tSubTexture->realSize().height();
			}

			tSubTexture = SkinC->getSubTextureSide( State, UISkinComplex::Left );

			if ( NULL != tSubTexture ) {
				tSize.x += tSubTexture->realSize().width();
			}

			tSubTexture = SkinC->getSubTextureSide( State, UISkinComplex::Right );

			if ( NULL != tSubTexture ) {
				tSize.x += tSubTexture->realSize().width();
			}
		}
	}

	return tSize;
}

Sizei UIControl::getSkinSize() {
	return getSkinSize( getSkin(), UISkinState::StateNormal );
}

UIControl * UIControl::nextComplexControl() {
	UIControl * Found		= NULL;
	UIControl * ChildLoop	= mChild;

	while( NULL != ChildLoop ) {
		if ( ChildLoop->visible() && ChildLoop->enabled() ) {
			if ( ChildLoop->isComplex() ) {
				return ChildLoop;
			} else {
				Found = ChildLoop->nextComplexControl();

				if ( NULL != Found ) {
					return Found;
				}
			}
		}

		ChildLoop = ChildLoop->mNext;
	}

	if ( NULL != mNext ) {
		if ( mNext->visible() && mNext->enabled() && mNext->isComplex() ) {
			return mNext;
		} else {
			return mNext->nextComplexControl();
		}
	} else {
		ChildLoop = mParentCtrl;

		while ( NULL != ChildLoop ) {
			if ( NULL != ChildLoop->mNext ) {
				if ( ChildLoop->mNext->visible() && ChildLoop->mNext->enabled() && ChildLoop->mNext->isComplex() ) {
					return ChildLoop->mNext;
				} else {
					return ChildLoop->mNext->nextComplexControl();
				}
			}

			ChildLoop = ChildLoop->mParentCtrl;
		}
	}

	return UIManager::instance()->mainControl();
}

void UIControl::doAftersetTheme() {
}

void UIControl::worldToControl( Vector2i& pos ) const {
	Vector2f Pos( pos.x, pos.y );

	std::list<UIControl*> parents;

	UIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		parents.push_front( ParentLoop );
		ParentLoop = ParentLoop->parent();
	}

	parents.push_back( const_cast<UIControl*>( reinterpret_cast<const UIControl*>( this ) ) );

	Vector2f scale(1,1);

	for ( std::list<UIControl*>::iterator it = parents.begin(); it != parents.end(); it++ ) {
		UIControl * tParent	= (*it);
		UIControlAnim * tP		= tParent->isAnimated() ? reinterpret_cast<UIControlAnim *> ( tParent ) : NULL;
		Vector2f pPos			( tParent->mPos.x * scale.x			, tParent->mPos.y * scale.y			);
		Vector2f Center;

		if ( NULL != tP && 1.f != tP->scale() ) {
			Center = tP->scaleOriginPoint() * scale;
			scale *= tP->scale();

			pPos.scale( scale, pPos + Center );
		}

		Pos -= pPos;

		if ( NULL != tP && 0.f != tP->angle() ) {
			Center = tP->rotationOriginPoint() * scale;
			Pos.rotate( -tP->angle(), Center );
		}
	}

	pos = Vector2i( Pos.x / scale.x, Pos.y / scale.y );
}

void UIControl::controlToWorld( Vector2i& pos ) const {
	Vector2f Pos( pos.x, pos.y );

	std::list<UIControl*> parents;

	UIControl * ParentLoop = mParentCtrl;

	while ( NULL != ParentLoop ) {
		parents.push_back( ParentLoop );
		ParentLoop = ParentLoop->parent();
	}

	parents.push_front( const_cast<UIControl*>( reinterpret_cast<const UIControl*>( this ) ) );

	for ( std::list<UIControl*>::iterator it = parents.begin(); it != parents.end(); it++ ) {
		UIControl * tParent	= (*it);
		UIControlAnim * tP		= tParent->isAnimated() ? reinterpret_cast<UIControlAnim *> ( tParent ) : NULL;
		Vector2f pPos			( tParent->mPos.x					, tParent->mPos.y					);

		Pos += pPos;

		if ( NULL != tP ) {
			Vector2f CenterAngle( pPos.x + tP->mRotationOriginPoint.x, pPos.y + tP->mRotationOriginPoint.y );
			Vector2f CenterScale( pPos.x + tP->mScaleOriginPoint.x, pPos.y + tP->mScaleOriginPoint.y );

			Pos.rotate( tP->angle(), CenterAngle );
			Pos.scale( tP->scale(), CenterScale );
		}
	}

	pos = Vector2i( eeceil( Pos.x ), eeceil( Pos.y ) );
}

}}