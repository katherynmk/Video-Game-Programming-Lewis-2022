#ifndef __FROG__GUI__SELECTORWIDGET_H__
#define __FROG__GUI__SELECTORWIDGET_H__

#include "FrogMemory.h"
#include "GUI/RadioButtonWidget.h"
#include "GUI/PressButtonWidget.h"

namespace Webfoot {

class SelectorItemWidget;
class TextDocumentWidget;
class JSONValue;

/// Key for the items with which to populate the list.
#define GUI_SELECTOR_WIDGET_ITEMS_KEY "Items"
/// Key for the keypad direction that corresponds to a shift to the next item.
#define SELECTOR_WIDGET_KEYPAD_DIRECTION_NEXT_KEY "KeypadDirectionNext"
/// Key for the keypad direction that corresponds to a shift to the previous
/// item.
#define SELECTOR_WIDGET_KEYPAD_DIRECTION_PREVIOUS_KEY "KeypadDirectionPrevious"
/// Widget modifiers with this tag should be synchronized to the selection
/// rather than played normally.  This may not be supported for all forms of
/// SelectorWidget or WidgetModifier.
#define SELECTOR_WIDGET_SELECTION_SYNC_MODIFIER_TAG "SelectionSync"
/// Time in milliseconds within a SelectionSync WidgetModifier animation that
/// should be used for the currently selected item.  Each second of time
/// represents a difference of 1 item index.
#define SELECTOR_WIDGET_SELECTION_SYNC_ITEM_SELECTED_TIME_KEY "SelectionSyncItemSelectedTime"
#define SELECTOR_WIDGET_SELECTION_SYNC_ITEM_SELECTED_TIME_DEFAULT 0

//=============================================================================

/// SelectorWidget is an abstract class used as a base for certain other widgets
/// for choosing an item from a list.  They may have buttons for incrementing
/// or decrementing the selection, as well as a TextDocumentWidget to show a
/// detailed explanation of the currently selected item.
///
/// Each implementation of SelectorWidget is responsible for loading the Items
/// array when the selector is ready.
class SelectorWidget : public ContainerWidget
{
public:
   SelectorWidget();
   virtual ~SelectorWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new SelectorWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget.
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set the selection to nothing.
   virtual void SelectionClear() { SelectionSet((SelectorItemWidget*)NULL, false); }
   /// Select the item with the given index.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.  Return true if successful.
   virtual bool SelectionSet(int index, bool userMadeChange);
   /// Select the first item with the given name.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.  Return true if successful.
   virtual bool SelectionSet(const char* name, bool userMadeChange);
   /// Select the given item.  Give NULL to clear the selection.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.
   virtual void SelectionSet(SelectorItemWidget* item, bool userMadeChange);
   /// Return the currently selected item or NULL if nothing is selected.
   virtual SelectorItemWidget* SelectionGet() { return selectedItem; }
   /// Return the name of the currently selected item or the NULL if
   /// nothing is selected.
   virtual const char* SelectionNameGet();
   /// Return the index of the currently selected item of -1 if nothing is
   /// selected.
   virtual int SelectionIndexGet();
   /// Shift the selection to the item with the index at the given offset
   /// from the currently selected item.  Wrap around if needed.
   /// If no item is selected, selected the first.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.
   virtual void SelectionShift(int offset, bool userMadeChange);

   /// Return the keypad direction that corresponds to a shift to the next
   /// item.
   Direction KeypadDirectionNextGet() { return keypadDirectionNext; }
   /// Set the keypad direction that corresponds to a shift to the next item.
   void KeypadDirectionNextSet(Direction _direction) { keypadDirectionNext = _direction; }
   /// Return the keypad direction that corresponds to a shift to the previous
   /// item.
   Direction KeypadDirectionPreviousGet() { return keypadDirectionPrevious; }
   /// Set the keypad direction that corresponds to a shift to the previous
   /// item.
   void KeypadDirectionPreviousSet(Direction _direction) { keypadDirectionPrevious = _direction; }

   /// Move the items into their target positions so that they don't need to
   /// move any farther to get into place.
   virtual void MovementFinish() = 0;

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Called when there is a change in what item has been selected.  This function
   /// will still be called even if both the old and new item have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection is undefined (clear),
   /// their name will be nil.  Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void OnSelectionChange(SelectorItemWidget* newItem, SelectorItemWidget* oldItem, bool userMadeChange);

   /// Add an item to the bottom of the list using the given specifications.
   virtual void ItemAddLast(JSONValue* itemSpecifications);
   /// Add the given array of items to the selector.
   virtual void ItemsAddLast(JSONValue* itemArray);
   /// Remove the first item with the given name.  Return true if successful.
   virtual bool ItemRemove(const char* name);
   /// Remove the given item from the list.
   virtual void ItemRemove(SelectorItemWidget* item);
   /// Remove all items from the list.
   virtual void ItemsRemoveAll();
   /// Return the number of items.
   virtual int ItemCountGet();

