#ifndef BES_FOUNDATION_TEST_H
#define BES_FOUNDATION_TEST_H

#include <bes/foundation/string.h>
#include <bes/foundation/macros.h>

typedef struct bes_test_entry bes_test_entry;

struct bes_test_entry
{
	const char *name;
	bes_bool (*function)(void);
};

#define BES_DEFINE_TEST(NAME) \
	static bes_bool NAME(void)

#define BES_DEFINE_TEST_LIST(NAME) \
	static const bes_test_entry NAME[] =

#define BES_ADD_TEST(NAME) \
	{ #NAME, &(NAME) }

#define BES_DEFINE_TEST_COMMAND(NAME, DESCRIPTION, TESTS, PRINTER) \
	bes_bool NAME(bes_size *tests, bes_size *failed) \
	{ \
		static const bes_size k_alignment = 70; \
		(*tests) = BES_ARRAY_SIZE(TESTS); \
		char buffer[1024]; \
		(PRINTER)("\e[1;37mrunning tests for "); \
		(PRINTER)((DESCRIPTION)); \
		(PRINTER)("\e[0m\n"); \
		bes_bool status = BES_TRUE; \
		for (bes_size i = 0; i < BES_ARRAY_SIZE(TESTS); i++) \
		{ \
			const bes_test_entry *const entry = &(TESTS)[i]; \
			/* Replace '_' with ' ' in entry name */ \
			bes_strcpy(buffer, entry->name); \
			char *current = buffer; \
			current = bes_strchr(current, '_'); \
			while (current) \
			{ \
				*current = ' '; \
				current = bes_strchr(current, '_'); \
			} \
			const bes_size size = bes_strlen(buffer); \
			(PRINTER)("  \e[35m "); \
			(PRINTER)(buffer); \
			if (size <= k_alignment) \
			{ \
				for (bes_size align = 0; align < k_alignment - size; align++) \
				{ \
					(PRINTER)(" "); \
				} \
			} \
			(PRINTER)("\e[0m "); \
			const bes_bool result = entry->function(); \
			if (result) \
			{ \
				(PRINTER)("\e[32mpassed\e[0m\n"); \
			} \
			else \
			{ \
				(PRINTER)("\e[31mfailed\e[0m\n"); \
				(*failed)++; \
				status = BES_FALSE; \
			} \
		} \
		return status; \
	}

#endif
