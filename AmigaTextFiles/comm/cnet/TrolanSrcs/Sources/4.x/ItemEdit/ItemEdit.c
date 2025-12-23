/*
** CNet/4 ItemEdit
**
** Last update: Sat Aug 01 13:35:04 1998
*/

#include "ItemEdit.h"

/* Global Variables */

struct Library  *BGUIBase   = NULL;
struct Library  *CNetBase   = NULL;
struct MainPort *myp        = NULL;
struct MsgPort  *IE_MsgPort = NULL;

/* Function Declarations */

struct Window *OpenIEMainWin(Object **IE_MainWO,
                             Object **IE_Gads);
BOOL           handle_IE_Main(struct Window *win,
                              Object        *WO,
                              Object        **IE_Gads);
void           init_GUI(LONG subboard);
ULONG          MakeRequest(struct Window *win,
                           char *title,
                           char *gads,
                           char *string, ...);

/* ReadArgs() Preparations */

#define CMD_TEMPLATE "SUBBOARD/N/A"

enum
{
   ARG_SUBBOARD = 0,
   NUM_ARGS
};

void main(void)
{
   struct RDArgs __aligned *rda;

   ULONG args [ NUM_ARGS ];

   if(rda = ReadArgs(CMD_TEMPLATE, (LONG *)args, NULL))
   {
      LONG subboard = *((LONG *)args[ARG_SUBBOARD]);

      FreeArgs(rda);

      if(CNetBase = OpenLibrary("cnet.library", 4L))
      {
         if(BGUIBase = OpenLibrary("bgui.library", 41L))
         {
            Forbid();
            myp = (struct MainPort *)FindPort(CNETPORT);
            Permit();

            if(myp)
            {
               init_GUI(subboard);

               CloseLibrary(BGUIBase);
            }
            else
            {
               MakeRequest(NULL, "Error!", "Ok", "CNet Control must be running!");
            }
         }
         CloseLibrary(CNetBase);
      }
   }
}

void init_GUI(LONG subboard)
{
   Object *IE_MainWO;
   Object *IE_Gads [ NUM_GADS ];

   struct Window *IE_Main;

   ULONG signals;

   BOOL done;

   if(IE_MsgPort = CreateMsgPort())
   {
      signals = 1 << IE_MsgPort->mp_SigBit;

      if(IE_Main = OpenIEMainWin(&IE_MainWO, IE_Gads))
      {
         do
         {
            Wait(signals);
            done = handle_IE_Main(IE_Main, IE_MainWO, IE_Gads);
         } while(!done);

         if(IE_MainWO)    DisposeObject(IE_MainWO);
      }                                             
      DeleteMsgPort(IE_MsgPort);
   }
}

struct Window *OpenIEMainWin(Object **IE_MainWO, Object **IE_Gads)
{
   struct Window *win = NULL;

   static struct NewMenu IE_Menu[] =
   {
      Title("Project"),
       Item("Save"     , "S", IE_SAVE),
       Item("About..." , "?", IE_ABOUT),
       ItemBar,
       Item("Quit"     , "Q", IE_QUIT),
      End
   };

   static ULONG sl2int  [] = { SLIDER_Level, STRINGA_LongVal, TAG_END };
   static ULONG int2sl  [] = { STRINGA_LongVal, SLIDER_Level, TAG_END };
   static ULONG cyc2page[] = { MX_Active, PAGE_Active, TAG_END };

   Object *pages;

   UBYTE *tab_labels[] =
   {
      "Stats",
      "Credits",
      "Flags",
      NULL,
   };

   UBYTE *file_integrety[] =
   {
      NULL, NULL, NULL, NULL, NULL
   };

   UBYTE *file_purge[] =
   {
      NULL, NULL, NULL, NULL, NULL, NULL
   };

   int i;

   for(i = 0; i < 4; i++)  /* Setup integrety cycle labels */
   {
      file_integrety[i] = myp->FirstTT->bm[1796+i];
   }

   for(i = 0; i < 5; i++)  /* Setup purge basis labels */
   {
      file_purge[i] = myp->FirstTT->bm[1837+i];
   }

