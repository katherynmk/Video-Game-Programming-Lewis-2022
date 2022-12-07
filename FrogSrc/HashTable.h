#ifndef __FROG__HASHTABLE_H__
#define __FROG__HASHTABLE_H__

#include "FrogMemory.h"
#include <algorithm>
#include <assert.h>
#include "Allocator.h"

namespace Webfoot {

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType> class HashTable;
template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType> struct HashTableNode;

//===============================================================================

/// \brief An iterator for the HashTable class
///
/// HashTableIterator is a bidirectional in-order iterator of the HashTable
/// class.  It is capable of being at any position in the list as well as one
/// position before the beginning and one position after the end.  If the item
/// to which it is pointing is removed, the iterator becomes invalid unless the
/// item was removed using this iterator as the parameter to HashTable::Remove.
/// It is safer and more efficient to use HashTable for POD items items like
/// pointers.
///
/// \param KeyType is the type of the key used by the HashTable.
/// \param ValueType is the type of the data used by the HashTable.
/// \param HashFunctionType is the type of the function that hashes keys.
/// \param KeysEqualType is the type of the function that returns true if both keys are equivalent.
template<typename KeyType, typename ValueType, typename HashFunctionType = size_t(*)(const KeyType& key),
   typename KeysEqualFunctionType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
class HashTableIterator
{
public:

   /// Set the iterator to begin on the given node of the specified HashTable.
   void Init(HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* _hashTable, HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* _node)
   {
      hashTable = _hashTable;
      node = _node;
   }

   /// Go to the next item in the sequence.
   void Next()
   {
      assert(hashTable);
      assert(node);
      if(node)
         node = node->next;
   }

   /// Go to the previous item in the sequence.
   void Previous()
   {
      assert(hashTable);
      assert(node);
      if(node)
         node = node->previous;
   }

   /// Return true if there is another item in the sequence after this position.
   bool NextCheck()
   {
      assert(hashTable);
      assert(node);
      if(node && hashTable)
         return node != &(hashTable->tail);
      else
         return false;
   }

   /// Return true if there is another item in the sequence before this position.
   bool PreviousCheck()
   {
      assert(hashTable);
      assert(node);
      if(node && hashTable)
         return node != &(hashTable->head);
      else
         return false;
   }

   /// Return a reference to the key at the current position.
   const KeyType& Key()
   {
      assert(node);
      return node->key;
   }

   /// Return a reference to the value at the current position.
   ValueType& Value()
   {
      assert(node);
      return node->value;
   }

   /// Return true if the iterator is within the sequence.  Return false if it
   /// is past the end or before the beginning.  This does not guarantee that
   /// the item at the current position was not removed from the sequence or
   /// that the iterator is not empty.
   bool WithinCheck()
   {
      assert(hashTable);
      assert(node);
      return (node != &(hashTable->head)) && (node != &(hashTable->tail));
   }

   /// Return a copy of this iterator.
   HashTableIterator<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType> Clone()
   {
      HashTableIterator<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType> clone;
      clone.Init(hashTable, node);
      return clone;
   }

protected:

   /// Internal reference to the current node.
   HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* node;
   /// The map over which this will be iterating.
   HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* hashTable;

   friend class HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>;
};

//===============================================================================

/// HashTableNode is simply a node for a single entry in a HashTable.
template<typename KeyType, typename ValueType, typename HashFunctionType = size_t(*)(const KeyType& key),
   typename KeysEqualFunctionType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
struct HashTableNode
{
public:
   void Reset()
   {
      previous = NULL;
      next = NULL;
      collidedPrevious = NULL;
      collidedNext = NULL;
      hash = 0;
   }

   KeyType key;
   ValueType value;
   size_t hash;

