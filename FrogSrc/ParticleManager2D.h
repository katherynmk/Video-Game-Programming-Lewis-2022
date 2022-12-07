#ifndef __FROG__PARTICLEMANAGER2D_H__
#define __FROG__PARTICLEMANAGER2D_H__

#include "FrogMemory.h"
#include "Map.h"

namespace Webfoot {

class ParticleEffectEmitter2D;
class JSONValue;

//==============================================================================

/// ParticleManager2D handles aspects of 2D particles that are centralized, like
/// the registries of modifiers.
///
/// This has no dependencies on the GUIManager, but if it is not already
/// initialized when theGUI is initialized, the theGUI will
/// automatically initialize theParticles2D and deinitalize it when theGUI
/// receives its Deinit call. 
class ParticleManager2D
{
public:
   void Init(HeapID _heapID = HEAP_DEFAULT, HeapID _heapTempID = HEAP_TEMP);
   void Deinit();
   
   /// Set up any necessary ParticleModifier2D and/or ParticleEmitterModifier2D
   /// instances for the given emitter.  If no actual new class instances are
   /// needed for this modifier, this function could just make one-time
   /// adjustments to the particles and emitters.  This will only be called at
   /// a time when all the particles are inactive.  'filename' may be provided
   /// for debug output purposes.
   typedef void (*ModifierInitializer)(const char* modifierType, ParticleEffectEmitter2D* emitter,
      JSONValue* specifications, HeapID heapID, HeapID tempHeapID, const char* filename);
   
   /// Register the given initializer with the given name. 
   void ModifierInitializerRegister(const char* name, ModifierInitializer modifierInitializer);
   /// Return the ModifierInitializer with the given name.
   ModifierInitializer ModifierInitializerGet(const char* name);
   
   /// Return true if this is currently initialized.
   bool InitializedCheck() { return initialized; }

   static ParticleManager2D instance;
   
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

ParticleManager2D* const theParticles2D = &ParticleManager2D::instance;

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__PARTICLEMANAGER2D_H__
