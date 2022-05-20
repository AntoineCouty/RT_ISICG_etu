#ifndef __RT_ISICG_MIRROR_MATERIAL__
#define __RT_ISICG_MIRROR_MATERIAL__
#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
namespace RT_ISICG
{
	class MirrorMaterial : public BaseMaterial
	{
	  public:
		MirrorMaterial( const std::string & p_name )
			: BaseMaterial( p_name )
		{
		}
		virtual ~MirrorMaterial() = default;
		Vec3f shade( const Vec3f & p_ray,
					 const HitRecord &	 p_hitRecord,
					 const Vec3f & p_lightSample ) const override
		{
			return BLACK;
		}
		inline const Vec3f & getFlatColor() const override
		{
			return BLACK;
		}

		const bool isMirror() override { return true; }
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_MIRROR_MATERIAL__