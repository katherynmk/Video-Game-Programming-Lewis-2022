#ifndef __FROG__EVENTREGISTRATION_H__
#define __FROG__EVENTREGISTRATION_H__ 

#include "FrogMemory.h"

namespace Webfoot {

class Event;
class EventManager;

//==============================================================================

/// \internal Faster lookup strings data
struct EventID
{
   unsigned hash;
   const char* name;
   
   EventID() { hash = 0; name = ""; }
   EventID(const char* eventName)
   {
      name = eventName;
      hash = GetHash(eventName);
   }
   
   bool operator< (const EventID& other) const
   {
      if(hash < other.hash) return true;
      if(other.hash < hash) return false;
      return strcmp(name, other.name) < 0; 
   }

   bool operator== (const EventID& other) const
   {
      if(hash != other.hash) return false;
      return strcmp(name, other.name) == 0; 
   }

   static unsigned GetHash(const char* p)
   {
      unsigned s1 = 0;
      unsigned s2 = 0;
      
      while(*p)
      {
         s1 += *reinterpret_cast<const unsigned char*>(p++);
         s2 += s1;
      }
      
      return s1 | (s2 << 16);    
   }
};

//==============================================================================

/// EventRegistration represents an instance of a function or method being
/// registered to receive events at a given priority.  It also contains a
/// link to the EventID.
class EventRegistration
{
public:
   EventRegistration() { remove = false; }
   virtual ~EventRegistration() { }

   virtual int OnTrigger(void* userData) = 0;

   /// Return the function or object associated with this registration.
   virtual inline const void* HandlerGet() const { return NULL; }
   /// Return the function, if any, associated with this listener
   virtual inline const void* FunctionGet() const { return NULL; }
   /// Return the object, if any, associated with this listener.
   virtual inline const void* ObjectGet() const { return NULL; }

   /// Listeners are called in ascending order of priority when events are triggered.
   inline int PriorityGet() const { return priority; }

   /// Return a pointer to the event associated with this registration.
   inline const EventID* EventIDGet() const { return eventID; }
   /// Set which event is associated with this registration.
   /// This should only be called by the EventManager.
   inline void EventIDSet(const EventID* _eventID) { eventID = _eventID; }

   /// Set a pointer to the Event object that holds this registration.
   /// This is probably only useful to the EventManager.
   inline Event* EventGet() const { return event; }
   /// Set which Event object holds this registration.
   /// This should only be called by the EventManager.
   inline void EventSet(Event* _event) { event = _event; }

   /// Mark the object so it knows it is out of the registration list and will
   /// soon be deinitialized.
   virtual void GarbageSet() { remove = true; }
   /// Return true if the object is scheduled for deletion.
   inline bool GarbageCheck() { return remove; }

   /// Comparator used to sort pointers to registrations by priority.  (descending order)
   static bool PriorityComparator(const EventRegistration* first, const EventRegistration* second)
   {
      return first->priority > second->priority;
   }

protected:
   /// True if this registration has been marked for removal.
   bool remove;
   /// Listeners are called in ascending order of priority when events are triggered.
   int priority;
   /// The event associated with this registration.
   const EventID* eventID;
   /// The class that contains the registration.
   Event* event;

