#ifndef __FROG__DUCK__DRAWABLE_H__
#define __FROG__DUCK__DRAWABLE_H__

#include "FrogMemory.h"
#include "Frustum.h"
#include "Matrix43.h"
#include "Matrix44.h"
#include "Point3.h"

namespace Webfoot {
namespace Duck {

class SceneNode;
class SceneNodeProjectorTexture;

//==============================================================================

/// Base class for objects which can be drawn.
class Drawable
{
public:
   virtual ~Drawable() {}
   
   /// Draw this object now.
   virtual void Draw() = 0;

   /// Used for drawing depth to a shadow map.
   virtual void DrawDepth(const Matrix44& lightProjectionMatrix,
      const Matrix43& lightViewMatrix, const Frustum& lightFrustum)
   {
      (void)lightProjectionMatrix; (void)lightViewMatrix; (void)lightFrustum;
   }
   /// Used for projecting a texture onto the drawable.
   /// 'sceneNodeProjectorTexture' is the node representing the projector,
   /// 'projectorMatrix' projects from world space to texture space, and 
   /// projectorTransform is a cached world space transform of
   /// 'sceneNodeProjectorTexture'.
   virtual void DrawTextureProjection(SceneNodeProjectorTexture* sceneNodeProjectorTexture,
      const Matrix44& projectorMatrix, const Matrix43& projectorTransform)
   {
      (void)sceneNodeProjectorTexture; (void)projectorMatrix; (void)projectorTransform;
   }

   /// Return the scene node associated with this drawable.
   virtual SceneNode* DrawableSceneNodeGet() = 0;
   /// Return the position of this drawable for depth-sorting purposes.
   virtual Point3F DrawableDepthSortPositionGet() = 0;
   /// Return the depth-sort priority if this drawable.  Depth-sorted drawables
   /// of numerically higher priority are drawn after depth-sorted drawables of
   /// lower priority.  Depth-sorted drawables of the same priority are sorted by
   /// depth.  The default priority is 0.
   virtual float DrawableDepthSortPriorityGet() = 0;

   /// Comparator for sorting opaque drawables by depth.
   static bool DrawableOpaqueDepthSortComparator(const Drawable* a, const Drawable* b)
   {
      // Sort opaque objects from front to back, but compare the depth sort
      // priority first.
      if(a->drawableDepthSortPriority < b->drawableDepthSortPriority)
         return true;
      else if(a->drawableDepthSortPriority > b->drawableDepthSortPriority)
         return false;
      else
         return a->drawableDepthSortValue < b->drawableDepthSortValue;
   }

   /// Comparator for sorting non-opaque drawables by depth.
   static bool DrawableTransparentDepthSortComparator(const Drawable* a, const Drawable* b)
   {
      // Sort transparent objects from back to front, but compare the depth
      // sort priority first.
      if(a->drawableDepthSortPriority < b->drawableDepthSortPriority)
         return true;
      else if(a->drawableDepthSortPriority > b->drawableDepthSortPriority)
         return false;
      else
         return a->drawableDepthSortValue > b->drawableDepthSortValue;
   }

   /// Helper for sorting drawables by depth.  This is for internal use only.
   float drawableDepthSortValue;
   /// Helper for sorting drawables by depth.  This is for internal use only.
   float drawableDepthSortPriority;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__DRAWABLE_H__
