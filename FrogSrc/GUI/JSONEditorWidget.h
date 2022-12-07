#ifndef __FROG__GUI__JSONEDITORWIDGET_H__
#define __FROG__GUI__JSONEDITORWIDGET_H__

#include "FrogMemory.h"
#include "GUI/TableWidget.h"

namespace Webfoot {

class TextEntryWidget;
class DropDownListItemButtonWidget;
class DropDownListItemButtonWidget;
class DropDownListWidget;

/// Part of a schema that specifies the custom types.
#define JSON_EDITOR_WIDGET_SCHEMA_TYPES_KEY "Types"
/// Base type for an item.
#define JSON_EDITOR_WIDGET_SCHEMA_TYPE_TYPE_KEY "Type"
/// Base type name for a JSON string.
#define JSON_EDITOR_WIDGET_SCHEMA_TYPE_STRING_TYPE_NAME "String"
/// List of possible values for a given data type.
#define JSON_EDITOR_WIDGET_SCHEMA_TYPE_VALUES_KEY "Values"

//==============================================================================

class JSONEditorWidget : public TableWidget
{
public:
   JSONEditorWidget();
   virtual ~JSONEditorWidget() {}

   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Set the characteristics of the values.  The schema must be set before
   /// setting the data.
   void SchemaSet(JSONValue* _schema);
   /// Return the specs for the given type name using the current schema.
   JSONValue* SchemaTypeSpecificationsGet(const char* typeName);
   /// Set the editor to use the given data.  The schema must be set before
   /// setting the data.  '_data' must be either a JSON object or a JSON array.
   /// '_dataInherited' gives the data which is inherited rather than explicit.
   void DataSet(JSONValue* _data, const char* _dataTypeName, JSONValue* _dataInherited = NULL);
   /// Return the data currently being edited.
   JSONValue* DataGet();
   /// Return the root of the data being edited.
   JSONValue* DataRootGet();
   /// Return the data which is inherited rather than explicit.
   JSONValue* DataInheritedGet();
   /// Return the type of the current data to display and edit.
   const char* DataTypeNameGet();
   /// Return the type of the root of the data to display and edit.
   const char* DataRootTypeNameGet();

   /// Assuming the current data is for a JSON object, begin editing the child
   /// object or array with the given name.
   void DataPush(const char* childName);
   /// Assuming the current data is for a JSON array, begin editing the child
   /// object or array at the given index.
   void DataPush(int childIndex);
   /// Go up one level in the data hierarchy.
   void DataPop();

   /// Rebuild the cells of the table for the current data.
   virtual void Refresh();

   /// See 'OnValueChangeCallback'.
   virtual void OnValueChange(int rowIndex);
   /// See 'OnBrowserRequestCallback'
   virtual void OnBrowserRequest(int rowIndex);
   /// See 'OnRootPopCallback'.
   virtual void OnRootPop();

   /// Return the key associated with the given row.  Return NULL if
   /// unsuccessful.
   const char* RowKeyGet(int rowIndex);
   /// Return the type of the data on the given row.
   const char* RowTypeNameGet(int rowIndex);

