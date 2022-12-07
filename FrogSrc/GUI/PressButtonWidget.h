#ifndef __FROG__GUI__PRESSBUTTONWIDGET_H__
#define __FROG__GUI__PRESSBUTTONWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ButtonWidget.h"

namespace Webfoot {

class LayerWidget;

//=============================================================================

/// Widget for a button that, when released after being clicked, goes back to being unpressed.
/// Call Init immediately after construction.
class PressButtonWidget : public ButtonWidget
{
public:
   virtual ~PressButtonWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   /// Call this or the other Init immediately after construction.
   virtual void Init(const char* _name, Widget* _parent);
   /// Call this immediately after construction.
   /// Initialize a new PressButtonWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// List of sprite indices for this button type based.
   /// Each index corresponds to a space for a sprite in the 'sprites' vector.
   enum PressButtonSprites
   {
      /// The mouse is unpressed and not over the button.
      PRESS_BUTTON_SPRITE_UP_OUT,
      /// The mouse is unpressed and over the button.
      PRESS_BUTTON_SPRITE_UP_OVER,
      /// The mouse button was pressed over the button and dragged off, but still held.
      PRESS_BUTTON_SPRITE_DOWN_OUT,
      /// The mouse button is pressed over the button.
      PRESS_BUTTON_SPRITE_DOWN_OVER,
      /// The button cannot currently be pressed
      PRESS_BUTTON_SPRITE_DISABLED,
      /// The number of sprites for this button.
      PRESS_BUTTON_SPRITE_COUNT
   };

   /// List of states for the current button based on interaction with the user.
   /// The mapping of states to sprites is not necessarily 1:1, and the actual mapping
   /// should be handled by SpriteIndexIdealGet().
   enum PressButtonStates
   {
      /// The mouse is unpressed and not over the button.
      PRESS_BUTTON_STATE_UP_OUT,
      /// The mouse is unpressed and over the button.
      PRESS_BUTTON_STATE_UP_OVER,
      /// The mouse button was pressed over the button and dragged off, but still held.
      PRESS_BUTTON_STATE_DOWN_OUT,
      /// The mouse button is pressed over the button.
      PRESS_BUTTON_STATE_DOWN_OVER,
      /// The number of states for this button.
      PRESS_BUTTON_STATE_COUNT
   };

   /// Update the widget and its sprites.
   virtual void Update(unsigned int dt);

   /// Called when the left mouse button is pressed with the cursor over the widget.
   virtual void OnPress();
   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();
   virtual void OnHold(int consecutiveCalls);
   /// Called when the right mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnRightClick();
   /// Called when the cursor is over the widget on the current update, but it was
   /// not on the previous update.
   virtual void OnMouseOver();
   /// Called when an input focus begins to affirm this widget.
   virtual void OnKeypadPress();
   /// Called when an input focus finishes affirming this widget.
   virtual void OnKeypadClick();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

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

   /// Type of the function that can receive the OnPress event.
   /// The PressButtonWidget* is the button that was clicked, and the void*
   /// is the userData pointer.
   typedef void (*OnPressCallback)(PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnPressRegister(OnPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnPress event occurs.
   void OnPressUnregister(OnPressCallback callback);
   /// Convenience function for finding a PressButtonWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnPressRegister(const char* path, OnPressCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnClick event.
   /// The PressButtonWidget* is the button that was clicked, and the void*
   /// is the userData pointer.
   typedef void (*OnClickCallback)(PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnClick event occurs.
   void OnClickUnregister(OnClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnHold event.
   /// The int is the number of consecutive OnHold events, the
   /// PressButtonWidget* is the button that was held, and the void* is the
   /// userData pointer.
   typedef void (*OnHoldCallback)(int, PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnHold event occurs.
   /// For now, only one function can be registered per instance.
   void OnHoldRegister(OnHoldCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnHold event occurs.
   void OnHoldUnregister(OnHoldCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnHoldRegister(const char* path, OnHoldCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnRightClick event.
   /// The PressButtonWidget* is the button that was right-clicked, and the void*
   /// is the userData pointer.
   typedef void (*OnRightClickCallback)(PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnRightClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnRightClickRegister(OnRightClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnRightClick event occurs.
   void OnRightClickUnregister(OnRightClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnRightClickRegister(const char* path, OnRightClickCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnMouseOver event.
   /// The PressButtonWidget* is the button that was moused over, and the void*
   /// is the userData pointer.
   typedef void (*OnMouseOverCallback)(PressButtonWidget*, void*);
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
   /// pointer when the OnKeypadPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnKeypadPressRegister(OnPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnKeypadPress event occurs.
   void OnKeypadPressUnregister(OnPressCallback callback);
   /// Convenience function for finding a PressButtonWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnKeypadPressRegister(const char* path, OnPressCallback callback, void* userData = NULL);

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnKeypadClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnKeypadClickRegister(OnClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnKeypadClick event occurs.
   void OnKeypadClickUnregister(OnClickCallback callback);
   /// Convenience function for finding a PressButtonWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnKeypadClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Convenience function for setting this to use the given callback and
   /// userData for both keypad and mouse clicks.
   void OnMouseAndKeypadClickRegister(OnClickCallback callback, void* userData = NULL);

   /// Convenience function for finding a PressButtonWidget by path and setting
   /// it to use the given callback and userData for both keypad and mouse
   /// presses.  Return true if successful.
   static bool OnMouseAndKeypadPressRegister(const char* path, OnPressCallback callback, void* userData = NULL);
   /// Convenience function for finding a PressButtonWidget by path and setting
   /// it to use the given callback and userData for both keypad and mouse
   /// clicks.  Return true if successful.
   static bool OnMouseAndKeypadClickRegister(const char* path, OnClickCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) PressButtonWidget(); }

protected:
   typedef ButtonWidget Inherited;

   /// Return the maximum number of sprites used for this button type.
   virtual int SpriteCountGet() const { return PRESS_BUTTON_SPRITE_COUNT; }
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

   /// Function to be called for the OnPress event.
   OnPressCallback onPressCallback;
   /// Pointer to be passed to the OnPress callback.
   void* onPressUserData;
   /// Function to be called for the OnClick event.
   OnClickCallback onClickCallback;
   /// Pointer to be passed to the OnClick callback.
   void* onClickUserData;
   /// Function to be called for the OnHold event.
   OnHoldCallback onHoldCallback;
   /// Pointer to be passed to the OnHold callback.
   void* onHoldUserData;
   /// Function to be called for the OnRightClick event.
   OnClickCallback onRightClickCallback;
   /// Pointer to be passed to the OnClick callback.
   void* onRightClickUserData;
   /// Function to be called for the OnMouseOver event.
   OnMouseOverCallback onMouseOverCallback;
   /// Pointer to be passed to the OnMouseOver callback.
   void* onMouseOverUserData;
   /// Function to be called for the OnKeypadPress event.
   OnPressCallback onKeypadPressCallback;
   /// Pointer to be passed to the OnKeypadPress callback.
   void* onKeypadPressUserData;
   /// Function to be called for the OnKeypadClick event.
   OnClickCallback onKeypadClickCallback;
   /// Pointer to be passed to the OnKeypadClick callback.
   void* onKeypadClickUserData;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__PRESSBUTTONWIDGET_H__
