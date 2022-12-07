#ifndef __FROG__GUI__CONTAINERWIDGET_H__
#define __FROG__GUI__CONTAINERWIDGET_H__

#include "FrogMemory.h"
#include "Table.h"
#include "GUI/Widget.h"

namespace Webfoot {

class Widget;
class Sprite;
class WidgetLayout;
struct FocusWidgetAndBounds;

/// Key for the type of a widget.
#define WIDGET_TYPE_KEY "Type"
/// Key for the list of children.
#define CONTAINER_WIDGET_CHILDREN_KEY "Children"
/// String that separates the widget names and indices in a path.
#define WIDGET_PATH_DELIMITER '.'

//=============================================================================

/// ContainerWidgetComparator is an interface for comparators that can be
/// passed to sort.  This helps abstract away the details of std::stable_sort
/// and allows for comparator polymorphism.
class ContainerWidgetComparator
{
public:
   virtual ~ContainerWidgetComparator() {}
   /// Return true only if widgetA is less than widgetB.  If they
   /// are equivalent, return false.
   virtual bool Compare(Widget* widgetA, Widget* widgetB) = 0;
};

//=============================================================================

/// A helper functor used to abstract away the details of
/// std::stable_sort predicates.
class ContainerWidgetComparatorWrapper
{
public:
   /// Create an object to wrap the given comparator.
   ContainerWidgetComparatorWrapper(ContainerWidgetComparator* _comparator)
   {
      comparator = _comparator;
   }

   /// Functor called by stable_sort
   bool operator()(Widget* rowA, Widget* rowB)
   {
      return comparator->Compare(rowA, rowB);
   }

private:
   ContainerWidgetComparator* comparator;
};

//=============================================================================

/// Widget that contains other widgets.
/// Do not add a given widget more than once.
/// Call Init immediately after construction.
class ContainerWidget : public Widget
{
public:
   ContainerWidget();
   virtual ~ContainerWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   virtual void Init(const char* _name, Widget* _parent);

   /// Clean up the widget
   virtual void Deinit();

   /// Call this immediately after construction.
   /// Initialize a new ContainerWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Update the widget and its children
   virtual void Update(unsigned int dt);
   /// Draw the widget and its children
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Refresh the text for the widget and its children.
   /// Call this after a language change.
   virtual void TextRefresh();

   /// Return the top-most widget in this object, including the object itself,
   /// that overlaps the given point.  Return NULL if none overlap it.  The point
   /// is in the local coordinates of the parent.
   virtual Widget* WidgetAtPointGet(const Point2F& point);

   /// Add the widget to the end of the list of children.
   virtual void ChildAdd(Widget* widget) { ChildAddLast(widget); }
   /// Add the widget to the beginning of the list of children.
   virtual void ChildAddFirst(Widget* widget);
   /// Add the widget to the end of the list of children.
   virtual void ChildAddLast(Widget* widget);
   /// Add the given widget before the child referenceWidget.
   /// Return true if successful.
   virtual bool ChildAddBefore(const char* referenceWidgetName, Widget* widget);
   /// Add the given widget before the child referenceWidget.
   /// Return true if successful.
   virtual bool ChildAddBefore(Widget* referenceWidget, Widget* widget);
   /// Add the given widget after the child referenceWidget.
   /// Return true if successful.
   virtual bool ChildAddAfter(const char* referenceWidgetName, Widget* widget);
   /// Add the given widget after the child referenceWidget.
   /// Return true if successful.
   virtual bool ChildAddAfter(Widget* referenceWidget, Widget* widget);
   /// Add the given widget at the given index.
   virtual void ChildAdd(int index, Widget* widget);
   
   /// Remove the child at the beginning of the list.
   /// Return true if successful.
   virtual bool ChildFirstRemove();
   /// Remove the child at the end of the list.
   /// Return true if successful.
   virtual bool ChildLastRemove();

   /// Remove the child with the given name from the container and add it to the garbage.
   /// Return true if successful.
   virtual bool ChildRemove(const char* widgetName);
   /// Remove the given child from the container and add it to the garbage.
   /// The child must currently be in the container.
   /// Return true if successful.
   virtual bool ChildRemove(Widget* widget);
   /// Remove the child with the given index from the container and add it to the garbage.
   /// Return true if successful.
   virtual bool ChildRemove(int index);
   /// Remove all the children and put them in the garbage.
   virtual void ChildrenRemoveAll();

