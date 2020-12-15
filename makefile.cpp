#include <fstream>
#include "gotoxy.h"

int makefile_main() {

    std::ofstream outfile;

    char filename[100];
    char memo[100];
    printf("만들 파일 이름 : ");
    scanf("%s", filename);
    outfile.open(filename, std::ios_base::app);
    gotoxy(3, 29);
    printf("파일에 입력할 간단한 메모:  "); //텍스트파일에 메모저장기능
    scanf("%s", memo);
    outfile << memo;
    return 0;
}
