#ifndef __WAGEMANAGERCOMMON_H__
#define __WAGEMANAGERCOMMON_H__

#include "Frog.h"

#include "WageDrawable.h"
#include "WageCamera.h"
#include "WageCube.h"
#include "WageMovingCube.h"
#include "WageSprite.h"
#include "WageMoodSprite.h"
#include "WageScrollingBackground.h"
#include "WageParticleEffect.h"

/// Vertical field of view, in degrees.
#define FIELD_OF_VIEW_VERTICAL 40.0f
/// Number of coordinates in a vertex position
#define POSITION_COORDS_PER_VERTEX 3

namespace Webfoot {

//==============================================================================

class WageManagerCommon
{
public:
typedef struct
{
   int32 number;
   float x;
   float y;
   float z;
   bool  invertX;
   bool  invertY;
   int32 transparency;
} MapSprite;

typedef struct
{
   int32 number;
   int32 y0;
   int32 y1;
   int32 y2;
   int32 y3;
   int32 y4;
   int32 y5;
   int32 y6;
   int32 y7;
} MapCube;

   WageManagerCommon();
   
   void Init();
   void Deinit();

   /// Call this on every frame to update the cubes and sprites
   virtual void Update(unsigned int dt);
   /// Loads the specified level, returns true if successful, false if there aren't any more levels
   bool LevelLoad(int levelNumber);
   /// Loads the previous level, returns true if successful, false if there aren't any more levels
   bool PreviousLevelLoad();
   /// Loads the next level, returns true if successful, false if there aren't any more levels
   bool NextLevelLoad();
   /// Reloads the current level, returns true if successful
   bool LevelReload();
   /// Returns the current level number
   int LevelNumberGet() { return levelNumber; }
   /// Returns the total level count
   int LevelCountGet() { return levelCount; }

   /// Level editor functions
   /// Set the level edit mode (cannot be unset)
   void LevelEditSet() { levelEditFlag = true; }
   /// Check if the engine is in level edit mode
   bool LevelEditCheck() { return levelEditFlag; }

   /// Get a wage cube by index
   WageCube* CubeGet(int index);
   /// Get a wage cube by position
   WageCube* CubeGet(const Point3F& position);
   /// Get the cube to the right of the current cube (NULL if not found)
   WageCube* AdjacentRightCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the left of the current cube (NULL if not found)
   WageCube* AdjacentLeftCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the front of the current cube (NULL if not found)
   WageCube* AdjacentFrontCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the back of the current cube (NULL if not found)
   WageCube* AdjacentBackCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the right of the current cube (NULL if not found)
   WageCube* AdjacentBackRightCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the left of the current cube (NULL if not found)
   WageCube* AdjacentBackLeftCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the front of the current cube (NULL if not found)
   WageCube* AdjacentFrontRightCubeGet(WageCubeCommon* currentCube);
   /// Get the cube to the back of the current cube (NULL if not found)
   WageCube* AdjacentFrontLeftCubeGet(WageCubeCommon* currentCube);
   /// Returns a table of all the wage cubes
   Table<WageCube*>* CubesGet() { return &cubes; }
   /// Create a cube with the given specifications
   WageCube* CubeCreate(JSONValue* specifications, int index);
   /// Create/add a sprite to be managed by the engine from a sprite template name
   WageCube* CubeAdd(const char* cubeTemplateName, int index);
   /// Create/add a sprite to be managed by the engine from a sprite template number
   WageCube* CubeAdd(const int cubeTemplateNumber, int index);
   /// Add a cube to the managed table
   void CubeAdd(WageCube* cube, int index);
   /// Returns the cube index for the position given
   int CubeIndexGet(const Point3F& position);

   /// Get a wage sprite by index
   WageSprite* SpriteGet(int index) { assert(index < sprites.SizeGet()); return sprites[index]; }
   /// Get a wage sprite by name (only first is returned if multiple of the same name)
   WageSprite* SpriteGet(const char* name);
   /// Create a new wage sprite using a JSON defined template
   WageSprite* SpriteCreate(JSONValue* specifications);
   /// Returns a table of all the managed wage sprites
   Table<WageSprite*>* SpritesGet() { return &sprites; }
   /// Returns a table of sprite collisions
   Table<WageSprite*>* SpriteCollisionListGet(WageSprite* sprite);
   /// Get a count of all the wage sprites
   int SpriteCountGet() { return sprites.SizeGet(); }
   /// Add a sprite to be managed by the engine
   void SpriteAdd(WageSprite* sprite) { sprites.Add(sprite); }
   /// Create/add a sprite to be managed by the engine from a sprite template name
   WageSprite* SpriteAdd(const char* spriteTemplateName);
   /// Create/add a sprite to be managed by the engine from a sprite template number
   WageSprite* SpriteAdd(const int spriteTemplateNumber);

