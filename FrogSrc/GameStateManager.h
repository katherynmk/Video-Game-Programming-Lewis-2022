#ifndef __FROG__GAMESTATEMANAGER_H__
#define __FROG__GAMESTATEMANAGER_H__

#include "FrogMemory.h"
#include "Table.h"

namespace Webfoot {

class GameState;

/// Event triggered by the GameStateManager after a state has been
/// deinitialized.
#define GAME_STATE_POST_DEINIT_EVENT_NAME "GameStatePostDeinit"

//==============================================================================

/// GameStateManager manages a stack of GameState objects to control program
/// flow.  GameStates should typically be singletons to prevent memory
/// fragmentation.  The manager will call the Init and Deinit methods of the 
/// GameState objects such that only one state is between calls to Init and
/// Deinit at any one time.  Requests to change state are deferred until the
/// manager's Update method is called.  This is to prevent bugs caused by two
/// states being active during the same frame.  If multiple state changes are
/// requested during a single frame, the actual change will be from the current
/// state to whichever state was last requested.
///
/// To facilitate transition effects, you can call StateChangeWait and
/// StateChangeContinue to prevent state changes from actually happening until
/// the effect has finished.
class GameStateManager
{
public:
   GameStateManager();

   /// Initialize the manager and the given initial state, if any.  Reserve
   /// space for the given number of states.
   void Init(GameState* firstState = NULL, int stackReserveSize = 32, HeapID heapID = HEAP_DEFAULT);
   /// If a state is still initialized, deinitialize it now.
   /// Clean up the manager as well.   
   void Deinit();
   
   /// Called at the beginning of each update to manage state changes.
   /// Return true if a state change occurred.
   bool Update();
   /// Update the current state, if any.
   void StateUpdate();
   /// Draw the current state, if any.
   void StateDraw();
   
   /// Request a change from the current state, if any, to the given state.
   /// It is acceptable to specify 'NULL' if you do not want any state to be
   /// active.
   void Set(GameState* state) { Pop(); Push(state); }
   /// Request a change to the given state and push it onto the stack.
   void Push(GameState* state);
   /// Insert the given 'state' into the stack below the given 'referenceState',
   /// assuming 'refrenceState' is already in the stack.
   void InsertBelow(GameState* referenceState, GameState* state);
   /// Remove the current state from the stack and request a change to the one
   /// below it.
   void Pop();
   /// Remove the given state from the stack.
   void RemoveIndex(int index);
   /// Remove all states from the stack and request that the current state
   /// be deinitialized.
   void Clear();
   
   /// Return true if a state change has been requested.  This will still
   /// return true if waiting due to a call to StateChangeWait().
   bool StateChangeCheck() { return stateChangeRequested; }
   
   /// Delay any state changes until StateChangeContinue is called.
   /// This uses a counter internally. 
   void StateChangeWait() { stateChangeWaiting++; }
   /// Proceed with a state change that was delayed by StateChangeWait on the
   /// next manager update.  This uses a counter internally. 
   void StateChangeContinue() { stateChangeWaiting--; }
   /// Return true if waiting for a state change.
   bool StateChangeWaitingCheck() { return stateChangeWaiting > 0; }
   
   /// Return a pointer to the active state, if any.
   GameState* StateCurrentGet() { return stateCurrent; }
   /// Return a pointer to the next state to which the manager will transition,
   /// if one has been specified.  Since you can change to a NULL state,
   /// do not use this to determine whether a state change has been requested.
   /// Use StateChangeCheck() for that.
   GameState* StateNextGet();

   /// Return the current number of states in the stack.
   int StackSizeGet() { return stateStack.SizeGet(); }
   /// Return the item at the given point in the stack.
   GameState* StateGet(int index);

   /// Refresh the text for the current GameState.  This is called automatically
   /// when the language changes.
   void TextRefresh();

   static GameStateManager instance;
    
protected:
   /// The active state, if any.
   GameState* stateCurrent;
   /// True if a state change has been requested.
   bool stateChangeRequested;
   /// True if state changes should be delayed for now.
   int stateChangeWaiting;
   /// Collection of GameStates.
   Table<GameState*> stateStack;
};

GameStateManager* const theStates = &GameStateManager::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__GAMESTATEMANAGER_H__
