#ifndef __FROG__SCREENOPENGL_H__
#define __FROG__SCREENOPENGL_H__

#include "FrogMemory.h"
#include "ScreenCommon.h"
#include "FrogOpenGL.h"
#include "Point2.h"
#include "Box2.h"
#include "Color.h"
#include "Matrix44.h"
#include "Texture.h"

namespace Webfoot {

class Bitmap;
class ImageOpenGL;
class JSONValue;
class ShaderProgramGLSL;

/// Standard name of the vertex attribute for position.
static const char* const SCREEN_VERTEX_ATTRIBUTE_POSITION_NAME = "attributePosition";
/// Standard name of the vertex attribute for texture coordinate 0.
static const char* const SCREEN_VERTEX_ATTRIBUTE_TEXCOORD0_NAME = "attributeTexCoord0";
/// Generic attribute index to use for positions.
#define SCREEN_VERTEX_ATTRIBUTE_POSITION_INDEX 0
/// Generic attribute index to use for TexCoord0.
#define SCREEN_VERTEX_ATTRIBUTE_TEXCOORD0_INDEX 1
/// Name of the shader uniform for the model view matrix.
#define SCREEN_MODEL_VIEW_MATRIX_NAME "modelViewMatrix"
/// Name of the shader uniform for the projection matrix.
#define SCREEN_PROJECTION_MATRIX_NAME "projectionMatrix"
/// Name of the shader uniform for the modulative color.
#define SCREEN_COLOR_MODULATIVE_NAME "colorModulative"
/// Name of the shader uniform for the mask matrix.
#define SCREEN_MASK_MATRIX_NAME "maskMatrix"
/// Name of the sampler for the mask texture.
#define SCREEN_MASK_TEXTURE_NAME "maskTexture"
/// Name of the sampler for the diffuse texture.
#define SCREEN_DIFFUSE_TEXTURE_NAME "diffuseTexture"
/// Index of the texture unit for the mask texture.
#define SCREEN_MASK_TEXTURE_UNIT_INDEX 0
/// Index of the texture unit for the diffuse texture.
#define SCREEN_DIFFUSE_TEXTURE_UNIT_INDEX 1
/// Default virtual screen width to use for iOS.
#define SCREEN_IOS_GRAPHICS_WIDTH_DEFAULT 1024
/// Default virtual screen height to use for iOS.
#define SCREEN_IOS_GRAPHICS_HEIGHT_DEFAULT 768

/// True if OpenGL 3.0 should be treated as supported.
extern bool FROG_OPENGL_VERSION_3_0;
/// True if OpenGL 3.2 should be treated as supported.
extern bool FROG_OPENGL_VERSION_3_2;
/// True if OpenGL 3.3 should be treated as supported.
extern bool FROG_OPENGL_VERSION_3_3;

//===============================================================================

/// Collection of parameters used to initialize the Screen.
class ScreenParametersOpenGL
{
public:
   ScreenParametersOpenGL()
   {
      allowOpenGL3 = false;
      texturedVertexShaderSources = NULL;
      texturedFragmentShaderSources = NULL;
      untexturedVertexShaderSources = NULL;
      untexturedFragmentShaderSources = NULL;
   }

   /// True if OpenGL 3 may be used if the driver appears to support it.
   bool allowOpenGL3;
   /// Sources to override the default GLSL vertex shader for textured drawing.
   const char* texturedVertexShaderSources;
   /// Sources to override the default GLSL fragment shader for textured drawing.
   const char* texturedFragmentShaderSources;
   /// Sources to override the default GLSL vertex shader for untextured drawing.
   const char* untexturedVertexShaderSources;
   /// Sources to override the default GLSL fragment shader for untextured drawing.
   const char* untexturedFragmentShaderSources;
};

//===============================================================================

/// Abstracts the details of setting up, using, and cleaning up the ability
/// to draw to the screen.
class ScreenOpenGL : public ScreenCommon
{
public:
   /// Specifies how the Screen class should handle differences between the
   /// the actual framebuffer size and the virtual screen resolution requested
   /// by the application programmer.
   enum OutputScaling
   {
      /// Stretch the output so that the requested virtual screen size covers
      /// the entire screen.
      STRETCH_FULL,
      /// Use the requested virtual screen size, and scale the output to fit
      /// the screen.  Use pillarbox or letter box if necessary to avoid
      /// distortion.
      STRETCH_KEEP_ASPECT_RATIO
      /// The virtual screen size is adjusted to match the actual framebuffer
      /// size.  The requested virtual screen size could act as a hint when
      /// choosing a fullscreen display mode, but the final virtual screen size
      /// may or may not be the requested size.
      //NO_STRETCH
   };