   /// Without adding the child to the garbage or implicitly setting its parent
   /// to NULL, remove the given child from the container.  Return true if
   /// successful.
   virtual bool ChildDetach(Widget* widget);

   /// Assuming the given widget is already a child, move it to the end of the
   /// list of children.
   virtual void ChildMoveToLast(Widget* child);

   /// Return a pointer to the first child in the list.
   /// Return NULL if the there are no children.
   virtual Widget* ChildFirstGet();
   /// Return a pointer to the last child in the list.
   /// Return NULL if the there are no children.
   virtual Widget* ChildLastGet();

   /// Return a pointer to the first child in the list
   /// with the given name.  Return NULL if the child is not found.
   virtual Widget* ChildGet(const char* _name);

   /// Return a pointer to the child with the given index.
   /// Return NULL if the index is out of bounds.
   virtual Widget* ChildGet(int index);

   /// Return the index of the child widget with the given name.
   /// Return -1 if the widget is not in the container.
   /// Bear in mind that the index of an individual widget can change
   /// as other widgets are added and removed, so don't use this
   /// unless you understand how that works.
   virtual int ChildIndexGet(const char* _name);

   /// Return the index of the given widget.
   /// Return -1 if the widget is not in the container.
   /// Bear in mind that the index of an individual widget can change
   /// as other widgets are added and removed, so don't use this
   /// unless you understand how that works.
   virtual int ChildIndexGet(Widget* widget);

   /// Return the number of immediate children.
   virtual int ChildCountGet() { return children.SizeGet(); }

   /// Reserve space for the given number of children.
   virtual void ChildrenReserve(int widgetCount) { children.Reserve(widgetCount); }

   /// Create a widget based on the given specs and add it to the end of the
   /// list of children.
   Widget* ChildCreate(JSONValue* newWidgetSpecs);

   /// Return a pointer to the given descendant widget based on the given path.
   /// If the widget is not found, return NULL.
   virtual Widget* DescendantGetByPath(const char* path);
   /// Return a pointer to the first descandant found with the given name.
   /// If no matching widget is found, return NULL.
   virtual Widget* DescendantGetByName(const char* descendantName);

   /// If the given mouse is being used by the gui, return true if
   /// the top-most enabled widget, if any, that is considered to be under
   /// its cursor as of the last Update() is one of the descendants of
   /// this widget.
   virtual bool MouseOverDescendantCheck(Mouse* mouse);

   /// Return true if the given widget is a descendant of this one.
   virtual bool WidgetDescendantCheck(Widget* widget);

   /// Sort all the children using the given comparator object.
   virtual void Sort(ContainerWidgetComparator* comparator);

   /// True if, when a descendant is pressed, the corresponding child should be
   /// moved to the end of the list of children.
   virtual bool ChildMoveToLastOnDescendantPressCheck() { return childMoveToLastOnDescendantPress; } 
   /// Set whether, when a descendant is pressed, the corresponding child
   /// should be moved to the end of the list of children.
   virtual void ChildMoveToLastOnDescendantPressSet(bool _childMoveToLastOnDescendantPress) { childMoveToLastOnDescendantPress = _childMoveToLastOnDescendantPress; }

   /// Called when the left mouse button is pressed with the cursor over the
   /// given descendant the widget.
   virtual void OnDescendantPress(Widget* widget);

   /// Called when the list of mice for the GUIManager has changed.
   virtual void OnMiceReset();

   /// Called when this widget or an ancestor is about to be removed from
   /// the tree of widgets.
   virtual void PreRemove();
   /// Called when this widget or an ancestor has just been removed from
   /// the tree of widgets.
   virtual void PostRemove();

   /// Return the union of the bounds of all the descendants relative
   /// to the origin of this widget.
   virtual Box2F DescendantBoundsFullGet();

   /// Begin the transition with the given name.
   virtual void TransitionBegin(const char* name);
   /// Return true if this widget or any of its descendants are in a transition
   /// that has not yet finished.
   virtual bool TransitioningCheck();
   /// Finish any pending transitions of this and any descendants.
   virtual void TransitionsFinish();

