#ifndef __FROG__GUI__SPRITEWIDGET_H__
#define __FROG__GUI__SPRITEWIDGET_H__

#include "FrogMemory.h"
#include "GUI/Widget.h"

namespace Webfoot {

class LayerWidget;
class WidgetChildSprite;
class SpriteAnimation;

//=============================================================================

/// Widget for a single sprite.
/// Call Init immediately after construction.
class SpriteWidget : public Widget
{
public:
   SpriteWidget();
   virtual ~SpriteWidget() {}

   /// Initialize a new SpriteWidget.
   /// Call this or the other Init immediately after construction.
   virtual void Init(const char* _name, Widget* _parent);
   /// Initialize a new SpriteWidget based on the specifications in the given JSONValue.
   /// Call this immediately after construction.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its sprite
   virtual void Update(unsigned int dt);
   /// Draw the sprite.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Use the sprite animation with the given name from the given file.
   /// If no sprite file is specified, it will assume it is in the widget's
   /// layer's sprite file.
   virtual void AnimationSet(const char* resourceFilename, const char* animationName);
   /// If a sprite animation is loaded, unload it.
   virtual void AnimationClear();
   /// Return the current sprite animation, if any.
   virtual SpriteAnimation* AnimationGet();

   /// Cancel any pending StartDelay timer.  This does not unpause the sprite.
   virtual void StartDelayClear() { startDelayTimer = 0; }

   /// Return the bounds of the images themselves across all the frames including transparent pixels.
   Box2F ImageBoundsFullGet();

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) SpriteWidget(); }

   virtual WidgetChildSprite* WidgetChildSpriteGet() { return sprite; }

protected:
   typedef Widget Inherited;

   /// The sprite displayed by this widget.
   WidgetChildSprite* sprite;
   /// Timer for delaying the start of the animation.
   unsigned int startDelayTimer;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__SPRITEWIDGET_H__
