#ifndef __FROG__REFLECTION_H__
#define __FROG__REFLECTION_H__

#include "FrogMemory.h"
#include "JSONValue.h"

#include <typeinfo>

// The following macros are used for annotating classes and members for
// GenerateReflection.lua.

#define FROG_OBJECT
#define FROG_OBJECT_ABSTRACT
#define FROG_FIELD
#define FROG_METHOD_INIT
#define FROG_METHOD_DEINIT
#define FROG_REFLECTION_DECLARATIONS static void ReflectionRegisterGenerated(); \
   virtual const char* ReflectionInstancePortableTypeNameGet();

/// Portable type name name stored in JSON serializations of ReflectionObjects
#define FROG_REFLECTION_OBJECT_PORTABLE_TYPE_KEY "___Type"

namespace Webfoot {

//==============================================================================

/// Base class for types registered with the reflection system.
class ReflectionType
{
public:
   ReflectionType()
   {
      typeName = NULL;
      pointerTypeName = NULL;
   }
   
   virtual ~ReflectionType() {}

   /// Initialize this object to represent the type specified in 'ObjectType'.
   /// '_portableTypeName' should give a string representation of the type
   /// which can be used consistently across platforms.
   template<typename ObjectType>
   void Init(const char* _portableTypeName)
   {
      portableTypeName = StringClone(_portableTypeName);
      typeName = StringClone(typeid(ObjectType).name());
      pointerTypeName = StringClone(typeid(ObjectType*).name());
   }

   virtual void Deinit()
   {
      StringDelete(pointerTypeName);
      StringDelete(typeName);
      StringDelete(portableTypeName);
   }

   /// Return a compiler-defined string representation of this type's name.
   const char* TypeNameGet() { return typeName; }
   /// Return a compiler-defined string representation of a pointer to an item
   /// of this type.
   const char* PointerTypeNameGet() { return pointerTypeName; }
   /// Return a a string representation of the type which can be used
   /// consistently across platforms.
   const char* PortableTypeNameGet() { return portableTypeName; }

   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) = 0;
   /// Initialize the given instance of the type.
   virtual void InstanceInit(void* _item, void* userData) { (void)_item; (void)userData; }
   /// Deinitialize the given instance of the type.
   virtual void InstanceDeinit(void* _item, void* userData) { (void)_item, (void)userData; }
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData) = 0;
   /// Assuming that the type of the given object is either this type, a type
   /// that inherits from this type, or a parent of this type, return the
   /// ReflectionType for the given object's most derived type.
   virtual ReflectionType* InstanceDerivedTypeGet(void* _item, void* userData) { (void)_item; (void)userData; return this; }

   /// Assuming that the type of the given object is either this type, a type
   /// that inherits from this type, or a parent of this type, return the
   /// ReflectionType that should be used to create the class or struct
   /// specified by 'jsonValue'.
   virtual ReflectionType* JSONPolymorphismTypeGet(JSONValue* jsonValue) { (void)jsonValue; return this; }
   /// Serialize the given object of this type to the given JSONValue.  Return
   /// true if successful.
   virtual bool JSONSave(void* _object, JSONValue* jsonValue, void* userData = NULL) = 0;
   /// Deserialize the given object of this type from the given JSON data.
   /// Return true if successful.
   virtual bool JSONLoad(void* _object, JSONValue* jsonValue, void* userData = NULL) = 0;

protected:
   /// String representation of the type which can be used consistently across
   /// platforms.
   const char* portableTypeName;
   /// Compiler-defined string representation of this type's name.
   const char* typeName;
   /// Compiler-defined string representation of a pointer to an item of this type.
   const char* pointerTypeName;
};

//==============================================================================

/// Base class for data members of ReflectionObjects.
class ReflectionField
{
public:
   ReflectionField()
   {
      name = NULL;
      typeName = NULL;
   }
   
   virtual ~ReflectionField() {}

   void Init(const char* _name, const char* _typeName)
   {
      name = StringClone(_name);
      typeName = StringClone(_typeName);
   }

   virtual void Deinit()
   {
      StringDelete(name);
      StringDelete(typeName);
   }

