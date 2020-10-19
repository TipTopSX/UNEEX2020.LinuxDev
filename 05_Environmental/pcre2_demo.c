#define PCRE2_CODE_UNIT_WIDTH 8

#include <ncurses.h>
#include <string.h>
#include <pcre2.h>

const uint32_t buf_size = 1024;

#ifdef without_UTF
const int no_UTF = 1;
#else
const int no_UTF = 0;
#endif

void search(const char *pattern, const char *subject, WINDOW *out) {
    pcre2_code *re;
    PCRE2_SIZE erroffset;
    int errcode;
    PCRE2_UCHAR8 buffer[buf_size];

    int rc;
    PCRE2_SIZE *ovector;

    size_t pattern_size = strlen(pattern);
    size_t subject_size = strlen(subject);
    uint32_t options;
    if (no_UTF) {
        options = 0;
    } else {
        options = PCRE2_UCP;
    }

    pcre2_match_data *match_data;
    uint32_t ovecsize = buf_size;

    re = pcre2_compile(pattern, pattern_size, options, &errcode, &erroffset, NULL);
    if (re == NULL) {
        pcre2_get_error_message(errcode, buffer, sizeof(buffer));
        wprintw(out, "%d\t%s\n", errcode, buffer);
    }

    match_data = pcre2_match_data_create(ovecsize, NULL);
    rc = pcre2_match(re, subject, subject_size, 0, options, match_data, NULL);
    wprintw(out, "%s: %s\n", pattern, subject);
    if (rc == 0) {
        wprintw(out, "offset vector too small: %d", rc);
    } else if (rc > 0) {
        ovector = pcre2_get_ovector_pointer(match_data);
        PCRE2_SIZE i;
        for (i = 0; i < rc; i++) {
            PCRE2_SPTR start = subject + ovector[2 * i];
            PCRE2_SIZE slen = ovector[2 * i + 1] - ovector[2 * i];
            wprintw(out, "%2lu: %.*s\n", i, (int) slen, (char *) start);
        }
    } else if (rc < 0) {
        wprintw(out, "No match\n");
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}

int main() {
    int DX = 1, c = 0;
    char pattern[buf_size];
    char subject[buf_size];
    initscr();
    echo();
    cbreak();
    WINDOW *pattern_win = newwin(2 * DX + 1, (COLS - 2 * DX) / 2, 0, DX);
    keypad(pattern_win, TRUE);
    WINDOW *subject_win = newwin(2 * DX + 1, (COLS - 2 * DX) / 2, 0, (COLS - 2 * DX) / 2 + DX);
    keypad(subject_win, TRUE);
    WINDOW *result_win = newwin(LINES - 4 * DX - 1, COLS - 2 * DX, 2 * DX + 1, DX);
    while (1) {
        werase(pattern_win);
        werase(subject_win);
        box(pattern_win, 0, 0);
        box(subject_win, 0, 0);
        wrefresh(pattern_win);
        wrefresh(subject_win);
        mvwgetnstr(pattern_win, 1, 1, pattern, buf_size);
        mvwgetnstr(subject_win, 1, 1, subject, buf_size);
        search(pattern, subject, result_win);
        wrefresh(result_win);
    } // ctrl+c to exit
    endwin();
    return 0;
}
