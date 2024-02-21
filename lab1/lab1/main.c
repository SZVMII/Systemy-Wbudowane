#include <stdio.h>
#include <stdlib.h>
#define BIT_VALUE(val,no_bit) (val>>no_bit)&1
unsigned int counter = 0;

void delay(){
    int a,b;
    for(a=0;a<=100000000000000000000;a++){
        for(b=0;b<=500;b++){
        }
    }
}

int main(void)
{
    int gray(unsigned char input){
        return(input >> 1)^input;
    }

    while(1){
        printf("\r");

        for(int i=7; i>=0; i--){
            printf("%d",BIT_VALUE(counter,i));
        }

        printf("         ");

        for(int i=7; i>=0; i--){
            printf("%d",BIT_VALUE(gray(counter),i));
        }
    }

    counter++;
    delay();
}
