#include <bes/foundation/test.h>
#include <bes/foundation/string.h>
#include <bes/foundation/memory.h>

BES_DEFINE_TEST(bes_memcpy_returns_destination)
{
	return bes_memcpy(0, 0, 0) == 0;
}

BES_DEFINE_TEST(bes_memcpy_copies_memory)
{
	int a = 1024;
	int b;
	bes_memcpy(&b, &a, sizeof b);
	return a == b;
}

BES_DEFINE_TEST(bes_memset_returns_destination)
{
	return bes_memset(0, 0, 0) == 0;
}

BES_DEFINE_TEST(bes_memset_sets_memory)
{
	bes_u32 a;
	bes_memset(&a, 0xAA, sizeof a);
	return a == 0xAAAAAAAA;
}

BES_DEFINE_TEST(utf8_to_utf16_back_to_utf8_is_same)
{
	bes_wchar *utf16_result = 0;
	char *utf8_result = 0;
	bes_utf8_to_utf16("hello", &utf16_result);
	bes_utf16_to_utf8(utf16_result, &utf8_result);
	const bes_bool result =
		utf8_result[0] == 'h' &&
		utf8_result[1] == 'e' &&
		utf8_result[2] == 'l' &&
		utf8_result[3] == 'l' &&
		utf8_result[4] == 'o';
	bes_free(utf16_result);
	bes_free(utf8_result);
	return result;
}

BES_DEFINE_TEST(utf16_to_utf8_back_to_utf16_is_same)
{
	const bes_wchar hello[] = { 0x0068, 0x0065, 0x006c, 0x006c, 0x006f, 0 };
	char *utf8_result = 0;
	bes_wchar *utf16_result = 0;
	bes_utf16_to_utf8(hello, &utf8_result);
	bes_utf8_to_utf16(utf8_result, &utf16_result);
	const bes_bool result =
		utf16_result[0] == hello[0] &&
		utf16_result[1] == hello[1] &&
		utf16_result[2] == hello[2] &&
		utf16_result[3] == hello[3] &&
		utf16_result[4] == hello[4];
	bes_free(utf8_result);
	bes_free(utf16_result);
	return result;
}

BES_DEFINE_TEST(bes_strlen_for_empty_string_is_zero)
{
	return bes_strlen("") == 0;
}

BES_DEFINE_TEST(bes_strlen_is_string_length)
{
	const char hello[] = "hello";
	return bes_strlen(hello) == sizeof hello - 1;
}

BES_DEFINE_TEST(bes_strcmp_0_for_empty_strings)
{
	return bes_strcmp("", "") == 0;
}

BES_DEFINE_TEST(bes_strcmp_0_for_same_strings)
{
	return bes_strcmp("hello", "hello") == 0;
}

BES_DEFINE_TEST(bes_strcmp_lt0_for_first_non_matching_lower_value_in_lhs)
{
	return bes_strcmp("a", "z") < 0;
}

BES_DEFINE_TEST(bes_strcmp_gt0_for_first_non_matching_greater_value_in_lhs)
{
	return bes_strcmp("z", "a") > 0;
}

BES_DEFINE_TEST(bes_strchrnul_not_found_points_to_null_byte)
{
	const char hi[] = "hi";
	return bes_strchrnul(hi, 'c') == hi + 2;
}

BES_DEFINE_TEST(bes_strchrnul_found_points_to_first_found_character)
{
	const char hi[] = "hihi";
	return bes_strchrnul(hi, 'i') == hi + 1;
}

BES_DEFINE_TEST(bes_strchr_not_found_returns_null)
{
	return bes_strchr("hi", 'c') == 0;
}

BES_DEFINE_TEST(bes_strchr_found_points_to_first_found_character)
{
	const char hi[] = "hihi";
	return bes_strchr(hi, 'i') == hi + 1;
}

BES_DEFINE_TEST(bes_strcspn_without_match_is_full_string_length)
{
	const char full[] = "this string contains no numbers";
	return bes_strcspn(full, "1234567890") == sizeof full - 1;
}

