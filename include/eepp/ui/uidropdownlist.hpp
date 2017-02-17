#ifndef EE_UICUIDROPDOWNLIST_HPP
#define EE_UICUIDROPDOWNLIST_HPP

#include <eepp/ui/uitextinput.hpp>
#include <eepp/ui/uilistbox.hpp>

namespace EE { namespace UI {

class EE_API UIDropDownList : public UITextInput {
	public:
		class CreateParams : public UITextInput::CreateParams {
			public:
				inline CreateParams() :
					UITextInput::CreateParams(),
					ListBox( NULL ),
					MinNumVisibleItems( 6 ),
					PopUpToMainControl( false )
				{
				}

				inline ~CreateParams() {}

				UIListBox * 	ListBox;
				Uint32			MinNumVisibleItems;
				bool			PopUpToMainControl;
		};

		UIDropDownList( UIDropDownList::CreateParams& Params );

		virtual ~UIDropDownList();

		virtual Uint32 getType() const;

		virtual bool isType( const Uint32& type ) const;

		virtual void setTheme( UITheme * Theme );

		UIListBox * getListBox() const;

		virtual void update();
	protected:
		UIListBox *	mListBox;
		Uint32			mMinNumVisibleItems;
		bool			mPopUpToMainControl;

		void showListBox();

		void onListBoxFocusLoss( const UIEvent * Event );

		virtual void onItemSelected( const UIEvent * Event );

		virtual void show();

		virtual void hide();

		Uint32 onMouseClick( const Vector2i& position, const Uint32 flags );

		virtual void onItemClicked( const UIEvent * Event );

		virtual void onItemKeyDown( const UIEvent * Event );

		virtual void onControlClear( const UIEvent * Event );

		Uint32 onKeyDown( const UIEventKey &Event );

		virtual void onSizeChange();

		virtual void autoSize();

		virtual void autoSizeControl();

		void destroyListBox();
};

}}

#endif