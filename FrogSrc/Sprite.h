#ifndef __FROG__SPRITE_H__
#define __FROG__SPRITE_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "SpriteAnimation.h"
#include "Color.h"
#include "Table.h"

namespace Webfoot {

//=============================================================================

/// A 'Sprite' displays an animation, keeps track of its own position, scale,
/// and rotation on the screen screen and its position in its current animation.
/// It must be updated on every frame to make sure it will display the correct image.
/// Scale and rotation are done relative to the origin of the current sprite frame.
/// Transforms are applied in the following order: scale, rotation, position.
class Sprite
{
public:
   /// Create an empty sprite.
   Sprite();
   /// Cleanup
   virtual ~Sprite() {}

   /// Initialize the sprite without setting its appearance.
   void Init();
   /// Initialize the sprite with the given animation from the given file.
   /// Do not include the extension when specifying the filename.
   void Init(const char* resourceFilename, const char* animationName);
   virtual void Deinit();

   /// Deinit any current animation, then load the given animation along with its defaults.
   /// Do not include the extension when specifying the filename.
   virtual void AnimationSet(const char* resourceFilename, const char* animationName);
   /// Same as the full form of AnimationSet, except that it assumes the same resource file.
   virtual void AnimationSet(const char* animationName);
   /// Return a pointer to the current animation, if any.
   SpriteAnimation* AnimationGet() const { return animation; }
   /// If sprite data is loaded, release it.
   virtual void AnimationClear();
   
   /// Add the given animation to the queue of animations to be played automatically in sequence.
   /// The switch will happen when an animation ends or when the current iteration of a looping
   /// animation ends.  Since this does not operate in a first-in-last-out fashion,
   /// only use this if you've preloaded all the sprites in question.
   void AnimationEnqueue(const char* resourceFilename, const char* animationName);
   /// Same as the full form of AnimationEnqueue, except that it assumes the same resource file
   /// as the one that is currently set.
   void AnimationEnqueue(const char* animationName);
   /// If sprite data is loaded for enqueued animations, release it.
   void AnimationQueueClear();
   /// If the given sprite animation is in the queue, remove it.
   void AnimationQueueRemove(const char* resourceFilename, const char* animationName);
   /// Reserve space for the given number of animations in the animation queue.
   void AnimationQueueReserve(int reserveSize) { animationQueue.Reserve(reserveSize); }

   /// Update the sprite's animation.
   virtual void Update(unsigned int dt);
   /// Draw the sprite at its current position.
   virtual void Draw();

   /// Return the current number of milliseconds into the animation.
   unsigned int TimeGet() { return (unsigned int)time; }
   /// Set the current number of milliseconds into the animation.
   void TimeSet(int _time) { time = (float)_time; }
   
   /// Set to true if the animation should pause here.
   /// Set to false to resume.
   void PausedSet(bool _paused) { paused = _paused; }
   /// Return true if the sprite animation is paused.
   virtual bool PausedCheck() { return paused; }

   /// Set to true if the sprite should be drawn
   void VisibleSet(bool _visible) { visible = _visible; }
   /// Return true if the sprite should be drawn.
   virtual bool VisibleCheck() { return visible; }

   /// Return the current position of the sprite.
   virtual Point2F PositionGet() { return position; }
   /// Return the current position of the sprite.
   Point3F Position3Get() { return Point3F::Create(position.x, position.y, depth); }
   /// Return the current position of the sprite in integers.
   /// Uses the position returned by PositionGet.
   Point2I IntegerPositionGet();

   /// Set the current position of the sprite.
   void PositionSet(float x, float y) { position.Set(x, y); }
   /// Set the current position of the sprite.
   void PositionSet(int x, int y) { position.Set((float)x, (float)y); }
   /// Set the current position of the sprite.
   void PositionSet(const Point2F& _position) { position = _position; }
   /// Set the current position of the sprite.
   void PositionSet(const Point2I& _position) { position.Set((float)_position.x, (float)_position.y); }
   /// Set the current position of the sprite.
   void PositionSet(const Point3F& _position) { position = Point2F::Create(_position.x, _position.y); depth = _position.z; }
   
