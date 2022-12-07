#ifndef __FROG__FONT_H__
#define __FROG__FONT_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Point2.h"
#include "Color.h"
#include "Utility.h"
#include "Map.h"

namespace Webfoot {

/// When including whitespace characters in the bounds of a string, this is
/// treated as the ascent of those characters.
#define FONT_WHITESPACE_CHARACTER_ASCENT 1.0f
/// When including whitespace characters in the bounds of a string, this is
/// treated as the height of those characters.
#define FONT_WHITESPACE_CHARACTER_HEIGHT 1.0f

//==============================================================================

/// Interface common to all font implementations.
class Font
{
public:
   Font() { lineSpacing = 0.0f; ascentMax = 0.0f; descentMax = 0.0f; }
   virtual ~Font() {}

   void Init(HeapID _heapID = HEAP_DEFAULT);
   virtual void Deinit();

   /// Return the bounds of visible part of the given string.
   /// If 'includeWhitespaceCharacters' is true, treat whitespace characters as
   /// opaque, 1-pixel-tall boxes on the baseline.
   virtual Box2F OpaqueBoundsGet(const char* string, bool includeWhitespaceCharacters = false) = 0;
   virtual Box2F OpaqueBoundsGet(const char* string, int stringBytes, bool includeWhitespaceCharacters = false) = 0;

   /// Draw the given string in the 'alignmentArea' with the alignment as specified.
   /// If 'useMaxAscentDescent' is true, the vertical alignment will be based
   /// on the maximum vertical bounds of the font rather than that of the string.
   /// If 'roundPosition' is true, the overall position of the string will be
   /// rounded to an integer.
   void Draw(const char* string, const Box2F& alignmentArea,
      int alignment = ALIGN_CENTER_X | ALIGN_CENTER_Y,
      bool useMaxAscentDescent = false, const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f, bool roundPosition = false);
   /// Draw the given string in the 'alignmentArea' with the alignment as specified.
   /// If 'useMaxAscentDescent' is true, the vertical alignment will be based
   /// on the maximum vertical bounds of the font rather than that of the string.
   /// Clip text to the given 'clipArea'.  If 'roundPosition' is true, the
   /// overall position of the string will be rounded to an integer.
   void Draw(const char* string, const Box2F& alignmentArea, const Box2F& clipArea,
      int alignment = ALIGN_CENTER_X | ALIGN_CENTER_Y,
      bool useMaxAscentDescent = false, const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f, bool roundPosition = false);
   /// Draw the given string with the baseline starting at the given position.
   virtual void Draw(const char* string, const Point2F& position, 
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f) = 0;
   /// Draw the given string with the baseline starting at the given position.
   /// Clip text to the given 'clipArea'.
   virtual void Draw(const char* string, const Point2F& position,
      const Box2F& clipArea, const ColorRGBA8& color = COLOR_RGBA8_WHITE,
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f) = 0;

   /// Return the position of the given character in the given string.
   virtual Point2F CharacterPositionGet(const char* string, int characterIndex) = 0;
   /// Return the position at which a caret should be drawn in the given string
   /// when the caret is at the given character index.
   virtual Point2F CaretPositionGet(const char* string, int characterIndex) = 0;

   /// Return the position of the baseline at which the given string should be drawn
   /// if it is to be aligned in the given way within the given 'alignmentArea' at the
   /// specified scale.  If 'useMaxAscentDescent' is true, the vertical alignment will be based
   /// on the maximum vertical bounds of the font rather than that of the string.
   /// In general, if you want to align text, you should use the Draw methods
   /// rather than using this directly.
   Point2F AlignedPositionGet(const char* string, const Box2F& alignmentArea,
      int alignment = ALIGN_CENTER_X | ALIGN_CENTER_Y, bool useMaxAscentDescent = false,
      const Point2F& scale = Point2F::Create(1.0f, 1.0f));
   Point2F AlignedPositionGet(const char* string, int stringBytes, const Box2F& alignmentArea,
      int alignment = ALIGN_CENTER_X | ALIGN_CENTER_Y, bool useMaxAscentDescent = false,
      const Point2F& scale = Point2F::Create(1.0f, 1.0f));

   /// Return the number of pixels between consecutive baselines.
   float LineSpacingGet() { return lineSpacing; }
   /// Set the number of pixels between consecutive baselines.
   void LineSpacingSet(float _lineSpacing) { lineSpacing = _lineSpacing; }

   /// Return the maximum distance above the baseline for the top of a
   /// character in this font.
   float AscentMaxGet() { return ascentMax; }
   /// Return the maximum distance below the baseline for the bottom of a
   /// character in this font.
   float DescentMaxGet() { return descentMax; }

   /// Print the list of characters that are undefined that were recorded.
   /// Characters may not appear correctly in the debug output window, so check
   /// the log using a text editor that supports UTF-8.
   void UndefinedCharactersDebugPrint();

protected:
   /// Draw the given string but only as many bytes that are provided
   virtual void Draw(const char* string, int stringBytes, const Point2F& position, 
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f) = 0;
   /// Given the string, returns the number of btyes until the first ecnounted '/n'
   /// Returns -1 if none are found
   int BytesUntilNewLineGet(const char* string);
   /// Given the string, returns the number of new lines requested
   int NewLinesFound(const char* string, int stringBytes);
   /// Helper function for AlignedPositionGet(), 
   /// Given the bounds from a string, determine where to position it.
   Point2F AlignedPositionGetHelper(Box2F bounds, const Box2F& alignmentArea,
      int alignment, bool useMaxAscentDescent, const Point2F& scale, int lineCount);
   
   /// Record that the given character was requested, but not defined.
   void UndefinedCharacterRecord(uint32 characterCode)
   {
      undefinedCharacters.Add(characterCode, characterCode);
   }

   /// Number of pixels between consecutive baselines.
   float lineSpacing;
   /// Maximum distance above the baseline for the top of a
   /// character in this font.
   float ascentMax;
   /// Return the maximum distance below the baseline for the bottom of a
   /// character in this font.
   float descentMax;

   /// Set of characters that have been requested, but are defined.
   Map<uint32,uint32> undefinedCharacters;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FONT_H__
