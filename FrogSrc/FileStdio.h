#ifndef __FROG__FILESTDIO_H__
#define __FROG__FILESTDIO_H__

#include "FrogMemory.h"
#include <stdio.h>
#include "File.h"
#include "Debug.h"

namespace Webfoot {

//==============================================================================

class FileManagerStdio;

/// File from a stdio file system with large file support.
/// Don't depend on being able to use large files in general, though.
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all implementations.
class FileStdio : public File
{
public:
   FileStdio();
   virtual ~FileStdio();
   
   /// Read the given number of bytes from the file.
   /// Return the number of bytes that were actually read.
   virtual size_t Read(void* data, size_t length);
   /// Write the given number of bytes from 'data' to the file.
   /// Return the number that were actually written.
   /// Not all File implementations support writing.
   virtual size_t Write(const void* data, size_t length);
   
   /// Flush any pending reads or writes.
   /// This is not applicable to all File implementations.
   /// Return true if successful or not applicable.
   virtual bool Flush();
   
   /// Return the length of the file.
   /// Return -1 if a problem is encountered.
   virtual int64 SizeGet();
   
   /// Return the current position in the file in bytes.
   /// Return -1 if a problem is encountered.
   virtual int64 Tell();
   
   /// Seek to the given part of the file relative to the given origin.
   /// Return true if successful.
   virtual bool Seek(int64 offset, FILE_ORIGIN origin);
   
   /// Return true if the end-of-file flag is set.  This happens
   /// when you try to read past the end of the file.  The flag is
   /// reset to false by calling Seek.
   virtual bool EndCheck();
   
   /// Return the underlying file handle.
   /// This is not cross-platform and should typically only be used in tools.
   FILE* GetFileHandle() { return fileHandle; }

   typedef File Inherited;
   
protected:
   /// Stdio file handle.
   FILE* fileHandle;
   
   friend class FileManagerStdio;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FILESTDIO_H__
