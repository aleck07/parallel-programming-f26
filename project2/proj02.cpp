#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

//Use this to disable barriers and compare to proper thread synchronization
// #define DISABLE_BARRIERS

omp_lock_t Lock;
volatile int NumInThreadTeam;
volatile int NumAtBarrier;
volatile int NumGone;
float Sqr(float x)
{
    return x * x;
}

void InitBarrier(int n)
{
    NumInThreadTeam = n; // number of threads you want to block at the barrier
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
}
void WaitBarrier()
{
#ifndef DISABLE_BARRIERS
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if (NumAtBarrier == NumInThreadTeam) // release the waiting threads
        {
            NumGone = 0;
            NumAtBarrier = 0;
            // let all other threads return before this one unlocks:
            while (NumGone != NumInThreadTeam - 1)
                ;
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);
    while (NumAtBarrier != 0)
        ;          // all threads wait here until the last one arrives â€¦
#pragma omp atomic // â€¦ and sets NumAtBarrier to 0
    NumGone++;
#endif
}

float Ranf(float low, float high)
{
    float r = (float)rand(); // 0 - RAND_MAX

    return (low + r * (high - low) / (float)RAND_MAX);
}


int	NowYear;		// 2024- 2030
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int NowNumAliens;   // number of aliens in the current population

const int END_YEAR = 2032;

const float GRAIN_GROWS_PER_MONTH =	       12.0; // inches, default 12
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		10.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

const float ONE_ALIEN_EATS_PER_MONTH = 1.0; // In this similation aliens eat deer
const float INVASION_CHANCE = 0.1;
const float INVASION_AMOUNT = 3;

void Watcher()
{
    while (NowYear < END_YEAR)
    {
        // do nothing
        WaitBarrier(); // 1.
        // do nothing
        WaitBarrier(); // 2.
        //<< write out the â€œNowâ€ state of data >>
        printf("%d, %d, %f, %f, %f, %d, %d\n", NowYear, NowMonth, NowPrecip, NowTemp, NowHeight, NowNumDeer, NowNumAliens);
        // Advance time
        NowMonth++;
        if (NowMonth > 11)
        {
            NowYear++;
            NowMonth = 0;
        }
        // Re-compute environment variables
        float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

        float temp = AVG_TEMP - AMP_TEMP * cos(ang);
        NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
        NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
        if (NowPrecip < 0.)
        {
            NowPrecip = 0.;
        }
        WaitBarrier(); // 3.
    }
}

void Deer()
{
    while (NowYear < END_YEAR)
    {
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)( NowHeight );
        if( nextNumDeer < carryingCapacity ) {  // When theres lots of grain, more deer can be created
            if (carryingCapacity > 2 * nextNumDeer)
                nextNumDeer += 2;
            else
                nextNumDeer++;
        }
        else
        if( nextNumDeer > carryingCapacity )
                nextNumDeer--;
        nextNumDeer -= (int)(NowNumAliens * ONE_ALIEN_EATS_PER_MONTH);
        if( nextNumDeer < 0 )
                nextNumDeer = 0;
        WaitBarrier(); // 1.
        // Copy the computed next state to the Now state
        NowNumDeer = nextNumDeer;
        WaitBarrier(); // 2.
        // // Do nothing
        WaitBarrier(); // 3.
    }
}

void Grain()
{
    while (NowYear < END_YEAR)
    {
        // Compute next state
        float tempFactor = exp(-Sqr((NowTemp - MIDTEMP) / 10.));

        float precipFactor = exp(-Sqr((NowPrecip - MIDPRECIP) / 10.));

        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        if (nextHeight < 0.) {
            nextHeight = 0.;
        }
        WaitBarrier(); // 1.
        // Copy the computed next state to the Now state
        NowHeight = nextHeight;
        WaitBarrier(); // 2.
        // Do nothing
        WaitBarrier(); // 3.
    }
}

void Alien()
{
    while (NowYear < END_YEAR) // Aliens invade in 2025
    {
        int nextNumAliens = NowNumAliens;
        int alienCapacity = NowNumDeer / 3; // 3 deer per alien
        if (nextNumAliens < alienCapacity)
            nextNumAliens++;
        else if (nextNumAliens > alienCapacity)
            nextNumAliens--;
        //Alien invasion 
        if(Ranf(0., 1.) < INVASION_CHANCE)
            nextNumAliens += INVASION_AMOUNT;
        if (nextNumAliens < 0)
            nextNumAliens = 0;
        WaitBarrier(); // 1.
        NowNumAliens = nextNumAliens;
        WaitBarrier(); // 2.
        WaitBarrier(); // 3.
    }
}

int main(int argc, char *argv[])
{
    printf("Year, Month, Precipitation, Temperature, Grain Height, Num Deer, Num Alien\n");

    // starting date and time:
    NowMonth =    0;
    NowYear  = 2024;

    // starting state (feel free to change this if you want):
    NowNumDeer = 2;
    NowHeight =  5.;
    NowNumAliens = 1;

    InitBarrier(4);
    omp_set_num_threads(4); // same as # of sections
#pragma omp parallel sections
    {
#pragma omp section
        {
            Deer();
        }

#pragma omp section
        {
            Grain();
        }

#pragma omp section
        {
            Watcher();
        }

#pragma omp section
        {
            Alien();
        }
    } // implied barrier -- all functions must return in order
      // to allow any of them to get past here
    return 0;
}
