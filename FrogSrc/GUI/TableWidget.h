#ifndef __FROG__GUI__TABLEWIDGET_H__
#define __FROG__GUI__TABLEWIDGET_H__

#include "FrogMemory.h"
#include "HoldTracker.h"
#include "GUI/ScrollBoxWidget.h"
#include "GUI/ContainerWidget.h"
#include "GUI/PressButtonWidget.h"
#include "GUI/ToggleButtonWidget.h"

namespace Webfoot {

class TableWidget;
class TableRowButtonWidget;
class TableRowCellsContainerWidget;

//=============================================================================

/// This is the container that holds the rows
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TableRowWidget : public ContainerWidget
{
public:
   TableRowWidget();
   virtual ~TableRowWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableRowWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TableWidget* _tableWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return a pointer to this row's row button.
   virtual TableRowButtonWidget* RowButtonGet() { return rowButton; }

   /// Return a pointer to the container with the row's cells.
   virtual TableRowCellsContainerWidget* CellsContainerGet() { return cellsContainer; }

   /// Return a pointer to the associated TableWidget.
   virtual TableWidget* TableWidgetGet() { return tableWidget; }

protected:
   typedef ContainerWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// The TableWidget to which this row belongs.
   TableWidget* tableWidget;
   /// The button that sits underneath this row.  Clicking it selects the row.
   TableRowButtonWidget* rowButton;
   /// Container for the widgets that make up the cells of the row.
   TableRowCellsContainerWidget* cellsContainer;
};

//=============================================================================

/// This is the part of a row that contains the widgets for the cells.
/// It helps route the paths when scripting those widgets.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TableRowCellsContainerWidget : public ContainerWidget
{
public:
   TableRowCellsContainerWidget();
   virtual ~TableRowCellsContainerWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableRowWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TableWidget* _tableWidget);
      /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Rather than using returning its own font, first try to return the font
   /// of the row button.  Failing that, return this widget's own font.
   /// This is necessary, because we want the cells to default to the right font
   /// for the current appearance of the row button.
   virtual Font* FontGet();

   /// Recursively create and return a string that gives the name
   /// of the Lua table for this widget.  Leave runningLength
   /// at zero for the initial call.  In this case, the class will just
   /// use the callback table of its ancestor TableWidget.
   virtual char* EventHandlerTableBuild(size_t runningLength = 0);

protected:
   typedef ContainerWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// The TableWidget to which this widget belongs.
   TableWidget* tableWidget;
};

//=============================================================================

/// A TableRowButtonWidget is a toggle button that sits underneath a row.
/// When clicked, it tells the TableWidget to update the selection.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TableRowButtonWidget : public ToggleButtonWidget
{
public:
   TableRowButtonWidget();
   virtual ~TableRowButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableRowButtonWidget based on the specifications in the given JSONValue;
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, TableWidget* _tableWidget);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

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

   /// The TableWidget to which this row belongs.
   TableWidget* tableWidget;
};

//=============================================================================

/// TableColumnTitleButtonWidget is a button used to tell when the user
/// has clicked a column title to indicate they want to sort.
/// This is a helper widget, so only use it in a TableWidget.
/// Call Init immediately after construction.
class TableColumnTitleButtonWidget : public PressButtonWidget
{
public:
   TableColumnTitleButtonWidget();
   virtual ~TableColumnTitleButtonWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableScrollBoxWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Called when the left mouse button is pressed over the widget, and when it is
   /// released, it is again over the widget.
   virtual void OnClick();

   /// Determine some information about the ancestors of this widget.
   /// This is safe to call after this widget has been added to its ancestors' data structures.
   virtual void AncestorsCache();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) TableColumnTitleButtonWidget(); }

protected:
   typedef PressButtonWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Pointer to this button's ancestral TableWidget.
   TableWidget* tableWidget;
   /// Index of the column number of which this button is a title.
   int columnIndex;
   /// true if AncestorsCache has been called.
   bool ancestorsCached;
};

//=============================================================================

