#ifndef __FROG__SAVEMANAGERCOMMON_H__
#define __FROG__SAVEMANAGERCOMMON_H__

#include "FrogMemory.h"
#include "Debug.h"
#include "Map.h"
#include "Table.h"
#include "Utility.h"
#include "ScriptManager.h"

namespace Webfoot {

/// Name of the table in which all saved Lua variables are located.
#define SAVE_MANAGER_VARIABLE_TABLE LUAVAR_TABLE
/// Magic string that appears at the beginning of a slot.
#define SAVE_MANAGER_MAGIC_STRING "GSID"
/// Length of 'SAVE_MANAGER_MAGIC_STRING'
#define SAVE_MANAGER_MAGIC_STRING_LENGTH 4

//==============================================================================

/// Helper class to SaveFileProperties.  This keeps track of the properties of
/// a given slot of a given file.
class SaveSlotProperties
{
public:
   SaveSlotProperties()
   {
      size = 0;
      buffer = NULL;
   }

   /// Set the slot to the given size.  If 'allocateBuffer' is true, a buffer
   /// of the appropriate size will be allocated using the given alignment and heap.
   void Init(size_t _size, bool allocateBuffer = false, HeapID bufferHeapID = HEAP_DEFAULT,
      int bufferAlignment = FROG_MEM_ALIGN);

   void Deinit()
   {
      size = 0;
      SmartFree(buffer);
   }

   /// Return the size specified for this slot.
   inline size_t SizeGet() const { return size; }
   
   /// Return the buffer, if any, allocated for this slot.
   inline void* BufferGet() const { return buffer; }

   /// Set whether to perform a batched write on the next SaveBatchExecute.
   /// This is not used by all SaveManager implementations.
   inline void BatchedSaveRequestedSet(bool _batchedSaveRequested) { batchedSaveRequested = _batchedSaveRequested; }
   /// Return true if a batched write should be performed on the next SaveBatchExecute.
   /// This is not used by all SaveManager implementations.
   inline bool BatchedSaveRequestedCheck() const { return batchedSaveRequested; }
   
   /// Set the amount to write when the batched save is executed.
   /// This is not used by all SaveManager implementations.
   inline void BatchedSaveSizeSet(size_t _batchedSaveSize) { batchedSaveSize = _batchedSaveSize; }
   /// Return the amount to write when the batched save is executed.
   /// This is not used by all SaveManager implementations.
   inline size_t BatchedSaveSizeGet() const { return batchedSaveSize; }
   
   /// Set whether the contents of the buffer match the data that's been recorded.
   /// This is not used by all SaveManager implementations.
   inline void BufferSynchronizedSet(bool _bufferSynchronized) { bufferSynchronized = _bufferSynchronized; }
   /// Return true if the contents of the buffer match the data that's been recorded.
   /// This is not used by all SaveManager implementations.
   inline bool BufferSynchronizedCheck() const { return bufferSynchronized; }
   
protected:
   /// Size of the slot
   size_t size;
   /// Buffer used by some implementations of SaveManager to cache the contents of the slot. 
   void* buffer;
   /// True if the contents of the buffer match the data that's been recorded.
   bool bufferSynchronized;
   /// True if a batched save has been requested for this file.
   bool batchedSaveRequested;
   /// The amount to write when the batched save is executed.
   size_t batchedSaveSize;
};

//==============================================================================

/// Helper class to SaveManager.  This keeps track of the properties of
/// save files and their slots. 
class SaveFileProperties
{
public:
   /// Initialize the file to have the given number of slots.
   void Init(int _slotCount, HeapID _heapID)
   {
      heapID = _heapID;
      slots.Init(theAllocatorHeaps[heapID]);
      slots.Reserve(_slotCount);
      SaveSlotProperties* nullSlot = NULL;
      for(int slotIndex = 0; slotIndex < _slotCount; slotIndex++)
         slots.Add(nullSlot);
   }
   
   void Deinit()
   {
      int slotCount = slots.SizeGet();
      for(int slotIndex = 0; slotIndex < slotCount; slotIndex++)
         SmartDelete(slots[slotIndex]);
      slots.Deinit();
   }

   /// Return the number of slots associated with this file.
   int SlotCountGet() { return slots.SizeGet(); }
   
