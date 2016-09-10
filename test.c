#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include "roman.h"

#define ASSERT_ROMAN_ADD_EQ(a, b, expected) \
    { \
        char result[ROMAN_SIZE]; \
        ck_assert_str_eq(roman_add(a, b, result), expected); \
    }

#define ASSERT_ROMAN_SUBTRACT_EQ(a, b, expected) \
    { \
        char result[ROMAN_SIZE]; \
        ck_assert_str_eq(roman_subtract(a, b, result), expected); \
    }

START_TEST(one_plus_one_is_two)
{
    ASSERT_ROMAN_ADD_EQ("I", "I", "II");
}
END_TEST

START_TEST(one_plus_two_is_three)
{
    ASSERT_ROMAN_ADD_EQ("I", "II", "III");
}
END_TEST

START_TEST(two_plus_two_is_four)
{
    ASSERT_ROMAN_ADD_EQ("II", "II", "IV");
}
END_TEST

START_TEST(two_plus_three_is_five)
{
    ASSERT_ROMAN_ADD_EQ("II", "III", "V");
}
END_TEST

START_TEST(three_plus_three_is_six)
{
    ASSERT_ROMAN_ADD_EQ("III", "III", "VI");
}
END_TEST

START_TEST(four_plus_one_is_five)
{
    ASSERT_ROMAN_ADD_EQ("IV", "I", "V");
}
END_TEST

START_TEST(one_plus_four_is_five)
{
    ASSERT_ROMAN_ADD_EQ("I", "IV", "V");
}
END_TEST

START_TEST(fourteen_plus_one_is_fifteen)
{
    ASSERT_ROMAN_ADD_EQ("XIV", "I", "XV");
}
END_TEST

START_TEST(one_plus_fourteen_is_fifteen)
{
    ASSERT_ROMAN_ADD_EQ("I", "XIV", "XV");
}
END_TEST

START_TEST(fourteen_plus_fourteen_is_twenty_eight)
{
    ASSERT_ROMAN_ADD_EQ("XIV", "XIV", "XXVIII");
}
END_TEST

START_TEST(nineteen_plus_nineteen_is_thirty_eight)
{
    ASSERT_ROMAN_ADD_EQ("XIX", "XIX", "XXXVIII");
}
END_TEST

START_TEST(forty_plus_forty_is_eighty)
{
    ASSERT_ROMAN_ADD_EQ("XL", "XL", "LXXX");
}
END_TEST

START_TEST(ninety_plus_ninety_is_one_hundred_eighty)
{
    ASSERT_ROMAN_ADD_EQ("XC", "XC", "CLXXX");
}
END_TEST

START_TEST(four_hundred_plus_four_hundred_is_eight_hundred)
{
    ASSERT_ROMAN_ADD_EQ("CD", "CD", "DCCC");
}
END_TEST

START_TEST(nine_hundred_plus_nine_hundred_is_one_thousand_eight_hundred)
{
    ASSERT_ROMAN_ADD_EQ("CM", "CM", "MDCCC");
}
END_TEST

START_TEST(twenty_plus_twenty_is_forty)
{
    ASSERT_ROMAN_ADD_EQ("XX", "XX", "XL");
}
END_TEST

START_TEST(two_hundred_plus_two_hundred_is_four_hundred)
{
    ASSERT_ROMAN_ADD_EQ("CC", "CC", "CD");
}
END_TEST

START_TEST(five_plus_four_is_nine)
{
    ASSERT_ROMAN_ADD_EQ("V", "IV", "IX");
}
END_TEST

START_TEST(fifty_plus_forty_is_ninety)
{
    ASSERT_ROMAN_ADD_EQ("L", "XL", "XC");
}
END_TEST

START_TEST(five_hundred_plus_four_hundred_is_nine_hundred)
{
    ASSERT_ROMAN_ADD_EQ("D", "CD", "CM");
}
END_TEST

////////////////////////////////////////////////////////////

START_TEST(two_minus_one_is_one)
{
    ASSERT_ROMAN_SUBTRACT_EQ("II", "I", "I");
}
END_TEST

START_TEST(three_minus_two_is_one)
{
    ASSERT_ROMAN_SUBTRACT_EQ("III", "II", "I");
}
END_TEST

START_TEST(five_minus_one_is_four)
{
    ASSERT_ROMAN_SUBTRACT_EQ("V", "I", "IV");
}
END_TEST

START_TEST(ten_minus_one_is_nine)
{
    ASSERT_ROMAN_SUBTRACT_EQ("X", "I", "IX");
}
END_TEST

START_TEST(fifty_minus_one_is_forty_nine)
{
    ASSERT_ROMAN_SUBTRACT_EQ("L", "I", "XLIX");
}
END_TEST

