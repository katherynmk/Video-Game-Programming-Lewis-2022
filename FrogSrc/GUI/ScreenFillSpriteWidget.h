#ifndef __FROG__GUI__SCREENFILLSPRITEWIDGET_H__
#define __FROG__GUI__SCREENFILLSPRITEWIDGET_H__

#include "FrogMemory.h"
#include "GUI/SpriteWidget.h"

namespace Webfoot {

//=============================================================================

/// ScreenFillSpriteWidget is a SpriteWidget that automatically scales and
/// positions itself to fill the screen.
class ScreenFillSpriteWidget : public SpriteWidget
{
public:
   /// Ways to make the sprite fill the screen.
   enum FillMode
   {
      /// Stretch the sprite to fill the part of the screen available to the
      /// game.  This includes the extended area, if allowed.  This does not
      /// necessarily preserve aspect ratio.
      FILL_MODE_FULL,

      /// While preserving the aspect ratio, stretch the sprite to fill the part
      /// of the screen available to the game.  This includes the extended area,
      /// if allowed.  The sprite may be cropped and centered.
      FILL_MODE_FULL_KEEP_ASPECT_RATIO,

      /// Number of FillModes
      FILL_MODE_COUNT
   };
   
   virtual ~ScreenFillSpriteWidget() {}
   /// Initialize a new ScreenFillSpriteWidget based on the specifications in the given JSONValue.
   /// Call this immediately after construction.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   // Use the sprite animation with the given name from the given file.
   // If no sprite file is specified, it will assume it is in the widget's layer's sprite file.
   virtual void AnimationSet(const char* resourceFilename, const char* animationName);
   // If a sprite animation is loaded, unload it.
   virtual void AnimationClear();

   /// Set how the sprite should fill the screen.
   virtual void FillModeSet(FillMode _fillMode);
   /// Return how the sprite should fill the screen.
   virtual FillMode FillModeGet() { return fillMode; }

   virtual void BoundsSet(Box2F _bounds);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ScreenFillSpriteWidget(); }

protected:
   typedef SpriteWidget Inherited;

   /// Update the position and scale to fill the screen.
   virtual void ScreenFillRefresh();

   /// How the sprite should fill the screen.
   FillMode fillMode;
   /// True if the bounds have been set explicitly.
   bool boundsExplicit;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__SCREENFILLSPRITEWIDGET_H__
