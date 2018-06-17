#ifndef BES_FOUNDATION_STRING_H
#define BES_FOUNDATION_STRING_H

/**
 * @defgroup String String
 *
 * @brief Optimized string functions
 *
 * @{
 */

#include <bes/foundation/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Copy block of memory
 *
 * Copies the values of @p n bytes from the location pointed by @p src
 * directly to the memory block pointed to by @p dst.
 *
 * @param dst The destination
 * @param src The source
 * @param n The amount of bytes to copy
 *
 * @return The destination is returned
 */
BES_EXPORT void* BES_API
bes_memcpy(void *BES_RESTRICT dst, const void *BES_RESTRICT src, bes_size n);

/**
 * @brief Compare the first @p n bytes of memory
 *
 * Compares the first @p n bytes (each intepreted as @ref bes_byte)
 * of the memory areas @p vl and @p vr.
 *
 * @param vl first block of memory
 * @param vr second block of memory
 * @param n bytes to compare
 *
 * @return An integer less than, equal to, or greater than zero if the
 * first @p n bytes of @p vl is found, respectively, to be less than,
 * to match, or be greater than the first @p n bytes of @p vr.
 *
 * For nonzero return value, the sign is determined by the sign of the
 * difference between the first pair of bytes (intepreted as @ref bes_byte)
 * that differ in @p vl and @p vr.
 *
 * If @p n is zero, the return value is zero
 */
BES_EXPORT int BES_API
bes_memcmp(const void *vl, const void *vr, bes_size n);

/**
 * @brief Fill block of memory
 *
 * Sets the first @p n bytes of the block of memory pointed by @p dst
 * to the specified value @p c interpreted as a bes_byte.
 *
 * @param dst Pointer to the block of memory to fill
 * @param c Value to be set. The value is passed as an int, but the function
 * fills the block of memory using the bes_byte conversion of this value.
 * @param n Number of bytes to set to the value.
 *
 * @return The destination is returned
 */
BES_EXPORT void* BES_API
bes_memset(void *dst, int c, bes_size n);

/**
 * @brief Convert UTF-8 string to UTF-16
 *
 * @param source The source string to convert
 * @param destination_ The converted string is stored here
 *
 * @warning This function returns allocated memory in @p destination_,
 * which must be freed by @ref bes_free.
 *
 * @return On success this function retrusn @ref BES_TRUE.
 */
BES_EXPORT bes_bool BES_API
bes_utf8_to_utf16(const char *source,
                    bes_wchar **const destination_);

/**
 * @brief Convert UTF-16 string to UTF-8
 *
 * @param source The source string to convert
 * @param destination_ The converted string is stored here
 *
 * @warning This function returns allocated memory in @p destination_,
 * which must be freed by @ref bes_free.
 *
 * @return On success this function retrusn @ref BES_TRUE.
 */
BES_EXPORT bes_bool BES_API
bes_utf16_to_utf8(const bes_wchar *contents,
                    char **const destination_);

/**
 * @brief Calculate the length of a string
 *
 * Calculates the length og the string pointed to by @p str, excluding
 * the null byte '\0'.
 *
 * @param str the string
 *
 * @return returns the number of characters in the string pointed to by
 * @p str.
 */
BES_EXPORT bes_size BES_API
bes_strlen(const char *str);

/**
 * @brief Compare two strings
 *
 * Compares two strings @p lhs and @p rhs. It returns an integer less
 * than, equal to, or greater than zero is @p lhs is found, respectively,
 * to be less than, to match, or be greater than @p rhs.
 *
 * @param lhs first string
 * @param rhs second string
 *
 * @return an integer less than, equal to, or greater than zero if @p lhs
 * is found, respectively, to be less than, to match, or be greater than
 * @p rhs.
 */
BES_EXPORT int BES_API
bes_strcmp(const char *lhs, const char *rhs);

/**
 * @brief Locate character in string
 *
 * Like @ref bes_strchr except that if @p c is not found in @p s, then
 * returns a pointer to the null byte at the end of @p s, rather than NULL.
 *
 * @p s the string to search in
 * @p c the character to search for
 *
 * @return pointer to the matches character, or a pointer to the null byte
 * at the end of @p s (i.e, s+bes_strlen(s)) if the character is not
 * found.
 */
BES_EXPORT char* BES_API
bes_strchrnul(const char *s, int c);

/**
 * @brief Locate character in string
 *
 * Returns a pointer to the first occurance of the character @p c in the
 * string @p s.
 *
 * @param s the string to search in
 * @param c the character to search for
 *
 * @return pointer to the matched character or NULL if the character
 * is not found. The terminating null byte is considered part of the string,
 * so that if @p c is specified as '\0', this function can return a pointer
 * to the terminator.
 */
BES_EXPORT char* BES_API
bes_strchr(const char *s, int c);

/**
 * @brief Get the length of a prefix substring
 *
 * Calculates the length of the initial segment of @p s which consists
 * entierly of bytes not in @p c.
 *
 * @param s string
 * @param c the reject string
 *
 * @return The number of bytes in the initial segment of @p s which are
 * not in the string @p c.
 */
BES_EXPORT bes_size BES_API
bes_strcspn(const char *s, const char *c);

/**
 * @brief Get the length of a prefix substring
 *
 * Calculates the length of the initial segment of @p s which consists
 * entierly of bytes in @p c.
 *
 * @param s string
 * @param c the reject string
 *
 * @return The number of bytes in the initial segment of @p s which
 * consists only of bytes from @p c.
 */
BES_EXPORT bes_size BES_API
bes_strspn(const char *s, const char *c);

BES_EXPORT char* BES_API
bes_strtok(char *s, const char *sep, char **p);

BES_EXPORT bes_f32 BES_API
bes_atof(const char *string);

BES_EXPORT int BES_API
bes_atoi(const char *string);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif
