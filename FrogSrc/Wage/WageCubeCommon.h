#ifndef __WAGECUBECOMMON_H__
#define __WAGECUBECOMMON_H__

#include "Frog.h"
#include "WageTexture.h"

/// Number of vertices per cube
#define CUBE_VERTEX_COUNT 8
/// Number of vertices per cube side
#define CUBE_SIDE_VERTEX_COUNT 4
/// Number of sides per cube
#define CUBE_SIDE_COUNT 6

namespace Webfoot {

//==============================================================================

class WageCubeCommon
{
public:
   WageCubeCommon();
   ~WageCubeCommon() {}

   enum Face
   {
      TOP_FACE,
      BOTTOM_FACE,
      LEFT_FACE,
      FRONT_FACE,
      RIGHT_FACE,
      BACK_FACE
   };
   
   virtual void Init(int _cellIndex, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void Draw();
   virtual void DrawFace(Face face, int v1, int v2, int v3, int v4) = 0;
   virtual void DrawOutline(const ColorRGBA8& lineColor, float lineWidth);
   virtual void DrawFaceOutline(int v0, int v1, int v2, int v3, const ColorRGBA8& lineColor, float lineWidth);

   /// Get the name of the wage cube
   const char* NameGet() { return name; }
   /// Returns the number of the wage cube
   int NumberGet() { return number; }
   /// Returns the cell index of the wage cube
   int CellIndexGet() { return cellIndex; }
   /// Check if this object is marked for deletion by the wage engine
   bool MarkForDeleteCheck() { return markForDelete; }
   /// Mark this object for deletion by the wage engine
   void MarkForDeleteSet() { markForDelete = true; }
   /// Returns the left bound of the cube
   float MinXGet() { return vertices[0].x; }
   /// Returns the right bound of the cube
   float MaxXGet() { return vertices[2].x; }
   /// Returns the highest y value of the top face
   float MaxYGet() 
   { 
      float maxY = vertices[0].y;
      for(int i = 1; i < 4; i++)
         maxY = maxY > vertices[i].y ? maxY : vertices[i].y;
      return maxY;
   }
   /// Returns the lowest y value of the bottom face
   float MinYGet()
   { 
      float minY = vertices[4].y;
      for(int i = 5; i < 8; i++)
         minY = minY < vertices[i].y ? minY : vertices[i].y;
      return minY;
   }
   /// Returns the back bound of the cube
   float MinZGet() { return vertices[1].z; }
   /// Returns the front bound of the cub
   float MaxZGet() { return vertices[0].z; }
   /// Offsets the altitude (y) values of all the vertices (to move a cube up/down)
   void FaceAltitudeOffsetSet(float offset);
   /// Gets the face up altitude at the center of the cube
   float FaceUpAltitudeGet();
   /// Sets the face up vertices to the given altitude
   void FaceUpAltitudeSet(float altitude) { VerticesSet(altitude, altitude, altitude, altitude, vertices[4].y, vertices[5].y, vertices[6].y, vertices[7].y); }
   /// Get the altitude of the middle of the left edge
   float FaceUpLeftAltitudeGet();
   /// Get the altitude of the middle of the right edge
   float FaceUpRightAltitudeGet();
   /// Get the altitude of the middle of the back edge
   float FaceUpBackAltitudeGet();
   /// Get the altitude of the middle of the front edge
   float FaceUpFrontAltitudeGet();
   /// Get the altitude of the front left corner of the cube
   float FaceUpFrontLeftAltitudeGet() { return vertices[0].y; }
   /// Get the altitude of the front right corner of the cube
   float FaceUpFrontRightAltitudeGet() { return vertices[3].y; }
   /// Get the altitude of the back left corner of the cube
   float FaceUpBackLeftAltitudeGet() { return vertices[1].y; }
   /// Get the altitude of the back right corner of the cube
   float FaceUpBackRightAltitudeGet() { return vertices[2].y; }
   /// Gets the face up altitude at the specified position
   float FaceUpAltitudeGet(const Point3F& position);
   /// Adjusts the altitude (y) value of the top face vertices by the offset
   void FaceUpAltitudeOffsetSet(float offset);
   /// Gets the face down altitude at the center of the cube
   float FaceDownAltitudeGet();
   /// Sets the face down vertices to the given altitude
   void FaceDownAltitudeSet(float altitude) { VerticesSet(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y, altitude, altitude, altitude, altitude); }
   /// Get the altitude of the middle of the left edge
   float FaceDownLeftAltitudeGet();
   /// Get the altitude of the middle of the right edge
   float FaceDownRightAltitudeGet();
   /// Get the altitude of the middle of the back edge
   float FaceDownBackAltitudeGet();
   /// Get the altitude of the middle of the front edge
   float FaceDownFrontAltitudeGet();
   /// Gets the face down altitude at the specified position
   float FaceDownAltitudeGet(const Point3F& position);
   /// Adjusts the altitude (y) value of the down face vertices by the offset
   void FaceDownAltitudeOffsetSet(float offset);
   /// Adjusts the altitude (y) value of the down left face vertices
   void FaceDownAltitudeLeftOffsetSet(float offset);
   /// Adjusts the altitude (y) value of the down right face vertices
   void FaceDownAltitudeRightOffsetSet(float offset);
   /// Sets the vertices 1-8
   void VerticesSet(int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7);
   /// Sets the vertices 1-8
   void VerticesSet(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7);
   /// Sets the input pointer with the y values of the cube
   void VerticesGet(float* verticesOut);
   /// Sets the cube color
   void ColorSet(const ColorRGBA8& _color) { color = _color; }
   /// Gets the cube color
   ColorRGBA8 ColorGet() { return color; } 
   /// Returns the center of top face of the cube
   Point3F CenterPositionGet() { return Point3F::Create((vertices[0].x + vertices[3].x) / 2.0f, 
                                                        (vertices[0].y + vertices[1].y + vertices[2].y + vertices[3].y) / 4.0f,
                                                        (vertices[0].z + vertices[1].z) / 2.0f); } 
   /// Returns the center of the front edge of top face of the cube
   Point3F CenterFrontPositionGet() { return Point3F::Create((vertices[0].x + vertices[3].x) / 2.0f, 
                                                        (vertices[0].y + vertices[1].y + vertices[2].y + vertices[3].y) / 4.0f,
                                                         vertices[0].z - 0.01f); } 
   /// Returns the x rotation of the top face
   float RotationXGet() 
   {
      float dz = MaxZGet() - MinZGet();
      float dy = vertices[1].y - vertices[0].y;

      return RadiansToDegrees(atan(dy/dz)) - 90.0f;
   }
   /// Returns the z rotation of the top face
   float RotationZGet()
   {
      float dx = MaxXGet() - MinXGet();
      float dy = vertices[3].y - vertices[0].y;

      return RadiansToDegrees(atan(dy/dx));
   }
   /// Changes the altitude of the rear vertices on the up/down face
   void FaceBackOffsetSet(float offset);
   /// Changes the altitude of the two rear vertices on the up face
   void FaceUpBackOffsetSet(float offset);
   /// Changes the altitude of the two rear vertices on the down face
   void FaceDownBackOffsetSet(float offset);
   /// Rotate the cube 90 degrees
   void Rotate();
   /// Rotate the top face of the cube 90 degrees
   void FaceUpRotate();
   /// Rotate the bottom face of the cube 90 degrees
   void FaceDownRotate();
   /// Returns the calculated collision box (height is from the lowest point on the cube to the  highest point)
   Box3F CollisionBoxGet() { return collisionBox; }
   /// Returns the texture requested
   WageTexture* TextureGet(Face face) { return textures[face]; }

protected:
   /// Name of the wage cube
   const char* name;
   /// Number of the wage cube
   int number;
   /// Marked for deletion?
   bool markForDelete;
   /// Cell index number in the x-z grid
   int cellIndex;
   /// Vertices to define the cube in world coordinates
   /// When facing the front of the cube, the vertices are (vertices[5] is the bottom/rear/left vertex):
   ///    1------2
   ///   /      /|
   ///  /      / |
   /// 0------3  |
   /// |      |  6
   /// |      | /
   /// |      |/
   /// 4------7
   Point3F vertices[CUBE_VERTEX_COUNT];
   /// 0Up/1Down/2Left/3Front/4Right/5Back textures
   WageTexture* textures[CUBE_SIDE_COUNT];
   /// Texture stretch
   float textureStretch;
   /// Color to apply to the cube
   ColorRGBA8 color;
   /// Collision box
   Box3F collisionBox;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGECUBECOMMON_H__
