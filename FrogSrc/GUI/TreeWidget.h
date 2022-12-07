#ifndef __FROG__GUI__TREEWIDGET_H__
#define __FROG__GUI__TREEWIDGET_H__

#include "FrogMemory.h"
#include "HoldTracker.h"
#include "GUI/ContainerWidget.h"
#include "GUI/ToggleButtonWidget.h"

namespace Webfoot {

class TreeWidget;
class TreeItemExpandedButtonWidget;
class TreeItemButtonWidget;
class ScrollBoxWidget;
class SpriteWidget;
class TreeItemCellsContainerWidget;

//=============================================================================

/// This is the container that holds the items
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TreeItemWidget : public ContainerWidget
{
public:
   TreeItemWidget();
   virtual ~TreeItemWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TreeItemWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TreeWidget* _treeWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Add an item to the bottom of this items children.  Return the item if
   /// successful.
   virtual TreeItemWidget* ChildItemAddLast();
   /// Add an item at the given index in this item's children.  Return the item
   /// if successful.
   virtual TreeItemWidget* ChildItemAdd(int index);
   /// Return the given child item of this item.
   virtual TreeItemWidget* ChildItemGet(int index);
   /// Return the index of the given child item or -1 if it is not found.
   virtual int ChildItemIndexGet(TreeItemWidget* childItem);
   /// Return the number of child items in the tree.
   virtual int ChildItemCountGet();
   /// Remove all child items.
   virtual void ChildItemsClear();
   /// Remove the given child item.
   virtual void ChildItemRemove(TreeItemWidget* item);

   /// Return the item which is the parent of this item.  Return NULL if this
   /// item has no parent item.
   virtual TreeItemWidget* ItemParentGet();
   /// Return true if this item should be considered enabled for selection
   /// purposes.
   virtual bool ItemEnabledCheck();
   /// Return the previous item in the tree for a pre-order depth-first search.
   virtual TreeItemWidget* ItemPreviousGet();
   /// Return the next item in the tree for a pre-order depth-first search.
   virtual TreeItemWidget* ItemNextGet();

   /// Return a pointer to this item's item button.
   virtual TreeItemButtonWidget* ItemButtonGet() { return itemButton; }

   /// Return the text to be displayed for this item.
   virtual const char* TextGet();
   /// Set the text to be displayed for this item.
   virtual void TextSet(const char* _text);
   /// Return the key for theText for the string to be displayed for this item.
   /// Return NULL if a text key is not in use.
   virtual const char* TextKeyGet();
   /// Set the key for theText for the string to be displayed for this item.
   /// This will replace any existing text, including when called with NULL.
   virtual void TextKeySet(const char* _textKey);

   /// Return a pointer to this item's icon.
   virtual SpriteWidget* IconGet() { return iconWidget; }

   /// Set the sprite animation for this item's icon.
   // Use the sprite animation with the given name from the given file.
   // If no sprite file is specified, it will assume it is in the widget's layer's sprite file.
   virtual void IconAnimationSet(const char* resourceFilename, const char* animationName);

   /// Return a pointer to the container with the item's cells.
   virtual TreeItemCellsContainerWidget* CellsContainerGet() { return cellsContainer; }

   /// Return a pointer to the associated TreeWidget.
   virtual TreeWidget* TreeWidgetGet() { return treeWidget; }

   /// Return false if this item is explicitly collapsed.  Note this may still
   /// return false if an ancestor is collapsed.
   virtual bool ExpandedCheck() { return expanded; }
   /// Set whether this item is explicitly collapsed or expanded.
   virtual void ExpandedSet(bool _expanded, bool userMadeChange);

   /// Return the first item in this part of the tree which is both visible and
   /// enabled in a pre-order depth-first search.
   TreeItemWidget* ItemEnabledVisibleFirstGet();

protected:
   typedef ContainerWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }
   /// Helper function for positioning an item and its descendants.
   virtual float ItemPositionsRefreshHelper(int indentDepth, int* indentDepthMax, float indentSize);
   /// Refresh whether the expanded button should be visible.
   virtual void ExpandedButtonVisibleRefresh();
   /// Return the position of this item relative to the clip container.
   Point2F ItemPositionGet();
   /// Of this item's currently visible descendants, return the one which would
   /// appear lowest in the tree.
   TreeItemWidget* ItemDescendantVisibleLowestGet();

