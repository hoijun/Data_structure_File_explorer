#include "sort.cpp"
#include <algorithm>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define UP 0
#define DOWN 1
#define QUIT 99
#define ZIP 3
#define ENTER 2

string fileTM(const struct stat *fileInfo) { // 파일 권한
    string temp;
    if (S_ISREG(fileInfo->st_mode)) {
        temp += "-";
    } else if (S_ISDIR(fileInfo->st_mode)) {
        temp += "d";
    } else if (S_ISLNK(fileInfo->st_mode)) {
        temp += "l";
    } else if (S_ISSOCK(fileInfo->st_mode)) {
        temp += "s";
    } else if (S_ISFIFO(fileInfo->st_mode)) {
        temp += "FIFO";
    } else if (S_ISCHR(fileInfo->st_mode)) {
        temp += "c";
    } else if (S_ISBLK(fileInfo->st_mode)) {
        temp += "b";
    } else if (S_TYPEISMQ(fileInfo)) {
        temp += "?";
    } else if (S_TYPEISSEM(fileInfo)) {
        temp += "?";
    } else if (S_TYPEISSHM(fileInfo)) {
        temp += "?";
    }

    if (S_IRUSR & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWUSR & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXUSR & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    if (S_IRGRP & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWGRP & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXGRP & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    if (S_IROTH & fileInfo->st_mode) {
        temp += "r";
    } else {
        temp += "-";
    }
    if (S_IWOTH & fileInfo->st_mode) {
        temp += "w";
    } else {
        temp += "-";
    }
    if (S_IXOTH & fileInfo->st_mode) {
        temp += "x";
    } else {
        temp += "-";
    }
    return temp;
}
void gotoxy(int x, int y) { // 좌표 이동
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}
void frame() { // 틀 출력
    gotoxy(1, 1);
    printf("┌──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────┐\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────│\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────│\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("└──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────┘\n");
    gotoxy(5, 2);
    printf("File Explorer");
}
void printfile(vector<sortfile> f) { // 파일 출력
    int x = 0;
    for (int i = 0; i < f.size(); i++) {
        if (i == 18)
            break;
        gotoxy(5, x + 7);
        printf("%s", f[i].tm.c_str());
        printf("      %-8s", f[i].username.c_str());
        printf("%8ld", f[i].size);
        printf("      %s", f[i].time.c_str());
        printf("      %s\n", f[i].filename.c_str());
        x++;
    }
}
char *timeToString(struct tm *t) { // m_time 년도월 시분초 변경
    static char s[20];
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900,
            t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}
bool comparen(sortfile a, sortfile b) {
    return a.filename < b.filename;
} // 파일 이름순 정렬
bool comparet(sortfile a, sortfile b) {
    return a.time < b.time;
} // 파일 시간순 정렬
int getch() { // 키보드 입력(원리 잘모름)
    int c;
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr); // 현재 터미널 설정 읽음
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // CANONICAL과 ECHO 끔
    newattr.c_cc[VMIN] = 1;  // 최소 입력 문자 수를 1로 설정
    newattr.c_cc[VTIME] = 0; // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // 터미널에 설정 입력
    c = getchar();                              // 키보드 입력 읽음
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // 원래의 설정으로 복구
    return c;
}
int keycontrol() { // 키보드 입력
    char t = getch();
    if (t == 'w') // w 입력시 위로
        return UP;
    else if (t == 's') // s 입력시 아래로
        return DOWN;
    else if (t == 'q') // q 입력시 종료
        return QUIT;
    else if (t == 'z')
        return ZIP;
    else if (t == 10)
        return ENTER;
}