BES_DEFINE_TEST(bes_strcspn_with_match_is_span_before_match)
{
	const char full[] = "this 1 string contains a number"; /* exists at position 5 */
	return bes_strcspn(full, "1234567890") == 5;
}

BES_DEFINE_TEST(bes_strspn_with_match_is_match_count)
{
	const char full[] = "128th";
	return bes_strspn(full, "1234567890") == 3;
}

BES_DEFINE_TEST(bes_strspn_without_match_is_zero)
{
	const char full[] = "hello";
	return bes_strspn(full, "1234567890") == 0;
}

BES_DEFINE_TEST(bes_strtok_returns_first_token_when_found)
{
	char data[] = "1+2-3";
	char *rest = data;
	char *token = bes_strtok(rest, "+-", &rest);
	return token == data && token[1] == '\0'; /* modifies contents to make token, should terminate string */
}

BES_DEFINE_TEST(bes_strtok_returns_beginning_when_not_found_first_time)
{
	char data[] = "1+2-3";
	char *save;
	char *token = bes_strtok(data, "*/", &save);
	return token == data;
}

BES_DEFINE_TEST(bes_strtok_returns_null_when_not_found_after_first_time)
{
	char data[] = "1+2-3";
	char *save;
	bes_strtok(data, "*/", &save);
	return !bes_strtok(0, "*/", &save);
}

BES_DEFINE_TEST(bes_atoi_zero_is_zero)
{
	return bes_atoi("0") == 0;
}

BES_DEFINE_TEST(bes_atoi_positive_zero_is_zero)
{
	return bes_atoi("+0") == 0;
}

BES_DEFINE_TEST(bes_atoi_negative_zero_is_zero)
{
	return bes_atoi("-0") == 0;
}

BES_DEFINE_TEST(bes_atoi_one_is_one)
{
	return bes_atoi("1") == 1;
}

BES_DEFINE_TEST(bes_atoi_negative_one_is_negative_one)
{
	return bes_atoi("-1") == -1;
}

BES_DEFINE_TEST(bes_atoi_spaces_before_ignored)
{
	return bes_atoi("  100") == 100;
}

BES_DEFINE_TEST(bes_atoi_spaces_after_terminate)
{
	return bes_atoi("100 200") == 100;
}

BES_DEFINE_TEST(bes_atoi_spaces_inbetween_sign_ignored)
{
	return bes_atoi(" + 100") == 100 &&
	       bes_atoi(" - 100") == -100;
}

BES_DEFINE_TEST(bes_atof_zero_is_zero)
{
	return bes_atof("0.0") == 0.0f;
}

BES_DEFINE_TEST(bes_atof_positive_zero_is_zero)
{
	return bes_atof("+0.0") == 0.0f;
}

BES_DEFINE_TEST(bes_atof_negative_zero_is_zero)
{
	return bes_atof("-0.0") == -0.0f;
}

BES_DEFINE_TEST(bes_atof_one_is_one)
{
	return bes_atof("1.0") == 1.0f;
}

BES_DEFINE_TEST(bes_atof_negative_one_is_negative_one)
{
	return bes_atof("-1.0") == -1.0;
}

BES_DEFINE_TEST(bes_atof_spaces_before_ignored)
{
	return bes_atof("  100.0") == 100.0f;
}

BES_DEFINE_TEST(bes_atof_spaces_after_terminate)
{
	return bes_atof("100.0 200.0") == 100.0f;
}

BES_DEFINE_TEST(bes_atof_spaces_inbetween_sign_ignored)
{
	return bes_atof(" + 100.0") ==  100.0f &&
	       bes_atof(" - 100.0") == -100.0f;
}

BES_DEFINE_TEST(bes_atof_starts_with_dot_treated_as_zero)
{
	return bes_atof(".5") == 0.5f;
}

BES_DEFINE_TEST(bes_atof_terminates_upon_encountering_second_dot)
{
	return bes_atof(".192.168.0.1") == 0.192f;
}

BES_DEFINE_TEST(bes_atof_spaces_after_dot_is_zero)
{
	return bes_atof(". 100") == 0.0f;
}

BES_DEFINE_TEST(bes_atof_without_dot)
{
	return bes_atof("100") == 100.0f;
}

