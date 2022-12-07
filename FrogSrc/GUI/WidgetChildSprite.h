#ifndef __FROG__GUI__WIDGETCHILDSPRITE_H__
#define __FROG__GUI__WIDGETCHILDSPRITE_H__

#include "FrogMemory.h"
#include "Sprite.h"

namespace Webfoot {

class Widget;

//=============================================================================

/// Sprite with extra functionality needed for use with a parent widget.
/// Call Init or InitFromJSON immediately after construction.
/// PositionSet/PositionGet are now effectively relative to the parent.
/// Call UpdatePosition whenever the parent moves.
class WidgetChildSprite : public Sprite
{
public:
   /// Create an empty sprite.
   WidgetChildSprite();
   virtual ~WidgetChildSprite() {}

   /// Initialize the sprite without setting an animation.
   /// '_parentWidget' is a reference to the widget that owns this sprite.
   virtual void Init(Widget* _parentWidget);
   /// Initialize a sprite with the given spriteName from the given file.
   /// '_parentWidget' is a reference to the widget that owns this sprite.
   virtual void Init(const char* resourceFilename, const char* spriteName, Widget* _parentWidget);
   /// Initialize the sprite based on the data in the given table.
   /// '_name' is the name of the object, not the sprite in the resource file.
   /// '_parentWidget' is a reference to the widget that owns this sprite.
   /// 'specifications' gives the details of the sprite.
   virtual void InitFromJSON(const char* _name, JSONValue* specifications, Widget* _parentWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init() { assert(false); }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* resourceFilename, const char* animationName) { assert(false); (void)resourceFilename; (void)animationName; }

   /// Draw the sprite at its current position.  Apply the parent's clip area.
   virtual void Draw();

   /// Return true if the given point is at least partly opaque.  On some
   /// platforms (OpenGL, for example), this will only work if "KeepBitmapData"
   /// has been set to true for this animation.  This does not necessarily work
   /// with all underlying bitmap formats.  'testPosition' should be given in
   /// the local coordinate space of the widget that owns this sprite.
   virtual bool OpaqueCheck(const Point2F& testPosition);

protected:
   typedef Sprite Inherited;

   /// The widget that owns this sprite.
   Widget* parentWidget;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__WIDGETCHILDSPRITE_H__
