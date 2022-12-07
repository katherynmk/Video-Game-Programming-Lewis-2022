#ifndef __FROG__LISTSTATIC_H__
#define __FROG__LISTSTATIC_H__

#include "FrogMemory.h"
#include "List.h"

namespace Webfoot
{

//===============================================================================

/** \brief Implementation of a linked list where nodes are statically allocated.

   ListStatics are doubly-linked lists where the nodes are statically
   allocated.  It keeps track of the list length and provides commonly used
   methods such as finding and managing the elements.  It can be used as
   a stack with Push, Pop, and Peek.  It can also be used as a queue
   with Enqueue, Dequeue, and Peek.  The same Peek command is used for both 
   stack and queue purposes.  Most interaction with the list, other than with
   the first or last elements, should be done using ListIterators, which can
   be created using Find, Begin, and End.
   It is safer and more efficient to use ListStatic for POD items like pointers.

   \param ValueType is the type of data to be stored.
   \param capacity is the size of the storage. Note that this maximum limit is fixed.
*/
template<typename ValueType, int capacity> class ListStatic
{
public:
   /// The iterator type for this class.
   typedef ListIterator<ValueType> Iterator;

   /// The node type for this class.
   typedef ListNode<ValueType> Node;

   /// Initialize an empty list.
   void Init();
   
   /// Empties the linked list
   void Clear();
   
   /// Return the number of nodes in the list.
   int SizeGet() const { return size; }
   
   /// Return the maximum number of nodes that can be in the list.
   int CapacityGet() const { return capacity; }
   
   /// Return true if there are no elements in the list.
   bool EmptyCheck() const { return (size == 0); }
   
   /// Return true if no more items can be added to the list.
   bool FullCheck() const { return (size == capacity); }
   
   /// Add the item to the end of the linked list.
   void AddBack(const ValueType &data) 
   { 
      // Get a node and set the data
      Node* newNode = AvailablePop();
      newNode->data = data;
      
      // Link the new node to the end of the list
      LinkBack(newNode);
      
      size++;
   }
   
   /// Same as AddBack()
   void Add(const ValueType &data) { AddBack(data); }
   
   /// Add the item to the beginning of the linked list.
   void AddFront(const ValueType &data) 
   { 
      // Get a node and set the data
      Node* newNode = AvailablePop();
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
      Node* newNode = AvailablePop();
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
      Node* newNode = AvailablePop();
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
   Iterator End()
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
   /// Number of nodes
   int size;
   /// Dummy node that goes before the first actual node in the list.
   Node head;
   /// Dummy node that goes after the last actual node in the list.
   Node tail;
   /// Dummy node that goes before the first actual node in the forward-linked
   /// list of unused nodes.
   Node availableHead;
   /// The pool of all this list's nodes.
   Node nodes[capacity];

   /// Put the given node in the available list
   void AvailablePush(Node* node)
   {
      node->next = availableHead.next;
      availableHead.next = node;
   }
   
   /// Remove the first node in the available list, and return a pointer to it.
   Node* AvailablePop()
   {
      assert(availableHead.next);
      
      Node* node = availableHead.next;
      availableHead.next = node->next;
      return node;
   }
   
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
      AvailablePush(node);
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
   
   /// Swap the positions of the given nodes in the list.
   void NodesSwap(Node* nodeA, Node* nodeB);
};

//-------------------------------------------------------------------------------

template<typename ValueType, int capacity>
void ListStatic<ValueType, capacity>::Init()
{
   assert(capacity);
   
   // Initially, the list is empty.
   size = 0;
   head.next = &tail;
   head.previous = NULL;
   tail.next = NULL;
   tail.previous = &head;
   
   // All statically allocated nodes must be linked together to form the 
   // singly-linked available list.
   int i;
   availableHead.next = NULL;
   Node* current = &nodes[0];
   for(i = 0; i < capacity; i++)
      AvailablePush(current++);
}

//-------------------------------------------------------------------------------

template<typename ValueType, int capacity>
void ListStatic<ValueType, capacity>::Clear()
{
   // Add all the nodes in the visible list to the available list.
   Node* current = head.next;
   while(current->next)
   {
      current = current->next;
      AvailablePush(current->previous);
   }
      
   // Reset the visible list
   size = 0;
   head.next = &tail;
   tail.previous = &head;
}

//-------------------------------------------------------------------------------

template<typename ValueType, int capacity>
ValueType& ListStatic<ValueType, capacity>::GetByIndex(int index)
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

template<typename ValueType, int capacity>
void ListStatic<ValueType, capacity>::NodesSwap(Node* nodeA, Node* nodeB)
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

#endif //#ifndef __FROG__LISTSTATIC_H__
