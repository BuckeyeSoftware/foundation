#include <stdio.h>
#include <stdlib.h>

#include <bes/foundation/memory.h>
#include <bes/foundation/macros.h>
#include <bes/foundation/string.h>

static void *allocate(bes_allocator *allocator, bes_size size)
{
	(void)allocator;
	return malloc(size);
}

static void *reallocate(bes_allocator *allocator, void *const ptr, bes_size size)
{
	(void)allocator;
	return realloc(ptr, size);
}

static void release(bes_allocator *allocator, void *const ptr)
{
	(void)allocator;
	free(ptr);
}

static bes_allocator allocator =
{
	&allocate,
	&reallocate,
	&release,
	0
};

typedef struct test_command test_command;

struct test_command
{
	const char *name;
	bes_bool (*command)(bes_size*, bes_size*);
};

extern bes_bool test_types_command(bes_size*, bes_size*); /* types.c */
extern bes_bool test_bswap_command(bes_size*, bes_size*); /* bswap.c */
extern bes_bool test_memory_command(bes_size*, bes_size*); /* memory.c */
extern bes_bool test_buffer_command(bes_size*, bes_size*); /* buffer.c */
extern bes_bool test_string_command(bes_size*, bes_size*); /* string.c */
extern bes_bool test_stream_command(bes_size*, bes_size*); /* stream.c */

static const test_command test_commands[] =
{
	{ "types", test_types_command },
	{ "bswap", test_bswap_command },
	{ "memory", test_memory_command },
	{ "buffer", test_buffer_command },
	{ "string", test_string_command },
	{ "stream", test_stream_command }
};

int main(int argc, char **argv)
{
	argc--;
	argv++;

	const bes_bool all = argc == 0;

	bes_allocator_set(&allocator);

	bes_size tests = 0;
	bes_size failed = 0;
	int status = EXIT_SUCCESS;
	if (all)
	{
		for (bes_size i = 0; i < BES_ARRAY_SIZE(test_commands); i++)
		{
			bes_size command_tests = 0;
			bes_size command_failed = 0;
			const test_command *const command = &test_commands[i];
			const bes_bool result = command->command(&command_tests, &command_failed);
			if (!result)
			{
				status = EXIT_FAILURE;
			}

			tests += command_tests;
			failed += command_failed;
			printf("\n");
		}
	}
	else
	{
		for (bes_size i = 0; i < BES_ARRAY_SIZE(test_commands); i++)
		{
			const test_command *const command = &test_commands[i];
			if (!bes_strcmp(command->name, *argv))
			{
				const bes_bool result = command->command(&tests, &failed);
				return result ? EXIT_SUCCESS : EXIT_FAILURE;
			}
		}

		fprintf(stderr, "'%s' is not a valid test command\n", *argv);
	}

	if (status)
	{
		printf("\e[1;91mfailed (%zu/%zu)\e[0m\n", failed, tests);
	}
	else
	{
		printf("\e[1;92mpassed (%zu/%zu)\e[0m\n", tests, tests - failed);
	}

	return status;
}
