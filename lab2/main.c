#include <stdio.h>
#include <stdlib.h>

#define BIT_VALUE(val,no_bit) (val>>no_bit)&1

int delay(){
    int i,j;

    for(i=0;i<77777;i++)
        for(j=0;j<7777;j++)
    {

    }
    return 0;
}

int generator(int seed){
    int wynik;

    do
    {
        wynik = BIT_VALUE(seed,0)^BIT_VALUE(seed,1)^BIT_VALUE(seed,4)^BIT_VALUE(seed,5);
        seed = (seed >> 1) | (wynik<<5);
        printf("%d \n",seed);
        delay(100);
    }
    while(seed != 0);

    return 0;

}


int generator2(int seed){
    int wynik,wynik2;
    int seed2=seed;

    do
    {
        wynik = BIT_VALUE(seed,0)^BIT_VALUE(seed,1)^BIT_VALUE(seed,4)^BIT_VALUE(seed,5);
        wynik2 = BIT_VALUE(seed2,0)^BIT_VALUE(seed2,3)^BIT_VALUE(seed2,4)^BIT_VALUE(seed2,5);
        seed2 = (seed2 >> 1) | (wynik2<<5);
        seed = (seed >> 1) | (wynik<<5);
        printf("%d \n",seed2);
        delay(100);
    }
    while(seed != 0);

    return 0;

}

int main()
{

    generator2(5);

    return 0;
}
