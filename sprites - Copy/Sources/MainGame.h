#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"


namespace Webfoot {



//==============================================================================

class MainGame : public MenuState
{
public:
   typedef MenuState Inherited;

   MainGame();
   virtual ~MainGame() {};
   
   virtual void Init();
   virtual void Deinit();
   virtual void Update();
   virtual void Draw();
   float distance(int x1, int y1, int x2, int y2);
   static MainGame instance;

protected:
   /// Returns the name of the GUI layer
   virtual const char* GUILayerNameGet();

   


};

MainGame* const theMainGame = &MainGame::instance;

//==============================================================================

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__
