#ifndef __FROG__GUI__LINEARSELECTORWIDGET_H__
#define __FROG__GUI__LINEARSELECTORWIDGET_H__

#include "FrogMemory.h"
#include "GUI/SelectorWidget.h"

namespace Webfoot {

//=============================================================================

/// LinearSelectorWidget is a type of SelectorWidget that displays its items
/// arranged in a straight line.
///
/// Classes should not inherit LinearSelectorWidget unless it isn't a problem
/// that LinearSelectorWidget::Init automatically loads the Items array.
class LinearSelectorWidget : public SelectorWidget
{
public:
   LinearSelectorWidget();
   virtual ~LinearSelectorWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new LinearSelectorWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Move the items into their target positions so that they don't need to
   /// move any farther to get into place.
   virtual void MovementFinish();

   /// Return true if this widget should respond to mouse drag-scrolling.
   virtual bool CanHandleMouseDragScroll() { return true; }
   /// Called when mouse drag scrolling begins.  This is different from
   /// dragging the widget.
   virtual void OnMouseDragScrollBegin();
   /// Called when a mouse drags over the widget to make it scroll.  This is
   /// different from dragging the widget.
   virtual void OnMouseDragScroll(const Point2F& scrollAmount);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) LinearSelectorWidget(); }

protected:
   typedef SelectorWidget Inherited;

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh();

   /// Return a pointer to the RadioButtonContainerWidget for this selector's
   /// items.
   virtual RadioButtonContainerWidget* ItemContainerGet() { return itemContainer; }

   /// Container for the items from which the user selects.
   RadioButtonContainerWidget* itemContainer;
   /// Position of the selected item relative to its parent.
   Point2F itemSelectedPositionRelative;
   /// Position offset from one item to the next.
   Point2F itemNextPositionOffset;
   /// Used to animate the list of items.  This gives the item which should
   /// appear to be selected, but it's a float to allow animating between items.
   /// The range for a collection of 'n' items would be [0, n).
   float displayItemSelectedIndex;
   /// Controls the speed at which the selected item approaches the designated
   /// position.  This must be greater than 0 and less than or equal to 1.
   /// The distance to the final position decays exponentially based on this
   /// number, so that the movement slows to a stop.
   float selectionAnimationSpeed;
   /// Amount of selection shifting since the mouse drag scroll began.
   int dragScrollSelectionShifts;

   friend class LinearSelectorItemComparatorFunctor;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__LINEARSELECTORWIDGET_H__