   *IE_MainWO = WindowObject,
      WINDOW_Title       , "Item Editor",
      WINDOW_ScreenTitle , "Item Editor",
      WINDOW_AutoAspect  , TRUE,
      WINDOW_AutoKeyLabel, TRUE,
      WINDOW_MenuStrip   , IE_Menu,
      WINDOW_SharedPort  , IE_MsgPort,
      WINDOW_ScaleWidth  , 30,
      WINDOW_ScaleHeight , 50,
      WINDOW_MasterGroup,
         HGroupObject, NormalOffset, NormalSpacing,
            StartMember,
               VGroupObject,
                  StartMember,
                     IE_Gads[IE_LISTVIEW] = ListviewObject,
                        LAB_Label, "Subboard Items",
                        LAB_Place, PLACE_ABOVE,
                        GA_ID, IE_LISTVIEW,
                     EndObject,
                  EndMember,
                  StartMember,
                     HGroupObject,
                        StartMember,
                           IE_Gads[IE_DELETE] = PrefButton("Delete", IE_DELETE),
                        EndMember,
                        StartMember,
                           IE_Gads[IE_QUIT] = PrefButton("Quit", IE_QUIT),
                        EndMember,
                     EndObject, FixMinHeight,
                  EndMember,
               EndObject,
            EndMember,
            StartMember,
               VGroupObject, NormalSpacing,
                  StartMember,
                     IE_Gads[IE_TABS] = Tabs(NULL, tab_labels, 0, IE_TABS),
                  EndMember,
                  StartMember,
                     pages = PageObject,
                        /*
                        ** Stats Page
                        */
                        PageMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject,
                                    StartMember,
                                       IE_Gads[IE_TITLE] = TabString("Title:", "", 40, IE_TITLE),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_SIZE] = TabInteger("Size:", 0, 15, IE_SIZE),
                                    EndMember,
                                    StartMember,
                                       HGroupObject,
                                          StartMember,
                                             IE_Gads[IE_PARTITION] = HorizSlider("Partition:", PARTITION_MIN, PARTITION_MAX, 0, IE_PARTITION),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_PARTITION_INT] = TabInteger(NULL, 0, 3, IE_PARTITION_INT),
                                             FixMinWidth,
                                          EndMember,
                                       EndObject,
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_LAST_DL_DATE] = TabString("Last Download:", "", 32, IE_LAST_DL_DATE),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_PURGE_DATE] = TabString("Purge Date:", "", 32, IE_PURGE_DATE),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_DL_AFTER_DATE] = TabString("Download After:", "", 32, IE_DL_AFTER_DATE),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_DL_BEFORE_DATE] = TabString("Download Before:", "", 32, IE_DL_BEFORE_DATE),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_DLS] = TabInteger("Downloads:", 0, 10, IE_DLS),
                                    EndMember,
                                    StartMember,
                                       IE_Gads[IE_CPS] = TabInteger("Best CPS:", 0, 10, IE_CPS),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        /*
                        ** Credits Page
                        */
                        PageMember,
                           VGroupObject, NormalOffset,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject,
                                    StartMember,
                                       VGroupObject,
                                          StartMember,
                                             IE_Gads[IE_FILE_CHARGE] = TabInteger("File charge$ to ULer:", 0, 6, IE_FILE_CHARGE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_FILE_CREDITS] = TabInteger("File credits to ULer:", 0, 10, IE_FILE_CREDITS),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_BYTE_CHARGE] = TabInteger("Byte charge$ to ULer:", 0, 6, IE_BYTE_CHARGE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_BYTE_CREDITS] = TabInteger("Byte credits to ULer:", 0, 10, IE_BYTE_CREDITS),
                                          EndMember,
                                       EndObject, FixMinHeight,
                                    EndMember,
                                 EndObject,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, NormalOffset,
                                    StartMember,
                                       IE_Gads[IE_OVERRIDE_CHECK] = PrefCheckBox("These Values override:", 0, IE_OVERRIDE_CHECK),
                                    EndMember,
                                    StartMember,
                                       HGroupObject, NormalSpacing,
                                          StartMember,
                                             VGroupObject,
                                                StartMember,
                                                   IE_Gads[IE_FILE_PAYBACK] = TabInteger("File payback to ULer:", 0, 6, IE_FILE_PAYBACK),
                                                EndMember,
                                                StartMember,
                                                   IE_Gads[IE_FILE_COST] = TabInteger("File cost to DLer:", 0, 6, IE_FILE_COST),
                                                EndMember,
                                             EndObject,
                                          EndMember,
                                          StartMember,
                                             VGroupObject,
                                                StartMember,
                                                   IE_Gads[IE_BYTE_PAYBACK] = TabInteger("Byte payback to ULer:", 0, 10, IE_BYTE_PAYBACK),
                                                EndMember,
                                                StartMember,
                                                   IE_Gads[IE_BYTE_COST] = TabInteger("Byte cost to DLer:", 0, 10, IE_BYTE_COST),
                                                EndMember,
                                             EndObject,
                                          EndMember,
                                       EndObject,
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        /*
                        ** Flags Page
                        */
                        PageMember,
                           HGroupObject, NormalOffset,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject,
                                    VarSpace(DEFAULT_WEIGHT),
                                    StartMember,
                                       VGroupObject,
                                          StartMember,
                                             IE_Gads[IE_PURGE] = PopCycle("Purge Status:", file_purge, 0, IE_PURGE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_INTEGRETY] = PopCycle("Integrety:", file_integrety, 0, IE_INTEGRETY),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_DEL_PURGE] = PrefCheckBox("Delete when purged:", 0, IE_DEL_PURGE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_XFORM] = PrefCheckBox("Transform Attempted:", 0, IE_XFORM),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_VIRUS_CHECKED] = PrefCheckBox("Virus Checked:", 0, IE_VIRUS_CHECKED),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_FREE_DL] = PrefCheckBox("Free Download:", 0, IE_FREE_DL),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_NOTIFY_UL] = PrefCheckBox("Notify ULer @DL:", 0, IE_NOTIFY_UL),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_DISABLE_RESPONSES] = PrefCheckBox("Disable Responses:", 0, IE_DISABLE_RESPONSES),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_FAVORITE] = PrefCheckBox("SysOp Favorite:", 0, IE_FAVORITE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_OFFLINE] = PrefCheckBox("Offline:", 0, IE_OFFLINE),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_ASCIIGRAB] = PrefCheckBox("ASCII Grab:", 0, IE_ASCIIGRAB),
                                          EndMember,
                                          StartMember,
                                             IE_Gads[IE_VOTELINK] = TabInteger("Votelink Serial #:", 0, 10, IE_VOTELINK),
                                          EndMember,
                                       EndObject, FixMinHeight,
                                    EndMember,
                                    VarSpace(DEFAULT_WEIGHT),
                                 EndObject,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                     EndObject,
                  EndMember,
               EndObject, Weight(50),
            EndMember,
         EndObject,
   EndObject;
            
   if(*IE_MainWO)
   {
      AddMap(IE_Gads[IE_TABS], pages, cyc2page);

      AddMap(IE_Gads[IE_PARTITION], IE_Gads[IE_PARTITION_INT], sl2int);
      AddMap(IE_Gads[IE_PARTITION_INT], IE_Gads[IE_PARTITION], int2sl);

      DoMethod(*IE_MainWO, WM_TABCYCLE_ORDER,
         IE_Gads[IE_TITLE         ],
         IE_Gads[IE_SIZE          ],
         IE_Gads[IE_PARTITION_INT ],
         IE_Gads[IE_LAST_DL_DATE  ],
         IE_Gads[IE_PURGE_DATE    ],
         IE_Gads[IE_DL_AFTER_DATE ],
         IE_Gads[IE_DL_BEFORE_DATE],
         IE_Gads[IE_DLS           ],
         IE_Gads[IE_CPS           ],
         NULL);

      DoMethod(*IE_MainWO, WM_TABCYCLE_ORDER,
         IE_Gads[IE_FILE_CHARGE ],
         IE_Gads[IE_FILE_CREDITS],
         IE_Gads[IE_BYTE_CHARGE ],
         IE_Gads[IE_BYTE_CREDITS],
         IE_Gads[IE_FILE_PAYBACK],
         IE_Gads[IE_FILE_COST   ],
         IE_Gads[IE_BYTE_PAYBACK],
         IE_Gads[IE_BYTE_COST   ],
         NULL);

      win = WindowOpen(*IE_MainWO);
   }

   return (win);
}

BOOL handle_IE_Main(struct Window *win, Object *IE_MainWO, Object **IE_Gads)
{
   BOOL done = FALSE;

   ULONG rc;

   while((rc = HandleEvent(IE_MainWO)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case WMHI_CLOSEWINDOW:
         case IE_QUIT:
            done = TRUE;
            break;

         case IE_ABOUT:
            MakeRequest(win, "About", "Ok", "CNet ItemEdit");
            break;
      }
   }

   return (done);
}

ULONG MakeRequest(struct Window *win, char *title, char *gads, char *string, ...)
{
   struct bguiRequest      req;

   memset(&req, 0, sizeof(struct bguiRequest));

   req.br_GadgetFormat = gads;
   req.br_Title        = title;
   req.br_TextFormat   = string;
   req.br_Underscore   = '_';
   req.br_Flags        = BREQF_CENTERWINDOW | BREQF_AUTO_ASPECT | BREQF_FAST_KEYS;

   return(BGUI_RequestA(win, &req, (ULONG *)(&string + 1)));
}
