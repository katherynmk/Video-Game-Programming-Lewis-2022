#ifndef __FROG__PARTICLEMODIFIER2D_H__
#define __FROG__PARTICLEMODIFIER2D_H__

#include "FrogMemory.h"
#include "Color.h"
#include "SplineLinear.h"

namespace Webfoot {

class ParticleEffectParticle2D;
class ParticleEffectEmitter2D;
class Particle2D;
class Progression;

//==============================================================================

/// ParticleModifier2D is a base class for objects attached to individual
/// particles to affect something.  For example, they could change the color
/// of the particle over time or add accelerations according to different
/// conditions.
class ParticleModifier2D
{
public:
   ParticleModifier2D() { particle = NULL; }
   virtual ~ParticleModifier2D() {}
   
   void Init(ParticleEffectParticle2D* _particle) { particle = _particle; }
   virtual void Deinit() { particle = NULL; }
   
   /// Called on every update of the associated particle.
   virtual void Update(unsigned int dt) { (void)dt; }
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate() {}
   
protected:
   /// Particle affected by this modifier.
   ParticleEffectParticle2D* particle;
};

//==============================================================================

/// ParticleEmitterModifier2D is a base class for objects that affect groups
/// of particles or the emitter itself.
class ParticleEmitterModifier2D
{
public:
   ParticleEmitterModifier2D() { emitter = NULL; }
   virtual ~ParticleEmitterModifier2D() {}
   
   void Init(ParticleEffectEmitter2D* _emitter) { emitter = _emitter; }
   virtual void Deinit() { emitter = NULL; }
   
   /// Called on every update of the associated emitter.
   virtual void Update(unsigned int dt) { (void)dt; }
   /// Called when a particle is emitted by the associated emitter, just before
   /// the particle is activated.
   virtual void OnEmit(Particle2D* particle) { (void)particle; }

protected:
   /// Emitter affected by this modifier.
   ParticleEffectEmitter2D* emitter;
};

//==============================================================================

/// ParticleEmitterModifierColorLinear2D is a ParticleEmitterModifier2D that
/// interpolates a particle's color over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierColorLinear2D : public ParticleEmitterModifier2D
{
public:
   virtual ~ParticleEmitterModifierColorLinear2D() {}
   
   void Init(ParticleEffectEmitter2D* _emitter, const ColorRGBA8& _colorBegin,
      const ColorRGBA8& _colorEnd, Progression* _progression);
   
   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle2D* particle);
   
   typedef ParticleEmitterModifier2D Inherited;
   
protected:
   /// Color of a particle at the beginning of its life.
   ColorRGBA8 colorBegin;
   /// Color of a particle at the end of it's normal lifespan.
   ColorRGBA8 colorEnd;
   /// Interpolation will follow this progression.
   Progression* progression;
};

//==============================================================================

/// ParticleEmitterModifierColorSplineLinear2D is a ParticleEmitterModifier2D that
/// interpolates a particle's color over time according to a SplineLinear.
/// By default, this is only valid if the normal duration of the particle's
/// life is defined.  In this case, the times should be normalized from 0 to
/// 1000 milliseconds, as though the particle's life was 1 second long.  The
/// sampling of the spline is adjusted based on the actual expected life of the
/// particle.  However, it is also possible to request that the times specified
/// in the spline be used as-is.  In this case, the normal duration of the
/// particle's life need not be defined.
class ParticleEmitterModifierColorSplineLinear2D : public ParticleEmitterModifier2D
{
public:
   virtual ~ParticleEmitterModifierColorSplineLinear2D() {}

   void Init(ParticleEffectEmitter2D* _emitter, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle2D* particle);

   typedef ParticleEmitterModifier2D Inherited;

protected:
   /// Spline which controls the progression of color over the particle's life.
   SplineLinear<ColorRGBA8> colorSplineLinear;
   /// True if time should be normalized to the expected life of the particle.
   bool timeNormalized;
};

//==============================================================================

/// ParticleEmitterModifierAdditiveBlendingLinear2D is a ParticleEmitterModifier2D that
/// interpolates a particle's additive blending setting over time according to a given progression.
/// This is only valid if the normal duration of the particle's life is defined.  
class ParticleEmitterModifierAdditiveBlendingLinear2D : public ParticleEmitterModifier2D
{
public:
   virtual ~ParticleEmitterModifierAdditiveBlendingLinear2D() {}
   
   void Init(ParticleEffectEmitter2D* _emitter, float _additiveBlendingBegin,
      float _additiveBlendingEnd, Progression* _progression);
   
   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle2D* particle);
   
   typedef ParticleEmitterModifier2D Inherited;
   
protected:
   /// Additive blending of a particle at the beginning of its life.
   float additiveBlendingBegin;
   /// Additive blending of a particle at the end of it's normal lifespan.
   float additiveBlendingEnd;
   /// Interpolation will follow this progression.
   Progression* progression;
};

//==============================================================================

/// ParticleModifierScaleLinear2D is a ParticleModifier2D that interpolates a
/// particle's scale over time according to a given progression.  This is only
/// valid if the normal duration of the particle's life is defined.  
class ParticleModifierScaleLinear2D : public ParticleModifier2D
{
public:
   virtual ~ParticleModifierScaleLinear2D() {}
   
   void Init(ParticleEffectParticle2D* _particle, const Point2F& _scaleBeginMin,
      const Point2F& _scaleBeginMax, const Point2F& _scaleEndMin,
      const Point2F& _scaleEndMax, Progression* _progression);
   
   /// Called on every update of the associated particle.
   virtual void Update(unsigned int dt);
   /// Called when the associated particle is activated to begin a new life.
   virtual void OnActivate();

   typedef ParticleModifier2D Inherited;
   
protected:
   /// Minimum scale of a particle at the beginning of its lifetime.
   Point2F scaleBeginMin;
   /// Maximum scale of a particle at the beginning of its lifetime.
   Point2F scaleBeginMax;
   /// Minimum scale of a particle at the beginning of its lifetime.
   Point2F scaleEndMin;
   /// Maximum scale of a particle at the beginning of its lifetime.
   Point2F scaleEndMax;
   /// Interpolation will follow this progression.
   Progression* progression;
   /// Initial scale to use for the current life of the particle.
   Point2F scaleBegin;
   /// Final scale to use for the current life of the particle.
   Point2F scaleEnd;
};

//==============================================================================

/// ParticleEmitterModifierScaleSplineLinear2D is a ParticleEmitterModifier2D
/// that interpolates a particle's scale over time according to a SplineLinear.
/// By default, this is only valid if the normal duration of the particle's
/// life is defined.  In this case, the times should be normalized from 0 to
/// 1000 milliseconds, as though the particle's life was 1 second long.  The
/// sampling of the spline is adjusted based on the actual expected life of the
/// particle.  However, it is also possible to request that the times specified
/// in the spline be used as-is.  In this case, the normal duration of the
/// particle's life need not be defined.
class ParticleEmitterModifierScaleSplineLinear2D : public ParticleEmitterModifier2D
{
public:
   virtual ~ParticleEmitterModifierScaleSplineLinear2D() {}

   void Init(ParticleEffectEmitter2D* _emitter, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void OnEmit(Particle2D* particle);

   typedef ParticleEmitterModifier2D Inherited;

protected:
   /// Spline which controls the progression of scale over the particle's life.
   SplineLinear<Point2F> scaleSplineLinear;
   /// True if time should be normalized to the expected life of the particle.
   bool timeNormalized;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PARTICLEMODIFIER2D_H__
