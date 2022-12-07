#ifndef __FROG__GUI__POPUP_H__
#define __FROG__GUI__POPUP_H__

#include "FrogMemory.h"

namespace Webfoot {

class LayerWidget;
class PopupManager;

/// Default name of the transition to play when a popup appears.
#define POPUP_INIT_TRANSITION_NAME_DEFAULT "Enter"
/// Default name of the transition to play when a popup is closed.
#define POPUP_DEINIT_TRANSITION_NAME_DEFAULT "Exit"

//==============================================================================

/// A Popup manages a GUI layer that is pushed onto the GUI layer stack
/// independently from the current GameState.  Multiple Popups can be displayed
/// at the same time.
class Popup
{
public:
   Popup();

   virtual void Init();
   /// Called automatically after the menu's GUILayer is created or recreated.
   /// Use this to set up event handlers, populate lists, etc.
   virtual void OnGUILayerInit(LayerWidget* _layerWidget) { (void)_layerWidget; }
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Close the popup.
   virtual void Close(bool animated = true);
   /// Return true if this popup is in the process of closing.
   virtual bool ClosingCheck() { return waitingForExitTransition || deinitDeferred; }

   /// Return true if this object should be deleted when it is complete.
   bool ShouldDeleteOnDeinitCheck() { return shouldDeleteOnDeinit; }
   /// Set whether this object should be deleted when it is complete.
   void ShouldDeleteOnDeinitSet(bool _shouldDeleteOnDeinit) { shouldDeleteOnDeinit = _shouldDeleteOnDeinit; }

   /// Return the name of the GUI interface for this popup.
   const char* GUILayerNameGet();
   /// Set the name of the GUI interface for this popup.  This should be called
   /// before the GUI layer is displayed.
   void GUILayerNameSet(const char* _guiLayerName);

   /// Return the name to use for the Init transition.
   const char* InitTransitionNameGet();
   /// Set the name to use for the Init transition.  It is generally safe to
   /// call this before calling Init.
   void InitTransitionNameSet(const char* _initTransitionName);

   /// Return the name to use for the Deinit transition.
   const char* DeinitTransitionNameGet();
   /// Set the name to use for the Deinit transition.  It is generally safe to
   /// call this before calling Init.
   void DeinitTransitionNameSet(const char* _deinitTransitionName);

   /// Refresh the text.  This is called automatically when the language
   /// changes.
   virtual void TextRefresh() {}

protected:
   enum
   {
      /// Size of the buffers used for different names.
      NAME_BUFFER_SIZE = 128
   };

   /// Callback for the GUILayer's OnInit event.
   static void _OnGUILayerInit(LayerWidget* layer, void* userData);

   /// True if the popup should be removed as soon as the exit transition is
   /// complete.
   bool waitingForExitTransition;
   /// True if this object should be deleted when it is complete.
   bool shouldDeleteOnDeinit;
   /// Widget for the root of the displayed popup.
   LayerWidget* layerWidget;
   /// Used by PopupManager to avoid removing items during an update.
   bool deinitDeferred;

   /// Name of the GUI interface for this popup.
   char guiLayerName[NAME_BUFFER_SIZE];
   /// Name to use for the Init transition.
   char initTransitionName[NAME_BUFFER_SIZE];
   /// Name to use for the Deinit transition.
   char deinitTransitionName[NAME_BUFFER_SIZE];

   /// True if 'guiLayerName' has been specified.
   bool guiLayerNameSpecified;
   /// True if 'initTransitionName' has been specified.
   bool initTransitionNameSpecified;
   /// True if 'deinitTransitionName' has been specified.
   bool deinitTransitionNameSpecified;

   friend class PopupManager;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GUI__POPUP_H__
