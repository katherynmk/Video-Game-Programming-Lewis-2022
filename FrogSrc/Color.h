#ifndef __FROG__COLOR_H__
#define __FROG__COLOR_H__

#include "FrogMemory.h"
#include "Port.h"
#include "FrogMath.h"

namespace Webfoot {

struct ColorRGBA8;
struct ColorHSVA8;

//==============================================================================

/// 24-bit truecolor data.
struct ColorRGB8
{
   /// Red component
   uchar red;
   /// Green component
   uchar green;
   /// Blue component
   uchar blue;

   /// Return a ColorRGB8 with the given components.
   static ColorRGB8 Create(uchar red, uchar green, uchar blue);
   /// Return a ColorRGB8 converted from the given ColorRGBA8.
   static ColorRGB8 Create(const ColorRGBA8& other);
};

//-----------------------------------------------------------------------------

/// Linearly interpolate between the given colors.
template<>
inline ColorRGB8 Lerp<ColorRGB8>(const ColorRGB8& valueBegin, const ColorRGB8& valueEnd, float t, bool clampT)
{
   ColorRGB8 result;
   if(clampT)
      t = std::min(1.0f, std::max(0.0f, t));
   float oneMinusT = 1.0f - t;
   result.red = (uchar)(((float)valueBegin.red * oneMinusT) + ((float)valueEnd.red * t));
   result.green = (uchar)(((float)valueBegin.green * oneMinusT) + ((float)valueEnd.green * t));
   result.blue = (uchar)(((float)valueBegin.blue * oneMinusT) + ((float)valueEnd.blue * t));
   return result;
}

//==============================================================================

/// 32-bit truecolor data with alpha.
struct ColorRGBA8
{
   /// Red component
   uchar red;
   /// Green component
   uchar green;
   /// Blue component
   uchar blue;
   /// Alpha component
   uchar alpha;
   
   bool operator==(const ColorRGBA8& other) const
   {
      return (red == other.red) && (green == other.green) && (blue == other.blue) && (alpha == other.alpha);
   }
   bool operator!=(const ColorRGBA8& other) const
   {
      return (red != other.red) || (green != other.green) || (blue != other.blue) || (alpha != other.alpha);
   }

