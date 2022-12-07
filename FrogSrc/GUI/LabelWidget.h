#ifndef __FROG__GUI__LABELWIDGET_H__
#define __FROG__GUI__LABELWIDGET_H__

#include "FrogMemory.h"
#include "Color.h"
#include "GUI/Widget.h"

namespace Webfoot {

class LayerWidget;

/// Key for the literal text to be displayed for the widget
#define LABEL_WIDGET_TEXT_KEY "Text"
/// Key for the key for theText that determines the text.
#define LABEL_WIDGET_TEXT_KEY_KEY "TextKey"
/// Key for the Lua expression that determines the text.
#define LABEL_WIDGET_TEXT_EXPRESSION_KEY "TextExpression"

//=============================================================================

/// Widget that displays a string in a single font aligned within the text bounds.
/// Call Init immediately after construction.  If using the bouncing text
/// effect, always call TextRefresh after changing the font.
class LabelWidget : public Widget
{
public:
   LabelWidget();
   virtual ~LabelWidget() {}

   /// Initialize a new widget with the defaults.
   /// If no name is given, it will default to the widget type name.
   /// Call this or the other Init immediately after construction.
   virtual void Init(const char* _name, Widget* _parent);
   /// Call this or the other Init immediately after construction.
   /// Initialize a new LabelWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Draw the widget.
   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Reload the text for the widget.
   /// Call this after a language change.
   virtual void TextRefresh();

   /// Return the text to be displayed by this widget.
   virtual const char* TextGet() { return text; }
   /// Set the text to be displayed by this widget.
   virtual void TextSet(const char* _text);

   /// Return the key for theText for the string to be displayed by this
   /// widget.  Return NULL if a text key is not in use.
   virtual const char* TextKeyGet() { return textKey; }
   /// Set the key for theText for the string to be displayed by this
   /// widget.  This will replace any existing text, including when called with
   /// NULL.  This method will call TextRefresh() internally if appropriate.
   virtual void TextKeySet(const char* _textKey);

   /// Return the Lua expression used to determine the text
   /// to be displayed for this widget.
   /// Return NULL if a text expresion is not in use.
   virtual const char* TextExpressionGet() { return textExpression; }
   /// Set the Lua expression used to determine the text
   /// to be displayed for this widget.  This will replace any existing text,
   /// including when called with NULL.  This method will call TextRefresh()
   /// internally if appropriate.
   virtual void TextExpressionSet(const char* _textExpression);

   /// Set the box, relative to the widget's position, within which the
   /// text should be aligned and clipped.
   virtual void TextBoundsSet(Box2F _textBounds);
   /// Return the box, relative to the widget's position, within which the
   /// text should be aligned and clipped.  If the text bounds were not
   /// defined, use the widget bounds.
   virtual Box2F TextBoundsGet();
   /// Use the bounds of the widget rather than any previous TextBounds.
   virtual void UseWidgetBoundsForText() { useWidgetBoundsForText = true; }

   /// Set the area in which this widget can be clicked relative to the widget's origin.
   virtual void BoundsSet(Box2F _bounds);

   /// Return the current text alignment.
   virtual int TextAlignmentGet() { return textAlignment; }
   /// Set the text alignment.
   virtual void TextAlignmentSet(int _textAlignment) { textAlignment = _textAlignment; }

   /// Return true if the text should bounce horizontally if it's wider
   /// than the text bounds.
   virtual bool TextBounceXCheck() { return textBounceX; }
   /// Set whether the text should bounce horizontally if it's wider
   /// than the text bounds.
   virtual void TextBounceXSet(bool _textBounceX);

   /// Return the speed at which horizontally bouncing text scrolls in pixels per second.
   virtual float TextBounceXSpeedGet() { return textBounceXSpeed; }
   /// Set the speed at which horizontally bouncing text scrolls in pixels per second.
   virtual void TextBounceXSpeedSet(float _textBounceXSpeed);

   /// Return the length of time, in milliseconds, that the text should wait
   /// at one side when bouncing horizontally.
   virtual int TextBounceXPauseDurationGet() { return textBounceXPauseDuration; }
   /// Set the length of time, in milliseconds, that the text should wait
   /// at one side when bouncing horizontally.
   virtual void TextBounceXPauseDurationSet(int _textBounceXPauseDuration);