   /// Add a particle effect to be managed by the engine
   void ParticleAdd(WageParticleEffect* particle) { particles.Add(particle); }
   /// Create a particle from the JSON specification and manage it
   WageParticleEffect* ParticleAdd(JSONValue* specification, const Point3F& position, const int startDelay = 0);
   /// Removes a particled managed by the engine but does not deinitialize it
   void ParticleRemove(WageParticleEffect* particle) {       
      for(int i = 0; i < particles.SizeGet(); i++)
      {
         if(particles[i] == particle)
         {
            particles.RemoveIndex(i);
            return;
         }
      }
   }

   /// Get the json specification for the currently loaded level
   JSONValue* LevelDataGet() { return levelData; }
   /// Get the json specification for a cube template by name
   JSONValue* CubeTemplateGet(const char* name);
   /// Get the json specification for a cube template by number
   JSONValue* CubeTemplateGet(int cubeTemplateNumber);
   /// Get the json specification for a sprite template by name
   JSONValue* SpriteTemplateGet(const char* name);
   /// Get the json specification for a sprite template by number
   JSONValue* SpriteTemplateGet(int _spriteTemplateNumber);

   /// Returns the width of the world (X) in cubes
   float WorldWidthGet() { return worldX; }
   /// Returns the depth of the world (Z) in cubes
   float WorldDepthGet() { return worldZ; }
   /// Sets the near plane distance
   void NearPlaneDistanceSet(float _distance) { nearPlaneDistance = _distance; }
   /// Returns the near plane distance
   float NearPlaneDistanceGet() { return nearPlaneDistance; }
   /// Sets the far plane distance
   void FarPlaneDistanceSet(float _distance) { farPlaneDistance = _distance; }
   /// Returns the far plane distance
   float FarPlaneDistanceGet() { return farPlaneDistance; }
   /// Returns the particle scale to scale between particle units and world units
   float ParticleScaleGet() { return particleScale; }
   /// Returns the texture data specifications in use
   JSONValue* TextureDataGet() { return textureData; }
   /// Returns the path of the map file in use
   const char* MapResourceFilenameGet() { return mapResourceFilename; }
   /// Returns the path of the sprite resource file in use
   const char* SpriteResourceFilenameGet() { return spriteResourceFilename; }
   /// Returns the cube origin calculated by the cell's index and cube sizes
   Point2F CubeOriginGet(int cellIndex);
   /// Returns the width (x) of the cube in world units
   float CubeWidthGet() { return cubeX; }
   /// Returns the depth (z) of the cube in world units
   float CubeDepthGet() { return cubeZ; }
   /// Start point get
   Point3F StartPointGet() { return startPoint; }
   /// Get the default z offset for sprite collisions that do not have a collision box defined
   float DefaultZOffsetGet() { return defaultZOffset; }
   /// Set the default z offset for sprite collisions that do not have a collision box defined
   void DefaultZOffsetSet(float _defaultZOffset) { defaultZOffset = _defaultZOffset; } 
   /// Returns the face up altitude at the point specified (x-z)
   float FaceUpAltitudeGet(const Point3F& position);
   /// Set fade from positon
   void FadeFromPositionSet(const Point3F& _fadeFromPosition) { fadeFromPosition = _fadeFromPosition; }
   /// Get the fade from position
   Point3F FadeFromPositionGet() { return fadeFromPosition; }
   /// Is fading to black set?
   bool FadeToBlackCheck() { return (fadeFromDistance != 0.0f && fadeToDistance != 0.0f); }
   /// Returns the interpolated color based on the distance between the fadeFromPosition, the position parameter and the near/far distances
   ColorRGBA8 FadeToBlackColorGet(const Point3F& position, ColorRGBA8 color = COLOR_RGBA8_WHITE);
   /// Returns the background sprite
   Sprite* BackgroundSpriteGet() { return backgroundSprite; }
   /// Sets a new background 
   void BackgroundSpriteSet(const char* _newBackground) { backgroundSprite->AnimationSet(_newBackground); }
   /// Sets a new background 
   void BackgroundSpriteSet(const char* resourceFilename, const char* _newBackground) { backgroundSprite->AnimationSet(resourceFilename, _newBackground); }
   /// Returns the wage scrolling background
   WageScrollingBackground* ScrollingBackgroundGet() { return scrollingBackground; }
   /// Return the region of the screen that Wage should use for drawing.
   Box2F DrawRegionGet() { return drawRegion; }
   /// Return the maximum number of pixels by which a fragment may be
   /// translated horizontally for stereoscopy.
   float StereoParallaxMaxGet() { return stereoParallaxMax; }
   /// Set or unset the ability to control a game with the mouse (if supported)
   void MouseControlAllowedSet(bool _mouseControlAllowed) { mouseControlAllowed = _mouseControlAllowed; }
   /// Check if mouse control is allowed
   bool MouseControlAllowedCheck() { return mouseControlAllowed; }
   //// Return the vertical field of view in degrees.
   float FieldOfViewVerticalGet() { return FIELD_OF_VIEW_VERTICAL; }

