#ifndef __FROG__CURSOR_H__
#define __FROG__CURSOR_H__

#include "FrogMemory.h"
#include "Image.h"
#include "Sprite.h"

namespace Webfoot {

//=============================================================================

/// Cursor allows you to draw an Image or SpriteAnimation at the position of a
/// Mouse.
class Cursor
{
public:
   Cursor();

   void Init();
   void Deinit();
   void Update(unsigned int dt);
   void Draw();

   /// Set which mouse from theMice to follow or -1 to follow theMouse instead.
   void MouseIndexSet(int _mouseIndex) { mouseIndex = _mouseIndex; }
   /// See 'MouseIndexSet'.
   int MouseIndexGet() { return mouseIndex; }

   /// Set 'imageFilename' to the image you wish to use for the cursor.
   void ImageSet(const char* imageFilename);
   /// Set the sprite animation to display for the cursor.
   void AnimationSet(const char* resourceFilename, const char* animationName);
   /// Set the sprite animation to display for the cursor.
   void AnimationSet(const char* animationName);

   /// Set whether to actually draw the cursor.
   void VisibleSet(bool _visible) { visible = _visible; }
   /// Return true if the cursor should actually be drawn.
   bool VisibleCheck() { return visible; }

   /// Set whether the cursor should be hidden automatically on touchscreens.
   void HideOnTouchscreenSet(bool _hideOnTouchscreen) { hideOnTouchscreen = _hideOnTouchscreen; }
   /// Return true if the cursor should be hidden automatically on
   /// touchscreens.
   bool HideOnTouchscreenCheck() { return hideOnTouchscreen; }

   /// Set the color to be applied to this cursor.
   void ColorSet(const ColorRGBA8& _color);
   /// Return the color applied to this cursor.
   ColorRGBA8 ColorGet() { return color; }

   /// Return the full bounds of the current cursor image or animation relative
   /// to the position of the cursor.
   Box2F FullBoundsGet();

protected:
   /// Clean up any loaded Image for 'cursorImage'.
   void ImageClear();

   /// True if the cursor should actually be drawn.
   bool visible;
   /// True if the cursor should be hidden automatically on touchscreens.
   bool hideOnTouchscreen;
   /// Index of the mouse from theMice.  Use theMouse if the index is -1.
   int mouseIndex;
   /// Color applied to this cursor.
   ColorRGBA8 color;
   /// Underlying image that can be used to draw the cursor.
   Image* cursorImage;
   /// Underlying sprite that can be used to draw the cursor.
   Sprite* cursorSprite;
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CURSOR_H__
