#ifndef __FROG__ANIMATEDBACKGROUNDPARTICLEEFFECT_H__
#define __FROG__ANIMATEDBACKGROUNDPARTICLEEFFECT_H__

#include "FrogMemory.h"
#include "AnimatedBackgroundItem.h"

namespace Webfoot {

class AnimatedBackground;
class JSONValue;
class ParticleEffect2D;

//=============================================================================

class AnimatedBackgroundParticleEffect : public AnimatedBackgroundItem
{
public:
   AnimatedBackgroundParticleEffect();
   virtual ~AnimatedBackgroundParticleEffect() {}

   /// Load the background item from the given JSONValue
   void Init(JSONValue* specifications, AnimatedBackground* _background);
   virtual void Deinit();
   virtual void Update(unsigned int dt);
   virtual void Draw(unsigned int currentTime);

protected:
   /// Particle effect to be drawn.
   ParticleEffect2D* particleEffect;

   typedef AnimatedBackgroundItem Inherited;
};

//=============================================================================

} // namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUNDPARTICLEEFFECT_H__
