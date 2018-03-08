#ifndef EE_SCENE_ACTION_MOVE_HPP
#define EE_SCENE_ACTION_MOVE_HPP

#include <eepp/scene/action.hpp>
#include <eepp/scene/actions/actioninterpolation2d.hpp>

namespace EE { namespace Scene { namespace Actions {

class EE_API Move : public ActionInterpolation2d {
	public:
		static Move * New( const Vector2f& start, const Vector2f& end, const Time& duration, const Ease::Interpolation& type = Ease::Linear );

		Action * clone() const override;

		Action * reverse() const override;
	protected:
		Move( const Vector2f& start, const Vector2f& end, const Time& duration, const Ease::Interpolation& type );

		void onStart() override;

		void onUpdate( const Time& time ) override;
	private:
		Move();
};

}}}

#endif
