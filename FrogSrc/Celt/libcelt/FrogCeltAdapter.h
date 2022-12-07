#ifndef __FROG__FROGCELTADAPTER_H__
#define __FROG__FROGCELTADAPTER_H__

#include "UtilityC.h"

EXTERN_C_BEGIN

#define OVERRIDE_CELT_ALLOC_SCRATCH
#define OVERRIDE_CELT_REALLOC
#define OVERRIDE_CELT_FREE
#define OVERRIDE_CELT_FREE_SCRATCH
#define OVERRIDE_CELT_FATAL
#define OVERRIDE_CELT_WARNING
#define OVERRIDE_CELT_WARNING_INT
#define OVERRIDE_CELT_NOTIFY

/** CELT wrapper for calloc(). To do your own dynamic allocation, all you need to do is replace this function, celt_realloc and celt_free 
    NOTE: celt_alloc needs to CLEAR THE MEMORY */
void *celt_alloc (int size);

/** Same as celt_alloc(), except that the area is only needed inside a CELT call (might cause problem with wideband though) */
void *celt_alloc_scratch (int size);

/** CELT wrapper for realloc(). To do your own dynamic allocation, all you need to do is replace this function, celt_alloc and celt_free */
void *celt_realloc (void *ptr, int size);

/** CELT wrapper for free(). To do your own dynamic allocation, all you need to do is replace this function, celt_realloc and celt_alloc */
void celt_free (void *ptr);

/** Same as celt_free(), except that the area is only needed inside a CELT call (might cause problem with wideband though) */
void celt_free_scratch (void *ptr);

void _celt_fatal(const char *str, const char *file, int line);

void celt_warning(const char *str);

void celt_warning_int(const char *str, int val);

void celt_notify(const char *str);

EXTERN_C_END

#endif // #ifndef __FROG__FROGCELTADAPTER_H__