   /// Return the name of the field in the associated object.
   const char* NameGet() { return name; }
   /// Return the compiler-defined typename for the field.
   const char* TypeNameGet() { return typeName; }

   /// Serialize this field of the given '_owner' object to JSON.  Return true
   /// if successful.
   virtual bool JSONSave(void* _owner, JSONValue* jsonValue, void* userData = NULL) = 0;
   virtual bool JSONLoad(void* _owner, JSONValue* jsonValue, void* userData = NULL) = 0;

protected:
   /// Name of the field in the associated object.
   const char* name;
   /// Compiler-defined typename for the field.
   const char* typeName;
};

//==============================================================================

/// Represents a specific member of a specific object type in the reflection
/// system.
template<typename OwnerType, typename FieldType>
class ReflectionFieldTyped : public ReflectionField
{
public:
   ReflectionFieldTyped()
   {
      field = NULL;
   }

   void Init(FieldType OwnerType::*_field, const char* _name)
   {
      Inherited::Init(_name, typeid(FieldType).name());
      field = _field;
   }

   virtual void Deinit()
   {
      field = NULL;
      Inherited::Deinit();
   }

   virtual bool JSONSave(void* _owner, JSONValue* jsonValue, void* userData = NULL);
   virtual bool JSONLoad(void* _owner, JSONValue* jsonValue, void* userData = NULL);

   typedef ReflectionField Inherited;

protected:
   /// Pointer-to-member for the field in the object.
   FieldType OwnerType::*field;
};

//==============================================================================

/// ReflectionTypeObject is a type of ReflectionType for classes and structs
/// where the members are defined recursively.  You should typically use
/// ReflectionTypeObjectTyped instead when creating registrations for objects.
class ReflectionTypeObject : public ReflectionType
{
public:
   template<typename ObjectType>
   void Init(const char* _portableTypeName)
   {
      fields.Init();
      parentNames.Init();
      Inherited::Init<ObjectType>(_portableTypeName);
   }

   virtual void Deinit();

   /// Register a class as a direct parent of this class.
   template<typename ParentType>
   void ParentAdd()
   {
      parentNames.AddBack(StringClone(typeid(ParentType).name()));
   }

   /// Add a field that will use the default handling for its type.
   template<typename OwnerType, typename FieldType>
   void FieldAdd(FieldType OwnerType::*field, const char* fieldName)
   {
      ReflectionFieldTyped<OwnerType, FieldType>* reflectionField = frog_new ReflectionFieldTyped<OwnerType, FieldType>();
      reflectionField->Init(field, fieldName);
      FieldAdd(reflectionField);
   }

   /// Add a field that may require custom handling.
   void FieldAdd(ReflectionField* reflectionField)
   {
      fields.AddBack(reflectionField);
   }

   /// Return the number of fields of this class, excluding those that are
   /// inherited.
   int FieldCountGet() { return fields.SizeGet(); }

   /// Return a field of this class, excluding those that are inherited.
   ReflectionField* FieldGet(int fieldIndex)
   {
      if((fieldIndex < 0) || (fieldIndex >= fields.SizeGet()))
         return NULL;
      else
         return fields[fieldIndex];
   }

   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) { (void)_heapID; (void)userData; return NULL; }
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData)  { (void)_item; (void)userData; }

   virtual bool JSONSave(void* _object, JSONValue* jsonValue, void* userData = NULL);
   virtual bool JSONLoad(void* _object, JSONValue* jsonValue, void* userData = NULL);

   typedef ReflectionType Inherited;

protected:
   /// Helper function to JSONSave for recursively handling parents.  Return
   /// true if successful.
   bool JSONSaveHelper(void*& _object, JSONValue* jsonValue, void* userData);

   Table<ReflectionField*> fields;
   /// Typenames of the direct parents of this class.
   Table<const char*> parentNames;
};

//==============================================================================

