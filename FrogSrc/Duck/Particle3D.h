#ifndef __FROG__DUCK__PARTICLE3D_H__
#define __FROG__DUCK__PARTICLE3D_H__

#include "FrogMemory.h"
#include "Box3.h"
#include "Color.h"
#include "Point3.h"

namespace Webfoot {
namespace Duck {

class SceneNode;
class SceneNodeParticleEmitter;
class SceneNodeSprite;

//=============================================================================

/// Class to represent a particle in the 3D particle system.
class Particle3D
{
public:
   Particle3D();
   virtual ~Particle3D() {}

   void Init();
   virtual void Deinit();

   /// Called every frame by the ParticleEmitter3D.
   virtual void Update(unsigned int dt);

   /// This is called after the emitter is done initializing the particle for a
   /// new life.
   virtual void Activate();

   /// End the particle's life.
   /// The particle will not necessarily be available again until the particle system is updated.
   virtual void Expire();

   /// Set the scene node that represents the particle.
   void ParticleSceneNodeSet(SceneNode* _particleSceneNode);

   /// Return the current position of the particle in world space.
   Point3F PositionGet() { return position; }
   /// Set the current position of the particle in world space.
   virtual void PositionSet(const Point3F& _position);
   /// Return the current linear velocity of the particle in world units per second.
   Point3F VelocityGet() { return velocity; }
   /// Set the current linear velocity of the particle in world units per second.
   void VelocitySet(const Point3F& _velocity) { velocity = _velocity; }
   /// Return the current linear acceleration of the particle in world units per second squared.
   Point3F AccelerationGet() { return acceleration; }
   /// Set the current linear acceleration of the particle in world units per second squared.
   void AccelerationSet(const Point3F& _acceleration) { acceleration = _acceleration; }

   /// Return the current rotation in degrees.
   float RotationGet() { return rotation; }
   /// Set the current rotation in degrees.
   void RotationSet(float _rotation);
   /// Return the current angular velocity of the particle in degrees per second.
   float AngularVelocityGet() { return angularVelocity; }
   /// Set the current angular velocity of the particle in degrees per second.
   void AngularVelocitySet(float _angularVelocity) { angularVelocity = _angularVelocity; }
   /// Return the current angular acceleration of the particle in degrees per second squared.
   float AngularAccelerationGet() { return angularAcceleration; }
   /// Set the current angular acceleration of the particle in degrees per second squared.
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

   /// Set the color to apply to all vertices when drawing.
   /// By manipulating the 'alpha' component, you can control the opacity of the sprite.
   void ColorSet(const ColorRGBA8& _color);
   /// Return the color to apply to all vertices when drawing.
   ColorRGBA8 ColorGet() { return color; }

   /// Set '_additiveBlending' to [0, 1] depending on whether you want normal
   /// alpha blending, additive blending, or something in between.
   void AdditiveBlendingSet(float _additiveBlending);
   /// See 'AdditiveBlendingSet'.
   float AdditiveBlendingGet() { return additiveBlending; }

   /// Set the scale of the sprite such that the current animation would have
   /// the given dimensions in world space.
   void SizeSet(const Point2F& _size);
   /// See 'SizeSet'.
   Point2F SizeGet() { return size; }

   /// Make the particle expire if its position (origin) leaves the specified region.
   void LifeAreaSet(const Box3F& _lifeArea) { lifeArea = _lifeArea; lifeAreaEnabled = true; }
   /// Return the area where the particle will expire if it leaves.  This will
   /// only happen if LifeAreaEnabledCheck() is returning true.
   Box3F LifeAreaGet() { return lifeArea; }
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

   /// Set a pointer to the SceneNodeParticleEmitter that owns this particle.
   /// This should only be called by the emitter in question.
   void EmitterSet(SceneNodeParticleEmitter* _emitter) { emitter = _emitter; }
   /// Return the SceneNodeParticleEmitter that owns this particle.
   SceneNodeParticleEmitter* EmitterGet() { return emitter; }

   /// Return true if the particle is between activation and expiration.
   bool ActiveCheck() { return active; }

protected:
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

   /// World-space position.
   Point3F position;
   /// Current velocity of the particle in units per second.
   Point3F velocity;
   /// Current acceleration of the particle in units per second squared.
   Point3F acceleration;

   /// Rotation angle in degrees.
   float rotation;
   /// Current angular velocity of the particle.
   float angularVelocity;
   /// Current angular acceleration of the particle.
   float angularAcceleration;

   /// Used to slow the linear velocity of the particle (see LinearDampingSet)
   float linearDamping;
   /// Used to slow the angular velocity of the particle (see LinearDampingSet)
   float angularDamping;

   /// Color to apply to the particle.
   ColorRGBA8 color;
   /// Addititve blending to use when drawing the particle.
   float additiveBlending;
   /// Size of the particle in world units.
   Point2F size;

   /// If lifeAreaEnabled is true, the particle will expire if it leaves this
   /// area in world space.
   Box3F lifeArea;

   /// The SceneNodeParticleEmitter that owns this particle.
   SceneNodeParticleEmitter* emitter;

   /// Scene node for the particle.
   SceneNode* particleSceneNode;
   /// Pointer to 'particleSceneNode' cast to a SceneNodeSprite.
   SceneNodeSprite* spriteSceneNode;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__PARTICLE3D_H__
