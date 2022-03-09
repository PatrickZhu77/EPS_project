
#include "stdio.h"
#include "mppt_pno.h"
#include "math.h"

mppt_data mpptD = {0};
mppt_data *pmpptD = &mpptD;

static double k = 1;
static uint8_t pv_eff = 100;    // 100: solar panel 100% efficiency; 50: 50%; 10: 10%

static double a, c;
static double I_temp = 0;
static double b = 1e-9;
static double b1 = 0;
static double b2 = 2e-9;

void mppt_hunts(mppt_data *data);
void mppt_pno(double V0, double V1, double P0, double P1, mppt_data *data2);
void IV_Curve(double V, mppt_data *data);
void IV_Curve2(double V, mppt_data *data);


int main()
{
    double I[1000]={0};
    double V[1000]={0};
    double P[1000]={0};

/**************15C********************/
    while (I_temp >= Imp_15 + 0.00001 || I_temp < Imp_15)
    {
        a = Isc_15 + b;
        c = 1/Voc_15*(log(a/b));
        I_temp = a - b*exp(c*Vmp_15);

        if(I_temp >= Imp_15)
        {
            b1 = b;
            b = 0.5*(b1 + b2);
        }
        else if(I_temp < Imp_15)
        {
            b2 = b;
            b = 0.5*(b1 + b2);
        }

    }

    a = Isc_15 + b;
    c = 1/Voc_15*(log(a/b));
    I_temp = 0;

    V[0] = 0;
    IV_Curve2(V[0],pmpptD);
    I[0] = pmpptD->preI;
    P[0] = V[0]*I[0];
    printf( "Mppt result I:%lf , V:%lf, P: %lf\n",I[0],V[0],P[0]);

    V[1] = 0.05;
    IV_Curve2(V[1],pmpptD);
    I[1] = pmpptD->preI;
    P[1] = V[1]*I[1];
    pmpptD->dir = 1;
    printf( "Mppt result I:%lf , V:%lf, P: %lf\n",I[1],V[1],P[1]);

    for(int i=2;i<300;i++)
    {
//        if (i<200)
//        {
//            k=1;
//        }
//        else if (i>=200 && i<500)
//        {
//            k=0.5;
//        }
//        else if (i>=500 && i<800)
//        {
//            k=0.1;
//        }
//        else
//        {
//            k=1;
//        }

        a = (Isc_15 + b);
        c = 1/Voc_15*(log(a/b));

        mppt_pno(V[i-2],V[i-1],P[i-2],P[i-1],pmpptD);

        V[i] = pmpptD->preV;
        I[i] = pmpptD->preI;
        P[i] = pmpptD->preP;

        printf( "Mppt result V:%lf , I:%lf, P: %lf\n",V[i],I[i],P[i]);
    }

/****************28C********************/
    b = 1e-9;
    b1 = 0;
    b2 = 2e-9;

    while (I_temp >= Imp_28 + 0.00001 || I_temp < Imp_28)
    {
        a = Isc_28 + b;
        c = 1/Voc_28*(log(a/b));
        I_temp = a - b*exp(c*Vmp_28);

        if(I_temp >= Imp_28)
        {
            b1 = b;
            b = 0.5*(b1 + b2);
        }
        else if(I_temp < Imp_28)
        {
            b2 = b;
            b = 0.5*(b1 + b2);
        }

    }

    a = Isc_28 + b;
    c = 1/Voc_28*(log(a/b));
    I_temp = 0;

    for(int i=300;i<500;i++)
    {

        a = (Isc_28 + b);
        c = 1/Voc_28*(log(a/b));

        mppt_pno(V[i-2],V[i-1],P[i-2],P[i-1],pmpptD);

        V[i] = pmpptD->preV;
        I[i] = pmpptD->preI;
        P[i] = pmpptD->preP;

        printf( "Mppt result V:%lf , I:%lf, P: %lf\n",V[i],I[i],P[i]);
    }

/****************75C********************/
    b = 1e-9;
    b1 = 0;
    b2 = 2e-9;

    while (I_temp >= Imp_75 + 0.00001 || I_temp < Imp_75)
    {
        a = Isc_75 + b;
        c = 1/Voc_75*(log(a/b));
        I_temp = a - b*exp(c*Vmp_75);

        if(I_temp >= Imp_75)
        {
            b1 = b;
            b = 0.5*(b1 + b2);
        }
        else if(I_temp < Imp_75)
        {
            b2 = b;
            b = 0.5*(b1 + b2);
        }

    }

    a = Isc_75 + b;
    c = 1/Voc_75*(log(a/b));
    I_temp = 0;

    for(int i=500;i<700;i++)
    {

        a = (Isc_75 + b);
        c = 1/Voc_75*(log(a/b));

        mppt_pno(V[i-2],V[i-1],P[i-2],P[i-1],pmpptD);

        V[i] = pmpptD->preV;
        I[i] = pmpptD->preI;
        P[i] = pmpptD->preP;

        printf( "Mppt result V:%lf , I:%lf, P: %lf\n",V[i],I[i],P[i]);
    }

/****************15C********************/
    b = 1e-9;
    b1 = 0;
    b2 = 2e-9;

    while (I_temp >= Imp_15 + 0.00001 || I_temp < Imp_15)
    {
        a = Isc_15 + b;
        c = 1/Voc_15*(log(a/b));
        I_temp = a - b*exp(c*Vmp_15);

        if(I_temp >= Imp_15)
        {
            b1 = b;
            b = 0.5*(b1 + b2);
        }
        else if(I_temp < Imp_15)
        {
            b2 = b;
            b = 0.5*(b1 + b2);
        }

    }

    a = Isc_15 + b;
    c = 1/Voc_15*(log(a/b));
    I_temp = 0;

    for(int i=700;i<1000;i++)
    {

        a = (Isc_15 + b);
        c = 1/Voc_15*(log(a/b));

        mppt_pno(V[i-2],V[i-1],P[i-2],P[i-1],pmpptD);

        V[i] = pmpptD->preV;
        I[i] = pmpptD->preI;
        P[i] = pmpptD->preP;

        printf( "Mppt result V:%lf , I:%lf, P: %lf\n",V[i],I[i],P[i]);
    }


/**********************************/
    FILE* fp1 = fopen("IV_output_V_temp.txt","w");

    if(fp1==NULL)
        return 1;

    for(int i=0;i<1000;++i)
        fprintf(fp1,"%lf\n",V[i]);

    fclose(fp1);

/**********************************/
    FILE* fp2 = fopen("IV_output_I_temp.txt","w");

    if(fp2==NULL)
        return 1;

    for(int i=0;i<1000;++i)
        fprintf(fp2,"%lf\n",I[i]);

    fclose(fp2);

/**********************************/
    FILE* fp3 = fopen("IV_output_P_temp.txt","w");

    if(fp3==NULL)
        return 1;

    for(int i=0;i<1000;++i)
        fprintf(fp3,"%lf\n",P[i]);

    fclose(fp3);
/**********************************/


    return 0;

}

