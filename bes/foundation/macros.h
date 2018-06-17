#ifndef BES_FOUNDATION_MACROS_H
#define BES_FOUNDATION_MACROS_H

#define BES_PP_CONCATENATE(TOKEN1, TOKEN2) \
	TOKEN1 ## TOKEN2

#define BES_PP_PASTE(TOKEN1, TOKEN2) \
	BES_PP_CONCATENATE(TOKEN1, TOKEN2)

#define BES_STATIC_ASSERT(EXPRESSION) \
	typedef int BES_PP_PASTE(bes_static_assert_, __LINE__)[(EXPRESSION) ? 1 : -1]

#define BES_ARRAY_SIZE(ARRAY) \
	(sizeof(ARRAY) / sizeof (ARRAY)[0])

/* Compiler hint to indicate alignment of a pointer. This permits the
 * compiler to produce more optimal load and stores using aligned
 * instructions where they would otherwise not be permitted. Pluck
 * heavily ensures memory for operations is always aligned on a byte
 * boundary specified by PLUCK_ALIGNED. This hint helps with ensuring
 * the compiler is aware of that alignment. */
#if defined(BES_COMPILER_GCC) || defined(BES_COMPILER_CLANG)
#define BES_ASSUME_ALIGNED(TYPE, POINTER, ALIGNMENT) \
	((POINTER) = (TYPE)__builtin_assume_aligned((POINTER), (ALIGNMENT)))
#elif defined(BES_COMPILER_MSVC)
#define BES_ASSUME_ALIGNED(TYPE, POINTER, ALIGNMENT) \
	__assume((bes_uintptr)(POINTER) % (ALIGNMENT) == 0)
#else
#define BES_ASSUME_ALIGNED(TYPE, POINTER, ALIGNMENT) \
	(void)0
#endif

/* Compiler hint to indicate the likeliness of something. This permits
 * the compiler to produce more optimal branching code and keep the
 * more likely stuff closer for more effective use of icache. */
#if defined(BES_COMPILER_GCC) || defined(BES_COMPILER_CLANG)
#define BES_LIKELY(EXPRESSION) \
	(__builtin_expect(!!(EXPRESSION), 1))
#define BES_UNLIKELY(EXPRESSION) \
	(__builtin_expect(!!(EXPRESSION), 0))
#else
#define BES_LIKELY(EXPRESSION) (EXPRESSION)
#define BES_UNLIKELY(EXPRESSION) (EXPRESSION)
#endif

/* Compiler hint to indicate that a function is pure. This permits
 * the compiler to apply optimizations like subexpression elimination
 * and loop optimizations in cases where it otherwise can't. Functions
 * are pure, if and only if the function has no side effects except the
 * return value which only depends on parameters. */
#if defined(BES_COMPILER_GCC) || defined(BES_COMPILER_CLANG)
#define BES_PURE \
	__attribute__((pure))
#else
#define BES_PURE
#endif

#endif
