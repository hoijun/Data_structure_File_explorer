#include "sort.hpp"

bool compareNameUp(sortfile a, sortfile b) { return a.filename < b.filename; }
bool compareTimeUp(sortfile a, sortfile b) { return a.time < b.time; }
bool compareSizeUp(sortfile a, sortfile b) {
    if (a.size == b.size)
        return a.filename < b.filename;
    else
        return a.size < b.size;
}
bool compareNameDown(sortfile a, sortfile b) { return a.filename > b.filename; }
bool compareTimeDown(sortfile a, sortfile b) { return a.time > b.time; }
bool compareSizeDown(sortfile a, sortfile b) {
    if (a.size == b.size)
        return a.filename > b.filename;
    else
        return a.size > b.size;
}

void sortfile::add(string u, time_t t, string f, string m, off_t s) {
    username = u;
    time = t;
    filename = f;
    tm = m;
    size = s;
}

void ListSort::add(sortfile info) {
    filelist->push_back(info);
    size++;
}

void ListSort::add(string u, time_t t, string f, string m, off_t s) {
    sortfile info(u, t, f, m, s);
    filelist->push_back(info);
    size++;
}

size_t ListSort::listSize() { return filelist->size(); }

void ListSort::sortNameUp() { filelist->sort(compareNameUp); }

void ListSort::sortTimeUp() { filelist->sort(compareTimeUp); }

void ListSort::sortSizeUp() { filelist->sort(compareSizeUp); }

void ListSort::sortNameDown() { filelist->sort(compareNameDown); }

void ListSort::sortTimeDown() { filelist->sort(compareTimeDown); }

void ListSort::sortSizeDown() { filelist->sort(compareSizeDown); }

list<sortfile> *ListSort::getList() { return filelist; }
