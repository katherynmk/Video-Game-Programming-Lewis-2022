#ifndef __FROG__MAP_H__
#define __FROG__MAP_H__

#include "FrogMemory.h"
#include <algorithm>
#include <assert.h>
#include "Allocator.h"

namespace Webfoot {

template<typename KeyType, typename ValueType, typename ComparatorType> class Map;
template<typename KeyType, typename ValueType, typename ComparatorType> struct MapNode;

//===============================================================================

/// \brief An iterator for the Map class
///
/// MapIterator is a bidirectional in-order iterator of the Map class.  It is
/// capable of being at any position in the list as well as one position
/// before the beginning and one position after the end.  If the item to which
/// it is pointing is removed, the iterator becomes invalid unless the item
/// was removed using this iterator as the parameter to Map::Remove.
/// It is safer and more efficient to use Map for POD items items like pointers.
/// 
/// \param KeyType is the type of the key used by the Map.
/// \param ValueType is the type of the data used by the Map.
/// \param ComparatorType is the type of the comparator by the Map.
template<typename KeyType, typename ValueType, typename ComparatorType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
class MapIterator
{
public:

   /// Set the iterator to begin on the given node of the specified Map.
   void Init(Map<KeyType,ValueType,ComparatorType>* _map, MapNode<KeyType,ValueType,ComparatorType>* _node)
   {
      map = _map;
      node = _node;
   }
   
   /// Go to the next item in the sequence.
   void Next()
   {
      assert(map);
      assert(node);
      node = map->NodeNextGet(node);
   }
   
   /// Go to the previous item in the sequence.
   void Previous()
   {
      assert(map);
      assert(node);
      node = map->NodePreviousGet(node);
   }
   
   /// Return true if there is another item in the sequence after this position.
   bool NextCheck()
   {
      assert(map);
      assert(node);
      return (map->NodeNextGet(node) != &(map->tail));
   }
   
   /// Return true if there is another item in the sequence before this position.
   bool PreviousCheck()
   {
      assert(map);
      assert(node);
      return (map->NodePreviousGet(node) != &(map->head));
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
   
   /// Return true if the iterator is within the sequence.  Return false if it is
   /// past the end or before the beginning.  This does not guarantee that the item
   /// at the current position was not removed from the sequence or that the iterator
   /// is not empty.
   bool WithinCheck()
   {
      assert(map);
      assert(node);
      return (node != &(map->head)) && (node != &(map->tail));
   }

   /// Return a copy of this iterator.
   MapIterator<KeyType,ValueType,ComparatorType> Clone()
   {
      MapIterator<KeyType,ValueType,ComparatorType> clone;
      clone.Init(map, node);
      return clone;
   }

protected:

   /// Internal reference to the current node.
   MapNode<KeyType,ValueType,ComparatorType>* node;
   /// The map over which this will be iterating.
   Map<KeyType,ValueType,ComparatorType>* map;

   friend class Map<KeyType,ValueType,ComparatorType>;
};

//===============================================================================

/// A comparison function for use with Map that simply uses the
/// '<' operator to return true if the first parameter should be
/// sorted lower than the second.
template<typename KeyType>
bool MapComparatorDefault(const KeyType& keyA, const KeyType& keyB)
{
   return keyA < keyB;
}

//===============================================================================

/// MapNode is simply a node for use within the binary tree of a Map.
template<typename KeyType, typename ValueType, typename ComparatorType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
struct MapNode
{
public:
   void Reset()
   {
      level = 1;
      parent = NULL;
      left = NULL;
      right = NULL;
   }

   KeyType key;
   ValueType value;

