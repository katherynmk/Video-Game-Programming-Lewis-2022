#ifndef __FROG__UDPSOCKETPOSIX_H__
#define __FROG__UDPSOCKETPOSIX_H__

#include "FrogMemory.h"
#include "IPAddressPOSIX.h"

namespace Webfoot {

class UDPSocketPOSIXHelper;

//==============================================================================

/// Structure used to send and receive data through a UDP socket.
struct UDPPacketPOSIX
{
   void* buffer;
   size_t bufferSize;
   IPAddressPOSIX remoteAddress;
   uint16 remotePortNumber;
};

//==============================================================================

/// Manages the sending and receiving of UDP packets.
class UDPSocketPOSIX
{
public:
   static bool SharedInit();
   static void SharedDeinit();

   UDPSocketPOSIX();

   void Init();
   void Deinit();
   void Update();

   /// Return true if the object is bound to an address.
   bool BoundCheck();

   /// Have the socket to send and receive on the given port.  Specify -1 if
   /// the local port number isn't important.  Set 'allowBroadcasting' to true
   /// if this socket may be used for broadcasting. Return true if successful.
   bool Bind(int localPortNumber = -1, bool allowBroadcasting = false);

   size_t Receive(UDPPacketPOSIX* packet);
   size_t Send(UDPPacketPOSIX* packet);

protected:
   UDPSocketPOSIXHelper* helper;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__UDPSOCKETPOSIX_H__
