/* PM User Interface for Back propogation Neural Network */
/* for Numerical Character Recognition */
/* By ROBERT ALDRIDGE - 10/10/94 */

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS

#define ID_BUTTON1 1
#define ID_BUTTON2 2
#define ID_BUTTON3 3

#define HI 0.4
#define LOW 0.1
#define nn 64


#include <os2.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct Neuron {
   float output;
   float weight[nn];
   float s;
} neuron;

neuron layer1[nn],layer2[nn];              /* 2 Layers of Neurons */

short ncnt, wcnt;                          /* A couple of counters */

float temp, t=0;

static LONG charcell[nn];
static float chararray[nn];
int result=0,cnt,oldresult=0;
char commentarr[nn]="0123456789KLMNOPQRSTUVWXYZ";

FILE *infile,*outfile,*outfile2;

MRESULT EXPENTRY WindowProc(HWND, ULONG, MPARAM, MPARAM);

void calcoutput();
void roundup(float *dnum);
void loadweights();

int main(int argc,char *argv[])
{
   char fname[12];
   static CHAR title[] = "Mouse Pen ";
   static CHAR ClientClass[] = "GridWindow";
   static ULONG flFrameFlags =   FCF_TITLEBAR | FCF_SYSMENU | FCF_BORDER |
                                 FCF_TASKLIST | FCF_MINBUTTON;
   HAB   hab;
   HMQ   hmq;
   HWND  hwndFrame, hwndClient;
   QMSG  qmsg;

   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue(hab, 0);

   WinRegisterClass (
                  hab,
                  ClientClass,
                  WindowProc,
                  CS_SYNCPAINT,
                  0);

   hwndFrame = WinCreateStdWindow(
                                 HWND_DESKTOP,
                                 0,
                                 &flFrameFlags,
                                 ClientClass,
                                 title,
                                 0L,
                                 0,
                                 0,
                                 &hwndClient);

  

   WinSetWindowPos(hwndFrame,HWND_TOP,200,200,160,200,SWP_SIZE | SWP_MOVE );
   WinShowWindow(hwndFrame,TRUE);

   WinCreateWindow(
                  hwndClient,
                  WC_BUTTON,
                  "Clear",
                  WS_VISIBLE | BS_PUSHBUTTON,
                  100,
                  10,
                  50,
                  25,
                  hwndClient,
                  HWND_BOTTOM,
                  ID_BUTTON1,
                  NULL,
                  NULL);

   WinCreateWindow(
                  hwndClient,
                  WC_BUTTON,
                  "Save",
                  WS_VISIBLE | BS_PUSHBUTTON,
                  100,
                  45,
                  50,
                  25,
                  hwndClient,
                  HWND_BOTTOM,
                  ID_BUTTON2,
                  NULL,
                  NULL);

   WinCreateWindow(
                  hwndClient,
                  WC_BUTTON,
                  "Start",
                  WS_VISIBLE | BS_PUSHBUTTON,
                  100,
                  80,
                  50,
                  25,
                  hwndClient,
                  HWND_BOTTOM,
                  ID_BUTTON3,
                  NULL,
                  NULL);

    strcpy(fname,argv[1]);
    if( (infile=fopen(fname,"rb"))==NULL )
      exit(0);
    loadweights();
    outfile=fopen("inputs.dat","w");
    outfile2=fopen("outputs.dat","w");

   while(WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
      WinDispatchMsg(hab,&qmsg);

   WinDestroyWindow(hwndFrame);
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return 0;
}

MRESULT EXPENTRY WindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   int xpos,ypos,row=0;
   static POINTL points,mpos,textpos;
   HPS pspace;
   static BOOL button1;
   static SHORT charcnt;
   static char resultmsg[]="Char ?";
   

   switch(msg) {

      case WM_CREATE :
         for(cnt=1; cnt<=nn; cnt++)
         charcell[cnt]=DRO_OUTLINE;
         charcnt=0;
         return 0;


      case WM_COMMAND :
         switch(COMMANDMSG(&msg)->cmd) {
            case ID_BUTTON1 :
                for(cnt=1; cnt<=nn; cnt++)
                   charcell[cnt]=DRO_OUTLINE;
                result=oldresult=0;
                sprintf(resultmsg,"Char ?");
                WinInvalidateRect(hwnd,NULL,TRUE);
                return 0;
             case ID_BUTTON2 :
                 for(cnt=0; cnt<=64; cnt++) {
                  if(charcell[cnt]==DRO_FILL)
                     chararray[cnt]=1;
                  else
                     chararray[cnt]=0;
               }
               for(cnt=1; cnt<=64; cnt++)
                  fprintf(outfile,"%d,",(int)chararray[cnt]);
               fprintf(outfile,"/* Char %c */\n",commentarr[charcnt++]);
               return 0;
            case ID_BUTTON3 :
               for(cnt=1; cnt<=nn; cnt++) {
                  if(charcell[cnt]==DRO_FILL)
                     chararray[cnt]=1;
                  else
                     chararray[cnt]=0;
               }
               calcoutput();
               for(ncnt=0; ncnt<nn; ncnt++) {
                  fprintf(outfile2,"%f \n",layer2[ncnt].output);
                  roundup(&layer2[ncnt].output);
               }
               oldresult=result;
               for(ncnt=0; ncnt<nn; ncnt++) {
                  if(layer2[ncnt].output==1) {
                     result=ncnt;
                     break;
                  }
               } 
               if(oldresult==result)
                  DosBeep(1000,100);
               for(ncnt=1; ncnt<=nn; ncnt++)
                  fprintf(outfile2,"%d, ",(int)chararray[ncnt]);
               fprintf(outfile2," - %c - \n\n",commentarr[result]);
               sprintf(resultmsg,"Char %c",commentarr[result]); 
               textpos.x=10;
               textpos.y=100;
               pspace=WinGetPS(hwnd);
               GpiSetColor(pspace,CLR_BLUE);
               GpiSetBackMix(pspace,BM_OVERPAINT);
               GpiCharStringAt(pspace,&textpos,8,(PCH)"         ");
               textpos.x=10;
               textpos.y=100;
               GpiCharStringAt(pspace,&textpos,8,(PCH)resultmsg);
               WinReleasePS(pspace);
               return 0;

         }
         break;



      case WM_BUTTON1DOWN :
         pspace=WinGetPS(hwnd);
         GpiSetColor(pspace,CLR_BLACK);
         mpos.x=MOUSEMSG(&msg)->x;
         mpos.y=MOUSEMSG(&msg)->y;
         if(mpos.x<80 && mpos.y<80) {
            xpos=mpos.x/10;
            ypos=mpos.y/10;
            row=7-ypos;
            charcell[xpos+(row*8)+1]=DRO_FILL;
            button1=TRUE;
            WinSetCapture(HWND_DESKTOP,hwnd);
            points.x=(xpos*10)+1;
            points.y=((ypos*10)+10)-1;
            GpiMove(pspace,&points);
            points.x+=8;
            points.y-=8;
            GpiBox(pspace,charcell[xpos+(row*8)+1],&points,0,0);
         }
         WinReleasePS(pspace);
         break;
      case WM_BUTTON1DBLCLK :
         pspace=WinGetPS(hwnd);
         GpiSetBackMix(pspace,BM_OVERPAINT);
         GpiSetColor(pspace,CLR_WHITE);
         mpos.x=MOUSEMSG(&msg)->x;
         mpos.y=MOUSEMSG(&msg)->y;
         if(mpos.x<80 && mpos.y<80) {
            xpos=mpos.x/10;
            ypos=mpos.y/10;
            row=7-ypos;
            charcell[xpos+(row*8)+1]=DRO_OUTLINE;
            points.x=(xpos*10)+1;
            points.y=((ypos*10)+10)-1;
            GpiMove(pspace,&points);
            points.x+=8;
            points.y-=8;
            GpiBox(pspace,DRO_FILL,&points,0,0);
            GpiSetColor(pspace,CLR_BLACK);
            GpiBox(pspace,charcell[xpos+(row*8)+1],&points,0,0);
         }
         WinReleasePS(pspace);
         break;

      case WM_MOUSEMOVE :
         pspace=WinGetPS(hwnd);
         GpiSetColor(pspace,CLR_BLACK);
         if(button1==TRUE) {
            mpos.x=MOUSEMSG(&msg)->x;
            mpos.y=MOUSEMSG(&msg)->y;
            if(mpos.x<80 && mpos.y<80) {
               xpos=mpos.x/10;
               ypos=mpos.y/10;
               row=7-ypos;
               charcell[xpos+(row*8)+1]=DRO_FILL;
               points.x=(xpos*10)+1;
               points.y=((ypos*10)+10)-1;
               GpiMove(pspace,&points);
               points.x+=8;
               points.y-=8;
               GpiBox(pspace,charcell[xpos+(row*8)+1],&points,0,0);
            }
         }
         WinReleasePS(pspace);
         break;

      case WM_BUTTON1UP :
         button1=FALSE;
         WinSetCapture(HWND_DESKTOP,NULLHANDLE);
         return 0;


      case WM_PAINT :
         pspace = WinBeginPaint(hwnd, NULLHANDLE, NULL);
         GpiErase(pspace);
         GpiSetColor(pspace,CLR_RED);
         for(cnt=0; cnt<=80; cnt+=10) {
         points.x=cnt;
         points.y=0;
         GpiMove(pspace,&points);
         points.x=cnt;
         points.y=80;
         GpiLine(pspace,&points);
         points.x=0;
         points.y=cnt;
         GpiMove(pspace,&points);
         points.x=80;
         GpiLine(pspace,&points);
         }
         GpiSetColor(pspace,CLR_BLACK);
         for(ypos=0; ypos<8; ypos++) {
            for(xpos=0; xpos<8; xpos++) {
               points.x=(xpos*10)+1;
               points.y=(80-(ypos*10))-1;
               GpiMove(pspace,&points);
               points.x+=8;
               points.y-=8;
               GpiBox(pspace,charcell[xpos+(row*8)+1],&points,0,0);
            }
            row++;
         }
         textpos.x=10;
         textpos.y=100;
         GpiSetColor(pspace,CLR_PALEGRAY);
         GpiCharStringAt(pspace,&textpos,7,(PCH)"       ");
         textpos.x=10;
         textpos.y=100;
         GpiCharStringAt(pspace,&textpos,7,(PCH)resultmsg);
         WinEndPaint(pspace);
         return 0;

    case WM_DESTROY :
         fclose(outfile);
         fclose(outfile2);
         return 0;
   }
   return WinDefWindowProc(hwnd,msg,mp1,mp2);
}

