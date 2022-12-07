#ifndef __FROG__LIST_H__
#define __FROG__LIST_H__

#include "FrogMemory.h"
#include "assert.h"
#include "Debug.h"
#include "Allocator.h"

namespace Webfoot
{

template<typename ValueType> class List;
template<typename ValueType> struct ListNode;
   
//===============================================================================

/// \addtogroup DataTypes
/// @{

/** \brief An iterator for the ListStatic class

   ListIterator is a bidirectional iterator of the List and ListStatic classes.  It is
   capable of being at any position in the list as well as one position
   before the beginning and one position after the end. Do not attempt 
   to use the iterator if its current item has been removed unless it 
   was removed using this iterator as a parameter to List::Remove or ListStatic::Remove.

   \param ValueType is the type of data stored by the List or ListStatic.
*/
template<typename ValueType> class ListIterator
{
public:
   /// Internal reference to the current node.
   ListNode<ValueType>* node;
   
   /// Initialize a new iterator that begins on the given node.
   void Init(ListNode<ValueType>* _node) { node = _node; }

   /// Go to the next item in the sequence.
   void Next()
   {
      // Go go the next node
      if(node->next)
         node = node->next;
   }
   
   /// Go to the previous item in the sequence.
   void Previous() 
   {
      // Go go the previous node
      if(node->previous)
         node = node->previous;
   }
   
   /// Return true if there is another item in the sequence after this position.
   bool NextCheck() { return (node->next && node->next->next); }
   
   /// Return true if there is another item in the sequence before this position.
   bool PreviousCheck() { return (node->previous && node->previous->previous); }
   
   /// Return a reference to the value at the current position.
   ValueType& Value()
   {
      return node->data;
   }
   
   /// Return true if the iterator is within the sequence.  Return false if it is
   /// past the end or before the beginning.  This does not guarantee that the item
   /// at the current position was not removed from the sequence or that the iterator
   /// is not empty.
   bool WithinCheck()
   {
      return (node->next && node->previous);
   }
   
   /// Return a copy of this iterator.
   ListIterator<ValueType> Clone()
   {
      ListIterator<ValueType> clone;
      clone.Init(node);
      return clone;
   }
};

//===============================================================================

/** \brief A node of a List or ListStatic

   ListNode is a doubly-linked member of a List or ListStatic.

   \param ValueType is the type of data to be stored.
*/
template<typename ValueType> struct ListNode
{
public:
   /// Previous node in the list or NULL if this is the head.
   ListNode* previous;
   /// Next node in the list or NULL if this is the tail.
   ListNode* next;
   
   /// The data stored by the node
   ValueType data;
};

//===============================================================================

/// \addtogroup DataTypes
/// @{

/** \brief Implementation of a linked list where nodes are kept in the heap.

   Lists are doubly-linked lists where nodes can be preallocated and dynamically
   allocated.  It keeps track of the list length and provides commonly used
   methods such as finding and managing the elements.  It can be used as
   a stack with Push, Pop, and Peek.  It can also be used as a queue
   with Enqueue, Dequeue, and Peek.  The same Peek command is used for both 
   stack and queue purposes.  Most interaction with the list, other than with
   the first or last elements, should be done using ListIterators, which can
   be created using Find, Begin, and BeginBack.
   It is safer and more efficient to use List for POD items like pointers.
   Don't forget to call Deinit when you're finished.

   \param ValueType is the type of data to be stored.
*/
template<typename ValueType> class List
{
public:
   /// The iterator type for this class.
   typedef ListIterator<ValueType> Iterator;

   /// The node type for this class.
   typedef ListNode<ValueType> Node;

   /// Initialize an empty list.
   void Init(Allocator* _allocator = theAllocatorDefault);
   /// Free all the nodes
   void Deinit();
   
   /// Empties the linked list.  This does not change the number of allocated nodes.
   void Clear();
   
   /// Return the number of items in the collection.
   int SizeGet() const { return size; }
   /// Return true if the collection is empty.
   bool EmptyCheck() const { return size == 0; }
   /// Return true if no more elements can be added without allocating more memory.
   bool FullCheck() { return availableNodeCount == 0; }
   /// Return the current number of items for which space is allocated.
   int CapacityGet() { return size + availableNodeCount; }

   /// Keep at least the given number of items allocated.
   /// If this is called a subsequent time with a lower value, 
   /// and unused items are currently allocated, they will be freed
   /// as needed to bring down the number held in reserve.
   void ReserveSizeSet(int _reserveSize);
   /// Return the minimum number of items this collection is currently keeping allocated.
   int ReserveSizeGet() { return reserveSize; }
   
   /// Same as AddBack()
   void Add(const ValueType &data) { AddBack(data); }

   /// Add the item to the end of the linked list.
   void AddBack(const ValueType &data) 
   { 
      // Get a node and set the data
      Node* newNode = NodeAvailableGet();
      newNode->data = data;
      
      // Link the new node to the end of the list
      LinkBack(newNode);
      
      size++;
   }
   
   /// Add the item to the beginning of the linked list.
   void AddFront(const ValueType &data) 
   { 
      // Get a node and set the data
      Node* newNode = NodeAvailableGet();
      newNode->data = data;
      
      // Link the new node to the beginning of the list
      LinkFront(newNode);
      
      size++;
   }
   
   /// Add the given item to the list before the position of the given iterator.
   void AddBefore(Iterator& reference, const ValueType &data)
   {
      assert(reference.node);
      
      // Get a node and set the data
      Node* newNode = NodeAvailableGet();
      newNode->data = data;
      
      // Link the new node before the given node.
      LinkBefore(reference.node, newNode);
      
      size++;
   }

   /// Add the given item to the list after the position of the given iterator.
   void AddAfter(Iterator& reference, const ValueType &data)
   {
      assert(reference.node);
      
      // Get a node and set the data
      Node* newNode = NodeAvailableGet();
      newNode->data = data;
      
      // Link the new node after the given node.
      LinkAfter(reference.node, newNode);
      
      size++;
   }
   
   /// Remove the item at the position of the iterator from the list, and
   /// advance the iterator to the next position.
   void Remove(Iterator& iterator)
   {
      assert(iterator.node);
      
      Node* node = iterator.node;
      iterator.Next();
      NodeRemove(node);
   }
   
   /// Remove the first instances of the given data from the list
   void Remove(const ValueType &data)
   {
      Node* node = NodeFind(data);
      if(node)
         NodeRemove(node);
   }
   
   /// Remove the first item in the list.
   void RemoveFront()
   {
      NodeRemove(head.next);
   }
   
   /// Remove the last item in the list.
   void RemoveBack()
   {
      NodeRemove(tail.previous);
   }
   
   /// Return a reference to the first item in the list.
   ValueType& GetFront()
   {
      assert(head.next && head.next->next);
      return head.next->data;
   }
   
   /// Return a reference to the last item in the list.
   ValueType& GetBack()
   {
      assert(tail.previous && tail.previous->previous);
      return tail.previous->data;
   }
   
   /// Push the given data onto the stack. (Same as AddBack)
   void Push(const ValueType& data) { AddBack(data); }

   /// Pop and return the item on top of the stack (same as GetBack followed by RemoveBack)
   ValueType Pop()
   {
      ValueType item = GetBack();
      RemoveBack();
      return item;
   }
   
   /// Return the data on top of the stack, which is also the front of the queue. (Same as GetBack)
   ValueType& Peek() { return GetBack(); }
   
   /// Add the item to the back of the queue
   void Enqueue(const ValueType& data) { AddFront(data); }
   
   /// Return and remove the data at the front of the queue.  (Same as Pop, or GetBack and RemoveBack)
   ValueType Dequeue() { return Pop(); }
   
   /// Move the node at the position of the iterator to the beginning of the list.
   void MoveToFront(Iterator& iterator)
   {
      assert(iterator.node);
      
      NodeMoveToFront(iterator.node);
   }
   
   /// Move the node at the position of the iterator to the end of the list.
   void MoveToBack(Iterator& iterator)
   {
      assert(iterator.node);
      
      NodeMoveToBack(iterator.node);
   }
   
   /// Swap the positions of the nodes in the given iterators.
   void Swap(Iterator& iteratorA, Iterator& iteratorB)
   {
      assert(iteratorA.node);
      assert(iteratorB.node);
      
      NodesSwap(iteratorA.node, iteratorB.node);
   }
   
   /// Return a reference to the data at the given position.
   /// This is an O(n) operation, and should not generally be used.
   ValueType& GetByIndex(int index);
   
   /// Same as GetByIndex
   ValueType& operator[](size_t i) { return GetByIndex((int)i); }
   /// Same as GetByIndex
   //const ValueType& operator[](size_t i) const { return GetByIndex((int)i); }
   /// Same as GetByIndex
   ValueType& operator[](int i) { return GetByIndex(i); }
   /// Same as GetByIndex
   //const ValueType& operator[](int i) const { return GetByIndex(i); }
   
   /// Return an iterator at the first item in the sequence.
   /// If the list is empty, the iterator will be past the end.
   Iterator Begin()
   {
      Iterator returnVal;
      returnVal.Init(head.next);
      return returnVal;
   }
   
   /// Return an iterator at the last item in the sequence.
   /// If the list is empty, the iterator will be before the beginning.
   Iterator BeginBack()
   {
      Iterator returnVal;
      returnVal.Init(tail.previous);
      return returnVal;
   }
   
   /// Return an iterator at the first instance of the given data.
   /// If the data is not present, the iterator will be positioned
   /// past the end of the list.
   Iterator Find(const ValueType &data)
   {
      Iterator returnVal;
      Node* node = NodeFind(data);
      if(node)
         returnVal.Init(node);
      else
         returnVal.Init(&tail);
      return returnVal;
   }

   /// Return an iterator at the last instance of the given data.
   /// If the data is not present, the iterator will be positioned
   /// past the beginning of the list.
   Iterator FindFromBack(const ValueType &data)
   {
      Iterator returnVal;
      Node* node = NodeFindFromBack(data);
      if(node)
         returnVal.Init(node);
      else
         returnVal.Init(&head);
      return returnVal;
   }
   
   /// Return an iterator at the first instance of the given data using the
   /// given 'reference' iterator as a starting point.  If the data is not 
   /// found on or after its position, the returned iterator will be 
   /// positioned past the end of the list.
   Iterator Find(const ValueType &data, Iterator& reference)
   {
      assert(reference.node);

      Iterator returnVal;
      Node* node = NodeFind(data, reference.node);
      if(node)
         returnVal.Init(node);
      else
         returnVal.Init(&tail);
      return returnVal;
   }

   /// Return an iterator at the last instance of the given data using the
   /// given 'reference' iterator as a starting point.  If the data is not 
   /// found on or before its position, the returned iterator will be 
   /// positioned before the beginning of the list.
   Iterator FindFromBack(const ValueType &data, Iterator& reference)
   {
      assert(reference.node);
      
      Iterator returnVal;
      Node* node = NodeFindFromBack(data, reference.node);
      if(node)
         returnVal.Init(node);
      else
         returnVal.Init(&head);
      return returnVal;
   }

   /// Return true if the given data is in the list.
   bool Contains(const ValueType &data)
   {
      Node* node = NodeFind(data);
      return (node != NULL);
   }
   
protected:
   /// Number of items in the collection
   int size;
   /// Keep at least this many nodes allocated.
   int reserveSize;
   /// This many nodes are currently allocated and unused.
   int availableNodeCount;
   /// Dummy node that goes before the first actual node in the list.
   Node head;
   /// Dummy node that goes after the last actual node in the list.
   Node tail;
   /// Head of the singly-linked list of allocated nodes that are not currently in use.
   Node* availableNodeHead;
   /// Pointer to the allocator used to create nodes.
   Allocator* allocator;
   
   /// Link the given unlinked node to the end of the list.
   void LinkBack(Node* node)
   {
      LinkBefore(&tail, node);
   }
   
   /// Link the given unlinked node to the beginning of the list.
   void LinkFront(Node* node)
   {
      LinkAfter(&head, node);
   }
   
   /// Insert the unlinked 'newNode' node after the linked 'reference' node
   void LinkAfter(Node* reference, Node* newNode)
   {
      assert(reference->next);
      
      reference->next->previous = newNode;
      newNode->next = reference->next;
      newNode->previous = reference;
      reference->next = newNode;
   }
   
   /// Insert the unlinked 'newNode' node before the linked 'reference' node
   void LinkBefore(Node* reference, Node* newNode)
   {
      assert(reference->previous);
      
      reference->previous->next = newNode;
      newNode->previous = reference->previous;
      newNode->next = reference;
      reference->previous = newNode;
   }
   
   /// Remove the node from the linked list, but don't
   /// add it to the available list.
   void NodeUnlink(Node* node)
   {
      assert(node->previous && node->next);
      
      node->previous->next = node->next;
      node->next->previous = node->previous;
   }
   
   /// Remove the given node from the list.
   void NodeRemove(Node* node)
   {
      assert(node);
      
      NodeUnlink(node);
      // Put the node back in the available list.
      NodeAvailableSet(node);
      size--;
   }
   
   /// Move the given node, which is already in the list, to the end.
   void NodeMoveToBack(Node* node)
   {
      // If this is already the last, we're done.
      if(node != tail.previous)
      {
         NodeUnlink(node);
         LinkBack(node);
      }
   }
   
   /// Move the given node, which is already in the list, to the beginning.
   void NodeMoveToFront(Node* node)
   {
      // If this is already the first, we're done.
      if(node != head.next)
      {
         NodeUnlink(node);
         LinkFront(node);
      }
   }

   /// Return the node for the first instance of the given data.
   /// Return NULL if the data is not present.
   Node* NodeFind(const ValueType &data)
   {
      return NodeFind(data, head.next);
   }
   
   /// Return the node for the first instance of the given data
   /// after the given node.  Return NULL if the data is not present.
   Node* NodeFind(const ValueType &data, Node* startNode)
   {
      // Be sure not to check the head node's data.
      if(!startNode->previous)
         startNode = startNode->next;
      
      // Traverse the node looking for the data
      while(startNode->next)
      {
         // If the data is found, return its node.
         if(startNode->data == data)
            return startNode;
         else
            startNode = startNode->next;
      }
      
      // If this point was reached, the data was not found.
      return NULL;
   }

   /// Return the node for the back-most instance of the given data.
   /// Return NULL if the data is not present.
   Node* NodeFindFromBack(const ValueType &data)
   {
      return NodeFindFromBack(data, tail.previous);
   }

   /// Return the node for the back-most instance of the given data
   /// before the given node.  Return NULL if the data is not present.
   Node* NodeFindFromBack(const ValueType &data, Node* startNode)
   {
      // Be sure not to check the tail node's data.
      if(!startNode->next)
         startNode = startNode->previous;
      
      // Traverse the node looking for the data
      while(startNode->previous)
      {
         // If the data is found, return its node.
         if(startNode->data == data)
            return startNode;
         else
            startNode = startNode->previous;
      }
      
      // If this point was reached, the data was not found.
      return NULL;
   }

   /// Return a fresh node to be used in the collection.
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

      return newNode;
   }

