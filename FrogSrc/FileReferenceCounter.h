#ifndef __FROG__FILEREFERENCECOUNTER_H__
#define __FROG__FILEREFERENCECOUNTER_H__

#include "FrogMemory.h"
#include "Utility.h"
#include "Map.h"

namespace Webfoot {

//==============================================================================

/// Facilitates reference counting of data associated with files.
class FileReferenceCounter
{
public:
   struct Entry
   {
      /// Number of open requests to keep the file loaded.
      int referenceCount;
      /// Data associated with the file. 
      void* data;
      /// Other data associated with the object being reference-counted.
      void* metadata;
   };
   typedef Map<FileKey, Entry*> EntryMap;

   void Init(HeapID _heapID = HEAP_DEFAULT);
   void Deinit();

   /// If an entry exists already at the given key, increment its reference
   /// count, and return the data.  If the entry does not exist, return NULL. 
   void* Increment(const char* filename, FileManager* fileManager);
   /// If an entry exists already for the given data, increment its reference
   /// count and return the data.  If the entry does not exist, return NULL. 
   void* Increment(void* data);
   /// Create a new entry with the given key and data.  An internal copy of the
   /// filename string will be made.  
   void Add(const char* filename, FileManager* fileManager, void* data, void* metadata = NULL);

   /// Return the data with the given key or NULL if it is not in the collection. 
   void* Get(const char* filename, FileManager* fileManager);
   /// Return the data if the data is in the collection or NULL if it is not.
   /// If 'entryMapIterator' is specified and the data is found, set
   /// 'entryMapIterator' to point to the entry for the data in question.
   void* Get(void* data, EntryMap::Iterator* entryMapIterator = NULL);

   /// Decrement the reference count of the item with the given key.  If the
   /// reference count has reached zero, return a pointer to the data.  If not,
   /// return NULL.
   void* Decrement(const char* filename, FileManager* fileManager);
   /// Decrement the reference count of the given data.  If the reference count
   /// has reached zero, return a pointer to the data.  If not, return NULL.
   void* Decrement(void* data);

   /// Return the current number of entries in the collection.
   int EntryCountGet() { return entries.SizeGet(); }
   /// Return a pointer to the collection of reference counted items.
   EntryMap* EntriesGet() { return &entries; }

protected:
   /// Used to make all allocations.
   HeapID heapID;
   /// Collection of reference counted items.
   EntryMap entries;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEREFERENCECOUNTER_H__