   /// The TreeWidget to which this item belongs.
   TreeWidget* treeWidget;
   /// Container with the item's TreeItemWidget children.
   ContainerWidget* childItemsContainer;
   /// The button for expanding and collapsing the display of this item's
   /// descendants.
   TreeItemExpandedButtonWidget* expandedButton;
   /// The button that sits underneath this item.  Clicking it selects the item.
   TreeItemButtonWidget* itemButton;
   /// Sprite widget for showing the icon for this item.
   SpriteWidget* iconWidget;
   /// Container for the widgets that make up the cells of the item.
   TreeItemCellsContainerWidget* cellsContainer;
   /// True if this item is opened (expanded) and false if it is closed
   /// (collapsed).
   bool expanded;

   friend class TreeWidget;
};

//=============================================================================

/// This is the part of an item that contains the widgets for the cells.
/// It helps route the paths when scripting those widgets.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TreeItemCellsContainerWidget : public ContainerWidget
{
public:
   TreeItemCellsContainerWidget();
   virtual ~TreeItemCellsContainerWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TreeItemWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TreeWidget* _treeWidget);
      /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Rather than using returning its own font, first try to return the font
   /// of the item button.  Failing that, return this widget's own font.
   /// This is necessary, because we want the cells to default to the right font
   /// for the current appearance of the item button.
   virtual Font* FontGet();

protected:
   typedef ContainerWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// The TreeWidget to which this widget belongs.
   TreeWidget* treeWidget;
};

//=============================================================================

/// A TreeItemButtonWidget is a toggle button that sits underneath an item.
/// When clicked, it tells the TreeWidget to update the selection.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TreeItemButtonWidget : public ToggleButtonWidget
{
public:
   TreeItemButtonWidget();
   virtual ~TreeItemButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TreeItemButtonWidget based on the specifications in the given JSONValue;
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TreeWidget* _treeWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Called when the left mouse button is pressed with the cursor over the widget.
   virtual void OnPress();
   virtual void OnClick();

   /// Called when the left mouse button is double-clicked with the cursor over the widget.
   virtual void OnDoubleClick();

   /// Return true if this button should default to toggling when clicked.
   virtual bool ToggleOnClickDefaultGet() { return false; }

protected:
   typedef ToggleButtonWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget type should play a mouseOver sound by default.
   virtual bool MouseOverSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a press sound by default.
   virtual bool PressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play an invalidPress sound by default.
   virtual bool InvalidPressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a click sound by default.
   virtual bool ClickSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// The TreeWidget to which this item belongs.
   TreeWidget* treeWidget;
};

//=============================================================================

/// A TreeItemExpandedButtonWidget is a toggle button that controls whether an
/// item's descendants are visible or hidden.  This is a helper widget, so
/// don't use it directly.
class TreeItemExpandedButtonWidget : public ToggleButtonWidget
{
public:
   virtual ~TreeItemExpandedButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TreeItemButtonWidget based on the specifications in the given JSONValue;
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   
   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Called when the left mouse button is pressed with the cursor over the widget.
   virtual void OnPress();
   virtual void OnClick();

   /// Return true if this button should default to toggling when clicked.
   virtual bool ToggleOnClickDefaultGet() { return false; }

protected:
   typedef ToggleButtonWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget type should play a mouseOver sound by default.
   virtual bool MouseOverSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a press sound by default.
   virtual bool PressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play an invalidPress sound by default.
   virtual bool InvalidPressSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget type should play a click sound by default.
   virtual bool ClickSoundEnabledDefaultCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }
};

//=============================================================================