   /// Called with the base clip area for the widgets is changed.
   virtual void OnClipAreaBaseChange();

   /// Set the box, relative to the widget's position, within which the
   /// children should be positioned for layout purposes.
   virtual void LayoutBoundsSet(Box2F _layoutBounds);
   /// Return the box, relative to the widget's position, within which the
   /// children should be positioned for layout purposes.  If the layout bounds
   /// were not defined, use the widget bounds.
   virtual Box2F LayoutBoundsGet();
   /// Use the bounds of the widget rather than any previous LayoutBounds.
   virtual void UseWidgetBoundsForLayout() { useWidgetBoundsForLayout = true; }

   /// Return true if this widget should draw its layout bounds.
   virtual bool LayoutBoundsDrawCheck() { return layoutBoundsDraw; }
   /// Set whether this widget should draw its layout bounds.
   virtual void LayoutBoundsDrawSet(bool _layoutBoundsDraw) { layoutBoundsDraw = _layoutBoundsDraw; }

   /// Return the color to use when drawing the layout bounds of the Widget.
   virtual ColorRGBA8 LayoutBoundsColorGet() { return layoutBoundsColor; }
   /// Set the color to use when drawing the layout bounds of the Widget.
   virtual void LayoutBoundsColorSet(const ColorRGBA8& _layoutBoundsColor) { layoutBoundsColor = _layoutBoundsColor; }

   /// Apply the current WidgetLayout to the current set of widgets.
   virtual void LayoutRefresh();

   /// Recompute the input focus links for this widget and its descendants.
   virtual void FocusLinksRefresh();
   /// Automatically set input focus links for this node and its descendants
   /// based on, among other things, their bounds and positions.
   void FocusLinksSetAutomatic(bool wrapX = true, bool wrapY = true);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ContainerWidget(); }

protected:
   typedef Widget Inherited;

   /// Helper function to make 'childrenCopy' match the contents of 'children'.
   void ChildrenCopyRefresh();

   /// Called just before the given widget is added to this container.
   virtual void PreChildAdd(Widget* widget) { (void)widget; }
   /// Called just after the given widget is added to this container.
   virtual void PostChildAdd(Widget* widget) { (void)widget; }

   /// Called just before the given widget is removed from this container.
   virtual void PreChildRemove(Widget* widget) { (void)widget; }
   /// Called just after the given widget is removed from this container.
   /// At this point, the widget will not yet have been deinitialized.
   virtual void PostChildRemove(Widget* widget) { (void)widget; }

   /// Helper function for building a collection of widgets that can receive
   /// input focus.
   static void FocusLinksSetAutomaticGatherWidgets(Table<FocusWidgetAndBounds*>* focusWidgets,
      ContainerWidget* currentWidget);

   /// True if the children should be drawn from last to first, rather than
   /// first to last.
   bool childrenDrawOrderReverse;
   /// True if, when a descendant is pressed, the corresponding child should be
   /// moved to the end of the list of children.
   bool childMoveToLastOnDescendantPress;
   /// True if input focus links for this node and its descendants should be
   /// set automatically based on, among other things, their bounds and positions.
   bool focusLinksSetAutomatic;
   /// True if automatically generated links should wrap horizontally.
   bool focusLinksSetAutomaticWrapX;
   /// True if automatically generated links should wrap vertically.
   bool focusLinksSetAutomaticWrapY;
   /// True if the widget's bounds should be used as the layout bounds.
   bool useWidgetBoundsForLayout;
   /// Area relative to the widget's position, within which the children should
   /// be positioned for layout purposes.
   Box2F layoutBounds;
   /// True if the widget should draw its layout bounds.
   bool layoutBoundsDraw;
   /// Color used to draw the layout bounds.
   ColorRGBA8 layoutBoundsColor;
   /// Current layout, if any.
   WidgetLayout* layout;
   /// Collection of widgets that are owned by this one.
   Table<Widget*> children;
   /// Copy of 'children' that is used internally to more safely iterate over
   /// children.
   Table<Widget*> childrenCopy;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__CONTAINERWIDGET_H__