BES_DEFINE_TEST(bes_atof_with_sign_then_dot)
{
	return bes_atof("-.5") == -0.5f &&
	       bes_atof("+.5") ==  0.5f;
}

BES_DEFINE_TEST(bes_atof_with_sign_then_space_then_dot)
{
	return bes_atof("-   .5") == -0.5f &&
	       bes_atof("+   .5") ==  0.5f;
}

BES_DEFINE_TEST_LIST(string_tests)
{
	BES_ADD_TEST(bes_memcpy_returns_destination),
	BES_ADD_TEST(bes_memcpy_copies_memory),
	BES_ADD_TEST(bes_memset_returns_destination),
	BES_ADD_TEST(bes_memset_sets_memory),
	BES_ADD_TEST(utf8_to_utf16_back_to_utf8_is_same),
	BES_ADD_TEST(utf16_to_utf8_back_to_utf16_is_same),
	BES_ADD_TEST(bes_strlen_for_empty_string_is_zero),
	BES_ADD_TEST(bes_strlen_is_string_length),
	BES_ADD_TEST(bes_strcmp_0_for_empty_strings),
	BES_ADD_TEST(bes_strcmp_0_for_same_strings),
	BES_ADD_TEST(bes_strcmp_lt0_for_first_non_matching_lower_value_in_lhs),
	BES_ADD_TEST(bes_strcmp_gt0_for_first_non_matching_greater_value_in_lhs),
	BES_ADD_TEST(bes_strchrnul_not_found_points_to_null_byte),
	BES_ADD_TEST(bes_strchrnul_found_points_to_first_found_character),
	BES_ADD_TEST(bes_strchr_not_found_returns_null),
	BES_ADD_TEST(bes_strchr_found_points_to_first_found_character),
	BES_ADD_TEST(bes_strcspn_without_match_is_full_string_length),
	BES_ADD_TEST(bes_strcspn_with_match_is_span_before_match),
	BES_ADD_TEST(bes_strspn_with_match_is_match_count),
	BES_ADD_TEST(bes_strspn_without_match_is_zero),
	BES_ADD_TEST(bes_strtok_returns_first_token_when_found),
	BES_ADD_TEST(bes_strtok_returns_beginning_when_not_found_first_time),
	BES_ADD_TEST(bes_strtok_returns_null_when_not_found_after_first_time),
	BES_ADD_TEST(bes_atoi_zero_is_zero),
	BES_ADD_TEST(bes_atoi_positive_zero_is_zero),
	BES_ADD_TEST(bes_atoi_negative_zero_is_zero),
	BES_ADD_TEST(bes_atoi_one_is_one),
	BES_ADD_TEST(bes_atoi_negative_one_is_negative_one),
	BES_ADD_TEST(bes_atoi_spaces_before_ignored),
	BES_ADD_TEST(bes_atoi_spaces_after_terminate),
	BES_ADD_TEST(bes_atoi_spaces_inbetween_sign_ignored),
	BES_ADD_TEST(bes_atof_zero_is_zero),
	BES_ADD_TEST(bes_atof_positive_zero_is_zero),
	BES_ADD_TEST(bes_atof_negative_zero_is_zero),
	BES_ADD_TEST(bes_atof_one_is_one),
	BES_ADD_TEST(bes_atof_negative_one_is_negative_one),
	BES_ADD_TEST(bes_atof_spaces_before_ignored),
	BES_ADD_TEST(bes_atof_spaces_after_terminate),
	BES_ADD_TEST(bes_atof_spaces_inbetween_sign_ignored),
	BES_ADD_TEST(bes_atof_starts_with_dot_treated_as_zero),
	BES_ADD_TEST(bes_atof_terminates_upon_encountering_second_dot),
	BES_ADD_TEST(bes_atof_spaces_after_dot_is_zero),
	BES_ADD_TEST(bes_atof_without_dot),
	BES_ADD_TEST(bes_atof_with_sign_then_dot),
	BES_ADD_TEST(bes_atof_with_sign_then_space_then_dot)
};

#include <stdio.h>
BES_DEFINE_TEST_COMMAND(test_string_command, "string", string_tests, printf)
