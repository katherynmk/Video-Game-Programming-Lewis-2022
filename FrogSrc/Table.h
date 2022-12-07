#ifndef __FROG__TABLE_H__
#define __FROG__TABLE_H__

#include "FrogMemory.h"
#include <algorithm>
#include "Debug.h"
#include "Utility.h"
#include "Allocator.h"
#include "FrogMath.h"

namespace Webfoot
{

//===============================================================================

/// A comparison function for use with Table and TableStatic that simply uses the
/// '<' operator to return true if the first parameter should be
/// sorted lower than the second.
template<typename ValueType>
bool TableComparatorDefault(const ValueType& valueA, const ValueType& valueB)
{
   return valueA < valueB;
}

//===============================================================================

/// ExpansionPolicy that rougly doubles the size of the Table when expansion
/// is needed.
template<typename ValueType> class TableExpansionPolicyDefault
{
public:
   static inline int NewCapacityGet(int oldCapacity) { return oldCapacity*2 + 8; }
};

//===============================================================================

/// Table is a dynamically expanding managed array.
/// 
/// Tables are enhanced arrays in that they keep track of the array size, 
/// and provide commonly used methods such as sorting, finding, and managing 
/// elements in the array.  They can also be used as both stacks and queues.
/// ValueType is the type of data to be stored.
/// ExpansionPolicy a class with a static NewCapacityGet member that returns
/// the new amount to allocate given the current capacity.
/// It is safer and more efficient to use Table for POD items like pointers.
/// Be sure to call Deinit when finished.
template<typename ValueType, typename ExpansionPolicy=TableExpansionPolicyDefault<ValueType> > class Table
{
public:
   Table()
   {
      allocator = NULL;
      size = 0;
      capacity = 0;
      data = NULL;
   }

   /// Create an empty Table.
   void Init(Allocator* _allocator = theAllocatorDefault)
   {
      allocator = _allocator;
   }

   /// Create a clone of the given Table using the given allocator.
   void Init(const Table& t, Allocator* _allocator = theAllocatorDefault)
   {
      data = CopyTableData(t.data, t.size, t.capacity, _allocator);
      allocator = _allocator;
      size = t.size;
      capacity = t.capacity;
   }
   
   void Deinit()
   {
      DestroyAndFreeData(data, size, allocator);
      allocator = NULL;
      size = 0;
      capacity = 0;
      data = NULL;
   }

   /// Create a clone of the given Table.
   Table& operator=(const Table& t)
   {
      ValueType* newData = CopyTableData(t.data, t.size, t.capacity, t.allocator);
      DestroyAndFreeData(data, size, allocator);

      data = newData;
      allocator = t.allocator;
      size = t.size;
      capacity = t.capacity;

      return *this;
   }

   /// Add the value to the end of the Table.
   void Add(const ValueType &newValue) { AddBack(newValue); }

   /// Add the value at the given index.
   void Add(const ValueType &newValue, int insertionIndex) { AddCount(&newValue, 1, insertionIndex); }

   /// Add the given value to the beginning of the Table
   /// and move all the other elements down.
   void AddFront(const ValueType &newValue) { Add(newValue, 0); }

   /// Add the value to the end of the Table.
   void AddBack(const ValueType &newValue) { Add(newValue, size); }

   /// Add the given values to the end of the Table.
   void AddCount(const ValueType* newValues, int newValueCount) { AddCount(newValues, newValueCount, size); }

   /// Insert the given values at the specified point in the Table.
   void AddCount(const ValueType* newValues, int newValueCount, int insertionIndex);

   /// Return a reference to the given element of the Table.
   ValueType& operator[](size_t i) { return data[i]; }
   /// Return a reference to the given element of the Table.
   const ValueType& operator[](size_t i) const { return data[i]; }
   /// Return a reference to the given element of the Table.
   ValueType& operator[](int i) { return data[i]; }
   /// Return a reference to the given element of the Table.
   const ValueType& operator[](int i) const { return data[i]; }

   /// Return a reference to the first item in the Table.
   ValueType& GetFront()
   {
      assert(data);
      return data[0];
   }
   
   /// Return a reference to the last item in the Table.
   ValueType& GetBack()
   {
      assert(data);
      return data[size-1];
   }

   /// Return a pointer to the first instance of the given value in the Table.
   /// Return NULL if it is not found.
   ValueType* Find(const ValueType &a)
   {
      int index = FindIndex(a);
      if(index != -1)
         return &data[index];
      else
         return NULL;
   }

   /// Return a pointer to the last instance of the given value in the Table.
   /// Return NULL if it is not found.
   ValueType* FindFromBack(const ValueType &a)
   {
      int index = FindIndexFromBack(a);
      if(index != -1)
         return &data[index];
      else
         return NULL;
   }

   /// Return the index of the first instance of the given value in the Table.
   /// Return -1 if it is not found.
   int FindIndex(const ValueType &a) const
   {
      for(int i = 0; i < size; i++)
      {
         if(data[i] == a)
            return i;
      }

      return -1;
   }

   /// Return the index of the last instance of the given value in the Table.
   /// Return -1 if it is not found.
   int FindIndexFromBack(const ValueType &a) const
   {
      if(!size)
         return -1;

      int index = size;
      do
      {
         index--;
         if(data[index] == a)
            return static_cast<int>(index);
      }
      while(index);
      
      return -1;
   }

   /// Remove the item at the given index.
   void RemoveIndex(int index)
   {
      assert(index < size);
      assert(index >= 0);
      if((index >= 0) && (index < size))
      {
         // Shift everything above it down.
         for(int destIndex = index; destIndex < (size-1); destIndex++)
            data[destIndex] = data[destIndex+1];
         size--;

         // Since we now have one fewer element, destroy the former top-most element.
         data[size].~ValueType();
      }
   }
   
   /// Remove the value at the given address in the Table.
   void Remove(ValueType *entry) { RemoveIndex((int)(entry-data)); }

   /// Remove the first instance of the given value.
   /// Return true if something is removed.
   bool Remove(const ValueType& value)
   {
      ValueType* p = Find(value);
      if(p)
      {
         Remove(p);
         return true;
      }
      else
      {
         return false;
      }
   }

   /// Remove the first item in the Table.
   void RemoveFront() { RemoveIndex(0); }
   
   /// Remove the last item in the Table.
   void RemoveBack() { RemoveIndex(size-1); }

   /// Move the given item from the old index to the new index.  Shift the other
   /// items forward or backward as needed.
   void Move(int newIndex, int oldIndex)
   {
      if((newIndex == oldIndex) || (newIndex < 0) || (newIndex >= size) ||
         (oldIndex < 0) || (oldIndex >= size))
         return;

      ValueType item = data[oldIndex];
      if(newIndex < oldIndex)
      {
         // Shift items right.
         for(int i = oldIndex; i > newIndex; i--)
            data[i] = data[i-1];
         data[newIndex] = item;
      }
      else //if(newIndex > oldIndex)
      {
         // Shift items left.
         for(int i = oldIndex; i < newIndex; i++)
            data[i] = data[i+1];
         data[newIndex] = item;
      }
   }

   /// Return true if the given value is present in the Table.
   bool Contains(const ValueType &a) const
   {
      for(int i = 0; i < size; i++)
      {
         if(data[i] == a) return true;
      }
      return false;
   }

   /// Set the number of elements in the Table.  Allocate more memory if needed.
   /// This will not reduce amount of space allocated.
   void SizeSet(int newSize)
   {
      if(newSize > capacity)
         CapacitySet(newSize);
      
      // If there are new elements, construct them.
      if(newSize > size)
      {
         for(int index = size; index < newSize; index++)
            new ((void*)(&data[index])) ValueType;
      }

      size = newSize;
   }

   /// Return the number of items in the Table.
   int SizeGet() const { return size; }

   /// Remove all items from the Table.  This will not free any memory.
   void Clear()
   {
      if(data)
      {
         // Destroy all items.
         for(int index = 0; index < size; index++)
            (data[index]).~ValueType();
      }
      size = 0;
   }

   /// Return true if the Table has no elements.
   bool EmptyCheck() const { return size == 0; }
   /// Return true if no more elements can be added without allocating more memory.
   bool FullCheck() { return capacity == size; }

   /// Set the amount of space allocated for the table.
   /// By default, if the requested capacity is less than the current number of elements,
   /// the new capacity will be just enough for the current elements.
   /// However, if 'force' is set to true, the requested capacity will be used regardless
   /// of the current number of elements.
   void CapacitySet(int newCapacity, bool force = false)
   {
      assert(allocator);

      // Don't allow it to lose elements, unless that is specifically requested.
      if(!force)
         newCapacity = std::max(size, newCapacity);
      int newSize = std::min(size, newCapacity);

      // If needed, make a new buffer with the right data.
      ValueType* newData = CopyTableData(data, newSize, newCapacity, allocator);
      DestroyAndFreeData(data, size, allocator);

      data = newData;
      capacity = newCapacity;
      size = newSize;
   }
   
   /// Return the current number of items for which space is allocated.
   int CapacityGet() { return capacity; }

   /// Ensure that at least enough room for 'reserveSize' items is allocated.
   /// This will not reduce the amount of space allocated.
   void Reserve(int reserveSize)
   {
      if(reserveSize > capacity)
         CapacitySet(reserveSize);
   }

   /// Push the given data onto the stack. (Same as AddBack)
   void Push(const ValueType& data) { AddBack(data); }
   /// Pop and return the item on top of the stack (same as GetBack followed by RemoveBack)
   ValueType Pop()
   {
      ValueType& item = GetBack();
      RemoveBack();
      return item;
   }

   /// Return the data on top of the stack, which is also the front of the queue. (Same as GetBack)
   ValueType& Peek() { return GetBack(); }
   
   /// Add the item to the back of the queue
   void Enqueue(const ValueType& data) { AddFront(data); }
   /// Return and remove the data at the front of the queue.  (Same as Pop, or GetBack and RemoveBack)
   ValueType Dequeue() { return Pop(); }

   /// Quicksort the items in the table into ascending order using the < operator.
   void Sort() 
   {
      Sort(TableComparatorDefault<ValueType>);
   }

   /// Quicksort the items in the table using the given comparator.
   /// The comparator can be a function pointer or functor and works
   /// exactly as it does for the Map class.  See the Map class for
   /// a more detailed explanation of these comparators.
   template<typename ComparatorType>
   void Sort(ComparatorType compare)
   {
      if(data && (size > 1))
         QuickSortSpan(data, &data[size-1], compare);
   }

   /// Helper object to SortStable.
   template<typename ComparatorType>
   class SortStableComparatorWrapper
   {
   public:
      SortStableComparatorWrapper(ComparatorType* _compare) { compare = _compare; }

      /// Comparator that is being wrapped.
      ComparatorType* compare;

      /// Compare two items first using the comparator and then using
      /// the pointer values to make it stable.
      bool operator() (ValueType*& a, ValueType*& b)
      {
         if((*compare)(*a, *b))
            return true;
         else if((*compare)(*b, *a))
            return false;
         else
            return a < b;
      }
   };

   /// Sort the items in the table into ascending order using the < operator,
   /// and keep the order of equivalent items consistent.
   /// This makes temporary allocations, so be sure to specify 'tempAllocator' if
   /// the default won't be adequate. 
   void SortStable(Allocator* tempAllocator = theAllocatorTemp) 
   {
      SortStable(TableComparatorDefault<ValueType>, tempAllocator);
   }

   /// Sort the items in the table, and keep the order of equivalent items consistent.
   /// This makes temporary allocations, so be sure to specify 'tempAllocator' if
   /// the default won't be adequate. 
   template<typename ComparatorType>
   void SortStable(ComparatorType compare, Allocator* tempAllocator = theAllocatorTemp);

   /// Return true if the items are sorted in ascending order using the < operator.
   bool SortedCheck() 
   {
      return SortedCheck(TableComparatorDefault<ValueType>);
   }

   /// Return true if the items are sorted using the given comparator.
   /// The comparator can be a function pointer or functor and works
   /// exactly as it does for the Map class.  See the Map class for
   /// a more detailed explanation of these comparators.
   template<typename ComparatorType>
   bool SortedCheck(ComparatorType compare)
   {
      for(int i = 0; i < size-1; ++i)
      {
         if(compare(data[i+1], data[i])) return false;
      }
      return true;
   }

   /// Perform a binary search to find the index of the given value in the Table.
   /// Return -1 if the value is not found.
   /// This assumes that all the items are already sorted in ascending
   /// order using the < operator.  It also assumes that there are no duplicates
   /// in the table as far as the < operator is concerned.
   int BinarySearch(const ValueType& value)
   {
      return BinarySearch(value, TableComparatorDefault<ValueType>);
   }

   /// Perform a binary search to find the index of the given value in the Table.
   /// Return -1 if the value is not found.
   /// This assumes that all the items are already sorted in ascending
   /// order using the given comparator.  It also assumes that there are no duplicates
   /// in the table as far as the comparator is concerned.
   /// The comparator can be a function pointer or functor and works
   /// exactly as it does for the Map class.  See the Map class for
   /// a more detailed explanation of these comparators.
   template<typename ComparatorType>
   int BinarySearch(const ValueType& value, ComparatorType compare);

   /// Randomly swap the elements of the Table.  The number of elements in the
   /// collection should not exceed the maximum value for the random number
   /// generator. 
   void Shuffle()
   {
      for(int index = 0; index < size; index++)
      {
         // Choose a random index with which to swap.
         int otherIndex = (int)FrogMath::Random(size);
         if(index != otherIndex)
            Swap(data[index], data[otherIndex]);
      }
   }

   /// Similar to the other form of Shuffle, but using a specific
   /// RandomNumberGenerator.
   void Shuffle(RandomNumberGenerator* randomNumberGenerator)
   {
      if(!randomNumberGenerator)
         return;

      for(int index = 0; index < size; index++)
      {
         // Choose a random index with which to swap.
         int otherIndex = (int)randomNumberGenerator->Random(size);
         if(index != otherIndex)
            Swap(data[index], data[otherIndex]);
      }
   }

protected:
   /// Number of elements in the Table
   int size;
   /// Maximum number of elements for which memory is currently allocated.
   int capacity;
   /// Actual array of elements.
   ValueType* data;
   /// Pointer to the allocator used to create arrays.
   Allocator* allocator;

   /// Destroy the first 'oldSize' elements of oldData and free it with the given allocator.
   void DestroyAndFreeData(ValueType* oldData, int oldSize, Allocator* _allocator)
   {
      if(oldData)
      {
         // Destroy all the old objects.
         for(int index = 0; index < oldSize; index++)
            oldData[index].~ValueType();
         
         // Free the old memory.
         if(_allocator)
            _allocator->Deallocate((void*)oldData);
      }
   }

   /// Return a buffer that duplicates the data in 'sourceTable' using the allocator
   /// in sourceTable.  If there is nothing to copy, return NULL.
   ValueType* CopyTableData(ValueType* sourceData, int sourceSize, int newCapacity, Allocator* _allocator);

   /// Helper function for sorting the Table.
   template<typename ComparatorType>
   static void QuickSortSpan(ValueType* lower, ValueType* upper, ComparatorType compare);
};

//===============================================================================

template<typename ValueType, typename ExpansionPolicy>
template<typename ComparatorType>
int Table<ValueType, ExpansionPolicy>::BinarySearch(const ValueType& value, ComparatorType compare)
{
   assert(size);
   assert(data);
   if(!size || !data)
      return -1;

   int minIndex = 0;
   int maxIndex = size-1;
   while(minIndex <= maxIndex)
   {
      int range = maxIndex - minIndex;
      int currentIndex = minIndex;
      if(range)
         currentIndex += (range/2);
         
      if(compare(value, data[currentIndex]))
         maxIndex = currentIndex - 1;
      else if(compare(data[currentIndex], value))
         minIndex = currentIndex + 1;
      else
         return currentIndex;
   }

   return -1;
}

//------------------------------------------------------------------------------

template<typename ValueType, typename ExpansionPolicy>
ValueType* Table<ValueType, ExpansionPolicy>::CopyTableData(ValueType* sourceData,
   int sourceSize, int newCapacity, Allocator* _allocator)
{
   if(newCapacity)
   {
      // Use the source table's allocator to get a new block of memory.
      ValueType* newData = (ValueType*)_allocator->Allocate(sizeof(ValueType) * newCapacity);
      if(sourceData)
      {
         int index = 0;
         try
         {
            // Try to create copies in the new array using the copy constructor.
            for(; index < sourceSize; index++)
               new(((void*)&newData[index])) ValueType(sourceData[index]);
         }
         catch(...)
         {
            // If there's a problem in a constructor, destroy anything that did
            // construct correctly and free the newly allocated memory.
            DestroyAndFreeData(newData, index, _allocator);
            throw;
         }
      }

      return newData;
   }
   else
   {
      return NULL;
   }
}

//------------------------------------------------------------------------------

template<typename ValueType, typename ExpansionPolicy>
void Table<ValueType, ExpansionPolicy>::AddCount(const ValueType* newValues, int newValueCount, int insertionIndex)
{
   assert(newValueCount > 0);
   assert(insertionIndex >= 0);
   assert(insertionIndex <= size);
   if((newValueCount <= 0) || (insertionIndex < 0) || (insertionIndex > size))
      return;

   int newSize = size+newValueCount;
   
   if(newSize > capacity)
   {
      int newCapacity = capacity;
      do
         newCapacity = ExpansionPolicy::NewCapacityGet(newCapacity);
      while(newSize > newCapacity);
      ValueType* newData = (ValueType*)allocator->Allocate(sizeof(ValueType) * newCapacity);
      int index = 0;
      try
      {
         // Copy everything before the new item.
         for(; index < insertionIndex; index++)
            new((void*)&newData[index]) ValueType(data[index]);
         
         // Add the new values
         for(int newValueIndex = 0; newValueIndex < newValueCount; newValueIndex++)
         {
            new((void*)&newData[index]) ValueType((ValueType)newValues[newValueIndex]);
            index++;
         }

         // Copy everything after the new item.
         for(; index < newSize; index++)
            new((void*)&newData[index]) ValueType(data[index-newValueCount]);
      }
      catch(...)
      {
         // If there's a problem in a constructor, destroy anything that did
         // construct correctly and free the newly allocated memory.
         DestroyAndFreeData(newData, index, allocator);
         throw;
      }
      
      DestroyAndFreeData(data, size, allocator);

      data = newData;
      capacity = newCapacity;
   }
   else
   {
      // We need to construct a new item at the end, so figure out
      // what the value will be.
      const ValueType* newLastValue;
      int insertionIndexLimit = insertionIndex + newValueCount;
      if(insertionIndex + newValueCount == newSize)
      {
         insertionIndexLimit--;
         newLastValue = (const ValueType*)&newValues[newValueCount-1];
      }
      else
      {      
         newLastValue = &data[size-1];
      }

      // Construct the new last item.
      new ((void*)(&data[newSize-1])) ValueType(*newLastValue);

      // Shift everything after the last new item, excluding the very last item,
      // which is already in place.
      if(size > 1)
      {
         for(int destIndex = newSize-2; destIndex >= insertionIndex+newValueCount; destIndex--)
            data[destIndex] = data[destIndex-newValueCount];
      }

      // Add any remaining new values to be inserted.
      for(int index = insertionIndex; index < insertionIndexLimit; index++)
         data[index] = (ValueType)newValues[index-insertionIndex];
   }
   size = newSize;
}

//------------------------------------------------------------------------------

template<typename ValueType, typename ExpansionPolicy>
template<typename ComparatorType>
void Table<ValueType, ExpansionPolicy>::QuickSortSpan(ValueType* lower, ValueType* upper, ComparatorType compare)
{
   if(lower >= upper) return;

   ValueType &pivotValue = *lower;
   
   // Selected pivot is chosen from Lower, Middle, Upper
   ValueType* alternative = lower + ((upper-lower) >> 1);
   if(lower != alternative)
   {
      if(compare(pivotValue, *alternative)) Swap(pivotValue, *alternative);
      if(compare(*upper, pivotValue)) Swap(pivotValue, *upper);
   }
   else
   {
      if(compare(*upper, *lower)) Swap(*lower, *upper);
      return;
   }

   // Do the Partition  
   ValueType* left   = lower;
   ValueType* right  = upper;
   while(1)
   {
      while(compare(pivotValue, *right)) { right--; }
      
      do
      {
         if(++left >= right) goto EndOfLoop;
      } while(compare(*left, pivotValue));
      
      Swap(*left, *right--);
   }

EndOfLoop:
   Swap(pivotValue, *right);
   ValueType* pivot = right;

   // And recurse
   QuickSortSpan(lower, pivot-1, compare);
   QuickSortSpan(pivot+1, upper, compare);
}

//------------------------------------------------------------------------------

template<typename ValueType, typename ExpansionPolicy>
template<typename ComparatorType>
void Table<ValueType, ExpansionPolicy>::SortStable(ComparatorType compare, Allocator* tempAllocator)
{
   assert(tempAllocator);
   if(!data || (size < 2))
      return;

   // This should be fine, but one possible improvement would be an optimized
   // merge-sort implementation.

   // Set up a temporary table of all the items
   Table<ValueType> tempTable;
   tempTable.Init(*this, tempAllocator);
   
   // Create a table that will store pointers to all the elements to be sorted.
   Table<ValueType*> sortTable;
   sortTable.Init(tempAllocator);
   sortTable.SizeSet(SizeGet());
   for(int index = 0; index < SizeGet(); index++)
      sortTable[index] = &(tempTable.data[index]);
      
   // Set up a comparator wrapper.
   SortStableComparatorWrapper<ComparatorType> comparatorWrapper(&compare);
   
   // Sort the temporary table.
   sortTable.Sort(comparatorWrapper);
   
   // Use the sorted list of pointers to copy everything into the sorted position.
   for(int index = 0; index < SizeGet(); index++)
      (*this)[index] = *(sortTable[index]);
   
   // Clean up the temporary table.
   sortTable.Deinit();
   tempTable.Deinit();
}

//------------------------------------------------------------------------------

} // namespace Webfoot

#endif //#ifndef __FROG__TABLE_H__
