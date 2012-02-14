#ifndef _TEST_H
#define _TEST_H

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED 
#endif

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
void log_proto(char *s);

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
