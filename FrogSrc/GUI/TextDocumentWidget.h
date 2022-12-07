#ifndef __FROG__GUI__TEXTDOCUMENTWIDGET_H__
#define __FROG__GUI__TEXTDOCUMENTWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ScrollBoxWidget.h"
#include "Utility.h"

namespace Webfoot {

class PressButtonWidget;
class TextDocumentWidgetContentContainer;
class JSONValue;

//=============================================================================

#if defined _DEBUG && PLATFORM_IS_WINDOWS

/// Helper object for keeping track of which text files to watch for
/// changes.
class TextDocumentWidgetFileReloadHelper
{
public:
   TextDocumentWidgetFileReloadHelper()
   {
      filename = NULL;
      fileModificationTime = 0;
   }

   void Deinit()
   {
      StringDelete(filename);
   }

   /// File to watch for changes.
   const char* filename;
   /// File modification time from when the associated file was last loaded.
   int64 fileModificationTime;
};

#endif

//=============================================================================

/// A collection of widgets that make up a line of a text document
class TextDocumentWidgetLine
{
public:
   /// Initialize the class members
   TextDocumentWidgetLine()
   {
      width = 0.0f;
      defaultLineSpacing = 0.0f;
      alignment = ALIGN_LEFT;
   }

   void Init() { widgets.Init(); }
   void Deinit() { widgets.Deinit(); }
   
   /// the width of the line in pixels.
   float width;
   /// Default spacing for this line if no widget is placed on the line.
   float defaultLineSpacing;
   /// How content should be aligned horizontally within this line.
   int alignment;
   
   /// Collection of segments that make up the line.
   Table<Widget*> widgets;
};

//=============================================================================

/// Helper class with data for an individual page of a TextDocumentWidget.
class TextDocumentWidgetPage
{
public:
   /// Index of the first widget for this page in the associated
   /// TextDocumentWidget's 'dynamicContainer'.
   int widgetFirstIndex;
   /// Number of widgets in the page.
   int widgetCount;
};

//=============================================================================

/// Used to associate a name with a specific part of the content of a
/// TextDocumentWidget.
class TextDocumentWidgetTarget
{
public:
   TextDocumentWidgetTarget() { name = NULL; widget = NULL; }
   
   void Init(const char* _name) { name = StringClone(_name); }
   void Deinit() { StringDelete(name); widget = NULL; }

   /// Return the name of this target.
   const char* NameGet() { return name; }
   /// Return the widget associated with this target.
   Widget* WidgetGet() { return widget; }
   /// Set the widget associated with this target.
   void WidgetSet(Widget* _widget) { widget = _widget; }

protected:
   /// Name of the target.
   const char* name;
   /// Widget targeted by this object.
   Widget* widget;
};

//=============================================================================

/// Entry for a single pair of strings in which one should replace the other.
class TextDocumentWidgetTextAlias
{
public:
   TextDocumentWidgetTextAlias()
   {
      before = NULL;
      after = NULL;
   }

   void Deinit()
   {
      StringDelete(before);
      StringDelete(after);
   }

   /// Text to be replaced.
   const char* before;
   /// Replacement text.
   const char* after;
};

//=============================================================================

/// Widget that displays formatted text using markup for changing fonts and
/// embedding images, and the syntax is similar to HTML.  To use an alternate font,
/// use "<Font=name>text to draw in that font</Font>".  This can be
/// used multiple times, but multiple Font tags cannot be nested.  You can embed
/// sprite animations from the GUI layer's sprite file with <Sprite=animationName>.
/// Sprites will be displayed in-line with the sprite's origin on the baseline
/// of the neighboring text.  To change the scale of the text,
/// use "<TextScale=x|y>text to be drawn at a different scale</TextScale>".
/// This can be used multiple times, but multiple TextScale tags cannot be nested.
/// To change the color of the text, use "<TextColor=red|green|blue|alpha>text to be drawn in a different color</TextColor>".
/// This can be used multiple times, but multiple TextColor tags cannot be nested.
/// All tags are case-sensitive.
/// This widget is a special case of a ScrollBox.  The sprites and text are added
/// to the line to make it wrap around mostly within the clip area of the clip
/// container.  Newlines and spaces in the input text are followed.  The
/// bounds of the ClipContainer are determined dynamically.  Horizontal scrollbars
/// are not necessary.
/// Call Init immediately after construction.
class TextDocumentWidget : public ScrollBoxWidget
{
public:
   TextDocumentWidget();
   virtual ~TextDocumentWidget() {}

