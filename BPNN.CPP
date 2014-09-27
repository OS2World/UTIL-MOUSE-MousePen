// Backpropogation Neural Network for Numerical character recognition
// By ROBERT ALDRIDGE - 17/09/94

#define INCL_VIO

#include <os2.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "num.h"

#define HI 0.8
#define LOW 0.2


typedef struct Neuron {
   float output;
   float weight[nn];
   float s;
} neuron;

neuron layer1[nn],layer2[nn];              // 2 Layers of Neurons

short ncnt, wcnt, tcnt;                    // A couple of counters

float L1delta[nn], L2delta[nn], temp, t, d;

FILE *fptr;

void weightinit();
void calcoutput();
void adjustweights();
void clrscr();
void saveweights();
void testnet();
void loadweights();
void roundup(float *dnum);

int main()
{
   short cnt2, iter;
   weightinit();
   clrscr();
   cout << "Enter No. of Passes : ";
   cin >> iter;
   cout << endl << "Enter Training Constant : ";
   cin >> d;
   cout << endl << "Enter Threshold Value : ";
   cin >> t;
   // Main Loop
   if(iter>=1) {
      for(cnt2=0; cnt2<iter; cnt2++) {
         tcnt=0;
         while(tcnt<inps) {
            VioSetCurPos(6,0,0);
            cout << "Pass : " << cnt2 << endl;
            calcoutput();
            adjustweights();
            tcnt++;
         }
      }
   saveweights();
   }
   loadweights();
   testnet();
   return 0;
}

// Initialize weights with random values
void weightinit()
{
   for(ncnt=0; ncnt<nn; ncnt++) {                             // Neuron Loop
      for(wcnt=0; wcnt<nn; wcnt++) {                          // Weight Loop
         layer1[ncnt].weight[wcnt]=0.000001*rand();
         layer2[ncnt].weight[wcnt]=0.000001*rand();
      }
   }
}


// Calculate outputs
void calcoutput()
{
   // Input Layer - layer1
   for(ncnt=0; ncnt<nn; ncnt++) {                              // Neuron Loop
      temp=0;
      for(wcnt=0; wcnt<nn; wcnt++)                             // Weight Loop
         temp+=(input[tcnt][wcnt]*layer1[ncnt].weight[wcnt])-t;
      layer1[ncnt].s=temp;
      layer1[ncnt].output=(1/(1+exp(-layer1[ncnt].s)));
   }

   // First hidden layer - layer2
   for(ncnt=0; ncnt<nn; ncnt++) {                              // Neuron Loop
      temp=0;
      for(wcnt=0; wcnt<nn; wcnt++)                             // Weight Loop
         temp+=(layer1[wcnt].output*layer2[ncnt].weight[wcnt])-t;
      layer2[ncnt].s=temp;
      layer2[ncnt].output=(1/(1+exp(-layer2[ncnt].s)));
   }

}

void adjustweights()
{
   float sum;
   int scnt;
   // Layer 2
   for(ncnt=0; ncnt<nn; ncnt++) {             // Find Delta for each Neuron
      L2delta[ncnt]=layer2[ncnt].output*(1-layer2[ncnt].output)
                    *(correctout[tcnt][ncnt]-layer2[ncnt].output);
         for(wcnt=0; wcnt<nn; wcnt++)         // Adjust each weight
            layer2[ncnt].weight[wcnt]+=d*L2delta[ncnt]*layer1[wcnt].output;
   }


   // Layer 1
   for(ncnt=0; ncnt<nn; ncnt++) {             // Find Error of each Neuron
      sum=0;
      for(scnt=0; scnt<nn; scnt++)
         sum+=L2delta[scnt]*layer2[scnt].weight[ncnt];
      L1delta[ncnt]=layer1[ncnt].output*(1-layer1[ncnt].output)*sum;
      for(wcnt=0; wcnt<nn; wcnt++)
         layer1[ncnt].weight[wcnt]+=d*L1delta[ncnt]*input[tcnt][wcnt];
   }
}

// OS/2 API Calls to Clear the screen
void clrscr()
{
   BYTE bCell[2];
   bCell[0] = 0x20;
   bCell[1] = 0x00;
   VioScrollDn(0,0,0xFFFF,0xFFFF,0xFFFF,bCell,0);
   VioSetCurPos(0,0,0);
}

void saveweights()
{
   char fname[14];
   cout << "Enter Weight filename : ";
   cin >> fname;
   fptr=fopen(fname,"wb");
   for(ncnt=0; ncnt<nn; ncnt++) {
      fwrite(layer1[ncnt].weight, sizeof(layer1[ncnt].weight),1,fptr);
      fwrite(layer2[ncnt].weight, sizeof(layer2[ncnt].weight),1,fptr);
   }
    fclose(fptr);
}

void loadweights()
{
   char fname[14];
   cout << "Enter Weight filename : ";
   cin >> fname;
   fptr=fopen(fname,"rb");
   for(ncnt=0; ncnt<nn; ncnt++) {
      fread(layer1[ncnt].weight,sizeof(layer1[ncnt].weight),1,fptr);
      fread(layer2[ncnt].weight,sizeof(layer2[ncnt].weight),1,fptr);
   }
    fclose(fptr);
}


void testnet()
{
   short cnt, cnt2;
   for(cnt2=0; cnt2<inps; cnt2++) {
      cout << endl << "Correct  Real" << endl;
      tcnt=cnt2;
      calcoutput();
      for(cnt=0; cnt<64; cnt++) {
         // roundup(&layer2[cnt].output);
         cout << correctout[cnt2][cnt] << "        " << layer2[cnt].output << endl;
      }
   }
}

void roundup(float* dnum)
{
   if(*dnum<LOW)
      *dnum=0;
   else if(*dnum>HI)
      *dnum=1;
}

