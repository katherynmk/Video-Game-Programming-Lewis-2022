#ifndef __FROG__GUI__TOGGLEBUTTONWIDGET_H__
#define __FROG__GUI__TOGGLEBUTTONWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ButtonWidget.h"

namespace Webfoot {

class LayerWidget;

/// Name of the state of a toggle button which is toggled off with the button
/// unpressed and without the cursor positioned over it.
#define TOGGLE_BUTTON_WIDGET_OFF_UP_OUT_STATE_NAME "OffUpOutState"
/// Name of the state of a toggle button which is toggled on with the button
/// unpressed and without the cursor positioned over it.
#define TOGGLE_BUTTON_WIDGET_ON_UP_OUT_STATE_NAME "OnUpOutState"

//=============================================================================

/// Widget for a button that, when clicked, toggles between being on and off.
/// Call Init immediately after construction.
class ToggleButtonWidget : public ButtonWidget
{
public:
   /// Make an empty ToggleButtonWidget
   ToggleButtonWidget();
   virtual ~ToggleButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new ToggleButtonWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// List of sprite indices for this button type based.
   /// Each index corresponds to a space for a sprite in the 'sprites' vector.
   enum ToggleButtonSprites
   {
      /// The mouse is unpressed and not over the button, and the button is toggled off.
      TOGGLE_BUTTON_SPRITE_OFF_UP_OUT,
      /// The mouse is unpressed and over the button, and the button is toggled off.
      TOGGLE_BUTTON_SPRITE_OFF_UP_OVER,
      /// The mouse button was pressed over the button and dragged off, but still held, and the button is toggled off.
      TOGGLE_BUTTON_SPRITE_OFF_DOWN_OUT,
      /// The mouse button is pressed over the button, and the button is toggled off.
      TOGGLE_BUTTON_SPRITE_OFF_DOWN_OVER,
      /// The button cannot currently be pressed, and the button is toggled off.
      TOGGLE_BUTTON_SPRITE_OFF_DISABLED,
      /// The mouse is unpressed and not over the button, and the button is toggled on.
      TOGGLE_BUTTON_SPRITE_ON_UP_OUT,
      /// The mouse is unpressed and over the button, and the button is toggled on.
      TOGGLE_BUTTON_SPRITE_ON_UP_OVER,
      /// The mouse button was pressed over the button and dragged off, but still held, and the button is toggled on.
      TOGGLE_BUTTON_SPRITE_ON_DOWN_OUT,
      /// The mouse button is pressed over the button, and the button is toggled on.
      TOGGLE_BUTTON_SPRITE_ON_DOWN_OVER,
      /// The button cannot currently be pressed, and the button is toggled on.
      TOGGLE_BUTTON_SPRITE_ON_DISABLED,
      /// The number of sprites for this button.
      TOGGLE_BUTTON_SPRITE_COUNT
   };

   /// List of states for the current button based on interaction with the user.
   /// The mapping of states to sprites is not necessarily 1:1, and the actual mapping
   /// should be handled by SpriteIndexIdealGet().  Enabled and Toggled On/Off are
   /// separate from this state machine.
   enum ToggleButtonStates
   {
      /// The mouse is unpressed and not over the button, and the button is toggled off.
      TOGGLE_BUTTON_STATE_UP_OUT,
      /// The mouse is unpressed and over the button, and the button is toggled off.
      TOGGLE_BUTTON_STATE_UP_OVER,
      /// The mouse button was pressed over the button and dragged off, but still held.
      TOGGLE_BUTTON_STATE_DOWN_OUT,
      /// The mouse button is pressed over the button.
      TOGGLE_BUTTON_STATE_DOWN_OVER,
      /// The number of states for this button.
      TOGGLE_BUTTON_STATE_COUNT
   };

