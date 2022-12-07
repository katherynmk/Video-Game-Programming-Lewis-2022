#ifndef __FROG__SPRITEANIMATION_H__
#define __FROG__SPRITEANIMATION_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "Table.h"
#include "Image.h"

namespace Webfoot {

class JSONValue;
class Sprite;
class SpriteResourceFile;

//=============================================================================

/// SpriteAnimation contains cached data for the specification of a single sprite animation.
/// It also includes reference counted copies of the image data for all
/// the frames of the animation.
class SpriteAnimation
{
public:
   SpriteAnimation();
   virtual ~SpriteAnimation() {}

   /// Draw the specified frame.  If 'sourceRegion' is provided, only that part of
   /// the frame will be drawn, and the top-left corner of the source region
   /// will be at the given position.  If 'sourceRegion' is not specified, draw
   /// the whole frame with the origin at the given position.
   void Draw(Point2F position, unsigned int time = 0, Box2F* sourceRegion = NULL,
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, float depth = 0.0f, float additiveBlending = 0.0f);
   /// Draw the specified frame.  If 'sourceRegion' is provided, only that part of
   /// the frame will be drawn, and the top-left corner of the source region
   /// will be at the given position.  If 'sourceRegion' is not specified, draw
   /// the whole frame with the origin at the given position.
   void Draw(const Point2I& position, unsigned int time = 0, Box2F* sourceRegion = NULL,
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, float depth = 0.0f, float additiveBlending = 0.0f)
   {
      Draw(Point2F::Create((float)position.x, (float)position.y), time, sourceRegion, color, depth, additiveBlending);
   }

   /// Return the name of the animation
   const char* NameGet() { return name; }
   /// Return the sum of all offsets applied to the image at the given time in the animation.
   /// Until frames have individual offsets, this is simply the animationOffset.
   Point2F OffsetGet(unsigned int time) { (void)time; return animationOffset; }
   /// Get the offset to be applied across all frames
   Point2F AnimationOffsetGet() { return animationOffset; }
   /// Set the offset to be applied across all frames
   void AnimationOffsetSet(const Point2F& _animationOffset) { animationOffset = _animationOffset; }
   /// Return the dimensions of the frame at the given time including transparent pixels.
   Point2F SizeGet(unsigned int time);
   /// Return the bounds of the animation across all the frames including transparent pixels.
   Box2F FullBoundsGet();
   /// Return the bounds of the animation across all the frames, excluding
   /// pixels that are always transparent.  On some platforms (OpenGL, for
   /// example), this will only work if "KeepBitmapData" has been set to true
   /// for this animation.  This does not necessarily work with all underlying
   /// bitmap formats.
   Box2F OpaqueBoundsGet();
   /// Return true if the given point is at least partly opaque.  On some
   /// platforms (OpenGL, for example), this will only work if "KeepBitmapData"
   /// has been set to true for this animation.  This does not necessarily work
   /// with all underlying bitmap formats.
   bool OpaqueCheck(const Point2F& testPosition, unsigned int time = 0);
   /// Return the duration of the animation in milliseconds.  If the animation is looping,
   /// this is the duration of a single loop.
   unsigned int DurationGet() { return duration; }

   /// Return the resource file from which this animation was loaded.
   SpriteResourceFile* ResourceFileGet() { return resourceFile; }

   /// Return true if the given sprite resource filename and animation name
   /// match this animation.
   bool EqualCheck(const char* _spriteResourceFilename, const char* _animationName);

   enum PlayType
   {
      PLAY_TYPE_LOOP,
      PLAY_TYPE_ONE_SHOT,
      PLAY_TYPE_PING_PONG_LOOP,
      PLAY_TYPE_REVERSE_LOOP,
      PLAY_TYPE_REVERSE_ONE_SHOT,
      PLAY_TYPE_COUNT
   };

   /// If the current platform does not need to keep the bitmap data to draw
   /// the animation (OpenGL, for example), free any bitmap data that is
   /// currently loaded for this animation.  Use this when KeepBitmapData
   /// was needed, but is no longer necessary.
   void UnnecessaryBitmapDataDeallocate();

   /// One showing of an image for this animation.
   struct Frame
   {
      /// Image to show for this frame.
      Image* image;
      /// Time, in milliseconds, at which this frame begins.
      unsigned int startTime;
      /// Image from which 'image' was derived, if any.
      Image* sourceImage;
   };

   /// Return the index of the frame to display at the given time.
   int FrameIndexGet(unsigned int time);
   /// Return the frame with the given index.  SpriteAnimations and their
   /// frames are shared and generally managed internally, so make changes at
   /// your own risk.
   Frame* FrameGetByIndex(int frameIndex);

   /// Return the sum of all offsets applied to the image at the given frame of the animation.
   /// Until frames have individual offsets, this is simply the animationOffset.
   Point2F FrameOffsetGet(int frameIndex) { (void)frameIndex; return animationOffset; }

protected:
   /// Load the animation with the given name from the specified animation
   /// definition data.  If 'animationEntry' stores a string, it will be used
   /// as a filename for a single-frame animation.  If it stores a JSON object,
   /// it will be treated as a full definition.
   void Init(const char* _name, JSONValue* _specifications, SpriteResourceFile* _resourceFile);
   void Deinit();

   /// Helper function for loading frames.
   void FrameLoad(const char* frameFilename, unsigned int frameStartTime);
   /// Helper function for loading frames from a numbered sequence.
   void FrameLoad(const char* animationFilename, unsigned int frameStartTime, int imageNumber);
   /// Helper function for loading frames that are subsets of other images.
   void FrameLoad(const char* sourceImageFilename, unsigned int frameStartTime,
      const Point2F& frameSize, int subsetNumber);

   /// Increment the reference count for this object.
   void ReferenceCountIncrement() { referenceCount++; }
   /// Decrement the reference count for this object.
   void ReferenceCountDecrement() { referenceCount--; }
   /// Return the reference count for this object.
   int ReferenceCountGet() { return referenceCount; }

   /// The name of the animation
   char* name;
   /// The pattern for loading the animation's frames.
   const char* filename;
   /// The resource file to which this animation data belongs.
   SpriteResourceFile* resourceFile;

   /// True if the bitmap data for this animation should be kept loaded even if
   /// it is not needed for drawing purposes after it is loaded.
   bool keepBitmapData;
   /// True if the alpha should be premultiplied.
   bool premultiplyAlpha;
   /// True if mipmaps should be generated.
   bool generateMipmaps;
   /// True if point filtering should be used instead of bilinear filtering.
   bool pointFiltering;
   /// The total number of frames
   int frameCount;
   /// Duration of the animation in milliseconds.
   unsigned int duration;
   /// How the animation should be played. (one-shot, looping, etc.)
   PlayType playType;
   /// Details of which image to show and when.
   Table<Frame> frames;
   /// The animation is always drawn at this offset from the requested position.
   Point2F animationOffset;
   /// The number of times the image data has been "loaded" and not yet freed.
   int referenceCount;
   /// JSON data with the details for this object.
   JSONValue* specifications;

   friend class SpriteResourceFile;
   friend class Sprite;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__SPRITEANIMATION_H__
