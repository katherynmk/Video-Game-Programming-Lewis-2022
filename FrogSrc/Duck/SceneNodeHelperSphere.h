#ifndef __FROG__DUCK__SCENENODEHELPERSPHERE_H__
#define __FROG__DUCK__SCENENODEHELPERSPHERE_H__

#include "FrogMemory.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

/// Radius of the sphere, excluding the influence of scaling.
#define DUCK_SCENE_NODE_HELPER_SPHERE_HELPER_SPHERE_RADIUS_KEY "HelperSphereRadius"
/// Color of the lines that make up the helper sphere.
#define DUCK_SCENE_NODE_HELPER_SPHERE_HELPER_SPHERE_COLOR_KEY "HelperSphereColor"
/// Width of the lines that make up the helper sphere in pixels.
#define DUCK_SCENE_NODE_HELPER_SPHERE_HELPER_SPHERE_LINE_WIDTH_KEY "HelperSphereLineWidth"

//==============================================================================

/// Scene node for providing a sphere centered on this object's origin,
/// although not generally for display purposes.
class SceneNodeHelperSphere : public SceneNode
{
public:
   SceneNodeHelperSphere();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Set the radius of the sphere, excluding the influence of scaling.
   void HelperSphereRadiusSet(float _helperSphereRadius) { helperSphereRadius = _helperSphereRadius; }
   /// Return the radius of the sphere, excluding the influence of scaling.
   float HelperSphereRadiusGet() { return helperSphereRadius; }
   /// Set the color of the lines that make up the helper sphere.
   void HelperSphereColorSet(const ColorRGBA8& _helperSphereColor) { helperSphereColor = _helperSphereColor; }
   /// Return the color of the lines that make up the helper sphere.
   ColorRGBA8 HelperSphereColorGet() { return helperSphereColor; }
   /// Set the width of the lines that make up the helper sphere in pixels.
   void HelperSphereLineWidthSet(float _helperSphereLineWidth) { helperSphereLineWidth = _helperSphereLineWidth; }
   /// Return the width of the lines that make up the helper sphere in pixels.
   float HelperSphereLineWidthGet() { return helperSphereLineWidth; }

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet();

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeHelperSphere(); }

   typedef SceneNode Inherited;

protected:
   /// Radius of the sphere, excluding the influence of scaling.
   float helperSphereRadius;
   /// Color of the lines that make up the helper sphere.
   ColorRGBA8 helperSphereColor;
   /// Width of the lines that make up the helper sphere in pixels.
   float helperSphereLineWidth;
   /// Object for drawing the sphere.
   SceneNodeHelperSphereDrawable* drawable;
};

//==============================================================================

/// Helper class for drawing helper spheres in Duck.
class SceneNodeHelperSphereDrawable : public Drawable
{
public:
   SceneNodeHelperSphereDrawable();

   void Init(SceneNodeHelperSphere* _sceneNodeHelperSphere);
   void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeHelperSphere; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

protected:
   /// SceneNodeHelperSphere to which this drawable belongs.
   SceneNodeHelperSphere* sceneNodeHelperSphere;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEHELPERSPHERE_H__