   /// Either free the given node or put it in the available list.
   void NodeAvailableSet(Node* oldNode)
   {
      if((size + availableNodeCount) < reserveSize)
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
   
   /// Swap the positions of the given nodes in the list.
   void NodesSwap(Node* nodeA, Node* nodeB);
};

//===============================================================================

template<typename ValueType>
void List<ValueType>::Init(Allocator* _allocator)
{
   // Initially, the list is empty.
   allocator = _allocator;
   size = 0;
   reserveSize = 0;
   availableNodeCount = 0;
   availableNodeHead = NULL;
   head.next = &tail;
   head.previous = NULL;
   tail.next = NULL;
   tail.previous = &head;
}

//-------------------------------------------------------------------------------

template<typename ValueType>
void List<ValueType>::Deinit()
{
   Clear();
   /// Free all memory allocated by the collection.
   ReserveSizeSet(0);
}

//-------------------------------------------------------------------------------

template<typename ValueType>
void List<ValueType>::Clear()
{
   // Add all the nodes in the visible list to the available list.
   Node* current = head.next;
   while(current->next)
   {
      current = current->next;
      NodeAvailableSet(current->previous);
   }
      
   // Reset the visible list
   size = 0;
   head.next = &tail;
   tail.previous = &head;
}

//-------------------------------------------------------------------------------

template<typename ValueType>
void List<ValueType>::ReserveSizeSet(int _reserveSize)
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

template<typename ValueType>
ValueType& List<ValueType>::GetByIndex(int index)
{
   assert(index < size);
   
   Node* node = head.next;
   while(node && index > 0)
   {
      node = node->next;
      index--;
   }
   
   assert(node);
   
   return node->data;
}

//-------------------------------------------------------------------------------

template<typename ValueType>
void List<ValueType>::NodesSwap(Node* nodeA, Node* nodeB)
{
   // Make sure these aren't the head or tail.
   assert(nodeA->next && nodeA->previous);
   assert(nodeB->next && nodeB->previous);
   
   if(nodeA->next == nodeB)
   {
      // -AB-
      nodeA->next = nodeB->next;
      nodeB->next->previous = nodeA;
      
      nodeB->previous = nodeA->previous;
      nodeA->previous->next = nodeB;
      
      nodeA->previous= nodeB;
      nodeB->next = nodeA;
   }
   else if(nodeB->next == nodeA)
   {
      // -BA-
      nodeB->next = nodeA->next;
      nodeA->next->previous = nodeB;
      
      nodeA->previous = nodeB->previous;
      nodeB->previous->next = nodeA;
      
      nodeB->previous= nodeA;
      nodeA->next = nodeB;
   }
   else
   {
      // -A-B- or -B-A- or -A-...-B- or -B-...-A-
   
      // Remember the old links.
      Node* oldANext = nodeA->next;
      Node* oldAPrevious = nodeA->previous;
      
      // Re-link node A
      nodeB->previous->next = nodeA;
      nodeB->next->previous = nodeA;
      nodeA->previous = nodeB->previous;
      nodeA->next = nodeB->next;
      
      // Re-link node B
      oldAPrevious->next = nodeB;
      oldANext->previous = nodeB;
      nodeB->previous = oldAPrevious;
      nodeB->next = oldANext;
   }
}

//-------------------------------------------------------------------------------
/// @}
} // namespace Webfoot

#endif //#ifndef __FROG__LIST_H__
