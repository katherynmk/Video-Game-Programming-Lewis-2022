#ifndef __FROG__WINDOWMENU_H__
#define __FROG__WINDOWMENU_H__

#include "FrogMemory.h"
#include "iOSPlatformManager.h"
#include <windows.h>

namespace Webfoot {

/// Maximum number of custom control configurations for a given platform.
#define WINDOW_MENU_CONTROL_CONFIGURATION_CUSTOM_COUNT_MAX 32
/// Maximum number of iOS devices which can be simulated.
#define WINDOW_MENU_IOS_DEVICE_COUNT_MAX 11
/// Maximum number of custom window sizes.
#define WINDOW_MENU_WINDOW_SIZE_CUSTOM_COUNT_MAX 16

/// Used to coordinate window menu item IDs for debug PC builds.
enum
{
   /// Used during initialization of Screen.
   DUMMY_ITEM_ID,
   /// Menu item ID for the "Ad Preview" button
   AD_PREVIEW_VISIBLE_ITEM_ID,
   /// Menu item ID for the "Intermediate Framebuffer" button
   INTERMEDIATE_FRAMEBUFFER_ENABLED_ITEM_ID,
   /// Menu item ID to use for the first device.
   IOS_DEVICE_BASE_ITEM_ID,
   /// Menu item to disable simulated iTunes errors.
   ITUNES_ERRORS_NONE_ITEM_ID = IOS_DEVICE_BASE_ITEM_ID + WINDOW_MENU_IOS_DEVICE_COUNT_MAX,
   /// Menu item for immediate simulated iTunes errors.
   ITUNES_ERRORS_IMMEDIATE_ITEM_ID,
   /// Menu item for deferred simulated iTunes errors.
   ITUNES_ERRORS_DEFERRED_ITEM_ID,
   /// Menu item for no delays in simulated iTunes tasks.
   ITUNES_DELAYS_NONE_ITEM_ID,
   /// Menu item for 1-second delays in simulated iTunes tasks.
   ITUNES_DELAYS_1_SECOND_ITEM_ID,
   /// Menu item for infinite delays in simulated iTunes tasks.
   ITUNES_DELAYS_INFINITE_ITEM_ID,
   /// Menu item for allowing simulated iTunes purchases.
   ITUNES_PURCHASES_ALLOWED_ITEM_ID,
   /// Menu item for canceling simulated iTunes purchases.
   ITUNES_CANCEL_PURCHASES_ITEM_ID,
   /// Show only the image for the left eye.
   CTR_STEREO_3D_TYPE_LEFT_ONLY_ITEM_ID,
   /// Show only the image for the right eye.
   CTR_STEREO_3D_TYPE_RIGHT_ONLY_ITEM_ID,
   /// Combine images using Dubois optimized anaglyph.
   CTR_STEREO_3D_TYPE_DUBOIS_ANAGLYPH_ITEM_ID,
   /// Combine images using grayscale anaglyph.
   CTR_STEREO_3D_TYPE_GRAYSCALE_ANAGLYPH_ITEM_ID,
   /// Combine images using basic red/cyan anaglyph.
   CTR_STEREO_3D_TYPE_SIMPLE_ANAGLYPH_ITEM_ID,
   /// Simulates the 3DS depth slider at 0%.
   CTR_DEPTH_SLIDER_0_ITEM_ID,
   /// Simulates the 3DS depth slider at 25%.
   CTR_DEPTH_SLIDER_25_ITEM_ID,
   /// Simulates the 3DS depth slider at 50%.
   CTR_DEPTH_SLIDER_50_ITEM_ID,
   /// Simulates the 3DS depth slider at 75%.
   CTR_DEPTH_SLIDER_75_ITEM_ID,
   /// Simulates the 3DS depth slider at 100%.
   CTR_DEPTH_SLIDER_100_ITEM_ID,
   /// 3DS screen magnification of 1x
   CTR_MAGNIFICATION_1X_ITEM_ID,
   /// 3DS screen magnification of 2x
   CTR_MAGNIFICATION_2X_ITEM_ID,
   /// 3DS layout type where the two screens have no separation between them.
   CTR_LAYOUT_TYPE_JOINED_ITEM_ID,
   /// 3DS layout type where the two screens have a separation between them.
   CTR_LAYOUT_TYPE_SEPARATE_ITEM_ID,
   /// 3DS window mask color of black
   CTR_MASK_COLOR_BLACK_ITEM_ID,
   /// 3DS window mask color of white
   CTR_MASK_COLOR_WHITE_ITEM_ID,
   /// 3DS window mask color of red
   CTR_MASK_COLOR_RED_ITEM_ID,
   /// 3DS window mask color of green
   CTR_MASK_COLOR_GREEN_ITEM_ID,
   /// 3DS window mask color of blue
   CTR_MASK_COLOR_BLUE_ITEM_ID,
   /// Menu item for using the empty control configuration for a platform.
   CONTROL_CONFIGURATION_NONE_ITEM_ID,
   /// Menu item for using the basic controls for a platform.
   CONTROL_CONFIGURATION_BASIC_ITEM_ID,
   /// Menu item for using the basic XInput controls for a platform.
   CONTROL_CONFIGURATION_XINPUT_ITEM_ID,
   /// Menu item for the first custom control configuration for a platform.
   CONTROL_CONFIGURATION_CUSTOM_BASE_ITEM_ID,
   /// Back button of an Android device.
   ANDROID_BACK_BUTTON_ITEM_ID = CONTROL_CONFIGURATION_CUSTOM_BASE_ITEM_ID + WINDOW_MENU_CONTROL_CONFIGURATION_CUSTOM_COUNT_MAX,
   /// Menu item ID to use for the default window size.
   WINDOW_SIZE_DEFAULT_ITEM_ID,
   /// Menu item ID to use for the first custom window size.
   WINDOW_SIZE_CUSTOM_BASE_ITEM_ID,
   /// Menu item ID to use for the first iOS key command.
   IOS_KEY_COMMAND_BASE_ITEM_ID = WINDOW_SIZE_CUSTOM_BASE_ITEM_ID + WINDOW_MENU_WINDOW_SIZE_CUSTOM_COUNT_MAX,
   /// Highest window menu item ID number reserved by Frog.
   FROG_ITEM_ID_LAST = IOS_KEY_COMMAND_BASE_ITEM_ID + IOS_KEY_COMMAND_COUNT
};

//===============================================================================

/// 'WindowMenu' is an interface to help add debug menus to Windows builds.
class WindowMenu
{
public:
   /// Called to allow the WindowMenu to add its content to the root window menu.
   virtual void SubmenusAdd() = 0;

   /// This is called with incoming Windows messages.  If this returns true, the
   /// caller should return the value referenced by 'returnValue'.
   virtual bool MessageNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, long* returnValue) = 0;
};

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__WINDOWMENU_H__