START_TEST(one_hundred_minus_one_is_ninety_nine)
{
    ASSERT_ROMAN_SUBTRACT_EQ("C", "I", "XCIX");
}
END_TEST

START_TEST(five_hundred_minus_one_is_four_hundred_ninety_nine)
{
    ASSERT_ROMAN_SUBTRACT_EQ("D", "I", "CDXCIX");
}
END_TEST

START_TEST(one_thousand_minus_one_is_nine_hundred_ninety_nine)
{
    ASSERT_ROMAN_SUBTRACT_EQ("M", "I", "CMXCIX");
}
END_TEST

START_TEST(can_subtract_five_from_all_larger_symbols)
{
    ASSERT_ROMAN_SUBTRACT_EQ("X", "V", "V");
    ASSERT_ROMAN_SUBTRACT_EQ("L", "V", "XLV");
    ASSERT_ROMAN_SUBTRACT_EQ("C", "V", "XCV");
    ASSERT_ROMAN_SUBTRACT_EQ("D", "V", "CDXCV");
    ASSERT_ROMAN_SUBTRACT_EQ("M", "V", "CMXCV");
}
END_TEST

START_TEST(can_subtract_ten_from_all_larger_symbols)
{
    ASSERT_ROMAN_SUBTRACT_EQ("L", "X", "XL");
    ASSERT_ROMAN_SUBTRACT_EQ("C", "X", "XC");
    ASSERT_ROMAN_SUBTRACT_EQ("D", "X", "CDXC");
    ASSERT_ROMAN_SUBTRACT_EQ("M", "X", "CMXC");
}
END_TEST

START_TEST(can_subtract_fifty_from_all_larger_symbols)
{
    ASSERT_ROMAN_SUBTRACT_EQ("C", "L", "L");
    ASSERT_ROMAN_SUBTRACT_EQ("D", "L", "CDL");
    ASSERT_ROMAN_SUBTRACT_EQ("M", "L", "CML");
}
END_TEST

START_TEST(can_subtract_one_hundred_from_all_larger_symbols)
{
    ASSERT_ROMAN_SUBTRACT_EQ("D", "C", "CD");
    ASSERT_ROMAN_SUBTRACT_EQ("M", "C", "CM");
}
END_TEST

START_TEST(one_thousand_minus_five_hundred_is_five_hundred)
{
    ASSERT_ROMAN_SUBTRACT_EQ("M", "D", "D");
}
END_TEST

START_TEST(twenty_minus_one_is_nineteen)
{
    ASSERT_ROMAN_SUBTRACT_EQ("XX", "I", "XIX");
}
END_TEST

START_TEST(subtraction_underflow_returns_empty_string)
{
    ASSERT_ROMAN_SUBTRACT_EQ("I", "XX", "");
}
END_TEST

///////////////////////////////////////////////////////////

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
    tcase_add_test(tc, four_hundred_plus_four_hundred_is_eight_hundred);
    tcase_add_test(tc, nine_hundred_plus_nine_hundred_is_one_thousand_eight_hundred);
    tcase_add_test(tc, twenty_plus_twenty_is_forty);
    tcase_add_test(tc, two_hundred_plus_two_hundred_is_four_hundred);
    tcase_add_test(tc, five_plus_four_is_nine);
    tcase_add_test(tc, fifty_plus_forty_is_ninety);
    tcase_add_test(tc, five_hundred_plus_four_hundred_is_nine_hundred);

    suite_add_tcase(s, tc);

    tc = tcase_create("roman_subtract");

    tcase_add_test(tc, two_minus_one_is_one);
    tcase_add_test(tc, three_minus_two_is_one);
    tcase_add_test(tc, five_minus_one_is_four);
    tcase_add_test(tc, ten_minus_one_is_nine);
    tcase_add_test(tc, fifty_minus_one_is_forty_nine);
    tcase_add_test(tc, one_hundred_minus_one_is_ninety_nine);
    tcase_add_test(tc, five_hundred_minus_one_is_four_hundred_ninety_nine);
    tcase_add_test(tc, one_thousand_minus_one_is_nine_hundred_ninety_nine);
    tcase_add_test(tc, can_subtract_five_from_all_larger_symbols);
    tcase_add_test(tc, can_subtract_ten_from_all_larger_symbols);
    tcase_add_test(tc, can_subtract_fifty_from_all_larger_symbols);
    tcase_add_test(tc, can_subtract_one_hundred_from_all_larger_symbols);
    tcase_add_test(tc, one_thousand_minus_five_hundred_is_five_hundred);
    tcase_add_test(tc, twenty_minus_one_is_nineteen);
    tcase_add_test(tc, subtraction_underflow_returns_empty_string);

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
