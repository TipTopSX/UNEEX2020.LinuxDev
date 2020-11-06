#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>

static jmp_buf escape;

static void
test_abort(void) {
    longjmp(escape, 1);
}

#define BUF_ABORT test_abort()

#include "buf.h"

#if _WIN32
#  define C_RED(s)     s
#  define C_GREEN(s)   s
#else
#  define C_RED(s)     "\033[31;1m" s "\033[0m"
#  define C_GREEN(s)   "\033[32;1m" s "\033[0m"
#endif

static uint32_t
pcg32(uint64_t *s) {
    uint64_t m = 0x5851f42d4c957f2d;
    uint64_t a = 0x8b260b70b8e98891;
    uint64_t p = *s;
    uint32_t x = ((p >> 18) ^ p) >> 27;
    uint32_t r = p >> 59;
    *s = p * m + a;
    return (x >> r) | (x << (-r & 31u));
}

static unsigned long
bench(uint64_t *rng) {
    unsigned long r = 0;
    uint32_t n = 1000000 + pcg32(rng) % 4000000;
    float *buf = 0;

    for (uint32_t i = 0; i < n; i++)
        buf_push(buf, pcg32(rng) / (double) UINT32_MAX);

    float threshold = pcg32(rng) / (double) UINT32_MAX;
    for (uint32_t i = 0; i < n; i++)
        r += buf[i] > threshold;

    buf_free(buf);
    return r;
}

#ifdef _WIN32
#include <windows.h>
uint64_t
uepoch(void)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /=10;
    tt -= UINT64_C(11644473600000000);
    return tt;
}
#else

#include <sys/time.h>
#include <check.h>

uint64_t
uepoch(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1000000LL * tv.tv_sec + tv.tv_usec;
}

#endif

START_TEST(init_free) {
/* initialization, buf_free() */
    float *a = 0;
    ck_assert(buf_capacity(a) == 0);
    ck_assert(buf_size(a) == 0);
    buf_push(a, 1.3f);
    ck_assert(buf_size(a) == 1);
    ck_assert(a[0] == (float) 1.3f);
    buf_clear(a);
    ck_assert(buf_size(a) == 0);
    ck_assert(a != 0);
    buf_free(a);
    ck_assert(a == 0);
}

END_TEST

START_TEST(clear) {
    float *a = 0;
    /* Clearing an NULL pointer is a no-op */
    buf_clear(a);
    ck_assert(buf_size(a) == 0);
    ck_assert(a == 0);
}

END_TEST

START_TEST(push_get) {
    /* buf_push(), [] operator */
    long *ai = 0;
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
    ck_assert(buf_size(ai) == 10000);
    int match = 0;
    for (int i = 0; i < (int) (buf_size(ai)); i++)
        match += ai[i] == i;
    ck_assert(match == 10000);
    buf_free(ai);
}

END_TEST

START_TEST(grow_trunc) {
    long *ai = 0;
    /* buf_grow(), buf_trunc() */
    buf_grow(ai, 1000);
    ck_assert(buf_capacity(ai) == 1000);
    ck_assert(buf_size(ai) == 0);
    buf_trunc(ai, 100);
    ck_assert(buf_capacity(ai) == 100);
    buf_free(ai);
}

END_TEST

START_TEST(pop) {
    float *a = 0;
    /* buf_pop() */
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
    ck_assert(buf_size(a) == 4);
    ck_assert(buf_pop(a) == (float) 1.4f);
    buf_trunc(a, 3);
    ck_assert(buf_size(a) == 3);
    ck_assert(buf_pop(a) == (float) 1.3f);
    ck_assert(buf_pop(a) == (float) 1.2f);
    ck_assert(buf_pop(a) == (float) 1.1f);
    ck_assert(buf_size(a) == 0);
    buf_free(a);
}

END_TEST

START_TEST(out_of_mem) {
    /* Memory allocation failures */
    volatile int aborted;

    {
        int *volatile p = 0;
        aborted = 0;
        if (!setjmp(escape)) {
            size_t max = (PTRDIFF_MAX - sizeof(struct buf)) / sizeof(*p) + 1;
            buf_grow(p, max);
            buf_grow(p, max);
        } else {
            aborted = 1;
        }
        buf_free(p);
        ck_assert(aborted);
    }
}

END_TEST

START_TEST(overfow_init) {
    volatile int aborted;

    int *volatile p = 0;
    aborted = 0;
    if (!setjmp(escape)) {
        buf_trunc(p, PTRDIFF_MAX);
    } else {
        aborted = 1;
    }
    buf_free(p);
    ck_assert(aborted);
}

END_TEST

START_TEST(overflow_grow) {
    volatile int aborted;

    int *volatile p = 0;
    aborted = 0;
    if (!setjmp(escape)) {
        buf_trunc(p, 1); /* force realloc() use next */
        buf_trunc(p, PTRDIFF_MAX);
    } else {
        aborted = 1;
    }
    buf_free(p);
    ck_assert(aborted);
}

END_TEST

START_TEST(bench_test) {
    uint64_t rng = 0x01;
    unsigned long r = 0;
    uint64_t start = uepoch();
    for (int i = 0; i < 10; i++)
        r += bench(&rng);
    double t = (uepoch() - start) / 1e6;
    printf("%.6gs : acutal %lu, expect 428859598\n", t, r);
}

END_TEST

Suite *buf_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("buf");

    /* Core test case */
    tc_core = tcase_create("core");
    tcase_add_test(tc_core, init_free);
    tcase_add_test(tc_core, clear);
    tcase_add_test(tc_core, push_get);
    tcase_add_test(tc_core, grow_trunc);
    tcase_add_test(tc_core, pop);
    tcase_add_test(tc_core, out_of_mem);
    tcase_add_test(tc_core, overfow_init);
    tcase_add_test(tc_core, overflow_grow);
    tcase_add_test(tc_core, bench_test);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = buf_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}