   /// Type of the function that can receive the OnSelectionChange event.
   /// The first SelectorItemWidget* is the newly selected item,
   /// the second SelectorItemWidget* is the formerly selected item,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the SelectorWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionChangeCallback)(SelectorItemWidget*,
      SelectorItemWidget*, bool, SelectorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionChangeRegister(OnSelectionChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionChange event occurs.
   void OnSelectionChangeUnregister(OnSelectionChangeCallback callback);
   /// Convenience function for finding a SelectorWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionChangeRegister(const char* path, OnSelectionChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnSelectorItemClick event.
   /// The bool is whether the button was already selected when it was clicked,
   /// the SelectorItemWidget* is the button that was clicked, and the void* is
   /// the userData pointer.
   typedef void (*OnSelectorItemClickCallback)(bool, SelectorItemWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnClick event occurs for a SelectorItemWidget.
   /// For now, only one function can be registered per instance.
   void OnSelectorItemClickRegister(OnSelectorItemClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnClick event occurs for a
   /// SelectorItemWidget.
   void OnSelectorItemClickUnregister(OnSelectorItemClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectorItemClickRegister(const char* path, OnSelectorItemClickCallback callback, void* userData = NULL);

   /// Called to notify this widget when the detail text of the given item is set.
   void OnItemDetailTextSet(SelectorItemWidget* item);

   /// Return the optional TextDocumentWidget for displaying detailed
   /// information about the currently selected item.
   TextDocumentWidget* DetailTextWidgetGet() { return detailTextWidget; }
   /// Return the button for moving the selection forward.
   PressButtonWidget* NextButtonGet() { return nextButton; }
   /// Return the button for moving the selection backward.
   PressButtonWidget* PreviousButtonGet() { return previousButton; }

   /// Return true if the selection is allowed to wrap around at the ends.
   bool WrapAroundCheck() { return wrapAround; }

   /// Called when a mouse wheel is scrolled over this widget.
   virtual void OnMouseScroll(int scrollAmount);
   /// Return true if this widget should respond to mouse wheel scrolling.
   virtual bool CanHandleMouseScroll() { return true; }

protected:
   typedef ContainerWidget Inherited;

   /// Create a new item widget according to the given specifications.  Return
   /// NULL if unsuccessful.
   virtual SelectorItemWidget* ItemCreate(const char* _name, Widget* _parent,
      JSONValue* _itemSpecifications);

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh() = 0;

   /// Refresh the detail text to display for the currently selected item.
   virtual void DetailTextRefresh();

   /// Return a pointer to the RadioButtonContainerWidget for this selector's
   /// items.
   virtual RadioButtonContainerWidget* ItemContainerGet() = 0;

   /// Currently selected item
   SelectorItemWidget* selectedItem;
   /// Specification used to initialize item widgets.
   JSONValue* itemTemplate;
   /// Optional TextDocumentWidget for displaying detailed information about
   /// the currently selected item.
   TextDocumentWidget* detailTextWidget;
   /// Button for moving the selection forward.
   PressButtonWidget* nextButton;
   /// Button for moving the selection backward.
   PressButtonWidget* previousButton;
   /// True if selection is allowed to wrap around.
   bool wrapAround;
   /// Accumulates small mouse scrolls so that slow scrolling can eventually
   /// cause a shift.
   float mouseScrollAccumulator;
   /// Keypad direction that corresponds to a shift to the next item.
   Direction keypadDirectionNext;
   /// Keypad direction that corresponds to a shift to the previous item.
   Direction keypadDirectionPrevious;
   /// Time used in SelectionSyncItem WidgetModifiers for the selected item.
   unsigned int selectionSyncItemSelectedTime;

   /// Function to be called for the OnSelectionChange event.
   OnSelectionChangeCallback onSelectionChangeCallback;
   /// Pointer to be passed to the OnSelectionChange callback.
   void* onSelectionChangeUserData;

   /// Function to be called for the OnClick event of a SelectorItemWidget.
   OnSelectorItemClickCallback onSelectorItemClickCallback;
   /// Pointer to be passed to the onSelectorItemClickCallback callback.
   void* onSelectorItemClickCallbackUserData;

   friend class SelectorItemWidget;
};

//=============================================================================

/// This is the widget type used for items that appear in the selector.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class SelectorItemWidget : public RadioButtonWidget
{
public:
   SelectorItemWidget();
   virtual ~SelectorItemWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new SelectorItemWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, SelectorWidget* _selectorWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Clean up the widget.
   virtual void Deinit();

   virtual void OnClick();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set whether the button is toggled to on.
   virtual void ToggledOnSet(bool _toggledOn);

   /// Set the text to be used in a TextDocumentWidget for a detailed
   /// explanation of this item.  Treat the text according to 'detailTextType'.
   void DetailTextSet(const char* _detailText, TextType _detailTextType);

   /// Return the text to be used in a TextDocumentWidget for a detailed explanation
   /// of this item.  Text should be used accoding to the value returned by
   /// DetailTextTypeGet().
   const char* DetailTextGet() { return detailText; }
   /// Return how the text returned by DetailTextGet() should be interpreted.
   TextType DetailTextTypeGet() { return detailTextType; }

   /// Return the SelectorWidget to which this item belongs.
   SelectorWidget* SelectorWidgetGet() { return selectorWidget; }

   /// Set the time of SelectionSync WidgetModifiers for this item.
   void SelectionSyncWidgetModifiersTimeSet(unsigned int time);

protected:
   typedef RadioButtonWidget Inherited;

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// The SelectorWidget to which this item belongs.
   SelectorWidget* selectorWidget;

   /// Text to be shown as a detailed explanation of this item according to
   /// 'detailTextType'.
   const char* detailText;
   /// How the 'detailText' should be interpreted
   TextType detailTextType;
   /// WidgetModifiers which should have their time synchronized with the
   /// selection.
   Table<WidgetModifier*> selectionSyncModifiers;

   friend class SelectorWidget;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__SELECTORWIDGET_H__
