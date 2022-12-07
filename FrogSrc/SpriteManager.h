#ifndef __FROG__SPRITEMANAGER_H__
#define __FROG__SPRITEMANAGER_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class SpriteAnimation;
class SpriteResourceFile;

//=============================================================================

/// SpriteManager is used to load sprite data from resource files.  The data is
/// reference counted and you have some control over what is loaded and when.
/// If you just want to load a single animation from a resource file and
/// automatically have all associated data unloaded when that animation is no
/// longer in use, just use AnimationLoad and AnimationUnload or set and clear the
/// animation of your sprite.  ResourceFileSpecificationsLoad and ResourceFileLoad are
/// for preloading while ResourceFileSpecificationsUnload and ResourceFileUnload undo
/// that preloading.  If you want to keep all data for a resource file
/// in memory except for the image data, use ResourceFileSpecificationsLoad and
/// ResourceFileSpecificationsUnload.  If you want to keep all sprite data for a
/// resource file in memory, including the image data, use ResourceFileLoad and
/// ResourceFileUnload.
/// Do not include the extension when specifying a resource filename.
class SpriteManager
{
public:
   SpriteManager();

   void Init();
   void Deinit();

   /// Return a pointer to the given SpriteAnimation in the given resource file.
   /// Reference counts will be incremented and data will be loaded as needed.
   /// Return NULL if unsuccessful.  When finished with the animation, pass it to AnimationUnload.
   SpriteAnimation* AnimationLoad(const char* resourceFilename, const char* spriteName);
   /// Decrement the reference count of the animation and resource file.
   /// Unload data as appropriate.
   void AnimationUnload(SpriteAnimation* animation);
   /// Decrement the reference count of the animation and resource file.
   /// Unload data as appropriate.
   void AnimationUnload(const char* resourceFilename, const char* animationName);
   /// Return true if the given animation exists in the given sprite resource
   /// file.
   bool AnimationExistsCheck(const char* resourceFilename, const char* animationName);

   /// Preload a resource file and all its SpriteAnimations.
   void ResourceFileLoad(const char* resourceFilename);
   /// Undo a call to ResourceFileLoad.  This will not necessarily
   /// free anything, but it will decrement reference counts;
   void ResourceFileUnload(const char* resourceFilename);
   /// Return true if the sprite resource file with the given filename exists.
   bool ResourceFileExistsCheck(const char* resourceFilename);
   
   /// Load the given resource file, but don't load the SpriteAnimations yet.
   void ResourceFileSpecificationsLoad(const char* resourceFilename);
   /// Decrement the reference count on the resource file and free
   /// data as appropriate.
   void ResourceFileSpecificationsUnload(const char* resourceFilename);

   /// Return a pointer to the given resource file.
   /// Return NULL if the file is not loaded.
   SpriteResourceFile* ResourceFileGet(const char* filename);

   static SpriteManager instance;

private:
   /// Deinit the given resource file regardless of its reference count
   /// and remove it from the collection.
   void ResourceFileDeinit(SpriteResourceFile* resourceFile);

   /// True if the class is between Init and Deinit calls.
   bool initialized;

   Table<SpriteResourceFile*> resourceFiles;
};

//=============================================================================

SpriteManager* const theSprites = &SpriteManager::instance;

} //namespace Webfoot

#endif //#ifndef __FROG__SPRITEMANAGER_H__