   /// Type of the function that can receive the OnValueChange event.
   /// Depending on the type of value being edited, this event can have false
   /// positives.  In other words, this may be called even if the value has not
   /// actually changed.  The JSONEditorWidget* is the widget where the edit
   /// took place, the integer is the index of the row where the value was
   /// value was changed, and the void* is the userData context pointer.
   typedef void(*OnValueChangeCallback)(JSONEditorWidget*, int, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnValueChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnValueChangeRegister(OnValueChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnValueChange event occurs.
   void OnValueChangeUnregister(OnValueChangeCallback callback);
   /// Convenience function for finding a JSONEditorWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnValueChangeRegister(const char* path, OnValueChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnBrowserRequest event.
   /// The JSONEditorWidget* is the editor widget with the item for which the
   /// broswer was requested, the integer is the index of the row requesting a
   /// browser, and the void* is the userData context pointer.
   typedef void(*OnBrowserRequestCallback)(JSONEditorWidget*, int, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnBrowserRequest event occurs.
   /// For now, only one function can be registered per instance.
   void OnBrowserRequestRegister(OnBrowserRequestCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnBrowserRequest event occurs.
   void OnBrowserRequestUnregister(OnBrowserRequestCallback callback);
   /// Convenience function for finding a JSONEditorWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnBrowserRequestRegister(const char* path, OnBrowserRequestCallback callback, void* userData = NULL);

   /// Type of function that can receive the OnRootPop event.
   typedef void(*OnRootPopCallback)(JSONEditorWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnRootPop event occurs.
   /// For now, only one function can be registered per instance.
   void OnRootPopRegister(OnRootPopCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnRootPop event occurs.
   void OnRootPopUnregister(OnRootPopCallback callback);
   /// Convenience function for finding a JSONEditorWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnRootPopRegister(const char* path, OnRootPopCallback callback, void* userData = NULL);

   /// Sort the table by the given column.  If 'ascending' is true sort ascending.
   /// If false, sort descending.  Only the first column can be sorted.
   virtual void Sort(int columnIndex, bool ascending);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) JSONEditorWidget(); }

   typedef TableWidget Inherited;

protected:
   /// Clear the stack for hierarchical editing.
   void DataStackClear();
   /// Begin showing the given data.  Return true if successful.
   bool DataSetHelper();
   /// Add a widget to represent the given item.
   void ItemWidgetsAdd(TableRowWidget* rowWidget, const char* itemKey, int itemIndex, JSONValue* itemSpecs);
   /// Helper function for printing a number to a string.
   static void NumberToString(double value, char* buffer, size_t bufferSize, void* userData);
   /// Helper function to gather information based on the callback's userdata.
   static bool ItemValueSetHelper(void* userData, JSONEditorWidget** jsonEditorWidget,
      const char** itemKey, int* itemIndex, int* rowIndex, JSONValue** defaultValue);
   /// Called when a read-only key or value is pressed.
   static void ItemReadOnlyOnPress(PressButtonWidget*, void* userData);
   /// Called when an item's value's TextEntryWidget gains focus.
   static void ItemTextEntryOnFocusGain(TextEntryWidget* widget, void* userData);
   /// Called when DropDownListWidget for an item is either opened or closed.
   static void ItemDropDownListOnOpenChange(DropDownListWidget*, bool open, void* userData);
   /// Called when enter is pressed when an item string widget has the keyboard
   /// focus.
   static void ItemStringOnEnterPress(TextEntryWidget*, void*);
   /// Called when a widget for an editable string loses the keyboard focus.
   static void ItemStringOnFocusLose(TextEntryWidget* widget, void* userData);
   /// Called when a widget for an editable number loses the keyboard focus.
   static void ItemStringNumberOnFocusLose(TextEntryWidget* widget, void* userData);
   /// Called when a string enumeration selection changes.
   static void ItemStringOnSelectionChange(DropDownListItemButtonWidget* newSelection,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void* userData);
   /// Called when a numerical enumeration selection changes.
   static void ItemNumberOnSelectionChange(DropDownListItemButtonWidget* newSelection,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void* userData);
   /// Called when a boolean selection changes.
   static void ItemBooleanOnSelectionChange(DropDownListItemButtonWidget* newSelection,
      DropDownListItemButtonWidget*, bool, DropDownListWidget*, void* userData);
   /// Called when an edit button is clicked.
   static void ItemEditButtonOnClick(PressButtonWidget*, void* userData);
   /// Called when an item browser button is clicked.
   static void ItemBrowserButtonOnClick(PressButtonWidget*, void* userData);
   /// Called when the back button is clicked.
   static void BackButtonOnClick(PressButtonWidget*, void* userData);
   
   /// Characteristics of the values.
   JSONValue* schema;
   
   /// JSON object containing the schema's data types.
   JSONValue* types;
   /// Template for read-only item keys.
   JSONValue* itemKeyReadOnlyTemplate;
   /// Template for writable strings.
   JSONValue* itemStringTemplate;
   /// Template for read-only strings.
   JSONValue* itemStringReadOnlyTemplate;
   /// Template for edit buttons.
   JSONValue* itemEditButtonTemplate;
   /// Template for browser buttons.
   JSONValue* itemBrowserButtonTemplate;
   /// Template for items with an enumeration value.
   JSONValue* itemEnumerationTemplate;

   /// Function to be called for the OnValueChange event.
   OnValueChangeCallback onValueChangeCallback;
   /// Pointer to be passed to the OnValueChange callback.
   void* onValueChangeUserData;
   /// Function to be called for the OnBrowserRequest event.
   OnBrowserRequestCallback onBrowserRequestCallback;
   /// Pointer to be passed to the OnBrowserRequest callback.
   void* onBrowserRequestUserData;
   /// Function to be called for the OnRootPop event.
   OnRootPopCallback onRootPopCallback;
   /// Pointer to be passed to the OnRootPop callback.
   void* onRootPopUserData;

   /// Information for a single data hierarchy level.
   struct StackLevel
   {
      StackLevel(JSONValue* _data, const char* _dataTypeName, JSONValue* _dataInherited)
      {
         data = _data;
         dataTypeName = _dataTypeName;
         dataInherited = _dataInherited;
         scrollPosition.Set(0.0f, 0.0f);
      }
      
      /// Data to display and edit.
      JSONValue* data;
      /// Type of the current data to display and edit.
      const char* dataTypeName;
      /// Data which is inherited rather than explicit.
      JSONValue* dataInherited;
      /// Scroll position that can be restored when returning to this level.
      Point2F scrollPosition;
   };

   /// Stack for editing hierarchical data.
   Table<StackLevel> dataStack;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__JSONEDITORWIDGET_H__
