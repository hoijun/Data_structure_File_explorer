#include "sort.hpp"

#include "delete.h"
#include "gotoxy.h"
#include "makedirectory.h"
#include "makefile.h"
#include "move.h"
#include "rename.h"

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
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
#define UNZIP 112
#define PUSH 113
#define MOVE 115
#define RENAME 114
#define ENTER 10

#define STARTINDEX 9
#define LASTINDEX 26
#define LRINDEX 7
using namespace std;

typedef struct _Check {
    bool nameUp;
    bool sizeUp;
    bool timeUp;
} Check;

string fileTM(const struct stat *fileInfo); // 파일 권한
void gotoxy(int x, int y);                  // 좌표 이동
void frame();                               // 틀 출력
void printfile(list<sortfile> *f);          // 파일 출력
char *timeToString(struct tm *t);           // m_time 년도월 시분초 변경
int getch();                           // 키보드 입력(원리 잘모름)
int keycontrol();                      // 키보드 입력
int menuSort(ListSort &files);         // sort menu
bool checkUp(Check check, int corsur); // check sort direction
void clearWindow();                    // 창에 있는 파일 목록 삭제

#define MAX_PATH_LEN 1024

string sortInfo = "name ascending";

int main() {
    while (1) {
    TOFIRST:
        clearWindow(); // 화면 초기화

        int zippoint = 0;
        int movepoint = 0;
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
        printf("|   \033[1;34mSort\033[0m   |  \033[1;34mDelete\033[0m  |   "
               "\033[1;34mFile\033[0m  |   "
               "\033[1;34mDirectory\033[0m   |\n");
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
            if (direct ==
                STARTINDEX) { // 반복문 진행으로 인한 커서 증발 현상 해결
                gotoxy(3, STARTINDEX);
                printf(">");
            }

            if (zippoint == 99 || movepoint == 99)
                break;

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
            } else if (n == LEFT && direct == STARTINDEX - 3 ||
                       direct == STARTINDEX - 2) {
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
                if (direct == STARTINDEX - 3) {

                    list<sortfile> *sortedFile = Files.getList();

                    if (leftright == LRINDEX) { // sort
                        gotoxy(5, 7);
                        cout << "|  Name ▲ |  Size   |  Time   |   "
                                "\033[36mSorted by : "
                             << sortInfo << "\033[0m\n";
                        direct = menuSort(Files);
                    } else if (leftright == LRINDEX + 11) {
                        gotoxy(3, 28);
                        delete_main(); // 파일, 디렉토리 삭제하기
                        goto TOFIRST;
                    } else if (leftright == LRINDEX + (11 * 2)) {
                        gotoxy(3, 28);
                        makefile_main(); //파일 만들기
                        goto TOFIRST;
                    } else if (leftright == LRINDEX + (11 * 3)) {
                        gotoxy(3, 28);
                        makedirectory_main(); //디렉토리 만들기
                        goto TOFIRST;
                    }
                    sortedFile = Files.getList();
                    it1 = sortedFile->begin();
                    clearWindow();
                    printfile(sortedFile);

                } else if (it1->tm.find("d") !=
                           string::npos) { // 디렉토리 파일일시
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
            } else if (n == ZIP) {
                if (direct > 8) {
                    int direct2 = direct;
                    gotoxy(5, 8);
                    printf("압축할 파일 선택중...");
                    list<sortfile>::iterator it2 = it1;
                    list<string> temp1;
                    list<string>::iterator it4;
                    string zipfilename = "";
                    while (1) {
                        gotoxy(3, 28);
                        printf("선택한 파일들: ");
                        gotoxy(18, 28);
                        printf("%s", zipfilename.c_str());
                        int n2 = keycontrol();
                        if (n2 == UP) {
                            if (direct2 > STARTINDEX) { // > 위로 이동
                                direct2--;
                                gotoxy(3, direct2 + 1);
                                printf(" ");
                                gotoxy(3, direct2);
                                printf(">");
                                it2--;
                            }
                        } else if (n2 == DOWN) {
                            if (direct2 < LASTINDEX &&
                                direct2 < Files.listSize() + STARTINDEX -
                                              1) { // > 아래로 이동
                                direct2++;
                                gotoxy(3, direct2 - 1);
                                printf(" ");
                                gotoxy(3, direct2);
                                printf(">");
                                it2++;
                            }
                        } else if (n2 == PUSH) {
                            if (it2->filename.compare(".") != 0 &&
                                it2->filename.compare("..") != 0) {
                                for (it4 = temp1.begin(); it4 != temp1.end();
                                     it4++) {
                                    if (*it4 == it2->filename) {
                                        break;
                                    }
                                }
                                if (*it4 == it2->filename) {
                                } else {
                                    zipfilename.append((it2->filename + " "));
                                    temp1.push_back(it2->filename);
                                }
                            }

                        } else if (n2 == ZIP) {
                            if (zipfilename.length() > 0) {
                                zippoint = 99;
                                char temp[1001];
                                gotoxy(3, 29);
                                printf("압축 파일의 이름을 작성해 주세요: ");
                                scanf("%s", temp);
                                string name = temp;
                                system(("tar -czvf " + name + ".tar.gz" + " " +
                                        zipfilename + "1>/dev/null")
                                           .c_str());
                                gotoxy(3, direct2);
                                printf(" ");
                                gotoxy(3, direct);
                                printf(">");
                                gotoxy(5, 8);
                                printf("                     ");
                                gotoxy(3, 28);
                                printf("                                       "
                                       "    "
                                       "    "
                                       "                             ");
                                gotoxy(3, 29);
                                printf("                                       "
                                       "    "
                                       "    "
                                       "                             ");
                                break;
                            }
                        } else if (n2 == QUIT) {
                            gotoxy(3, direct2);
                            printf(" ");
                            gotoxy(3, direct);
                            printf(">");
                            gotoxy(5, 8);
                            printf("                     ");
                            gotoxy(3, 28);
                            printf("                                           "
                                   "    "
                                   "    "
                                   "                                 ");
                            break;
                        }
                    }
                }
            } else if (n == UNZIP) {
                if (it1->filename.find(".tar.gz") != string::npos) {
                    system(("tar -xzvf" + it1->filename).c_str());
                    break;
                }
            } else if (n == MOVE) {
                if (direct > 8) {
                    int direct3 = direct;
                    gotoxy(5, 8);
                    printf("이동할 파일 선택중...");
                    list<sortfile>::iterator it3 = it1;
                    list<string> temp2;
                    list<string>::iterator it5;
                    string movefilename = "";
                    while (1) {
                        gotoxy(3, 28);
                        printf("선택한 파일들: ");
                        gotoxy(18, 28);
                        printf("%s", movefilename.c_str());
                        int n3 = keycontrol();
                        if (n3 == UP) {
                            if (direct3 > STARTINDEX) { // > 위로 이동
                                direct3--;
                                gotoxy(3, direct3 + 1);
                                printf(" ");
                                gotoxy(3, direct3);
                                printf(">");
                                it3--;
                            }
                        } else if (n3 == DOWN) {
                            if (direct3 < LASTINDEX &&
                                direct3 < Files.listSize() + STARTINDEX -
                                              1) { // > 아래로 이동
                                direct3++;
                                gotoxy(3, direct3 - 1);
                                printf(" ");
                                gotoxy(3, direct3);
                                printf(">");
                                it3++;
                            }
                        } else if (n3 == PUSH) {
                            if (it3->filename.compare(".") != 0 &&
                                it3->filename.compare("..") != 0) {
                                for (it5 = temp2.begin(); it5 != temp2.end();
                                     it5++) {
                                    if (*it5 == it3->filename) {
                                        break;
                                    }
                                }
                                if (*it5 == it3->filename) {
                                } else {
                                    movefilename.append((it3->filename + " "));
                                    temp2.push_back(it3->filename);
                                }
                            }
                        } else if (n3 == MOVE) {
                            if (movefilename.length() > 0) {
                                movepoint = 99;
                                move_main(movefilename);
                                gotoxy(3, direct3);
                                printf(" ");
                                gotoxy(3, direct);
                                printf(">");
                                gotoxy(5, 8);
                                printf("                     ");
                                gotoxy(3, 28);
                                printf("                                       "
                                       "    "
                                       "    "
                                       "                             ");
                                gotoxy(3, 29);
                                printf("                                       "
                                       "    "
                                       "    "
                                       "                             ");
                                break;
                            }
                        } else if (n3 == QUIT) {
                            gotoxy(3, direct3);
                            printf(" ");
                            gotoxy(3, direct);
                            printf(">");
                            gotoxy(5, 8);
                            printf("                     ");
                            gotoxy(3, 28);
                            printf("                                           "
                                   "    "
                                   "    "
                                   "                                 ");
                            break;
                        }
                    }
                }
            } else if (n == RENAME) {
                if (direct > 8) {
                    if (it1->filename.compare(".") != 0 &&
                        it1->filename.compare("..") != 0) {
                        rename_main(it1->filename);
                        break;
                    }
                }
            } else if (n == QUIT) {
                system("clear");
                exit(-1);
            }
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

void frame() {
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

    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────│\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│                                                              "
           "    "
           "                   │\n");
    printf("│──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────│\n");
    printf("│ \033[1;32mNormal mode:\033[0m w, a, s, d : move "
           "\033[33m/\033[0m q: quit \033[33m/\033[0m z: Zip mode "
           "\033[33m/\033[0m x: unzip\033[33m/\033[0mm: Mo"
           "ve\033[33m/\033[0mr"
           ": rename │\n");
    printf("│ \033[1;32mZip mode:\033[0m p: push files for zip \033[33m/"
           "\033[0m z: express "
           "files \033[33m/\033[0m q: quit zip "
           "    "
           "               │\n");
    printf("│ \033[1;32mMove mode:\033[0m p: push files for move \033[33m/"
           "\033[0m m: move "
           "files \033[33m/\033[0m q: quit move  "
           "    "
           "              │\n");
    printf("└──────────────────────────────────────────────────────────────"
           "────"
           "───────────────────┘\n");
    gotoxy(5, 2);
    printf("\033[1;33mFile Explorer\033[0m");
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
    else if (t == 'x')
        return UNZIP;
    else if (t == 'p')
        return PUSH;
    else if (t == 'm')
        return MOVE;
    else if (t == 'r')
        return RENAME;
    else if (t == 10)
        return ENTER;
}
int menuSort(ListSort &files) {
    const int rowIndex = 7;
    int corsur = LRINDEX;
    const int updownIndex = LRINDEX + 6;
    int triangle = updownIndex;
    Check check{true, true, true};
    gotoxy(corsur, rowIndex - 1);
    printf(" ");
    gotoxy(corsur, rowIndex);
    printf(">");
    while (1) {
        int key = keycontrol();
        if (key == LEFT && corsur > LRINDEX) {
            corsur = corsur - 10;
            gotoxy(corsur + 10, rowIndex);
            printf(" ");
            gotoxy(corsur, rowIndex);
            printf(">");
            triangle = triangle - 10;
            gotoxy(triangle + 10, rowIndex);
            printf(" ");
            gotoxy(triangle, rowIndex);
            if (checkUp(check, corsur))
                printf("▲");
            else
                printf("▼");

        } else if (key == RIGHT && corsur < LRINDEX + 20) {
            corsur = corsur + 10;
            gotoxy(corsur - 10, rowIndex);
            printf(" ");
            gotoxy(corsur, rowIndex);
            printf(">");
            triangle = triangle + 10;
            gotoxy(triangle - 10, rowIndex);
            printf(" ");
            gotoxy(triangle, rowIndex);
            if (checkUp(check, corsur))
                printf("▲");
            else
                printf("▼");
        } else if (key == DOWN) {
            gotoxy(LRINDEX - 3, rowIndex);
            printf("                                                           "
                   "     ");
            gotoxy(3, STARTINDEX);
            printf(">");
            return STARTINDEX;
        } else if (key == UP) {
            gotoxy(LRINDEX - 3, rowIndex);
            printf("                                                           "
                   "     ");
            gotoxy(LRINDEX, rowIndex - 1);
            printf(">");
            return STARTINDEX - 3;
        } else if (key == ENTER) {
            string direction;
            if (corsur == LRINDEX) { // name
                if (check.nameUp) {
                    direction = "ascending ";
                    files.sortNameUp();
                } else {
                    direction = "descending";
                    files.sortNameDown();
                }
                sortInfo = "name " + direction;

                gotoxy(LRINDEX + 44, rowIndex);
                cout << "\033[36mname " << direction << "\033[0m";
                check.nameUp =
                    !check.nameUp; // enter 입력했으므로 화살표 방향 바꿈

            } else if (corsur == LRINDEX + 10) { // size
                if (check.sizeUp) {
                    direction = "ascending ";
                    files.sortSizeUp();
                } else {
                    direction = "descending";
                    files.sortSizeDown();
                }
                sortInfo = "size " + direction;

                gotoxy(LRINDEX + 44, rowIndex);
                cout << "\033[36msize " << direction << "\033[0m";
                check.sizeUp = !check.sizeUp;

            } else if (corsur == LRINDEX + 20) { // time
                if (check.timeUp) {
                    direction = "ascending ";
                    files.sortTimeUp();
                } else {
                    direction = "descending";
                    files.sortTimeDown();
                }
                sortInfo = "time " + direction;

                gotoxy(LRINDEX + 44, rowIndex);
                cout << "\033[36mtime " << direction << "\033[0m";
                check.timeUp = !check.timeUp;
            }
            gotoxy(triangle, rowIndex);
            if (checkUp(check, corsur))
                printf("▲");
            else
                printf("▼");

            clearWindow();
            printfile(files.getList());
        } else if (key == QUIT) {
            system("clear");
            exit(-2);
        }
    }
}

bool checkUp(Check check, int corsur) {
    if (corsur == LRINDEX) {
        return check.nameUp;
    } else if (corsur == LRINDEX + 10) {
        return check.sizeUp;
    } else if (corsur == LRINDEX + 20) {
        return check.timeUp;
    }
    return true;
}
void clearWindow() {
    for (int i = 0; i < 18; i++) {
        gotoxy(2, STARTINDEX + i);
        printf("                                                              "
               "    "
               "                   ");
    }
}
