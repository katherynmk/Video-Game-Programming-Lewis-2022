#ifndef __FROG__VIDEOSTREAM_H__
#define __FROG__VIDEOSTREAM_H__

#include "FrogMemory.h"
#include "VideoStreamDefault.h"

namespace Webfoot {

/// Select the implementation of VideoStream to use here.
typedef VideoStreamDefault VideoStream;

} // namespace Webfoot {

#endif //#ifdef __FROG__VIDEOSTREAM_H__
