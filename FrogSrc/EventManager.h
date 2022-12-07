#ifndef __FROG__EVENTMANAGER_H__
#define __FROG__EVENTMANAGER_H__

#include "FrogMemory.h"
#include "Map.h"
#include "Table.h"
#include "EventRegistration.h"

namespace Webfoot {

/// Default priority to use for event handlers.
const int EVENT_PRIORITY_DEFAULT = 0;

//=============================================================================

/// Helper class to EventManager.
/// Manages all data for a given registered event.
class Event
{
public:
   /// Initialize the object and use the given heap for internal allocations.
   /// Make an internal copy of the event string.
   void Init(const EventID* _eventID, HeapID _heapID);
   void Deinit();

   /// Add the given registration to the event.
   void Register(EventRegistration* eventRegistration);

   /// Unregister and free all 
   void UnregisterAll();
   /// Remove the given registration.  This will not free 'eventRegistration' internally.
   void Unregister(EventRegistration* eventRegistration);

   /// Sort the registrations by priority if needed.
   void Sort();

   /// Reserve space for this many entries.  This can help deal with fragmentation.
   void ReservationSet(int _reservationSize);
   /// Remove the registrations so that the object will be deleted
   /// when empty.  This is not the same as calling ReservationSet(0).
   void ReservationRemove();

   /// Return true if the list is empty and there are no reservations.
   bool ShouldDeinitCheck();

   /// Call all listeners for this event in descending order of priority.
   /// 'userData' will be passed to listeners that accept it.
   void Trigger(void* userData);
   /// Call all listeners for this event until one of them returns non-zero.
   /// If a listener accepts it, it will be passed 'userData'.
   /// If a listener returned non-zero, return that value here.  If not, return zero.
   int TriggerWithEarlyOut(void* userData);

   /// Event descriptor
   EventID eventID;

protected:
   /// Helper function called toward the beginning of a trigger.
   void PreTrigger(Table<EventRegistration*>* registrationsCopy);
   /// Helper function called toward the end of a trigger.
   void PostTrigger(Table<EventRegistration*>* registrationsCopy);
   /// Deinit all registrations in the garbage.
   void GarbageDeinit();

   /// False if the 'registrations' need to be sorted.
   bool sorted;
   /// Reserve space in 'registrations' for this many entries.
   /// -1 if there should be no reservations.
   int reservationSize;
   /// Internal copy of the event name.
   char* eventName;
   /// Number of active trigger calls for this event.
   int triggerPendingCount;
   /// Collection of to be called for the event.
   Table<EventRegistration*> registrations;
   /// Collection of registrations to be deleted.
   Table<EventRegistration*> registrationsGarbage;
};

//=============================================================================

/// EventManager is a centralized system that allows functions and methods in
/// any part of the program to listen for events.  Similarly, these events
/// can be triggered from any part of the program.
/// Threading and interrupt issues still apply though.
/// You can register either functions or methods and identify the event either
/// by its name or EventID.  These handlers can either take no parameters or take different combinations
/// of void* userData or const EventRegistration*, which can help give context to the event.
/// Events handlers are called in descending order of registration priority number.
///
/// Events can typically be referred to either by a string or an EventID.
///    theEvents->Trigger("Update");
///
///    EventID updateEvent("Update");
///    theEvents->Trigger(&updateEvent);
///
/// You can use the Register function to register a method or a function
/// for a particular event.
///    theEvents->Register("Update", Update);
///
///    theEvents->Register("Update", this, &MyClass::Update);
///
/// The Register function returns an EventRegistration object which can
/// be used to identify that particular registration.
///    EventRegistration* priorityHighRegistration = theEvents->Register("Update", Update, EXAMPLE_PRIORITY_HIGH);
///    EventRegistration* priorityLowRegistration = theEvents->Register("Update", Update, EXAMPLE_RIORITY_LOW);
///
///    void Update(const EventRegistration* registration)
///    {
///       if(registration->PriorityGet() == EXAMPLE_PRIORITY_HIGH)
///          ...
///       else
///          ...
///    }
///
/// The Unregister function can be used in different ways.  It can cancel all registrations
/// for a particular object or function.
///    theEvents->Unregister(this);
///
/// It can cancel all registrations of a particular object or function for a specific event.
///    theEvents->Unregister("Update", this);
///
/// It can also cancel a specific registration.
///    theEvents->Unregister(priorityHighRegistration);
///
/// A registered function or method can only accept certain parameters.
///    void Update();
///    void Update(void* userData);
///    void Update(const EventRegistration* registration);
///    void Update(void* userData, const EventRegistration* registration);
///
/// When triggering an event, you can pass an arbitrary pointer to all the handlers
/// that accept it.
///    UpdateData updateData;
///    theEvents->Trigger("Update", (void*)(&updateData));
class EventManager
{
public:
   void Init(HeapID _heapID = HEAP_DEFAULT);
   void Deinit();

