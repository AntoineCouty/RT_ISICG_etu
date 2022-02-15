#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{

	class PointLight : public BaseLight
	{
	  public:
		virtual ~PointLight() = default;

		PointLight( const Vec3f & p_color, float p_power, const Vec3f & p_position ): BaseLight( p_color, p_power ), _position( p_position ){}

		inline const Vec3f & getPosition() const { return _position; }

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_POINT_LIGHT__