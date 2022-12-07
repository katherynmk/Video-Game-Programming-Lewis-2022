#ifndef __FROG__LOADERITERATIVE_H__
#define __FROG__LOADERITERATIVE_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Map.h"
#include "JSONValue.h"

namespace Webfoot {

class SpriteResourceFile;

/// Value used by LoaderIterative to indicate no progress has been made.
#define LOADER_ITERATIVE_PROGRESS_MIN 0.0f
/// Value used by LoaderIterative to indicate loading is complete
#define LOADER_ITERATIVE_PROGRESS_MAX 100.0f
/// String that identifies how a resource should be loaded.
#define LOADER_ITERATIVE_TYPE_KEY "Type"
/// String that specifies the Sound resource type.
#define LOADER_ITERATIVE_SOUND_TYPE_NAME "Sound"
/// Filename of the sound to be loaded or unloaded.
#define LOADER_ITERATIVE_SOUND_FILENAME_KEY "Filename"
/// String that specifies the Font resource type.
#define LOADER_ITERATIVE_FONT_TYPE_NAME "Font"
/// Filename of the font to be loaded or unloaded.
#define LOADER_ITERATIVE_FONT_FILENAME_KEY "Filename"
/// String that specifies the SpriteResourceFile resource type.
#define LOADER_ITERATIVE_SPRITE_RESOURCE_FILE_TYPE_NAME "SpriteResourceFile"
/// Filename of the sprite resource file to be loaded or unloaded.
#define LOADER_ITERATIVE_SPRITE_SPRITE_RESOURCE_FILE_FILENAME_KEY "Filename"
/// String that specifies the GUI Layer resource type.
#define LOADER_ITERATIVE_GUI_LAYER_TYPE_NAME "GUILayer"
/// Name of the GUI Layer to be loaded or unloaded.
#define LOADER_ITERATIVE_GUI_LAYER_NAME_KEY "Filename"
/// String that specifies the Texture resource type.
#define LOADER_ITERATIVE_TEXTURE_TYPE_NAME "Texture"
/// Filename of the texture to be loaded or unloaded.
#define LOADER_ITERATIVE_TEXTURE_FILENAME_KEY "Filename"
/// String that specifies the texture resource file resource type.
#define LOADER_ITERATIVE_TEXTURE_RESOURCE_FILE_TYPE_NAME "TextureResourceFile"
/// Name of the texture resource file to be loaded or unloaded.
#define LOADER_ITERATIVE_TEXTURE_RESOURCE_FILE_FILENAME_KEY "Filename"

//==============================================================================

/// Handles the loading and unloading of a particular type of resource for 
/// LoaderIterative.
class LoaderIterativeDelegate
{
public:
   LoaderIterativeDelegate() { specifications = NULL; }
   virtual ~LoaderIterativeDelegate() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications) { specifications = _specifications; }
   /// Called to perform cleanup.
   virtual void Deinit() { specifications = NULL; }
   /// Called regularly to continue loading.
   virtual void Update() {}
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() = 0;
   /// Return a value between 0 and 100 (inclusive) for a very rough estimate
   /// of the percentage of loading that has been completed.
   virtual float ProgressGet() { return FinishedCheck() ? LOADER_ITERATIVE_PROGRESS_MIN : LOADER_ITERATIVE_PROGRESS_MAX; }
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel() = 0;
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.  This is not intended to be used along with
   /// the other methods, like Init, Update, and Deinit.
   virtual void Unload(JSONValue* _specifications) = 0;
   
protected:
   JSONValue* specifications;
};

//==============================================================================

/// Handles the loading and unloading of a Sound for LoaderIterative.
class LoaderIterativeDelegateSound : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateSound() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() { return finished; }
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegate Inherited;
   
   /// True if loading was completed.
   bool finished;
};

//==============================================================================

/// Handles the loading and unloading of a Font for LoaderIterative.
class LoaderIterativeDelegateFont : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateFont() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() { return finished; }
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegate Inherited;
   
   /// True if loading was completed.
   bool finished;
};

//==============================================================================

/// Handles the loading and unloading of a SpriteResourceFile for LoaderIterative.
class LoaderIterativeDelegateSpriteResourceFile : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateSpriteResourceFile() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() { return finished; }
   /// Return a value between 0 and 100 (inclusive) for a very rough estimate
   /// of the percentage of loading that has been completed.
   virtual float ProgressGet();
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegate Inherited;

   /// Set 'filenameBuffer' to the filename to use for the sprite resource
   /// file.  Return true if successful.
   virtual bool FilenameGet(JSONValue* _specifications, char* filenameBuffer, size_t filenameBufferSize);
   
   /// Pointer to the sprite resource file that was loaded.
   SpriteResourceFile* spriteResourceFile;
   /// Number of sprite animations loaded so far.
   int animationsLoaded;
   /// Next sprite animation to load.
   JSONValue::ObjectIterator spriteAnimationIterator;
   /// True if the resource file and all its animations have been loaded.
   bool finished;
};

//==============================================================================

/// Handles the loading and unloading of a SpriteResourceFile for LoaderIterative.
class LoaderIterativeDelegateGUILayer : public LoaderIterativeDelegateSpriteResourceFile
{
public:
   virtual ~LoaderIterativeDelegateGUILayer() {}

   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck();
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegateSpriteResourceFile Inherited;

   /// Return the name of the layer to load.
   const char* LayerNameGet(JSONValue* _specifications);
   /// Set 'filenameBuffer' to the filename to use for the sprite resource
   /// file.  Return true if successful.
   virtual bool FilenameGet(JSONValue* _specifications, char* filenameBuffer, size_t filenameBufferSize);
   
