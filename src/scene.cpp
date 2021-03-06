#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/blinn_phong_material.hpp"
#include "materials/micro_face_material.hpp"
#include "materials/matte_material.hpp"
#include "materials/mirror_material.hpp"
#include "materials/transparent_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "objects/implicit_sphere.hpp"
#include "objects/implicit_torus.hpp"
#include "objects/mandlebulb.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"
#include "lights/circle_light.hpp"
#include "lights/spot_light.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <objects/triangle_mesh.hpp>


//scene selector
//#define SCENE_LIGHTING
//#define SCENE_REFRACT
//#define SCENE_ALL_MATERIAL
#define SCENE_PATH_TRACE
//#define SCENE_IMPLICIT
//#define SCENE_MESH


//obj selector for SCENE_MESH
#define BUNNY
//#define CONFERENCE


namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
#ifdef SCENE_1
		// Add objects.

		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.

		_addMaterial( new MatteMaterial( "Blue", GREY , 0.6f ) );
		_addMaterial( new LambertMaterial( "Red", RED ) );


		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight(new PointLight( WHITE, 30, Vec3f( -3.f, 2.f, 3.f ) ) );
		_addLight( new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );
#endif // SCENE_1

#ifdef SCENE_2
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new MirrorMaterial( "Mirror" ) );
		_addMaterial( new TransparentMaterial( "Transparent", false ) );
		_addMaterial( new TransparentMaterial( "TransparentAppro", true ) );
		_addMaterial( new PlasticMaterial( "GreyPlastic", GREY, GREY, 16 ) );
		//_addMaterial( new BlinnPhongMaterial( "GreyBlinnPhong", GREY, 16 ) );
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		_addMaterial( new MatteMaterial( "CyanMatte", CYAN, 0.6f ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// Spheres .

		_addObject( new Sphere( "Sphere1", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_addObject( new Sphere( "Sphere2", Vec3f(-2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Transparent", "Sphere1" );
		_attachMaterialToObject( "Mirror", "Sphere2" );

		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneFront" );
		_addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		_attachMaterialToObject( "CyanMatte", " PlaneRear " );
		// ================================================================
		// Add lights .
		// ================================================================
		//_addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		//_addLight(new SpotLight( WHITE, 50, Vec3f( 0.f, 5.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 1.f ), 2.f, 60.f ) );
		_addLight(new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );

#endif // SCENE_2

#ifdef SCENE_ALL_MATERIAL
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new LambertMaterial( "Lambert", MAGENTA ) );
		_addMaterial( new PlasticMaterial( "Plastic", 0.7f*CYAN, 0.3f*WHITE, 16 ) );
		_addMaterial( new BlinnPhongMaterial( "Blinn", 0.7f*CYAN, 0.3f*WHITE, 16 ) );
		_addMaterial( new MicroFaceMaterial( "Micro", Vec3f(1.f, 0.85f, 0.57f), 0.6f, 0.8f ) );
		_addMaterial( new MatteMaterial( "Matte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// Spheres .

		_addObject( new Sphere( "Sphere0", Vec3f( 0.f, 2.f, 3.f ), 1.f ) );
		_addObject( new Sphere( "Sphere1", Vec3f( 3.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Sphere( "Sphere2", Vec3f( -3.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Sphere( "Sphere3", Vec3f( 3.f, 3.f, 3.f ), 1.f ) );
		_addObject( new Sphere( "Sphere4", Vec3f( -3.f, 3.f, 3.f ), 1.f ) );

		_attachMaterialToObject( "Micro", "Sphere0" );
		_attachMaterialToObject( "Plastic", "Sphere1" );
		_attachMaterialToObject( "Blinn", "Sphere2" );
		_attachMaterialToObject( "Lambert", "Sphere3" );
		_attachMaterialToObject( "Matte", "Sphere4" );

		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneFront" );
		// ================================================================
		// Add lights .
		// ================================================================
		//_addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		//_addLight(new SpotLight( WHITE, 50, Vec3f( 0.f, 5.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 1.f), 2.f, 60.f ) );
		 _addLight(new QuadLight( WHITE, 40, Vec3f( 3.5f, 3.5f, -5.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f(0.f, 2.f, 0.f ) ) );
#endif // SCENE_3

#ifdef SCENE_PATH_TRACE
		 // ================================================================
		 // Add materials .
		 // ================================================================
		 _addMaterial( new MirrorMaterial( "Mirror" ) );
		 _addMaterial( new TransparentMaterial( "Transparent", false ) );
		 _addMaterial( new TransparentMaterial( "TransparentAppro", true ) );
		 _addMaterial( new PlasticMaterial( "GreyPlastic", GREY, GREY, 16 ) );
		 _addMaterial( new MicroFaceMaterial( "Micro", Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.7f ) );
		 //_addMaterial( new BlinnPhongMaterial( "GreyBlinnPhong", GREY, 16 ) );
		 _addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		 _addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		 _addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		 _addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		 _addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		 _addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		 _addMaterial( new MatteMaterial( "CyanMatte", CYAN, 0.6f ) );
		 // ================================================================
		 // Add objects .
		 // ================================================================
		
		 _addObject( new Sphere( "Sphere1", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		 _addObject( new Sphere( "Sphere2", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		 _addObject( new Sphere( "SphereLight", Vec3f( 0.f, 5.f, 3.f ), 0.5f ) );

		 //_addObject( new ImplicitTorus( "Torus", Vec3f( -2.f, 0.f, 3.f ), 1.5f, 0.5f ) );

		 _attachMaterialToObject( "Transparent", "Sphere1" );
		 _attachMaterialToObject( "TransparentAppro", "Sphere2" );
		 _attachMaterialToObject( "GreyPlastic", "SphereLight" );

		 // Pseudo Cornell box made with infinite planes .
		 _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		 _attachMaterialToObject( "GreyMatte", "PlaneGround" );
		 _addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		 _attachMaterialToObject( "RedMatte", "PlaneLeft" );
		 _addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		 _attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		 _addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		 _attachMaterialToObject( "BlueMatte", "PlaneRight" );
		 _addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		 _attachMaterialToObject( "MagentaMatte", "PlaneFront" );
		 _addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		 _attachMaterialToObject( "CyanMatte", " PlaneRear " );
		 // ================================================================
		 // Add lights .
		 // ================================================================
		 _addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		 //_addLight(new SpotLight( WHITE, 50, Vec3f( 0.f, 5.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 1.f
		 //), 2.f, 60.f ) );
		 //_addLight(new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );

#endif // SCENE_4

		 
#ifdef SCENE_IMPLICIT
		 // ================================================================
		 // Add materials .
		 // ================================================================
		 _addMaterial( new MirrorMaterial( "Mirror" ) );
		 _addMaterial( new TransparentMaterial( "Transparent", false ) );
		 _addMaterial( new TransparentMaterial( "TransparentAppro", true ) );
		 _addMaterial( new PlasticMaterial( "GreyPlastic", GREY, GREY, 16 ) );
		 _addMaterial( new MicroFaceMaterial( "Micro", Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.7f ) );
		 //_addMaterial( new BlinnPhongMaterial( "GreyBlinnPhong", GREY, 16 ) );
		 _addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		 _addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		 _addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		 _addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		 _addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		 _addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		 _addMaterial( new MatteMaterial( "CyanMatte", CYAN, 0.6f ) );
		 // ================================================================
		 // Add objects .
		 // ================================================================

		 _addObject( new ImplicitSphere( "Sphere", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		 _addObject( new ImplicitTorus( "Torus", Vec3f( 2.f, 0.f, 3.f ), 1.5f, 0.5f ) );
		 _addObject( new Mandlebulb( "MandleBulb", Vec3f( 0.f, 0.f, 1.f ) ) );

		 //_addObject( new ImplicitTorus( "Torus", Vec3f( -2.f, 0.f, 3.f ), 1.5f, 0.5f ) );

		 _attachMaterialToObject( "CyanMatte", "Sphere" );
		 _attachMaterialToObject( "CyanMatte", "Torus" );
		 _attachMaterialToObject( "Micro", "MandleBulb" );

		 // Pseudo Cornell box made with infinite planes .
		 _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		 _attachMaterialToObject( "GreyMatte", "PlaneGround" );
		 _addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		 _attachMaterialToObject( "RedMatte", "PlaneLeft" );
		 _addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		 _attachMaterialToObject( "GreenMatte", "PlaneCeiling" );
		 _addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		 _attachMaterialToObject( "BlueMatte", "PlaneRight" );
		 _addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		 _attachMaterialToObject( "MagentaMatte", "PlaneFront" );
		 _addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		 _attachMaterialToObject( "CyanMatte", " PlaneRear " );
		 // ================================================================
		 // Add lights .
		 // ================================================================
		 //_addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		 //_addLight(new SpotLight( WHITE, 50, Vec3f( 0.f, 5.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 1.f
		 //), 2.f, 60.f ) );
		 _addLight(
			 new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );

#endif // SCENE_4



#ifdef SCENE_5
	#ifdef BUNNY
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new MirrorMaterial( "Mirror" ) );
		_addMaterial( new TransparentMaterial( "Transparent", false ) );
		_addMaterial( new MicroFaceMaterial( "Micro", Vec3f( 1.f, 0.85f, 0.57f ), 0.6f, 0.8f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );
		_addMaterial( new MatteMaterial( "YellowMatte", YELLOW, 0.6f ) );
		_addMaterial( new MatteMaterial( "CyanMatte", CYAN, 0.6f ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// OBJ.
	
		loadFileTriangleMesh( "bunny", DATA_PATH + "bunny.obj" );
		_attachMaterialToObject( "Micro", "bunny_defaultobject" );

		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyMatte", " PlaneGround " );
		_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", " PlaneLeft " );
		_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "CyanMatte", " PlaneCeiling " );
		_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueMatte", " PlaneRight " );
		_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaMatte", " PlaneFront " );
		_addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		_attachMaterialToObject( "YellowMatte", " PlaneRear " );
		//_addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		_addLight(new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );
	#endif // BUNNY

	#ifdef CONFERENCE
		loadFileTriangleMesh( "conference", DATA_PATH + "conference/conference.obj" );
	#endif // CONFERECE


		
		// ================================================================
		// Add lights .
		// ================================================================
		//_addLight( new PointLight( WHITE, 100.f, Vec3f( 900.f, 600.f, -300.f ) ) );
		_addLight( new QuadLight( WHITE, 20.f, Vec3f( 900.f, 600.f, -300.f ), Vec3f( -800.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 300.f ) ) );
		//_addLight( new QuadLight( WHITE, 40, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );
#endif // SCENE_3

	}
	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const { 
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) )
			{
				return true;
			}
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;
		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );
		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }
		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;
		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }
			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;
			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;
			const bool	   hasUV   = mesh->HasTextureCoords( 0 );
			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}
			triMesh->buildBVH();
			_addObject( triMesh );
			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f	  kd = WHITE;
				Vec3f	  ks = BLACK;
				float	  s	 = 0.f;
				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				#ifdef CONFERENCE
				_addMaterial( new PlasticMaterial( std::string( mtlName.C_Str() ), kd, ks, s ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
				#endif // CONFERENCE
			}
			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

} // namespace RT_ISICG
