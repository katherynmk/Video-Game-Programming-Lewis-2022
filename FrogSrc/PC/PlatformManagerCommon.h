#ifndef __FROG__PLATFORMMANAGERCOMMON_H__
#define __FROG__PLATFORMMANAGERCOMMON_H__

#include "FrogMemory.h"

namespace Webfoot {

class JSONValue;
class FileManager;

//==============================================================================

/// Base class for managing miscellaneous aspects of PC builds of projects
/// targeting other platforms.
class PlatformManagerCommon
{
public:
   PlatformManagerCommon();
   virtual ~PlatformManagerCommon() {}

   void Init();
   void Deinit();

   /// Return JSON data common to all the PC builds of projects for this
   /// platform.
   JSONValue* PlatformSettingsGet();
   /// Save the settings common to all the PC builds of projects for this
   /// platform.
   void PlatformSettingsSave();

protected:
   /// Return the name of the folder with data common to all PC builds of
   /// projects for the platform.
   virtual const char* PlatformSaveDataFolderGet() = 0;

   /// FileManager for files that are common to all of the PC builds of projects
   /// for this platform.
   FileManager* platformFileManager;
   /// JSON data common to all the PC builds of projects for this platform.
   JSONValue* platformSettings;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PLATFORMMANAGERCOMMON_H__