   ScreenOpenGL();

   /// Call this before beginning the display calls for a particular frame.
   /// On some platforms, this will wait until a vertical interrupt or equivalent. 
   void PreDraw();
   /// Call this when finished making display calls for a particular frame.
   void PostDraw();

   /// Return true if between a PreDraw and a PostDraw call.
   bool MidDrawCheck() { return midDraw; }

   /// Return the dimensions of the window area for the game, from the
   /// perspective of the application.  This does not include any bars for
   /// letterbox or pillarbox.  This is ideally in pixels, but not necessarily.
   Point2I SizeGet() { return size; }
   /// Return the width from SizeGet.
   int WidthGet() { return size.x; }
   /// Return the height from SizeGet.
   int HeightGet() { return size.y; }

   /// Set the color used to clear the screen on every frame.  For now, this
   /// includes the bars for pillarbox and letterbox added to maintain an
   /// aspect ratio when running in fullscreen.
   void ClearColorSet(const ColorRGBA8& color);

   /// Draw a line of the given color and width between the two points.
   virtual void LineDraw(const Point3F& point1, const Point3F& point2, const ColorRGBA8& color, float width = 1.0f, float _additiveBlending = 0.0f);
   /// Draw a line of the given color and width between the two points.
   void LineDraw(const Point2F& point1, const Point2F& point2, const ColorRGBA8& color, float width = 1.0f, float _additiveBlending = 0.0f);
   /// Do not use this in general.  This is a form of LineDraw for 
   /// experimenting with different kinds of blending in OpenGL.  For now,
   /// 'color' should have an alpha of 255.
   void LineDrawAlternate(const Point2F& point1, const Point2F& point2, const ColorRGBA8& color, float width = 1.0f);
   /// Draw a filled rectangle of the given color.
   virtual void RectangleFilledDraw(const Box2F& _rectangle, const ColorRGBA8& color, float _additiveBlending = 0.0f, const Box2F* clipArea = NULL);
   /// Draw a textured quad using the given vertex positions and texture
   /// coordinates.  'vertexBuffer' should point to an array of floats grouped
   /// into 4 sets of 5.  The first 3 of each group give the x, y, and z
   /// position coordiantes of the associated vertex.  The last 2 give the s
   /// and t texture coordiantes.
   void QuadTexturedDraw(const float* vertexBuffer, Texture* texture,
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, float _additiveBlending = 0.0f);

   /// Prepare for 2D drawing.  If 'forceTrueResolution' is true, do not
   /// compensate for differences between the true resolution and the
   /// resolution from the game's perspective.
   void DrawMode2D(bool forceTrueResolution = false);

   /// Clear the current depth buffer.
   void DepthBufferClear();

   /// Flush any changes to the model-view transform matrix.
   void ModelViewMatrixFlush(bool force = false);
   /// Flush any chagnes to the mask matrix.
   void MaskMatrixFlush(bool force = false);

   /// Return true if Texture objects must retain their associated bitmap pixel data
   /// in order to draw with them.
   bool BitmapDataNeededForDrawingCheck() { return false; }

   /// Create and return a new Image instance.
   ImageOpenGL* ImageCreate(HeapID heapID = HEAP_DEFAULT);
   /// Create and return a new Texture instance.
   TextureOpenGL* TextureCreate(HeapID heapID = HEAP_DEFAULT);
   
   /// Bit flags for which vertex attributes should be enabled.
   enum VertexAttributeFlag
   {
      /// Position
      VERTEX_ATTRIBUTE_POSITION_FLAG = 1,
      /// Texture coordinates
      VERTEX_ATTRIBUTE_TEXCOORD0_FLAG = 2
   };
   
   /// Enable and disable vertex attributes.
   void VertexAttributesEnabledSet(uint32 _vertexAttributesEnabled);
   /// Enable or disable the given native vertex attribute.
   void VertexAttributeNativeEnabledSet(int vertexAttributeIndexNative, bool enabled);
   /// Set whether texture mapping should be enabled or disabled.
   void TexturingEnabledSet(bool _texturingEnabled, bool force = false);
   /// Set which mask texture to use for 2D drawing, if any.
   void MaskTextureSet(Texture* _maskTexture, bool force = false);
   /// Return which mask texture to use for 2D drawing, if any.
   Texture* MaskTextureGet();
   /// Set the color with which 2D drawing is blended.  This will pre-multiply
   /// the alpha internally.  Set '_additiveBlending' to [0, 1] depending on
   /// whether you want normal alpha blending, additive blending, or something
   /// in between.
   void ColorPremultiplyAlphaSet(const ColorRGBA8& _color, float _additiveBlending = 0.0f, bool force = false);
   
