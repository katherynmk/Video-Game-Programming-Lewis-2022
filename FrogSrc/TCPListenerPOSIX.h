#ifndef __FROG__TCPLISTENERPOSIX_H__
#define __FROG__TCPLISTENERPOSIX_H__

#include "FrogMemory.h"

namespace Webfoot {

class TCPConnectionPOSIX;
class TCPListenerPOSIXHelper;

//==============================================================================

/// TCPListenerPOSIX listens for incoming TCP connections and creates
/// TCPConnectionPOSIX for them as they are accepted.
class TCPListenerPOSIX
{
public:
   TCPListenerPOSIX();

   void Init();
   void Deinit();
   void Update();

   /// Begin listening on the given port.  Return true if successful.
   bool Listen(int portNumber);

   /// Return true if listening.
   bool ListeningCheck();

   /// Type of the function that can receive the OnConnect event.
   /// The TCPConnectionPOSIX* is the new connection, TCPListenerPOSIX* is the
   /// listener which accepted the connection, and the void* is the userData
   /// pointer.
   typedef void (*OnConnectCallback)(TCPConnectionPOSIX*, TCPListenerPOSIX*, void*);
   /// Register the given function to be called with the specified 'userData'
   /// pointer when the OnConnect event occurs.
   /// For now, only one function can be registered per instance.
   void OnConnectRegister(OnConnectCallback callback, void* userData = NULL);
   /// Stop calling the given function when the OnConnect event occurs.
   void OnConnectUnregister(OnConnectCallback callback);

protected:
   void OnConnect(TCPConnectionPOSIX* connection);

   /// Helper for abstracting the underlying API.
   TCPListenerPOSIXHelper* helper;
   /// Function to be called for the OnConnect event.
   OnConnectCallback onConnectCallback;
   /// Pointer to be passed to the OnConnect callback.
   void* onConnectUserData;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TCPLISTENERPOSIX_H__