   /// Return a ColorRGBA8 with the given components.
   static ColorRGBA8 Create(uchar red, uchar green, uchar blue, uchar alpha);
   /// Return a ColorRGBA8 converted from the given ColorRGB8.
   static ColorRGBA8 Create(const ColorRGB8& other);
   /// Return a ColorRGBA8 converted from the given ColorHSVA8.
   static ColorRGBA8 Create(const ColorHSVA8& other);
};

//-----------------------------------------------------------------------------

extern ColorRGBA8 COLOR_RGBA8_WHITE;
extern ColorRGBA8 COLOR_RGBA8_RED;
extern ColorRGBA8 COLOR_RGBA8_GREEN;
extern ColorRGBA8 COLOR_RGBA8_BLUE;
extern ColorRGBA8 COLOR_RGBA8_YELLOW;
extern ColorRGBA8 COLOR_RGBA8_MAGENTA;
extern ColorRGBA8 COLOR_RGBA8_CYAN;
extern ColorRGBA8 COLOR_RGBA8_ORANGE;
extern ColorRGBA8 COLOR_RGBA8_BLACK;
extern ColorRGBA8 COLOR_RGBA8_TRANSPARENT;
extern ColorRGBA8 COLOR_RGBA8_TRANSPARENT_WHITE;

//-----------------------------------------------------------------------------

/// Linearly interpolate between the given colors.
template<>
inline ColorRGBA8 Lerp<ColorRGBA8>(const ColorRGBA8& valueBegin, const ColorRGBA8& valueEnd, float t, bool clampT)
{
   ColorRGBA8 result;
   if(clampT)
      t = std::min(1.0f, std::max(0.0f, t));
   float oneMinusT = 1.0f - t;
   result.red = (uchar)(((float)valueBegin.red * oneMinusT) + ((float)valueEnd.red * t));
   result.green = (uchar)(((float)valueBegin.green * oneMinusT) + ((float)valueEnd.green * t));
   result.blue = (uchar)(((float)valueBegin.blue * oneMinusT) + ((float)valueEnd.blue * t));
   result.alpha = (uchar)(((float)valueBegin.alpha * oneMinusT) + ((float)valueEnd.alpha * t));
   return result;
}

//-----------------------------------------------------------------------------

/// Bezier quadratic interpolation
template<>
inline ColorRGBA8 BezierQuadraticInterpolate<ColorRGBA8>(const ColorRGBA8& valueBegin, const ColorRGBA8& valueMiddle, const ColorRGBA8& valueEnd, float t)
{
   ColorRGBA8 result;
   if(t > 1.0f)
      t = 1.0f;
   else if(t < 0.0f)
      t = 0.0f;
   float mt = 1 - t;
   result.red = (uchar)(mt * mt * valueBegin.red + 2 * mt * t * valueMiddle.red + t * t * valueEnd.red);
   result.green = (uchar)(mt * mt * valueBegin.green + 2 * mt * t * valueMiddle.green + t * t * valueEnd.green);
   result.blue = (uchar)(mt * mt * valueBegin.blue + 2 * mt * t * valueMiddle.blue + t * t * valueEnd.blue);
   result.alpha = (uchar)(mt * mt * valueBegin.alpha + 2 * mt * t * valueMiddle.alpha + t * t * valueEnd.alpha);
   return result;
}

//-----------------------------------------------------------------------------

/// Multiply the colors as if each channel was a floating point number between 0 and 1.
ColorRGBA8 Modulate(const ColorRGBA8& colorA, const ColorRGBA8& colorB);

//==============================================================================

/// 8-bits: all for luminance.
struct ColorL8
{
   /// Luminance component
   uchar luminance;
};

//==============================================================================

/// 16-bits: 8 for luminance and 8 for alpha.
struct ColorLA8
{
   /// Luminance component
   uchar luminance;
   /// Alpha component
   uchar alpha;
};

//==============================================================================

/// 16-bits: all for luminance.
struct ColorL16
{
   /// Luminance component
   uint16 luminance;
};

//==============================================================================

/// 24-bit truecolor data for iPhone.
struct ColorBGR8
{
   /// Blue component
   uchar blue;
   /// Green component
   uchar green;
   /// Red component
   uchar red;
};

//==============================================================================

/// 32-bit truecolor data with alpha for iPhone.
struct ColorBGRA8
{
   /// Blue component
   uchar blue;
   /// Green component
   uchar green;
   /// Red component
   uchar red;
   /// Alpha component
   uchar alpha;
};

//==============================================================================

/// 32-bit color defined in terms of hue, saturation, and value with alpha.
struct ColorHSVA8
{
   /// Hue component
   uchar hue;
   /// Saturation component
   uchar saturation;
   /// Value (~brightness) component
   uchar value;
   /// Alpha component
   uchar alpha;

   bool operator==(const ColorHSVA8& other) const
   {
      return (hue == other.hue) && (saturation == other.saturation) && (value == other.value) && (alpha == other.alpha);
   }
   bool operator!=(const ColorHSVA8& other) const
   {
      return (hue != other.hue) || (saturation != other.saturation) || (value != other.value) || (alpha != other.alpha);
   }

   /// Return a ColorHSVA8 converted from the given ColorRGBA8.
   static ColorHSVA8 Create(const ColorRGBA8& other);
};

//==============================================================================

/// 128-bit color with alpha where each component is a 32-bit float.
struct ColorRGBA32F
{
   /// Red component
   float32 red;
   /// Green component
   float32 green;
   /// Blue component
   float32 blue;
   /// Alpha component
   float32 alpha;

   bool operator==(const ColorRGBA32F& other) const
   {
      return (red == other.red) && (green == other.green) && (blue == other.blue) && (alpha == other.alpha);
   }
   bool operator!=(const ColorRGBA32F& other) const
   {
      return (red != other.red) || (green != other.green) || (blue != other.blue) || (alpha != other.alpha);
   }

   /// Return a ColorRGBA32F with the given components.
   static ColorRGBA32F Create(float32 red, float32 green, float32 blue, float32 alpha);
   static ColorRGBA32F Create(const ColorRGB8& other);
   static ColorRGBA32F Create(const ColorRGBA8& other);
};

//-----------------------------------------------------------------------------

extern ColorRGBA32F COLOR_RGBA32F_WHITE;
extern ColorRGBA32F COLOR_RGBA32F_BLACK;
extern ColorRGBA32F COLOR_RGBA32F_TRANSPARENT;
extern ColorRGBA32F COLOR_RGBA32F_TRANSPARENT_WHITE;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__COLOR_H__
