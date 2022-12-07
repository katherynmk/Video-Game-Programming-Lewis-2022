#ifndef __FROG__GUI__BUTTONWIDGET_H__
#define __FROG__GUI__BUTTONWIDGET_H__

#include "FrogMemory.h"
#include "Table.h"
#include "GUI/LabelWidget.h"

namespace Webfoot {

class LayerWidget;
class Sprite;
class ButtonWidget;
class ButtonWidgetChildSprite;

//=============================================================================

/// Widget that serves as a base class for specific types of buttons.
/// Call Init immediately after construction.
class ButtonWidget : public LabelWidget
{
public:
   ButtonWidget();
   virtual ~ButtonWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   /// Call this or the other Init immediately after construction.
   virtual void Init(const char* _name, Widget* _parent);
   /// Call this immediately after construction.
   /// Initialize a new ButtonWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget
   virtual void Deinit();

   /// Update the widget and its sprites.
   virtual void Update(unsigned int dt);
   /// Draw the right sprite.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return the box, relative to the widget's position, within which the
   /// text should be aligned and clipped.  If the text bounds were not
   /// defined, use the widget bounds.  Apply the current textPositionOffset.
   virtual Box2F TextBoundsGet();

   /// Return the font.  If the font is not defined for this
   /// widget, it's parent's will be returned.  If there is no parent,
   /// NULL will be returned.  If the currently displayed
   /// sprite has a defined index, return that.
   virtual Font* FontGet();
   /// Return the scale at which the text will be drawn.
   virtual Point2F TextScaleGet();
   /// Return the color to apply to the text when drawing.
   virtual ColorRGBA8 TextColorGet();

   /// Set the animation to use for the given sprite index.
   void AnimationSet(int spriteIndex, const char* animationName);
   /// Set the animation to use for the given sprite index.  Do not include the
   /// extension when specifying the 'spriteResourceFilename'.
   void AnimationSet(int spriteIndex, const char* spriteResourceFilename, const char* animationName);

   /// Rather than basing the displayed state of the button on the normal rules,
   /// try to use the given sprite index.  Call this with -1 to return to the
   /// normal rules.
   virtual void SpriteIndexForcedSet(int spriteIndex) { spriteIndexForced = spriteIndex; }
   /// If something other than -1 is returned, try to use that as the sprite
   /// index for the appearance of the button.
   virtual int SpriteIndexForcedGet() { return spriteIndexForced; }

   /// Return the color to use when drawing the bounds of the Widget.
   virtual ColorRGBA8 BoundsColorGet();
   /// Return the color to use when drawing the filled bounds of the Widget.
   virtual ColorRGBA8 BoundsFilledColorGet();

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// If false, only show the disabled state of the button if the button itself
   /// is specifically disabled.  If the button is only disabled because of an
   /// ancestor, continue to show the normal, enabled state.
   virtual bool DisplayDisabledImplicitlyCheck() { return displayDisabledImplicitly; }
   /// See DisplayDisabledImplicitlyCheck.
   virtual void DisplayDisabledImplicitlySet(bool _displayDisabledImplicitly) { displayDisabledImplicitly = _displayDisabledImplicitly; }

protected:
   typedef LabelWidget Inherited;

   // If the requested sprite is not loaded, try to find the next
   // closest sprite that is loaded.  Different requested sprites
   // have their own order of other sprites to try.
   /// This does not guarantee the sprite at the returned index will be loaded.
   int SpriteIndexLoadedGet(int spriteIndex) const;

   /// Set the state of the button and change the appearance
   /// to match the new state.
   void StateSet(int newState);

   /// Return the maximum number of sprites that can be used by this button type.
   virtual int SpriteCountGet() const = 0;
   /// Return an array of the names of the JSON objects for all the sprites that
   /// can be used by this button type.  The number of names is the same number
   /// returned by SpriteCountGet().
   virtual const char** SpriteTableNamesGet() const = 0;
   /// Return a multidimensional of sprite indices.  The outer array is of desired sprite
   /// indices.  Each inner array is an ordered list of sprite indices to try if
   /// there is no sprite loaded for the requested index.
   /// -1 means no substitution should be made, and no sprite should be shown.
   virtual const int* SpriteFallbackListGet() const = 0;
   /// Return the first choice for the sprite that should be displayed, if it's loaded.
   virtual int SpriteIndexIdealGet() = 0;

   /// Return the default alignment for the text.
   virtual int TextAlignmentDefaultGet();

   /// If something other than -1, try to use that as the sprite index for the
   /// appearance of the button.
   int spriteIndexForced;
   /// Current state of the button.  The enumeration of the states is defined in
   /// the classes that inherit this widget.  There is a 1:1 mapping of states
   /// to sprites in the 'sprites' vector.
   int state;
   /// If false, only show the disabled state of the button if the button itself
   /// is specifically disabled.  If the button is only disabled because of an
   /// ancestor, continue to show the normal, enabled state.
   bool displayDisabledImplicitly;

   /// Offset from the normal position at which the text should be drawn.
   Point2F textPositionOffset;
   /// 'textPositionOffset' specified at the button level or defaulted to at
   /// the button level.
   Point2F buttonTextPositionOffset;

   /// The sprites displayed by this widget.
   Table<ButtonWidgetChildSprite*> sprites;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__BUTTONWIDGET_H__
