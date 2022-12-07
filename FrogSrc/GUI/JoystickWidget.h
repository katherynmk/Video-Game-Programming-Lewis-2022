#ifndef __FROG__GUI__JOYSTICKWIDGET_H__
#define __FROG__GUI__JOYSTICKWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"
#include "GUI/PressButtonWidget.h"

namespace Webfoot {

class JoystickNubWidget;
class WidgetChildSprite;

//==============================================================================

/// Simulate an analog stick on the screen.
class JoystickWidget : public ContainerWidget
{
public:
   JoystickWidget();
   virtual ~JoystickWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new JoystickWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();
   /// Update the widget and its children.
   virtual void Update(unsigned int dt);
   /// Draw the widget and its children.
   virtual void Draw();

   /// Return the value for the joystick's position.  The maximum length of the
   /// vector is 1.0.  The range of the axes is [-1, 1].  Positive X is right
   /// and positive Y is up/forward.
   virtual Point2F ValueGet();
   
   /// Set the fraction of the normal range from the center of the joystick that
   /// is ignored.  [0, 1)
   virtual void DeadZoneSet(float _deadZone) { deadZone = _deadZone; }
   /// Return the fraction of the normal range from the center of the joystick
   /// that is ignored.
   virtual float DeadZoneGet() { return deadZone; }

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   virtual bool OpaqueCheck(const Point2F& testPosition);

   /// Return true if the joystick nub is currently pressed.
   virtual bool NubPressedCheck();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) JoystickWidget(); }

   typedef ContainerWidget Inherited;

protected:
   /// Sprite displayed behind the other child widgets.
   WidgetChildSprite* backgroundSprite;
   /// The part of the joystick that can be dragged.
   JoystickNubWidget* nubWidget;
   /// Fraction of the normal range from the center of the joystick that is
   /// ignored.  [0, 1)
   float deadZone;
};

//==============================================================================

/// Helper class to JoystickWidget.  This represents the part of the joystick
/// that is dragged.
class JoystickNubWidget : public PressButtonWidget
{
public:
   virtual ~JoystickNubWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new widget based on the specifications in the given JSONValue.
   virtual void Init(const char* name, Widget* parent, JSONValue* specifications);
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Called when the widget is dragged by the cursor.
   virtual void OnDrag();

   /// Return the position relative to the parent for when the joystick is at
   /// rest.
   virtual Point2F RestPositionRelativeGet() { return restPosition; }

   /// Helper function for getting the maximum distance that the nub can be
   /// dragged from the rest position.
   virtual float DragDistanceMaxGet();

   typedef PressButtonWidget Inherited;

protected:
   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return false; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return false; }

   /// Return true if a Lua table for event handlers should be automatically
   /// created for each instance of this widget.
   virtual bool EventHandlerTableRequiredCheck() { return false; }

   /// Position relative to the parent for when the joystick is at rest.
   Point2F restPosition;
   /// Speed at which the nub returns to the rest position when it isn't being
   /// dragged.
   float returnSpeed;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__JOYSTICKWIDGET_H__
