#include <ncurses.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 5000
#define HEIGHT_COLS 15
#define WIDTH_ROW  88

#define WIN_HEIGHT 20
#define WIN_WIDTH 100

#define FRAME_OFFSET_Y 7
#define FRAME_OFFSET_X 3
#define OFFSET_FROM_ENUMERATION 10

int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

void cutStr(char* str) {
    if (strlen(str) >= WIDTH_ROW)
        str[WIDTH_ROW-1] = '\0';
}

void deleteNewLineFromString(char* str) {
    for (int i = strlen(str) - 1 ; i >= 0; ++i) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

void readFile(char* file, char** lines) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    FILE* fp;
    fp = fopen(file, "r");
    if (fp == NULL)
        printf("ya eblan\n");

    int i = 0;    

    while ((read = getline(&line, &len, fp)) != -1) {
        char* tmp_buff = (char*)malloc(strlen(line) + 1);
        strcpy(tmp_buff, line);
        lines[i] = tmp_buff;
        cutStr(lines[i]);
        deleteNewLineFromString(lines[i]);
        ++i;
    }

    fclose(fp);

}

int itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

void finalize(WINDOW* win) {
    delwin(win);
    endwin();
    curs_set(1);
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        printf("application requires 2 args\n");
        return -1;
    }
    
    initscr();
    curs_set(0);
    printw(argv[1]);
    refresh();
    
    char* lines[MAX_LINES];
    readFile(argv[1], lines);

    WINDOW* win = newwin(WIN_HEIGHT, WIN_WIDTH, 5, 0);
    for (int i = 0; i < HEIGHT_COLS; ++i) {
        if (lines[i] != NULL) {
            char s1[15];
            itoa(i, s1, 10);
            const char* s2 = ".";
            char *result = (char*)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
            // in real code you would check for errors in malloc here
            strcpy(result, s1);
            strcat(result, s2);

            mvaddstr(FRAME_OFFSET_Y+i, FRAME_OFFSET_X, result);
            mvaddstr(FRAME_OFFSET_Y+i, OFFSET_FROM_ENUMERATION, lines[i]);
        }
    }
    box(win, 0, 0);
    wrefresh(win);

    int curr_pos = 0;

    while( true ) {
        int ch = getch();
        if (ch == 32) {
            ++curr_pos;
            werase(win);
            for (int i = 0 ; i < HEIGHT_COLS ; ++i) {
                if ((i + curr_pos) < MAX_LINES && lines[i + curr_pos] != NULL) {
                    char s1[15];
                    itoa(i + curr_pos, s1, 10);
                    const char* s2 = ".";
                    char *result = (char*)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator

                    strcpy(result, s1);
                    strcat(result, s2);

                    mvaddstr(FRAME_OFFSET_Y+i, FRAME_OFFSET_X, result);
                    mvaddstr(FRAME_OFFSET_Y+i, OFFSET_FROM_ENUMERATION, lines[i + curr_pos]);
                } else {
                    break;
                }
            }
            box(win, 0, 0);
            wrefresh(win);
        }
        else if (ch == 27) {
            finalize(win);
            return 0;
        }
            
        else
            continue;
    }
    getch();
    
    finalize(win);
    
    return 0;
}