   /// Set how much to rotate the sprite about its origin.  Positive rotations are counter-clockwise.
   /// Angles should be given in degrees.
   void RotationSet(float _rotation) { rotation = _rotation; }
   /// How much to rotate the sprite about its origin.  Positive rotations are counter-clockwise.
   /// Angles are in degrees.
   float RotationGet() { return rotation; }

   /// Set how to scale the sprite.
   void ScaleSet(const Point2F& _scale) { scale = _scale; }
   /// Return how to scale the sprite.
   Point2F ScaleGet() { return scale; }

   /// Set the color to apply to all vertices when drawing.
   /// By manipulating the 'alpha' component, you can control the opacity of the sprite.
   void ColorSet(const ColorRGBA8& _color) { color = _color; }
   /// Return the color to apply to all vertices when drawing.
   ColorRGBA8 ColorGet() { return color; }

   /// Set '_additiveBlending' to [0, 1] depending on whether you want normal
   /// alpha blending, additive blending, or something in between.
   void AdditiveBlendingSet(float _additiveBlending) { additiveBlending = _additiveBlending; }
   /// See 'AdditiveBlendingSet'.
   float AdditiveBlendingGet() { return additiveBlending; }

   /// Set the depth component of the sprite's position.
   void DepthSet(float _depth) { depth = _depth; }
   /// Return the depth component of the sprite's position.
   virtual float DepthGet() { return depth; }

   /// Set the play style (one-shot, loop, etc.)
   void PlayTypeSet(SpriteAnimation::PlayType _playType);
   /// Return the current animation type.
   SpriteAnimation::PlayType PlayTypeGet() { return playType; }

   /// Set a coefficient applied to time for animation purposes.  This should
   /// be non-negative.  1.0f plays at the default speed, 2.0f plays at twice
   /// the default speed, and so on.
   void TimeCoefficientSet(float _timeCoefficient) { timeCoefficient = _timeCoefficient; }
   /// See 'AnimationTimeCoefficientSet'.
   float TimeCoefficientGet() { return timeCoefficient; }

   /// Draw the specified subset of the animation with the top-left corner of
   /// the region at the sprite's position.
   void SourceRegionSet(const Box2F _sourceRegion) { sourceRegionUse = true; sourceRegion = _sourceRegion; }
   /// Clear any restrictions previously set by SourceRegionSet.
   void SourceRegionClear() { sourceRegionUse = false; }

   /// Return true if the animation is finished playing.  Not true if it's not finished playing.
   bool FinishedPlayingCheck();

   /// Return the sum of all offsets applied to the bitmap at the given time.
   Point2F OffsetGet(unsigned int _time);
   /// Return the dimensions at the given time including transparent pixels.
   Point2F SizeGet(unsigned int _time);
   /// Return the bounds of the animation across all the frames including transparent pixels.
   Box2F FullBoundsGet();
   
protected:
   /// Without loading or unloading anything, set the current animation.
   virtual void AnimationSetHelper(SpriteAnimation* animation);

   /// The data for the current appearance of the sprite.
   SpriteAnimation* animation;
   /// The position of the sprite
   Point2F position;
   /// The depth component of the sprite's position
   float depth;
   /// How much to rotate the sprite about its origin.  Positive rotations are counter-clockwise.
   /// Angles should be given in degrees.
   float rotation;
   /// How to scale the sprite.
   Point2F scale;
   /// Color to apply to all vertices when drawing.
   ColorRGBA8 color;
   /// [0, 1] depending on whether you want normal alpha blending, additive
   /// blending, or something in between.
   float additiveBlending;
   /// Number of milliseconds into the animation.
   float time;
   /// Coefficient applied to time for animation purposes.
   float timeCoefficient;
   /// If true, the sprite should be shown.
   bool visible;
   /// If true, the animation should not progress
   bool paused;
   /// How to update the animation.
   SpriteAnimation::PlayType playType;
   /// True if 'sourceRegion' should be applied when drawing the animation.
   bool sourceRegionUse;
   /// Subset of the animation to draw if 'sourceRegionUse' is true.
   Box2F sourceRegion;
   /// Animations to be played in sequence.
   Table<SpriteAnimation*> animationQueue;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__SPRITE_H__
