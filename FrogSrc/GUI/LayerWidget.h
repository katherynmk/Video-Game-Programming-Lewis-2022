#ifndef __FROG__GUI__LAYERWIDGET_H__
#define __FROG__GUI__LAYERWIDGET_H__

#include "FrogMemory.h"
#include "GUI/ContainerWidget.h"

namespace Webfoot {

class Widget;

/// Pattern used to make the filename for a layer's sprite file.
#define GUI_LAYER_SPRITE_RESOURCE_FILENAME_PATTERN "%s/Sprites"
/// Pattern used to make the filename for the layer widgets specification file.
#define GUI_LAYER_WIDGETS_FILENAME_PATTERN "%s/Widgets"

//=============================================================================

/// Collection of widgets that appear together and are defined externally.
/// Call InitFromFolder immediately after construction.
/// Call OnInit immediately after the widget has been added to the tree of widgets.
class LayerWidget : public ContainerWidget
{
public:
   LayerWidget();
   virtual ~LayerWidget() {}

   /// Initialize a new LayerWidget object based on the data in the given folder.
   /// Give the object the specified name.  Be sure that _name is the
   /// same alias used in the list of guis.  If 'specifications' is provided,
   /// use it instead of loading the specifications from the file.
   /// Call this immediately after construction.
   virtual void InitFromFolder(const char* _name, Widget* rootWidget,
      const char* folderName, JSONValue* specifications);

   /// Clean up the widget.
   virtual void Deinit();

   /// Update the widget.  When available, use the specific input event handlers
   /// rather than checking directly for input.
   virtual void Update(unsigned int dt);

   /// Return true if the widget can respond to user input.
   virtual bool EnabledCheck();

   /// Return the name of the type of this widget.
   virtual const char* WidgetTypeNameGet();

   /// Return true if this widget should call event scripts.
   virtual bool EventScriptsUseCheck() { return true; }

   /// This must be called after this widget has been added to the tree of widgets.
   /// If this were called from Init before being added to the tree of widgets,
   /// there would be a chance that the paths used in the OnInit script would not work.
   virtual void OnInit();

   /// Called whenever the left mouse button is released.
   /// Warning: This function will be called regardless of whether the widget is currently enabled.
   /// Be sure to check EnabledCheck if you want to do something only when it is enabled.
   virtual void OnMouseUp();

   /// Called when this widget or an ancestor is about to be removed from
   /// the tree of widgets.
   /// Call the OnDeinit script from here, because this is the last point
   /// in deinitialization where the tree of widgets has not yet been
   /// disassembled in any way.  We use OnDeinit instead of PreRemove,
   /// because Deinit is the conventional term in our scripts for deinitialization.
   /// The actual Deinit function would not be as stable a place from which
   /// to do anything in script.
   virtual void PreRemove();

   /// Return true if the layer with the given name exists.
   /// Specifically, this checks for the Widgets.lua file in the appropriate folder.
   static bool ExistsCheck(const char* name);

   /// Set whether layers below this one should be treated as disabled.
   void DisableLayersBelowSet(bool _disableLayersBelow) { disableLayersBelow = _disableLayersBelow; }
   /// Return true if layers below this one should be treated as disabled.
   virtual bool DisableLayersBelowCheck() { return disableLayersBelow; }

   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnInit event occurs.
   /// For now, only one function can be registered per instance.
   void OnInitRegister(LayerWidgetOnInitCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnInit event occurs.
   void OnInitUnregister(LayerWidgetOnInitCallback callback);

   /// Type of the function that can receive the Update event.
   /// The unsigned int should be treated as the number of milliseconds that
   /// have passed, the LayerWidget* is the widget that was updated, and the
   /// void* is the userData pointer.
   typedef void (*OnUpdateCallback)(unsigned int, LayerWidget*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the Update method is called.
   /// For now, only one function can be registered per instance.
   void OnUpdateRegister(OnUpdateCallback callback, void* userData = NULL);
   /// Stop calling the given function when Update is called.
   void OnUpdateUnregister(OnUpdateCallback callback);
   /// Convenience function for finding a widget by path and setting
   /// it to use the given callback and userData.  Return true if successful.
   static bool OnUpdateRegister(const char* path, OnUpdateCallback callback, void* userData = NULL);

protected:
   typedef ContainerWidget Inherited;

   /// Whether this instance currently has its reference count incremented
   bool referenceCountIncremented;
   /// True if layers below this one should be treated as disabled.
   bool disableLayersBelow;

   /// Function to be called for the OnInit event.
   LayerWidgetOnInitCallback onInitCallback;
   /// Pointer to be passed to the OnInit callback.
   void* onInitUserData;

   /// Function to be called when Update is called.
   OnUpdateCallback onUpdateCallback;
   /// Pointer to be passed to the OnUpdate callback.
   void* onUpdateUserData;

#if defined _DEBUG && PLATFORM_IS_WINDOWS
   /// Time at which the file containing the widget specifications was last modified.
   /// This is only used by GUIManager.
   int64 widgetsFileModificationTime;
#endif //#if _DEBUG && PLATFORM_IS_WINDOWS

   friend class GUIManager;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__GUI__LAYERWIDGET_H__
