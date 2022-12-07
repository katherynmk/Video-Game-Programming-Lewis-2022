#ifndef __FROG__SPRITERESOURCEFILE_H__
#define __FROG__SPRITERESOURCEFILE_H__

#include "FrogMemory.h"
#include "Map.h"

namespace Webfoot {

class SpriteAnimation;
class JSONValue;

//=============================================================================

/// Sprite specifications are provided in JSON files.
/// SpriteResourceFile is a collection of all the SpriteAnimation for
/// one of those files.  Do not include the extension when specifying
/// the filename.
class SpriteResourceFile
{
public:
   /// Create a new SpriteResourceFile based on the given file.
   SpriteResourceFile();
   virtual ~SpriteResourceFile() {}

   /// Return true if successful.
   bool Init(const char* _filename);
   void Deinit();

   /// Return a pointer to the SpriteAnimation with the given name.
   /// Increment reference counts and load its image data if needed.
   /// Return NULL if it is not found.
   SpriteAnimation* AnimationLoad(const char* animationName);
   /// Decrement reference counts and free data as needed.
   void AnimationUnload(SpriteAnimation* animation);
   /// Without affecting the reference count, return the given animation if it
   /// is loaded and NULL if it is not.
   SpriteAnimation* AnimationGet(const char* animationName);
   /// Return true if the given animation is defined for this resource file.
   bool AnimationExistsCheck(const char* animationName);

   /// Return the filename used to build this SpriteResourceFile.
   const char* FilenameGet() { return (const char*)filename; }

   /// Increment reference counts on all animations and load data as needed.
   void AllLoad();
   /// Decrement reference counts on all animations and free data as needed.
   void AllUnload();

   /// Increment the reference count for this file.
   void ReferenceCountIncrement() { referenceCount++; }
   /// Decrement the reference count for this file.
   void ReferenceCountDecrement() { referenceCount--; }
   /// Return the reference count for this file.
   int ReferenceCountGet() { return referenceCount; }

   /// Return the JSON data with the details for this object.
   JSONValue* SpecificationsGet() { return specifications; }

private:
   typedef Map<const char*, SpriteAnimation*> SpriteAnimationMap;

   /// Keeps track of how many things want this file or its animations loaded.
   int referenceCount;
   /// The name of the file in question.
   char* filename;
   /// JSON data with the details for this object.
   JSONValue* specifications;
   /// Collection of animations in this file.
   SpriteAnimationMap spriteAnimations;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__SPRITERESOURCEFILE_H__