   /// Call this immediately after construction.
   /// Initialize a new TextDocumentWidget based on the specifications in the given JSONValue.
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);
   /// Clean up the widget
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Refresh the text for the widget and its children.
   /// Call this after a language change.
   virtual void TextRefresh();
   
   /// Return the text to be interpreted and displayed by this widget.
   virtual const char* TextGet() { return text;}
   /// Set the text to be interpreted and displayed by this widget.
   virtual void TextSet(const char* _text);

   /// Return the key for theText for the string to be displayed by this
   /// widget.  Return NULL if a text key is not in use.
   virtual const char* TextKeyGet() { return textKey; }
   /// Set the key for theText for the string to be displayed by this
   /// widget.  This will replace any existing text,
   /// including when called with NULL.  This method will call
   /// TextRefresh() internally if appropriate.
   virtual void TextKeySet(const char* _textKey);

   /// Return the Lua expression used to determine the text
   /// to be displayed for this widget.
   /// Return NULL if a text expresion is not in use.
   virtual const char* TextExpressionGet() { return textExpression;}
   /// Set the Lua expression used to determine the text
   /// to be displayed for this widget.  This will replace any existing text,
   /// including when called with NULL.  This method will call TextRefresh()
   /// internally if appropriate.
   virtual void TextExpressionSet(const char* _textExpression);

   /// Return the name of the file to be displayed by this
   /// widget.  The filename should be relative to the "Text/[Language]"
   /// folder, and it should not include an extension.  Return NULL if the text
   /// is not coming from a file.
   virtual const char* TextFilenameGet() { return textFilename; }
   /// Set the name of the file to display.  The filename should be relative to
   /// the "Text/[Language]" folder, and it should not include an extension.
   /// This will replace any existing text, including when called with NULL.
   /// This method will call TextRefresh() internally if appropriate.
   virtual void TextFilenameSet(const char* _textFilename);
   /// Return true if the given text file exists.  The filename should be
   /// relative to the "Text/[Language]" folder, and it should not include an
   /// extension.
   virtual bool TextFileExistsCheck(const char* _textFilename);

   /// Return the dimensions of the dynamic content in pixels.
   Point2F ContentSizeGet() { return contentSize; }

   /// Sets a new text area for clipping and refreshes the text
   virtual void TextAreaSet(const Box2F& _textArea);
   /// Return the current text alignment.
   virtual int TextAlignmentGet() { return textAlignment; }
   /// Set the text alignment.  This will not change the alignment
   virtual void TextAlignmentSet(int _textAlignment) { textAlignment = _textAlignment; TextProcess(); }

   /// Return the scale at which the text will be drawn, excluding the effects of TextScaleCoefficient.
   virtual Point2F TextScaleGet() { return textScale; }
   /// Set the scale at which the text will be drawn, excluding the effects of TextScaleCoefficient.
   virtual void TextScaleSet(const Point2F& _textScale) { textScale = _textScale; TextProcess(); }

   /// Return the coefficient that will be applied to TextScale to determine
   /// the effective text scale.
   virtual Point2F TextScaleCoefficientGet() { return textScaleCoefficient; }
   /// Set the coefficient that will be applied to TextScale to determine the
   /// effective text scale.
   virtual void TextScaleCoefficientSet(const Point2F& _textScaleCoefficient) { textScaleCoefficient = _textScaleCoefficient; TextProcess(); }

   /// Return the color to apply to the text when drawing.
   virtual ColorRGBA8 TextColorGet() { return textColor; }
   /// Set the color to apply to the text when drawing.
   virtual void TextColorSet(const ColorRGBA8& _textColor) { textColor = _textColor; TextProcess(); }