   friend class EventManager;
};

//==============================================================================

template<typename R> class Function0EventRegistration : public EventRegistration
{
public:
   Function0EventRegistration(R (*aFunction)(), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~Function0EventRegistration() { }

   virtual int OnTrigger(void*) { return function(); }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   R (*function)();  
};

//==============================================================================

template<> class Function0EventRegistration<void> : public EventRegistration
{
public:
   Function0EventRegistration(void (*aFunction)(), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~Function0EventRegistration() { }

   virtual int OnTrigger(void*) { function(); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   void (*function)();  
};

//==============================================================================

template<typename R> class FunctionVEventRegistration : public EventRegistration
{
public:
   FunctionVEventRegistration(R (*aFunction)(void*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionVEventRegistration() { }

   virtual int OnTrigger(void* userData) { return function(userData); }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   R (*function)(void*);
};

//==============================================================================

template<> class FunctionVEventRegistration<void> : public EventRegistration
{
public:
   FunctionVEventRegistration(void (*aFunction)(void*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionVEventRegistration() { }

   virtual int OnTrigger(void* userData) { function(userData); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   void (*function)(void*);
};

//==============================================================================

template<typename R> class FunctionREventRegistration : public EventRegistration
{
public:
   FunctionREventRegistration(R (*aFunction)(const EventRegistration*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionREventRegistration() { }

   virtual int OnTrigger(void*) { return function(this); }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   R (*function)(const EventRegistration*);
};

//==============================================================================

template<> class FunctionREventRegistration<void> : public EventRegistration
{
public:
   FunctionREventRegistration(void (*aFunction)(const EventRegistration*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionREventRegistration() { }

   virtual int OnTrigger(void*) { function(this); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   void (*function)(const EventRegistration*);
};

//==============================================================================

template<typename R> class FunctionVREventRegistration : public EventRegistration
{
public:
   FunctionVREventRegistration(R (*aFunction)(void*, const EventRegistration*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionVREventRegistration() { }

   virtual int OnTrigger(void* userData) { return function(userData, this); }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   R (*function)(void*, const EventRegistration*);
};

//==============================================================================

template<> class FunctionVREventRegistration<void> : public EventRegistration
{
public:
   FunctionVREventRegistration(void (*aFunction)(void*, const EventRegistration*), int _priority)
   {
      function = aFunction;
      priority = _priority;
   }
   virtual ~FunctionVREventRegistration() { }

   virtual int OnTrigger(void* userData) { function(userData, this); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)function; }
   virtual inline const void* FunctionGet() const { return (void*)function; }

private:
   void (*function)(void*, const EventRegistration*);
};

//==============================================================================

template<typename T, typename R> class Method0EventRegistration : public EventRegistration
{
public:
   Method0EventRegistration(T* aObject, R (T::*aMethod)(), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~Method0EventRegistration() { }
   
   virtual int OnTrigger(void*) { return (object->*method)(); }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   R (T::*method)();
};

//==============================================================================

template<typename T> class Method0EventRegistration<T, void> : public EventRegistration
{
public:
   Method0EventRegistration(T* aObject, void (T::*aMethod)(), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~Method0EventRegistration() { }
   
   virtual int OnTrigger(void*) { (object->*method)(); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   void (T::*method)();
};

//==============================================================================

template<typename T, typename R> class MethodVEventRegistration : public EventRegistration
{
public:
   MethodVEventRegistration(T* aObject, R (T::*aMethod)(void*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodVEventRegistration() { }

   virtual int OnTrigger(void* userData) { return (object->*method)(userData); }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }

private:
   T* object;
   R (T::*method)(void*);
};

//==============================================================================

template<typename T> class MethodVEventRegistration<T, void> : public EventRegistration
{
public:
   MethodVEventRegistration(T* aObject, void (T::*aMethod)(void*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodVEventRegistration() { }

   virtual int OnTrigger(void* userData) { (object->*method)(userData); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }

private:
   T* object;
   void (T::*method)(void*);
};

//==============================================================================

template<typename T, typename R> class MethodREventRegistration : public EventRegistration
{
public:
   MethodREventRegistration(T* aObject, R (T::*aMethod)(const EventRegistration*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodREventRegistration() { }
   
   virtual int OnTrigger(void*) { return (object->*method)(this); }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   R (T::*method)(const EventRegistration*);
};

//==============================================================================

template<typename T> class MethodREventRegistration<T, void> : public EventRegistration
{
public:
   MethodREventRegistration(T* aObject, void (T::*aMethod)(const EventRegistration*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodREventRegistration() { }
   
   virtual int OnTrigger(void*) { (object->*method)(this); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   void (T::*method)(const EventRegistration*);
};

//==============================================================================

template<typename T, typename R> class MethodVREventRegistration : public EventRegistration
{
public:
   MethodVREventRegistration(T* aObject, R (T::*aMethod)(void*, const EventRegistration*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodVREventRegistration() { }
   
   virtual int OnTrigger(void* userData) { return (object->*method)(userData, this); }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   R (T::*method)(void*, const EventRegistration*);
};

//==============================================================================

template<typename T> class MethodVREventRegistration<T, void> : public EventRegistration
{
public:
   MethodVREventRegistration(T* aObject, void (T::*aMethod)(void*, const EventRegistration*), int _priority)
   {
      object = aObject;
      method = aMethod;
      priority = _priority;
   }
   virtual ~MethodVREventRegistration() { }
   
   virtual int OnTrigger(void* userData) { (object->*method)(userData, this); return 0; }
   virtual inline const void* HandlerGet() const { return (void*)object; }
   virtual inline const void* ObjectGet() const { return (void*)object; }
   
private:
   T* object;
   void (T::*method)(void*, const EventRegistration*);
};

//==============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__EVENTREGISTRATION_H__
