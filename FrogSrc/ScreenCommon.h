#ifndef __FROG__SCREENCOMMON_H__
#define __FROG__SCREENCOMMON_H__

#include "FrogMemory.h"
#include "Matrix43.h"
#include "Point2.h"
#include "Point3.h"
#include "Table.h"
#include "FileManager.h"

namespace Webfoot {

class Quaternion;
class JSONValue;

#define SCREEN_GRAPHICS_PATH_DEFAULT "Graphics"

//==============================================================================

/// Associates a filename suffix with an internal to be applied to Images which
/// use that suffix.
struct ImageScale
{
   float scale;
   const char* suffix;
};

//==============================================================================

/// Functionality common to all Screen implementations
class ScreenCommon
{
public:
   ScreenCommon();

   void Init(HeapID _heapID = HEAP_DEFAULT, const char* _graphicsPath = SCREEN_GRAPHICS_PATH_DEFAULT,
      const char* _graphicsPathSecondary = NULL);
   void Deinit();

   /// Call this before beginning the display calls for a particular frame.
   /// On some platforms, this will wait until a vertical interrupt or equivalent. 
   void PreDraw();

   /// Push a copy of the current matrix onto the stack.
   void MatrixPush();
   /// Pop the top matrix from the stack.
   void MatrixPop();
   /// Set the top matrix.
   void MatrixSet(const Matrix43& transform);
   /// Return the top matrix.
   Matrix43 MatrixGet();
   /// Apply a counter-clockwise rotation about the Z axis to the current matrix.
   void MatrixRotate(float angle);
   /// Apply the given rotation to the current matrix.
   void MatrixRotate(const Quaternion& rotation);
   /// Apply a counter-clockwise pre-rotation about the Z axis to the current matrix.
   void MatrixPreRotate(float angle);
   /// Apply the given pre-rotation to the current matrix.
   void MatrixPreRotate(const Quaternion& rotation);
   /// Apply the given scaling to the x and y of the current matrix.
   void MatrixScale(const Point2F& scale) { MatrixScale(Point3F::Create(scale.x, scale.y, 1.0f)); }
   /// Apply the given scaling to the current matrix.
   void MatrixScale(const Point3F& scale);
   /// Apply the given pre-scaling to the x and y of the current matrix.
   void MatrixPreScale(const Point2F& scale) { MatrixPreScale(Point3F::Create(scale.x, scale.y, 1.0f)); }
   /// Apply the given pre-scaling to the current matrix.
   void MatrixPreScale(const Point3F& scale);
   /// Apply the given translation to the current matrix.
   void MatrixTranslate(const Point2F& translation) { MatrixTranslate(Point3F::Create(translation.x, translation.y, 0.0f)); }
   /// Apply the given translation to the current matrix.
   void MatrixTranslate(const Point3F& translation);
   /// Apply the given pre-translation to the current matrix.
   void MatrixPreTranslate(const Point2F& translation) { MatrixPreTranslate(Point3F::Create(translation.x, translation.y, 0.0f)); }
   /// Apply the given pre-translation to the current matrix.
   void MatrixPreTranslate(const Point3F& translation);

   /// Set the mask matrix.
   void MaskMatrixSet(const Matrix43& transform);
   /// Return the mask matrix.
   Matrix43 MaskMatrixGet() { return maskMatrix; }

   /// Draw a line of the given color and width between the two points.
   virtual void LineDraw(const Point3F& point1, const Point3F& point2, const ColorRGBA8& color, float width = 1.0f, float _additiveBlending = 0.0f) = 0;
   /// Draw an empty rectangle using lines of the given color and width.
   void RectangleDraw(const Box2F& rectangle, const ColorRGBA8& color,
      float lineWidth = 1.0f, float _additiveBlending = 0.0f, const Box2F* clipArea = NULL);
   /// Draw a filled rectangle of the given color.
   virtual void RectangleFilledDraw(const Box2F& _rectangle, const ColorRGBA8& color, float _additiveBlending = 0.0f, const Box2F* clipArea = NULL) = 0;
   /// Draw a circle of the given radius centered at the given point.
   void CircleDraw(const Point3F& center, float radius, const ColorRGBA8& color,
      float lineWidth = 1.0f, float _additiveBlending = 0.0f, int segmentCount = 32);
   /// Draw a circle of the given radius centered at the given point.
   void CircleDraw(const Point2F& center, float radius, const ColorRGBA8& color,
      float lineWidth = 1.0f, float _additiveBlending = 0.0f, int segmentCount = 32);

