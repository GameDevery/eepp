#ifndef EE_UICUISKIN_HPP
#define EE_UICUISKIN_HPP

#include <eepp/ui/base.hpp>
#include <eepp/ui/uiskinstate.hpp>

namespace EE { namespace Graphics {
class SubTexture;
}}

namespace EE { namespace UI {

class UITheme;

class EE_API UISkin {
	public:
		enum UISkinType {
			SkinSimple,
			SkinComplex,
			SkinTypeCount
		};

		static const char * getSkinStateName( const Uint32& State );

		UISkin( const std::string& getName, const Uint32& Type );

		virtual ~UISkin();

		virtual void draw( const Float& X, const Float& Y, const Float& Width, const Float& Height, const Uint32& Alpha, const Uint32& State ) = 0;

		virtual void setSkin( const Uint32& State ) = 0;

		virtual SubTexture * getSubTexture( const Uint32& State ) const = 0;

		virtual void setColor( const Uint32& State, const ColorA& Color );

		virtual const ColorA& getColor( const Uint32& State ) const;

		virtual void setSkins();

		const std::string& getName() const;

		void setName( const std::string& name );

		const Uint32& getId() const;

		UITheme * theme() const;

		void theme( UITheme * theme );

		virtual UISkin * copy() = 0;

		const Uint32& getType() const;
	protected:
		friend class UIControl;
		friend class UISkinState;

		Uint32		mType;
		std::string mName;
		Uint32		mNameHash;
		Uint32		mColorDefault;
		ColorA 	mColor[ UISkinState::StateCount ];
		UITheme * 	mTheme;

		void stateBack( const Uint32& State );

		void setPrevState();

		bool getColorDefault( const Uint32& State );

		virtual void stateNormalToState( const Uint32& State ) = 0;
};

}}

#endif