   /// Previous node for iterator purposes.
   HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* previous;
   /// Next node for iterator purposes.
   HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* next;
   /// Previous node in the linked list of nodes at a given index in the hash table.
   HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* collidedPrevious;
   /// Next node in the linked list of nodes at a given index in the hash table.
   HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* collidedNext;
};

//===============================================================================

/// HashTable allows you to store items using a hash of a key.  It cannot
/// contain more than one item with the same key.  The keys cannot change while
/// they are in the collection. Be sure to call Deinit when you're finished with
/// the collection and clean up the keys.
///
/// Here is a simple example of creating a HashTable of strings to ints.
///
///  HashTable<const char*,int>* testHashTable = frog_new HashTable<const char*,int>();
///  testHashTable->Init(StringHash, StringsEqualCheck);
///
/// If you need a hash function or key equivalence function with access to
/// non-global data that isn't contained in the keys, use a functor instead of a
/// function pointer.
///
///  class ExampleFunctor
///  {
///  public:
///     /// This is what would actually be called for hashing.
///     size_t operator () (const TestKey& key)
///     {
///        return anotherClass->Hash(key);
///     }
///
///     AnotherClass* anotherClass;
///  };
///
/// Providing a functor to the constructor is pass-by-value, so the syntax would
/// look something like the following.
///  ExampleFunctor exampleFunctor;
///  exampleFunctor->anotherClass = someClass;
///  HashTable<TestKey,TestValue,ExampleFunctor>* exampleHashTable = frog_new HashTable<TestKey,TestValue,ExampleFunctor>();
///  exampleHashTable->Init(exampleFunctor);
///
///  HashTable<TestKey,TestValue,ExampleFunctor>::Iterator iterator = exampleHashTable->Begin();
///
/// If you get tired of specifying the key type, value type, and hash function type
/// every time you declare a complicated map or iterator, try using typedefs.
///  typedef HashTable<TestKey,TestValue,TestHashFunctionType,TestKeysEqual> TestHashTable;
///
/// This simpifies the syntax to something like...
///  ExampleFunctor exampleFunctor;
///  exampleFunctor->anotherClass = someClass;
///  TestHashTable* exampleHashTable = frog_new TestHashTable();
///  exampleHashTable->Init(exampleFunctor)
///
///  TestHashTable::Iterator iterator = exampleHashTable->Begin();
template<typename KeyType, typename ValueType, typename HashFunctionType = size_t(*)(const KeyType& key),
   typename KeysEqualFunctionType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
class HashTable
{
public:
   /// The iterator type for this class.
   typedef HashTableIterator<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType> Iterator;

   /// The node type for this class.
   typedef HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType> Node;

   HashTable()
   {
      size = 0;
      reserveSize = 0;
      availableNodeCount = 0;
      hashFunction = NULL;
      keysEqualFunction = NULL;
      allocator = NULL;
      availableNodeHead = NULL;
      buckets = NULL;
      bucketsSize = 0;
      hashMask = 0;
      deallocationsDeferred = false;
      oldBuckets = NULL;
      head.Reset();
      tail.Reset();
   }

   /// Initialize a new, empty collection.
   void Init(HashFunctionType _hashFunction, KeysEqualFunctionType _keysEqualFunction,
      Allocator* _allocator = theAllocatorDefault)
   {
      hashFunction = _hashFunction;
      keysEqualFunction = _keysEqualFunction;
      allocator = _allocator;
      size = 0;
      reserveSize = 0;
      availableNodeCount = 0;
      availableNodeHead = NULL;
      buckets = NULL;
      bucketsSize = 0;
      hashMask = 0;
      deallocationsDeferred = false;
      oldBuckets = NULL;
      head.Reset();
      tail.Reset();
      head.next = &tail;
      tail.previous = &head;
   }

   /// Clean up the collection.
   void Deinit()
   {
      Clear();
      /// Free all memory allocated by the collection.
      ReserveSizeSet(0);
      if(allocator && buckets)
         allocator->Deallocate(buckets);
      DeallocationsDeferredSet(false);
      buckets = NULL;
      bucketsSize = 0;
      hashMask = 0;
      hashFunction = NULL;
      keysEqualFunction = NULL;
      allocator = NULL;
      head.Reset();
      tail.Reset();
   }

   /// Add the given key/value pair.  Return true if successful and false if
   /// there was already an item in the collection with the specified key.
   bool Add(const KeyType& key, const ValueType& value);

