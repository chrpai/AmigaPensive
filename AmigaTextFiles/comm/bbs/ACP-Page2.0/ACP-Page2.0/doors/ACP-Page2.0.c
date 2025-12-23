#include <libraries/gtlayout.h>
#include <proto/timer.h>
#include <devices/timer.h>
#include <exec/devices.h>
#include <proto/dos.h>
#include <proto/gadtools.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <proto/exec.h>

struct GTLayoutBase *GTLayoutBase;
struct Library *TimerBase;
struct LayoutHandle *Handle;
struct timerequest *MyIO;
struct MsgPort *MyMSG;
struct EClockVal EClock;

//* ShowAnim
int ShowAnim(void)
    {
    FILE *anim;
    int c;

    anim = fopen("Doors:ACP-Page.ansianim","r");
    if (!anim)
           {
           printf ("%s\n","Can't find Doors:ACP-Page.ansianim");
           return(0);
           }
        else
           {
           c=32;
           while (!feof(anim))
              {
              fputc(c,stdout);
              c=fgetc(anim);
              }
           fclose(anim);
           fputc(0x1b,stdout); printf("[5A");
           fputc(0x1b,stdout); printf("[1C");
           flushall();
           return(1);
           }
    }
///
//* InReason
char *InReason(void)
    {
    char input[80];
    BPTR file;
    int cnt;
    char in;
    BOOL Done = FALSE;

    file = Open("*",MODE_OLDFILE);
    SetMode(file,1);
    cnt=0; input[0]=0;
    do
        {
        in=FGetC(file);
        if(SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C) Done = TRUE;
        FPutC(file,in);
        if (in==0x08)
               {
               if (cnt>1) cnt--;
               FPutC(file,' ');
               FPutC(file,0x08);
               }
           else
                {
                in = in&0x7f;
                input[cnt]=in;
                cnt++;
                input[cnt]=0;
                }
        } while ((in!=0x0d)&&(cnt<79)&&(!Done));
    input[cnt-1]=0;
    SetMode(file,0);
    Close(file);
    if (Done) strcpy(input,"###PANIC");
    return(input);
    }
///
//* Wait1sec
void Wait1sec(void)
    {
    LONG loticks;

    do
       {
       loticks=EClock.ev_lo>>17;
       ReadEClock(&EClock);
       } while (EClock.ev_lo>>17==loticks);
    }
///

