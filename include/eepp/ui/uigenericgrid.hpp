#ifndef EE_UICUIGENERICGRID_HPP
#define EE_UICUIGENERICGRID_HPP

#include <eepp/ui/uicontrolanim.hpp>
#include <eepp/ui/uigridcell.hpp>
#include <eepp/ui/uiscrollbar.hpp>
#include <eepp/ui/uiitemcontainer.hpp>

namespace EE { namespace UI {

class EE_API UIGenericGrid : public UIComplexControl {
	public:
		class CreateParams : public UIComplexControl::CreateParams {
			public:
				inline CreateParams() :
					UIComplexControl::CreateParams(),
					SmoothScroll( true ),
					VScrollMode( UI_SCROLLBAR_AUTO ),
					HScrollMode( UI_SCROLLBAR_AUTO ),
					CollumnsCount(1),
					RowHeight( 24 ),
					GridWidth( 0 ),
					TouchDragDeceleration( 0.01f )
				{
				}

				inline ~CreateParams() {}

				bool				SmoothScroll;
				UI_SCROLLBAR_MODE	VScrollMode;
				UI_SCROLLBAR_MODE	HScrollMode;
				Uint32				CollumnsCount;
				Uint32				RowHeight;
				Uint32				GridWidth;
				Recti				PaddingContainer;
				Float				TouchDragDeceleration;
		};

		UIGenericGrid( const UIGenericGrid::CreateParams& Params );

		~UIGenericGrid();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void setTheme( UITheme * Theme );

		void add( UIGridCell * Cell );

		void remove( UIGridCell * Cell );

		void remove( std::vector<Uint32> ItemsIndex );

		void remove( Uint32 ItemIndex );

		void collumnWidth( const Uint32& CollumnIndex, const Uint32& collumnWidth );

		Uint32 getCount() const;

		const Uint32& getCollumnsCount() const;

		const Uint32& collumnWidth( const Uint32& CollumnIndex ) const;

		void rowHeight( const Uint32& height );

		const Uint32& rowHeight() const;

		UIGridCell * getCell( const Uint32& CellIndex ) const;

		void verticalScrollMode( const UI_SCROLLBAR_MODE& Mode );

		const UI_SCROLLBAR_MODE& verticalScrollMode();

		void horizontalScrollMode( const UI_SCROLLBAR_MODE& Mode );

		const UI_SCROLLBAR_MODE& horizontalScrollMode();

		Uint32 getCellPosition( const Uint32& CollumnIndex );

		UIScrollBar * verticalScrollBar() const;

		UIScrollBar * horizontalScrollBar() const;

		Uint32 getItemIndex( UIGridCell * Item );

		UIGridCell * getItemSelected();

		Uint32 getItemSelectedIndex() const;

		Uint32 onMessage( const UIMessage * Msg );

		UIItemContainer<UIGenericGrid> * getContainer() const;

		virtual void update();

		bool touchDragEnable() const;

		void touchDragEnable( const bool& enable );

		bool touchDragging() const;

		void touchDragging( const bool& dragging );
	protected:
		friend class UIItemContainer<UIGenericGrid>;
		friend class UIGridCell;

		Recti						mPadding;
		bool						mSmoothScroll;
		UIItemContainer<UIGenericGrid> *	mContainer;
		UIScrollBar *				mVScrollBar;
		UIScrollBar *				mHScrollBar;
		UI_SCROLLBAR_MODE			mVScrollMode;
		UI_SCROLLBAR_MODE			mHScrollMode;
		std::vector<UIGridCell*>	mItems;
		Uint32						mCollumnsCount;
		Uint32						mRowHeight;
		std::vector<Uint32>			mCollumnsWidth;
		std::vector<Uint32>			mCollumnsPos;
		Uint32						mTotalWidth;
		Uint32						mTotalHeight;
		Uint32						mLastPos;
		Uint32						mVisibleFirst;
		Uint32						mVisibleLast;
		Int32						mHScrollInit;
		Int32						mItemsNotVisible;
		Int32						mSelected;

		Vector2i					mTouchDragPoint;
		Float						mTouchDragAcceleration;
		Float						mTouchDragDeceleration;

		bool						mCollWidthAssigned;

		void updateCells();

		void updateCollumnsPos();

		void autoPadding();

		virtual void onSizeChange();

		virtual void onAlphaChange();

		void containerResize();

		void onScrollValueChange( const UIEvent * Event );

		void setDefaultCollumnsWidth();

		void updateScroll( bool FromScrollChange = false );

		void updateSize();

		virtual Uint32 onSelected();

		void updateVScroll();

		void updateHScroll();
};

}}

#endif