/// ReflectionTypeObjectTypedAbstract is an extension of ReflectionTypeObject which
/// is intended for abstract object types and fills out methods like
/// JSONPolymorphismTypeGet.
template<typename ObjectType>
class ReflectionTypeObjectTypedAbstract : public ReflectionTypeObject
{
public:  
   void Init(const char* _portableTypeName)
   {
      Inherited::Init<ObjectType>(_portableTypeName);
   }
   /// Assuming that the type of the given object is either this type, a type
   /// that inherits from this type, or a parent of this type, return the
   /// ReflectionType for the given object's most derived type.
   virtual ReflectionType* InstanceDerivedTypeGet(void* _item, void* userData);

   /// Assuming that the type of the given object is either this type, a type
   /// that inherits from this type, or a parent of this type, return the
   /// ReflectionType that should be used to create the class or struct
   /// specified by 'jsonValue'.
   virtual ReflectionType* JSONPolymorphismTypeGet(JSONValue* jsonValue);

   typedef ReflectionTypeObject Inherited;
};

//==============================================================================

/// ReflectionTypeObjectTyped is an extension of
/// ReflectionTypeObjectTypedAbstract which fills out methods like
/// InstanceAllocate and InstanceDelete.  It must not be used with abstract
/// object types.
template<typename ObjectType>
class ReflectionTypeObjectTyped : public ReflectionTypeObjectTypedAbstract<ObjectType>
{
public:
   ReflectionTypeObjectTyped()
   {
      instanceInitMethod0 = NULL;
      instanceInitMethod1 = NULL;
      instanceDeinitMethod0 = NULL;
      instanceDeinitMethod1 = NULL;
   }
   
   void Deinit()
   {
      instanceInitMethod0 = NULL;
      instanceInitMethod1 = NULL;
      instanceDeinitMethod0 = NULL;
      instanceDeinitMethod1 = NULL;
      Inherited::Deinit();
   }

   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) { (void)userData; return (void*)(frog_new_ex(_heapID) ObjectType()); }
   /// Initialize the given instance of the type.
   virtual void InstanceInit(void* _item, void* userData)
   {
      ObjectType* item = (ObjectType*)_item;
      
      if(instanceInitMethod1)
         (item->*instanceInitMethod1)(userData);
      else if(instanceInitMethod0)
         (item->*instanceInitMethod0)();
      else 
         Inherited::InstanceInit(_item, userData);
   }
   /// Deinitialize the given instance of the type.
   virtual void InstanceDeinit(void* _item, void* userData)
   {
      ObjectType* item = (ObjectType*)_item;
      
      if(instanceDeinitMethod1)
         (item->*instanceDeinitMethod1)(userData);
      else if(instanceDeinitMethod0)
         (item->*instanceDeinitMethod0)();
      else 
         Inherited::InstanceDeinit(_item, userData);
   }
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData)  { (void)userData; ObjectType*& item = (ObjectType*&)_item; SmartDelete(item); }

   /// Set the method pointer for initializing an item of this type.
   void InstanceInitSet(void (ObjectType::*_instanceInitMethod0)()) { instanceInitMethod1 = NULL; instanceInitMethod0 = _instanceInitMethod0; }
   /// Set the method pointer for initializing an item of this type.
   void InstanceInitSet(void (ObjectType::*_instanceInitMethod1)(void*)) { instanceInitMethod0 = NULL; instanceInitMethod1 = _instanceInitMethod1; }
   /// Set the method pointer for deinitializing an item of this type.
   void InstanceDeinitSet(void (ObjectType::*_instanceDeinitMethod0)()) { instanceDeinitMethod1 = NULL; instanceDeinitMethod0 = _instanceDeinitMethod0; }
   /// Set the method pointer for deinitializing an item of this type.
   void InstanceDeinitSet(void (ObjectType::*_instanceDeinitMethod1)(void*)) { instanceDeinitMethod0 = NULL; instanceDeinitMethod1 = _instanceDeinitMethod1; }

   typedef ReflectionTypeObjectTypedAbstract<ObjectType> Inherited;

protected:
   /// Method pointer for initializing an item of this type.
   void (ObjectType::*instanceInitMethod0)();
   /// Method pointer for initializing an item of this type.
   void (ObjectType::*instanceInitMethod1)(void*);
   /// Method pointer for deinitializing an item of this type.
   void (ObjectType::*instanceDeinitMethod0)();
   /// Method pointer for deinitializing an item of this type.
   void (ObjectType::*instanceDeinitMethod1)(void*);
};