/// TableScrollBoxWidget is a ScrollBox that has been modified to include
/// column titles and highlights.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class TableScrollBoxWidget : public ScrollBoxWidget
{
public:
   TableScrollBoxWidget();
   virtual ~TableScrollBoxWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableScrollBoxWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return the container with the StateContainerWidgets for the columns.
   virtual ContainerWidget* ColumnsGet() { return columns; }

protected:
   typedef ScrollBoxWidget Inherited;

   /// Container for the widgets that make up the columns.
   ContainerWidget* columns;
};

//=============================================================================

/// A Table widget is a vertically scrollable table where at most one row can
/// be selected. The cells of the table are stored by row. If the user clicks
/// on the title of a column, the rows can be sorted by that column. The widths
/// of the columns cannot be adjusted by the user.
///
/// Specifying a Table widget in JSON is complicated. Here is a general outline
/// of what should be defined...
///
/// Table
///     |____RowTemplate
///     |        |____RowButton
///     |        |____CellContainer
///     |                 |____CellWidget1
///     |                 |____CellWidget2
///     |                 |____...
///     |
///     |____ScrollBox
///              |____BackgroundSprite
///              |____Columns (Container)
///              |         |____Column 1 StateContainer
///              |         |         |____ Unsorted Container
///              |         |         |         |____ TableColumnTitleButton
///              |         |         |         |____ Column Backdrop Sprite
///              |         |         |____ Ascending Container
///              |         |         |____ Descending Container
///              |         |
///              |         |____Column 2 StateContainer
///              |         |____...
///              |
///              |____ClipContainer (instantiated rows are placed here automatically)
///              |____VerticalScrollbar
///
/// The details given in "RowTemplate" are used as a template when adding rows to the
/// table. Each row has two parts: RowButton and CellContainer. RowButton is a
/// ToggleButton that goes behind the cells of the row. It is toggled on when
/// the row is selected and toggled off when it is not. Clicking the RowButton
/// selects the row, so its bounds should typically span the entire row.
/// CellContainer is a Container where each child is the cell for a given
/// column of the row. These children should be listed in order from left to
/// right. To keep cell widgets from blocking the RowButtons, be sure to use
/// empty bounds for cells unless they need to be clickable. For example,
/// if the cell is a button, feel free to use non-empty bounds. However, if it
/// is a regular label, you should typically use TextBounds rather than Bounds.
///
/// The "ScrollBox" widget is a type of ScrollBox that contains everything
/// visible in the TableWidget. The ScrollBox's ClipContainer holds all the
/// rows, and you should not specify its Bounds or Children. No
/// HorizontalScrollbar should be given for the ScrollBox.
///
/// The "Columns" widget is a Container where each child is a StateContainer
/// that corresponds to a column. The columns should be listed in order from
/// left to right. Each of these StateContainers has three states: Unsorted,
/// Ascending, and Descending. This is because you may want a column or its
/// title to appear differently based on whether you are sorting in ascending
/// order, sorting in descending order, or not sorting primarily by that
/// column. Each of these states is a Container. Unless this is a hidden
/// column, the list of children for each state should include a button with
/// the Type set to "TableColumnTitleButton". This is a special type of
/// PressButton that automatically causes the Table to be sorted by the
/// associated column when clicked. The text for this button should be set to
/// the title of the column. If you would like the column itself to have a
/// distinct appearance for each of the three states, you can add other child
/// widgets to each state. For example, you could add a Sprite widget with a
/// unique animation to each state. When designing the graphics for a Table
/// widget, be aware that the "Colunms" container is drawn behind the rows. 
class TableWidget : public ContainerWidget
{
public:
   TableWidget();
   virtual ~TableWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TableWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Set the selection to nothing.
   virtual void SelectionClear() { SelectionSet((TableRowWidget*)NULL, false); }
   /// Select the given row.  Return true if successful.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual bool SelectionSet(int rowIndex, bool userMadeChange);
   /// Select the given row.  Give NULL to clear the selection.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void SelectionSet(TableRowWidget* row, bool userMadeChange);
   /// Return the currently selected row or NULL if nothing is selected.
   virtual TableRowWidget* SelectionGet() { return selectedRow; }
   /// Return the number of the currently selected row or -1 if
   /// nothing is selected.
   virtual int SelectionIndexGet();

