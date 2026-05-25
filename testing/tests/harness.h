#ifndef HARNESS_H
#define HARNESS_H

#define TEST(name, expr)                                        \
    do {                                                        \
      tests_run++;                                              \
      if (expr) {                                               \
        printf("PASS  %s\n", name);                             \
        tests_passed++;                                         \
      } else {                                                  \
        printf("FAIL  %s  (line %d)\n", name, __LINE__);        \
        tests_failed++;                                         \
        test_abort();                                           \
      }                                                         \
    } while (0)

/*
 * SKIP(fn_name) -- use instead of calling a broken function directly.
 * Prints a SKIP line and jumps to the label 'fn_skipped' in the enclosing
 * function, which should sit just before the closing brace.
 */
#define SKIP(fn_name)                                                   \
    do {                                                                \
      tests_skipped++;                                                  \
      printf("SKIP  %s (known broken)\n", #fn_name);                    \
      goto fn_skipped;                                                  \
    } while (0)

#define SECTION(title) printf("\n--- %s ---\n", title)

// Suppress cc65 warning about no goto
#define END_OF_TEST()                           \
  goto fn_skipped;                              \
fn_skipped:                                     \
 return;

extern int tests_run, tests_passed, tests_skipped, tests_failed;

extern void end_testing(int code);
extern void test_abort(void);

#endif /* HARNESS_H */
