#ifndef __FROG__IPADDRESSPOSIX_H__
#define __FROG__IPADDRESSPOSIX_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// Object to encapsulate an IP address.
class IPAddressPOSIX
{
public:
   static bool SharedInit();
   static void SharedDeinit();

   IPAddressPOSIX();

   /// Set this to a copy of the given address.
   void Set(IPAddressPOSIX* otherAddress);
   /// Set the IPv4 address using the explicit bytes.
   void SetIPv4(uchar byteA, uchar byteB, uchar byteC, uchar byteD);
   /// Get the bytes of the IPv4 address.  Return true if successful.
   bool GetIPv4(uchar* byteA, uchar* byteB, uchar* byteC, uchar* byteD);
   /// Set the address to the given hostname.  Return true if successful.
   bool SetByHostname(const char* hostname);

   /// Return true if the address is valid.
   bool ValidCheck() { return valid; }

   /// Return true if the two addresses are equal.
   bool operator==(const IPAddressPOSIX& other) const;
   /// Return true if the two addresses are different.
   bool operator!=(const IPAddressPOSIX& other) const;

protected:
   /// True if the address is valid.
   bool valid;
   /// Bytes that make up the address.
   uchar addressBytes[4];
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__IPADDRESSPOSIX_H__
