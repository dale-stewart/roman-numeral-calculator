#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include "roman.h"

START_TEST(one_plus_one_is_two)
{
    ck_assert_str_eq(roman_add("I", "I"), "II");
}
END_TEST

START_TEST(one_plus_two_is_three)
{
    ck_assert_str_eq(roman_add("I", "II"), "III");
}
END_TEST

START_TEST(two_plus_two_is_four)
{
    ck_assert_str_eq(roman_add("II", "II"), "IV");
}
END_TEST

START_TEST(two_plus_three_is_five)
{
    ck_assert_str_eq(roman_add("II", "III"), "V");
}
END_TEST

START_TEST(three_plus_three_is_six)
{
    ck_assert_str_eq(roman_add("III", "III"), "VI");
}
END_TEST

START_TEST(four_plus_one_is_five)
{
    ck_assert_str_eq(roman_add("IV", "I"), "V");
}
END_TEST

START_TEST(one_plus_four_is_five)
{
    ck_assert_str_eq(roman_add("I", "IV"), "V");
}
END_TEST

START_TEST(fourteen_plus_one_is_fifteen)
{
    ck_assert_str_eq(roman_add("XIV", "I"), "XV");
}
END_TEST

START_TEST(one_plus_fourteen_is_fifteen)
{
    ck_assert_str_eq(roman_add("I", "XIV"), "XV");
}
END_TEST

START_TEST(fourteen_plus_fourteen_is_twenty_eight)
{
    ck_assert_str_eq(roman_add("XIV", "XIV"), "XXVIII");
}
END_TEST

START_TEST(nineteen_plus_nineteen_is_thirty_eight)
{
    ck_assert_str_eq(roman_add("XIX", "XIX"), "XXXVIII");
}
END_TEST

START_TEST(forty_plus_forty_is_eighty)
{
    ck_assert_str_eq(roman_add("XL", "XL"), "LXXX");
}
END_TEST

START_TEST(ninety_plus_ninety_is_one_hundred_eighty)
{
    ck_assert_str_eq(roman_add("XC", "XC"), "CLXXX");
}
END_TEST

Suite * roman_suite(void)
{
    Suite * s;
    TCase * tc;

    s = suite_create("roman");
    tc = tcase_create("roman_add");

    tcase_add_test(tc, one_plus_one_is_two);
    tcase_add_test(tc, one_plus_two_is_three);
    tcase_add_test(tc, two_plus_two_is_four);
    tcase_add_test(tc, two_plus_three_is_five);
    tcase_add_test(tc, three_plus_three_is_six);
    tcase_add_test(tc, four_plus_one_is_five);
    tcase_add_test(tc, one_plus_four_is_five);
    tcase_add_test(tc, fourteen_plus_one_is_fifteen);
    tcase_add_test(tc, one_plus_fourteen_is_fifteen);
    tcase_add_test(tc, fourteen_plus_fourteen_is_twenty_eight);
    tcase_add_test(tc, nineteen_plus_nineteen_is_thirty_eight);
    tcase_add_test(tc, forty_plus_forty_is_eighty);
    tcase_add_test(tc, ninety_plus_ninety_is_one_hundred_eighty);

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