   int level;
   MapNode<KeyType,ValueType,ComparatorType>* parent;
   MapNode<KeyType,ValueType,ComparatorType>* left;
   MapNode<KeyType,ValueType,ComparatorType>* right;
};

//===============================================================================

/// Map is an AA Tree, which is a type of balanced binary search tree.
/// Compared to a Red Black Tree, it typically performs rotations more frequently,
/// but it's much simpler internally and provides similar performance.
/// A Map cannot contain more than one item with the same key.
/// The keys cannot change while they are in the collection.  More specifically,
/// the sorting order of the keys cannot change while they are in the collection.
/// Be sure to call Deinit when you're finished with the collection.
///
/// Here is a simple example of creating a Map of ints to ints.
///  Map<int,int>* testMap = frog_new Map<int,int>();
///  testMap->Init();
///
/// The default comparator, MapComparatorDefault, simply uses the < operator to compare keys.
/// If this is inadequate, one alternative is to use a custom function for comparing keys.
/// It must return true if the first parameter should be sorted before the second parameter.
/// Otherwise, it must return false.
/// It can be a stand-alone function or a static method of a class.
/// The default function prototype is...
///  bool(*)(const KeyType& keyA, const KeyType& keyB)
///
/// The following is an example of a comparator of this type.
///  bool ExampleComparator(const int& keyA, const int& keyB)
///  {
///     return keyA > keyB;
///  }
///
/// Here is an example of how to create a map that uses this function pointer comparator.
///  Map<int,int>* testMap = frog_new Map<int,int>();
///  testMap->Init(ExampleComparator);
///
/// If you need a comparator with access to non-global data that isn't contained
/// in the keys, use a functor instead of a function pointer.
///
///  class ExampleFunctor
///  {
///  public:
///     /// This is what would actually be called for comparing keys
///     bool operator () (const TestKey& keyA, const TestKey& keyB)
///     {
///        return anotherClass->Compare(keyA, keyB);
///     }
///  
///     AnotherClass* anotherClass;
///  };
///
/// Providing a functor to the constructor is pass-by-value, so the syntax would 
/// look something like the following.
///  ExampleFunctor exampleFunctor;
///  exampleFunctor->anotherClass = someClass;
///  Map<TestKey,TestValue,ExampleFunctor>* exampleMap = frog_new Map<TestKey,TestValue,ExampleFunctor>();
///  exampleMap->Init(exampleFunctor);
///  
///  Map<TestKey,TestValue,ExampleFunctor>::Iterator iterator = exampleMap->Begin();
///
/// If you get tired of specifying the key type, value type, and comparator type
/// every time you declare a complicated map or iterator, try using typedefs.
///  typedef Map<TestKey,TestValue,TestFunctor> TestMap;
///
/// This simpifies the syntax to something like...
///  ExampleFunctor exampleFunctor;
///  exampleFunctor->anotherClass = someClass;
///  TestMap* exampleMap = frog_new TestMap();
///  exampleMap->Init(exampleFunctor)
/// 
///  TestMap::Iterator iterator = exampleMap->Begin();
template<typename KeyType, typename ValueType, typename ComparatorType = bool(*)(const KeyType& keyA, const KeyType& keyB)>
class Map
{
public:
   /// The default for ComparatorType.
   typedef bool(*DefaultComparatorType)(const KeyType& keyA, const KeyType& keyB);

   /// The iterator type for this class.
   typedef MapIterator<KeyType,ValueType,ComparatorType> Iterator;

   /// The node type for this class.
   typedef MapNode<KeyType,ValueType,ComparatorType> Node;

   Map()
   {
      compare = NULL;
      allocator = NULL;
      size = 0;
      reserveSize = 0;
      availableNodeCount = 0;
      nullNode = NULL;
      root = NULL;
      availableNodeHead = NULL;
      sentinal.Reset();
      head.Reset();
      tail.Reset();
   }

   /// Initialize a new, empty collection.  If you need to use something other than '<'
   /// for the comparator, specify it here.
   void Init(ComparatorType _compare = MapComparatorDefault<KeyType>, Allocator* _allocator = theAllocatorDefault)
   {
      compare = _compare;
      allocator = _allocator;
      size = 0;
      reserveSize = 0;
      availableNodeCount = 0;
      nullNode = &sentinal;
      nullNode->parent = nullNode;
      nullNode->left = nullNode;
      nullNode->right = nullNode;
      nullNode->level = 0;
      root = nullNode;
      availableNodeHead = NULL;
      head.Reset();
      tail.Reset();
   }

   /// Clean up the collection.
   void Deinit()
   {
      Clear();
      /// Free all memory allocated by the collection.
      ReserveSizeSet(0);
   }

   /// Add the given key/value pair.  Return true if successful and false if
   /// there was already an item in the collection with the specified key.
   bool Add(const KeyType& key, const ValueType& value)
   {
      Node* newNode = NodeAvailableGet();
      newNode->key = key;
      newNode->value = value;
      bool success = true;
      root = NodeAdd(newNode, root, &success);
      if(success)
         size++;
      else
         NodeAvailableSet(newNode);
      return success;
   }