//==============================================================================

class ReflectionTypeInt : public ReflectionType
{
public:
   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) { (void)userData; return (void*)(frog_new_ex(_heapID) int); }
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData)  { (void)userData; int*& item = (int*&)_item; SmartDelete(item); }

   virtual bool JSONSave(void* _object, JSONValue* jsonValue, void* userData = NULL);
   virtual bool JSONLoad(void* _object, JSONValue* jsonValue, void* userData = NULL);
};

//==============================================================================

class ReflectionTypeUnsignedInt : public ReflectionType
{
public:
   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) { (void)userData; return (void*)(frog_new_ex(_heapID) unsigned int); }
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData)  { (void)userData; unsigned int*& item = (unsigned int*&)_item; SmartDelete(item); }

   virtual bool JSONSave(void* _object, JSONValue* jsonValue, void* userData = NULL);
   virtual bool JSONLoad(void* _object, JSONValue* jsonValue, void* userData = NULL);
};

//==============================================================================

/// Represents Tables with elements of a given type.
template<typename ElementType>
class ReflectionTypeTable : public ReflectionType
{
public:
   /// Initialize this object to represent the type specified in 'ObjectType'.
   /// '_portableTypeName' should give a string representation of the type
   /// which can be used consistently across platforms.
   void Init(const char* _portableTypeName)
   {
      Inherited::Init<Table<ElementType> >(_portableTypeName);
   }

   /// Return the compiler-defined string representation of the type of the
   /// individual elements of the table.
   const char* ElementTypeNameGet() { return typeid(ElementType).name(); }
   
   /// Return a newly allocated but uninitialized instance of the type.
   virtual void* InstanceAllocate(HeapID _heapID, void* userData) { (void)userData; return (void*)(frog_new_ex(_heapID) Table<ElementType>); }
   /// Initialize the given instance of the type.
   virtual void InstanceInit(void* _item, void* userData) { (void)userData; if(_item) ((Table<ElementType>*)_item)->Init(); }
   /// Deinitialize the given instance of the type.
   virtual void InstanceDeinit(void* _item, void* userData);
   /// Delete the given deinitialized instance of the type.  If successful, set
   /// the reference to NULL.
   virtual void InstanceDelete(void*& _item, void* userData)  { (void)userData; Table<ElementType>*& item = (Table<ElementType>*&)_item; SmartDelete(item); }
   
   /// Serialize the given object of this type to the given JSONValue.  Return
   /// true if successful.
   virtual bool JSONSave(void* _object, JSONValue* jsonValue, void* userData = NULL);
   /// Deserialize the given object of this type from the given JSON data.
   /// Return true if successful.
   virtual bool JSONLoad(void* _object, JSONValue* jsonValue, void* userData = NULL);

   typedef ReflectionType Inherited;
};

//==============================================================================

/// ReflectionManager manages type information for reflection.  The manager has
/// a collection of registered ReflectionTypes.  Primitive types and a few
/// others have their own implementations of ReflectionType.
/// ReflectionTypeObjects are a more general subclass of ReflectionType for
/// structs and classes, and they can have a set of data members called
/// ReflectionFields.  The registered fields typically also use registered
/// types, which allows recursive definitions of objects.
class ReflectionManager
{
public:
   void Init();
   void Deinit();

   /// Return the ReflectionType, if any, for the object of the given type.
   template<typename ObjectType>
   ReflectionType* TypeGet()
   {
      return TypeGetByTypeName(typeid(ObjectType).name());
   }

   /// Return the ReflectionType, if any, for the object with the given
   /// compiler-defined typename.
   ReflectionType* TypeGetByTypeName(const char* typeName);

   /// Return the ReflectionType, if any, for the object with the given
   /// portable type name.
   ReflectionType* TypeGetByPortableTypeName(const char* typeName);

