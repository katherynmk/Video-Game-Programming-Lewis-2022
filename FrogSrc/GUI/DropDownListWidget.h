#ifndef __FROG__GUI__DROPDOWNLISTWIDGET_H__
#define __FROG__GUI__DROPDOWNLISTWIDGET_H__

#include "FrogMemory.h"
#include "HoldTracker.h"
#include "GUI/ContainerWidget.h"
#include "GUI/ToggleButtonWidget.h"

namespace Webfoot {

class DropDownListWidget;
class ScrollBoxWidget;
class DropDownListOpenButtonWidget;

//=============================================================================

/// This is the button that displays the current selection, and opens/closes
/// the list.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class DropDownListOpenButtonWidget : public ToggleButtonWidget
{
public:
   virtual ~DropDownListOpenButtonWidget() {}

   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

protected:
   typedef ToggleButtonWidget Inherited;

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }
};

//=============================================================================

/// This is the button type used for items that appear in the list.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class DropDownListItemButtonWidget : public ToggleButtonWidget
{
public:
   DropDownListItemButtonWidget();
   virtual ~DropDownListItemButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new DropDownListItemButtonWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, DropDownListWidget* _dropDownList);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Called when the left mouse button is released with the cursor over the widget.
   virtual void OnRelease();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

protected:
   typedef ToggleButtonWidget Inherited;

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// The DropDownList to which this item belongs.
   DropDownListWidget* dropDownList;
};

//=============================================================================

/// A DropDownListWidget is a list with a single currently selected item, and
/// the list is hidden when the user is not making a selection.  This list is
/// toggled between visibile and hidden by clicking a toggle button.  Clicking
/// an item in the list selects it and hides the list.
/// Call Init immediately after construction.
class DropDownListWidget : public ContainerWidget
{
public:
   DropDownListWidget();
   virtual ~DropDownListWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new DropDownListWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Return true if the list is showing.
   virtual bool OpenCheck() { return isOpen; }
   /// Set whether the list is showing.
   virtual void OpenSet(bool _isOpen);

   /// Set the selection to nothing.
   virtual void SelectionClear() { SelectionSet((DropDownListItemButtonWidget*)NULL, false); }
   /// Select the first item with the given name.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.  Return true if successful.
   virtual bool SelectionSet(const char* name, bool userMadeChange);
   /// Select the item at the given index.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.  Return true if successful.
   virtual bool SelectionSet(int index, bool userMadeChange);
   /// Select the given item.  Give NULL to clear the selection.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void SelectionSet(DropDownListItemButtonWidget* item, bool userMadeChange);
   /// Return the currently selected item or NULL if nothing is selected.
   virtual DropDownListItemButtonWidget* SelectionGet() { return selectedItem; }
   /// Return the name of the currently selected item or the NULL if
   /// nothing is selected.
   virtual const char* SelectionNameGet();
   /// Return the index of the currently selected item or -1 if nothing is
   /// selected.
   virtual int SelectionIndexGet();