   /// Given a framebuffer size and a list of resolutions for which graphics have
   /// been provided, set '_virtualSize' to the screen resolution to use, from the game's
   /// perspective, set '_graphicsPath' to the path to use as the primary graphics path,
   /// and set '_graphicsPathSecondary' to the secondary graphics path or an empty string if there
   /// should only be one.  This method favors closer aspect ratios over greater screen resolution.
   /// 'graphicsResolutionsProvided' must be a JSON array of Point2Is.  '_virtualSize',
   /// 'graphicsResolutionsProvided', and '_framebufferSize' must all be in
   /// the same orientation.  If no appropriate provided graphics resolution is
   /// found, 'virtualSizeDefault' will be used instead.
   static void iOSResolutionSettingsGet(const Point2I& _framebufferSize,
      JSONValue* graphicsResolutionsProvided, Point2I* _virtualSize,
      char* _graphicsPath, size_t _graphicsPathSize, char* _graphicsPathSecondary,
      size_t _graphicsPathSecondarySize, const Point2I& virtualSizeDefault =
      Point2I::Create(SCREEN_IOS_GRAPHICS_WIDTH_DEFAULT, SCREEN_IOS_GRAPHICS_HEIGHT_DEFAULT));

   /// Return the size of the underlying framebuffer.  Application programmers
   /// should typically ignore this.
   Point2I FramebufferSizeGet() { return framebufferSize; }

   /// Return false if content should generally be restricted to the area of the
   /// screen that remains after applying letterbox or pillarbox.
   bool ExtendedAreaAllowedCheck() { return extendedAreaAllowed; }
   /// Return the full area of the screen from the game's perspective, including
   /// any space taken up by letterbox and pillarbox.  The main area for the
   /// game would be in the center of this region.
   Box2F ExtendedAreaFloatGet() { return extendedAreaFloat; }
   /// Return the rounded version of extendedAreaFloat.
   Box2I ExtendedAreaGet() { return extendedArea; }

   /// Return the maximum region for which the game is generally prepared.
   /// This must be at least as large as the requested size of the screen from
   /// the game's perspective, and it must be centered on that requested area.
   /// Clipping may be extended out to this area even if the full extended area
   /// is not allowed.
   Box2I PreparedAreaGet() { return preparedArea; }
   /// Return the subset of the prepared area that should be visible.
   Box2F PreparedAreaVisibleFloatGet();
   /// Return the rounded version of 'PreparedAreaVisibleFloatGet'.
   Box2I PreparedAreaVisibleGet();

#if !FROG_OPENGL_ES
   /// Copy the contents of the framebuffer to the given Bitmap.  In most
   /// cases, this will be the front framebuffer.  Be sure to provide an RGBA8
   /// or RGB8 Bitmap with the correct size allocated.
   void ScreenshotCopy(Bitmap* screenCopy);

   /// Return the maximum texture size allowed for a 32-bit texture on this computer.
   int TextureSizeMax32Get() { return textureSizeMax32; }
   /// Return the maximum texture size allowed for a 24-bit texture on this computer.
   int TextureSizeMax24Get() { return textureSizeMax24; }

   /// Return the major version number of the main GLSL version supported on
   /// this system.
   int GLSLVersionMajorGet() { return glslVersionMajor; }
   /// Return the minor version number of the main GLSL version supported on
   /// this system.
   int GLSLVersionMinorGet() { return glslVersionMinor; }
#endif //#if !FROG_OPENGL_ES

   /// Return the vertex attribute location for position.
   GLint VertexAttributePositionLocationGet();
   /// Return the vertex attribute location for TexCoord0.
   GLint VertexAttributeTexCoord0LocationGet();

   /// Use the given 4x4 matrix as the projection matrix, using OpenGL
   /// projection matrix conventions.  This may not be available on all
   /// platforms.
   void ProjectionMatrixSet(const Matrix44& _projectionMatrix);

   /// Use the given 4x4 matrix as the projection matrix.
   void ProjectionNativeSet(const Matrix44& _projectionMatrixNative);

