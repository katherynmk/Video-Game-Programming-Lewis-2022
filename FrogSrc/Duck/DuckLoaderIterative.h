#ifndef __FROG__DUCK__DUCKLOADERITERATIVE_H__
#define __FROG__DUCK__DUCKLOADERITERATIVE_H__

#include "FrogMemory.h"
#include "LoaderIterative.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

/// String that specifies the Duck scene type.
#define DUCK_LOADER_ITERATIVE_SCENE_TYPE_NAME "DuckScene"
/// Name of the Duck scene file to be loaded or unloaded.
#define DUCK_LOADER_ITERATIVE_SCENE_FILENAME_KEY "Filename"

/// String that specifies the Duck environment map type.
#define DUCK_LOADER_ITERATIVE_ENVIRONMENT_MAP_TYPE_NAME "DuckEnvironmentMap"
/// Name of the Duck environment map file to be loaded or unloaded.
#define DUCK_LOADER_ITERATIVE_ENVIRONMENT_MAP_FILENAME_KEY "Filename"

//==============================================================================

/// Handles the loading and unloading of a Duck::Scene for LoaderIterative.
class LoaderIterativeDelegateDuckScene : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateDuckScene() {}

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

/// Handles the loading and unloading of a Duck::EnvironmentMap for
/// LoaderIterative.
class LoaderIterativeDelegateDuckEnvironmentMap : public LoaderIterativeDelegate
{
public:
   virtual ~LoaderIterativeDelegateDuckEnvironmentMap() {}

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

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__DUCKLOADERITERATIVE_H__
