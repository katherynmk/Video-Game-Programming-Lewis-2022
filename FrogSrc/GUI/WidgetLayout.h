#ifndef __FROG__GUI__WIDGETLAYOUT_H__
#define __FROG__GUI__WIDGETLAYOUT_H__

#include "FrogMemory.h"
#include "Point2.h"

namespace Webfoot {

class ContainerWidget;
class JSONValue;

/// Name of the type of widget layout to be used for a container.
#define WIDGET_LAYOUT_TYPE_KEY "Type"

//==============================================================================

/// A WidgetLayout controls the positions of widgets within a ContainerWidget.
class WidgetLayout
{
public:
   virtual ~WidgetLayout() {}

   virtual void Init(ContainerWidget* _containerWidget, JSONValue* specifications)
   {
      containerWidget = _containerWidget;
      (void)specifications;
   }
   virtual void Deinit() { containerWidget = NULL; }

   /// Apply the layout to the current set of children in the container.
   virtual void Refresh() = 0;

protected:
   /// Manage the positions of this widget's children.
   ContainerWidget* containerWidget;
};

//==============================================================================

/// A WidgetLayout that places the children in one or more rows or columns.
class WidgetLayoutLinear : public WidgetLayout
{
public:
   virtual ~WidgetLayoutLinear() {}

   virtual void Init(ContainerWidget* _containerWidget, JSONValue* specifications);
   virtual void Refresh();

   typedef WidgetLayout Inherited;

   enum Orientation
   {
      /// Items in a line go from left to right.  Consecutive lines go from top
      /// to bottom.
      ORIENTATION_HORIZONTAL,
      /// Items in a line go from top to bottom.  Consecutive lines go from left
      /// to right.
      ORIENTATION_VERTICAL
   };

   /// Factory method for making objects of this type.
   static WidgetLayout* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) WidgetLayoutLinear(); }

protected:
   /// How should items and lines be positioned relative to each other.
   Orientation orientation;
   /// How the items should be aligned within lines and relative to the layout
   /// bounds.
   int alignment;
   /// True if the children can be arranged into multiple lines.
   bool multipleLinesAllowed;
   /// Fixed space between children along the main direction of a line.
   float childGap;
   // Fixed space between adjacent lines when multiple lines are used.
   float lineGap;
   /// Additional position offset that should be applied to consecutive
   /// children in the same line.
   Point2F childPositionOffsetAdditional;
   /// Additional position offset that should be applied to consecutive lines
   /// when multiple lines are used.
   Point2F linePositionOffsetAdditional;
   /// True if the offset between lines should be fixed rather than determined
   /// using margins.
   bool useFixedLinePositionOffset;
   /// If using fixed position offsets between lines, use this as the offset
   /// from one line to the next.
   Point2F linePositionOffset;
   /// True if the system should target a specific number of children per line
   /// rather than using layout bounds.
   bool useFixedChildCountPerLine;
   /// Number of children per line if targeting a specific number.
   int childrenPerLine;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__WIDGETLAYOUT_H__