   /// Return the scale at which the text will be drawn.
   virtual Point2F TextScaleGet() { return textScale; }
   /// Set the scale at which the text will be drawn.
   virtual void TextScaleSet(const Point2F& _textScale);
   /// Return the text scale to actually use when drawing text.
   virtual Point2F TextScaleDisplayGet();
   /// Return true if the text should be scaled down to fit within the text 
   /// bounds.
   virtual bool TextScaleDownToFitCheck() { return textScaleDownToFit; }
   /// Set whether the text should be scaled down to fit within the text
   /// bounds.
   virtual void TextScaleDownToFitSet(bool _textScaleDownToFit) { textScaleDownToFit = _textScaleDownToFit; }

   /// Return the color to apply to the text when drawing.
   virtual ColorRGBA8 TextColorGet() { return textColor; }
   /// Set the color to apply to the text.  The actual color used for
   /// drawing is this value modulated with the result of ColorGet().
   virtual void TextColorSet(const ColorRGBA8& _textColor) { textColor = _textColor; }

   /// Return true if the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   virtual bool TextAscentDescentMaxUseCheck() { return textAscentDescentMaxUse; }
   /// Set whether the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   virtual void TextAscentDescentMaxUseSet(bool _textAscentDescentMaxUse) { textAscentDescentMaxUse = _textAscentDescentMaxUse; }

   /// In debug builds, return true if this widget should draw its text bounds.
   virtual bool TextBoundsDrawCheck() { return textBoundsDraw; }
   /// In debug builds, set whether this widget should draw its text bounds.
   virtual void TextBoundsDrawSet(bool _textBoundsDraw) { textBoundsDraw = _textBoundsDraw; }

   /// Return the color to use when drawing the text bounds of the Widget in debug builds.
   virtual ColorRGBA8 TextBoundsColorGet() { return textBoundsColor; }
   /// Set the color to use when drawing the text bounds of the Widget in debug builds.
   virtual void TextBoundsColorSet(const ColorRGBA8& _textBoundsColor) { textBoundsColor = _textBoundsColor; }

   /// Set a limit on the number of characters to draw, including whitespace.
   /// This is not necessarily the same as the number of bytes.
   virtual void CharacterCountLimitSet(int _characterCountLimit) { characterCountLimit = _characterCountLimit; useCharacterCountLimit = true; }
   /// Stop using a character count limit for this widget.
   virtual void CharacterCountLimitClear() { useCharacterCountLimit = false; }
   /// Return the limit on the number of characters to draw, including
   /// whitespace.  This is not necessarily the same as the number of bytes.
   /// If there currently is no limit, return -1.
   virtual int CharacterCountLimitGet();
   /// Return the relative position of the given character in the displayed text.
   Point2F CharacterPositionGet(int characterIndex);
   /// Return the position at which a caret should be drawn in the text when
   /// the caret is at the given character index.
   Point2F CaretPositionGet(int characterIndex);

   /// Set the range of characters (not necessarily bytes) for which the text
   /// should be displayed as selected.
   virtual void TextSelectionSet(int _textSelectionCharacterIndexBegin, int _textSelectionCharacterIndexBeginEnd);
   /// Stop showing text as selected.
   virtual void TextSelectionClear();
   /// Set the color to be used to draw selected text.
   virtual void TextSelectedColorSet(const ColorRGBA8& _textSelectedColor) { textSelectedColor = _textSelectedColor; useTextColorForSelectedText = false; }
   /// Use the regular text color when drawing selected text.
   virtual void UseTextColorForSelectedText() { useTextColorForSelectedText = true; }
   /// Return the color to be used to draw selected text.
   virtual ColorRGBA8 TextSelectedColorGet();
   /// Set the color of the rectangle to be drawn behind selected text.
   virtual void TextSelectedBackgroundColorSet(const ColorRGBA8& _textSelectedBackgroundColor) { textSelectedBackgroundColor = _textSelectedBackgroundColor; }
   /// Return the color of the rectangle to be drawn behind selected text.
   virtual ColorRGBA8 TextSelectedBackgroundColorGet() { return textSelectedBackgroundColor; }

   /// Assuming the text consists of a number, return it as a double.
   virtual double DoubleValueGet();

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) LabelWidget(); }

