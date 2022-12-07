#ifndef __FROG__GUI__JSONWIDGET_H__
#define __FROG__GUI__JSONWIDGET_H__

#include "FrogMemory.h"
#include "GUI/Widget.h"

namespace Webfoot {

class JSONValue;

//==============================================================================

/// Helper class which contains JSON data.  This is useful for TableWidget cells
/// which need to store JSON data.
class JSONWidget : public Widget
{
public:
   JSONWidget();
   virtual ~JSONWidget() {}

   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set the data for this widget to hold.
   virtual void DataSet(JSONValue* _data);
   /// Return the data held by this widget.
   virtual JSONValue* DataGet() { return data; }

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) JSONWidget(); }

   typedef Widget Inherited;

protected:
   /// JSON data held by this widget.
   JSONValue* data;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__JSONWIDGET_H__