   /// Set the currently active native shader program.  Return 0 if none are
   /// active.
   void ShaderProgramNativeSet(GLuint _nativeShaderProgram);
   /// Return the currently active native shader program.  Return 0 if none are
   /// active.
   GLuint ShaderProgramNativeGet() { return activeShaderProgram; }

   /// Return the shader program for textured 2D drawing.
   ShaderProgramGLSL* ShaderProgramTexturedGet() { return texturedShaderProgram; }
   /// Use the given shader program for textured 2D drawing.
   void ShaderProgramTexturedSet(ShaderProgramGLSL* _texturedShaderProgram) { texturedShaderProgram = _texturedShaderProgram; }
   /// Return the shader program for untextured 2D drawing.
   ShaderProgramGLSL* ShaderProgramUntexturedGet() { return untexturedShaderProgram; }
   /// Use the given shader program for untextured 2D drawing.
   void ShaderProgramUntexturedSet(ShaderProgramGLSL* _untexturedShaderProgram) { untexturedShaderProgram = _untexturedShaderProgram; }

   /// Add  declarations to the beginning of the shader to help with compatibility across different OpenGL versions.
   void ShaderProgramInitialDeclarationsAdd(ShaderProgramGLSL* shaderProgram, bool includeFragmentColor = true);

   /// Return the scale to be applied to the projection matrix to compensate
   /// for aspect ratio, assuming compensation is being requested.
   Point2F AspectRatioCompensationGet();
   /// Return the size of what could be causing an aspect ratio discrepancy.
   virtual Point2I AspectRatioDiscrepancyCauseSizeGet();
   /// Return the area to use for the DrawMode2D projection matrix before aspect
   /// ratio compensation is applied.
   Box2I ProjectionArea2DGet() { return Box2I::Create(0, 0, size.x, size.y); }

   /// Return the maximum level of anisotropic filtering to use.  A value of 1
   /// means anisotropic filtering should be disabled.
   int AnisotropicFilteringLevelMaxGet() { return anisotropicFilteringLevelMax; }
   /// Set the maximum level of anisotropic filtering to use.  A value of 1
   /// means anisotropic filtering should be disabled.
   void AnisotropicFilteringLevelMaxSet(int _anisotropicFilteringLevelMax);

   /// Return the maximum supported number of vertex attributes.  This may be
   /// different from VERTEX_ATTRIBUTE_COUNT_MAX.
   int VertexAttributeCountMaxGet() { return vertexAttributeCountMax; }

   /// Return the normal desktop resolution.
   virtual Point2I DesktopSizeGet() = 0;

   /// Print the list of extensions to the debug output.
   void DebugPrintExtensions();

   #if _DEBUG && PLATFORM_IS_WINDOWS
      /// Return a matrix which can be applied to the projection matrix to
      /// allow the user to zoom in and show the game's logical pixels at 1:1
      /// with the framebuffer pixels.  This also includes a shift amount to
      /// allow moving of the view.
      virtual Matrix43 DebugOneToOneMatrixGet();
   #endif

   enum
   {
      /// Maximum supported number of vertex attributes.  On some platforms,
      /// like OpenGL, this is an oversimplification.  If needed, consider
      /// using the VertexAttributeCountMaxGet method to get a more accurate
      /// number.
      VERTEX_ATTRIBUTE_COUNT_MAX = 8,
      /// Maximum supported number of texture units.
      TEXTURE_UNIT_COUNT_MAX = 8
   };

protected:
   typedef ScreenCommon Inherited;

   /// Details for one mode of a display device.
   struct DisplayMode
   {
      /// Number used to identify this mode.
      PlatformDisplayMode platformDisplayMode;
      /// Width and height of the display in this mode, in pixels.
      Point2I resolution;
   };

   /// Return true if successful and false otherwise.
   bool OpenGLInit(ScreenParametersOpenGL* screenParameters);
   /// Cleanup OpenGL objectes created by OpenGLInit.
   void OpenGLDeinit();

   /// Return the OutputScaling mode that should be in use.
   OutputScaling OutputScalingGet() { return outputScaling; }

   /// Fill the given table with eligible modes for this screen.
   virtual void DisplayModesGet(Table<DisplayMode>* displayModes) { (void)displayModes; }
   /// Set 'bestDisplayMode' to the most appropriate mode based on the current
   /// desktop resolution, given preferred resolution, and whether to
   /// compensate for the aspect ratio.  If 'aspectRatioCompensate' is true,
   /// internally adjust the 'preferredSize' when appropriate to account for
   /// aspect ratio compensation.  Leave it set to false if 'preferredSize'
   /// really is the target size.  Return true if successful.
   bool BestDisplayModeGet(DisplayMode* bestDisplayMode,
      const Point2I& preferredSize, bool aspectRatioCompensate);