   /// Increment the polygon count for the current frame.
   void PolygonCountAdd(int polygons) { polygonCount += polygons; }
   /// Return the number of polygons drawn since the last call to PreDraw.
   int PolygonCountGet() { return polygonCount; }
   /// Return the maximum number of polygons drawn on a single frame.
   int PolygonCountMaxGet() { return polygonCountMax; }

   /// Increment the number of draw calls for the current frame.
   void DrawCallCountAdd(int drawCalls) { drawCallCount += drawCalls; }
   /// Return the number of draw calls since the last call to PreDraw.
   int DrawCallCountGet() { return drawCallCount; }
   /// Return the maximum number of draw calls made on a single frame.
   int DrawCallCountMaxGet() { return drawCallCountMax; }

   /// Note that the application should act as though it has lost the OS focus.
   /// Each call to this function should have a corresponding call to OSFocusGain.
   /// This will trigger the "OnOSFocusLose" event, if the focus is not already currently lost.
   void OSFocusLose();
   /// Note that the application has regained the OS focus.  This will trigger the OnOSFocusGain
   /// event if this call brings the counter back down to zero.
   void OSFocusGain();
   /// Alternative to OSFocusLose and OSFocusGain for telling the game directly
   /// whether it has the focus.
   void OSFocusSet(bool hasFocus);
   /// Return true if the game should consider itself to currently have the focus.
   bool OSFocusCheck() { return osFocusCount > 0; }

   /// Return true if the game is in fullscreen mode, as opposed to windowed
   /// mode.
   virtual bool FullscreenCheck() { return true; }

   /// Return true if the screen is currently using MSAA anti-aliasing.
   virtual bool AntiAliasingModeMSAACheck() { return false; }

   /// Set whether the color buffer should automatically be cleared at the
   /// beginning of a frame.  This is not necessarily followed on all
   /// platforms.
   void ColorBufferClearOnPreDrawSet(bool _colorBufferClearOnPreDraw) { colorBufferClearOnPreDraw = _colorBufferClearOnPreDraw; }
   /// Return true if the color buffer should automatically be cleared at the
   /// beginning of a frame.  This is not necessarily followed on all
   /// platforms.
   bool ColorBufferClearOnPreDrawCheck() { return colorBufferClearOnPreDraw; }

   /// Add an ImageScale.
   void ImageScaleAdd(float scale, const char* suffix);
   /// Add a set of ImageScales based on the given JSON specifications.
   void ImageScalesAdd(JSONValue* imageScalesSpecs);
   /// Return the number of ImageScales.
   int ImageScaleCountGet() { return imageScales.SizeGet(); }
   /// Return the filename suffix for the given ImageScale.
   const char* ImageScaleSuffixGet(int imageScaleIndex);
   /// Return the scale amount for the given ImageScale.
   float ImageScaleAmountGet(int imageScaleIndex);

protected:
   /// True if there has been a change to the model-view matrix stack since the
   /// last time it was given to the underlying graphics API.
   bool modelViewMatrixDirty;
   /// Stack of model-view matrices
   Table<Matrix43> modelViewMatrixStack;
   /// True if there has been a change to the mask matrix since the
   /// last time it was given to the underlying graphics API.
   bool maskMatrixDirty;
   /// Matrix for the mask texture.
   Matrix43 maskMatrix;
   /// Number of polygons drawn since the last call to PreDraw.
   int polygonCount;
   /// Greatest value reached by 'polygonCount'.
   int polygonCountMax;
   /// Number of draw calls made since the last call to PreDraw.
   int drawCallCount;
   /// Greatest value reached by 'drawCallCount'.
   int drawCallCountMax;
   /// Counter to keep track of the number of OSFocusLose calls versus the number of OSFocusGain calls.
   int osFocusCount;
   /// Heap to use for normal allocations.
   HeapID heapID;
   /// Return true if the color buffer should automatically be cleared at the
   /// beginning of a frame.  This is not necessarily followed on all
   /// platforms.
   bool colorBufferClearOnPreDraw;
   /// Collection of Image filename suffixes and associated internal scales to
   /// be applied to the images.
   Table<ImageScale*> imageScales;
   /// True if 'imageScales' is initialized.
   bool imageScalesInitialized;
};

//==============================================================================

} // namespace Webfoot

#endif //#ifdef __FROG__SCREENCOMMON_H__
