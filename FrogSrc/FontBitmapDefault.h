#ifndef __FROG__FONTBITMAPDEFAULT_H__
#define __FROG__FONTBITMAPDEFAULT_H__

#include "FrogMemory.h"
#include "Font.h"
#include "Image.h"
#include "Map.h"
#include "List.h"

namespace Webfoot {

//==============================================================================

/// FontBitmapDefault is an implementation of Font that uses Images, as opposed
/// to vectors, to represent the font.
class FontBitmapDefault : public Font
{
public:   
   typedef Font Inherited;
   
   virtual ~FontBitmapDefault() {}
   
   void Init(HeapID _heapID = HEAP_DEFAULT);
   virtual void Deinit();
   
   /// Return the bounds of visible part of the given string.
   /// If 'includeWhitespaceCharacters' is true, treat whitespace characters as
   /// opaque, 1-pixel-tall boxes on the baseline.
   virtual Box2F OpaqueBoundsGet(const char* string, bool includeWhitespaceCharacters = false);
   virtual Box2F OpaqueBoundsGet(const char* string, int stringBytes, bool includeWhitespaceCharacters = false);

   /// Draw the given string with the baseline starting at the given position.
   virtual void Draw(const char* string, const Point2F& position, 
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f);
   /// Draw the given string with the baseline starting at the given position.
   /// Clip text to the given 'clipArea'.
   virtual void Draw(const char* string, const Point2F& position,
      const Box2F& clipArea, const ColorRGBA8& color = COLOR_RGBA8_WHITE,
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f);

   /// Return the position of the given character in the given string.
   virtual Point2F CharacterPositionGet(const char* string, int characterIndex);
   /// Return the position at which a caret should be drawn in the given string
   /// when the caret is at the given character index.
   virtual Point2F CaretPositionGet(const char* string, int characterIndex);
   
   /// Data associated for a single character.
   struct CharacterData
   {
      /// Image that contains the character
      Image* image;
      /// Bounds of the character within the associated image
      Box2F bounds;
      /// Number of pixels from the top of the bounds to the baseline
      float ascent;
   };
   
   /// Add this image to the collection of images to be unloaded when this font
   /// is deinitialized.  Return true if the image was not already in the
   /// collection.
   bool ImageAdd(Image* image);
   /// Add the given character to the font.  Return the number of bytes in the
   /// character if successful.  Return -1 if unsuccessful.
   int CharacterAdd(const char* character, CharacterData* characterData);

   /// Return the offset between characters in pixels.
   float CharacterSpacingGet() { return characterSpacing; }
   /// Set the offset between characters in pixels.
   void CharacterSpacingSet(float _characterSpacing) { characterSpacing = _characterSpacing; }

   /// Return the width of a blank space in pixels.
   float BlankSpaceWidthGet() { return blankSpaceWidth; }
   /// Set the width of a blank space in pixels.
   void BlankSpaceWidthSet(float _blankSpaceWidth) { blankSpaceWidth = _blankSpaceWidth; }

   /// Return the width of a tab in pixels.
   float TabWidthGet() { return tabWidth; }
   /// Set the width of a tab in pixels.
   void TabWidthSet(float _tabWidth) { tabWidth = _tabWidth; }

   /// Add mappings so that each 'source' character that is encountered is automatically
   /// replaced with the corresponding character from the 'dest' string.
   /// Return true if successful.
   bool CharacterReplacementsAdd(const char* dest, const char* source);
   
protected:
   /// Draw the given string but only as many bytes that are provided.
   virtual void Draw(const char* string, int stringBytes, const Point2F& position, 
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, 
      const Point2F& scale = Point2F::Create(1.0f, 1.0f), float depth = 0.0f,
      float additiveBlending = 0.0f);

   /// Helper function to handle the drawing of text.
   void DrawHelper(const char* string, int stringBytes, const Point2F& _position,
      const Box2F* _clipArea, const ColorRGBA8& color, const Point2F& scale, float depth, float additiveBlending);

   /// Draw the given part of the given image.
   virtual void CharacterDraw(Image* image, const Box2F& imageSubset, const Point2F& position,
      const ColorRGBA8& color, float depth, float additiveBlending);
   /// Finish any buffered drawing.
   virtual void DrawFlush(const ColorRGBA8& color, float additiveBlending) { (void)color; (void)additiveBlending; }

   /// Helper function for getting the position of characters and carets.
   Point2F CharacterPositionGetHelper(const char* string, int characterIndex, bool forCaret);

   typedef List<Image*> ImageList;
   typedef Map<uint32, CharacterData> CharacterMap;
   typedef Map<uint32, uint32> CharacterReplacementMap;

   /// Heap from which long-term memory is drawn.
   HeapID heapID;
   /// Offset between characters.
   float characterSpacing;
   /// Width of a blank space in pixels
   float blankSpaceWidth;
   /// Width of a tab in pixels
   float tabWidth;
   /// Collection of data for the characters
   CharacterMap characters;
   /// Collection of images to be unloaded when this font is deinitialized.
   ImageList images;
   /// Mapping of unavailable characters to available characters.
   CharacterReplacementMap characterReplacements;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FONTBITMAPDEFAULT_H__