   /// Assuming ObjectType is a pointer to a type, return the ReflectionType,
   /// if any, for an object of that type.
   template<typename ObjectType>
   ReflectionType* TypeGetByPointerType()
   {
      return TypeGetByPointerTypeName(typeid(ObjectType).name());
   }

   /// Assuming 'typeName' is for a pointer for a given type, return the
   /// ReflectionType, if any, for that compiler-defined typename.
   ReflectionType* TypeGetByPointerTypeName(const char* typeName);

   /// Add the given ReflectionType* as the data for the associated object type.
   void TypeAdd(ReflectionType* reflectionType);

   /// Serialize the given object to the given JSONValue.  Return ture if
   /// successful.
   template<typename ObjectType>
   bool JSONSave(ObjectType* object, JSONValue* jsonValue, void* userData = NULL);
   /// Deserialize the given json data to the given object.  Return true if
   /// successful.
   template<typename ObjectType>
   bool JSONLoad(ObjectType* object, JSONValue* jsonValue, void* userData = NULL);

   /// If ObjectType is not a pointer, attempt to simply deinit the object.  If
   /// ObjectType is a pointer, attempt to deinit it, delete it, and set it to
   /// NULL.
   template<typename ObjectType>
   void InstanceCleanup(ObjectType* object, void* userData = NULL);

   static ReflectionManager instance;

protected:
   /// Collection of type information available to the reflection system.
   Table<ReflectionType*> types;
};

ReflectionManager* const theReflection = &ReflectionManager::instance;

//------------------------------------------------------------------------------

template<typename ObjectType>
bool ReflectionManager::JSONSave(ObjectType* object, JSONValue* jsonValue, void* userData)
{
   if(!object || !jsonValue)
      return false;
   // See if 'object' is a pointer to a registered type.
   const char* typeName = typeid(*object).name();
   ReflectionType* reflectionType = TypeGetByTypeName(typeName);
   if(reflectionType)
      return reflectionType->JSONSave((void*)object, jsonValue, userData);
   // See if 'object' is a pointer to a pointer to a registered type.
   reflectionType = TypeGetByPointerTypeName(typeName);
   // 'typeid' won't necessarily give us the most derived type here.  Instead,
   // use it for a starting point, then use ReflectionType to find the most
   // derived type.
   if(reflectionType)
      reflectionType = reflectionType->InstanceDerivedTypeGet(*(void**)object, userData);
   if(reflectionType)
   {
      if(*((void**)object))
      {
         // If the object is defined, serialize it.
         return reflectionType->JSONSave(*((void**)object), jsonValue, userData);
      }
      else
      {
         // If the object is not defined, return a JSONValue of type null.
         jsonValue->NullSet();
         return true;
      }
   }
   
   WarningPrintf("ReflectionManager::JSONSave -- Unable to save object of type: %s\n", typeid(ObjectType).name());

   // If serialization fails, set the JSONValue to type null.
   jsonValue->NullSet();
   return false;
}

//------------------------------------------------------------------------------

template<typename ObjectType>
bool ReflectionManager::JSONLoad(ObjectType* object, JSONValue* jsonValue, void* userData)
{
   if(!object)
      return false;
   // See if 'object' is simply a pointer to a registered type.
   const char* typeName = typeid(*object).name();
   ReflectionType* reflectionType = TypeGetByTypeName(typeName);
   if(reflectionType)
      return reflectionType->JSONLoad((void*)object, jsonValue, userData);

   // See if 'object' is a pointer to a pointer to a registered type.
   reflectionType = TypeGetByPointerTypeName(typeName);
   ReflectionType* existingInstanceReflectionType = NULL;
   if(reflectionType)
   {
      // Get the appropriate type information for the existing object instance and the requested type.
      existingInstanceReflectionType = reflectionType->InstanceDerivedTypeGet(*(void**)object, userData);
      reflectionType = reflectionType->JSONPolymorphismTypeGet(jsonValue);
   }
   if(reflectionType && existingInstanceReflectionType)
   {
      if(!jsonValue || jsonValue->NullCheck())
      {
         // If the JSON is indicating that the object shouldn't exist, attempt
         // to clean it up.
         if(*(void**)object)
         {
            existingInstanceReflectionType->InstanceDeinit(*(void**)object, userData);
            existingInstanceReflectionType->InstanceDelete(*(void**)object, userData);
         }
         return true;
      }
      else
      {
         // If the object exists, but it's the wrong polymorphic type, first
         // destroy the existing object.
         if((*(void**)object) && (reflectionType != existingInstanceReflectionType))
         {
            existingInstanceReflectionType->InstanceDeinit(*(void**)object, userData);
            existingInstanceReflectionType->InstanceDelete(*(void**)object, userData);
         }

         // If the object doesn't exist, try to create an instance for it.
         if(!(*(void**)object))
         {
            *((void**)object) = reflectionType->InstanceAllocate(HEAP_DEFAULT, userData);
            if(*((void**)object))
               reflectionType->InstanceInit(*((void**)object), userData);
         }

         // If the object exists now, try to deserialize.
         if(*(void**)object)
            return reflectionType->JSONLoad((*(void**)object), jsonValue, userData);
      }
   }

   WarningPrintf("ReflectionManager::JSONLoad -- Unable to load object of type: %s\n", typeid(ObjectType).name());
   return false;
}

