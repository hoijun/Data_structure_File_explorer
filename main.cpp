#include "sort.hpp"

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#define MAX_PATH_LEN 1024
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define QUIT 99
#define ZIP 111
#define ENTER 10

#define STARTINDEX 9
#define LASTINDEX 26
#define LRINDEX 7
using namespace std;

string fileTM(const struct stat *fileInfo); // 파일 권한
void gotoxy(int x, int y);                  // 좌표 이동
void frame();                               // 틀 출력
void printfile(list<sortfile> *f);          // 파일 출력
char *timeToString(struct tm *t);           // m_time 년도월 시분초 변경
int getch();      // 키보드 입력(원리 잘모름)
int keycontrol(); // 키보드 입력
int menuSort(ListSort &files);   // sort menu
#define MAX_PATH_LEN 1024

int main() {
    while (1) {
        struct stat fileInfo;
        DIR *dirp;
        struct dirent *dirInfo;
        struct passwd *userInfo;
        ListSort Files = ListSort(); // 파일 담을 리스트
        char cwd[MAX_PATH_LEN + 1] = {
            '\0',
        };
        if (getcwd(cwd, MAX_PATH_LEN) == NULL) {
            perror("getcwd() error!");
            exit(-3);
        }
        if (stat(cwd, &fileInfo) == -1) {
            perror("stat() error!");
            exit(-4);
        }
        system("clear");
        frame();
        gotoxy(5, 4);
        printf("Current Directory: %s\n", cwd);
        gotoxy(5, 6);
        printf("|   Sort   |   Creat  |  Delete  |  Select Mode  |\n");
        dirp = opendir(cwd);
        while ((dirInfo = readdir(dirp)) != NULL) {
            if (stat(("%s/%s", cwd, dirInfo->d_name), &fileInfo) == -1) {
                perror("stat()	error!");
                exit(-6);
            }
            string Tm = fileTM(&fileInfo);
            userInfo = getpwuid(fileInfo.st_uid);
            Files.add(userInfo->pw_name, fileInfo.st_mtime, dirInfo->d_name, Tm,
                      fileInfo.st_size);
        }
        Files.sortNameUp(); // 파일 이름순 정렬
        int direct = STARTINDEX;
        int leftright = LRINDEX;
        gotoxy(3, direct); // >의 처음 위치
        printf(">");
        printfile(Files.getList()); // 파일 출력

        list<sortfile> *file = Files.getList();
        list<sortfile>::iterator it1;
        it1 = file->begin();
        while (1) {
            int n = keycontrol(); // 키보드 입력
            if (n == UP) {
                if (direct == STARTINDEX) {
                    direct = direct - 3;
                    gotoxy(3, direct + 3);
                    printf(" ");
                    gotoxy(7, direct);
                    printf(">");
                    leftright = LRINDEX;
                } else if (direct > STARTINDEX) { // > 위로 이동
                    direct--;
                    gotoxy(3, direct + 1);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                    it1--;
                }
            } else if (n == DOWN) {
                if (direct == STARTINDEX - 3) {
                    direct = direct + 3;
                    gotoxy(leftright, direct - 3);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                    leftright = LRINDEX;
                } else if (direct < LASTINDEX &&
                           direct < Files.listSize() + STARTINDEX -
                                        1) { // > 아래로 이동
                    direct++;
                    gotoxy(3, direct - 1);
                    printf(" ");
                    gotoxy(3, direct);
                    printf(">");
                    it1++;
                }
            } else if (n == LEFT && direct == STARTINDEX - 3 || direct == STARTINDEX - 2) {
                if (leftright > LRINDEX) {
                    leftright = leftright - 11;
                    gotoxy(leftright + 11, direct);
                    printf(" ");
                    gotoxy(leftright, direct);
                    printf(">");
                }
            } else if (n == RIGHT && direct == STARTINDEX - 3 ||
                       direct == STARTINDEX - 2) {
                if (leftright < LRINDEX + 33) {
                    leftright = leftright + 11;
                    gotoxy(leftright - 11, direct);
                    printf(" ");
                    gotoxy(leftright, direct);
                    printf(">");
                }
            } else if (n == ENTER) {
                if (direct == STARTINDEX - 3){
                    if (leftright == LRINDEX){  // sort
                        gotoxy(5, 7);
                        printf("|  Name ▲ |  Size   |  Time   |\n");
                        direct = menuSort(Files);
                        list<sortfile> *sortedFile = Files.getList();
                        it1 = sortedFile->begin();
                        gotoxy(3, 29);
                        printf("sort function");
                    }
                    else if (leftright == LRINDEX + 11){
                        gotoxy(3, 29);
                        printf("creat function");
                    } else if (leftright == LRINDEX + (11 * 2)) {
                        gotoxy(3, 29);
                        printf("delete function");
                    } else if (leftright == LRINDEX + (11 * 3)) {
                        gotoxy(3, 29);
                        printf("select function");
                    }
                }
                else if (it1->tm.find("d") != string::npos) { // 디렉토리 파일일시
                    if (it1->filename.compare(".") == 0 ||
                        it1->filename.compare("..") ==
                            0) { //. .. 이면 그냥 이동
                        if (chdir(it1->filename.c_str()) == -1) {
                            perror("chdir()	error!");
                            exit(-1);
                        }
                    } else {
                        if (chdir(("./" + it1->filename).c_str()) ==
                            -1) { // . .. 이 아니면 ./을 앞에 붙여서 이동
                            perror("chdir()	error!");
                            exit(-1);
                        }
                    }
                    break;
                }
            } else if (n == QUIT)
                exit(-1);
        }
    }
    return 0;
}
string fileTM(const struct stat *fileInfo) {
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
void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}
void frame() {
    gotoxy(1, 1);
    printf("┌──────────────────────────────────────────────────────────────"
           "────"
           "─────────────┐\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "─────────────│\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "─────────────│\n");
    printf("│                                                              "
           "    "
           "             │\n");

    printf("│                                                              "
           "    "
           "             │\n");

    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");

    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "─────────────│\n");
    printf("│ w: up s: down q: quit                                        "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("│                                                              "
           "    "
           "             │\n");
    printf("└──────────────────────────────────────────────────────────────"
           "────"
           "─────────────┘\n");
    gotoxy(5, 2);
    printf("File Explorer");
}
void printfile(list<sortfile> *f) {
    list<sortfile>::iterator it;
    int i = 0;
    for (it = f->begin(); it != f->end(); it++) {
        if (i == 18)
            break;
        gotoxy(5, i + STARTINDEX);
        printf("%s", it->tm.c_str());
        printf("      %-8s", it->username.c_str());
        printf("%8ld", it->size);
        printf("      %s", timeToString(localtime(&it->time)));
        printf("      %s\n", it->filename.c_str());

        i++;
    }
}
char *timeToString(struct tm *t) {
    static char s[20];
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900,
            t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}
