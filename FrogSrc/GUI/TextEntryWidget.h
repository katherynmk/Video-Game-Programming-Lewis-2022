#ifndef __FROG__GUI__TEXTENTRYWIDGET_H__
#define __FROG__GUI__TEXTENTRYWIDGET_H__

#include "FrogMemory.h"
#include "GUI/LabelWidget.h"

namespace Webfoot {

class LayerWidget;
class Keyboard;

//=============================================================================

/// Widget that accepts text from the keyboard and clipboard.  It displays the
/// single-line string in a single font aligned within the text bounds.
/// While this does extend LabelWidget, text expressions are not supported.
/// Call Init immediately after construction.  This uses the bounce effect
/// internally, so always call TextRefresh after changing the font.
/// The TextKey and TextExpression are ignored for this label.
class TextEntryWidget : public LabelWidget
{
public:
   TextEntryWidget();
   virtual ~TextEntryWidget() {}
   
   /// Call this or the other Init immediately after construction.
   /// Initialize a new LabelWidget based on the specifications in the given JSONValue
   virtual void Init(const char* _name, Widget* _parent, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   virtual void Draw();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Set the text to be displayed by this widget.
   virtual void TextSet(const char* _text);

   /// Not supported by this widget.
   virtual const char* TextKeyGet() { return NULL;}
   /// Not supported by this widget.
   virtual void TextKeySet(const char* _textKey) { (void)_textKey; }

   /// Not supported by this widget.
   virtual const char* TextExpressionGet() { return NULL;}
   /// Not supported by this widget.
   virtual void TextExpressionSet(const char* _textExpression) { (void)_textExpression; }

   /// Return the maximum number of characters that can be typed.
   virtual int TextLengthMaxGet() { return maxTextLength; }
   /// Set the maximum number of characters that can be typed.
   virtual void TextLengthMaxSet(int _maxTextLength);

   /// Set whether this is for entering a password.
   virtual void PasswordSet(bool _password);
   /// Return true if this is for entering a password.
   virtual bool PasswordCheck() { return password; }

   /// Set whether there should be any restrictions on which characters can be
   /// entered.  Setting this to false does not currently remove any newly
   /// disallowed characters from the current text.  Calling this will clear
   /// the allowed characters list from 'CharactersAllowedSet'.
   virtual void AllCharactersAllowedSet(bool _allCharactersAllowed);
   /// Return true if there are no restrictions on which characters can be entered.
   virtual bool AllCharactersAllowedCheck() { return allCharactersAllowed; }
   /// Provide a whitelist of characters that can be entered.  This will
   /// automatically set 'allCharactersAllowed' to false.  This does not
   /// currently remove any newly disallowed characters from the current text.
   /// Calling this with NULL restores the default list of allowed characters.
   virtual void CharactersAllowedSet(const char* _charactersAllowed);

   /// Insert the given text at the current editing location.  If
   /// 'lineEndingFound' is specified, it will be set to true if a '\n' or
   /// '\r' character is encountered.  If 'tabFound' is specified, it will be
   /// set to true if a '\t' is encountered.
   void TextInsert(const char* textToInsert, bool* lineEndingFound = NULL, bool* tabFound = NULL);
   /// Delete the character in front of the caret.
   void TextDeleteForward();

   /// Return true if this widget should display as though it has focus,
   /// regardless of whether it actually does.
   virtual bool HasAnyFocusDisplayCheck();

   /// Set whether this widget should accept keyboard input as long as it is
   /// enabled, regardless of whether it has focus.
   virtual void AcceptInputWithoutFocusSet(bool _acceptInputWithoutFocus) { acceptInputWithoutFocus = _acceptInputWithoutFocus; }
   /// Return true if this widget should accept keyboard input as long as it is
   /// enabled, regardless of whether it has focus.
   virtual bool AcceptInputWithoutFocusCheck() { return acceptInputWithoutFocus; }

   /// Called when the left mouse button is pressed with the cursor over the widget.
   virtual void OnPress();
   /// Called when the enter key is pressed while the Widget has focus.
   virtual void OnEnterPress();
   /// Called when the tab key is pressed while the Widget has focus.
   virtual void OnTabPress();
   /// Called when the text is changed.
   virtual void OnTextChange();
   /// Called when the widgets gains focus.
   virtual void OnFocusGain(int focusIndex, bool keypadDirectionMovedFocus);
   /// Called when the widgets loses focus.
   virtual void OnFocusLose(int focusIndex, bool keypadDirectionMovedFocus);

   /// Type of the function that can receive the OnEnter event.
   /// The TextEntryWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnEnterPressCallback)(TextEntryWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnEnterPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnEnterPressRegister(OnEnterPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnEnterPress event occurs.
   void OnEnterPressUnregister(OnEnterPressCallback callback);
   /// Convenience function for finding a TextEntryWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnEnterPressRegister(const char* path, OnEnterPressCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnTab event.
   /// The TextEntryWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnTabPressCallback)(TextEntryWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnTabPress event occurs.
   /// For now, only one function can be registered per instance.
   void OnTabPressRegister(OnTabPressCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnTabPress event occurs.
   void OnTabPressUnregister(OnTabPressCallback callback);
   /// Convenience function for finding a TextEntryWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnTabPressRegister(const char* path, OnTabPressCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnTextChange event.
   /// The TextEntryWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnTextChangeCallback)(TextEntryWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnTextChange event occurs.
   /// For now, only one function can be registered per instance.
   void OnTextChangeRegister(OnTextChangeCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnTextChange event occurs.
   void OnTextChangeUnregister(OnTextChangeCallback callback);
   /// Convenience function for finding a TextEntryWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnTextChangeRegister(const char* path, OnTextChangeCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnFocusGain event.
   /// The TextEntryWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnFocusGainCallback)(TextEntryWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnFocusGain event occurs.
   /// For now, only one function can be registered per instance.
   void OnFocusGainRegister(OnFocusGainCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnFocusGain event occurs.
   void OnFocusGainUnregister(OnFocusGainCallback callback);
   /// Convenience function for finding a TextEntryWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnFocusGainRegister(const char* path, OnFocusGainCallback callback, void* userData = NULL);

   /// Type of the function that can receive the OnFocusLose event.
   /// The TextEntryWidget* is the source of the event and the void* is the
   /// userData context pointer.
   typedef void (*OnFocusLoseCallback)(TextEntryWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnFocusLose event occurs.
   /// For now, only one function can be registered per instance.
   void OnFocusLoseRegister(OnFocusLoseCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnFocusLose event occurs.
   void OnFocusLoseUnregister(OnFocusLoseCallback callback);
   /// Convenience function for finding a TextEntryWidget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnFocusLoseRegister(const char* path, OnFocusLoseCallback callback, void* userData = NULL);

   /// Factory method for making objects of this type.
   static Widget* Create(const char* type, HeapID heapID) { (void)type; return frog_new_ex(heapID) TextEntryWidget(); }

protected:
   typedef LabelWidget Inherited;

   /// Return true if TextRefresh() is allowed to use the TextKey.
   virtual bool TextKeyUseCheck() { return false; }
   /// Return true if TextRefresh() is allowed to use the TextExpression.
   virtual bool TextExpressionUseCheck() { return false; }

   /// Return true if this widget should default to accepting input focus when
   /// pressed by a mouse.
   virtual bool FocusOnMousePressDefaultCheck() { return true; }
   /// Return true if this widget should default to being able to get
   /// input focus.
   virtual bool CanHaveFocusDefaultCheck() { return true; }

   /// Determine what text should be displayed.
   virtual void TextDisplayUpdate(unsigned int dt = 0);

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }
   /// Return true if this widget type should play a mouseOver sound by default.
   virtual bool MouseOverSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play a press sound by default.
   virtual bool PressSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play an invalidPress sound by default.
   virtual bool InvalidPressSoundEnabledDefaultCheck() { return true; }
   /// Return true if this widget type should play a click sound by default.
   virtual bool ClickSoundEnabledDefaultCheck() { return true; }

   /// Return the text as it should be displayed, not necessarily
   /// how it is stored.
   virtual const char* TextDisplayGet() { return displayText; }
   /// Return the alignment with which to draw the text right now,
   /// which is not necessarily what's been requested.
   virtual int TextAlignmentDisplayGet();
   /// Offset applied to the text when displaying.
   virtual Point2F TextOffsetDisplayGet();
   /// Return true if text should be drawn as bouncing right now.
   virtual bool TextBounceXDisplayCheck();
   /// Return the opaque bounds of the string for purposes of automatic
   /// scaling.
   virtual Box2F TextOpaqueBoundsForScalingGet();

   /// Return true if the given UTF-8 character is safe to accept as input.
   /// For now, accept only spaces and single-byte alphanumeric characters.
   virtual bool CharacterAllowedCheck(const char* theChar);
   /// Remove all unsafe characters from the given string.
   //virtual void DisallowedCharactersRemove(char* theText);

   /// Helper function for handling input from a given keyboard.
   virtual void KeyboardInputHelper(Keyboard* keyboard,
      bool* triggerEnterPressEvent, bool* triggerTabPressEvent,
      bool* leftPressed, bool* rightPressed,
      bool* beginningOfLinePressed, bool* endOfLinePressed,
      bool* deleteForwardPressed, bool* selectPressed,
      bool* cutPressed, bool* copyPressed, bool* pastePressed);

   /// Return the position of the edit point in bytes, rather than in UTF-8
   /// characters.  Return -1 if unsuccessful.
   int CaretByteIndexGet();

   /// Delete the selected text.  Return true if successful.
   bool TextSelectionDelete();

   /// Helper function for building a string for display purposes in which all
   /// the characters have been replaced with asterisks.
   bool PasswordDisplayTextHelper(char* buffer, size_t bufferSize);

   /// True if the inherited LabelWidget is done initializing.
   bool inheritedInitialized;
   /// Maximum number of characters.
   int maxTextLength;
   /// Size of the text buffers.
   int textBufferSize;
   /// Position at which edits should be made.
   int caretCharacterIndex;
   /// Index of the character from which selecting text began.  This is -1 if no
   /// text is selected.
   int textSelectionMarkCharacterIndex;
   /// Timer that controls the blinking of the caret.
   unsigned int blinkTimer;
   /// True if the widget was displayed as though it had the focus on the last
   /// frame.
   bool hadFocusDisplay;
   /// True if this is for entering a password.
   bool password;
   /// True if there are no restrictions on which characters can be entered.
   bool allCharactersAllowed;
   /// Whitelist of characters to allow.
   const char* charactersAllowed;
   /// Buffer that stores the text as it should be displayed.
   char* displayText;
   /// Buffer used as scratch space when aligning text.
   char* scratchText;
   /// True if this widget should accept keyboard input as long as it is
   /// enabled, regardless of whether it has focus.
   bool acceptInputWithoutFocus;

   /// Function to be called for the OnEnterPress event.
   OnEnterPressCallback onEnterPressCallback;
   /// Pointer to be passed to the OnEnterPress callback.
   void* onEnterPressUserData;
   /// Function to be called for the OnTabPress event.
   OnTabPressCallback onTabPressCallback;
   /// Pointer to be passed to the OnTabPress callback.
   void* onTabPressUserData;
   /// Function to be called for the OnTextChange event.
   OnTextChangeCallback onTextChangeCallback;
   /// Pointer to be passed to the OnTextChange callback.
   void* onTextChangeUserData;
   /// Function to be called for the OnFocusGain event.
   OnFocusGainCallback onFocusGainCallback;
   /// Pointer to be passed to the OnFocusGain callback.
   void* onFocusGainUserData;
   /// Function to be called for the OnFocusLose event.
   OnFocusLoseCallback onFocusLoseCallback;
   /// Pointer to be passed to the OnFocusLose callback.
   void* onFocusLoseUserData;

   /// Determines when to respond to a left arrow key being held.
   HoldTracker leftKeyHoldTracker;
   /// Determines when to respond to a right arrow key being held.
   HoldTracker rightKeyHoldTracker;
   /// Determines when to respond to a delete forward key being held.
   HoldTracker deleteForwardKeyHoldTracker;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__TEXTENTRYWIDGET_H__
