#include <stdio.h>

int delete_main() {

    char Path[100];

    printf("삭제할 파일 또는 디렉토리: ");
    scanf("%s", Path); //삭제할 파일, 디렉토리 이름 입력받기

    int nResult = remove(Path);
    if (nResult == 0) {

        printf(" 삭제 성공\n");
    } else if (nResult == -1) {
        
        perror(" 삭제 실패\n");
    }
    return 0;
}
