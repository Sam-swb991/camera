#include "calc.h"
#include <math.h>
#include "common.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
calc::calc()
{
    spi = new spiCtrl();
    sctrl = new sensorCtrl(spi);
    sctrl->init_sensor();


    ectrl = new eepromCtrl(spi);
    ectrl->readAll();
    //ectrl->printvalue();
    E_C_DATA *ec_data = ectrl->get_common_data();
    pixC_delta = (ec_data->pixCmax - ec_data->pixCmin)/65535;
    pixC_muti = ec_data->epsilon*ec_data->GlobalGrain/1000000;
    pow_gradscale = pow(2,ec_data->gradscale);
    pow_vddscgrad = pow(2,ec_data->vddScGrad);
    pow_vddscoff = pow(2,ec_data->vddScOff);
    VDD_delta = ec_data->VDDTH2-ec_data->VDDTH1;
    PTAT_delta = ec_data->PTATTH2 - ec_data->PTATTH1;
    pixCmin = ec_data->pixCmin;
    VDDTH1 = ec_data->VDDTH1;
    PTATTH1 = ec_data->PTATTH1;
    PTAT_gradient = ec_data->PTAT_gradient;
    PTAT_offset = ec_data->PTAT_offset;
    DeadPixAdr[0] = 63*80+60;
    DeadPixAdr[1] = (63-8)*80+60;
    DeadPixAdr[2] = (63-16)*80+60;
    DeadPixAdr[3] = (63-24)*80+60;
//    DeadPixAdr[4] = 63*80+40;
//    DeadPixAdr[5] = (63-8)*80+40;
//    DeadPixAdr[6] = (63-16)*80+40;
//    DeadPixAdr[7] = (63-24)*80+40;

}
calc::~calc()
{
    spi->close();
}
void calc::get_all_temp(int **temp)
{
    sctrl->read_all();
    //sctrl->printvalue();

    S_C_DATA *sc_data = sctrl->get_common_data();
    printf("ta is %d\n",static_cast<int>(sc_data->PTAT_av*PTAT_gradient+PTAT_offset));
    for(int i =0;i<5120;++i)
    {
        E_M_DATA *em_data = ectrl->get_muti_data(i);
        S_M_DATA *sm_data = sctrl->get_muti_data(i);

        temp[i/80][i%80] = get_one_temp(em_data,sm_data,sc_data);
        delete em_data;
        delete sm_data;

    }
//    for(int i =0;i<64;++i)
//    {
//        for(int j=0;j<80;++j)
//        {
//            printf("%d ",temp[i][j]);
//        }
//        printf("\n");
//    }
    sort_temp(temp);

    //repaire bad value
//    for(int i=0;i<4;i++)
//    {

//        int x = DeadPixAdr[i]/80;
//        int y = DeadPixAdr[i]%80;
//        if(x == 63)
//        {
//            temp[x][y] = ((temp[x-1][y-1]+temp[x-1][y]+temp[x-1][y+1]+
//                    temp[x][y-1]+temp[x][y+1])/5);
//            //cout<<"bad value repaire is :"<<temp[x][y]<<endl;
//        }
//        else
//        {
//            temp[x][y] = (temp[x-1][y-1]+temp[x-1][y]+temp[x-1][y+1]+
//                    temp[x][y-1]+temp[x][y+1]+temp[x+1][y-1]+temp[x+1][y]+
//                    temp[x+1][y+1])/8;
//            //cout<<"bad value repaire is :"<<temp[x][y]<<endl;
//        }
//    }

    delete sc_data;
}
void calc::change_sort(int *src0,int *src1,size_t len)
{
 //   printf("********************************\n");
    int *tmp = new int[len];
    memcpy(tmp,src0,len*4);
//    for(int i=0;i<80;i++)
//        printf("%d ",tmp[i]);
//    printf("\n\n");
    memcpy(src0,src1,len*4);
    memcpy(src1,tmp,len*4);
    delete []tmp;

}
void calc::sort_temp(int **temp)
{

    for(int i =0;i<16;++i)
    {
 //       printf("i = %d\n",i);
        change_sort(temp[63-i],temp[32+i],80);
    }
}
int calc::get_one_temp(E_M_DATA* em_data,S_M_DATA* sm_data,S_C_DATA * sc_data)
{
   // printf("start!\n");
    int PTAT_av = sc_data->PTAT_av;

    int VDD_av = sc_data->VDD_av;

    int Ta =static_cast<int>(PTAT_av*PTAT_gradient+PTAT_offset);

    int temp = static_cast<int>(em_data->thGrad);

    temp *=PTAT_av;
    temp /=pow_gradscale;

    int V_ij_comp = sm_data->pixel-temp-em_data->thOffSet;

    int V_ij_comp_x =V_ij_comp-sm_data->ele_offset;

    int V_ij_vdd_comp = static_cast<int>((em_data->vddCompGrad*PTAT_av)/pow_vddscgrad);
    V_ij_vdd_comp +=em_data->vddCompOff;
    temp =VDD_av-VDDTH1-(VDD_delta/PTAT_delta)*(PTAT_av-PTATTH1);
    V_ij_vdd_comp *=temp;
    V_ij_vdd_comp /=pow_vddscoff;
    V_ij_vdd_comp = V_ij_comp_x - V_ij_vdd_comp;

    float PixC_ij = (em_data->p*pixC_delta+pixCmin)*pixC_muti;


    int V_ij_pixC = static_cast<int>(V_ij_vdd_comp *pow(10,8)/static_cast<double>(PixC_ij));

    int x=-1,y;
    for(int i = 0 ;i<NROFTAELEMENTS;++i)
    {
        if(static_cast<int>(common::XTATemps[i])<=Ta&&static_cast<int>(common::XTATemps[i+1])>Ta)
        {
            x=i;
            break;
        }
    }
    if(x==-1)
    {
        printf("calc is error!\n");
    }
    int val = V_ij_pixC +TABLEOFFSET;

    y = val>>ADEXPBITS;

    unsigned int temp_y_x = common::TempTable[y][x];
    unsigned int temp_y_x1 = common::TempTable[y][x+1];
    unsigned int temp_y1_x = common::TempTable[y+1][x];
    unsigned int temp_y1_x1 = common::TempTable[y+1][x+1];
    unsigned int D_Ta = static_cast<unsigned int>(Ta)-common::XTATemps[x];
    unsigned int Vx = (temp_y_x1-temp_y_x)*D_Ta/TAEQUIDISTANCE+temp_y_x;
    unsigned int Vy = (temp_y1_x1-temp_y1_x)*D_Ta/TAEQUIDISTANCE+temp_y1_x;

    unsigned int Tobject = (Vy-Vx)*(static_cast<unsigned int>(val)-common::YADValues[y])/ADEQUIDISTANCE+Vx;
    float K = -273.15f;
    float ret = static_cast<float>(Tobject)/10 +K;
#ifdef DEBUG
    printf("PTAT_AV is %d\n",PTAT_av);
    printf("VDD_AV is %d\n",VDD_av);
    printf("Ta is %d\n",Ta);
    printf("temp is %d\n",temp);
    printf("pixel is %d\n",sm_data->pixel);
    printf("V_ij_comp is %d  thGrad is :%02x  thoffset is :%d\n",V_ij_comp,em_data->thGrad,em_data->thOffSet);
    printf("ele_offset is %d\n",sm_data->ele_offset);
    printf("V_ij_comp_x is %d\n",V_ij_comp_x);
    printf("V_ij_vdd_comp is %d vddCompGrad is :%d vddcompoff is %d\n",V_ij_vdd_comp,em_data->vddCompGrad,em_data->vddCompOff);
    std::cout<<"pixc_ij is :"<<PixC_ij<<endl;
    cout<<"P is :";
    common::print_V(&em_data->p,1,2);
    cout<<endl;
    printf("V_ij_PicC is :%d\n",V_ij_pixC);
    printf("val is :%d\n",val);
    printf("x = %d,y = %d\n",x,y);
    cout<<"1:"<<temp_y_x<<" 2:"<<temp_y_x1<<" 3:"<<temp_y1_x<<" 4:"<<temp_y1_x1<<endl;
    cout<<"vx :"<<Vx<<" vy :"<<Vy<<endl;
#endif
    return static_cast<int>(ret);


}