   /// Remove the entry with the given key.  Return true if successful.
   bool Remove(const KeyType& key)
   {
      Node* node = NodeGet(key);
      if(!node || (node == &head) || (node == &tail))
         return false;
      NodeRemove(node);
      return true;
   }

   /// Remove the item at the current position of the iterator.
   /// If successful, return true and set the iterator to the subsequent item.
   bool Remove(Iterator& iterator)
   {
      Node* currentNode = iterator.node;
      if(!currentNode || (iterator.hashTable != this))
         return false;
      if((currentNode == &tail) || (currentNode == &head))
         return false;

      Node* nextNode = currentNode->next;
      NodeRemove(currentNode);
      iterator.Init(this, nextNode);
      return true;
   }

   /// Remove all the entries in the hash table.
   /// This does not necessarily deallocate any memory.
   void Clear()
   {
      // Clear the 'buckets' array.
      if(buckets && bucketsSize)
         memset((void*)buckets, 0, sizeof(Node*) * bucketsSize);

      // Remove all nodes.
      Node* node = head.next;
      while(node != &tail)
      {
         size--;
         Node* nextNode = node->next;
         NodeAvailableSet(node);
         node = nextNode;
      }
      size = 0;
   }

   /// Return a reference to the value for the given key.  If there is no
   /// entry with that key yet, a new one is added.
   ValueType& ValueGet(const KeyType& key) { return (*this)[key]; }
   /// Same as ValueGet
   ValueType& operator[](const KeyType& key);

   /// Return the number of items in the collection.
   int SizeGet() const { return size; }
   /// Return true if the collection is empty.
   bool EmptyCheck() const { return size == 0; }
   /// Return true if no more elements can be added without allocating more memory.
   bool FullCheck() const { return availableNodeCount == 0; }
   /// Return the current number of items for which space is allocated.
   int CapacityGet() const { return size + availableNodeCount; }

   /// Return an iterator at the position of the item with the given key.
   /// If the item is not found, the iterator will be pointing one item past
   /// the end of the collection.
   Iterator Find(const KeyType& key)
   {
      Iterator iterator;
      iterator.Init(this, NodeGet(key));
      return iterator;
   }

   /// Return an iterator pointing the the first element in the collection.
   /// If the collection is empty, it will be pointing one element past the end.
   Iterator Begin()
   {
      Iterator iterator;
      iterator.Init(this, NodeFrontGet());
      return iterator;
   }

   /// Return an iterator pointing the the last element in the collection.
   /// If the collection is empty, it will be pointing one element before the beginning.
   Iterator BeginBack()
   {
      Iterator iterator;
      iterator.Init(this, NodeBackGet());
      return iterator;
   }

   /// Keep at least the given number of items allocated.
   /// If this is called a subsequent time with a lower value,
   /// and unused items are currently allocated, they will be freed
   /// as needed to bring down the number held in reserve.
   void ReserveSizeSet(int _reserveSize);

   /// Return the minimum number of items this collection is currently keeping allocated.
   int ReserveSizeGet() { return reserveSize; }

   /// Set whether allocations should be deferred until Deinit at the latest.
   /// Unless an appropriate reserve size was set early enough, memory can be
   /// deallocated internally in the course of adding and removing items from
   /// the collection.  This can cause memory fragmentation, which can hurt
   /// performance.  This feature can be enabled in situations where you would
   /// rather keep old allocations than fragment the heap.
   void DeallocationsDeferredSet(bool _deallocationsDeferred);
   /// See 'DeallocationsDeferredSet'.
   bool DeallocationsDeferredCheck() { return deallocationsDeferred; }

protected:
   /// Number of items in the collection
   int size;
   /// Keep at least this many nodes allocated.
   int reserveSize;
   /// This many nodes are currently allocated and unused.
   int availableNodeCount;
   /// Pointer to a function or functor used to hash keys.
   HashFunctionType hashFunction;
   /// Pointer to a function or functor used to test whether two keys are
   /// equivalent.
   KeysEqualFunctionType keysEqualFunction;
   /// Pointer to the allocator used to create nodes and tables.
   Allocator* allocator;
   /// Head of the singly-linked list of allocated nodes that are not currently in use.
   Node* availableNodeHead;
   /// Array into which nodes are placed based on the hash of their key.
   Node** buckets;
   /// Size of the 'buckets' array.
   size_t bucketsSize;
   /// Mask to be applied to hashes to get indices in the 'buckets' array.
   size_t hashMask;
   /// True if all deallocations should be deferred until Deinit at the latest.
   bool deallocationsDeferred;
   /// Linked list of old 'buckets' arrays which are awaiting deallocation.
   void** oldBuckets;
   /// Dummy node used for iterators to mark one item before the first one.
   Node head;
   /// Dummy node used for iterators to mark one item past the last one.
   Node tail;