/// A TreeWidget displays a scrollable area containing a hierarchy of items
/// where at most one item can be selected.
///
/// Specifying a TreeWidget in JSON is complicated.  Here is a general outline
/// of what should be defined...
///
/// Tree
///     |____ItemTemplate
///     |        |____ExpandedButton
///     |        |____Icon
///     |        |____ItemButton
///     |        |____CellContainer (Optional)
///     |                 |____CellWidget1
///     |                 |____CellWidget2
///     |                 |____...
///     |
///     |____ScrollBox
///              |____BackgroundSprite (Optional)
///              |____ClipContainer (instantiated items are placed here automatically)
///              |____HorizontalScrollbar
///              |____VerticalScrollbar
///
/// The details given in "ItemTemplate" are used as a template when adding
/// items to the tree.  Each item has multiple parts: ExpandedButton, Icon,
/// ItemButton, and CellContainer.  ExpandedButton is a ToggleButton for
/// controlling whether the item's descendants are currently hidden or
/// displayed.  If an item has no descendants, its ExpandedButton should be
/// hidden.  ItemButton is a ToggleButton that goes behind the item and
/// generally shows its text.  It is toggled on when the item is selected and
/// toggled off when it is not.  Clicking the ItemButton selects the item, so
/// its bounds should typically span the entire item.  Icon is a SpriteWidget
/// for showing the category of an item.  CellContainer is a Container for
/// additional child widgets of an item.  To keep cell widgets from blocking
/// the ItemButtons, be sure to use empty bounds for cells unless they need to
/// be clickable.  For example, if the cell is a button, feel free to use
/// non-empty bounds.  However, if it is a regular label, you should typically
/// use TextBounds rather than Bounds.  Positioning of items is based on their
/// bounds, so include Bounds when specifying the ItemTemplate.
///
/// The "ScrollBox" widget is a type of ScrollBox that contains everything
/// visible in the TreeWidget.  The ScrollBox's ClipContainer holds all the
/// items, and you should not specify its Bounds or Children.
class TreeWidget : public ContainerWidget
{
public:
   TreeWidget();
   virtual ~TreeWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TreeWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Set the selection to nothing.
   virtual void SelectionClear() { SelectionSet((TreeItemWidget*)NULL, false); }
   /// Select the given item.  Give NULL to clear the selection.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void SelectionSet(TreeItemWidget* item, bool userMadeChange);
   /// Return the currently selected item or NULL if nothing is selected.
   virtual TreeItemWidget* SelectionGet() { return selectedItem; }

   /// Called when there is a change in what item has been selected.  This function
   /// will still be called even if both the old and new item have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection
   /// is undefined (clear), their name will be nil.  Call with 'userMadeChange' 
   /// set to true if the user caused this call to be made by using the widget.
   virtual void OnSelectionChange(TreeItemWidget* newItem, TreeItemWidget* oldItem, bool userMadeChange);
   /// Called when a TreeItemWidget changes whether it's expanded.
   virtual void OnItemExpandedChange(TreeItemWidget* item, bool expanded, bool userMadeChange);

   /// Called when the enter key is pressed while the TreeWidget has focus.
   virtual void OnEnterPress();

   /// Add an item to the bottom of the top-level list.
   /// Return the item if successful.
   virtual TreeItemWidget* ItemAddLast();
   /// Add an item at the given top-level index.  Return the item if successful.
   virtual TreeItemWidget* ItemAdd(int index);
   /// Return the given top-level item in the tree.
   virtual TreeItemWidget* ItemGet(int index);
   /// Return the index of the top-level item or -1 if it is not found.
   virtual int ItemIndexGet(TreeItemWidget* item);
   /// Return the number of top-level items in the tree.
   virtual int ItemCountGet();
   /// Remove all items.
   virtual void ItemsClear();
   /// Remove the given top-level item from the tree.  To remove a node lower
   /// in the tree, use the ItemRemove method of its parent TreeItemWidget.
   virtual void ItemRemove(TreeItemWidget* item);

   /// Called when an item button is double clicked.  'itemWidget' indicates
   /// which was clicked.
   virtual void OnItemDoubleClick(TreeItemWidget* itemWidget);

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

   /// Set the size in pixels of an indentation for one level of the
   /// hierarchy of items.
   virtual void IndentSizeSet(float _indentSize);
   /// Return the size in pixels of an indentation for one level of the
   /// hierarchy of items.
   virtual float IndentSizeGet() { return indentSize; }

   /// Set whether items should be expanded by default.
   virtual void ItemsExpandedDefaultSet(bool _itemsExpandedDefault) { itemsExpandedDefault = _itemsExpandedDefault; }
   /// Return true if items should be expanded by default.
   virtual bool ItemsExpandedDefaultCheck() { return itemsExpandedDefault; }

