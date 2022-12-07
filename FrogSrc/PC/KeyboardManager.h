#ifndef __FROG__KEYBOARDMANAGER_H__
#define __FROG__KEYBOARDMANAGER_H__

#include "FrogMemory.h"
#include "KeyboardCommon.h"
#include <windows.h>

namespace Webfoot {

class KeyboardManager;

//==============================================================================

/// Windows-specific keyboard class that gives full access to the Windows-specific
/// KeyboardManager
class Keyboard : public KeyboardCommon
{
public:
   friend class KeyboardManager;
};

//==============================================================================

/// Windows-specific class for keeping any keyboard objects up-to-date.
class KeyboardManager
{
public:
   void Init();
   void Deinit();
   
   /// Call this on every frame to update the keyboard objects with recent data.
   /// Set 'hasFocus' to true if the window currently has the focus.
   /// Update must only be called on the same thread as MessageNotify.
   void Update(bool hasFocus);
   
   /// Call this with incoming Windows messages.  If this returns true, the
   /// caller should return the value referenced by 'returnValue' rather than
   /// calling DefWindowProc.  Update must only be called on the same thread as
   /// MessageNotify.
   bool MessageNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, long* returnValue);

   /// Singleton Keyboard
   static Keyboard keyboard;
   /// Singleton KeyboardManager
   static KeyboardManager instance;
};

/// Singleton Keyboard
Keyboard* const theKeyboard = &KeyboardManager::keyboard;
/// Singleton KeyboardManager
KeyboardManager* const theKeyboardManager = &KeyboardManager::instance;

//==============================================================================