   /// Scroll to the given target in the current content.
   virtual void ScrollToTarget(const char* targetName);

   /// Set the JSON array of flags to use for conditional markup.
   virtual void ConditionalFlagsSet(JSONValue* _conditionalFlags);
   /// Add a conditional flag.
   virtual void ConditionalFlagAdd(const char* flagName);

   /// Set whether text should be broken up into pages rather than a single
   /// scrollable section.
   virtual void PagesEnabledSet(bool _pagesEnabled);
   /// Return true if text should be broken up into pages rather than a single
   /// scrollable section.
   virtual bool PagesEnabledCheck() { return pagesEnabled; }
   /// Assuming pages are enabled, go to the given page.
   virtual void PageIndexSet(int _pageIndex);
   /// Assuming pages are enabled, return the index of the current page.
   virtual int PageIndexGet() { return pageIndex; }
   /// Assuming pages are enabled, return the number of pages.
   virtual int PageCountGet() { return pages.SizeGet(); }

   /// Begin revealing content with a teletype effect.
   virtual void TeletypeEffectBegin();
   /// Finish the teletype effect now, revealing all content.
   virtual void TeletypeEffectFinish();
   /// Return true if a teletype effect is active on this widget.
   virtual bool TeletypeEffectActiveCheck() { return teletypeEffectActive; }
   /// Set the rate at which characters are revealed in characters per second.
   virtual void TeletypeEffectSpeedSet(float _teletypeEffectSpeed) { teletypeEffectSpeed = _teletypeEffectSpeed; }
   /// Return the rate at which characters are revealed in characters per second.
   virtual float TeletypeEffectSpeedGet() { return teletypeEffectSpeed; }
   /// Return the current position of the teletype effect relative to this
   /// widget.  If there is no effect active, return (0,0).
   Point2F TeletypeEffectPositionOffsetGet();