   /// Return the first item in the tree which is both visible and enabled in a
   /// pre-order depth-first search.
   TreeItemWidget* ItemEnabledVisibleFirstGet();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) TreeWidget(); }

   /// Type of the function that can receive the OnEnter event.
   /// The TreeWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnEnterPressCallback)(TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnEnterPressRegister(OnEnterPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnEnterPress event occurs.
   void OnEnterPressUnregister(OnEnterPressCallback callback);
   /// Convenience function for finding a TreeWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnEnterPressRegister(const char* path, OnEnterPressCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnSelectionChange event.
   /// The first TreeItemWidget* is the newly selected item,
   /// the second TreeItemWidget* is the formerly selected item,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the TreeWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionChangeCallback)(TreeItemWidget*,
      TreeItemWidget*, bool, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionChangeRegister(OnSelectionChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionChange event occurs.
   void OnSelectionChangeUnregister(OnSelectionChangeCallback callback);
   /// Convenience function for finding a TreeWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionChangeRegister(const char* path, OnSelectionChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnItemDoubleClick event.
   /// The TreeItemWidget* is the item which was double-clicked, the
   /// TreeWidget* is the source of the event, and the void* is the userData
   /// context pointer.
   typedef void (*OnItemDoubleClickCallback)(TreeItemWidget*, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnItemDoubleClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnItemDoubleClickRegister(OnItemDoubleClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnItemDoubleClick event occurs.
   void OnItemDoubleClickUnregister(OnItemDoubleClickCallback callback);
   /// Convenience function for finding a TreeWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnItemDoubleClickRegister(const char* path, OnItemDoubleClickCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnItemExpandedChange event.
   /// The TreeItemWidget* is the item which was either expanded or collapsed,
   /// the first boolean is whether the item is now expanded, the second
   /// boolean is whether the user directly expanded or collapsed the item, the
   /// TreeWidget* is the source of the event, and the void* is the userData
   /// context pointer.
   typedef void (*OnItemExpandedChangeCallback)(TreeItemWidget*, bool, bool, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnItemExpandedChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnItemExpandedChangeRegister(OnItemExpandedChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnItemExpandedChange event
   /// occurs.
   void OnItemExpandedChangeUnregister(OnItemExpandedChangeCallback callback);

   /// Type of the function that can receive the OnItemDragAndDrop event.
   /// The first TreeItemWidget* is the item which was dragged, the second
   /// TreeItemWidget* is the new parent of the item being dragged, the first
   /// int is the new index of the item which was dragged, the third
   /// TreeItemWidget* is the old parent of the item which was dragged, the
   /// second int is the old index of the item which was dragged, the
   /// TreeWidget* is the source of the event, and the void* is the userData
   /// context pointer.
   /// (TreeItemWidget* item, TreeItemWidget* newParentItem, int newItemIndex,
   ///  TreeItemWidget* oldParentItem, int oldItemIndex,
   ///  TreeWidget* treeWidget, void* userData)
   typedef void (*OnItemDragAndDropCallback)(TreeItemWidget*, TreeItemWidget*, int, TreeItemWidget*, int, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnItemDragAndDrop event occurs.
   /// For now, only one function can be registered per instance.
   void OnItemDragAndDropRegister(OnItemDragAndDropCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnItemDragAndDropCheck event
   /// occurs.
   void OnItemDragAndDropUnregister(OnItemDragAndDropCallback callback);

   /// Same as OnItemDragAndDropRegister, but for before the move is made.
   void PreItemDragAndDropRegister(OnItemDragAndDropCallback callback, void* userData = NULL);
   /// Same as OnItemDragAndDropUnregister, but for before the move is made.
   void PreItemDragAndDropUnregister(OnItemDragAndDropCallback callback);

   /// Type of the function that can receive the CanDragAndDropItemCheck event.
   /// The TreeItemWidget* is the item to be dragged, the TreeWidget* is the
   /// source of the event, and the void* is the userData context pointer.
   /// Return true if the item should be allowed to be dragged.
   typedef bool (*CanDragAndDropItemCheckCallback)(TreeItemWidget*, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the CanDragAndDropItemCheck event occurs.
   /// For now, only one function can be registered per instance.
   void CanDragAndDropItemCheckRegister(CanDragAndDropItemCheckCallback callback, void* userData = NULL);
   /// Stop calling the given function when the CanDragAndDropItemCheck event
   /// occurs.
   void CanDragAndDropItemCheckUnregister(CanDragAndDropItemCheckCallback callback);

   /// Type of the function that can receive the CanItemContainItemsCheck
   /// event.  The TreeItemWidget* is the item into which items would be be
   /// dragged, the TreeWidget* is the source of the event, and the void* is
   /// the userData context pointer.  Return true if items can be dragged into
   /// the given item.
   typedef bool (*CanItemContainItemsCheckCallback)(TreeItemWidget*, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the CanItemContainItemsCheck event occurs.
   /// For now, only one function can be registered per instance.
   void CanItemContainItemsCheckRegister(CanItemContainItemsCheckCallback callback, void* userData = NULL);
   /// Stop calling the given function when the CanItemContainItemsCheck event
   /// occurs.
   void CanItemContainItemsCheckUnregister(CanItemContainItemsCheckCallback callback);

   /// Type of the function that can receive the CanDragAndDropItemToDestinationCheck
   /// event.  The first TreeItemWidget* is the item to be dragged, the second 
   /// TreeItemWidget* is the item into which the item would be be dragged, the
   /// int is the index to which the item would be dragged, the TreeWidget* is
   /// the source of the event, and the void* is the userData context pointer.
   /// Return true if the given item can be dragged to the given location.
   typedef bool (*CanDragAndDropItemToDestinationCheckCallback)(TreeItemWidget*, TreeItemWidget*, int, TreeWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the CanDragAndDropItemToDestinationCheck event occurs.
   /// For now, only one function can be registered per instance.
   void CanDragAndDropItemToDestinationCheckRegister(CanDragAndDropItemToDestinationCheckCallback callback, void* userData = NULL);
   /// Stop calling the given function when the CanDragAndDropItemToDestinationCheck
   /// event occurs.
   void CanDragAndDropItemToDestinationCheckUnregister(CanDragAndDropItemToDestinationCheckCallback callback);
   
protected:
   typedef ContainerWidget Inherited;

   /// Instantiate and return a new item.  Return null if unsuccessful.
   virtual TreeItemWidget* ItemCreate(Widget* _parent);

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh();
   
   /// Shift the selection up or down among the items which are currently
   /// visible due to items being expanded or collapsed.  If no item is
   /// selected, select the first.  Call with 'userMadeChange' set to true if
   /// the user caused this call to be made by using the widget.
   virtual void SelectionShift(int offset, bool userMadeChange);

   /// Called when an item is dragged and dropped.
   virtual void OnItemDragAndDrop(TreeItemWidget* item, TreeItemWidget* newParentItem, int newItemIndex);
   /// Called when the mouse position during a drag-and-drop causes scrolling.
   virtual void OnItemDragAndDropScroll(float scrollAmount);
   /// Return true if the given item can be dragged.
   bool CanDragAndDropItemCheck(TreeItemWidget* item);
   /// Return true if an item can be dragged into this one.
   bool CanItemContainItemsCheck(TreeItemWidget* item);
   /// Return true if the given item can be dragged to the given index of the
   /// given parent item.
   bool CanDragAndDropItemToDestinationCheck(TreeItemWidget* itemToDrag,
      TreeItemWidget* destinationParent, int destinationIndex);
   /// Given a relative position, determine where the given item would go for
   /// drag-and-drop purposes.
   bool DragAndDropDestinationGet(const Point2F& position, TreeItemWidget* itemToDrag,
      TreeItemWidget** destinationParent, int* destinationIndex,
      int* visualDestinationIndexOffset, bool* dragInto);
   /// The logic for drag and drop works partly in terms of the two items with the closest vertical midpoints to the indicated position.  This function helps find those two items.
   TreeItemWidget* DragAndDropDestinationGetHelper(TreeItemWidget* item,
      TreeItemWidget** aboveItem, float* searchStepsRemaining);
   /// Return the position in the clip container for the given child index of
   /// the given item.
   Point2F ItemPositionGetByParentAndIndex(TreeItemWidget* parentItem, int childIndex);
   /// Begin dragging the given item with the given mouse for drag-and-drop
   /// purposes.
   void ItemDragAndDropBegin(TreeItemWidget* _dragAndDropItem, GUIMouse* _dragAndDropGUIMouse);

   /// Currently selected item
   TreeItemWidget* selectedItem;
   /// ScrollBox containing the widgets for the list of items.
   ScrollBoxWidget* scrollBox;
   /// Widget containing the tree of items.
   ContainerWidget* itemContainer;
   /// Widget being dragged for drag-and-drop purposes.
   TreeItemWidget* dragAndDropItem;
   /// Mouse which is dragging the drag-and-drop item.
   GUIMouse* dragAndDropGUIMouse;
   /// True if the mouse has moved enough that the drag-and-drop can be treated
   /// as having started.
   bool dragAndDropThresholdReached;
   /// Position of the mouse when it was pressed for the current drag and drop.
   Point2F dragAndDropMousePressPosition;
   /// Widget for showing when a drag-and-drop destination is an item.
   SpriteWidget* dragAndDropIntoWidget;
   /// Widget for showing when a drag-and-drop destination is between
   /// or outside items.
   SpriteWidget* dragAndDropBetweenWidget;
   /// JSONValue* used to initialize new items.
   JSONValue* itemSpecifications;
   /// Size in pixels of an indentation for one level of the hierarchy of items.
   float indentSize;
   /// True if items should be expanded by default.
   bool itemsExpandedDefault;

   /// Function to be called for the OnEnterPress event.
   OnEnterPressCallback onEnterPressCallback;
   /// Pointer to be passed to the OnEnterPress callback.
   void* onEnterPressUserData;
   /// Function to be called for the OnSelectionChange event.
   OnSelectionChangeCallback onSelectionChangeCallback;
   /// Pointer to be passed to the OnSelectionChange callback.
   void* onSelectionChangeUserData;
   /// Function to be called for the OnItemDoubleClick event.
   OnItemDoubleClickCallback onItemDoubleClickCallback;
   /// Pointer to be passed to the OnItemDoubleClick callback.
   void* onItemDoubleClickUserData;
   /// Function to be called for the OnItemExpandedChange event.
   OnItemExpandedChangeCallback onItemExpandedChangeCallback;
   /// Pointer to be passed to the OnItemExpandedChange callback.
   void* onItemExpandedChangeUserData;
   /// Function to be called for the OnItemDragAndDrop event.
   OnItemDragAndDropCallback onItemDragAndDropCallback;
   /// Pointer to be passed to the OnItemDragAndDropCheck callback.
   void* onItemDragAndDropUserData;
   /// Function to be called for the PreItemDragAndDrop event.
   OnItemDragAndDropCallback preItemDragAndDropCallback;
   /// Pointer to be passed to the PreItemDragAndDropCheck callback.
   void* preItemDragAndDropUserData;
   /// Function to be called for the CanDragAndDropItemCheck event.
   CanDragAndDropItemCheckCallback canDragAndDropItemCheckCallback;
   /// Pointer to be passed to the CanDragAndDropItemCheck callback.
   void* canDragAndDropItemCheckUserData;
   /// Function to be called for the CanItemContainItemsCheck event.
   CanItemContainItemsCheckCallback canItemContainItemsCheckCallback;
   /// Pointer to be passed to the CanItemContainItemsCheck callback.
   void* canItemContainItemsCheckUserData;
   /// Function to be called for the CanDragAndDropItemToDestinationCheck event.
   CanDragAndDropItemToDestinationCheckCallback canDragAndDropItemToDestinationCheckCallback;
   /// Pointer to be passed to the CanDragAndDropItemToDestinationCheck callback.
   void* canDragAndDropItemToDestinationCheckUserData;

   /// Determines when to respond to an up arrow key being held.
   HoldTracker upKeyHoldTracker;
   /// Determines when to respond to a down arrow key being held.
   HoldTracker downKeyHoldTracker;
   /// Determines when to respond to a left arrow key being held.
   HoldTracker leftKeyHoldTracker;
   /// Determines when to respond to a right arrow key being held.
   HoldTracker rightKeyHoldTracker;
   /// Determines when to respond to a page up key being held.
   HoldTracker pageUpKeyHoldTracker;
   /// Determines when to respond to a page down key being held.
   HoldTracker pageDownKeyHoldTracker;
   /// Determines when to respond to an enter key being held.
   HoldTracker enterKeyHoldTracker;
   /// Determines when to scroll up as part of a drag-and-drop.
   HoldTracker dragAndDropScrollUpHoldTracker;
   /// Determines when to scroll down as part of a drag-and-drop.
   HoldTracker dragAndDropScrollDownHoldTracker;

   friend class TreeItemWidget;
   friend class TreeItemButtonWidget;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__TREEWIDGET_H__
