#ifndef ISSEND_H
#define ISSEND_H

#include <time.h>
typedef struct {
    int alarmmode;
    time_t time;
    bool ctrl;
    bool ack;
}ISSEND;
class issend
{
public:
    issend(long time);
    bool send(ISSEND now);
private:
    ISSEND last;
    const long deltatime = 300;

};

#endif // ISSEND_H