   /// Update the widget and its sprites.
   virtual void Update(unsigned int dt);

   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();
   /// Called when the cursor is over the widget on the current update, but it was
   /// not on the previous update.
   virtual void OnMouseOver();
   /// Called when an input focus finishes affirming this widget.
   virtual void OnKeypadClick();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return true if the button is toggled to on.
   bool ToggledOnCheck() { return toggledOn; }
   /// Set whether the button is toggled to on.
   virtual void ToggledOnSet(bool _toggledOn) { toggledOn = _toggledOn; }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }
   /// Return true if this widget type should play a mouseOver sound by default.
   virtual bool MouseOverSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play a press sound by default.
   virtual bool PressSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play an invalidPress sound by default.
   virtual bool InvalidPressSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play a click sound by default.
   virtual bool ClickSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Return true if this button should toggle between on and off when clicked.
   virtual bool ToggleOnClickCheck() { return toggleOnClick; }
   /// Set whether this button should toggle between on and off when clicked.
   virtual void ToggleOnClickSet(bool _toggleOnClick) { toggleOnClick = _toggleOnClick; }

   /// Return true if this button should default to toggling when clicked.
   virtual bool ToggleOnClickDefaultGet() { return true; }

   /// Type of the function that can receive the OnClick event.
   /// The bool is whether the button is now toggled on, the ToggleButtonWidget*
   /// is the button that was clicked, and the void* is the userData pointer.
   typedef void (*OnClickCallback)(bool, ToggleButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnClick event occurs.
   void OnClickUnregister(OnClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnMouseOver event.
   /// The ToggleButtonWidget* is the button that was moused over and the void*
   /// is the userData pointer.
   typedef void (*OnMouseOverCallback)(ToggleButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnMouseOver event occurs.
   /// For now, only one function can be registered per instance.
   void OnMouseOverRegister(OnMouseOverCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnMouseOver event occurs.
   void OnMouseOverUnregister(OnMouseOverCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnMouseOverRegister(const char* path, OnMouseOverCallback callback, void* userData = NULL);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnKeypadClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnKeypadClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnKeypadClick event occurs.
   void OnKeypadClickUnregister(OnClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnKeypadClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Convenience function for setting this to use the given callback and
   /// userData for both keypad and mouse clicks.
   void OnMouseAndKeypadClickRegister(OnClickCallback callback, void* userData = NULL);

   /// Convenience function for finding a ToggleButtonWidget by path and setting
   /// it to use the given callback and userData for both keypad and mouse
   /// clicks.  Return true if successful.
   static bool OnMouseAndKeypadClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ToggleButtonWidget(); }

protected:
   typedef ButtonWidget Inherited;

   /// Return the maximum number of sprites used for this button type.
   virtual int SpriteCountGet() const { return TOGGLE_BUTTON_SPRITE_COUNT; }
   /// Return an array of the names of the Lua tables for all the sprites that
   /// can be used by this button type.  The number of names is the same number
   /// returned by SpriteCountGet().
   virtual const char** SpriteTableNamesGet() const;
   /// Return a multidimensional array of arrays of sprite indices.  The outer array is of desired sprite
   /// indices.  Each inner array is an ordered list of sprite indices to try if
   /// there is no sprite loaded for the requested index.
   /// -1 means no substitution should be made, and no sprite should be shown.
   virtual const int* SpriteFallbackListGet() const;
   /// Return the first choice for the sprite that should be displayed, if it's loaded.
   virtual int SpriteIndexIdealGet();

   /// true if the button should toggle between on and off when clicked.
   bool toggleOnClick;
   /// true if the button is toggled to on.
   bool toggledOn;
   /// Function to be called for the OnClick event.
   OnClickCallback onClickCallback;
   /// Pointer to be passed to the OnClick callback.
   void* onClickUserData;
   /// Function to be called for the OnMouseOver event.
   OnMouseOverCallback onMouseOverCallback;
   /// Pointer to be passed to the OnMouseOver callback.
   void* onMouseOverUserData;
   /// Function to be called for the OnKeypadClick event.
   OnClickCallback onKeypadClickCallback;
   /// Pointer to be passed to the OnKeypadClick callback.
   void* onKeypadClickUserData;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__TOGGLEBUTTONWIDGET_H__