   /// Resize the 'buckets' array to the given size, and place all items into
   /// the new array.  '_bucketsSize' must be a power of 2.
   void BucketsSizeSet(size_t _bucketsSize);

   /// Free any old buckets awaiting deallocation.
   void OldBucketsDeallocate()
   {
      while(oldBuckets)
      {
         void** nextOldBuckets = (void**)*oldBuckets;
         if(allocator)
            allocator->Deallocate((void*)oldBuckets);
         oldBuckets = nextOldBuckets;
      }
   }

   /// Free available nodes if needed to bring the total number of nodes down
   /// toward the reserve size.
   void ExcessNodesDeallocate()
   {
      if((size + availableNodeCount > reserveSize) && availableNodeCount)
      {
         // If too many nodes are allocated, free some of them.
         int numberToFree;
         if(size >= reserveSize)
            numberToFree = availableNodeCount;
         else
            numberToFree = (size + availableNodeCount) - reserveSize;
         for(int i = 0; i < numberToFree; i++)
            NodeDeallocate(NodeAvailableGet());
      }
   }

   /// Search for a node with the given key.  Return a pointer to 'tail' if the
   /// node is not found.
   Node* NodeGet(const KeyType& key);

   /// Helper function for inserting new items
   Node* NodeAdd(Node* newNode, Node* currentNode, bool* success);

   /// Helper function for removing items.
   void NodeRemove(Node* node);

   /// Return a pointer to the first node in the collection or 'tail'
   /// if the container is empty.
   Node* NodeFrontGet()
   {
      if(!size)
         return &tail;
      else
         return head.next;
   }

   /// Return a pointer to the last node in the collection or 'head'
   /// if the container is empty.
   Node* NodeBackGet()
   {
      if(!size)
         return &head;
      else
         return tail.previous;
   }

   /// Return a fresh reset node to be used in the collection.
   Node* NodeAvailableGet()
   {
      Node* newNode;
      if(availableNodeHead)
      {
         // If an allocated node is available, unlink it and use it.
         newNode = availableNodeHead;
         availableNodeHead = availableNodeHead->next;
         availableNodeCount--;
      }
      else
      {
         // If there are no free allocated nodes, allocate a new one.
         newNode = NodeAllocate();
      }

      newNode->Reset();
      return newNode;
   }

   /// Either free the given node or put it in the available list.
   void NodeAvailableSet(Node* oldNode)
   {
      if(deallocationsDeferred || ((size + availableNodeCount) < reserveSize))
      {
         // If we need to keep the node in reserve, add it to the free list
         oldNode->next = availableNodeHead;
         availableNodeHead = oldNode;
         availableNodeCount++;
      }
      else
      {
         // Otherwise, delete it.
         NodeDeallocate(oldNode);
      }
   }

   /// Get a new node from the allocator.
   Node* NodeAllocate()
   {
      //return new Node();

      void* nodeSpace = allocator->Allocate(sizeof(Node));
      Node* newNode = NULL;

      try
      {
         newNode = new (nodeSpace) Node;
      } catch (...) {
         allocator->Deallocate(nodeSpace);
         throw;
      }

      return newNode;
   }