   /// Return the widget, if any, at the given row and column.
   virtual Widget* CellGet(int rowIndex, int columnIndex);

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }

   /// Called when a selection is made, regardless of whether anything changed.  This function
   /// will still be called even if both the old and new row have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection
   /// is undefined (clear), their name will be nil.  Call with 'userMadeChange' 
   /// set to true if the user caused this call to be made by using the widget.
   virtual void OnSelection(TableRowWidget* newItem, TableRowWidget* oldItem, bool userMadeChange);
   /// Called when there is a change in what row has been selected.  This function
   /// will still be called even if both the old and new row have the same name.
   /// This will also be called if the selection was previously an item, but is now
   /// cleared, or vice versa.  When either the old selection or the new selection
   /// is undefined (clear), their name will be nil.  Call with 'userMadeChange' 
   /// set to true if the user caused this call to be made by using the widget.
   virtual void OnSelectionChange(TableRowWidget* newItem, TableRowWidget* oldItem, bool userMadeChange);

   /// Called when the enter key is pressed while the TableWidget has focus.
   virtual void OnEnterPress();

   /// Add a row to the bottom of the list.
   /// Return true if successful.
   virtual bool RowAddLast();
   
   /// Remove the row with the index.  Return true if successful.
   virtual bool RowRemove(int rowIndex);
   /// Remove the given row from the list.
   virtual void RowRemove(TableRowWidget* row);
   /// Remove all rows.
   virtual void RowsRemoveAll();

   /// Return the row at the given index.
   virtual TableRowWidget* RowGet(int rowIndex);
   /// Return the index of the given row.  Return -1 if unsuccessful.
   virtual int RowIndexGet(TableRowWidget* row);

   /// Return the number of rows in the TableWidget.
   virtual int RowCountGet();

   /// Called when the title button for the given column is clicked
   virtual void OnTitleClick(int columnIndex);

   /// Sort the table by the given column.  If 'ascending' is true sort ascending.
   /// If false, sort descending.
   virtual void Sort(int columnIndex, bool ascending);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Sort(ContainerWidgetComparator* comparator) { assert(false); (void)comparator; }

   /// Set the appearance of the given column to the given state.
   /// Return true if successful.
   bool ColumnAppearanceSet(int columnIndex, const char* stateName);

   /// Return a pointer to the given descendant widget based on the given path.
   /// If the widget is not found, return NULL and show a warning.
   /// Assume we're only interested in the contents of clipContainer.
   /// In this case, if you want 'Item' on row 2, use
   /// "Row2.Item".  Here, the row indices are 1-based.
   virtual Widget* DescendantGetByPath(const char* path);

   /// Return the index of the row whose widgets last called an event script.
   /// Return -1 if they have not yet called one.
   virtual int EventRowIndexGet();

   /// Called when a row button is double clicked.  'rowIndex' indicates which
   /// was clicked.
   virtual void OnRowDoubleClick(TableRowWidget* rowWidget);

   /// Return true if this widget should default to accepting input focus when
   /// pressed by a mouse.
   virtual bool FocusOnMousePressDefaultCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Calculate and return the number of rows by which to skip when pressing the PageUp or PageDown keys.
   /// Scroll by one less than the number of items it takes to fill the clip area.
   /// If there are no items in the list, return 0.
   virtual int PageScrollLengthGet();

   /// Set whether pressing 'enter' should shift the selection down and wrap around if needed.
   virtual bool ShiftDownOnEnterCheck() { return shiftDownOnEnter; }
   /// Return true if pressing 'enter' should shift the selection down and wrap around if needed.
   virtual void ShiftDownOnEnterSet(bool _shiftDownOnEnter) { shiftDownOnEnter = _shiftDownOnEnter; }

   /// Return the ScrollBox containing the widgets for the list of items.
   TableScrollBoxWidget* ScrollBoxGet() { return scrollBox; }

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) TableWidget(); }

   /// Type of the function that can receive the OnEnter event.
   /// The TableWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnEnterPressCallback)(TableWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnEnterPressRegister(OnEnterPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnEnterPress event occurs.
   void OnEnterPressUnregister(OnEnterPressCallback callback);
   /// Convenience function for finding a TableWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnEnterPressRegister(const char* path, OnEnterPressCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnSelection event.
   /// The first TableRowWidget* is the newly selected row,
   /// the second TableRowWidget* is the formerly selected row,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the TableWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionCallback)(TableRowWidget*,
      TableRowWidget*, bool, TableWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelection event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionRegister(OnSelectionCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelection event occurs.
   void OnSelectionUnregister(OnSelectionCallback callback);
   /// Convenience function for finding a TableWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionRegister(const char* path, OnSelectionCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnSelectionChange event.
   /// The first TableRowWidget* is the newly selected row,
   /// the second TableRowWidget* is the formerly selected row,
   /// the bool is true if the event was triggered directly by the user
   /// interacting with the widget, the TableWidget* is the source
   /// of the event, and the void* is the userData context pointer.
   typedef void (*OnSelectionChangeCallback)(TableRowWidget*,
      TableRowWidget*, bool, TableWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnSelectionChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnSelectionChangeRegister(OnSelectionChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnSelectionChange event occurs.
   void OnSelectionChangeUnregister(OnSelectionChangeCallback callback);
   /// Convenience function for finding a TableWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnSelectionChangeRegister(const char* path, OnSelectionChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnRowDoubleClick event.
   /// The TableRowWidget* is the row which was double-clicked, the
   /// TableWidget* is the source of the event, and the void* is the userData
   /// context pointer.
   typedef void (*OnRowDoubleClickCallback)(TableRowWidget*, TableWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnRowDoubleClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnRowDoubleClickRegister(OnRowDoubleClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnRowDoubleClick event occurs.
   void OnRowDoubleClickUnregister(OnRowDoubleClickCallback callback);
   /// Convenience function for finding a TableWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnRowDoubleClickRegister(const char* path, OnRowDoubleClickCallback callback, void* userData = NULL);

protected:
   typedef ContainerWidget Inherited;

   /// Instantiate and return a new row.  Return null if unsuccessful.
   virtual TableRowWidget* RowCreate(Widget* _parent);

   /// Position all the item buttons based on their positions in the list.
   virtual void RowPositionsRefresh();

   /// Shift the selection to the item with the index at the given offset
   /// from the currently selected item.  Wrap around if 'wrapAround' is true.
   /// If no item is selected, select the first.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void SelectionShift(int offset, bool wrapAround, bool userMadeChange);

   /// True if pressing 'enter' should shift the selection down and wrap around if needed.
   bool shiftDownOnEnter;
   /// true if the last sort was ascending and false if it was descending.
   bool sortAscending;
   /// Index of the last column by which the table was sorted.
   /// -1 if it has not yet been sorted.
   int sortedColumnIndex;
   /// Currently selected item
   TableRowWidget* selectedRow;
   /// ScrollBox containing the widgets for the list of items.
   TableScrollBoxWidget* scrollBox;
   /// JSONValue* used to initialize new rows.
   JSONValue* rowSpecifications;

   /// Function to be called for the OnEnterPress event.
   OnEnterPressCallback onEnterPressCallback;
   /// Pointer to be passed to the OnEnterPress callback.
   void* onEnterPressUserData;
   /// Function to be called for the OnSelection event.
   OnSelectionCallback onSelectionCallback;
   /// Pointer to be passed to the OnSelection callback.
   void* onSelectionUserData;
   /// Function to be called for the OnSelectionChange event.
   OnSelectionChangeCallback onSelectionChangeCallback;
   /// Pointer to be passed to the OnSelectionChange callback.
   void* onSelectionChangeUserData;
   /// Function to be called for the OnRowDoubleClick event.
   OnRowDoubleClickCallback onRowDoubleClickCallback;
   /// Pointer to be passed to the OnRowDoubleClick callback.
   void* onRowDoubleClickUserData;

   /// Determines when to respond to an up arrow key being held.
   HoldTracker upKeyHoldTracker;
   /// Determines when to respond to a down arrow key being held.
   HoldTracker downKeyHoldTracker;
   /// Determines when to respond to a page up key being held.
   HoldTracker pageUpKeyHoldTracker;
   /// Determines when to respond to a page down key being held.
   HoldTracker pageDownKeyHoldTracker;
   /// Determines when to respond to an enter key being held.
   HoldTracker enterKeyHoldTracker;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__TABLEWIDGET_H__
