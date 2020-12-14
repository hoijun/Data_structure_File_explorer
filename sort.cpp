#include "sort.hpp"

bool compareNameUp(FileInfo a, FileInfo b) { return a.name < b.name; }
bool compareTimeUp(FileInfo a, FileInfo b) { return a.time < b.time; }
bool compareSizeUp(FileInfo a, FileInfo b) {
    if (a.time == b.time)
        return a.name < b.name;
    else
        return a.size < b.size;
}
bool compareNameDown(FileInfo a, FileInfo b) { return a.name > b.name; }
bool compareTimeDown(FileInfo a, FileInfo b) { return a.time > b.time; }
bool compareSizeDown(FileInfo a, FileInfo b) {
    if (a.time == b.time)
        return a.name > b.name;
    else
        return a.size > b.size;
}

void FileInfo::add(string n, int t, size_t s) {
    name = s;
    time = t;
    size = s;
}

void ListSort::add(FileInfo info) {
    filelist->push_back(info);
    size++;
}

void ListSort::add(string n, int t, size_t s) {
    FileInfo info(n, t, s);
    filelist->push_back(info);
    size++;
}

size_t ListSort::listSize() { return size; }

void ListSort::sortNameUp() { filelist->sort(compareNameUp); }

void ListSort::sortTimeUp() { filelist->sort(compareTimeUp); }

void ListSort::sortSizeUp() { filelist->sort(compareSizeUp); }

void ListSort::sortNameDown() { filelist->sort(compareNameDown); }

void ListSort::sortTimeDown() { filelist->sort(compareTimeDown); }

void ListSort::sortSizeDown() { filelist->sort(compareSizeDown); }

list<FileInfo> *ListSort::getList() { return filelist; }
