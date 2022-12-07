#ifndef __FROG__FILE_H__
#define __FROG__FILE_H__

#include "FrogMemory.h"
#include "Port.h"
#include "Utility.h"
#include <stdarg.h>

namespace Webfoot {

//==============================================================================

/// Interface for file objects, typically created by FileManagers.  
/// The destructor will not close the file, so be sure to return this to its FileManager. 
/// Depending on the file system, you may not be able to open certain files for reading. 
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all implementations.
class File
{
public:
   virtual ~File() {}
   
   /// Read the given number of bytes from the file.
   /// Return the number of bytes that were actually read.
   virtual size_t Read(void* data, size_t length) = 0;
   /// Read the given POD-type object from the file.
   /// Return the number of bytes that were actually read.
   template<typename ValueType>
   size_t Read(ValueType& value) { return Read(&value, sizeof(ValueType)); }
   /// Read and return a POD-type object from the file.
   template<typename ValueType>
   ValueType Read() { ValueType value; Read(&value, sizeof(ValueType)); return value; }
   /// Read the given array of POD-type objects from the file.
   /// Return the number of bytes that were actually read.
   template<typename ValueType>
   size_t ReadCount(ValueType* valueArray, size_t count) { return Read(valueArray, sizeof(ValueType)*count); }
   
   /// Write the given number of bytes from 'data' to the file.
   /// Return the number that were actually written.
   /// Not all File implementations support writing.
   virtual size_t Write(const void* /*data*/, size_t /*length*/) { return 0; }
   /// Write the given POD-type object to the file.
   /// Return the number of bytes that were actually written.
   /// Not all File implementations support writing.
   template<typename ValueType>
   size_t Write(const ValueType& value) { return Write(&value, sizeof(ValueType)); }
   /// Write the given array of POD-type objects to the file.
   /// Return the number of bytes that were actually written.
   /// Not all File implementations support writing.
   template<typename ValueType>
   size_t WriteCount(const ValueType* valueArray, size_t count) { return Write(valueArray, sizeof(ValueType)*count); }

   /// Write UTF-8 text to the file.  Return true if successful.
   bool UTF8Printf(const char* format, ...)
   {
      va_list arguments;
      va_start(arguments, format);
      bool success = UTF8Vprintf(format, arguments);
      va_end(arguments);
      return success;
   }
   /// Write UTF-8 text to the file.  Return true if successful.
   bool UTF8Vprintf(const char* format, va_list args)
   {
      const size_t BUFFER_SIZE = 2048;
      char buffer[BUFFER_SIZE];
      if(!UTF8Vsnprintf(buffer, BUFFER_SIZE, format, args))
         return false;
      size_t amountToWrite = strlen(buffer);
      return Write((const void*)buffer, amountToWrite) == amountToWrite;
   }

   /// Flush any pending reads or writes.
   /// This is not applicable to all File implementations.
   /// Return true if successful or not applicable.
   virtual bool Flush() { return true; }
   
   /// Return the length of the file in bytes.
   /// Return -1 if a problem is encountered.
   virtual int64 SizeGet() = 0;
   
   /// Return the current position in the file in bytes.
   /// Return -1 if a problem is encountered.
   virtual int64 Tell() = 0;
   
   /// Possible values for the 'origin' parameter of the 'Seek' method.
   enum FILE_ORIGIN
   {
      /// The beginning of the file.
      FRONT,
      /// The current position of the file.
      CURRENT,
      /// The end of the file.
      BACK
   };
   
   /// Not supported by all File implementations.  Files with numerically
   /// higher priority have higher priority.
   enum Priority
   {
      PRIORITY_NORMAL,
      PRIORITY_PLUS_ONE
   };
   
   /// Seek to the given part of the file relative to the given origin.
   /// Return true if successful.
   virtual bool Seek(int64 offset, FILE_ORIGIN origin) = 0;
   
   /// Return true if the end-of-file flag is set.  This happens
   /// when you try to read past the end of the file.  The flag is
   /// reset to false by calling Seek.
   virtual bool EndCheck() = 0;
   
   /// Set the priority of operations made through this File object.
   /// This is not supported by all File implementations.
   virtual void PrioritySet(Priority priority) { (void)priority; }
   
   /// ReadAsync is not intended for general use and should probably only
   /// be used by platform-specific code internal to Frog.
   /// For most File implementations, it's simply a wrapper for Read,
   /// and not actually asynchronous.  On platforms that need it,
   /// like the Wii, there may be additional rules.  For instance,
   /// on the Wii, 'data' needs to be 32-byte aligned, 'length'
   /// needs to be a multiple of 32, and the 'sourceOffset' must also
   /// be a multiple of 32.  Once you start an asynchronous
   /// read with a File object, don't do anything else with that object
   /// until ReadPendingCheck has returned false.
   virtual void ReadAsync(void* data, size_t length, int64 sourcePosition)
   {
      int64 oldPosition = Tell();
      Seek(sourcePosition, File::FRONT);
      Read(data, length);
      Seek(oldPosition, File::FRONT);
   }
   /// Return true if this file has an asynchronous read that has not yet completed.
   virtual bool ReadPendingCheck() { return false; }
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FILE_H__
