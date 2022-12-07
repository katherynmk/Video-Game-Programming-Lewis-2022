#ifndef YAJL_VERSION_H_
#define YAJL_VERSION_H_

//FROG_CHANGE_BEGIN
#include "api/yajl_common.h"

//#define YAJL_MAJOR ${YAJL_MAJOR}
//#define YAJL_MINOR ${YAJL_MINOR}
//#define YAJL_MICRO ${YAJL_MICRO}
#define YAJL_MAJOR 2
#define YAJL_MINOR 1
#define YAJL_MICRO 0
//FROG_CHANGE_END

#define YAJL_VERSION ((YAJL_MAJOR * 10000) + (YAJL_MINOR * 100) + YAJL_MICRO)

#ifdef __cplusplus
extern "C" {
#endif

extern int YAJL_API yajl_version(void);

#ifdef __cplusplus
}
#endif

#endif /* YAJL_VERSION_H_ */

