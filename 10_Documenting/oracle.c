#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define _(STRING) gettext(STRING)
#define HELP _("\
Program for guessing number in arabic or roman notation\n\
\n\
Usage: oracle [OPTIONS]\n\
\n\
\t--help\t\tprint this message\n\
\t-r\t\troman notation mode\n\
")

char *ROMAN_NUMS[] = {
        "O", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
        "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
        "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
        "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL",
        "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L",
        "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", "LX",
        "LXI", "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", "LXIX", "LXX",
        "LXXI", "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", "LXXVIII", "LXXIX", "LXXX",
        "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", "LXXXVII", "LXXXVIII", "LXXXIX", "XC",
        "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", "XCVII", "XCVIII", "XCIX", "C"};

/**
 * Converts integer @num to Roman numerals
 *
 * @param num number to convert
 * @returns string with Roman representation
 */
char *to_roman(int num) {
    return ROMAN_NUMS[num];
}

/**
 * Converts Roman numeral @roman_num to integer
 *
 * @param roman_num string with Roman numeral
 * @returns integer
 */
int from_roman(char *roman_num) {
    for (int i = 0; i < 101; ++i) {
        if (!strcmp(ROMAN_NUMS[i], roman_num)) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");
    bindtextdomain("oracle", ".");
    textdomain("oracle");

    int ROMAN_MODE = 0;
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-r"))
            ROMAN_MODE = 1;
        else if (!strcmp(argv[i], "--help"))
            return !printf("%s\n", HELP);
    }

    if (ROMAN_MODE)
        printf(_("Guess a number from I to C\n"));
    else
        printf(_("Guess a number from 1 to 100\n"));
    char answer[32];
    int right = 100;
    for (int left = 1, mid = (left + right) / 2; right != left; mid = (left + right) / 2) {
        if (ROMAN_MODE)
            printf(_("Is your number greater than %s?\n"), to_roman(mid));
        else
            printf(_("Is your number greater than %d?\n"), mid);
        printf("(%s/%s)\n", _("yes"), _("no"));
        scanf("%32s", answer);
        if (!strcmp(answer, _("yes"))) {
            left = mid + 1;
        } else if (!strcmp(answer, _("no"))) {
            right = mid;
        } else {
            printf(_("Understand you not. Try again!\n"));
        }
    }
    printf(_("Your number is %d\n"), right);
    return 0;
}
