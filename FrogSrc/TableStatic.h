#ifndef __FROG__TABLESTATIC_H__
#define __FROG__TABLESTATIC_H__

#include "FrogMemory.h"
#include "Debug.h"
#include "Utility.h"
#include "Table.h"

namespace Webfoot
{

//===============================================================================

/// \addtogroup DataTypes
/// @{

/** \brief Simple template class to enhance basic static arrays.

   TableStatic is an enhanced array in that it keeps track of the array size, 
   and provides commonly used methods such as sorting, finding, and managing 
   elements in the array.
   It is safer and more efficient to use TableStatic for POD items like pointers.

   \param ValueType is the type of data to be stored.
   \param capacity is the size of the storage. Note that this maximum limit is fixed.
*/
template<typename ValueType, int capacity> class TableStatic
{
public:
   /// Construct an empty TableStatic.
   TableStatic() { size = 0; }

   /// Add the value to the end of the TableStatic.
   void Add(const ValueType &newValue) { AddBack(newValue); }

   /// Add the value at the given index.
   void Add(const ValueType &newValue, int insertionIndex) { AddCount(&newValue, 1, insertionIndex); }

   /// Add the given value to the beginning of the TableStatic
   /// and move all the other elements down.
   void AddFront(const ValueType &newValue) { Add(newValue, 0); }

   /// Add the value to the end of the TableStatic.
   void AddBack(const ValueType &newValue) { Add(newValue, size); }

   /// Add the given values to the end of the TableStatic.
   void AddCount(const ValueType* newValues, int newValueCount) { AddCount(newValues, newValueCount, size); }

   /// Insert the given values at the specified point in the TableStatic.
   void AddCount(const ValueType* newValues, int newValueCount, int insertionIndex);

   /// Return a reference to the given element of the TableStatic.
   ValueType& operator[](size_t i) { return data[i]; }
   /// Return a reference to the given element of the TableStatic.
   const ValueType& operator[](size_t i) const { return data[i]; }
   /// Return a reference to the given element of the TableStatic.
   ValueType& operator[](int i) { return data[i]; }
   /// Return a reference to the given element of the TableStatic.
   const ValueType& operator[](int i) const { return data[i]; }

   /// Return a reference to the first item in the TableStatic.
   ValueType& GetFront()
   {
      assert(size);
      return data[0];
   }
   
   /// Return a reference to the last item in the TableStatic.
   ValueType& GetBack()
   {
      assert(size);
      return data[size-1];
   }

   /// Return a pointer to the first instance of the given value in the TableStatic.
   /// Return NULL if it is not found.
   ValueType* Find(const ValueType &a)
   {
      int index = FindIndex(a);
      if(index != -1)
         return &data[index];
      else
         return NULL;
   }

   /// Return a pointer to the last instance of the given value in the TableStatic.
   /// Return NULL if it is not found.
   ValueType* FindFromBack(const ValueType &a)
   {
      int index = FindIndexFromBack(a);
      if(index != -1)
         return &data[index];
      else
         return NULL;
   }

   /// Return the index of the first instance of the given value in the TableStatic.
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

   /// Return the index of the last instance of the given value in the TableStatic.
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
         for(int destIndex = index; destIndex < (size-1); destIndex++)
            data[destIndex] = data[destIndex+1];
         size--;
      }
   }
   
   /// Remove the value at the given address in the TableStatic.
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

   /// Remove the first item in the TableStatic.
   void RemoveFront() { RemoveIndex(0); }
   
   /// Remove the last item in the TableStatic.
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

   /// Return true if the given value is present in the TableStatic.
   bool Contains(const ValueType &a) const
   {
      for(int i = 0; i < size; i++)
      {
         if(data[i] == a)
            return true;
      }
      return false;
   }

   /// Set the number of elements in the TableStatic.
   void SizeSet(int newCount)
   {
      assert(newCount <= capacity);
      size = std::min(newCount, capacity);
   }

   /// Return the number of items in the TableStatic.
   int SizeGet() const { return size; }

   /// Remove all items from the TableStatic.
   void Clear() { size = 0; }

   /// Return true if the TableStatic has no elements.
   bool EmptyCheck() const { return size == 0; }
   /// Return true if no more elements can be added.
   bool FullCheck() { return capacity == size; }

   /// Return the number of items for which space is allocated.
   int CapacityGet() { return capacity; }

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

   /// Quicksort the items in the TableStatic into ascending order using the < operator.
   void Sort() 
   {
      Sort(TableComparatorDefault<ValueType>);
   }

   /// Quicksort the items in the TableStatic using the given comparator.
   /// The comparator can be a function pointer or functor and works
   /// exactly as it does for the Map class.  See the Map class for
   /// a more detailed explanation of these comparators.
   template<typename ComparatorType>
   void Sort(ComparatorType compare)
   {
      if(size > 1)
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

   /// Perform a binary search to find the index of the given value in the TableStatic.
   /// Return -1 if the value is not found.
   /// This assumes that all the items are already sorted in ascending
   /// order using the < operator.  It also assumes that there are no duplicates
   /// in the table as far as the < operator is concerned.
   int BinarySearch(const ValueType& value)
   {
      return BinarySearch(value, TableComparatorDefault<ValueType>);
   }

   /// Perform a binary search to find the index of the given value in the TableStatic.
   /// Return -1 if the value is not found.
   /// This assumes that all the items are already sorted in ascending
   /// order using the given comparator.  It also assumes that there are no duplicates
   /// in the table as far as the < operator is concerned.
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

protected:
   /// Number of elements in the TableStatic
   int size;
   /// Actual array of elements.
   ValueType data[capacity];

   /// Helper function for sorting the TableStatic.
   template<typename ComparatorType>
   static void QuickSortSpan(ValueType* lower, ValueType* upper, ComparatorType compare);
};

//===============================================================================

template<typename ValueType, int capacity>
template<typename ComparatorType>
int TableStatic<ValueType, capacity>::BinarySearch(const ValueType& value, ComparatorType compare)
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

template<typename ValueType, int capacity>
void TableStatic<ValueType, capacity>::AddCount(const ValueType* newValues, int newValueCount, int insertionIndex)
{
   assert(newValueCount > 0);
   assert(insertionIndex >= 0);
   assert(insertionIndex <= size);
   int newSize = size+newValueCount;
   assert(newSize <= capacity);
   if((newValueCount <= 0) || (insertionIndex < 0) || (insertionIndex > size) || (newSize > capacity))
      return;

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

   size = newSize;
}

//------------------------------------------------------------------------------

template<typename ValueType, int capacity>
template<typename ComparatorType>
void TableStatic<ValueType, capacity>::QuickSortSpan(ValueType* lower, ValueType* upper, ComparatorType compare)
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

template<typename ValueType, int capacity>
template<typename ComparatorType>
void TableStatic<ValueType, capacity>::SortStable(ComparatorType compare, Allocator* tempAllocator)
{
   assert(tempAllocator);
   if(!data || (size < 2))
      return;

   // This should be fine, but one possible improvement would be an optimized
   // merge-sort implementation.

   // Set up a temporary table of all the items
   Table<ValueType> tempTable;
   tempTable.Init(tempAllocator);
   tempTable.AddCount(data, SizeGet());
   
   // Create a table that will store pointers to all the elements to be sorted.
   Table<ValueType*> sortTable;
   sortTable.Init(tempAllocator);
   sortTable.SizeSet(SizeGet());
   for(int index = 0; index < SizeGet(); index++)
      sortTable[index] = &(tempTable[index]);
      
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

/// @}
} // namespace Webfoot

#endif //#ifndef __FROG__TABLESTATIC_H__