//------------------------------------------------------------------------------

template<typename ObjectType>
void ReflectionManager::InstanceCleanup(ObjectType* object, void* userData)
{
   if(!object)
      return;

   // See if 'object' is simply a pointer to a registered type.
   const char* typeName = typeid(*object).name();
   ReflectionType* reflectionType = TypeGetByTypeName(typeName);
   if(reflectionType)
   {
      reflectionType->InstanceDeinit((void*)object, userData);
      return;
   }

   // See if 'object' is a pointer to a pointer to a registered type.
   reflectionType = TypeGetByPointerTypeName(typeName);
   if(reflectionType)
   {
      // If the JSON is indicating that the object shouldn't exist, attempt
      // to clean it up.
      if(*(void**)object)
      {
         reflectionType->InstanceDeinit(*(void**)object, userData);
         reflectionType->InstanceDelete(*(void**)object, userData);
         return;
      }
   }

   WarningPrintf("ReflectionManager::InstanceCleanup -- Unrecognized type: %s\n", typeid(ObjectType).name());
}

//==============================================================================

template<typename OwnerType, typename FieldType>
bool ReflectionFieldTyped<OwnerType, FieldType>::JSONSave(void* _owner, JSONValue* jsonValue, void* userData)
{
   if(!_owner || !field || !jsonValue)
      return false;

   // Use the ReflectionManager to recursively serialize the field.
   return theReflection->JSONSave(&(((OwnerType*)_owner)->*field), jsonValue, userData);
}

//------------------------------------------------------------------------------

template<typename OwnerType, typename FieldType>
bool ReflectionFieldTyped<OwnerType, FieldType>::JSONLoad(void* _owner, JSONValue* jsonValue, void* userData)
{
   if(!_owner || !field)
      return false;

   // Use the ReflectionManager to recursively deserialize the field.
   return theReflection->JSONLoad(&(((OwnerType*)_owner)->*field), jsonValue, userData);
}

//------------------------------------------------------------------------------

template<typename ObjectType>
ReflectionType* ReflectionTypeObjectTypedAbstract<ObjectType>::InstanceDerivedTypeGet(void* _item, void* userData)
{
   (void)userData;
   ObjectType* object = (ObjectType*)_item;
   if(!object)
      return this;
   
   // Get the object's portable type name.
   const char* objectPortableTypeName = object->ReflectionInstancePortableTypeNameGet();
   if(!objectPortableTypeName)
      return this;

   return theReflection->TypeGetByPortableTypeName(objectPortableTypeName);
}

//------------------------------------------------------------------------------

