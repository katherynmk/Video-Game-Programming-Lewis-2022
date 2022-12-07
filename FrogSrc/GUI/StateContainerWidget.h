#ifndef __FROG__GUI__STATECONTAINERWIDGET_H__
#define __FROG__GUI__STATECONTAINERWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"

namespace Webfoot {

//=============================================================================

/// A StateContainerWidget keeps only one child visible and enabled at a time.
/// That child is called the state.  Note that this will set visible and
/// enabled of children on every state change.  Initially, all children are
/// hidden, except the first, which is shown.
/// Call Init immediately after construction.
class StateContainerWidget : public ContainerWidget
{
public:
   StateContainerWidget();
   virtual ~StateContainerWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   virtual void Init(const char* _name, Widget* _parent);
   /// Call this immediately after construction.
   /// Initialize a new StateContainerWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();
   
   /// Set the state to the widget with the given name.
   /// Return true if successful
   virtual bool StateSet(const char* newState);
   /// Set the state to the given widget.  Be sure the widget is actually an
   /// immediate descendant of this widget.
   virtual void StateSet(Widget* newState);
   /// Return the widget for the current state.
   virtual Widget* StateGet() { return state; }
   /// Return the name of the widget for the current state.
   /// Return NULL if there is no current state or if the current state has no name.
   virtual const char* StateNameGet();

   /// Called when the state changes.
   virtual void OnStateChange(Widget* newState, Widget* oldState);

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }

   /// Type of the function that can receive the OnStateChange event.
   /// The first Widget* is the new state, the second Widget* is the former
   /// state, the StateContainerWidget* is the source of the event, and the void*
   /// is the userData context pointer.
   typedef void (*OnStateChangeCallback)(Widget*, Widget*, StateContainerWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnStateChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnStateChangeRegister(OnStateChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnStateChange event occurs.
   void OnStateChangeUnregister(OnStateChangeCallback callback);
   /// Convenience function for finding a StateContainerWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnStateChangeRegister(const char* path, OnStateChangeCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) StateContainerWidget(); }

protected:
   typedef ContainerWidget Inherited;

   /// The widget for the current state.
   Widget* state;

   /// Function to be called for the OnStateChange event.
   OnStateChangeCallback onStateChangeCallback;
   /// Pointer to be passed to the OnStateChange callback.
   void* onStateChangeUserData;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__STATECONTAINERWIDGET_H__