   /// Remove the entry with the given key.  Return true if successful.
   bool Remove(const KeyType& key)
   {
      Node* removedNode = nullNode;
      Node* previousNode = nullNode;
      Node* movedNode = nullNode;
      root = NodeRemove(key, root, &removedNode, &previousNode, &movedNode);
      if(removedNode != nullNode)
      {
         // The node was found.
         size--;
         NodeAvailableSet(removedNode);
         return true;
      }
      else
      {
         return false;
      }
   }

   /// Remove the item at the current position of the iterator.
   /// If successful, return true and set the iterator to the subsequent item.
   bool Remove(Iterator& iterator)
   {
      Node* currentNode = iterator.node;
      Node* nextNode = NodeNextGet(currentNode);
      KeyType nextKey = nextNode->key;
      if((currentNode == &tail) || (currentNode == &head))
         return false;

      bool success = Remove(currentNode->key);
      if(success)
      {
         if(nextNode == &tail)
            iterator.Init(this, nextNode);
         else
            iterator.Init(this, NodeGet(nextKey, root));
      }
      return success;
   }

   /// Remove all the entries in the map.
   /// This does not necessarily deallocate any memory.
   void Clear()
   {
      // Remove all the nodes in the tree without worrying about balancing.
      Node* currentNode = NodePostorderFrontGet();
      Node* nextNode;
      while(currentNode != &tail)
      {
         nextNode = NodePostorderNextGet(currentNode);
         NodeAvailableSet(currentNode);
         currentNode = nextNode;
      }

      size = 0;
      root = nullNode;
   }

   /// Return a reference to the value for the given key.  If there is no
   /// entry with that key yet, a new one is added.
   ValueType& ValueGet(const KeyType& key)
   {
      // First, try to find an entry with this key
      Node* node = NodeGet(key, root);
      if(node != &tail)
         return node->value;
      
      // If it's not in there yet, make a new node.
      ValueType dummyValue = nullNode->value;
      Add(key, dummyValue);
      node = NodeGet(key, root);
      return node->value;
   }

   /// Same as ValueGet
   ValueType& operator[](const KeyType& key) { return ValueGet(key); }
   /// Same as ValueGet
   //const ValueType& operator[](const KeyType& key) const { return ValueGet(key); }

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
      iterator.Init(this, NodeGet(key, root));
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

protected:
   /// Number of items in the collection
   int size;
   /// Keep at least this many nodes allocated.
   int reserveSize;
   /// This many nodes are currently allocated and unused.
   int availableNodeCount;
   /// Pointer to a function or functor used to compare the keys in the collection.
   ComparatorType compare;
   /// Pointer to the allocator used to create nodes.
   Allocator* allocator;
   /// Pointer to the current root node
   Node* root;
   /// This node is used in place of NULL.
   Node sentinal;
   /// Pointer to the sential;
   Node* nullNode;
   /// Dummy node used for iterators to mark one item before the first one.
   Node head;
   /// Dummy node used for iterators to mark one item past the last one.
   Node tail;
   /// Head of the singly-linked list of allocated nodes that are not currently in use.
   Node* availableNodeHead;

   /// Search for a node with the given key beginning at the given node.
   /// Return a pointer to 'tail' if the node is not found.
   Node* NodeGet(const KeyType& key, Node* currentNode)
   {
      while(currentNode != nullNode)
      {
         if(compare(key, currentNode->key))
            currentNode = currentNode->left;
         else if(compare(currentNode->key, key))
            currentNode = currentNode->right;
         else
            return currentNode;
      }

      // If this point was reached, the node was not found.
      return &tail;
   }

   /// Helper function for inserting new items
   Node* NodeAdd(Node* newNode, Node* currentNode, bool* success);

   /// Helper function for removing items.  At the end of the recursive call, 
   /// 'removedNode' will be set to point to the node that was taken out of the tree.
   /// 'previousNode' helps determine when we're on our way back up the tree.
   /// 'movedNode' is used to correct local pointers when a node higher in the tree was removed.
   Node* NodeRemove(const KeyType& key, Node* currentNode, Node** removedNode, Node** previousNode, Node** movedNode);
   
