#if defined (RCNWINDOWS)

#if defined(_WIN32) && !defined(WIN32)
	#define WIN32
#endif

#define OPT_GENERIC

#define ASMALIGN_BYTE 1

#define strcasecmp _strcmpi
#define strncasecmp _strnicmp
#define strdup _strdup

#define HAVE_STRERROR 1
#define HAVE_SYS_TYPES_H 1

#define HAVE_STRDUP
#define HAVE_STDLIB_H
#define HAVE_STRING_H

/* We want some frame index, eh? */
#define FRAME_INDEX 1
#define INDEX_SIZE 1000

/* also gapless playback! */
#define GAPLESS 1
/* #ifdef GAPLESS
#undef GAPLESS
#endif */

// #define DEBUG 1
// #define EXTRA_DEBUG

#define REAL_IS_FLOAT

#define inline __inline

/* we are on win32 */
#define HAVE_WINDOWS_H

/* use the unicode support within libmpg123 */
#ifdef UNICODE
	#define WANT_WIN32_UNICODE
#endif

#elif defined(RCNIOS)

#include <TargetConditionals.h>


#define HAVE_STRERROR 1
#define HAVE_SYS_TYPES_H 1

#define HAVE_STRDUP
#define HAVE_STDLIB_H
#define HAVE_STRING_H

/* We want some frame index, eh? */

#define FRAME_INDEX 1
#define INDEX_SIZE 1000

/* also gapless playback! */

#define GAPLESS 1
//#define NO_8BIT
//#define NO_32BIT
//#define NO_REAL
//#define NO_NTOM
//#define NO_LAYER1
//#define NO_LAYER2
//#define NO_LAYER12
//#define NO_DOWNSAMPLE
//#define NO_ERROR
//#define NO_WARNING
//#define NO_ICY
//#define NO_ID3V2
//#define NO_STRING

#define REAL_IS_FLOAT

#if TARGET_IPHONE_SIMULATOR
	#define OPT_GENERIC
#elif TARGET_OS_IPHONE
	#ifdef __ARM_NEON__
		#define OPT_NEON
	#else
		#define OPT_GENERIC
	#endif
#endif

// OLLY: Disabled neon intrinsics for now. For some reason the neon assembly files
// seem to be missing :( - FIX THIS

#undef OPT_NEON
#define OPT_GENERIC

#elif defined(RCNANDROID)

#define HAVE_STRERROR 1
#define HAVE_SYS_TYPES_H 1

#define HAVE_STRDUP
#define HAVE_STDLIB_H
#define HAVE_STRING_H

/* We want some frame index, eh? */
#define FRAME_INDEX 1
#define INDEX_SIZE 1000

/* also gapless playback! */
#define GAPLESS 1
//#define NO_8BIT
//#define NO_32BIT
//#define NO_REAL
//#define NO_NTOM
//#define NO_LAYER1
//#define NO_LAYER2
//#define NO_LAYER12
//#define NO_DOWNSAMPLE
//#define NO_ERROR
//#define NO_WARNING
//#define NO_ICY
//#define NO_ID3V2
//#define NO_STRING

#define OPT_GENERIC
#define REAL_IS_FLOAT

#endif