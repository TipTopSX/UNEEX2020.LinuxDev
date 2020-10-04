#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: a.out filename\n");
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Can't open file \"%s\"\n", argv[1]);
        return 1;
    }
    struct stat st;
    stat(argv[1], &st);
    size_t size = st.st_size;
    char *buf = malloc(size);

    fread(buf, sizeof(char), size, f);
    int num_str = 0;
    for (char *p = buf; p < buf + size; ++p) {
        if (*p == '\n') {
            ++num_str;
        }
    }
    char **strings = calloc(num_str, sizeof(char *));
    strings[0] = buf;
    size_t sn = 0;
    for (char *p = buf; p < buf + size - 1; ++p) {
        if (*p == '\n') {
            strings[++sn] = p + 1;
            *p = 0;
        }
    }

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();

    int c = 0, DX = 0, DY = 0;
    WINDOW *win = newwin(LINES - 2 * DX, COLS - 2 * DX, DX, DX);
    do {
        werase(win);
        wprintw(win, "File: %s, %d bytes\n", argv[1], size);
        for (int i = 1; i < LINES - 2 * DX; ++i) {
            wprintw(win, "  %d: %s\n", DY + i, strings[DY + i]);
        }
        wrefresh(win);
        switch (c = wgetch(win)) {
        case 'j':
        case KEY_DOWN:
            DY += DY < num_str - LINES ? 1 : 0;
            break;
        case 'k':
        case KEY_UP:
            DY -= DY > 0 ? 1 : 0;
            break;
        case KEY_PPAGE:
            if (DY > LINES) {
                DY -= LINES;
            } else {
                DY = 0;
            }
            break;
        case KEY_NPAGE:if (DY < num_str - 2 * LINES) {
                DY += LINES;
            } else if (DY < num_str - LINES) {
                DY = num_str - LINES;
            }
            break;
        case ' ':
            if (DY < num_str - 2 * LINES) {
                DY += LINES;
            } else if (DY < num_str - LINES) {
                DY = num_str - LINES;
            } else {
                c = 27;
            }
            break;
        default:
            break;
        }
    } while (c != 27);
    endwin();
    fclose(f);
    free(strings);
    free(buf);
    return 0;
}