int getch() {
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
    else if (t == 'a') // a 입력시 좌측으로
        return LEFT;
    else if (t == 'd') // d 입력시 우측으로
        return RIGHT;
    else if (t == 'q') // q 입력시 종료
        return QUIT;
    else if (t == 'z')
        return ZIP;
    else if (t == 10)
        return ENTER;
}
int menuSort(ListSort &files){
    const int rowIndex = 7;
    int corsur = LRINDEX;
    const int updownIndex = LRINDEX + 8;
    int triangle = updownIndex;
    int checkUpDown = 0;    //0이면 upward, 1이면 downward
    gotoxy(corsur, rowIndex - 1);
    printf(" ");
    gotoxy(corsur, rowIndex);
    printf(">");
    while (1) {
        int key = keycontrol();
        if (key == LEFT && corsur > LRINDEX){
            corsur = corsur - 10;
            gotoxy(corsur + 10, rowIndex);
            printf(" ");
            gotoxy(corsur, rowIndex);
            printf(">");
        } else if (key == RIGHT && corsur < LRINDEX + 20) {
            corsur = corsur + 10;
            gotoxy(corsur - 10, rowIndex);
            printf(" ");
            gotoxy(corsur, rowIndex);
            printf(">");
        } else if (key == DOWN){
            gotoxy(LRINDEX - 3, rowIndex);
            printf("                                ");
            gotoxy(3, rowIndex + 2);
            printf(">");
            return STARTINDEX;
        } else if (key == UP){
            gotoxy(LRINDEX - 3, rowIndex);
            printf("                                ");
            gotoxy(LRINDEX, rowIndex - 1);
            printf(">");
            return STARTINDEX - 3;
        }
    }
}