   /// True if theGUI->Preload has already been called, if appropriate.
   bool preloadHandled;
};

//==============================================================================

/// Handles the loading and unloading of a Texture for LoaderIterative.
class LoaderIterativeDelegateTexture : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateTexture() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() { return finished; }
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegate Inherited;
   
   /// True if loading was completed.
   bool finished;
};

//==============================================================================

/// Handles the loading and unloading of a Font for LoaderIterative.
class LoaderIterativeDelegateTextureResourceFile : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateTextureResourceFile() {}
   
   /// Prepare to load the given resource.
   virtual void Init(JSONValue* _specifications);
   /// Called regularly to continue loading.
   virtual void Update();
   /// Returns true when the given resource is done loading.
   virtual bool FinishedCheck() { return finished; }
   /// Return a value between 0 and 100 (inclusive) for a very rough estimate
   /// of the percentage of loading that has been completed.
   virtual float ProgressGet();
   
   /// If this is called before Deinit is called, unload anything loaded since
   /// Init.
   virtual void Cancel();
   
   /// Called to unload the given resource.  This function should block until
   /// unloading is complete.
   virtual void Unload(JSONValue* _specifications);
   
protected:
   typedef LoaderIterativeDelegate Inherited;

   /// Set 'filenameBuffer' to the filename to use for the sprite resource
   /// file.  Return true if successful.
   virtual bool FilenameGet(JSONValue* _specifications, char* filenameBuffer, size_t filenameBufferSize);
   
   /// Pointer to the texture resource file data that was loaded.
   JSONValue* textureFileSpecifications;
   /// Number of sprite animations loaded so far.
   int animationsLoaded;
   /// Next sprite animation to load.
   JSONValue::ObjectIterator textureAnimationIterator;
   /// True if the resource file and all its animations have been loaded.
   bool finished;
};

//==============================================================================

/// LoaderIterative allows you to load lists of resources over the course of
/// multiple frames, which is useful for loading screens.  The lists are
/// provided as JSON arrays of objects. Each object represents a single item to
/// be loaded and must have a "Type" entry to indicate the type of resource
/// (Font, Sound, etc.). The other entries for an object depend on the type.
/// In general, this system should also be used to unload the resources that it
/// loads.
class LoaderIterative
{
public:
   LoaderIterative();

   void Init(HeapID _heapID = HEAP_TEMP);
   void Deinit();
   /// Call this regularly to proceed with loading.  Do not call this while
   /// unloading.
   void Update();
   
   /// Add the given list of specifications to the queue of resources to load.
   /// This will not make an internal copy, so don't clean up the
   /// specifications until the loader is deinitialized.
   void ListAdd(JSONValue* _specifications);
   
   /// If you want to cancel or undo loading before calling Deinit, call this
   /// method to unload anything that was loaded since Init.
   void Cancel();
   
   /// Unload all items in the lists.  This method is synchronous, so it will
   /// return when unloading is complete.  This is not intended to be used
   /// in the same Init/Deinit session as 'Update'.  It should be used when
   /// resources previously loaded by LoaderIterative are no longer needed.
   void Unload();
   
   /// Return true if loading is complete.
   bool FinishedCheck();
   /// Return a value between 0 and 100 (inclusive) for a very rough estimate
   /// of the percentage of loading that has been completed.
   float ProgressGet();
   
   /// Register the given loader/unloader to the given type.  LoaderIterativeDelegate
   /// objects are stateful, so don't register the same instance to more than
   /// one LoaderIterative if both LoaderIterative objects will be used at the
   /// same time.
   void DelegateRegister(const char* name, LoaderIterativeDelegate* _delegate); 

   /// Try to spend at least this much time loading during a call to update.
   void UpdateDurationMinSet(unsigned int _updateDurationMin) { updateDurationMin = _updateDurationMin; }
   
   /// Return true if this is between calls to Init and Deinit.
   bool InitializedCheck() { return initialized; }
      
protected:
   typedef Map<const char*, LoaderIterativeDelegate*> DelegateMap;

   /// Return the delegate registered for the given type name or NULL if none
   /// are registered for it.
   LoaderIterativeDelegate* DelegateGet(const char* name);

   /// True if this is between calls to Init and Deinit.
   bool initialized;
   /// Heap from which relatively long-term allocations of this class are made.
   HeapID heapID;
   /// Try to spend at least this much time loading during a call to update.
   unsigned int updateDurationMin;
   /// Which resource is being loaded.
   int listItemIndex;
   /// Which collection of specifications is being loaded.
   int listIndex;
   /// The current delegate loading a resource.
   LoaderIterativeDelegate* currentDelegate;
   /// Queue of specifications to be loaded.
   Table<JSONValue*> specificationLists;
   /// Map of names to delegates for loading resources of different types.
   DelegateMap delegates;

   /// Delegate instance for loading sounds.
   LoaderIterativeDelegateSound delegateSound;
   /// Delegate instance for loading fonts.
   LoaderIterativeDelegateFont delegateFont;
   /// Delegate instance for loading sprite resource files.
   LoaderIterativeDelegateSpriteResourceFile delegateSpriteResourceFile;
   /// Delegate instance for loading GUI layers.
   LoaderIterativeDelegateGUILayer delegateGUILayer;
   /// Delegate instance for loading textures.
   LoaderIterativeDelegateTexture delegateTexture;
   /// Delegate instance for loading texture resource files.
   LoaderIterativeDelegateTextureResourceFile delegateTextureResourceFile;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__LOADERITERATIVE_H__
