#include "myqueue.h"
#include <iostream>
using namespace std;
myqueue::myqueue()
{

}

void myqueue::add(int **temp)
{
    if(q.size()==60)
    {
        q.pop();
        cout<<"queue pop"<<endl;
    }
    q.push(temp);

}

int ** myqueue::getfirst()
{
    return q.front();
}

int myqueue::size()
{
    return static_cast<int>(q.size());
}