/***************************************************************************
 * @brief
 *   Hunt's algorithm to decide the proper value of increment/decrement
 *
 * @param[in] data
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_hunts(mppt_data *data)
{
//    uint8_t n = 1;           //basic increment

    if(data->counter < 8)               //when change with same direction for 200  or less times
    {
        data->increment = 1;
        //data->counter2 = 8;
    }
    else//(data->counter>=10 && data->counter <20)        //when change with same direction for 200 to 400 times //&& (data->counter >= 200)
    {
//        if((int)(data->counter/data->counter2) == 2)
//        {
//
//            data->counter2 = data->counter;
//            data->n++;
//        }
        data->increment = 1 << (int)(data->counter/8);

    }
//    else//(data->counter >= 400)         //when change with same direction for more than 400 times
//    {
//        data->increment = temp_incr*10;
//    }

}

/***************************************************************************
 * @brief
 *   Perturb and observe algorithm to decide the change of boost converter
 *
 * @param[in] data1
 *   Pointer to public data structure of Housekeeping data.
 *
 * @param[in] data2
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/

void mppt_pno(double V0, double V1, double P0, double P1, mppt_data *data2)
{

//    IV_Curve(V,data2);

//    double power = V*data2->preI;

    int temp_int = 0;
    double temp_double = 0;


    if(P1 > P0)         // P(k) > P(k-1), current power greater than previous power
    {
        if(V1 > V0)        // V(k) > V(k-1), current voltage greater than previous voltage
        {
            if(data2->dir == 1)            // If the direction does not change, counter plus one.
            {
                data2->counter++;
            }
            else                           // Otherwise clear the counter.
            {
                data2->counter = 0;
            }

            data2->dir = 1;
            mppt_hunts(data2);
            //printf("Voltage increased by %d V.\n",(int)data2->increment);

        }
        else                                        // V(k) < V(k-1), current voltage smaller than previous voltage
        {
            if(data2->dir == 0)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;
            }

            data2->dir = 0;
            mppt_hunts(data2);
            //printf("Voltage decreased by %d V.\n",(int)data2->increment);

        }

    }
    else                                    // P(k) > P(k-1), current power smaller than previous power
    {
        if(V1 > V0)        // V(k) > V(k-1), current voltage greater than previous voltage
        {
            if(data2->dir == 0)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;
            }

            data2->dir = 0;
            mppt_hunts(data2);
            //printf("Voltage decreased by %d V.\n",(int)data2->increment);

        }
        else                                        // V(k) < V(k-1), current voltage smaller than previous voltage
        {
            if(data2->dir == 1)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;

            }

            data2->dir = 1;
            mppt_hunts(data2);
            //printf("Voltage increased by %d V.\n",(int)data2->increment);

        }

    }

    if(mpptD.dir == 0)
    {
        data2->preV = V1-0.005*mpptD.increment;
    }
    else
    {
        data2->preV = V1+0.005*mpptD.increment;
    }

    IV_Curve2(data2->preV,data2);

    temp_double = (data2->preV*data2->preI)*10000;
    temp_int = (int)temp_double;
    data2->preP = (double)temp_int/10000;

}

void IV_Curve(double V, mppt_data *data)
{
    int temp_int = 0;
    double temp_double = 0;

    if(pv_eff == 100)
    {
        if(V>=0 && V<17.204)
        {
            temp_double = k*(-0.005*V+0.44)*1000;
            temp_int = (int)temp_double;
        }
        else if(V>=17.204 && V<20.004)
        {
            temp_double = k*(1.0/(9*V-182.6)+0.39)*1000;
            temp_int = (int)temp_double;
        }
//        else if(V>=19.761 && V<=20)
//        {
//            temp_double = k*(-0.5*V+10)*1000;
//            temp_int = (int)temp_double;
//        }
        else
        {
            printf("Error. Voltage exceeds range.\n");
        }
    }
//    else if(pv_eff == 50)
//    {
//        if(V>=0 && V<4.916)
//        {
//            temp_double = (-V/80.0+(double)3.0/4)*1000;
//            temp_int = (int)temp_double;
//        }
//        else if(V>=4.916 && V<7.953)
//        {
//            temp_double = (1.0/(V-9.45)+(double)10.0/11)*1000;
//            temp_int = (int)temp_double;
//        }
//        else if(V>=7.953 && V<=8.221)
//        {
//            temp_double = (-0.9*V+7.399)*1000;
//            temp_int = (int)temp_double;
//        }
//        else
//        {
//            printf("Error. Voltage exceeds range.\n");
//        }
//    }
//    else if(pv_eff == 10)
//    {
//        if(V>=0 && V<4.294)
//        {
//            temp_double = (-V/80.0+0.15)*1000;
//            temp_int = (int)temp_double;
//        }
//        else if(V>=4.294 && V<6.219)
//        {
//            temp_double = (1.5/(V-11.0)+0.32)*1000;
//            temp_int = (int)temp_double;
//        }
//        else if(V>=6.219 && V<=6.25)
//        {
//            temp_double = (-0.2*V+1.25)*1000;
//            temp_int = (int)temp_double;
//        }
//        else
//        {
//            printf("Error. Voltage exceeds range.\n");
//        }
//    }


    data->preI = (double)temp_int/1000;
}

void IV_Curve2(double V, mppt_data *data)
{
    data->preI = a - b*exp(c*V);
}
