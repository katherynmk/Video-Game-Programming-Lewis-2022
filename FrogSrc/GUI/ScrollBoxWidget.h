#ifndef __FROG__GUI__SCROLLBOXWIDGET_H__
#define __FROG__GUI__SCROLLBOXWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "GUI/SliderWidget.h"
#include "TimedTransition.h"

namespace Webfoot {

class WidgetChildSprite;
class ScrollbarWidget;
class ScrollBoxClipContainerWidget;

/// Key for the clip container
#define SCROLL_BOX_CLIP_CONTAINER_KEY "ClipContainer"

//=============================================================================

/// ScrollBoxClipContainerWidget is a child of a ScrollBoxWidget that holds all the
/// children that need to be clipped and scrolled.  See ScrollBoxWidget for more.
/// Whenever anything other than a scrollbar changes the relative position of this widget,
/// be sure to call ScrollBoxWidget::ScrollBarsRefresh().
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class ScrollBoxClipContainerWidget : public ContainerWidget
{
public:
   virtual ~ScrollBoxClipContainerWidget() {}

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Recursively create and return a string that gives the name
   /// of the Lua table for this widget.  Leave runningLength
   /// at zero for the initial call.  In this case, the class will just
   /// use the callback table of its parent.
   virtual char* EventHandlerTableBuild(size_t runningLength = 0);

   /// Set the area to which the widget and its children will be clipped for drawing and clicking.
   /// Give the area relative to the parent's position.
   /// This does not take into account the clip areas of the ancestors of this widget.
   virtual void ClipAreaSet(Box2F _clipArea);

   /// Set the area in which this widget can be clicked relative to the widget's origin.
   virtual void BoundsSet(Box2F _bounds);

protected:
   typedef ContainerWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }
};

//=============================================================================

/// ScrollbarWidget is a SliderWidget that is bound to a ScrollBox.
/// This is a helper widget, so don't use it directly.
/// Call Init immediately after construction.
class ScrollbarWidget : public SliderWidget
{
public:
   ScrollbarWidget() { isVertical = true; }
   virtual ~ScrollbarWidget() {}
   
   /// Call this immediately after construction.
   /// Initialize a new SliderWidget based on the specifications in the given JSONValue.
   /// If '_isVertical' is true, this will be a vertical scrollbar.  If false, it will be horizontal.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications, bool _isVertical);
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent) { assert(false); (void)_name; (void)_parent; }
   /// Do not call this.  It exists to suppress a compiler warning.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications) { assert(false); (void)_name; (void)_parent; (void)specifications; }

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Called when the value of the widget changes.
   /// Call with 'userMadeChange' set to true if the user caused this call to be made by
   /// using the widget.
   virtual void OnValueChange(float newValue, float oldValue, bool userMadeChange);

protected:
   typedef SliderWidget Inherited;

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }
   
   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// True if the scrollbar is vertical, and false if it's horizontal
   bool isVertical;
};

//=============================================================================

/// A ScrollBoxWidget contains other widgets and keeps them within a clipped,
/// scrollable area.  This widget contains a ScrollBoxClipContainerWidget named
/// "ClipContainer", which is a child of a ScrollBoxWidget that holds all the
/// children that need to be clipped and scrolled.  While it must be included
/// in the definition of a ScrollBoxWidget, it is circumvented for the regular
/// script paths.  For instance, you would not have "MainGame.MyBox.ClipContainer.MyButton",
/// you would have "MainGame.MyBox.MyButton".  Similarly, you would not have
/// an event function for GUI.MainGame.MyBox.ClipContainer.MyButton.OnClick(), but 
/// you could have an event function for GUI.MainGame.MyBox.MyButton.OnClick().
/// Set the ClipArea of the 'ClipContainer' relative to the ScrollBoxWidget to set
/// the box in which the children are visible.  The contents will be scrollable
/// based on the Bounds of the 'ClipContainer'.
/// Call Init immediately after construction.
class ScrollBoxWidget : public ContainerWidget
{
public:
   ScrollBoxWidget();
   virtual ~ScrollBoxWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new ScrollBoxWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget and its children.
   virtual void Update(unsigned int dt);
   /// Draw the widget and its children.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Refresh the visibility and range of the scrollbars.
   virtual void ScrollBarsRefresh();

   /// Set the offset of the contents due to scrolling.
   virtual void ScrollPositionSet(Point2F scrollPosition);
   /// Return the offset of the contents due to scrolling.
   virtual Point2F ScrollPositionGet();

   /// Return a pointer to the given descendant widget based on the given path.
   /// If the widget is not found, return NULL and show a warning.
   /// Assume we're only interested in the contents of clipContainer.
   virtual Widget* DescendantGetByPath(const char* path);

   /// Called when a mouse wheel is scrolled over this widget.
   virtual void OnMouseScroll(int scrollAmount);
   /// Called when mouse drag scrolling begins.  This is different from
   /// dragging the widget.
   virtual void OnMouseDragScrollBegin();
   /// Called when a mouse drags over the widget to make it scroll.  This is
   /// different from dragging the widget.
   virtual void OnMouseDragScroll(const Point2F& scrollAmount);

   /// Return a pointer to the container in which the contents are clipped and scrolled.
   virtual ScrollBoxClipContainerWidget* ClipContainerGet() { return clipContainer; }

   /// Scroll if necessary to show as much of the given area as needed.
   /// Don't move from the current scroll position by any more than needed.
   /// 'targetArea' is in the coordinates of the scrollable area.
   virtual void ScrollToArea(Box2F targetArea);
   virtual void ScrollToArea(Box2F targetArea, unsigned int duration, Progression* progression = theProgressionSmooth);

   /// Return true if this widget should respond to mouse wheel scrolling.
   virtual bool CanHandleMouseScroll() { return true; }
   /// Return true if this widget should respond to mouse drag-scrolling.
   virtual bool CanHandleMouseDragScroll() { return true; }

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// Set whether the scroll position should be restricted to try to keep the
   /// clip area overlapping the bounds of the contents.
   virtual void ScrollPositionRestrictedSet(bool _scrollPositionRestricted);
   /// Return true if the scroll position should be restricted to try to keep
   /// the clip area overlapping the bounds of the contents.
   virtual bool ScrollPositionRestrictedCheck() { return scrollPositionRestricted; }

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) ScrollBoxWidget(); }

protected:
   typedef ContainerWidget Inherited;

   /// Sprite displayed behind the other child widgets.
   WidgetChildSprite* backgroundSprite;

   /// Set the offset of the contents due to scrolling.
   void ScrollPositionSetHelper(Point2F scrollPosition);
   /// Given the requested target area, return the position of scroll such that the new
   /// target area is shown.
   Point2F ScrollPositionGetByTargetArea(Box2F targetArea);

   /// True if the scroll position should be restricted to try to keep the clip
   /// area overlapping the bounds of the contents.
   bool scrollPositionRestricted;
   /// Slider for scrolling up and down.
   ScrollbarWidget* verticalScrollbar;
   /// Slider for scrolling left and right.
   ScrollbarWidget* horizontalScrollbar;
   /// Container that holds the contents of the scrollbox that will be scrollable
   ScrollBoxClipContainerWidget* clipContainer;
   /// The transition for the scroll box to use if the user requests a transition from
   /// one area to another.
   TimedTransitionLinear<Point2F> scrollPositionTransition;
   /// Scroll position of this widget at the time the most recent drag scroll
   /// began.
   Point2F mouseDragScrollBeginScrollPosition;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__SCROLLBOXWIDGET_H__
