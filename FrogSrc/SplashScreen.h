#ifndef __SPLAHSCREEN_H__
#define __SPLAHSCREEN_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Image.h"
#include "GameState.h"
#include "GameStateManager.h"

namespace Webfoot {

//==============================================================================

/// Show a sequence of Images and fade between them.  The Image sequence must be
/// specified before the SplashScreen state is initialized.  When the sequence
/// is over, this state will be popped from the stack automatically.  The
/// FadeManager will be faded-out when this state finishes.
class SplashScreen : public GameState
{
public:
   SplashScreen();
   virtual ~SplashScreen() {}
   
   virtual void Init();
   virtual void Deinit();
   virtual void Update();
   virtual void Draw();
   
   /// Set the sequence of images to show the next time the SplashScreen is
   /// initialized.  'sequenceStringNext' should be a |-delimited list of Images.
   /// This should be called shortly before Init is called. 
   void SequenceSet(const char* _sequenceStringNext, HeapID heapID = HEAP_TEMP);
   
   /// Set the length of a fade-in or fade-out in milliseconds.
   /// It is safe to call this while the object is not initialized.
   void FadeDurationSet(unsigned int _fadeDuration) { fadeDuration = _fadeDuration; }
   /// Set the length of time to show a single image at full opacity in milliseconds.
   /// It is safe to call this while the object is not initialized.
   void OpaqueDurationSet(unsigned int _opaqueDuration) { opaqueDuration = _opaqueDuration; }

   static SplashScreen instance;
   
protected:
   /// Proceed to the beginning of the next image.
   void NextImage();

   /// Called when the end of the sequence is reached.  This is not called if
   /// Deinit is called before the sequence has ended.
   virtual void OnSequnceFinish() { theStates->Pop(); }

   /// |-delimited list of Images to be shown the next time Init is called.
   const char* sequenceStringNext;

   /// Length of a fade-in or fade-out in milliseconds.
   unsigned int fadeDuration;
   /// Length of time to show a single image at full opacity in milliseconds.
   unsigned int opaqueDuration;
   /// Many many milliseconds into a given image's display time when it begins to fade out
   unsigned int opaqueEndTime;
   /// How many milliseconds a single image will be visible.
   unsigned int imageDuration;
   
   /// Which sprite in the series is currently being shown.
   int currentImageIndex;
   /// How many milliseconds have passed since the current sprite started to fade in.
   unsigned int currentImageTime;

   /// Series of images to be displayed.
   Table<Image*> images;
};

SplashScreen* const theSplashScreen = &SplashScreen::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __SPLAHSCREEN_H__
