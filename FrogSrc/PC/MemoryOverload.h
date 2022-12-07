#ifndef __FROG__MEMORYOVERLOAD_H__
#define __FROG__MEMORYOVERLOAD_H__

// This should only be #included from Memory.h

void* operator new(size_t size, Webfoot::HeapID heapId) throw();
void operator delete(void* ptr, Webfoot::HeapID heapId) throw();
void* operator new[](size_t size, Webfoot::HeapID heapId) throw();
void operator delete[](void* ptr, Webfoot::HeapID heapId) throw();
void* operator new(size_t size) throw();
void operator delete(void* ptr) throw();
void* operator new[](size_t size) throw();
void operator delete[](void* ptr) throw();

void* operator new(size_t size, Webfoot::HeapID heapId, const char* file, int line) throw();
void operator delete(void* ptr, Webfoot::HeapID heapId, const char* file, int line) throw();
void* operator new[](size_t size, Webfoot::HeapID heapId, const char* file, int line) throw();
void operator delete[](void* ptr, Webfoot::HeapID heapId, const char* file, int line) throw();

#endif //#ifndef __FROG__MEMORYOVERLOAD_H__