   /// Prepare the given slot.  If 'allocateBuffer' is true, a buffer
   /// of the appropriate size will be allocated using the given alignment and heap.
   void SlotInit(int slotIndex, size_t slotSize, bool allocateBuffer = false,
      int bufferAlignment = FROG_MEM_ALIGN, HeapID bufferHeapID = HEAP_DEFAULT)
   {
      int slotCount = SlotCountGet();
      assert(slotIndex >= 0);
      assert(slotIndex < slotCount);
      if((slotIndex >= 0) && (slotIndex < slotCount))
      {
         SaveSlotProperties* slot = slots[slotIndex];
         if(slot)
         {
            WarningPrintf("SaveFileProperties::SlotInit -- slot %d has already been initialized.\n", slotIndex);
         }
         else
         {
            slot = frog_new_ex(heapID) SaveSlotProperties();
            slot->Init(slotSize, allocateBuffer, bufferHeapID, bufferAlignment);
            slots[slotIndex] = slot;
         }
      }
   }
   
   /// Return the size of the given slot or 0 if unsuccessful.
   size_t SlotSizeGet(int slotIndex)
   {
      int slotCount = SlotCountGet();
      assert(slotIndex < slotCount);
      assert(slotIndex >= 0);
      if((slotIndex >= 0) && (slotIndex < slotCount))
         return slots[slotIndex]->SizeGet();
      else
         return 0;
   }

   /// Return the position of the beginning of the given slot in the file.
   /// Return -1 if there is a problem.
   int64 SlotOffsetGet(int slotIndex)
   {
      int slotCount = SlotCountGet();
      assert(slotIndex < slotCount);
      assert(slotIndex >= 0);
      if((slotIndex >= 0) && (slotIndex < slotCount))
      {
         int64 slotOffset = 0;
         for(int tempIndex = 0; tempIndex < slotIndex; tempIndex++)
         {
            if(slots[tempIndex])
               slotOffset += (int64)slots[tempIndex]->SizeGet();
            else
               return -1;
         }
         return slotOffset;
      }
      else
         return -1;
   }

   /// Return the slotBuffer, if any, associated with the given slot.
   void* SlotBufferGet(int slotIndex)
   {
      int slotCount = SlotCountGet();
      assert(slotIndex < slotCount);
      assert(slotIndex >= 0);
      if((slotIndex >= 0) && (slotIndex < slotCount))
         return slots[slotIndex]->BufferGet();
      else
         return NULL;
   }

   /// Return a pointer to the SaveSlotProperties object for the given slot.
   /// If there is no associated object, return NULL.
   SaveSlotProperties* SaveSlotPropertiesGet(int slotIndex)
   {
      int slotCount = SlotCountGet();
      assert(slotIndex < slotCount);
      assert(slotIndex >= 0);
      if((slotIndex >= 0) && (slotIndex < slotCount))
         return slots[slotIndex];
      else
         return NULL;
   }
   
   /// Return the expected size of the file or -1 if a problem is encountered.
   int64 SizeGet()
   {
      int64 fileSize = 0;
      int slotCount = slots.SizeGet();
      for(int slotIndex = 0; slotIndex < slotCount; slotIndex++)
      {
         SaveSlotProperties* slotProperties = slots[slotIndex];
         if(!slotProperties)
            return -1;
         
         fileSize += (int64)slotProperties->SizeGet();
      }
      
      return fileSize;
   }
   
protected:
   /// Heap from which all allocations are made unless specified otherwise.
   HeapID heapID;
   /// Collection of slots for this file.
   Table<SaveSlotProperties*> slots;
};

//==============================================================================

/// Header used at the beginning of all save slots.
struct SaveSlotHeader
{
   /// The first four bytes should correspond to "GSID" 
   char magicString[SAVE_MANAGER_MAGIC_STRING_LENGTH];
   /// This is the CRC of the rest of the slot, excluding any unused space
   /// at the end.   
   uint32 slotCRC;
   /// Size of the used part of the slot.  This includes everything from the
   /// beginning of the header except any unused space at the end.
   uint32 slotUsedSize;
   /// Version number of the save manager data format.
   uint32 versionNumber; 
};

//==============================================================================

/// Additional header data needed for loading and saving a list of Lua variables.
struct SaveSlotLuaHeader
{
   /// CRC of the string that lists the variables that were saved.
   uint32 variableListCRC;
};

//==============================================================================

/// Interface for saving and loading data.  When providing filenames of save
/// files, do not include the extension.
class SaveManager
{
public:
   /// An error code for the saving and loading operations.
   enum ErrorCode
   {
      /// No errors
      SUCCESS,
      /// The slot does not seem to have been used before.  This includes
      /// situations where the save file is absent.
      EMPTY,
      /// The slot exists, but it's corrupted.  The corruption may be limited
      /// to a single slot.
      SLOT_CORRUPT,
      /// File file exists, but it is corrupted.  Assume nothing can be recovered
      /// from any slots.
      FILE_CORRUPT,
      /// The slot exists, and it's not corrupted, but the varaible list
      /// doesn't match.  This typically means the variable list has changed
      /// since the last time this slot was saved.  Treat it as though it was empty.
      OBSOLETE,
      /// The slot was too small for the associated data.  Note that this
      /// problem may not be detected if the slot size is incorrect, but still
      /// big enough for the current data. 
      SIZE_INCORRECT,
      /// The slot is using a version number newer than this application supports. 
      FUTURE_VERSION,
      /// A problem was encounted opening, reading, or writing the file.
      IO_FAILED,
      /// All other failures
      FAILED      
   };