   void Update();

   /// Reserve enough room for then given event to have the specified number of listeners.
   /// More memory will still be allocated when these listeners are registered, but those allocations will
   /// be freed as each is unregistered.  Use Unreserve when it becomes safe to delete the event.
   void Reserve(const char* eventName, int reserveCount)
   {
      EventID eventID(eventName);
      Reserve(&eventID, reserveCount);
   }
   /// Reserve enough room for then given event to have the specified number of listeners.
   /// More memory will still be allocated when these listeners are registered, but those allocations will
   /// be freed as each is unregistered.  Use Unreserve when it becomes safe to delete the event.
   void Reserve(const EventID* eventID, int reserveSize);

   /// Clean up the effects of Reserve.
   void Unreserve(const char* eventName)
   {
      EventID eventID(eventName);
      Unreserve(&eventID);
   }
   /// Clean up the effects of Reserve.
   void Unreserve(const EventID* eventID);

   /// Register a function or method to receive calls for the given event.

   template<typename R> inline EventRegistration* Register(const char* eventName,
      R (*function)(), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const char* eventName,
      R (*function)(void* userData), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const char* eventName,
      R (*function)(const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const char* eventName,
      R (*function)(void* userData, const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);

   template<typename T, typename R> inline EventRegistration* Register(const char* eventName,
      T* object, R (T::*method)(), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const char* eventName,
      T* object, R (T::*method)(void* userData), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const char* eventName,
      T* object, R (T::*method)(const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const char* eventName,
      T* object, R (T::*method)(void* userData, const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);

   template<typename R> inline EventRegistration* Register(const EventID* eventID,
      R (*function)(), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const EventID* eventID,
      R (*function)(void* userData), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const EventID* eventID,
      R (*function)(const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename R> inline EventRegistration* Register(const EventID* eventID,
      R (*function)(void* userData, const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);

   template<typename T, typename R> inline EventRegistration* Register(const EventID* eventID,
      T* object, R (T::*method)(), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const EventID* eventID,
      T* object, R (T::*method)(void* userData), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const EventID* eventID,
      T* object, R (T::*method)(const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);
   template<typename T, typename R> inline EventRegistration* Register(const EventID* eventID,
      T* object, R (T::*method)(void* userData, const EventRegistration* eventRegistration), int priority = EVENT_PRIORITY_DEFAULT);

   /// If passed a handler object or function, all registrations for that handler
   /// will be unregistered.
   template<typename T> inline void Unregister(T handler)
   {
      UnregisterHandlerHelper((void*)handler);
   }

   /// If the given registration is found it will be removed and freed.
   inline void Unregister(EventRegistration* eventRegistration)
   {
      UnregisterRegistrationHelper(eventRegistration);
   }

   /// Unregister the given listener function or object from the given event.
   template<typename T> inline void Unregister(const char* eventName, T handler)
   {
      EventID eventID(eventName);
      UnregisterHandlerHelper(&eventID, (void*)handler);
   }
   /// Unregister the given listener function or object from the given event.
   template<typename T> inline void Unregister(const EventID* eventID, T handler)
   {
      UnregisterHandlerHelper(eventID, (void*)handler);
   }

   /// Call all listeners for the given event in descending order of priority.
   /// 'userData' will be passed to listeners that accept it.
   void Trigger(const char* eventName, void* userData = NULL)
   {
      EventID eventID(eventName);
      Trigger(&eventID, userData);
   }
   /// Call all listeners for the given event in descending order of priority.
   /// 'userData' will be passed to listeners that accept it.
   void Trigger(const EventID* eventID, void* userData = NULL);

   /// Call all listeners for the given event in descending order of priority
   /// until one of them returns non-zero. If a listener accepts it, it will be passed 'userData'.
   /// If a listener returned non-zero, return that value here.  If not, return zero.
   int TriggerWithEarlyOut(const char* eventName, void* userData = NULL)
   {
      EventID eventID(eventName);
      return TriggerWithEarlyOut(&eventID, userData);
   }
   /// Call all listeners for the given event in descending order of priority
   /// until one of them returns non-zero. If a listener accepts it, it will be passed 'userData'.
   /// If a listener returned non-zero, return that value here.  If not, return zero.
   int TriggerWithEarlyOut(const EventID* eventID, void* userData = NULL);

   /// Schedule the given event to be triggered on the first EventManager
   /// Update on or after the given time. 'eventTime' should correspond to
   /// a value to be expected from theClick->TickCountGet().
   /// If a listener accepts it, it will be passed 'userData'.
   //Schedule(const char* eventName, uint32 eventTime, void* userData = NULL);
   //ScheduleRepeater(const char* eventName, uint32 interval, void* userData = NULL);
   //Unschedule(const char* eventName);

   /// Used for using EventID pointers as keys.
   static bool EventIDComparator(const EventID* first, const EventID* second)
   {
      return (*first) < (*second);
   }

   static EventManager instance;

protected:
   /// Collection of event registrations
   typedef Map<const EventRegistration*, EventRegistration*> HandlerRegistrationMap;
   /// Map of EventIDs to data associated with that event.
   typedef Map<const EventID*, Event*, bool(*)(const EventID*, const EventID*)> EventMap;
   /// Map of event handlers to lists of events registered to that handler.
   typedef Map<const void*, HandlerRegistrationMap* > EventHandlerMap;

   /// Return a pointer to the Event object for the given EventID.
   /// If it does not exist, create it first, then return it.
   Event* EventGetOrAdd(const EventID* eventID);
   /// Return a pointer to the HandlerRegistrationMap object for the given handler.
   /// If it does not exist, create it first, then return it.
   HandlerRegistrationMap* HandlerRegistrationMapGetOrAdd(const void* handler);

   /// Helper function that records the new registration.
   void Register(const EventID* eventID, EventRegistration* eventRegistration, const void* handler);

   /// If the given registration is found it will be removed and freed.
   void UnregisterRegistrationHelper(EventRegistration* eventRegistration);
   /// Unregister the given handler from all events.
   void UnregisterHandlerHelper(void* handler);
   /// Unregister the given handler from the given event.
   void UnregisterHandlerHelper(const EventID* eventID, void* handler);

   /// Helper function that cleans up and deletes the given event object if appropriate.
   void EventDeinitIfNeeded(Event* event);
   /// Helper function that cleans up and deletes the given HandlerRegistrationMap if appropriate.
   void HandlerRegistrationMapDeinitIfNeeded(const void* handler, HandlerRegistrationMap* handlerRegistrationMap);

   /// Heap from which all relevant memory is drawn.
   HeapID heapID;

   /// Collection of collections of event data.
   EventMap eventMap;
   /// Collection that maps registered functions and objects to their associated registrations.
   EventHandlerMap eventHandlerMap;
};

EventManager* const theEvents = &EventManager::instance;

//=============================================================================

template<typename R> inline EventRegistration* EventManager::Register(const char* eventName, R (*function)(), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, function, priority);
}
template<typename R> inline EventRegistration* EventManager::Register(const char* eventName, R (*function)(void*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, function, priority);
}
template<typename R> inline EventRegistration* EventManager::Register(const char* eventName, R (*function)(const EventRegistration*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, function, priority);
}
template<typename R> inline EventRegistration* EventManager::Register(const char* eventName, R (*function)(void*, const EventRegistration*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, function, priority);
}

//-----------------------------------------------------------------------------

template<typename T, typename R> inline EventRegistration* EventManager::Register(const char* eventName, T* object, R (T::*method)(), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, object, method, priority);
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const char* eventName, T* object, R (T::*method)(void*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, object, method, priority);
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const char* eventName, T* object, R (T::*method)(const EventRegistration*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, object, method, priority);
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const char* eventName, T* object, R (T::*method)(void*, const EventRegistration*), int priority)
{
   EventID eventID(eventName);
   return Register(&eventID, object, method, priority);
}

//-----------------------------------------------------------------------------

template<typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, R (*function)(), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) Function0EventRegistration<R>(function, priority);
   Register(eventID, eventRegistration, (const void*)function);
   return eventRegistration;
}
template<typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, R (*function)(void*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) FunctionVEventRegistration<R>(function, priority);
   Register(eventID, eventRegistration, (const void*)function);
   return eventRegistration;
}
template<typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, R (*function)(const EventRegistration*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) FunctionREventRegistration<R>(function, priority);
   Register(eventID, eventRegistration, (const void*)function);
   return eventRegistration;
}
template<typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, R (*function)(void*, const EventRegistration*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) FunctionVREventRegistration<R>(function, priority);
   Register(eventID, eventRegistration, (const void*)function);
   return eventRegistration;
}

//-----------------------------------------------------------------------------

template<typename T, typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, T* object, R (T::*method)(), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) Method0EventRegistration<T, R>(object, method, priority);
   Register(eventID, eventRegistration, (const void*)object);
   return eventRegistration;
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, T* object, R (T::*method)(void*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) MethodVEventRegistration<T, R>(object, method, priority);
   Register(eventID, eventRegistration, (const void*)object);
   return eventRegistration;
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, T* object, R (T::*method)(const EventRegistration*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) MethodREventRegistration<T, R>(object, method, priority);
   Register(eventID, eventRegistration, (const void*)object);
   return eventRegistration;
}
template<typename T, typename R> inline EventRegistration* EventManager::Register(const EventID* eventID, T* object, R (T::*method)(void*, const EventRegistration*), int priority)
{
   EventRegistration* eventRegistration = frog_new_ex(heapID) MethodVREventRegistration<T, R>(object, method, priority);
   Register(eventID, eventRegistration, (const void*)object);
   return eventRegistration;
}

//=============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__EVENTMANAGER_H__
