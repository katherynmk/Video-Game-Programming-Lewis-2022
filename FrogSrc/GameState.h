#ifndef __FROG__GAMESTATE_H__
#define __FROG__GAMESTATE_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// Classes can inherit GameState for use with GameStateManager as a way of
/// managing program flow.
class GameState
{
public:
   virtual ~GameState() {}

   /// Called before Init.
   virtual void PreInit() {}
   /// Make the state active.
   virtual void Init() {}
   /// Called before Deinit.
   virtual void PreDeinit() {}
   /// Make the state inactive.
   virtual void Deinit() {}

   /// Called once at the beginning of each frame, at the end of the
   /// GameStateManager::Update call.
   virtual void UpdateEarly() {}
   /// Called once per frame for most logic that does not involve drawing.
   virtual void Update() {}
   /// Called once per screen per frame for anything the state needs to draw.
   virtual void Draw() {}

   /// Refresh the text.  This is called automatically when the language
   /// changes.
   virtual void TextRefresh() {}
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GAMESTATE_H__
