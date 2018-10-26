#ifndef MYQUEEN_H
#define MYQUEEN_H
#include <queue>
using namespace std;
class myqueue
{
public:
    myqueue();
    void add(int **);
    int ** getfirst();
    int size();
private:
    queue<int **> q;
};

#endif // MYQUEEN_H
