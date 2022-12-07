#ifndef __FROG__TCPCONNECTIONPOSIX_H__
#define __FROG__TCPCONNECTIONPOSIX_H__

#include "FrogMemory.h"
#include "IPAddressPOSIX.h"

namespace Webfoot {

class TCPConnectionPOSIXHelper;

//==============================================================================

/// TCPConnectionPOSIX manages one side of a TCP connection.  It may be either
/// the client side or the server side.  If you want to use the same object for
/// multiple connections, you must call Deinit and Init between connections to
/// clean up.
class TCPConnectionPOSIX
{
public:
   static bool SharedInit();
   static void SharedDeinit();

   TCPConnectionPOSIX();

   void Init();
   void Deinit();
   void Update();

   /// Return true if this is connected.  Beware that it may appear to be
   /// connected but a subsequent attempt to send or receive may reveal that it
   /// was not connected.
   bool ConnectedCheck() { return connected; }
   /// Return true if connection attempt has been started, but it hasn't
   /// succeeded or failed yet.
   bool ConnectingCheck() { return connecting; }

   /// Make a client connection to the given server.
   bool Connect(IPAddressPOSIX* remoteAddress, int portNumber);
   /// Attempt to send the first 'amountToSend' bytes from 'buffer'.  Return
   /// the number of bytes which were sent.  It may take more than one call to
   /// send an entire message.
   size_t Send(const void* buffer, size_t amountToSend);
   /// Attempt to receive the next 'amountToReceive' bytes from the
   /// connection to 'buffer'.  Return the number of bytes which were
   /// received.  It may take more than one call to receive an entire message.
   size_t Receive(void* buffer, size_t amountToReceive);

   /// Return the IP address of the other side of the connection.
   IPAddressPOSIX* RemoteAddressGet() { return &remoteAddress; }
   /// Return the port number of the other side of the connection.
   uint16 RemotePortNumberGet() { return remotePortNumber; }

protected:
   /// Form of Init called by TCPListenerPOSIX for a new incoming connection.
   void Init(void* _connectionSocket);

   /// Helper for abstracting the underlying API.
   TCPConnectionPOSIXHelper* helper;
   /// True if this is connected.
   bool connected;
   /// True if connection attempt has been started, but it hasn't succeeded or
   /// failed yet.
   bool connecting;
   /// IP address of the other side of the connection.
   IPAddressPOSIX remoteAddress;
   /// Port number of the other side of the connection.
   uint16 remotePortNumber;

   /// Tracks the balance of SharedInit and SharedDeinit calls.
   static int sharedInitCounter;

   friend class TCPListenerPOSIX;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TCPCONNECTIONPOSIX_H__