template<typename ObjectType>
ReflectionType* ReflectionTypeObjectTypedAbstract<ObjectType>::JSONPolymorphismTypeGet(JSONValue* jsonValue)
{
   // Make sure we have enough information to check.
   if(!jsonValue || !jsonValue->ObjectCheck() || !portableTypeName)
      return NULL;

   const char* requestedPortableTypeName = jsonValue->Get(FROG_REFLECTION_OBJECT_PORTABLE_TYPE_KEY);
   if(requestedPortableTypeName)
   {
      // If the requested name matches the name for this type, it's a match.
      if(!strcmp(portableTypeName, requestedPortableTypeName))
         return this;
      else
         return theReflection->TypeGetByPortableTypeName(requestedPortableTypeName);
   }
   else
   {
      // If no type name was specified, it's implicitly requesting an instance
      // of a base class.  If this class has no parents, it's already considered
      // a base class.
      if(parentNames.SizeGet() == 0)
         return this;
      else
         return NULL;
   }
}

//==============================================================================

template<typename ElementType>
void ReflectionTypeTable<ElementType>::InstanceDeinit(void* _item, void* userData)
{
   if(!_item)
      return;
   
   // Clean up any items that are still in the collection.
   Table<ElementType>& object = *(Table<ElementType>*)_item;
   int elementCount = object.SizeGet();
   for(int elementIndex = 0; elementIndex < elementCount; elementIndex++)
      theReflection->InstanceCleanup(&(object[elementIndex]), userData);

   object.Deinit();
}

//------------------------------------------------------------------------------

template<typename ElementType>
bool ReflectionTypeTable<ElementType>::JSONSave(void* _object, JSONValue* jsonValue, void* userData)
{
   if(!_object || !jsonValue)
      return false;
   Table<ElementType>& object = *((Table<ElementType>*)_object);

   // Set the JSONValue to an array, if needed.
   if(!jsonValue->ArrayCheck())
      jsonValue->ArraySet();

   // Set the size of the array.
   int elementCount = object.SizeGet();
   jsonValue->SizeSet(elementCount);

   // Serialize the individual elements of the table.
   bool successful = true;
   for(int elementIndex = 0; elementIndex < elementCount; elementIndex++)
   {
      if(!theReflection->JSONSave(&(object[elementIndex]), jsonValue->Get(elementIndex), userData))
         successful = false;
   }

   return successful;
}

//------------------------------------------------------------------------------

template<typename ElementType>
bool ReflectionTypeTable<ElementType>::JSONLoad(void* _object, JSONValue* jsonValue, void* userData)
{
   if(!_object || !jsonValue || !jsonValue->ArrayCheck())
      return false;

   bool success = true;

   // Get the number of items in the collection before and after
   // deserialization.
   Table<ElementType>& object = *((Table<ElementType>*)_object);
   int jsonElementCount = jsonValue->SizeGet();
   int oldElementCount = object.SizeGet();

   // If elements need to be removed from the Table, clean them up now.
   if(oldElementCount > jsonElementCount)
   {
      for(int elementIndex = jsonElementCount; elementIndex < oldElementCount; elementIndex++)
         theReflection->InstanceCleanup(&(object[elementIndex]), userData);
   }

   // Make room for the new items.
   object.SizeSet(jsonElementCount);

   // If there are more items now than before, prepare them.
   if(jsonElementCount > oldElementCount)
   {
      if(theReflection->TypeGetByPointerType<ElementType>())
      {
         // If the items are pointers, initialize those pointers to NULL before loading them.
         for(int elementIndex = oldElementCount; elementIndex < jsonElementCount; elementIndex++)
            reinterpret_cast<void*&>(object[elementIndex]) = NULL;
      }
      else
      {
         // If the new items are not pointers, initialize them before loading.
         ReflectionType* elementType = theReflection->TypeGet<ElementType>();
         if(elementType)
         {
            for(int elementIndex = oldElementCount; elementIndex < jsonElementCount; elementIndex++)
               elementType->InstanceInit((void*)&(object[elementIndex]), userData);
         }
      }
   }

   // Deserialize the elements that are now supposed to exist.
   for(int elementIndex = 0; elementIndex < jsonElementCount; elementIndex++)
   {
      if(!theReflection->JSONLoad(&(object[elementIndex]), jsonValue->Get(elementIndex), userData))
         success = false;
   }

   return success;
}

//------------------------------------------------------------------------------

} //namespace Webfoot {

#endif //#ifndef __FROG__REFLECTION_H__