   /// Make preparations for drawing that are common to both framebuffers for
   /// stereoscopic display.  This is not required for all platforms.
   void PreDrawStereoCommon() {}
   /// Clean up after PreDrawStereoCommon().  This is not required for all platforms.
   void PostDrawStereoCommon() {}
   
   /// Key press handling (up/down/left/right, action, and menu buttons)
   virtual bool KeyUpJustPressed() = 0;
   virtual bool KeyUpPressed() = 0;
   virtual bool KeyDownJustPressed() = 0;
   virtual bool KeyDownPressed() = 0;
   virtual bool KeyLeftJustPressed() = 0;
   virtual bool KeyLeftPressed() = 0;
   virtual bool KeyRightJustPressed() = 0;
   virtual bool KeyRightPressed() = 0;
   virtual bool KeyActionJustPressed() = 0;
   virtual bool KeyActionPressed() = 0;
   virtual bool KeySecondaryActionJustPressed() = 0;
   virtual bool KeySecondaryActionPressed() = 0;
   virtual bool KeyMenuJustPressed() = 0;
   virtual bool KeyMenuPressed() = 0;

protected:
   /// Init a level using the level data json (cube/sprite definitions) and the map data json (positioning definitions)
   void LevelInit();
   /// Deinits/unloads the current level
   void LevelDeinit();
   /// World width in cubes
   float worldX;
   /// World depth in cubes
   float worldZ;
   /// Cube width in world units
   float cubeX;
   /// Cube depth in world units
   float cubeZ;
   /// Camera near plane
   float nearPlaneDistance;
   /// Camera far plane
   float farPlaneDistance;
   /// Particle scale to map particle units to world units
   float particleScale;
   /// Starting point in world units as specified by the level def
   Point3F startPoint;
   /// Region of the screen that Wage should use for drawing.
   Box2F drawRegion;
   /// The maximum number of pixels by which a fragment may be
   /// translated horizontally for stereoscopy.
   float stereoParallaxMax;
   /// Background sprite
   Sprite* backgroundSprite;
   /// Background sprite size (in percent)
   Point2F backgroundSize;
   /// Pointer to scrolling background object in use
   WageScrollingBackground* scrollingBackground;
   /// Managed array of wage cube objects
   Table<WageCube*> cubes;
   /// Managed array of wage sprite objects
   Table<WageSprite*> sprites;
   /// Managed array of wage sprite objects
   Table<WageParticleEffect*> particles;
   /// Levels 
   JSONValue* levels;
   /// Currently loaded level data
   JSONValue* levelData;
   /// Currently loaded texture data
   JSONValue* textureData;
   /// Sprite resource filename 
   const char* spriteResourceFilename;
   /// Map resource filename
   const char* mapResourceFilename;
   /// Total levels available
   int levelCount;
   /// Current level number
   int levelNumber;
   /// Default z offset for WageSprites that do not have a collision box defined
   float defaultZOffset;
   /// Position from which to start the fade to black (if enabled)
   Point3F fadeFromPosition;
   /// Distance from which to start fading
   float fadeFromDistance;
   /// Distance from the fadeFromPosition to stop fading (beyond which is black)
   float fadeToDistance;
   /// Flag to allow mouse control for movement
   bool mouseControlAllowed;

   /// Level editor
   bool levelEditFlag;

private:

};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEMANAGERCOMMON_H__