int main(ULONG argc,char *argv[]) {
       char Name[80];
       STRPTR line[3];
       char *Reason;
       int t;

       if (ShowAnim())
           {

       if (argc==1) strcpy(Name,"<< NO NAME >>"); else
           {
           strcpy(Name,argv[1]);
           t=2;
           while (t<argc)
               {
               strcat(Name," ");
               strcat(Name,argv[t]);
               t++;
               }
           }

       if(!(SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C))
          {
          Reason = InReason();
          if (strcmp(Reason,"###PANIC"))
             {
             line[0]=strcpy(malloc(80),Name);
             line[1]=strcpy(malloc(80),Reason);
             line[2]=NULL;

             GTLayoutBase = (struct GTLayoutBase*) OpenLibrary("gtlayout.library",12);

             MyMSG = CreateMsgPort();
             MyIO = (struct timerequest *)CreateIORequest(MyMSG,sizeof(struct timerequest));
             OpenDevice("timer.device",UNIT_MICROHZ,(struct IORequest *)MyIO,NULL);
             TimerBase = (struct Library *)&MyIO->tr_node.io_Device->dd_Library;

             if(Handle = LT_CreateHandleTags(NULL,LH_AutoActivate,FALSE,TAG_DONE))
             {
                struct Window *Window;

                LT_New(Handle,
                   LA_Type,       VERTICAL_KIND,
                TAG_DONE);

                LT_New(Handle,
                   LA_Type,      VERTICAL_KIND,  /* A vertical group. */
                   LA_LabelText, "Chat Reason",   /* Group title text. */
                TAG_DONE);
                {
                   LT_New(Handle,
                      LA_Type,      BOX_KIND, /* A plain button. */
                      LA_Chars,50,
                      LABX_AlignText,ALIGNTEXT_CENTERED,
                      LABX_Lines, &line,
                   TAG_DONE);

                   LT_New(Handle,
                      LA_Type,    END_KIND,
                   TAG_DONE);
                }

                LT_New(Handle,
                   LA_Type,         HORIZONTAL_KIND,
                   LAGR_SameSize,1,
                   LAGR_Spread,1,
                   LAGR_IndentX,1,
                TAG_DONE);
                {
                   LT_New(Handle,
                      LA_Type,      BUTTON_KIND,
                      LA_LabelText, "    OK Let's chat    ",
                      LA_ID,        1,
                   TAG_DONE);

                   LT_New(Handle,
                      LA_Type,      BUTTON_KIND,
                      LA_LabelText, "      Deny Chat      ",
                      LA_ID,        2,
                   TAG_DONE);

                   LT_New(Handle,
                       LA_Type,   END_KIND,
                   TAG_DONE);
                }

                LT_New(Handle,
                   LA_Type,       VERTICAL_KIND,
                TAG_DONE);
                {
                   LT_New(Handle,
                      LA_Type,      XBAR_KIND,
                   TAG_DONE);

                LT_New(Handle,
                      LA_Type,      END_KIND,
                TAG_DONE);


                LT_New(Handle,
                      LA_Type,      HORIZONTAL_KIND,
                      LAGR_SameSize,1,
                TAG_DONE);

                   LT_New(Handle,
                      LA_Type,      GAUGE_KIND,
                      LAGA_Percent,0,
                      LAGA_InfoText,"Remaining time to answer chat request",
                      LA_Chars,50,
                      LA_ID,        10,
                   TAG_DONE);

                   LT_New(Handle,
                      LA_Type,      END_KIND,    /* This ends the current group. */
                   TAG_DONE);
                }

                LT_New(Handle,
                      LA_Type,       END_KIND,
                TAG_DONE);

                if(Window = LT_Build(Handle,LAWN_Title,"ACP-Page 2.0 (C) by ACP-Development 1994-95",LAWN_IDCMP,IDCMP_CLOSEWINDOW,WA_CloseGadget,1,WA_DepthGadget,1,WA_DragBar,1,TAG_DONE))
                {   struct IntuiMessage *Message;
                    ULONG                MsgQualifier,
                                         MsgClass,
                                         t,i;
                    UWORD                MsgCode;
                    struct Gadget       *MsgGadget;
                    BOOL                 Done = FALSE;
                    BOOL                 CWin = FALSE;

                    LT_ShowWindow(Handle,1);

                    i=0;
                    printf("\n\n\n\n\n\nPaging ");

                    do
                    {
                        //WaitPort(Window -> UserPort);
                        Wait1sec();

                        if(SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
                           {
                           Done=TRUE;
                           }

                        printf("."); flushall();
                        t = i*2;

                        if (!CWin)
                        {
                            LT_SetAttributes(Handle,10,LAGA_Percent,t,TAG_DONE);

                            while(Message = GT_GetIMsg(Window -> UserPort))
                            {
                               MsgClass     = Message -> Class;
                               MsgCode      = Message -> Code;
                               MsgQualifier = Message -> Qualifier;
                               MsgGadget    = Message -> IAddress;

                               GT_ReplyIMsg(Message);
                               LT_HandleInput(Handle,MsgQualifier,&MsgClass,&MsgCode,&MsgGadget);

                               switch(MsgClass)
                               {
                                  case IDCMP_CLOSEWINDOW:

                                      Done = TRUE;
                                      break;

                                  case IDCMP_GADGETUP:

                                      switch(MsgGadget -> GadgetID)
                                      {
                                          case 1: Done = TRUE;
                                                  printf("\n\nThe SYSOP is going to enter the chat mode in a moment\n");
                                                  break;

                                          case 2: CWin = TRUE;
                                                  break;
                                      }
                                      break;
                               }
                            }
                        if (CWin) LT_DeleteHandle(Handle);
                        }
                    i++;
                    } while ((!Done)&&(i<50));
             printf("\n\n");
             if (!CWin) LT_DeleteHandle(Handle);
             }
         }
      DeleteMsgPort(MyMSG);
      CloseDevice((struct IORequest *)MyIO);
      DeleteIORequest((struct timerequest *)MyIO);
      }
   }
}
return(0);
}

