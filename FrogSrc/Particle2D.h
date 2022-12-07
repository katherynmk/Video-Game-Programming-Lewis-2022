#ifndef __FROG__PARTICLE2D_H__
#define __FROG__PARTICLE2D_H__

#include "FrogMemory.h"
#include "Sprite.h"

namespace Webfoot {

class ParticleEmitter2D;

//=============================================================================

/// Class to represent a particle in the 2D particle system.
class Particle2D : public Sprite
{
public:
   Particle2D();
   virtual ~Particle2D() {}
   
   /// Initialize the particle without setting its appearance.
   void Init();
   /// Initialize the particle with the given sprite animation from the given file.
   /// Do not include the extension when specifying the filename.
   void Init(const char* resourceFilename, const char* animationName);
   virtual void Deinit();
   
   /// Called every frame by the ParticleEmitter2D.
   virtual void Update(unsigned int dt);
   
   /// This is called after the emitter is done initializing the particle for a
   /// new life.
   virtual void Activate();

   /// End the particle's life.
   /// The particle will not necessarily be available again until the particle system is updated.
   virtual void Expire();

   /// Return the current linear velocity of the particle in units per second.
   Point2F VelocityGet() { return velocity; }
   /// Set the current linear velocity of the particle in units per second.
   void VelocitySet(const Point2F& _velocity) { velocity = _velocity; }
   /// Return the current linear acceleration of the particle in units per second squared.
   Point2F AccelerationGet() { return acceleration; }
   /// Set the current linear acceleration of the particle in units per second squared.
   void AccelerationSet(const Point2F& _acceleration) { acceleration = _acceleration; }
   
   /// Return the current angular velocity of the particle.
   float AngularVelocityGet() { return angularVelocity; }
   /// Set the current angular velocity of the particle.
   void AngularVelocitySet(float _angularVelocity) { angularVelocity = _angularVelocity; }
   /// Return the current angular acceleration of the particle.
   float AngularAccelerationGet() { return angularAcceleration; }
   /// Set the current angular acceleration of the particle.
   void AngularAccelerationSet(float _angularAcceleration) { angularAcceleration = _angularAcceleration; }
   
   /// Return how the particle's linear velocity should be slowed.  (see LinearDampingSet)
   float LinearDampingGet() { return linearDamping; }
   /// Set how the particle's linear velocity should be slowed.  The value must
   /// be between 0 and 1 (inclusive).  For every second, the given fraction
   /// of the linear velocity will be removed.  So if it is 0, the velocity will
   /// not be slowed, if it's 1, it will be stopped immediately, and if it's 0.5,
   /// it will lose half its velocity every second. 
   /// This can be used to simulate friction.
   void LinearDampingSet(float _linearDamping) { linearDamping = _linearDamping; }
   /// Return how the particle's angular velocity should be slowed.  (see LinearDampingSet)
   float AngularDampingGet() { return angularDamping; }
   /// Set how the particle's angular velocity should be slowed.  (see LinearDampingSet)
   void AngularDampingSet(float _angularDamping) { angularDamping = _angularDamping; }  
   
   /// Make the particle expire if its position (origin) leaves the specified region.
	void LifeAreaSet(const Box2F& _lifeArea) { lifeArea = _lifeArea; lifeAreaEnabled = true; }
	/// Return the area where the particle will expire if it leaves.  This will
   /// only happen if LifeAreaEnabledCheck() is returning true.
	Box2F LifeAreaGet() { return lifeArea; }
	/// Return true if the particle will expire if it leaves a certain area
	bool LifeAreaEnabledCheck() { return lifeAreaEnabled; }
	/// Disable the life area for this particle.
	void LifeAreaClear() { lifeAreaEnabled = false; }
	   
   enum
   {
      /// value used for 'life' if the particle should not expire due to time.
      LIFE_DURATION_INDEFINITE = -1
   };
   
   /// Return the age of the particle in milliseconds at which it will expire.
   /// Return LIFE_DURATION_INDEFINITE if it will not expire from time alone.
   int LifeDurationGet() { return lifeDuration; }
   /// Set the age of the particle in milliseconds at which it will expire.
   /// Set to LIFE_DURATION_INDEFINITE if it should not expire from time alone.
   void LifeDurationSet(int _life) { lifeDuration = _life; }
   
   /// Return the number of milliseconds since the particle was activated.
   int AgeGet() { return age; }
   
   /// Set a pointer to the ParticleEmitter2D that owns this particle.
   /// This should only be called by the emitter in question.
   void EmitterSet(ParticleEmitter2D* _emitter) { emitter = _emitter; }
   /// Return the ParticleEmitter2D that owns this particle.
   ParticleEmitter2D* EmitterGet() { return emitter; }
   
   /// Return true if the particle is between activation and expiration.
   bool ActiveCheck() { return active; }
   
protected:
   typedef Sprite Inherited;

   /// Handle initialization common to different forms of Init.
   void InitHelper();

   /// True if Init() has been called.
   bool initialized;
   
   /// True if the particle is between activation and expiration;
   bool active;
   
   /// True if the particle should expire if it leaves the lifeArea.
   bool lifeAreaEnabled;
   
   /// The age of the particle in milliseconds at which it will expire.
   /// If this is LIFE_DURATION_INDEFINITE, the particle will not expire due to time.
   int lifeDuration;
   /// Number of milliseconds since the particle was activated.
   int age;
   
   /// Current velocity of the particle in units per second.
   Point2F velocity;
   /// Current acceleration of the particle in units per second squared.
   Point2F acceleration;

   /// Current angular velocity of the particle.
   float angularVelocity;
   /// Current angular acceleration of the particle.
   float angularAcceleration;
   
   /// Used to slow the linear velocity of the particle (see LinearDampingSet)
   float linearDamping;
   /// Used to slow the angular velocity of the particle (see LinearDampingSet)
   float angularDamping;
   
   /// If lifeAreaEnabled is true, the particle will expire if it leaves this area.
   Box2F lifeArea;
   
   /// The ParticleEmitter2D that owns this particle.
   ParticleEmitter2D* emitter;
};

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__PARTICLE2D_H__
