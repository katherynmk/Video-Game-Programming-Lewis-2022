#ifndef __FROG__DUCK__OPENGL__SCENENODETERRAINTILEDFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODETERRAINTILEDFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "ShaderProgramGLSL.h"
#include "Duck/SceneNodeTerrainTiled.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper class for drawing tiled terrain in Duck.
class SceneNodeTerrainTiledDrawableForwardOpenGL : public SceneNodeTerrainTiledDrawable
{
public:
   virtual void Init(SceneNodeTerrainTiled* _sceneNodeTerrainTiled);
   virtual void Deinit();
   virtual void Draw();

   typedef SceneNodeTerrainTiledDrawable Inherited;

protected:
   void ShaderProgramInit();
   void ShaderProgramDeinit() { shaderProgram.Deinit(); }

   /// Shader for drawing the terrain.
   ShaderProgramGLSL shaderProgram;
};

//==============================================================================

/// Helper class for drawing all cells with a given tile at once.
class SceneNodeTerrainTiledTileForwardOpenGL : public SceneNodeTerrainTiledTile
{
public:
   SceneNodeTerrainTiledTileForwardOpenGL();

   virtual void Deinit();

   typedef SceneNodeTerrainTiledTile Inherited;

   /// Number of cells to be drawn with this tile.
   int cellCount;
   /// OpenGL buffer object for the index buffer.
   GLuint indexBufferID;
   /// OpenGL buffer object for the vertex buffer.
   GLuint vertexBufferID;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODETERRAINTILEDFORWARDOPENGL_H__