   /// Make the given list item the highlighted item.
   virtual void HighlightedItemSet(DropDownListItemButtonWidget* item);
   /// Make it so no item is highlighted.
   virtual void HighlightedItemClear() { HighlightedItemSet((DropDownListItemButtonWidget*)NULL); }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }

   /// Called when a selection is made, regardless of whether anything changed.  This function
   /// will still be called even if both the old and new item have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection is undefined (clear),
   /// their name will be nil.  Call with 'userMadeSelection' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void OnSelection(DropDownListItemButtonWidget* newItem, DropDownListItemButtonWidget* oldItem, bool userMadeSelection);
   /// Called when there is a change in what item has been selected.  This function
   /// will still be called even if both the old and new item have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection is undefined (clear),
   /// their name will be nil.  Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void OnSelectionChange(DropDownListItemButtonWidget* newItem, DropDownListItemButtonWidget* oldItem, bool userMadeChange);
   /// Called when the DropDownList is either opened or closed.
   virtual void OnOpenChange(bool open);
   
   /// Make an item with the given name and label and add it to the bottom of the list.
   /// Use the text according to the specified 'textType'.
   /// Return true if successful.
   virtual bool ItemAddLast(const char* name, const char* text, TextType textType);
   
   /// Remove the first item with the given name.  Return true if successful.
   virtual bool ItemRemove(const char* name);
   /// Remove the given item from the list.
   virtual void ItemRemove(DropDownListItemButtonWidget* item);

   /// Return the number of items in the widget.
   virtual int ItemCountGet();

   /// Remove all items in the collection.
   virtual void ItemsRemoveAll();

   /// Return true if this widget should default to accepting input focus when
   /// pressed by a mouse.
   virtual bool FocusOnMousePressDefaultCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Calculate and return the number of items by which to skip when pressing the PageUp or PageDown keys.
   /// Scroll by one less than the number of items it takes to fill the clip area.
   /// If there are no items in the list, return 0.
   virtual int PageScrollLengthGet();

   /// Type of the function that can receive the OnSelection event.
   /// The first DropDownListItemButtonWidget* is the newly selected item,
   /// the second DropDownListItemButtonWidget* is the formerly selected item,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the DropDownListWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionCallback)(DropDownListItemButtonWidget*,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelection event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionRegister(OnSelectionCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelection event occurs.
   void OnSelectionUnregister(OnSelectionCallback callback);
   /// Convenience function for finding a DropDownListWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionRegister(const char* path, OnSelectionCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnSelectionChange event.
   /// The first DropDownListItemButtonWidget* is the newly selected item,
   /// the second DropDownListItemButtonWidget* is the formerly selected item,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the DropDownListWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionChangeCallback)(DropDownListItemButtonWidget*,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionChangeRegister(OnSelectionChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionChange event occurs.
   void OnSelectionChangeUnregister(OnSelectionChangeCallback callback);
   /// Convenience function for finding a DropDownListWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionChangeRegister(const char* path, OnSelectionChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnOpenChange event.
   /// The DropDownListWidget* is the widget that either opened or closed, the
   /// bool is true if the widget is now open, and the void* is the userData
   /// context pointer.
   typedef void (*OnOpenChangeCallback)(DropDownListWidget*, bool, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnOpenChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnOpenChangeRegister(OnOpenChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnOpenChange event occurs.
   void OnOpenChangeUnregister(OnOpenChangeCallback callback);
   /// Convenience function for finding a DropDownListWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnOpenChangeRegister(const char* path, OnOpenChangeCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) DropDownListWidget(); }

protected:
   typedef ContainerWidget Inherited;

   /// Create a new item button with the given name, and use the given text for
   /// its label.  Use the text according to the specified 'textType'.  Return
   /// null if unsuccessful.
   virtual DropDownListItemButtonWidget* ItemButtonCreate(const char* name,
      Widget* _parent, const char* text, TextType textType);

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh();

   /// Shift the highlight to the item with the index at the given offset
   /// from the currently highlighted item.  Do not wrap around.
   /// If no item is highlighted, highlight the first.
   virtual void HighlightShift(int offset);

   /// True if the list is showing.
   bool isOpen;

   /// Currently selected item
   DropDownListItemButtonWidget* selectedItem;
   /// Currently highlighted item
   DropDownListItemButtonWidget* highlightedItem;
   /// ScrollBox containing the widgets for the list of items.
   ScrollBoxWidget* scrollBox;
   /// The button that displays the current selection and opens/closes the list.
   DropDownListOpenButtonWidget* openButton;
   /// Specification used to initialize item buttons.
   JSONValue* itemButtonSpecifications;

   /// Function to be called for the OnSelection event.
   OnSelectionCallback onSelectionCallback;
   /// Pointer to be passed to the OnSelection callback.
   void* onSelectionUserData;
   /// Function to be called for the OnSelectionChange event.
   OnSelectionChangeCallback onSelectionChangeCallback;
   /// Pointer to be passed to the OnSelectionChange callback.
   void* onSelectionChangeUserData;
   /// Function to be called for the OnOpenChange event.
   OnOpenChangeCallback onOpenChangeCallback;
   /// Pointer to be passed to the OnOpenChange callback.
   void* onOpenChangeUserData;

   /// Determines when to respond to an up arrow key being held.
   HoldTracker upKeyHoldTracker;
   /// Determines when to respond to a down arrow key being held.
   HoldTracker downKeyHoldTracker;
   /// Determines when to respond to a page up key being held.
   HoldTracker pageUpKeyHoldTracker;
   /// Determines when to respond to a page down key being held.
   HoldTracker pageDownKeyHoldTracker;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__DROPDOWNLISTWIDGET_H__