void calcoutput()
{
   /* Input Layer - layer1 */
   for(ncnt=0; ncnt<nn; ncnt++) {                              /* Neuron Loop */
      temp=0;
      for(wcnt=0; wcnt<nn; wcnt++)                             /* Weight Loop */
         temp+=(chararray[wcnt+1]*layer1[ncnt].weight[wcnt])-t;
      layer1[ncnt].s=temp;
      layer1[ncnt].output=(1/(1+exp(-layer1[ncnt].s)));
   }

   /* First hidden layer - layer2 */
   for(ncnt=0; ncnt<nn; ncnt++) {                              /* Neuron Loop */
      temp=0;
      for(wcnt=0; wcnt<nn; wcnt++)                             /* Weight Loop */
         temp+=(layer1[wcnt].output*layer2[ncnt].weight[wcnt])-t;
      layer2[ncnt].s=temp;
      layer2[ncnt].output=(1/(1+exp(-layer2[ncnt].s)));
   }
   DosBeep(500,100);
}


void roundup(float* dnum)
{
   if(*dnum<LOW)
      *dnum=0;
   else if(*dnum>HI)
      *dnum=1;
}


void loadweights()
{
   for(ncnt=0; ncnt<nn; ncnt++) {
      fread(layer1[ncnt].weight,sizeof(layer1[ncnt].weight),1,infile);
      fread(layer2[ncnt].weight,sizeof(layer2[ncnt].weight),1,infile);
   }
   fclose(infile);
}

