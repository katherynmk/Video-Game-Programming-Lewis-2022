#ifndef __FROG__GUI__RINGSELECTORWIDGET_H__
#define __FROG__GUI__RINGSELECTORWIDGET_H__

#include "FrogMemory.h"
#include "GUI/SelectorWidget.h"

namespace Webfoot {

//=============================================================================

/// RingSelectorWidget is a type of SelectorWidget that displays its items
/// arranged in a ring.  The bounds of the items don't count for GUI purposes.
///
/// Classes should not inherit RingSelectorWidget unless it isn't a problem
/// that RingSelectorWidget::Init automatically loads the Items array.
class RingSelectorWidget : public SelectorWidget
{
public:
   RingSelectorWidget();
   virtual ~RingSelectorWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new RingSelectorWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Shift the selection to the item with the index at the given offset
   /// from the currently selected item.  Wrap around if needed.
   /// If no item is selected, selected the first.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.
   virtual void SelectionShift(int offset, bool userMadeChange);

   /// Move the items into their target positions so that they don't need to
   /// move any farther to get into place.
   virtual void MovementFinish();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) RingSelectorWidget(); }

   /// Return true if the item reflections should be shown.
   bool ItemReflectionVisibleCheck() { return itemReflectionVisible; }
   /// Return the color modulated with the normal color of an item to get the
   /// reflection color.
   ColorRGBA8 ItemReflectionColorGet() { return itemReflectionColor; }
   /// Return the scale multiplied by the item scale for the reflection.
   Point2F ItemReflectionScaleGet() { return itemReflectionScale; }

protected:
   typedef SelectorWidget Inherited;

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh();

   /// Return a pointer to the RadioButtonContainerWidget for this selector's
   /// items.
   virtual RadioButtonContainerWidget* ItemContainerGet() { return itemContainer; }

   /// Container for the items from which the user selects.
   RadioButtonContainerWidget* itemContainer;
   /// Amount by which the items have been rotated from their default position.
   float ringRotation;
   /// Controls the speed at which the selected item approaches the bottom of
   /// the ring.  This must be greater than 0 and less than or equal to 1.
   /// The distance to the final rotation decays exponentially based on this
   /// number, so that the rotation slows to a stop. 
   float ringRotationSpeed;
   /// Region to which item positions will be restricted.
   /// The items themselves are not clipped to this area.
   Box2F ringBounds;
   /// Scale of items at the very front (bottom) of the ring.
   Point2F itemFrontScale;
   /// Scale of items at the very back (top) of the ring.
   Point2F itemBackScale;
   /// Color of items at the very front (bottom) of the ring.
   ColorRGBA8 itemFrontColor;
   /// Color of items at the very back (top) of the ring.
   ColorRGBA8 itemBackColor;
   /// True if the item reflections should be shown.
   bool itemReflectionVisible;
   /// Color modulated with the normal color of an item to get the
   /// reflection color.
   ColorRGBA8 itemReflectionColor;
   /// Scale multiplied by the item scale for the reflection.
   Point2F itemReflectionScale;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__RINGSELECTORWIDGET_H__
