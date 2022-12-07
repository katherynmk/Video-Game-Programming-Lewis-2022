#ifndef __FROG__DUCK__SCENENODESPRITE_H__
#define __FROG__DUCK__SCENENODESPRITE_H__

#include "FrogMemory.h"
#include "Sprite.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

class SceneNodeSpriteDrawable;

/// Key for the name of the sprite animation.
#define DUCK_SCENE_NODE_SPRITE_ANIMATION_KEY "SpriteAnimation"
/// Width and height of the sprite in world units.
#define DUCK_SCENE_NODE_SPRITE_SIZE_KEY "SpriteSize"

//==============================================================================

/// Scene node for drawing a sprite.
class SceneNodeSprite : public SceneNode
{
public:
   SceneNodeSprite();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet() { return drawablesBoundingSphere; }

   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Use the sprite animation with the given name from the given file.  If no
   /// sprite file is specified, it will fall back on SpriteResourceFilenameGet.
   virtual void SpriteAnimationSet(const char* _resourceFilename, const char* animationName);
   /// Return true if the given sprite animation exists in the current sprite
   /// resource file.
   bool SpriteAnimationExistsCheck(const char* animationName);
   /// Return true if the animation is finished playing.
   bool SpriteFinishedPlayingCheck() { return sprite.FinishedPlayingCheck(); }
   /// Set the current number of milliseconds into the sprite animation.
   void SpriteTimeSet(int _time) { sprite.TimeSet(_time); }
   /// Set how much to rotate the sprite about its origin.  Positive rotations are counter-clockwise.
   /// Angles should be given in degrees.
   void SpriteRotationSet(float _rotation) { sprite.RotationSet(_rotation); }
   /// See 'SpriteRotationSet'.
   float SpriteRotationGet() { return sprite.RotationGet(); }
   /// Set the scale of the sprite such that the current animation would have
   /// the given dimensions in world space.
   void SpriteSizeSet(const Point2F& _spriteSize);
   /// Set whether the sprite should be flipped horizontally.
   void SpriteFlipHorizontalSet(bool _spriteFlipHorizontal);
   /// Return true if the sprite should be flipped horizontally.
   bool SpriteFlipHorizontalCheck() { return spriteFlipHorizontal; }
   /// Set the color to apply to all vertices when drawing.
   /// By manipulating the 'alpha' component, you can control the opacity of the sprite.
   void SpriteColorSet(const ColorRGBA8& _color) { sprite.ColorSet(_color); }
   /// Set '_additiveBlending' to [0, 1] depending on whether you want normal
   /// alpha blending, additive blending, or something in between.
   void SpriteAdditiveBlendingSet(float _additiveBlending) { sprite.AdditiveBlendingSet(_additiveBlending); }
   /// Returns the sprite object owned by this node.
   Sprite* SpriteGet() { return &sprite; }

   /// Refresh the bounding volume of the sprite in local coordinates.
   void BoundingVolumesRefresh();

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeSprite(); }

   typedef SceneNode Inherited;

protected:
   /// Set the scale of the sprite based on the current settings.
   void SpriteScaleRefresh();

   /// Frog sprite
   Sprite sprite;
   /// Object for drawing the sprite.
   SceneNodeSpriteDrawable* drawable;
   /// Bounding sphere of the sprite in local coordinates.
   Sphere drawablesBoundingSphere;
   /// Bounding box of arbitrary narrow thickness.
   Box3F drawablesBoundingBox;
   /// Dimensions of the sprite in world space.
   Point2F spriteSize;
   /// True if the sprite should be flipped horizontally.
   bool spriteFlipHorizontal;
};

//==============================================================================

/// Helper class for drawing sprites in Duck.
class SceneNodeSpriteDrawable : public Drawable
{
public:
   SceneNodeSpriteDrawable();

   void Init(SceneNodeSprite* _sceneNodeSprite);
   void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeSprite; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

protected:
   /// SceneNodeSprite to which this drawable belongs.
   SceneNodeSprite* sceneNodeSprite;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODESPRITE_H__