protected:
   typedef Widget Inherited;

   /// Used to display the given text.
   void TextSetHelper(const char* _text);

   /// Return true if TextRefresh() is allowed to use the TextKey.
   virtual bool TextKeyUseCheck() { return true; }
   /// Return true if TextRefresh() is allowed to use the TextExpression.
   virtual bool TextExpressionUseCheck() { return true; }

   /// Pre-compute relevant variables for making the text bounce correctly.
   virtual void BounceCompute();

   /// Return the alignment to use based on the given alignment name.
   int TextAlignmentGetByName(const char* alignmentName);

   /// Return the default alignment for the text.
   virtual int TextAlignmentDefaultGet();

   /// Return the text as it should be displayed, not necessarily
   /// how it is stored.
   virtual const char* TextDisplayGet() { return TextGet(); }
   /// Return the alignment with which to draw the text right now,
   /// which is not necessarily what's been requested.
   virtual int TextAlignmentDisplayGet() { return TextAlignmentGet(); }
   /// Offset applied to the text when displaying.
   virtual Point2F TextOffsetDisplayGet() { return Point2F::Create(0,0); }
   /// Return true if text should be drawn as bouncing right now.
   virtual bool TextBounceXDisplayCheck() { return TextBounceXCheck(); }
   /// Return the opaque bounds of the string for purposes of automatic
   /// scaling.
   virtual Box2F TextOpaqueBoundsForScalingGet();
   /// Helper function for getting the positions of characters and carets.
   Point2F CharacterPositionGetHelper(int characterIndex, bool forCaret);

   /// True if a warning has already been given about this widget's font being NULL.
   bool nullFontWarningGiven;
   /// True if the widget's bounds should be used as the text bounds.
   bool useWidgetBoundsForText;
   /// True if the widget's text should bounce horizontally when
   /// it's wider than the text bounds.
   bool textBounceX;
   /// True if the current text is wide enough and it should bounce.
   bool animateTextBounceX;
   /// True if the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   bool textAscentDescentMaxUse;
   /// The text to actually display.
   char* text;
   /// If defined, this will be passed to theText to get the string to display.
   char* textKey;
   /// If defined, the result of evaluating this Lua expression
   /// will be used as this widget's text.
   char* textExpression;
   /// Combination of flags to use for alignment of the text.
   int textAlignment;
   /// The speed at which horizontally bouncing text scrolls in pixels per millisecond.
   float textBounceXSpeed;
   /// Length of time, in milliseconds, that the text should wait
   /// at one side when bouncing horizontally.
   int textBounceXPauseDuration;
   /// Distance the text must scroll when bouncing horizontally.
   int textBounceXDistance;
   /// Length of time it takes for text to horizontally scroll from one end to the other.
   int textBounceXScrollDuration;
   /// Number of milliseconds in a horizontal text bounce loop before it is all the way to the left.
   int textBounceXLeftScrollEndTime;
   /// Number of milliseconds in a horizontal text bounce loop before the first pause ends.
   int textBounceXLeftPauseEndTime;
   /// Length of the hoizontal text bounce loop in milliseconds.
   int textBounceXLoopDuration;
   /// Current position in the horizontal text bounce loop.
   int textBounceXLoopPosition;
   /// The offset from the left side of the text bounds at which to draw the text when
   /// horizontally scrolling.
   int textBounceXOffset;
   /// Area relative to the widget's position, within which the
   /// text should be aligned and clipped.
   Box2F textBounds;
   /// How the text should be scaled when drawing.
   Point2F textScale;
   /// True if text should be scaled down automatically so that it fits within
   /// the TextBounds.
   bool textScaleDownToFit;
   /// Color to be applied to the text when drawing.
   ColorRGBA8 textColor;
   /// Limit on the number of characters do draw, including whitespace.
   /// This is not necessarily the same as the number of bytes.  This is only
   /// followed if 'useCharacterCountLimit' is true.
   int characterCountLimit;
   /// True if 'characterCountLimit' should be enforced.
   bool useCharacterCountLimit;
   /// Index of the first character (but not necessarily the byte) for which
   /// text should be displayed as selected.  If this is the same value as
   /// 'textSelectionCharacterIndexEnd', no text should be displayed as
   /// selected.
   int textSelectionCharacterIndexBegin;
   /// Index of the last character (exclusive) (but not necessarily the byte) at
   /// which text should be displayed as selected.  If this is the same value as
   /// 'textSelectionCharacterIndexBegin', no text should be displayed as
   /// selected.
   int textSelectionCharacterIndexEnd;
   /// True if the regular text color should be used when drawing selected
   /// text.
   bool useTextColorForSelectedText;
   /// Color to be used to draw selected text.
   ColorRGBA8 textSelectedColor;
   /// Color of the rectangle to be drawn behind selected text.
   ColorRGBA8 textSelectedBackgroundColor;

   /// True if the widget should draw its text bounds.
   bool textBoundsDraw;
   /// Color used to draw the text bounds.
   ColorRGBA8 textBoundsColor;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__LABELWIDGET_H__