   virtual ~SaveManager() {}
   
   void Init(HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   virtual void Deinit();
   
   /// Set the number of slots to use for the given file.
   /// If the slot should consist of a single dynamically sized slot,
   /// do not bother specifying anything with this method.
   virtual void SlotCountSet(const char* filename, int slotCount);
   /// Return the number of slots to use for the given file.
   /// If this returns zero, nothing has been specified for the given file,
   /// which could mean it is intended to consist of a single dynamically
   /// expanding slot.
   int SlotCountGet(const char* filename);

   /// Set the size of the given slot in the given file.
   /// If a file should have a single dynamically sized slot, do not
   /// bother calling this method for it.
   virtual void SlotSizeSet(const char* filename, int slotIndex, size_t slotSize);
   /// Return the size of the given slot of the given file in bytes.
   /// If the slot has no specified size or if the slot does not exist,
   /// according to SlotCountGet, this will return zero.
   size_t SlotSizeGet(const char* filename, int slotIndex);

   /// Attempt to save the given data to the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.  'usedSize' should be exactly the amount of space that is
   /// actually used in 'buffer'.  If the given slot has a specific size, 'usedSize' must be
   /// less than or equal to that size.  'buffer' and 'usedSize' must include
   /// space at the beginning for the SaveSlotHeader.  The header will be filled
   /// out automatically based on the parameters.
   virtual ErrorCode Save(void* buffer, size_t usedSize, const char* filename, int slotIndex = -1) = 0;
   /// Attempt to load data from the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.  'bufferSize' must be large enough to hold all the data
   /// from the given slot.  'buffer' and 'bufferSize' must include space at the
   /// beginning for the SaveSlotHeader.  If successful and 'usedSize' is
   /// specified, it will be set to the value that would have been used in the
   /// corresponding Save call.
   virtual ErrorCode Load(void* buffer, size_t bufferSize, size_t* usedSize, const char* filename, int slotIndex = -1) = 0;     
   
   /// Attempt to save the given variables to the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.
   virtual ErrorCode Save(const char* variableList, const char* filename, int slotIndex = -1) = 0;
   /// Attempt to load the given variables from the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.
   virtual ErrorCode Load(const char* variableList, const char* filename, int slotIndex = -1) = 0;

   /// Reduce the number of times that files are opened and closed by writing
   /// multiple slots, possibly to the same files, at once.
   /// This marks the beginning of a group of saves to perform together.
   /// This is not available on all platforms, and it does not include files
   /// with a single dynamically sized slot.
   virtual ErrorCode SaveBatchBegin();
   /// Perform all the saves that were requested since BatchBegin.
   /// This is not available on all platforms, and it does not include files
   /// with a single dynamically sized slot.
   virtual ErrorCode SaveBatchExecute();
   /// Exit batch mode without actually saving anything immediately.  This will
   /// not actually undo anything, it just changes the state.  For example,
   /// data you requested to save may end up being written on the next save.
   virtual ErrorCode SaveBatchCancel();

   /// Remove the given save file.  Return SUCCESS if the file was either deleted
   /// or not found.
   virtual ErrorCode Erase(const char* filename) { (void)filename; return FAILED; }
   
   /// Set up the Wii save banner file.  If the file does not already exist, 
   /// or if the contents are incorrect, create it.  Use the given game title, which should be
   /// in UTF-8 in the local language.  The comment should typically be "Save Game" in
   /// the local language.  Both 'title' and 'comment' should be no more than
   /// 21 characters long, including the null terminator.  'graphicsFilename'
   /// should be the path to the TPL file containing the banner bitmap at index 0 and
   /// the icon at index 1.  This assumes the icon is not animated.  Do not include
   /// the file extension.  This only actually does anything on the Wii.
   /// This must only be called after all other necessary save files have been
   /// successfully created and initialized.  This cannot be part of a batch.
   virtual ErrorCode WiiSaveBannerInit(const char* title, const char* comment, const char* graphicsFilename)
   {
      (void)title;
      (void)comment;
      (void)graphicsFilename;
      return SUCCESS;
   }
   
protected:
   /// Return the position of the beginning of the given slot in the given file.
   /// Return -1 if there is a problem.
   int64 SlotOffsetGet(const char* filename, int slotIndex);

   /// Attempt to write the data from the given variables to the given buffer.
   /// 'buffer' must include space at the beginning for the SaveSlotHeader and SaveSlotLuaHeader.
   /// 'bufferSize' should be the full size of buffer.  'usedSize'
   /// will be set to the actual number of bytes that are used, which can be
   /// passed to SaveManager::Save.  The SaveSlotLuaHeader will be filled out
   /// automatically in little-endian format.
   ErrorCode BufferVariablesWrite(void* buffer, size_t bufferSize, const char* variableList, size_t* usedSize);
   /// Attempt to extract the data for the given variables from the given buffer.
   /// 'buffer' must include a valid, little-endian SaveSlotHeader and SaveSlotLuaHeader
   /// This assumes you have already checked the SaveSlotHeader with SlotHeaderCheck.
   /// 'bufferSize' is the size of 'buffer', including the headers.
   ErrorCode BufferVariablesRead(void* buffer, size_t bufferSize, const char* variableList);
   
   /// This will set 'minSize' to the minimum size in bytes needed to save the data
   /// currently in the given variables, including the space for the headers.
   /// Return true if successful.
   bool BufferSizeMinGet(const char* variableList, size_t* minSize);

   /// Fill out the slot header for the given buffer.  'usedSize' is the number
   /// of bytes that are actually used, including the space for the header.
   /// Be sure 'buffer' is at least big enough for the header.
   /// Return true if successful.
   static bool SlotHeaderSet(void* buffer, size_t usedSize);
   /// Use the header to check the given slot buffer for problems.
   /// If 'usedSize' is specified, it will be set to the amount
   /// of space that the header says was actually used. 
   static ErrorCode SlotHeaderCheck(void* buffer, size_t* usedSize);

   /// Return a pointer to the SaveFileProperties object for the given file.
   /// If there is no associated object, return NULL.
   SaveFileProperties* SaveFilePropertiesGet(const char* filename);
   
   /// Return true if the manager is between calls to SaveBatchBegin and SaveBatchExecute.
   bool BuildingBatchCheck() { return buildingBatch; }
   
   /// Set 'fullFilename' to the concatenation of 'filename' and the appropriate extension.
   /// Return true if successful.
   virtual bool FullFilenameBuild(char* fullFilename, const char* filename,
      size_t fullFilenameBufferSize) = 0;

   ///// Lua-bound function for calling theSaves->SlotCountSet.
   //static void LuaSlotCountSet(const char* filename, float slotCount);
   ///// Lua-bound function for calling theSaves->LuaSlotSizeSet.
   //static void LuaSlotSizeSet(const char* filename, float slotIndex, float slotSize);
   ///// Lua-bound function for calling theSaves->Save.
   //static float LuaSlotSave(const char* variableList, const char* filename, float slotIndex);
   ///// Lua-bound function for calling theSaves->Load.
   //static float LuaSlotLoad(const char* variableList, const char* filename, float slotIndex);
   ///// Lua-bound function for calling theSaves->SaveBatchBegin.
   //static float LuaSaveBatchBegin();
   ///// Lua-bound function for calling theSaves->SaveBatchExecute.
   //static float LuaSaveBatchExecute();
   ///// Lua-bound function for calling theSaves->SaveBatchCancel.
   //static float LuaSaveBatchCancel();
   ///// Lua-bound function for calling theSaves->Erase.
   //static float LuaSaveFileErase(const char* filename);
   ///// Lua-bound function for calling theSaves->WiiSaveBannerInit.
   //static float LuaWiiSaveBannerInit(const char* title, const char* comment, const char* graphicsFilename);
   
   /// True if the manager is between calls to SaveBatchBegin and SaveBatchExecute.
   bool buildingBatch;
   /// Heap for long-term allocations.
   HeapID heapID;
   /// Heap for short-term allocations.
   HeapID tempHeapID;
   /// Stored properties of save files and their slots.
   Map<StringHashPair, SaveFileProperties*> saveFiles;

   friend class SaveSlotProperties;
};

/// Pointer to the default SaveManager for saving and loading data.
/// This must be set by the application before any attempts to read are made.
//extern SaveManager* theSaves;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SAVEMANAGERCOMMON_H__
