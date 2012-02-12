#ifndef _TEST_H
#define _TEST_H

#include <stddef.h>

#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED 
#endif

UNUSED static char *current_test = NULL;

#define TEST(name)                                                             \
static void test__##name(void);                                                \
static void test_##name(void) {                                                \
    current_test = #name;                                                      \
    test__##name();                                                            \
    printf("%s: PASSED\n", current_test);                                      \
}                                                                              \
static void test__##name(void)

#endif