   /// Compute the current extended area for the screen.
   void ExtendedAreaRefresh();
   /// Helper function for determining how to scale output to compensate for
   /// differences in aspect ratio.
   Point2F AspectRatioCompensationGetHelper(const Point2I& sourceSize, const Point2I& discrepancyCauseSize);

   /// Return true if an OpenGL error was found.  Show a warning including the
   /// given 'tag' text if a problem is found.
   bool ErrorCheck(const char* tag);

   #if !FROG_OPENGL_ES
      /// Determine the maximum supported texture sizes.
      int TextureSizeMaxGet();
   #endif //#if !FROG_OPENGL_ES

   /// True if between a PreDraw and a PostDraw call.
   bool midDraw;
   /// The height and width of the window for the application programmer.
   Point2I size;
   /// True resolution of the framebuffer in pixels.
   Point2I framebufferSize;
   /// OutputScaling mode that should be in use.
   OutputScaling outputScaling;
   /// False if content should generally be restricted to the area of the
   /// screen that remains after applying letterbox or pillarbox.
   bool extendedAreaAllowed;
   /// Rounded version of 'extendedAreaFloat'.
   Box2I extendedArea;
   /// Full area of the screen from the game's perspective, including
   /// any space taken up by letterbox and pillarbox.  The main area for the
   /// game would be in the center of this region.
   Box2F extendedAreaFloat;
   /// Maximum region for which the game is generally prepared.  This must be
   /// at least as large as the requested size of the screen from the game's
   /// perspective, and it must be centered on that requested area.  Clipping
   /// may be extended out to this area even if the full  extended area is not
   /// allowed.
   Box2I preparedArea;
   /// True if texture mapping should be enabled.
   bool texturingEnabled;
   /// Equivalent of a vertex color.
   ColorRGBA8 modulativeColor;
   /// [0, 1] depending on whether you want normal alpha blending, additive
   /// blending, or something in between.
   float additiveBlending;
   /// Maximum level of anisotropic filtering to use
   int anisotropicFilteringLevelMax;
   /// Maximum level of anisotropic filtering supported by the system.
   int anisotropicFilteringLevelSystemMax;
   /// Cached copy of the projection matrix in the native format.
   Matrix44 projectionMatrixNative;
   /// Actual maximum number of vertex attributes for the current device.  This
   /// may be different from 'VERTEX_ATTRIBUTE_COUNT_MAX'.
   int vertexAttributeCountMax;
   /// Cached values of which native vertex attributes are enabled.
   Table<bool> vertexAttributesNativeEnabled;
   /// Mask texture to use for drawing with full opacity;
   TextureOpenGL* maskTextureOpaque;
   /// Current texture to use as the mask.
   TextureOpenGL* maskTexture;

#if !FROG_OPENGL_ES
   /// Major version number of the main GLSL version supported on this system.
   int glslVersionMajor;
   /// Minor version number of the main GLSL version supported on this system.
   int glslVersionMinor;
   /// Shared vertex array object.
   GLuint vertexArrayID;
#endif //#if !FROG_OPENGL_ES

   /// OpenGL buffer object for the shared vertex buffer.
   GLuint vertexBufferID;
   /// Shader program that is currently in use.
   GLuint activeShaderProgram;
   /// Shader for textured 2D drawing.
   ShaderProgramGLSL* texturedShaderProgram;
   /// Shader for untextured 2D drawing.
   ShaderProgramGLSL* untexturedShaderProgram;
   /// Cached location of the model-view matrix.
   GLint uniformModelViewMatrixLocation;
   /// Cached location of the projection matrix.
   GLuint uniformProjectionMatrixLocation;
   /// Cached location of the modulative color.
   GLint uniformColorModulativeLocation;
   /// Cached location of the mask matrix.
   GLint uniformMaskMatrixLocation;

#if !FROG_OPENGL_ES
   /// Maximum supported 32-bit, square, LOD level 0 texture size for this computer.
   int textureSizeMax32;
   /// Maximum supported 24-bit, square, LOD level 0 texture size for this computer.
   int textureSizeMax24;
#endif //#if !FROG_OPENGL_ES

   friend class ImageOpenGL;
};

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SCREENOPENGL_H__