   /// Type of the function that can receive the OnPressButtonClick event.
   /// The TextDocumentWidget* is the document containing the button that was
   /// clicked, the PressButtonWidget* is the button that was clicked, and the
   /// void* is the userData pointer.
   typedef void (*OnPressButtonClickCallback)(TextDocumentWidget*, PressButtonWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnPressButtonClick event occurs.
   /// For now, only one function can be registered per instance.
   void OnPressButtonClickRegister(OnPressButtonClickCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnPressButtonClick event occurs.
   void OnPressButtonClickUnregister(OnPressButtonClickCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnPressButtonClickRegister(const char* path, OnPressButtonClickCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) TextDocumentWidget(); }

protected:
   typedef ScrollBoxWidget Inherited;

   /// Used to display the given text.
   void TextSetHelper(const char* _text);

   /// Clear the widgets currently used to display the processed text, and
   /// create new objects based on the current text.
   void TextProcess();

   /// Helper function to TextSet that starts a new line.  A pointer
   /// the the new line is returned.
   TextDocumentWidgetLine* TextSet_NewLine(float* lineRoomLeft, bool* lineNew,
      bool* lineFull, Font* currentFont, const Point2F& currentTextScale);
   /// Return true if the string contains no non-whitespace characters.
   /// If markup is present, it will return false.
   bool StringBlankCheck(const char* theString);
   /// Remove and clean up all the dynamic widgets.
   void DynamicWidgetsClear();
   /// Set the y positions of all the lines.
   void LinePositionsPrepare();
   /// Return true if the given character is whitespace.
   bool WhiteSpaceCheck(char theChar);
   /// Return the bounds of the given string according to the needs of this class.
   Box2F StringOpaqueBoundsGet(const char* string, Font* font, const Point2F& scale);
   /// Bind any unbound targets to the given Widget.
   void TargetBindHelper(Widget* widget);
   /// Helper function for determining whether the next segment should be included.
   bool IfTrueHelper(Table<uint32>* ifTrueStack);
   /// Return the object for the given page.
   TextDocumentWidgetPage* PageGet(int _pageIndex);
   /// Apply the text aliases to the given string, possibly deallocating and
   /// replacing it.
   void TextAliasesApply(char*& _text);

   /// Return the alignment to use based on the given alignment name.
   int TextAlignmentGetByName(const char* alignmentName);
   /// Return the default alignment for the text.
   virtual int TextAlignmentDefaultGet();

   /// Return true if TextRefresh() is allowed to use the TextKey.
   virtual bool TextKeyUseCheck() { return true; }
   /// Return true if TextRefresh() is allowed to use the TextExpression.
   virtual bool TextExpressionUseCheck() { return true; }
   /// Return true if TextRefresh() is allowed to use the TextFilename.
   virtual bool TextFilenameUseCheck() { return true; }

   /// Callback for when a press button in the text is clicked.
   static void OnPressButtonClick(PressButtonWidget* pressButtonWidget, void* userData);
   /// Comparator for sorting text aliases into the order in which they are
   /// applied.
   static bool TextAliasComparator(const TextDocumentWidgetTextAlias* a, const TextDocumentWidgetTextAlias* b);

   /// Holds all the dynamically generated child widgets.
   TextDocumentWidgetContentContainer* dynamicContainer;

   /// Flags to use for alignment of the text.
   int textAlignment;
   /// The markup to interpret and display.
   char* text;
   /// If defined, this will be passed to theText to get the string to
   /// be interpreted and displayed.
   char* textKey;
   /// If defined, the result of evaluating this Lua expression
   /// will be interpreted and displayed.
   char* textExpression;
   /// If defined, this file will be displayed by this widget..
   char* textFilename;
   /// Size of the dynamic content in pixels.
   Point2F contentSize;
   /// How the text should be scaled when drawing.
   Point2F textScale;
   /// Coefficient that will be applied to TextScale to determine the
   /// effective text scale.
   Point2F textScaleCoefficient;
   /// Color to be applied to the text when drawing.
   ColorRGBA8 textColor;
   /// True if text should be broken up into pages rather than a single
   /// scrollable section.
   bool pagesEnabled;
   /// Index of the current page, assuming 'pagesEnabled' is true.
   int pageIndex;
   /// True if a teletype effect is active on this widget.
   bool teletypeEffectActive;
   /// Rate at which characters are revealed in characters per second.
   float teletypeEffectSpeed;
   /// Counter for the number of characters to reveal for the teletype effect.
   float teletypeEffectCharactersToReveal;
   /// Index of the current widget modified by the teletype effect.
   int teletypeEffectPositionWidgetIndex;
   /// The current widget for the teletype effect is limited to this many
   /// characters.
   int teletypeEffectPositionWidgetCharacterCountLimit;
   /// Sound to play in a loop while a teletype effect is active.
   Sound* teletypeEffectSound;
   /// Used to start and stop the teletype effect sound.
   SoundSource* teletypeEffectSoundSource;
   /// Function to be called for the OnPressButtonClick event.
   OnPressButtonClickCallback onPressButtonClickCallback;
   /// Pointer to be passed to the OnPressButtonClick callback.
   void* onPressButtonClickUserData;

   /// Used to keep track of which dynamic widgets belong to different lines.
   Table<TextDocumentWidgetLine*> lines;
   /// Keeps track of details of individual pages.
   Table<TextDocumentWidgetPage*> pages;
   /// Used to associate names with specific parts of the content of a
   /// TextDocumentWidget.
   Table<TextDocumentWidgetTarget*> targets;
   /// Strings to be replaced.
   Table<TextDocumentWidgetTextAlias*> textAliases;
   /// JSON array of flags to use for conditional markup.
   JSONValue* conditionalFlags;

   #if defined _DEBUG && PLATFORM_IS_WINDOWS
      /// Called when the program gains the focus of the OS.
      void OnOSFocusGain();
      /// File modification time from when the main text file was last loaded.
      int64 textFileModificationTime;
      /// List of files to watch for changes that were included with markup.
      Table<TextDocumentWidgetFileReloadHelper*> includedTextFileModificationTimes;
   #endif //#if _DEBUG && PLATFORM_IS_WINDOWS

   friend class TextDocumentWidgetContentContainer;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__TEXTDOCUMENTWIDGET_H__
