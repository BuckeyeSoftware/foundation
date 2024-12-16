#ifndef BES_FOUNDATION_CONFIG_H
#define BES_FOUNDATION_CONFIG_H

/* Determine compiler */
#if defined(_MSC_VER)           /* Defined by Visual Studio */
#define BES_COMPILER_MSVC
#elif defined(__clang__)        /* Defined by Clang */
#define BES_COMPILER_CLANG
#elif defined(__SUNPRO_C)       /* Defined by Sun Studio (C compiler) */ \
 ||   defined(__SUNPRO_CC)      /* Defined by Sun Studio (C++ compiler) */
#define BES_COMPILER_SUN
#elif defined(__INTEL_COMPILER) /* Defined by Intel */
#define BES_COMPILER_INTEL
#elif defined(__GNUC__)         /* Defined by GCC and Clang */
#define BES_COMPILER_GCC
#elif defined(__SNC__)          /* Defined by SNC */
#define BES_COMPILER_SNC
#else
#error Unrecognized or unsupported compiler.
#endif

/* Determine architecture */
#if defined(__i386__)           /* Defined by GCC, Clang and Sun Studio */ \
 || defined(__i386)             /* Defined by GCC, Clang and Sun Studio */ \
 || defined(__x86__)            /* Defined by GCC, Clang and Sun Studio */ \
 || defined(__x86)              /* Defined by Visual Studio */ \
 || defined(_M_IX86)            /* Defined by Visual Studio */
#define BES_ARCH_X86
#elif defined(__amd64__)        /* Defined by GCC, Clang and Sun Studio */ \
 ||   defined(__amd64)          /* Defined by GCC, Clang and Sun Studio */ \
 ||   defined(__x86_64__)       /* Defined by GCC, Clang and Sun Studio */ \
 ||   defined(__x86_64)         /* Defined by GCC, Clang and Sun Studio */ \
 ||   defined(_M_X64)           /* Defined by Visual Studio */ \
 ||   defined(_M_AMD64)         /* Defined by Visual Studio */
#define BES_ARCH_X86_64
#elif defined(__aarch64__)      /* Defined by GCC and Clang */
#define BES_ARCH_ARM64
#else
#error Unrecognized or unsupported architecture.
#endif

/* Determine platform */
#if defined(_WIN32)             /* Defined for both 32-bit and 64-bit environments */ \
 || defined(_WIN64)             /* Defined for 64-bit environments */
#define BES_PLATFORM_WINDOWS
#elif defined(__linux__)        /* Defined by all Linux environments */
#define BES_PLATFORM_LINUX
#elif defined(__SCE__)          /* Defined by SCE SDK */ \
  &&  defined(__arm__)          /* Defined by compiler */
#define BES_PLATFORM_VITA
#elif defined(__ORBIS__)        /* Defined by SNC PS4 TC and SCE SDK */
#define BES_PLATFORM_PS4
#elif defined(NN_PLATFORM_CTR)  /* Defined by compiler */
#define BES_PLATFORM_3DS
#elif defined(__NX)             /* Defined by compiler */
#define BES_PLATFORM_SWITCH
#elif defined(ANDROID)          /* Defined by some compilers */ \
  ||  defined(__ANDROID__)      /* Defined by some compilers */
#define BES_PLATFORM_ANDROID
#elif defined(_XBOX_ONE)        /* Defined by XSDK */
#define BES_PLATFORM_XBOXONE
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC               /* Defined by TargetConditionals.h */
#define BES_PLATFORM_MAC
#elif TARGET_OS_IOS             /* Defined by TargetConditionals.h */
#define BES_PLATFORM_IOS
#endif
#else
#error Unrecognized or unsupported platform.
#endif

/* Determine data model */
#if defined(_ILP32)                 /* Defined by HP aCC and Sun Studio */ \
 || defined(__ILP32__)              /* Defined by GCC and Clang */ \
 || defined(BES_PLATFORM_WINDOWS) /* Windows is always going to be ILP32 */ \
 || defined(BES_PLATFORM_XBOXONE) /* XboxOne is also Windows */
#define BES_ILP32
#elif defined(_LP64)                /* Defined by HP aCC and Sun Studio */ \
 ||   defined(__LP64__)             /* Defined by GCC and Clang */
#define BES_LP64
#else
#error Unrecognized or unsupported data model.
#endif

/* Type large enough to hold a pointer */
#if defined(BES_ILP32)
#define BES_ADDR long long
#else
#define BES_ADDR long
#endif

/* Various compiler specific attributes handled here */
#if defined(BES_COMPILER_GCC) \
 || defined(BES_COMPILER_CLANG) \
 || defined(BES_COMPILER_SNC)
#define BES_ATTRIBUTE_ALIGN(a) \
	__attribute__((__aligned__(a)))
#define BES_ATTRIBUTE_ALWAYS_INLINE \
	__attribute__((__always_inline__)) inline
#define BES_RESTRICT \
	restrict
#elif defined(BES_COMPILER_MSVC) || defined(BES_COMPILER_INTEL)
#define BES_ATTRIBUTE_ALIGN(a) \
	__declspec(align(a))
#define BES_ATTRIBUTE_ALWAYS_INLINE \
	__forceinline
#define BES_RESTRICT \
	__restrict
#endif

#define BES_ALIGNMENT 16
#define BES_EXPORT
#define BES_API
#define BES_ASSERT(...)

#endif
