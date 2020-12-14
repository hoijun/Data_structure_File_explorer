#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

int makedirectory_main() {

    char name[100];
    cout << "directory name : ";
    cin >> name;
    // Creating a directory
    if (mkdir(name, 0777) == -1)
        cerr << "Error :  " << strerror(errno) << endl;
    //안될경우 메세지 출력
    else
        cout << "Directory created\n";
}
