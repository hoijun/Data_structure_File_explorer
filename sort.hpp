#include <list>
#include <string>

using namespace std;

// 파일 정보 담는 클래스
// 파일 이름, 파일의 마지막 edit 시간, 파일 사이즈
class FileInfo {
public:
    FileInfo(){}
    FileInfo(string n, int t, size_t s){
        name = n;
        time = t;
        size = s;
    }
    void add(string n, int t, size_t s);

    string name;
    int time;
    size_t size;
};

// list 컨테이너 변형
// 간단하게 add와 sort위한 list 라고 생각하면 됨.
// list 이긴 하지만 list를 이루는 요소들에 직접적인 접근은 불가능
// *getList()를 통해 *filelist를 추출해 사용해야 함.
class ListSort {
public:
    ListSort() {
        filelist = new list<FileInfo>;
        size = 0;
    }
    ListSort(list<FileInfo> *flist) {
        filelist = flist;
        size = 0;
    }

    void add(FileInfo info);
    void add(string n, int t, size_t s);
    size_t listSize();

    void sortNameUp();  //오름차순 정렬
    void sortTimeUp();
    void sortSizeUp();
    void sortNameDown();    // 내림차순 정렬
    void sortTimeDown();
    void sortSizeDown();

    list<FileInfo> *getList();

  private:
    list<FileInfo> *filelist;
    size_t size;
};

bool compareNameUp(FileInfo a, FileInfo b);
bool compareTimeUp(FileInfo a, FileInfo b);
bool compareSizeUp(FileInfo a, FileInfo b);
bool compareNameDown(FileInfo a, FileInfo b);
bool compareTimeDown(FileInfo a, FileInfo b);
bool compareSizeDown(FileInfo a, FileInfo b);
