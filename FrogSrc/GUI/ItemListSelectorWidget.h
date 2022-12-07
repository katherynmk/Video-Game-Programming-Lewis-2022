#ifndef __FROG__GUI__ITEMLISTSELECTOR_H__
#define __FROG__GUI__ITEMLISTSELECTOR_H__

#include "FrogMemory.h"
#include "GUI/SelectorWidget.h"
#include "GUI/SpriteWidget.h"
#include "TimedTransition.h"
#include "GUI/ScrollBoxWidget.h"

namespace Webfoot {

//=============================================================================

/// 
class ItemListSelectorWidget : public SelectorWidget
{
public:
   ItemListSelectorWidget();
   virtual ~ItemListSelectorWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new ItemListSelectorWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ItemListSelectorWidget(); }

   /// Select the given item.  Give NULL to clear the selection.
   /// Call with 'userMadeChange' set to true if the user caused this call through
   /// direct interaction with the widget.
   virtual void SelectionSet(SelectorItemWidget* item, bool userMadeChange);

   /// Move the items into their target positions so that they don't need to
   /// move any farther to get into place.
   virtual void MovementFinish();

   /// Sets the visiblity of the cursor sprite.
   void CursorVisibleSet(bool _visible) { if (cursorSprite) cursorSprite->VisibleSet(_visible); }

protected:
   /// The type of positioning to use when refresing the item list.
   enum ItemPositioningType
   {
      Horizontal,
      Vertical,
      Offset_Defined
   };

   typedef SelectorWidget Inherited;

   /// Given the item, return the offset amount for the next item in the list
   Point2F PositioningOffsetGetByLastItem(Widget* _item);

   /// Position all the item buttons based on their positions in the list.
   virtual void ItemPositionsRefresh();

   /// Return a pointer to the RadioButtonContainerWidget for this selector's
   /// items.
   virtual RadioButtonContainerWidget* ItemContainerGet() { return itemContainer; }

   /// The scroll box in which all other containers will be placed.
   ScrollBoxWidget* scrollBox;
   /// Container for the items from which the user selects.
   RadioButtonContainerWidget* itemContainer;
   /// Sprite that represents the cursor.
   SpriteWidget* cursorSprite;
   /// The type of positioning to use for the layout of items.
   ItemPositioningType itemPositioningType;
   /// The amount of spacing to use between items.
   Point2F itemSpacing;
   /// The offset to use when positioning the cursor on the selected item.
   Point2F cursorOffset;
   /// The transition for the cursor to move from selected items
   TimedTransitionLinear<Point2F> cursorPositionTransition;
   /// The bobbing transition for the cursor
   TimedTransitionBezierQuadratic<Point2F> cursorBobbingTransition;
   
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__ITEMLISTSELECTOR_H__
