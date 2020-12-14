#include "sort.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;
int main() {
    sortfile first("first", 1, 2);
    cout << first.name << endl;
    sortfile second("second", 2, 1);
    sortfile third("third", 3, 3);

    ListSort slist = ListSort();
    slist.add(first);
    slist.add(second);
    slist.add(third);

    slist.sortSizeDown();
    list<sortfile> *getlist = slist.getList();

    list<sortfile>::iterator it;
    for (it = getlist->begin(); it != getlist->end(); it++){
        cout << it->name << endl;
    }

    return 0;
}
