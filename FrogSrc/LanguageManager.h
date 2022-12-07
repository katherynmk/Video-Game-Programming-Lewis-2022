#ifndef __FROG__LANGUAGEMANAGER_H__
#define __FROG__LANGUAGEMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Name of the event triggered when the language changes.
#define ON_LANGUAGE_CHANGE_EVENT_NAME "OnLanguageChange"

//==============================================================================

/// Possible languages for the game.  Not all are necessarily supported by
/// each game.
enum Language
{
   LANGUAGE_INVALID = -1,
   LANGUAGE_ENGLISH,
   LANGUAGE_FRENCH,
   LANGUAGE_SPANISH,
   LANGUAGE_ITALIAN,
   LANGUAGE_GERMAN,
   LANGUAGE_RUSSIAN,
   LANGUAGE_DUTCH,
   LANGUAGE_PORTUGUESE_BRAZIL,
   LANGUAGE_COUNT
};

//==============================================================================

/// When the OnLanguageChange event is triggered, a pointer to an instance
/// of this structure will be passed as the void*.
struct LanguageChangeData
{
   /// Language that was in-use before the change.
   Language oldLanguage;
   /// Language that is now in-use after the change.
   Language newLanguage;
};

//==============================================================================

/// Keeps track of the current language for localization purposes.
/// The default language is English.
class LanguageManager
{
public:
   LanguageManager();
   
   /// Switch to the given language.  If this is different from the current
   /// language, the OnLanguageChange event will be triggered.
   void Set(Language _language);
   /// Return the current language.
   Language Get() { return language; }
   
   /// Return the name of the current language in English.  For example,
   /// "Spanish", not "Español".
   const char* NameGet();
   /// Return the name of the given language in English.
   const char* NameGet(Language _language);
   /// Return the language with the given name in English.  For example,
   /// "Spanish", not "Español".
   Language GetByName(const char* name);

   /// Reload the current language.
   void Refresh();
   
   static LanguageManager instance;
   
protected:
   /// Currently selected language.
   Language language;
};

LanguageManager* const theLanguage = &LanguageManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__LANGUAGEMANAGER_H__
