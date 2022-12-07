#ifndef __FROG__FILERAM_H__
#define __FROG__FILERAM_H__

#include "FrogMemory.h"
#include "File.h"

namespace Webfoot {

class FileRAM : public File
{
public:
   FileRAM() {};
   virtual ~FileRAM() {}
   
   void Init(void* _data, size_t _length);
   
   /// Read the given number of bytes from the file.
   /// Return the number of bytes that were actually read.
   virtual size_t Read(void* data, size_t length);
   
   /// Return the length of the file.
   virtual int64 SizeGet() { return length; }
   
   /// Return the current position in the file in bytes.
   virtual int64 Tell() { return offset; }
   
   /// Seek to the given part of the file relative to the given origin.
   /// Return true if successful.
   virtual bool Seek(int64 offset, FILE_ORIGIN origin);
   
   /// Return true if the file offset is at the end of the file
   virtual bool EndCheck();
   
   typedef File Inherited;
   
protected:
   /// Pointer to the data in memory
   void* data;
   /// Offset from the start position
   int64 offset;
   /// Size of the data in memory
   size_t length;

   friend class FileManagerRAM;
};

//==============================================================================

} //namespace Webfoot {

#endif // #ifdef __FROG__FILERAM_H__