   /// AA Tree Balancing helper function
   Node* Skew(Node* currentNode)
   {
      if(currentNode == nullNode)
         return nullNode;

      Node* leftChild = currentNode->left;
      if((leftChild != nullNode) && (leftChild->level == currentNode->level))
      {
         currentNode->left = leftChild->right;
         if(currentNode->left != nullNode)
            currentNode->left->parent = currentNode;
         leftChild->right = currentNode;
         currentNode->parent = leftChild;
         return leftChild;
      }
      else
      {
         return currentNode;
      }
   }

   /// AA Tree Balancing helper function
   Node* Split(Node* currentNode)
   {
      if(currentNode == nullNode)
         return nullNode;

      if(currentNode->right->right->level == currentNode->level)
      {
         Node* rightChild = currentNode->right;
         if(rightChild != nullNode)
         {
            currentNode->right = rightChild->left;
            if(currentNode->right != nullNode)
               currentNode->right->parent = currentNode;
            rightChild->left = currentNode;
            currentNode->parent = rightChild;
            currentNode = rightChild;
         }
         currentNode->level++;
      }

      return currentNode;
   }

   /// Return a pointer to the first node in the collection or 'tail'
   /// if the container is empty.
   Node* NodeFrontGet()
   {
      if(!size)
         return &tail;

      // Just go as left as possible.
      Node* currentNode = root;
      while(currentNode->left != nullNode)
         currentNode = currentNode->left;

      return currentNode;
   }

   /// Return a pointer to the last node in the collection or 'head'
   /// if the container is empty.
   Node* NodeBackGet()
   {
      if(!size)
         return &head;

      // Just go as right as possible.
      Node* currentNode = root;
      while(currentNode->right != nullNode)
         currentNode = currentNode->right;

      return currentNode;
   }

   /// Return the next in-order item in the series or a pointer to 'tail' if there is no next element.
   Node* NodeNextGet(Node* currentNode);

   /// Return the previous in-order item in the series or a pointer to 'head' if there is no previous element.
   Node* NodePreviousGet(Node* currentNode);

   /// Return the first node in the tree for a postorder traversal.
   Node* NodePostorderFrontGet()
   {
      // If there are no elements, go past the end.
      if(!size)
         return &tail;

      // Find the bottom-most node when trying to keep to the left.
      Node* currentNode = root;
      while(true)
      {
         if(currentNode->left != nullNode)
            currentNode = currentNode->left;
         else if(currentNode->right != nullNode)
            currentNode = currentNode->right;
         else
            return currentNode;
      }
   }

   /// Get the next node the tree for a postorder traversal.
   /// This means all descendants of a given node are reached before that node is reached.
   Node* NodePostorderNextGet(Node* currentNode);

   /// Return a fresh reset node to be used in the collection.
   Node* NodeAvailableGet()
   {
      Node* newNode;
      if(availableNodeHead)
      {
         // If an allocated node is available, unlink it and use it.
         newNode = availableNodeHead;
         availableNodeHead = availableNodeHead->right;
         availableNodeCount--;
      }
      else
      {
         // If there are no free allocated nodes, allocate a new one.
         newNode = NodeAllocate();
      }

      newNode->Reset();
      newNode->left = nullNode;
      newNode->right = nullNode;
      newNode->parent = nullNode;
      return newNode;
   }

