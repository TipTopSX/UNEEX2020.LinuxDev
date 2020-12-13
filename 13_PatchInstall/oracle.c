#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <libgen.h>

#define _(STRING) gettext(STRING)

int main(int argc, char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    bindtextdomain("oracle", dirname(realpath(argv[0], NULL)));
    textdomain("oracle");

    printf(_("Guess a number from 1 to 100\n"));
    char answer[32];
    int right = 100;
    for (int left = 1, mid = (left + right) / 2; right != left; mid = (left + right) / 2) {
        printf(_("Is your number greater than %d? "), mid);
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
