#include "tempwriter.h"
#include "common.h"
#include <sys/stat.h>
#include <unistd.h>
#define DATETIME_SIZE  32
tempwriter::tempwriter()
{
    struct stat stat_buf;
    stat(sd_path.c_str(),&stat_buf);
    cout<<"file size(K):"<<stat_buf.st_size/1024<<endl;
    if(stat_buf.st_size>=1024*1024)
    {
        string date = common::GetDate();
        string new_name = "/mnt/sd_card/temp-"+date+".txt";
        rename(sd_path.c_str(),new_name.c_str());
    }
    file.open(sd_path,ios::app);
    is_open = file.is_open();
}

void tempwriter::write(TEMP_C temp_c,string rectname,int ptat)
{
    if(is_open)
    {
        string time = common::getsystimebyzone();
        float PTAT =ptat/10-273.15f;
        file<<time<<" "<<rectname<<" "<<temp_c.highTemp<<" "<<temp_c.lowTemp<<" "<<PTAT<<endl;
        file.close();
    }
    else
        cout<<"tempwriter open failed!"<<endl;
}
