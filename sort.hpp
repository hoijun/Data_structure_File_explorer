#include <list>
#include <string>

using namespace std;

// 파일 정보 담는 클래스
class sortfile { // 파일 정렬 클래스
  public:
    string tm;          // 권한
    string username;    // 유저 이름
    time_t time;        // 수정 시간
    string filename;    // 파일 이름
    off_t size;         // 파일 사이즈
    sortfile(string u, time_t t, string f, string m, off_t s) {
        username = u;
        time = t;
        filename = f;
        tm = m;
        size = s;
    }
    void add(string u, time_t t, string f, string m, off_t s);
};


// list 컨테이너 변형
// 간단하게 add와 sort위한 list 라고 생각하면 됨.
// list 이긴 하지만 list를 이루는 요소들에 직접적인 접근은 불가능
// *getList()를 통해 *filelist를 추출해 사용해야 함.
class ListSort {
public:
    ListSort() {
        filelist = new list<sortfile>;
        size = 0;
    }
    ListSort(list<sortfile> *flist) {
        filelist = flist;
        size = 0;
    }

    void add(sortfile info);
    void add(string u, time_t t, string f, string m, off_t s);
    size_t listSize();

    void sortNameUp();  //오름차순 정렬
    void sortTimeUp();
    void sortSizeUp();
    void sortNameDown();    // 내림차순 정렬
    void sortTimeDown();
    void sortSizeDown();

    list<sortfile> *getList();

  private:
    list<sortfile> *filelist;
    size_t size;
};

bool compareNameUp(sortfile a, sortfile b);
bool compareTimeUp(sortfile a, sortfile b);
bool compareSizeUp(sortfile a, sortfile b);
bool compareNameDown(sortfile a, sortfile b);
bool compareTimeDown(sortfile a, sortfile b);
bool compareSizeDown(sortfile a, sortfile b);
