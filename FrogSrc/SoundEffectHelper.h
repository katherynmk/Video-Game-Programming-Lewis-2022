#ifndef __FROG__SOUNDEFFECTHELPER_H__
#define __FROG__SOUNDEFFECTHELPER_H__

#include "FrogMemory.h"
#include "Table.h"
#include "JSONValue.h"
#include "Sound.h"

namespace Webfoot {

class SoundEffectHelperSoundGroup;
class SoundEffectHelperSoundSourceGroup;

//==============================================================================

/// SoundEffectHelper helps deal with some common issues with playing sound
/// effects.  Groups of sound effects can be specified in JSON, and this system
/// can be instructed to choose and play one of them.  Each of these groups can
/// have a cooldown time to prevent too many instances of sounds from the group
/// being started too quickly.
///
/// All sound groups must also name a sound source group.  A sound source group
/// contains a specified number of SoundSource objects.  This helps prevent too
/// many concurrent sounds from a given sound group or from a set of sound
/// groups which use the same sound source group.  This can also be used to
/// prevent mutually exclusive sounds from playing at once.
///
/// The system does not automatically load or unload sounds.  Any sounds
/// specified for this system should already be loaded, such as using a loading
/// screen, before you attempt to use them.
class SoundEffectHelper
{
public:
   void Init(JSONValue* specifications);
   void Deinit();
   void Update(unsigned int dt);

   /// Play one sound from the given sound group.
   void GroupPlayOne(const char* soundGroupName, Sound::Usage usage = Sound::USAGE_DEFAULT, int volume = Sound::VOLUME_DEFAULT);

   /// Return true if all sounds played through this system are stopped.
   bool AllSoundsStoppedCheck();

   static SoundEffectHelper instance;

protected:
   /// Return the sound group with the given name.
   SoundEffectHelperSoundGroup* SoundGroupGet(const char* name);
   /// Return the sound source group with the given name.
   SoundEffectHelperSoundSourceGroup* SoundSourceGroupGet(const char* name);

   /// Collection of sound groups managed by this object.
   Table<SoundEffectHelperSoundGroup*> soundGroups;
   /// Collection of sound groups managed by this object.
   Table<SoundEffectHelperSoundSourceGroup*> soundSourceGroups;

   friend class SoundEffectHelperSoundGroup;
};

SoundEffectHelper* const theSoundEffectHelper = &SoundEffectHelper::instance;

//==============================================================================

/// Helper object for 'SoundEffectHelper' which manages a set of sounds.
class SoundEffectHelperSoundGroup
{
public:
   SoundEffectHelperSoundGroup();

   void Init(const char* _name, JSONValue* specs);
   void Deinit();
   void Update(unsigned int dt);

protected:
   /// Name of this group.
   const char* name;
   /// Filenames of the sounds which make up the group.
   Table<const char*> soundFilenames;
   /// Minimum number of milliseconds between playing of sounds in this group.
   unsigned int cooldown;
   /// Timer for restricting the speed at which sounds in this group can be
   /// started.
   unsigned int cooldownTimer;
   /// Sound source group to use when playing sounds in this group.
   SoundEffectHelperSoundSourceGroup* soundSourceGroup;

   friend class SoundEffectHelper;
};

//==============================================================================

/// Helper object for 'SoundEffectHelper' which manages a set of sound sources.
class SoundEffectHelperSoundSourceGroup
{
public:
   SoundEffectHelperSoundSourceGroup() { name = NULL; }

   void Init(const char* _name, JSONValue* specs);
   void Deinit();

protected:
   /// Name of this group.
   const char* name;
   /// Collection of sources to use for this group.
   Table<SoundSource*> soundSources;

   friend class SoundEffectHelper;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SOUNDEFFECTHELPER_H__
