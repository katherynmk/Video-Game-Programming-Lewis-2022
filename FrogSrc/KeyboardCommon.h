#ifndef __FROG__KEYBOARDCOMMON_H__
#define __FROG__KEYBOARDCOMMON_H__

#include "FrogMemory.h"
#include "Port.h"
#include "Utility.h"

namespace Webfoot {

//==============================================================================

/// KeyboardCommon is a platform-independent representation of a keyboard that is updated
/// by a platform-specific KeyboardManager.  The key codes are included with the
/// individual managers.
class KeyboardCommon
{
public:
   void Init();
   void Deinit();
   
   /// Call this on every frame, and set 'hasFocus' to true if the window
   /// currently has the focus.
   void Update(bool hasFocus);
   
   /// Return true if any key is currently pressed.
   bool KeyPressed();
   /// Return true if the given key is currently pressed.
   bool KeyPressed(int keyCode);

   /// Return true if any key was just pressed as of the last update.
   bool KeyJustPressed();
   /// Return true if the given key was just pressed as of the last update.
   bool KeyJustPressed(int keyCode);

   /// Return true if any key was just released as of the last update.
   bool KeyJustReleased();
   /// Return true if the given key was just released as of the last update.
   bool KeyJustReleased(int keyCode);
   
   /// Return the number of characters typed during the last frame.
   int TextInputCountGet() const { return textInputCountFront; }
   /// Return a pointer to the UTF-8 character of the given index in the
   /// sequence of characters typed during the last frame.
   /// Each character is given as a NULL-terminated UTF-8 string.
   const char* TextInputGet(int characterIndex) const { return (const char*)(&textInputFront[characterIndex * CHAR_BUFFER_SIZE]); }

   /// Return true if the keyboard should generally be treated as connected.
   bool ConnectedCheck() { return connected; }
   /// Set whether the keyboard should generally be treated as connected.  This should
   /// typically only be called by a KeyboardManager or for debug purposes.
   void ConnectedSet(bool _connected) { connected = _connected; }

   /// Return a platform-independent string representation of the given
   /// key code.  Return NULL if unsuccessful.
   static const char* KeyCodeToString(int keyCode);
   /// Return the key code that correspends to the given platform-independent
   /// string.  Return KEY_NONE if unsuccessful.
   static int KeyStringToCode(const char* keyString);

   enum
   {
      /// Number of keys to track.
      KEY_COUNT_MAX = 512,
      /// Maximum size of a UTF-8 character plus a NULL.
      CHAR_BUFFER_SIZE = UTF8_CHARACTER_SIZE_MAX + 1,
      /// Maximum number of typed characters that can be captured on a given frame.
      TEXT_CHAR_COUNT_MAX = 256,
      /// Number of internal buffers for key states.
      KEY_STATE_BUFFER_COUNT = 3,
      /// Number of internal buffers for text input.
      TEXT_INPUT_BUFFER_COUNT = 2
   };
   
protected:
   /// Buffer of key states established on the Update before last.
   uchar* keyStatesLast;
   /// Buffer of key states for the most recent Update.
   uchar* keyStatesCurrent;
   /// Back buffer used for the key states of the next Update.
   uchar* keyStatesNext;

   /// Buffer into which incoming text should be written.
   char* textInputBack;
   /// Buffer from which text that has already arrived should be read.
   char* textInputFront;
   /// Number of characters typed and captured during the last frame.
   int textInputCountFront;
   /// Running count of the number of characters captured since the last update.
   int textInputCountBack;

   /// True if the keyboard should generally be treated as connected.
   bool connected;

   /// One set of keys is for the current back buffer, one is for the current front buffer,
   /// and one is for the previous current buffer.
   /// The uchar for a given key is non-zero when pressed.
   uchar keyStates[KEY_STATE_BUFFER_COUNT][KEY_COUNT_MAX];
   /// Array of captured UTF-8 characters, each followed by a NULL terminator.
   /// One set of characters is for the current back buffer and the other is for
   /// the front buffer.
   char textInput[TEXT_INPUT_BUFFER_COUNT][TEXT_CHAR_COUNT_MAX][CHAR_BUFFER_SIZE];
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__KEYBOARDCOMMON_H__
