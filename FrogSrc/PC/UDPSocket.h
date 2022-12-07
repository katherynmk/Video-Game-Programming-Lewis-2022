#ifndef __FROG__UDPSOCKET_H__
#define __FROG__UDPSOCKET_H__

#include "FrogMemory.h"
#include "UDPSocketPOSIX.h"

namespace Webfoot {
   class UDPSocketPOSIX;
   typedef UDPSocketPOSIX UDPSocket;

   struct UDPPacketPOSIX;
   typedef UDPPacketPOSIX UDPPacket;
} //namespace Webfoot {

#endif //#ifndef __FROG__UDPSOCKET_H__