   /// Give the node to the allocator to be freed.
   void NodeDeallocate(Node* node)
   {
      //delete node;
      node->~Node();
      allocator->Deallocate((void*)node);
   }

   friend class HashTableIterator<KeyType, ValueType, HashFunctionType, KeysEqualFunctionType>;
};

//===============================================================================

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
bool HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::Add(const KeyType& key, const ValueType& value)
{
   // See if there is already an entry with this key.
   Node* existingNode = NodeGet(key);
   if(existingNode && (existingNode != &tail))
      return false;

   // See if the buckets array should be resized.
   if(((size_t)size + 1) > bucketsSize)
   {
      size_t newBucketsSize;
      if(!bucketsSize)
         newBucketsSize = 8;
      else
         newBucketsSize = bucketsSize * 2;
      BucketsSizeSet(newBucketsSize);
   }
   if(!buckets || !bucketsSize)
      return false;

   // Create a node for the new entry.
   Node* node = NodeAvailableGet();
   node->key = key;
   node->value = value;
   node->hash = hashFunction(key);

   // Add the new node to the end of the linked list for iterators.
   node->next = &tail;
   node->previous = tail.previous;
   tail.previous->next = node;
   tail.previous = node;

   // Add the new node to the appropriate bucket.
   size_t tableIndex = node->hash & hashMask;
   Node* collidedNode = buckets[tableIndex];
   buckets[tableIndex] = node;
   node->collidedPrevious = NULL;
   node->collidedNext = collidedNode;
   if(collidedNode)
      collidedNode->collidedPrevious = node;
   size++;

   return true;
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
ValueType& HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::operator[](const KeyType& key)
{
   // See if there is already an entry with this key.
   Node* node = NodeGet(key);
   if(node && (node != &tail))
      return node->value;

   // See if the buckets array should be resized.
   if(((size_t)size + 1) > bucketsSize)
   {
      size_t newBucketsSize;
      if(!bucketsSize)
         newBucketsSize = 8;
      else
         newBucketsSize = bucketsSize * 2;
      BucketsSizeSet(newBucketsSize);
   }
   if(!buckets || !bucketsSize)
      return tail.value;

   // Create a node for the new entry.
   node = NodeAvailableGet();
   node->key = key;
   node->value = tail.value;
   node->hash = hashFunction(key);

   // Add the new node to the end of the linked list for iterators.
   node->next = &tail;
   node->previous = tail.previous;
   tail.previous->next = node;
   tail.previous = node;

   // Add the new node to the appropriate bucket.
   size_t tableIndex = node->hash & hashMask;
   Node* collidedNode = buckets[tableIndex];
   buckets[tableIndex] = node;
   node->collidedPrevious = NULL;
   node->collidedNext = collidedNode;
   if(collidedNode)
      collidedNode->collidedPrevious = node;
   size++;

   return node->value;
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
void HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::ReserveSizeSet(int _reserveSize)
{
   int oldReserveSize = reserveSize;
   reserveSize = _reserveSize;

   // Reallocate the buckets array so that it should have enough room to hold
   // the given number of items without further resizing.
   if((reserveSize >= size) && (!deallocationsDeferred || (reserveSize > oldReserveSize)))
      BucketsSizeSet(PowerOf2Ceil(reserveSize));

   if(size + availableNodeCount < reserveSize)
   {
      // If more nodes are needed, allocate them.
      int numberToAllocate = reserveSize - (availableNodeCount + size);
      for(int i = 0; i < numberToAllocate; i++)
      {
         try
         {
            NodeAvailableSet(NodeAllocate());
         }
         catch(...)
         {
            reserveSize -= (numberToAllocate - i);
            throw;
         }
      }
   }
   else if(!deallocationsDeferred)
   {
      // If too many nodes are allocated, free some of them.
      ExcessNodesDeallocate();
   }
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
void HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::DeallocationsDeferredSet(bool _deallocationsDeferred)
{
   deallocationsDeferred = _deallocationsDeferred;

   // If deferral of deallocations has been turned off, free any deferred
   // deallocations.
   if(!deallocationsDeferred)
   {
      OldBucketsDeallocate();
      ExcessNodesDeallocate();
   }
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
void HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::BucketsSizeSet(size_t _bucketsSize)
{
   if(_bucketsSize == bucketsSize)
      return;

   // Clean up the old array for the hash table.
   if(!allocator)
      return;
   if(_bucketsSize && !PowerOf2Check(_bucketsSize))
      return;
   if(buckets)
   {
      // If deallocations are deferred, add this buckets array to the linked
      // list of arrays to deallocate later.
      if(deallocationsDeferred)
      {
         *((void**)buckets) = oldBuckets;
         oldBuckets = (void**)buckets;
      }
      else
      {
         allocator->Deallocate((void*)buckets);
      }
      buckets = NULL;
   }
   if(!_bucketsSize)
   {
      bucketsSize = 0;
      hashMask = 0;
      return;
   }

   // Allocate a new array for the hash table.
   size_t bucketsSizeBytes = sizeof(Node*) * _bucketsSize;
   buckets = (Node**)allocator->Allocate(bucketsSizeBytes);
   if(!buckets)
   {
      bucketsSize = 0;
      hashMask = 0;
      return;
   }
   bucketsSize = _bucketsSize;
   memset((void*)buckets, 0, bucketsSizeBytes);

   // Compute the new mask for converting hashes into indices.  (Use the
   // low-order bits of the hash as the index.)
   hashMask = 0;
   while(true)
   {
      size_t newHashMask = (hashMask << 1) | 1;
      if((newHashMask > hashMask) && (newHashMask < bucketsSize))
         hashMask = newHashMask;
      else
         break;
   }

   // Add each existing item to the new array for the hash table.
   Node* node = head.next;
   while(node != &tail)
   {
      size_t tableIndex = node->hash & hashMask;
      Node* collidedNode = buckets[tableIndex];
      buckets[tableIndex] = node;
      node->collidedPrevious = NULL;
      node->collidedNext = collidedNode;
      if(collidedNode)
         collidedNode->collidedPrevious = node;
      node = node->next;
   }
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
HashTableNode<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>* HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::NodeGet(const KeyType& key)
{
   if(!size || !buckets)
      return &tail;

   // Hash the key and apply the mask to get the index in current 'buckets'
   // array.
   size_t keyHash = hashFunction(key);
   size_t tableIndex = keyHash & hashMask;

   // Look up the item in the 'buckets' array.  Use the linked lists to handle
   // hash collisions.
   Node* searchNode = buckets[tableIndex];
   while(searchNode)
   {
      if(searchNode->hash == keyHash)
      {
         // The full hash matches that of the requested key.  Make sure the
         // keys are equivalent.
         if(keysEqualFunction(key, searchNode->key))
            return searchNode;
      }
      else if((searchNode->hash & hashMask) != tableIndex)
      {
         // The search has gone past the end of the nodes for this entry in
         // the 'buckets' array.
         break;
      }
      searchNode = searchNode->collidedNext;
   }

   // No match was found.
   return &tail;
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename HashFunctionType, typename KeysEqualFunctionType>
void HashTable<KeyType,ValueType,HashFunctionType,KeysEqualFunctionType>::NodeRemove(Node* node)
{
   if(!node || !buckets || (node == &tail) || (node == &head))
      return;

   // Update the 'buckets' array if needed.
   size_t tableIndex = node->hash & hashMask;
   if(buckets[tableIndex] == node)
   {
      // The next node should replace this one as the first entryin the table
      // at this index.
      buckets[tableIndex] = node->collidedNext;
   }

   // Remove the node from the linked lists.
   if(node->next)
      node->next->previous = node->previous;
   if(node->previous)
      node->previous->next = node->next;
   if(node->collidedNext)
      node->collidedNext->collidedPrevious = node->collidedPrevious;
   if(node->collidedPrevious)
      node->collidedPrevious->collidedNext = node->collidedNext;

   size--;
   NodeAvailableSet(node);
}

//===============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__HASHTABLE_H__
