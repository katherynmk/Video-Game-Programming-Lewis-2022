#ifndef __CUSTOM_SUPPORT_H__
#define __CUSTOM_SUPPORT_H__

#define restrict
#ifndef __cplusplus
   #define inline
#endif

#if OVERRIDE_CELT_ALLOC
#include "FrogCeltAdapter.h"
#endif

#endif //#ifndef __CUSTOM_SUPPORT_H__