/// Key codes are declared at the root of the Webfoot namespace for ease-of-use.
enum KeyCode
{
   KEY_NONE = -1,
   KEY_BACKSPACE = VK_BACK,
   KEY_TAB = VK_TAB,
   KEY_ENTER = VK_RETURN,
   /// Pause Break
   KEY_PAUSE = VK_PAUSE,
   KEY_CAPSLOCK = VK_CAPITAL,
   /// Esc
   KEY_ESCAPE = VK_ESCAPE,
   /// Spacebar
   KEY_SPACE = VK_SPACE,
   KEY_PAGEUP = VK_PRIOR,
   KEY_PAGEDOWN = VK_NEXT,
   KEY_END = VK_END,
   KEY_HOME = VK_HOME,
   /// Left Arrow
   KEY_LEFT = VK_LEFT,
   /// Up Arrow
   KEY_UP = VK_UP,
   /// Right Arrow
   KEY_RIGHT = VK_RIGHT,
   /// Down Arrow
   KEY_DOWN = VK_DOWN,
   /// PrtScn SysRq
   KEY_PRINTSCREEN = VK_SNAPSHOT,
   KEY_INSERT = VK_INSERT,
   KEY_DELETE = VK_DELETE,
   KEY_0 = 0x30,
   KEY_1 = 0x31,
   KEY_2 = 0x32,
   KEY_3 = 0x33,
   KEY_4 = 0x34,
   KEY_5 = 0x35,
   KEY_6 = 0x36,
   KEY_7 = 0x37,
   KEY_8 = 0x38,
   KEY_9 = 0x39,
   KEY_A = 0x41,
   KEY_B = 0x42,
   KEY_C = 0x43,
   KEY_D = 0x44,
   KEY_E = 0x45,
   KEY_F = 0x46,
   KEY_G = 0x47,
   KEY_H = 0x48,
   KEY_I = 0x49,
   KEY_J = 0x4A,
   KEY_K = 0x4B,
   KEY_L = 0x4C,
   KEY_M = 0x4D,
   KEY_N = 0x4E,
   KEY_O = 0x4F,
   KEY_P = 0x50,
   KEY_Q = 0x51,
   KEY_R = 0x52,
   KEY_S = 0x53,
   KEY_T = 0x54,
   KEY_U = 0x55,
   KEY_V = 0x56,
   KEY_W = 0x57,
   KEY_X = 0x58,
   KEY_Y = 0x59,
   KEY_Z = 0x5A,
   KEY_NUMPAD0 = VK_NUMPAD0,
   KEY_NUMPAD1 = VK_NUMPAD1,
   KEY_NUMPAD2 = VK_NUMPAD2,
   KEY_NUMPAD3 = VK_NUMPAD3,
   KEY_NUMPAD4 = VK_NUMPAD4,
   KEY_NUMPAD5 = VK_NUMPAD5,
   KEY_NUMPAD6 = VK_NUMPAD6,
   KEY_NUMPAD7 = VK_NUMPAD7,
   KEY_NUMPAD8 = VK_NUMPAD8,
   KEY_NUMPAD9 = VK_NUMPAD9,
   KEY_NUMPAD_MULTIPLY = VK_MULTIPLY,
   KEY_NUMPAD_ADD = VK_ADD,
   KEY_NUMPAD_SUBTRACT = VK_SUBTRACT,
   KEY_NUMPAD_DECIMAL = VK_DECIMAL,
   KEY_NUMPAD_DIVIDE = VK_DIVIDE,
   KEY_F1 = VK_F1,
   KEY_F2 = VK_F2,
   KEY_F3 = VK_F3,
   KEY_F4 = VK_F4,
   KEY_F5 = VK_F5,
   KEY_F6 = VK_F6,
   KEY_F7 = VK_F7,
   KEY_F8 = VK_F8,
   KEY_F9 = VK_F9,
   KEY_F10 = VK_F10,
   KEY_F11 = VK_F11,
   KEY_F12 = VK_F12,
   KEY_F13 = VK_F13,
   KEY_F14 = VK_F14,
   KEY_F15 = VK_F15,
   KEY_F16 = VK_F16,
   KEY_F17 = VK_F17,
   KEY_F18 = VK_F18,
   KEY_F19 = VK_F19,
   KEY_F20 = VK_F20,
   KEY_F21 = VK_F21,
   KEY_F22 = VK_F22,
   KEY_F23 = VK_F23,
   KEY_F24 = VK_F24,
   KEY_NUMLOCK = VK_NUMLOCK,
   /// ScrLk
   KEY_SCROLLLOCK = VK_SCROLL,
   /// Left Shift
   KEY_LSHIFT = VK_LSHIFT,
   /// Right Shift
   KEY_RSHIFT = VK_RSHIFT,
   /// Left Ctrl
   KEY_LCONTROL = VK_LCONTROL,
   /// Right Ctrl
   KEY_RCONTROL = VK_RCONTROL,
   /// Left Alt
   KEY_LALT = VK_LMENU,
   /// Right Alt
   KEY_RALT = VK_RMENU,
   /// ; :
   KEY_SEMICOLON = VK_OEM_1,
   /// + = (As opposed to KEY_NUMPAD_PLUS)
   KEY_PLUS = VK_OEM_PLUS,
   /// , <
   KEY_COMMA = VK_OEM_COMMA,
   /// - _ (As opposed to KEY_NUMPAD_MINUS)
   KEY_MINUS = VK_OEM_MINUS,
   /// > . (As opposed to KEY_NUMPAD_DECIMAL)
   KEY_PERIOD = VK_OEM_PERIOD,
   /// (As opposed to KEY_NUMPAD_DIVIDE)
   KEY_SLASH = VK_OEM_2,
   /// ` ~
   KEY_GRAVE = VK_OEM_3,
   /// [ {
   KEY_LBRACKET = VK_OEM_4,
   /// \ |
   KEY_BACKSLASH = VK_OEM_5,
   /// ] }
   KEY_RBRACKET = VK_OEM_6,
   /// ' "
   KEY_QUOTE = VK_OEM_7,

   // Left Windows Key
   KEY_LWIN = VK_LWIN,
   /// Right Windows Key
   KEY_RWIN = VK_RWIN,
   /// Windows Applications Key
   KEY_APPS = VK_APPS,
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__KEYBOARDMANAGER_H__
