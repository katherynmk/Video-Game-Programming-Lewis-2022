#ifndef __FROG__FILEDAT_H__
#define __FROG__FILEDAT_H__

#include "FrogMemory.h"
#include "File.h"
#include "Debug.h"

namespace Webfoot {

//==============================================================================

class FileManagerDat;

/// File from a Wrappers resource file.  Large files are not supported.
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all implementations.
class FileDat : public File
{
public:
   FileDat();
   virtual ~FileDat();
   
   /// Read the given number of bytes from the file.
   /// Return the number of bytes that were actually read.
   virtual size_t Read(void* data, size_t length);
   
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

   typedef File Inherited;
   
protected:
   /// File from the parent file system.
   File* resourceFile;
   /// If not zero, this is the index+1 of the current file in the resource
   /// file's FAT.
   int32 hresource;
   /// FileManager that created this file.
   FileManagerDat* parent;

   friend class FileManagerDat;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEDAT_H__