   /// Either free the given node or put it in the available list.
   void NodeAvailableSet(Node* oldNode)
   {
      if((size + availableNodeCount) < reserveSize)
      {
         // If we need to keep the node in reserve, add it to the free list
         oldNode->right = availableNodeHead;
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

   friend class MapIterator<KeyType, ValueType, ComparatorType>;
};

//===============================================================================

template<typename KeyType, typename ValueType, typename ComparatorType>
void Map<KeyType,ValueType,ComparatorType>::ReserveSizeSet(int _reserveSize)
{
   reserveSize = _reserveSize;
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
   else if((size + availableNodeCount > reserveSize) && availableNodeCount)
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

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename ComparatorType>
MapNode<KeyType,ValueType,ComparatorType>* Map<KeyType,ValueType,ComparatorType>::NodeAdd(MapNode<KeyType,ValueType,ComparatorType>* newNode,
   MapNode<KeyType,ValueType,ComparatorType>* currentNode, bool* success)
{
   if(currentNode == nullNode)
      return newNode;
   else if(compare(newNode->key, currentNode->key))
   {
      currentNode->left = NodeAdd(newNode, currentNode->left, success);
      if(currentNode->left != nullNode)
         currentNode->left->parent = currentNode;
   }
   else if(compare(currentNode->key, newNode->key))
   {
      currentNode->right = NodeAdd(newNode, currentNode->right, success);
      if(currentNode->right != nullNode)
         currentNode->right->parent = currentNode;
   }
   else
   {
      // An entry with this key was already present.
      *success = false;
      return currentNode;
   }

   Node* parentNode = currentNode->parent;
   currentNode = Skew(currentNode);
   if(currentNode != nullNode)
   {
      currentNode->parent = parentNode;
      currentNode = Split(currentNode);
      if(currentNode != nullNode)
         currentNode->parent = parentNode;
   }
   return currentNode;
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename ComparatorType>
MapNode<KeyType,ValueType,ComparatorType>* Map<KeyType,ValueType,ComparatorType>::NodeRemove(const KeyType& key, 
   MapNode<KeyType,ValueType,ComparatorType>* currentNode,
   MapNode<KeyType,ValueType,ComparatorType>** removedNode,
   MapNode<KeyType,ValueType,ComparatorType>** previousNode,
   MapNode<KeyType,ValueType,ComparatorType>** movedNode)
{
   if(currentNode == nullNode)
      return nullNode;

   *previousNode = currentNode;

   // Find the node to remove, and at the same time look for a node to take
   // its position in the tree.
   if(compare(key, currentNode->key))
   {
      currentNode->left = NodeRemove(key, currentNode->left, removedNode, previousNode, movedNode);
      if((currentNode == *removedNode) && (*movedNode != nullNode))
         currentNode = *movedNode;
      if(currentNode->left != nullNode)
         currentNode->left->parent = currentNode;
   }
   else
   {
      *removedNode = currentNode;
      Node* newRightNode = NodeRemove(key, currentNode->right, removedNode, previousNode, movedNode);
      if((currentNode == *removedNode) && (*movedNode != nullNode))
         currentNode = *movedNode;
      currentNode->right = newRightNode;
      if(currentNode->right != nullNode)
         currentNode->right->parent = currentNode;
   }

   // On the way back up...

   if((currentNode == *previousNode) && (*movedNode == nullNode))
   {
      if((*removedNode != nullNode) && !compare((*removedNode)->key, key) && !compare(key, (*removedNode)->key))
      {
         // Remember the child that will take the place of the current node at the bottom.
         Node* rightChild = currentNode->right;
         
         // Without disturbing the relationships of the other nodes right now,
         // we want to move the key and value from this bottom node to the
         // node that will be removed.  The calling function will want a reference
         // to the node so it can delete it as appropriate.  We'll
         // change 'removedNode' to point to the node that is actually being unlinked.
         if(currentNode != *removedNode)
         {
            *movedNode = currentNode;

            (*movedNode)->level = (*removedNode)->level;
            (*movedNode)->parent = (*removedNode)->parent;
            (*movedNode)->left = (*removedNode)->left;
            if((*movedNode)->left != nullNode)
               (*movedNode)->left->parent = (*movedNode);
            if((*removedNode)->right != (*movedNode))
            {
               (*movedNode)->right = (*removedNode)->right;
               if((*movedNode)->right != nullNode)
                  (*movedNode)->right->parent = (*movedNode);
            }
         }
         
         // Resume the ascent from the right child.
         currentNode = rightChild;
      }
      else
      {
         // If the keys don't match, note that the node was not found.
         *removedNode = nullNode;
      }
   }
   else
   {
      if((currentNode->left->level < currentNode->level - 1) ||
         (currentNode->right->level < currentNode->level - 1))
      {
         currentNode->level--;

         if(currentNode->right->level > currentNode->level)
            currentNode->right->level = currentNode->level;
         Node* parentNode = currentNode->parent;
         currentNode = Skew(currentNode);
         if(currentNode != nullNode)
         {
            currentNode->parent = parentNode;
            currentNode->right = Skew(currentNode->right);
            if(currentNode->right != nullNode)
            {
               currentNode->right->parent = currentNode;
               currentNode->right->right = Skew(currentNode->right->right);
               if(currentNode->right->right != nullNode)
                  currentNode->right->right->parent = currentNode->right;
            }
         }
         parentNode = currentNode->parent;
         currentNode = Split(currentNode);
         if(currentNode != nullNode)
         {
            currentNode->parent = parentNode;
            currentNode->right = Split(currentNode->right);
            if(currentNode->right != nullNode)
            {
               currentNode->right->parent = currentNode;
            }
         }
      }
   }

   return currentNode;
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename ComparatorType>
MapNode<KeyType,ValueType,ComparatorType>* Map<KeyType,ValueType,ComparatorType>::NodeNextGet(MapNode<KeyType,ValueType,ComparatorType>* currentNode)
{
   // If we're past the end or there are no elements, go past the end.
   if((currentNode == nullNode) || (currentNode == &tail) || !size)
      return &tail;

   // If we're before the beginning and there are elements, get the first.
   if(currentNode == &head)
      return NodeFrontGet();

   if(currentNode->right != nullNode)
   {
      // If the node has a right child, find the left-most descendant on the right.
      currentNode = currentNode->right;
      while(currentNode->left != nullNode)
         currentNode = currentNode->left;
      return currentNode;
   }
   else
   {
      // If we need to back up, return the first ancestor of which this
      // node is a descendant on a left side.
      while(currentNode->parent != nullNode)
      {
         if(currentNode->parent->left == currentNode)
            return currentNode->parent;
         else
            currentNode = currentNode->parent;
      }

      // If this point was reached, then we're at the end of the collection.
      return &tail;
   }
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename ComparatorType>
MapNode<KeyType,ValueType,ComparatorType>* Map<KeyType,ValueType,ComparatorType>::NodePreviousGet(MapNode<KeyType,ValueType,ComparatorType>* currentNode)
{
   // If we're before the beginning or there are no elements, go before the beginning.
   if((currentNode == nullNode) || (currentNode == &head) || !size)
      return &head;

   // If we're past the end and there are elements, get the last.
   if(currentNode == &tail)
      return NodeBackGet();

   if(currentNode->left != nullNode)
   {
      // If the node has a left child, find the right-most descendant on the left.
      currentNode = currentNode->left;
      while(currentNode->right != nullNode)
         currentNode = currentNode->right;
      return currentNode;
   }
   else
   {
      // If we need to back up, return the first ancestor of which this
      // node is a descendant on a right side.
      while(currentNode->parent != nullNode)
      {
         if(currentNode->parent->right == currentNode)
            return currentNode->parent;
         else
            currentNode = currentNode->parent;
      }

      // If this point was reached, then we're at the beginning of the collection.
      return &head;
   }
}

//-------------------------------------------------------------------------------

template<typename KeyType, typename ValueType, typename ComparatorType>
MapNode<KeyType,ValueType,ComparatorType>* Map<KeyType,ValueType,ComparatorType>::NodePostorderNextGet(MapNode<KeyType,ValueType,ComparatorType>* currentNode)
{
   // If we're past the end or there are no elements, go past the end.
   if((currentNode == nullNode) || (currentNode == &tail) || !size)
      return &tail;

   // If we're before the beginning and there are elements, get the first.
   if(currentNode == &head)
      return NodePostorderFrontGet();

   if(currentNode->parent != nullNode)
   {
      if(currentNode->parent->left == currentNode)
      {
         if(currentNode->parent->right != nullNode)
         {
            // If this was a left child and the parent has a right child,
            // go to the first post-order node in that part of the tree.
            currentNode = currentNode->parent->right;
            while(true)
            {
               if(currentNode->left != nullNode)
                  currentNode = currentNode->left;
               else if(currentNode->right != nullNode)
                  currentNode = currentNode->right;
               else
                  break;
            }
         }
         else
         {
            // If the parent has no right child, just go to the parent.
            currentNode = currentNode->parent;
         }
      }
      else
      {
         // If this was a right child, go to the parent.
         currentNode = currentNode->parent;
      }
   }
   else
   {
      // If this is the root, we're done.
      currentNode = &tail;
   }
   return currentNode;
}

//===============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__MAP_H__
