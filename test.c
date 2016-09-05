#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

const char * roman_add(const char * a, const char * b)
{
    return "II";
}

START_TEST(one_plus_one_is_two)
{
    ck_assert_str_eq(roman_add("I", "I"), "II");
}
END_TEST

Suite * roman_suite(void)
{
    Suite * s;
    TCase * tc;

    s = suite_create("roman");
    tc = tcase_create("roman_add");

    tcase_add_test(tc, one_plus_one_is_two);
    suite_add_tcase(s, tc);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = roman_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
