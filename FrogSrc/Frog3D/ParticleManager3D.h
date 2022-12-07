#ifndef __FROG__FROG3D__PARTICLEMANAGER3D_H__
#define __FROG__FROG3D__PARTICLEMANAGER3D_H__

#include "FrogMemory.h"
#include "Map.h"

namespace Webfoot {

class ParticleEffectEmitterSceneNode;
class JSONValue;

//==============================================================================

/// ParticleManager3D handles aspects of 3D particles that are centralized, like
/// the registries of modifiers.
///
/// This has no dependencies on the GUIManager, but if it is not already
/// initialized when theGUI is initialized, the theGUI will
/// automatically initialize theParticles3D and deinitalize it when theGUI
/// receives its Deinit call. 
class ParticleManager3D
{
public:
   void Init(HeapID _heapID = HEAP_DEFAULT, HeapID _heapTempID = HEAP_TEMP);
   void Deinit();
   
   /// Set up any necessary ParticleModifier3D and/or ParticleEmitterModifier3D
   /// instances for the given emitter.  If no actual new class instances are
   /// needed for this modifier, this function could just make one-time
   /// adjustments to the particles and emitters.  This will only be called at
   /// a time when all the particles are inactive.  'filename' may be provided
   /// for debug output purposes.
   typedef void (*ModifierInitializer)(const char* modifierType, ParticleEffectEmitterSceneNode* emitter,
      JSONValue* specifications, HeapID heapID, HeapID tempHeapID, const char* filename);
   
   /// Register the given initializer with the given name. 
   void ModifierInitializerRegister(const char* name, ModifierInitializer modifierInitializer);
   /// Return the ModifierInitializer with the given name.
   ModifierInitializer ModifierInitializerGet(const char* name);
   
   /// Return true if this is currently initialized.
   bool InitializedCheck() { return initialized; }

   static ParticleManager3D instance;
   
protected:
   typedef Map<const char*, ModifierInitializer> ModifierInitializerMap;
   
   bool initialized;
   /// Used for long-term allocations
   HeapID heapID;
   /// Used for short-term allocations.
   HeapID heapTempID;
   /// Map of names to functions that set up modifiers.
   ModifierInitializerMap modifierInitializerMap;
};

ParticleManager3D* const theParticles3D = &ParticleManager3D::instance;

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__PARTICLEMANAGER3D_H__
