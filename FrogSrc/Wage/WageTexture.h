#ifndef __WAGETEXTURE_H__
#define __WAGETEXTURE_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageTexture
{
public:
   WageTexture();
   virtual ~WageTexture() {}
   
   enum Type
   {
      ONE_SHOT,
      LOOP,
      PING_PONG_LOOP
   };
   
   virtual void Init(JSONValue* specifications, const char* key);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual Texture* FrogTextureGet() { return textures[frameIndex]; }
   virtual const char* NameGet() { return name; }
   virtual Type TypeGet() { return type; }

protected:
   /// Name of the texture
   const char* name;
   /// Type of texture animation
   Type type;
   /// Table of textures
   Table<Texture*> textures;
   /// Number of frames
   int frameCount;
   /// Current frame index
   int frameIndex;
   /// Frame duration
   unsigned int frameDuration;
   /// Animation duration for one iteration
   unsigned int animationDuration;
   /// Time the texture has been animating
   unsigned int animationTime;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGETEXTURE_H__
