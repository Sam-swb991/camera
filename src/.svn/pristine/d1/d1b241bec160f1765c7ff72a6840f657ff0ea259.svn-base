#ifndef RECOVERY_H
#define RECOVERY_H
#include "sharedspace.h"

class recovery
{
public:
    recovery(sharedspace *ss);
    int getret();
private:
    int cp_default_file(const char *src, const char *dst);
    void recover_sql();
    sharedspace *ss;
    int ret =0;
};

#endif // RECOVERY_H
