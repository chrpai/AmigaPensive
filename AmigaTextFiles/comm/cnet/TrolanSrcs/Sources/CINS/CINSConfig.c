/*
** CINS Config - GUI module and main
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:29:32 1999
*/

#include "CINS.h"
#include "CINSConfig.h"

const char *version = VERTAG;

Object *WO_CINSMainWin,
       *WO_CINSAccWin,
       *WO_CINSArchieWin,
       *WO_CINSDefaultUserWin,
       *WO_CINSFTPWin,
       *WO_CINSIRCWin,
       *WO_CINSTelnetWin;

LONG acc_lv_id           = -1;
LONG arc_lv_id           = -1;
LONG ftp_list_lv_id      = -1;
LONG ftp_server_lv_id    = -1;
LONG irc_lv_id           = -1;
LONG telnet_list_lv_id   = -1;
LONG telnet_server_lv_id = -1;

struct CINSCfg        Config;

/* Internal Messaging port */
struct MsgPort *CINSCfgMsgPort;

/* Gadget stuff */
Object *CINSGads[CINS_GADGET_COUNT];

/* CNet structs */
struct MainPort      *myp;    /* Pointer to CNet port--ALL info!      */
struct PortData      *z;
struct Library       *CNetBase  = NULL;
struct Library       *BGUIBase  = NULL;
struct Library       *CINSBase  = NULL;
struct Library       *CNetCBase = NULL;   /* only set if run from CNet */
struct CNetCContext  *context   = NULL;

void main(int argc, char **argv)
{
   struct CPort *cport = NULL;  /* temp port */

   Forbid();
   if(argc > 1)
      cport = (struct CPort *)FindPort(argv[1]);
   Permit();

   if(argc < 2 && !cport)
   {
      Forbid();
      myp = (struct MainPort *)FindPort("cnetport");
      Permit();

      if(!myp)
      {
         Printf("CNet/Control must be running!\n");
         exit(20);
      }

      if(CINSBase = OpenLibrary(CINSLIBNAME, CINSLIBVERSION))
      {
         if(load_config())
         {
            GUI_main();

            if(Config.AE)
               FreeVec(Config.AE);

            if(Config.ftp)
               free_data_list(&Config.ftp);
            if(Config.telnet)
               free_data_list(&Config.telnet);
            if(Config.archie)
               free_list(&Config.archie);
            if(Config.irc)
               free_list(&Config.irc);
         }

         CloseLibrary(CINSBase);
      }
      exit(0);
   }

   if(!(CNetCBase = OpenLibrary("cnetc.library", 4L)))
   {
      Printf("Unable to setup CNetC library!\n");
      exit(0);
   }

   if(!(context = CNC_InitContext(argv[1])))
   {
      CloseLibrary(CNetCBase);
      Printf("Unable to setup CNetC messaging!\n");
      exit(0);
   }

   if(!(CNetBase = OpenLibrary("cnet.library", 3)))
   {
      GetOut();
   }

   z   = context->z;
   myp = context->myp;

   CNC_SetDoing("CINS Config");

   if(load_config())
     VDE_main();

   GetOut();
}

void GUI_main(void)
{
   struct Window *CINSMainWin,
                 *CINSAccWin         = NULL,
                 *CINSArchieWin      = NULL,
                 *CINSDefaultUserWin = NULL,
                 *CINSFTPWin         = NULL,
                 *CINSIRCWin         = NULL,
                 *CINSTelnetWin      = NULL,
                 *sigwin;

   ULONG signal,
         rc;

   BOOL done = FALSE;

   if(BGUIBase = OpenLibrary(BGUINAME, BGUIVERSION))
   {
      if(CINSCfgMsgPort = CreateMsgPort())
      {
         signal = 1 << CINSCfgMsgPort->mp_SigBit;

         if(CINSMainWin = OpenCINSMainWindow())
         {
            do
            {
               Wait(signal);
               while(sigwin = GetSignalWindow(WO_CINSMainWin))
               {
                  if(sigwin == CINSMainWin)
                  {
                     while((rc = HandleEvent(WO_CINSMainWin)) != WMHI_NOMORE)
                     {
                        switch(rc)
                        {
                           case CINS_SAVE_BUTTON:
                              GUI_pre_save((struct Gadget **)CINSGads);
                              break;

                           case CINS_SAVEQUIT_BUTTON:
                              done = TRUE;
                              GUI_pre_save((struct Gadget **)CINSGads);
                              break;

                           case WMHI_CLOSEWINDOW:
                           case CINS_QUIT_BUTTON:
                              done = TRUE;
                              break;

                           case CINS_ABOUT:
                              {
                                 char buff [ 32 ] = "";

                                 MakeRequest(CINSMainWin,
                                    "About CINS Config",
                                    "Okay",
                                    "CINS Config version "VERS"\n"
                                     "Copyright 1996-9, Kelly Cochran\n\n"
                                     "Compiled on "__DATE__"\n"
                                    buff);
                              }
                              break;

                           case CINS_ACCOUNTING_BUTTON:
                              if(!CINSAccWin)
                              {
                                 CINSAccWin = OpenCINSAccWindow();
                              }
                              break;

                           case CINS_DEFAULTUSER_BUTTON:
                              if(!CINSDefaultUserWin)
                              {
                                 CINSDefaultUserWin = OpenCINSDefaultUserWindow();
                              }
                              break;

                           case CINS_EDITARCHIE_BUTTON:
                              if(!CINSArchieWin)
                              {
                                 CINSArchieWin = OpenCINSArchieWindow();
                              }
                              break;

                           case CINS_EDITFTP_BUTTON:
                              if(!CINSFTPWin)
                              {
                                 CINSFTPWin = OpenCINSFTPWindow();
                              }
                              break;

                           case CINS_EDITIRC_BUTTON:
                              if(!CINSIRCWin)
                              {
                                 CINSIRCWin = OpenCINSIRCWindow();
                              }
                              break;

                           case CINS_EDITTELNET_BUTTON:
                              if(!CINSTelnetWin)
                              {
                                 CINSTelnetWin = OpenCINSTelnetWindow();
                              }
                              break;
                        }
                     }
                  }
                  else if(sigwin == CINSAccWin)
                  {
                     CINSAccWin = HandleAccWindow(CINSAccWin);
                  }
                  else if(sigwin == CINSArchieWin)
                  {
                     CINSArchieWin = HandleArchieWindow(CINSArchieWin);
                  }
                  else if(sigwin == CINSDefaultUserWin)
                  {
                     CINSDefaultUserWin = HandleDefaultUserWindow(CINSDefaultUserWin);
                  }
                  else if(sigwin == CINSFTPWin)
                  {
                     CINSFTPWin = HandleFTPWindow(CINSFTPWin);
                  }
                  else if(sigwin == CINSIRCWin)
                  {
                     CINSIRCWin = HandleIRCWindow(CINSIRCWin);
                  }
                  else if(sigwin == CINSTelnetWin)
                  {
                     CINSTelnetWin = HandleTelnetWindow(CINSTelnetWin);
                  }
               }
            } while (!done);

            if(WO_CINSTelnetWin)       DisposeObject(WO_CINSTelnetWin);
            if(WO_CINSIRCWin)          DisposeObject(WO_CINSIRCWin);
            if(WO_CINSFTPWin)          DisposeObject(WO_CINSFTPWin);
            if(WO_CINSDefaultUserWin)  DisposeObject(WO_CINSDefaultUserWin);
            if(WO_CINSAccWin)          DisposeObject(WO_CINSAccWin);
            if(WO_CINSArchieWin)       DisposeObject(WO_CINSArchieWin);
            if(WO_CINSMainWin)         DisposeObject(WO_CINSMainWin);
         }
         DeleteMsgPort(CINSCfgMsgPort);
      }
      CloseLibrary(BGUIBase);
   }

   make_config_VDE();
}

struct Window *OpenCINSMainWindow(void)
{
   struct Window *CINSMainWin = NULL;

   ULONG sl2int[] = { SLIDER_Level, STRINGA_LongVal, TAG_END };
   ULONG int2sl[] = { STRINGA_LongVal, SLIDER_Level, TAG_END };

   static struct NewMenu CINSCfgMenu[] =
   {
      Title("Project"),
       Item("Save"     , "S", CINS_SAVE_BUTTON),
       Item("About..." , "?", CINS_ABOUT),
       ItemBar,
       Item("Quit"     , "Q", CINS_QUIT_BUTTON),
      Title("Edit"),
       Item("Accounting..."    , "A", CINS_ACCOUNTING_BUTTON),
       Item("Archie Servers...", "R", CINS_EDITARCHIE_BUTTON),
       Item("Default User..."  , "D", CINS_DEFAULTUSER_BUTTON),
       Item("FTP Servers..."   , "F", CINS_EDITFTP_BUTTON),
       Item("IRC Servers..."   , "I", CINS_EDITIRC_BUTTON),
       Item("Telnet Servers...", "T", CINS_EDITTELNET_BUTTON),
      End
   };

   WO_CINSMainWin = WindowObject,
      WINDOW_Title, "CINS Config",
      WINDOW_ScreenTitle, "CINS Config",
      WINDOW_AutoAspect,   TRUE,
      WINDOW_AutoKeyLabel, TRUE,
      WINDOW_MenuStrip, CINSCfgMenu,
      WINDOW_SharedPort, CINSCfgMsgPort,
      WINDOW_ScaleWidth, 25,
      WINDOW_MasterGroup,
         VGroupObject, NormalOffset,
            VarSpace(DEFAULT_WEIGHT),
            StartMember,
               HGroupObject,
                  StartMember,
                     CINSGads[CINS_ACCESS_SLIDER] = HorizSlider("Min. _Access:", CINS_ACCESS_SLIDER_MIN, CINS_ACCESS_SLIDER_MAX, Config.Access, CINS_ACCESS_SLIDER),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_ACCESS_INTEGER] = TabInteger(NULL, Config.Access, 3, CINS_ACCESS_INTEGER),
                     FixMinWidth,
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
            StartMember,
               CINSGads[CINS_FILEDIR_STRING] = TabString("_UserDir:", Config.UserDir, 60, CINS_FILEDIR_STRING),
               FixMinHeight,
            EndMember,
            StartMember,
               CINSGads[CINS_URL_STRING] = TabString("Default UR_L:", Config.URL, 60, CINS_URL_STRING),
               FixMinHeight,
            EndMember,
            StartMember,
               HGroupObject,
                  StartMember,
                     CINSGads[CINS_IRC_STRING] = TabString("_IRC:", Config.IRC, 60, CINS_IRC_STRING),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_EDITIRC_BUTTON] = PrefButton("Edit...", CINS_EDITIRC_BUTTON),
                     FixMinWidth,
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
            StartMember,
               HGroupObject,
                  StartMember,
                     CINSGads[CINS_ARCHIE_STRING] = TabString("Ar_chie:", Config.Archie, 60, CINS_ARCHIE_STRING),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_EDITARCHIE_BUTTON] = PrefButton("Edit...", CINS_EDITARCHIE_BUTTON),
                     FixMinWidth,
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
            StartMember,
               CINSGads[CINS_GOPHER_STRING] = TabString("_Gopher:", Config.Gopher, 60, CINS_GOPHER_STRING),
               FixMinHeight,
            EndMember,
            StartMember,
               HGroupObject, NormalSpacing,
                  StartMember,
                     VGroupObject,
                        StartMember,
                           CINSGads[CINS_LOG_INTEGER] = TabInteger("L_og:", Config.Log, 4, CINS_LOG_INTEGER),
                        EndMember,
                        StartMember,
                           CINSGads[CINS_FTP_INTEGER] = TabInteger("Ma_x FTPs:", Config.FTPin, 4, CINS_FTP_INTEGER),
                        EndMember,
                        StartMember,
                           CINSGads[CINS_DEFAULTUSER_BUTTON] = PrefButton("_User Config...", CINS_DEFAULTUSER_BUTTON),
                        EndMember,
                     EndObject, FixMinHeight,
                  EndMember,
                  StartMember,
                     VGroupObject,
                        StartMember,
                           CINSGads[CINS_MENU_INTEGER] = TabInteger("BBS_Menu:", Config.Menu, 5, CINS_MENU_INTEGER),
                        EndMember,
                        StartMember,
                           CINSGads[CINS_USERDB_CHECK] = PrefCheckBox("Add Users _to DB?:", Config.AddNewToDB, CINS_USERDB_CHECK),
                        EndMember,
                     EndObject, FixMinHeight,
                  EndMember,
               EndObject,
            EndMember,
            VarSpace(DEFAULT_WEIGHT),
            StartMember,
               HorizSeperator,
            EndMember,
            VarSpace(DEFAULT_WEIGHT),
            StartMember,
               HGroupObject,
                  StartMember,
                     CINSGads[CINS_EDITTELNET_BUTTON] = PrefButton("Edit Telnet Sites...", CINS_EDITTELNET_BUTTON),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_ACCOUNTING_BUTTON] = PrefButton("Accou_nting...", CINS_ACCOUNTING_BUTTON),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_EDITFTP_BUTTON] = PrefButton("Edit FTP Sites...", CINS_EDITFTP_BUTTON),
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
            StartMember,
               HGroupObject,
                  StartMember,
                     CINSGads[CINS_SAVE_BUTTON] = PrefButton("_Save", CINS_SAVE_BUTTON),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_SAVEQUIT_BUTTON] = PrefButton("Sa_ve/Quit", CINS_SAVEQUIT_BUTTON),
                  EndMember,
                  StartMember,
                     CINSGads[CINS_QUIT_BUTTON] = PrefButton("_Quit", CINS_QUIT_BUTTON),
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
         EndObject,
   EndObject;

   AddMap(CINSGads[CINS_ACCESS_SLIDER], CINSGads[CINS_ACCESS_INTEGER], sl2int);
   AddMap(CINSGads[CINS_ACCESS_INTEGER], CINSGads[CINS_ACCESS_SLIDER], int2sl);

   if(WO_CINSMainWin)
   {
      if(CINSMainWin = WindowOpen(WO_CINSMainWin))
      {
         DoMethod(WO_CINSMainWin, WM_TABCYCLE_ORDER,
            CINSGads[CINS_FILEDIR_STRING],
            CINSGads[CINS_URL_STRING    ],
            CINSGads[CINS_IRC_STRING    ],
            CINSGads[CINS_ARCHIE_STRING ],
            CINSGads[CINS_GOPHER_STRING ],
            CINSGads[CINS_LOG_INTEGER   ],
            CINSGads[CINS_FTP_INTEGER   ],
            CINSGads[CINS_MENU_INTEGER  ],
            NULL);
      }
   }

   return (CINSMainWin);
}

struct Window *OpenCINSAccWindow(void)
{
   struct Window *CINSAccWin = NULL;

   static UBYTE *aglist[] =
   {
       "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",
       "8",   "9",  "10",  "11",  "12",  "13",  "14",  "15",
      "16",  "17",  "18",  "19",  "20",  "21",  "22",  "23",
      "24",  "25",  "26",  "27",  "28",  "29",  "30",  "31",
      NULL
   };

   if(!WO_CINSAccWin)
   {
      WO_CINSAccWin = WindowObject,
         WINDOW_Title, "CINS Accounting",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_ScaleWidth, 33,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset, NormalSpacing,
               StartMember,
                  HGroupObject, NormalSpacing,
                     StartMember,
                        CINSGads[CINS_ACC_AG_LISTVIEW] = ListviewObject,
                           LAB_Label, "_Access Group",
                           LAB_Place, PLACE_ABOVE,
                           LISTV_EntryArray, aglist,
                           GA_ID, CINS_ACC_AG_LISTVIEW,
                        EndObject, FixMinWidth,
                     EndMember,
                     StartMember,
                        VGroupObject,
                           VarSpace(DEFAULT_WEIGHT),
                           StartMember,
                              CINSGads[CINS_ACC_FTPFREE_INTEGER] = TabInteger("_UserDir MaxBytes:", 0, 9, CINS_ACC_FTPFREE_INTEGER),
                              FixMinHeight,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_WEBFREE_INTEGER] = TabInteger("_WebDir MaxBytes:", 0, 9, CINS_ACC_WEBFREE_INTEGER),
                              FixMinHeight,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_RATE_INTEGER] = TabInteger("_Rate:", 0, 9, CINS_ACC_RATE_INTEGER),
                              FixMinHeight,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_PURGEDAYS_INTEGER] = TabInteger("_Purge Days:", 0, 9, CINS_ACC_PURGEDAYS_INTEGER),
                              FixMinHeight,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_UPLOADFTP_CHECK] = PrefCheckBox("U/L to U_serDir?:", 0, CINS_ACC_UPLOADFTP_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_UPLOADWEB_CHECK] = PrefCheckBox("U/_L to WebDir?:", 0, CINS_ACC_UPLOADWEB_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_ACC_CD_CHECK] = PrefCheckBox("CD _Enabled?:", 0, CINS_ACC_CD_CHECK),
                           EndMember,
                           VarSpace(DEFAULT_WEIGHT),
                        EndObject,
                     EndMember,
                  EndObject,
               EndMember,
               StartMember,
                  CINSGads[CINS_ACC_QUIT_BUTTON] = PrefButton("_Quit", CINS_ACC_QUIT_BUTTON), 
                  FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSAccWin && (CINSAccWin = WindowOpen(WO_CINSAccWin)))
   {
      DoMethod(WO_CINSAccWin, WM_TABCYCLE_ORDER,
         CINSGads[CINS_ACC_FTPFREE_INTEGER  ],
         CINSGads[CINS_ACC_WEBFREE_INTEGER  ],
         CINSGads[CINS_ACC_RATE_INTEGER     ],
         CINSGads[CINS_ACC_PURGEDAYS_INTEGER],
         NULL);

      if(acc_lv_id == -1)
      {
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_FTPFREE_INTEGER  ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_WEBFREE_INTEGER  ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_RATE_INTEGER     ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_PURGEDAYS_INTEGER], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADFTP_CHECK  ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADWEB_CHECK  ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
         SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_CD_CHECK         ], CINSAccWin, NULL, GA_Disabled, TRUE, TAG_END);
      }
   }

   return (CINSAccWin);
}

struct Window *HandleAccWindow(struct Window *win)
{
   ULONG rc;

   while((rc = HandleEvent(WO_CINSAccWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         ULONG tmp;

         case CINS_ACC_AG_LISTVIEW:
            if(acc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_FTPFREE_INTEGER  ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_WEBFREE_INTEGER  ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_RATE_INTEGER     ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_PURGEDAYS_INTEGER], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADFTP_CHECK  ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADWEB_CHECK  ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_CD_CHECK         ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CINSGads[CINS_ACC_AG_LISTVIEW], (ULONG *)&acc_lv_id);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_FTPFREE_INTEGER  ], win, NULL, STRINGA_LongVal, Config.AE[acc_lv_id].FTPFree  , TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_WEBFREE_INTEGER  ], win, NULL, STRINGA_LongVal, Config.AE[acc_lv_id].WebFree  , TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_RATE_INTEGER     ], win, NULL, STRINGA_LongVal, Config.AE[acc_lv_id].Rate     , TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_PURGEDAYS_INTEGER], win, NULL, STRINGA_LongVal, Config.AE[acc_lv_id].PurgeDays, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADFTP_CHECK  ], win, NULL, GA_Selected    , Config.AE[acc_lv_id].Flags & CINSAEF_UPLOADFTP, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_UPLOADWEB_CHECK  ], win, NULL, GA_Selected    , Config.AE[acc_lv_id].Flags & CINSAEF_UPLOADWEB, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ACC_CD_CHECK         ], win, NULL, GA_Selected    , Config.AE[acc_lv_id].Flags & CINSAEF_CD       , TAG_END);
            break;

         case CINS_ACC_FTPFREE_INTEGER:
            GetAttr(STRINGA_LongVal, CINSGads[CINS_ACC_FTPFREE_INTEGER], (ULONG *)&tmp);
            Config.AE[acc_lv_id].FTPFree = tmp;
            break;

         case CINS_ACC_WEBFREE_INTEGER:
            GetAttr(STRINGA_LongVal, CINSGads[CINS_ACC_WEBFREE_INTEGER], (ULONG *)&tmp);
            Config.AE[acc_lv_id].WebFree = tmp;
            break;

         case CINS_ACC_RATE_INTEGER:
            GetAttr(STRINGA_LongVal, CINSGads[CINS_ACC_RATE_INTEGER   ], (ULONG *)&tmp);
            Config.AE[acc_lv_id].Rate = tmp;
            break;

         case CINS_ACC_PURGEDAYS_INTEGER:
            GetAttr(STRINGA_LongVal, CINSGads[CINS_ACC_PURGEDAYS_INTEGER   ], (ULONG *)&tmp);
            Config.AE[acc_lv_id].PurgeDays = tmp;
            break;

         case CINS_ACC_UPLOADFTP_CHECK:
            Config.AE[acc_lv_id].Flags ^= CINSAEF_UPLOADFTP;
            break;

         case CINS_ACC_UPLOADWEB_CHECK:
            Config.AE[acc_lv_id].Flags ^= CINSAEF_UPLOADWEB;
            break;

         case CINS_ACC_CD_CHECK:
            Config.AE[acc_lv_id].Flags ^= CINSAEF_CD;
            break;

         case WMHI_CLOSEWINDOW:
         case CINS_ACC_QUIT_BUTTON:
            WindowClose(WO_CINSAccWin);
            win = NULL;
            break;
      }
   }

   return (win);
}

struct Window *OpenCINSArchieWindow(void)
{
   struct Window *CINSArchieWin = NULL;

   if(!WO_CINSArchieWin)
   {
      WO_CINSArchieWin = WindowObject,
         WINDOW_Title, "CINS Archie Editor",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_ScaleHeight, 25,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset,
               StartMember,
                  CINSGads[CINS_ARC_LISTVIEW] = ListviewObject,
                                 LAB_Label, "A_rchie Servers",
                                 LAB_Place, PLACE_ABOVE,
                                 GA_ID, CINS_ARC_LISTVIEW,
                  EndObject,
               EndMember,
               StartMember,
                  CINSGads[CINS_ARC_SERVER_STRING] = TabString("_Server:", "", 64, CINS_ARC_SERVER_STRING),
                  FixMinHeight,
               EndMember,
               StartMember,
                  HGroupObject,
                     StartMember,
                        CINSGads[CINS_ARC_ADD_BUTTON] = PrefButton("_Add", CINS_ARC_ADD_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_ARC_DELETE_BUTTON] = PrefButton("_Delete", CINS_ARC_DELETE_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_ARC_INSERT_BUTTON] = PrefButton("_Insert", CINS_ARC_INSERT_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_ARC_QUIT_BUTTON] = PrefButton("_Quit", CINS_ARC_QUIT_BUTTON),
                     EndMember,
                  EndObject, FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSArchieWin)
   {
      if(!Config.archie)
      {
         int entries = load_archie_config(&Config.archie);
         int x;
         struct ArchieEntry *work;

         for(x = 0; x < entries; x++)
         {
            work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, x);
            AddEntry(NULL, CINSGads[CINS_ARC_LISTVIEW], &work->arc_Site.Site, LVAP_TAIL);
         }
      }

      if(CINSArchieWin = WindowOpen(WO_CINSArchieWin))
      {
         if(arc_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_DELETE_BUTTON], CINSArchieWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_INSERT_BUTTON], CINSArchieWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], CINSArchieWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
      }
   }

   return (CINSArchieWin);
}

struct Window *HandleArchieWindow(struct Window *win)
{
   struct ArchieEntry *work;
   struct ArchieSite   as;

   UBYTE *name,
         *str;

   ULONG rc;

   while((rc = HandleEvent(WO_CINSArchieWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case CINS_ARC_LISTVIEW:
            if(arc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_DELETE_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_INSERT_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CINSGads[CINS_ARC_LISTVIEW], (ULONG *)&arc_lv_id);

            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_ARC_LISTVIEW]))
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, STRINGA_TextVal, name, TAG_END);
            }

            break;

         case CINS_ARC_SERVER_STRING:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_ARC_LISTVIEW]))
            {
               if(work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, arc_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_ARC_SERVER_STRING], (ULONG *)&str);
                  strcpy(work->arc_Site.Site, str);
                  ReplaceEntry(win, CINSGads[CINS_ARC_LISTVIEW], name, work->arc_Site.Site);
               }
            }
            break;                              

         case CINS_ARC_ADD_BUTTON:
            if(arc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_DELETE_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_INSERT_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&as, 0, sizeof(struct ArchieSite)); /* add blank to list */
            CINS_AddArchieName(Config.archie, &as);

            GetAttr(LISTV_NumEntries, CINSGads[CINS_ARC_LISTVIEW], (ULONG *)&arc_lv_id); /* n entries */

            if(work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, arc_lv_id))       /* new entry will be nth entry */
            {
               AddEntrySelect(win, CINSGads[CINS_ARC_LISTVIEW], &work->arc_Site.Site, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->arc_Site.Site, TAG_END);
            }

            break;

         case CINS_ARC_DELETE_BUTTON:
            work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, arc_lv_id);

            RemoveSelected(win, CINSGads[CINS_ARC_LISTVIEW]);

            if(work && ((work->arc_Node.ln_Succ)->ln_Succ == NULL))
            {
               CINS_RemoveNode(Config.archie, arc_lv_id);
               arc_lv_id--;
            }
            else
            {
               CINS_RemoveNode(Config.archie, arc_lv_id);
            }

            if(arc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_DELETE_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_INSERT_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            }
            else
            {
               if(work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, arc_lv_id))
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->arc_Site.Site, TAG_END);
               }
            }
            break;

         case CINS_ARC_INSERT_BUTTON:
            memset(&as, 0, sizeof(struct ArchieSite)); /* add blank to list */
            CINS_InsertArchieName(Config.archie, &as, arc_lv_id);

            work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, arc_lv_id);       /* new entry will be nth entry */

            InsertEntrySelect(win, CINSGads[CINS_ARC_LISTVIEW], &work->arc_Site.Site, arc_lv_id);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_ARC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->arc_Site.Site, TAG_END);

            break;

         case WMHI_CLOSEWINDOW:
         case CINS_ARC_QUIT_BUTTON:
            WindowClose(WO_CINSArchieWin);
            win = NULL;
            break;
      }
   }

   return (win);
}

struct Window *OpenCINSDefaultUserWindow(void)
{
   struct Window *win = NULL;

   static UBYTE *ftp_progress[] =
   {
      "None",
      "Dot-Notation",
      "Bar-Graph",
      "KB Received",
      NULL
   };

   static UBYTE *ftp_verbosity[] =
   {
      "Terse",
      "System Messages",
      "All Messages",
      NULL
   };

   if(!WO_CINSDefaultUserWin)
   {
      WO_CINSDefaultUserWin = WindowObject,
         WINDOW_Title, "CINS Default User Editor",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_SizeGadget, FALSE,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset, NormalSpacing,
               StartMember,
                  HGroupObject, NormalSpacing,
                     StartMember,
                        VGroupObject, DefaultFrame, FrameTitle("CINS Options"),
                           TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                           VarSpace(DEFAULT_WEIGHT),
                           StartMember,
                              CINSGads[CINS_DEFUSER_ANSIDIR_CHECK] = PrefCheckBox("_ANSI Dir/Web:", Config.def_DirANSI, CINS_DEFUSER_ANSIDIR_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_SHELL_CHECK] = PrefCheckBox("_Shell Mode:", Config.def_Shell, CINS_DEFUSER_SHELL_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_MUFFLE_CHECK] = PrefCheckBox("Auto-_Muffle:", Config.def_Muff, CINS_DEFUSER_MUFFLE_CHECK),
                           EndMember,
                           VarSpace(DEFAULT_WEIGHT),
                        EndObject,
                     EndMember,
                     StartMember,
                        VGroupObject, DefaultFrame, FrameTitle("ftp Options"),
                           TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                           VarSpace(DEFAULT_WEIGHT),
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPPROGRESS_CYCLE] = PopCycle("Progress _Meter:", ftp_progress, Config.def_ftp.Progress, CINS_DEFUSER_FTPPROGRESS_CYCLE),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPANSI_CHECK] = PrefCheckBox("ANSI _Escapes:", Config.def_ftp.ANSIEsc, CINS_DEFUSER_FTPANSI_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPAUTOBIN_CHECK] = PrefCheckBox("Auto-_Binary:", Config.def_ftp.AutoBin, CINS_DEFUSER_FTPAUTOBIN_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPVERBOSE_CYCLE] = PopCycle("_Verbosity:", ftp_verbosity, Config.def_ftp.Verbose, CINS_DEFUSER_FTPVERBOSE_CYCLE),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPAUTOANON_CHECK] = PrefCheckBox("Auto-Anon_ymous:", Config.def_ftp.AutoAnon, CINS_DEFUSER_FTPAUTOANON_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPRECENT_CHECK] = PrefCheckBox("_Recent:", Config.def_ftp.Recent, CINS_DEFUSER_FTPRECENT_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPTIPS_CHECK] = PrefCheckBox("_Tips:", Config.def_ftp.Tips, CINS_DEFUSER_FTPTIPS_CHECK),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_DEFUSER_FTPMGET_CHECK] = PrefCheckBox("_Prompt on mget:", Config.def_ftp.MGet, CINS_DEFUSER_FTPMGET_CHECK),
                           EndMember,
                           VarSpace(DEFAULT_WEIGHT),
                        EndObject,
                     EndMember,
                  EndObject, FixMinHeight,
               EndMember,
               StartMember,
                  CINSGads[CINS_DEFUSER_QUIT_BUTTON] = PrefButton("_Quit", CINS_DEFUSER_QUIT_BUTTON),
                  FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSDefaultUserWin)
   {
      win = WindowOpen(WO_CINSDefaultUserWin);
   }

   return (win);
}

struct Window *HandleDefaultUserWindow(struct Window *win)
{
   ULONG rc;
   ULONG temp;

   while((rc = HandleEvent(WO_CINSDefaultUserWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case CINS_DEFUSER_ANSIDIR_CHECK:
            Config.def_DirANSI ^= TRUE;
            break;

         case CINS_DEFUSER_SHELL_CHECK:
            Config.def_Shell ^= TRUE;
            break;

         case CINS_DEFUSER_MUFFLE_CHECK:
            Config.def_Muff ^= TRUE;
            break;

         case CINS_DEFUSER_FTPPROGRESS_CYCLE:
            GetAttr(CYC_Active, CINSGads[CINS_DEFUSER_FTPPROGRESS_CYCLE], (ULONG *)&temp);
            Config.def_ftp.Progress = temp;
            break;

         case CINS_DEFUSER_FTPANSI_CHECK:
            Config.def_ftp.ANSIEsc ^= TRUE;
            break;

         case CINS_DEFUSER_FTPAUTOBIN_CHECK:
            Config.def_ftp.AutoBin ^= TRUE;
            break;

         case CINS_DEFUSER_FTPVERBOSE_CYCLE:
            GetAttr(CYC_Active, CINSGads[CINS_DEFUSER_FTPVERBOSE_CYCLE], (ULONG *)&temp);
            Config.def_ftp.Verbose = temp;
            break;

         case CINS_DEFUSER_FTPAUTOANON_CHECK:
            Config.def_ftp.AutoAnon ^= TRUE;
            break;

         case CINS_DEFUSER_FTPRECENT_CHECK:
            Config.def_ftp.Recent ^= TRUE;
            break;

         case CINS_DEFUSER_FTPTIPS_CHECK:
            Config.def_ftp.Tips ^= TRUE;
            break;

         case CINS_DEFUSER_FTPMGET_CHECK:
            Config.def_ftp.MGet ^= TRUE;
            break;

         case WMHI_CLOSEWINDOW:
         case CINS_DEFUSER_QUIT_BUTTON:
            WindowClose(WO_CINSDefaultUserWin);
            win = NULL;
            break;
      }
   }
   return (win);
}

struct Window *OpenCINSFTPWindow(void)
{
   struct Window *CINSFTPWin = NULL;

   if(!WO_CINSFTPWin)
   {
      WO_CINSFTPWin = WindowObject,
         WINDOW_Title, "CINS FTP Editor",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_ScaleHeight, 25,
         WINDOW_ScaleWidth, 30,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset,
               StartMember,
                  HGroupObject,
                     StartMember,
                        CINSGads[CINS_FTP_LIST_LISTVIEW] = ListviewObject,
                           LAB_Label, "F_TP Categories",
                           LAB_Place, PLACE_ABOVE,
                           GA_ID, CINS_FTP_LIST_LISTVIEW,
                        EndObject,
                     EndMember,
                     StartMember,
                        CINSGads[CINS_FTP_SERVER_LISTVIEW] = ListviewObject,
                           LAB_Label, "FTP _Servers",
                           LAB_Place, PLACE_ABOVE,
                           GA_ID, CINS_FTP_SERVER_LISTVIEW,
                        EndObject,
                     EndMember,
                  EndObject,
               EndMember,
               StartMember,
                  HGroupObject,
                     StartMember,
                        VGroupObject,
                           StartMember,
                              HGroupObject,
                                 StartMember,
                                    CINSGads[CINS_FTP_ADDLIST_BUTTON] = PrefButton("_Add", CINS_FTP_ADDLIST_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_FTP_DELLIST_BUTTON] = PrefButton("De_lete", CINS_FTP_DELLIST_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_FTP_INSLIST_BUTTON] = PrefButton("_Insert", CINS_FTP_INSLIST_BUTTON),
                                 EndMember,
                              EndObject,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_FTP_LIST_STRING] = TabString("_Category:", "", 80, CINS_FTP_LIST_STRING),
                           EndMember,
                        EndObject, FixMinHeight,
                     EndMember,
                     StartMember,
                        VGroupObject,
                           StartMember,
                              HGroupObject,
                                 StartMember,
                                    CINSGads[CINS_FTP_ADDSERVER_BUTTON] = PrefButton("A_dd", CINS_FTP_ADDSERVER_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_FTP_DELSERVER_BUTTON] = PrefButton("D_elete", CINS_FTP_DELSERVER_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_FTP_INSSERVER_BUTTON] = PrefButton("Inse_rt", CINS_FTP_INSSERVER_BUTTON),
                                 EndMember,
                              EndObject,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_FTP_NAME_STRING] = TabString("_Name:", "", 80, CINS_FTP_NAME_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_FTP_HOST_STRING] = TabString("_Host/IP:", "", 80, CINS_FTP_HOST_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_FTP_COMM1_STRING] = TabString("Comment _1:", "", 80, CINS_FTP_COMM1_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_FTP_COMM2_STRING] = TabString("Comment _2:", "", 80, CINS_FTP_COMM2_STRING),
                           EndMember,
                        EndObject, FixMinHeight,
                     EndMember,
                  EndObject,
               EndMember,
               StartMember,
                  CINSGads[CINS_FTP_QUIT_BUTTON] = PrefButton("_Quit", CINS_FTP_QUIT_BUTTON),
                  FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSFTPWin)
   {
      if(!Config.ftp)
      {
         int entries = load_ftp_config(&Config.ftp);
         int x;
         struct FTPMenu *work;

         for(x = 0; x < entries; x++)
         {
            work = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, x);
            work->entries = load_ftp_data_config(&work->ftpm_List, x);
            AddEntry(NULL, CINSGads[CINS_FTP_LIST_LISTVIEW], &work->ftpm_Site.Menu, LVAP_TAIL);
         }
      }

      if(CINSFTPWin = WindowOpen(WO_CINSFTPWin))
      {
         DoMethod(WO_CINSFTPWin, WM_TABCYCLE_ORDER,
            CINSGads[CINS_FTP_LIST_STRING ],
            CINSGads[CINS_FTP_NAME_STRING ],
            CINSGads[CINS_FTP_HOST_STRING ],
            CINSGads[CINS_FTP_COMM1_STRING],
            CINSGads[CINS_FTP_COMM2_STRING],
            NULL);

         if(ftp_list_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELLIST_BUTTON], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSLIST_BUTTON], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING   ], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_ADDSERVER_BUTTON], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
         if(ftp_server_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], CINSFTPWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
      }
   }

   return (CINSFTPWin);
}

struct Window *HandleFTPWindow(struct Window *win)
{
   struct FTPEntry  *work;
   struct FTPSite    ftps;
   struct FTPMenu   *mwork;
   struct FTPMSite   ftpms;

   UBYTE *name,
         *str;

   ULONG rc;

   while((rc = HandleEvent(WO_CINSFTPWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case CINS_FTP_LIST_LISTVIEW:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_FTP_LIST_LISTVIEW]))
            {
               if(ftp_list_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_ADDLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_ADDSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               }

               GetAttr(LISTV_LastClickedNum, CINSGads[CINS_FTP_LIST_LISTVIEW], (ULONG *)&ftp_list_lv_id);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING], win, NULL, STRINGA_TextVal, name, TAG_END);

               if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
               {
                  int x;

                  ClearList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);
                  for(x = 0; x < mwork->entries; x++)
                  {
                     if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, x))
                     {
                        AddEntry(NULL, CINSGads[CINS_FTP_SERVER_LISTVIEW], &work->ftp_Site.Name, LVAP_TAIL);
                     }
                  }
                  RefreshList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

                  ftp_server_lv_id = -1;
               }
            }

            break;

         case CINS_FTP_LIST_STRING:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_FTP_LIST_LISTVIEW]))
            {
               if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_FTP_LIST_STRING], (ULONG *)&str);
                  strcpy(mwork->ftpm_Site.Menu, str);
                  ReplaceEntry(win, CINSGads[CINS_FTP_LIST_LISTVIEW], name, mwork->ftpm_Site.Menu);
               }
            }
            break;                              

         case CINS_FTP_ADDLIST_BUTTON:
            if(ftp_list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ftpms, 0, sizeof(struct FTPMSite)); /* add blank to list */
            CINS_AddFTPName(Config.ftp, &ftpms);

            GetAttr(LISTV_NumEntries, CINSGads[CINS_FTP_LIST_LISTVIEW], (ULONG *)&ftp_list_lv_id); /* n entries */

            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))       /* new entry will be nth entry */
            {
               mwork->ftpm_List = CINS_InitList();
               AddEntrySelect(win, CINSGads[CINS_FTP_LIST_LISTVIEW], &mwork->ftpm_Site.Menu, LVAP_TAIL);

               ClearList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING ], win, NULL, STRINGA_TextVal, mwork->ftpm_Site.Menu, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

               ftp_server_lv_id = -1;
            }
            break;

         case CINS_FTP_DELLIST_BUTTON:
            mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id);

            RemoveSelected(win, CINSGads[CINS_FTP_LIST_LISTVIEW]);

            if(mwork)
            {
               if(mwork->ftpm_List)
               {
                  free_list(&mwork->ftpm_List);
               }

               if((mwork->ftpm_Node.ln_Succ)->ln_Succ == NULL)
               {
                  CINS_RemoveNode(Config.ftp, ftp_list_lv_id);
                  ftp_list_lv_id--;
               }
               else
               {
                  CINS_RemoveNode(Config.ftp, ftp_list_lv_id);
               }
            }

            if(ftp_list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELLIST_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSLIST_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING], win, NULL, GA_Disabled, TRUE, TAG_END);
            }
            else if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               int x;

               ClearList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);
               for(x = 0; x < mwork->entries; x++)
               {
                  if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, x))
                  {
                     AddEntry(NULL, CINSGads[CINS_FTP_SERVER_LISTVIEW], &work->ftp_Site.Name, LVAP_TAIL);
                  }
               }
               RefreshList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);
            }

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

            ftp_server_lv_id = -1;

            break;

         case CINS_FTP_INSLIST_BUTTON:
            memset(&ftpms, 0, sizeof(struct FTPMSite)); /* add blank to list */
            CINS_InsertFTPName(Config.ftp, &ftpms, ftp_list_lv_id);

            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))       /* new entry will be nth entry */
            {
               mwork->ftpm_List = CINS_InitList();
               InsertEntrySelect(win, CINSGads[CINS_FTP_LIST_LISTVIEW], &mwork->ftpm_Site.Menu, ftp_list_lv_id);

               ClearList(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_LIST_STRING ], win, NULL, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

               ftp_server_lv_id = -1;
            }
            break;

         case CINS_FTP_SERVER_LISTVIEW:
            if(ftp_server_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CINSGads[CINS_FTP_SERVER_LISTVIEW], (ULONG *)&ftp_server_lv_id);

            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Name, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Host, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm1, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm2, TAG_END);
               }
            }

            break;

         case CINS_FTP_ADDSERVER_BUTTON:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(ftp_server_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               }

               memset(&ftps, 0, sizeof(struct FTPSite)); /* add blank to list */
               CINS_AddFTPDataName(mwork->ftpm_List, &ftps);
               mwork->entries++;

               GetAttr(LISTV_NumEntries, CINSGads[CINS_FTP_SERVER_LISTVIEW], (ULONG *)&ftp_server_lv_id); /* n entries */
               work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id);       /* new entry will be nth entry */

               AddEntrySelect(win, CINSGads[CINS_FTP_SERVER_LISTVIEW], &work->ftp_Site.Name, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Name, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Host, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm1, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm2, TAG_END);
            }
            break;

         case CINS_FTP_DELSERVER_BUTTON:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  RemoveSelected(win, CINSGads[CINS_FTP_SERVER_LISTVIEW]);

                  if(work && ((work->ftp_Node.ln_Succ)->ln_Succ == NULL))
                  {
                     CINS_RemoveNode(mwork->ftpm_List, ftp_server_lv_id);
                     ftp_server_lv_id--;
                  }
                  else
                  {
                     CINS_RemoveNode(mwork->ftpm_List, ftp_server_lv_id);
                  }
                  mwork->entries--;

                  if(ftp_server_lv_id == -1)
                  {
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  }
                  else
                  {
                     work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id);
                     
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Name, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Host, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm1, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm2, TAG_END);
                  }
               }
            }
            break;

         case CINS_FTP_INSSERVER_BUTTON:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  memset(&ftps, 0, sizeof(struct FTPSite)); /* add blank to list */
                  CINS_InsertFTPDataName(mwork->ftpm_List, &ftps, ftp_server_lv_id);
                  mwork->entries++;

                  work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id);       /* new entry will be nth entry */

                  InsertEntrySelect(win, CINSGads[CINS_FTP_SERVER_LISTVIEW], &work->ftp_Site.Name, ftp_server_lv_id);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_NAME_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Name, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_HOST_STRING ], win, NULL, STRINGA_TextVal, work->ftp_Site.Host, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM1_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm1, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_FTP_COMM2_STRING], win, NULL, STRINGA_TextVal, work->ftp_Site.Comm2, TAG_END);
               }
            }
            break;

         case CINS_FTP_NAME_STRING:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_FTP_NAME_STRING], (ULONG *)&str);
                  strcpy(work->ftp_Site.Name, str);

                  if(name = (UBYTE *)FirstSelected(CINSGads[CINS_FTP_SERVER_LISTVIEW]))
                  {
                     ReplaceEntry(win, CINSGads[CINS_FTP_SERVER_LISTVIEW], name, work->ftp_Site.Name);
                  }
               }
            }
            break;

         case CINS_FTP_HOST_STRING:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_FTP_HOST_STRING], (ULONG *)&str);
                  strcpy(work->ftp_Site.Host, str);
               }
            }
            break;

         case CINS_FTP_COMM1_STRING:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_FTP_COMM1_STRING], (ULONG *)&str);
                  strcpy(work->ftp_Site.Comm1, str);
               }
            }
            break;

         case CINS_FTP_COMM2_STRING:
            if(mwork = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, ftp_list_lv_id))
            {
               if(work = (struct FTPEntry *)CINS_FindOrdinal(mwork->ftpm_List, ftp_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_FTP_COMM2_STRING], (ULONG *)&str);
                  strcpy(work->ftp_Site.Comm2, str);
               }
            }
            break;

         case CINS_FTP_QUIT_BUTTON:
         case WMHI_CLOSEWINDOW:
            WindowClose(WO_CINSFTPWin);
            win = NULL;
            break;
      }
   }

   return (win);
}

struct Window *OpenCINSIRCWindow(void)
{
   struct Window *CINSIRCWin = NULL;

   if(!WO_CINSIRCWin)
   {
      WO_CINSIRCWin = WindowObject,
         WINDOW_Title, "CINS IRC Editor",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_ScaleHeight, 25,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset,
               StartMember,
                  CINSGads[CINS_IRC_LISTVIEW] = ListviewObject,
                                 LAB_Label, "I_RC Servers",
                                 LAB_Place, PLACE_ABOVE,
                                 GA_ID, CINS_IRC_LISTVIEW,
                  EndObject,
               EndMember,
               StartMember,
                  CINSGads[CINS_IRC_SERVER_STRING] = TabString("_Server:", "", 80, CINS_IRC_SERVER_STRING),
                  FixMinHeight,
               EndMember,
               StartMember,
                  CINSGads[CINS_IRC_HOST_STRING] = TabString("_Hostname:", "", 80, CINS_IRC_HOST_STRING),
                  FixMinHeight,
               EndMember,
               StartMember,
                  HGroupObject,
                     StartMember,
                        CINSGads[CINS_IRC_ADD_BUTTON] = PrefButton("_Add", CINS_IRC_ADD_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_IRC_DELETE_BUTTON] = PrefButton("_Delete", CINS_IRC_DELETE_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_IRC_INSERT_BUTTON] = PrefButton("_Insert", CINS_IRC_INSERT_BUTTON),
                     EndMember,
                     StartMember,
                        CINSGads[CINS_IRC_QUIT_BUTTON] = PrefButton("_Quit", CINS_IRC_QUIT_BUTTON),
                     EndMember,
                  EndObject, FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSIRCWin)
   {
      if(!Config.irc)
      {
         int entries = load_irc_config(&Config.irc);
         int x;
         struct IRCEntry *work;

         for(x = 0; x < entries; x++)
         {
            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, x);
            AddEntry(NULL, CINSGads[CINS_IRC_LISTVIEW], &work->irc_Site.Site, LVAP_TAIL);
         }
      }

      if(CINSIRCWin = WindowOpen(WO_CINSIRCWin))
      {
         DoMethod(WO_CINSIRCWin, WM_TABCYCLE_ORDER,
                  CINSGads[CINS_IRC_SERVER_STRING],
                  CINSGads[CINS_IRC_HOST_STRING  ],
                  NULL);

         if(irc_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_DELETE_BUTTON], CINSIRCWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_INSERT_BUTTON], CINSIRCWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], CINSIRCWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING  ], CINSIRCWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
      }
   }

   return (CINSIRCWin);
}

struct Window *HandleIRCWindow(struct Window *win)
{
   struct IRCEntry *work;
   struct IRCSite   ircs;

   UBYTE *name,
         *str;

   ULONG rc;

   while((rc = HandleEvent(WO_CINSIRCWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case CINS_IRC_LISTVIEW:
            if(irc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_DELETE_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_INSERT_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CINSGads[CINS_IRC_LISTVIEW], (ULONG *)&irc_lv_id);

            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Site, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING  ], win, NULL, STRINGA_TextVal, work->irc_Site.Host, TAG_END);

            break;

         case CINS_IRC_SERVER_STRING:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_IRC_LISTVIEW]))
            {
               work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);

               GetAttr(STRINGA_TextVal, CINSGads[CINS_IRC_SERVER_STRING], (ULONG *)&str);
               strcpy(work->irc_Site.Site, str);
               ReplaceEntry(win, CINSGads[CINS_IRC_LISTVIEW], name, work->irc_Site.Site);
            }
            break;                              

         case CINS_IRC_HOST_STRING:
            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);

            GetAttr(STRINGA_TextVal, CINSGads[CINS_IRC_HOST_STRING], (ULONG *)&str);
            strcpy(work->irc_Site.Host, str);

            break;                              

         case CINS_IRC_ADD_BUTTON:
            if(irc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_DELETE_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_INSERT_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ircs, 0, sizeof(struct IRCSite)); /* add blank to list */
            CINS_AddIRCName(Config.irc, &ircs);

            GetAttr(LISTV_NumEntries, CINSGads[CINS_IRC_LISTVIEW], (ULONG *)&irc_lv_id); /* n entries */
            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);       /* new entry will be nth entry */

            AddEntrySelect(win, CINSGads[CINS_IRC_LISTVIEW], &work->irc_Site.Site, LVAP_TAIL);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Site, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Host, TAG_END);

            break;

         case CINS_IRC_DELETE_BUTTON:
            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);

            RemoveSelected(win, CINSGads[CINS_IRC_LISTVIEW]);

            if(work && ((work->irc_Node.ln_Succ)->ln_Succ == NULL))
            {
               CINS_RemoveNode(Config.irc, irc_lv_id);
               irc_lv_id--;
            }
            else
            {
               CINS_RemoveNode(Config.irc, irc_lv_id);
            }

            if(irc_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_DELETE_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_INSERT_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            }
            else
            {
               if(work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id))
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Site, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Host, TAG_END);
               }
            }
            break;

         case CINS_IRC_INSERT_BUTTON:
            memset(&ircs, 0, sizeof(struct IRCSite)); /* add blank to list */
            CINS_InsertIRCName(Config.irc, &ircs, irc_lv_id);

            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, irc_lv_id);       /* new entry will be nth entry */

            InsertEntrySelect(win, CINSGads[CINS_IRC_LISTVIEW], &work->irc_Site.Site, irc_lv_id);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_SERVER_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Site, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_IRC_HOST_STRING], win, NULL, STRINGA_TextVal, work->irc_Site.Host, TAG_END);

            break;

         case CINS_IRC_QUIT_BUTTON:
         case WMHI_CLOSEWINDOW:
            WindowClose(WO_CINSIRCWin);
            win = NULL;
            break;
      }
   }

   return (win);
}

struct Window *OpenCINSTelnetWindow(void)
{
   struct Window *CINSTelnetWin = NULL;

   if(!WO_CINSTelnetWin)
   {
      WO_CINSTelnetWin = WindowObject,
         WINDOW_Title, "CINS Telnet Editor",
         WINDOW_AutoKeyLabel, TRUE,
         WINDOW_AutoAspect, TRUE,
         WINDOW_ScaleHeight, 25,
         WINDOW_ScaleWidth, 30,
         WINDOW_SharedPort, CINSCfgMsgPort,
         WINDOW_MasterGroup,
            VGroupObject, NormalOffset,
               StartMember,
                  HGroupObject,
                      StartMember,
                         CINSGads[CINS_TELNET_LIST_LISTVIEW] = ListviewObject,
                            LAB_Label, "_Telnet Categories",
                            LAB_Place, PLACE_ABOVE,
                            GA_ID, CINS_TELNET_LIST_LISTVIEW,
                         EndObject,
                      EndMember,
                      StartMember,
                         CINSGads[CINS_TELNET_SERVER_LISTVIEW] = ListviewObject,
                            LAB_Label, "Telnet _Servers",
                            LAB_Place, PLACE_ABOVE,
                            GA_ID, CINS_TELNET_SERVER_LISTVIEW,
                         EndObject,
                     EndMember,
                  EndObject,
               EndMember,
               StartMember,
                  HGroupObject,
                     StartMember,
                        VGroupObject,
                           StartMember,
                              HGroupObject,
                                 StartMember,
                                    CINSGads[CINS_TELNET_ADDLIST_BUTTON] = PrefButton("_Add", CINS_TELNET_ADDLIST_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_TELNET_DELLIST_BUTTON] = PrefButton("De_lete", CINS_TELNET_DELLIST_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_TELNET_INSLIST_BUTTON] = PrefButton("_Insert", CINS_TELNET_INSLIST_BUTTON),
                                 EndMember,
                              EndObject,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_TELNET_LIST_STRING] = TabString("_Category:", "", 80, CINS_TELNET_LIST_STRING),
                           EndMember,
                        EndObject, FixMinHeight,
                     EndMember,
                     StartMember,
                        VGroupObject,
                           StartMember,
                              HGroupObject,
                                 StartMember,
                                    CINSGads[CINS_TELNET_ADDSERVER_BUTTON] = PrefButton("A_dd", CINS_TELNET_ADDSERVER_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_TELNET_DELSERVER_BUTTON] = PrefButton("D_elete", CINS_TELNET_DELSERVER_BUTTON),
                                 EndMember,
                                 StartMember,
                                    CINSGads[CINS_TELNET_INSSERVER_BUTTON] = PrefButton("Inse_rt", CINS_TELNET_INSSERVER_BUTTON),
                                 EndMember,
                              EndObject,
                           EndMember,
                           StartMember,
                              CINSGads[CINS_TELNET_NAME_STRING] = TabString("_Name:", "", 80, CINS_TELNET_NAME_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_TELNET_HOST_STRING] = TabString("_Host/IP:", "", 80, CINS_TELNET_HOST_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_TELNET_COMM1_STRING] = TabString("Comment _1:", "", 80, CINS_TELNET_COMM1_STRING),
                           EndMember,
                           StartMember,
                              CINSGads[CINS_TELNET_COMM2_STRING] = TabString("Comment _2:", "", 80, CINS_TELNET_COMM2_STRING),
                           EndMember,
                        EndObject, FixMinHeight,
                     EndMember,
                  EndObject,
               EndMember,
               StartMember,
                  CINSGads[CINS_TELNET_QUIT_BUTTON] = PrefButton("_Quit", CINS_TELNET_QUIT_BUTTON),
                  FixMinHeight,
               EndMember,
            EndObject,
      EndObject;
   }

   if(WO_CINSTelnetWin)
   {
      if(!Config.telnet)
      {
         int entries = load_telnet_config(&Config.telnet);
         int x;
         struct TelnetMenu *work;

         for(x = 0; x < entries; x++)
         {
            work = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, x);
            work->entries = load_telnet_data_config(&work->tnm_List, x);
            AddEntry(NULL, CINSGads[CINS_TELNET_LIST_LISTVIEW], &work->tnm_Site.Menu, LVAP_TAIL);
         }
      }

      if(CINSTelnetWin = WindowOpen(WO_CINSTelnetWin))
      {
         DoMethod(WO_CINSTelnetWin, WM_TABCYCLE_ORDER,
            CINSGads[CINS_TELNET_LIST_STRING ],
            CINSGads[CINS_TELNET_NAME_STRING ],
            CINSGads[CINS_TELNET_HOST_STRING ],
            CINSGads[CINS_TELNET_COMM1_STRING],
            CINSGads[CINS_TELNET_COMM2_STRING],
            NULL);

         if(telnet_list_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELLIST_BUTTON], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSLIST_BUTTON], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING   ], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_ADDSERVER_BUTTON], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
         if(telnet_server_lv_id == -1)
         {
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], CINSTelnetWin, NULL, GA_Disabled, TRUE, TAG_END);
         }
      }
   }

   return (CINSTelnetWin);
}

struct Window *HandleTelnetWindow(struct Window *win)
{
   struct TelnetEntry  *work;
   struct TelnetSite    tns;
   struct TelnetMenu   *mwork;
   struct TelnetMSite   tnms;

   UBYTE *name,
         *str;

   ULONG rc;

   while((rc = HandleEvent(WO_CINSTelnetWin)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case CINS_TELNET_LIST_LISTVIEW:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_TELNET_LIST_LISTVIEW]))
            {
               if(telnet_list_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_ADDLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_ADDSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               }

               GetAttr(LISTV_LastClickedNum, CINSGads[CINS_TELNET_LIST_LISTVIEW], (ULONG *)&telnet_list_lv_id);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING], win, NULL, STRINGA_TextVal, name, TAG_END);

               if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
               {
                  int x;

                  ClearList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);
                  for(x = 0; x < mwork->entries; x++)
                  {
                     if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, x))
                     {
                        AddEntry(NULL, CINSGads[CINS_TELNET_SERVER_LISTVIEW], &work->tn_Site.Name, LVAP_TAIL);
                     }
                  }
                  RefreshList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

                  telnet_server_lv_id = -1;
               }
            }

            break;

         case CINS_TELNET_LIST_STRING:
            if(name = (UBYTE *)FirstSelected(CINSGads[CINS_TELNET_LIST_LISTVIEW]))
            {
               if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_TELNET_LIST_STRING], (ULONG *)&str);
                  strcpy(mwork->tnm_Site.Menu, str);
                  ReplaceEntry(win, CINSGads[CINS_TELNET_LIST_LISTVIEW], name, mwork->tnm_Site.Menu);
               }
            }
            break;                              

         case CINS_TELNET_ADDLIST_BUTTON:
            if(telnet_list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSLIST_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&tnms, 0, sizeof(struct TelnetMSite)); /* add blank to list */
            CINS_AddTelnetName(Config.telnet, &tnms);

            GetAttr(LISTV_NumEntries, CINSGads[CINS_TELNET_LIST_LISTVIEW], (ULONG *)&telnet_list_lv_id); /* n entries */

            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))       /* new entry will be nth entry */
            {
               mwork->tnm_List = CINS_InitList();
               AddEntrySelect(win, CINSGads[CINS_TELNET_LIST_LISTVIEW], &mwork->tnm_Site.Menu, LVAP_TAIL);

               ClearList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING ], win, NULL, STRINGA_TextVal, mwork->tnm_Site.Menu, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

               telnet_server_lv_id = -1;
            }
            break;

         case CINS_TELNET_DELLIST_BUTTON:
            mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id);

            RemoveSelected(win, CINSGads[CINS_TELNET_LIST_LISTVIEW]);

            if(mwork)
            {
               if(mwork->tnm_List)
               {
                  free_list(&mwork->tnm_List);
               }

               if((mwork->tnm_Node.ln_Succ)->ln_Succ == NULL)
               {
                  CINS_RemoveNode(Config.telnet, telnet_list_lv_id);
                  telnet_list_lv_id--;
               }
               else
               {
                  CINS_RemoveNode(Config.telnet, telnet_list_lv_id);
               }
            }

            if(telnet_list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELLIST_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSLIST_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING], win, NULL, GA_Disabled, TRUE, TAG_END);
            }
            else if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               int x;

               ClearList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);
               for(x = 0; x < mwork->entries; x++)
               {
                  if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, x))
                  {
                     AddEntry(NULL, CINSGads[CINS_TELNET_SERVER_LISTVIEW], &work->tn_Site.Name, LVAP_TAIL);
                  }
               }
               RefreshList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);
            }

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
            SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

            telnet_server_lv_id = -1;

            break;

         case CINS_TELNET_INSLIST_BUTTON:
            memset(&tnms, 0, sizeof(struct TelnetMSite)); /* add blank to list */
            CINS_InsertTelnetName(Config.telnet, &tnms, telnet_list_lv_id);

            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))       /* new entry will be nth entry */
            {
               mwork->tnm_List = CINS_InitList();
               InsertEntrySelect(win, CINSGads[CINS_TELNET_LIST_LISTVIEW], &mwork->tnm_Site.Menu, telnet_list_lv_id);

               ClearList(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_LIST_STRING ], win, NULL, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

               telnet_server_lv_id = -1;
            }
            break;

         case CINS_TELNET_SERVER_LISTVIEW:
            if(telnet_server_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CINSGads[CINS_TELNET_SERVER_LISTVIEW], (ULONG *)&telnet_server_lv_id);

            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Name, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Host, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm1, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm2, TAG_END);
               }
            }

            break;

         case CINS_TELNET_ADDSERVER_BUTTON:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(telnet_server_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, FALSE, TAG_END);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, FALSE, TAG_END);
               }

               memset(&tns, 0, sizeof(struct TelnetSite)); /* add blank to list */
               CINS_AddTelnetDataName(mwork->tnm_List, &tns);
               mwork->entries++;

               GetAttr(LISTV_NumEntries, CINSGads[CINS_TELNET_SERVER_LISTVIEW], (ULONG *)&telnet_server_lv_id); /* n entries */
               work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id);       /* new entry will be nth entry */

               AddEntrySelect(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW], &work->tn_Site.Name, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Name, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Host, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm1, TAG_END);
               SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm2, TAG_END);
            }
            break;

         case CINS_TELNET_DELSERVER_BUTTON:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  RemoveSelected(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW]);

                  if(work && ((work->tn_Node.ln_Succ)->ln_Succ == NULL))
                  {
                     CINS_RemoveNode(mwork->tnm_List, telnet_server_lv_id);
                     telnet_server_lv_id--;
                  }
                  else
                  {
                     CINS_RemoveNode(mwork->tnm_List, telnet_server_lv_id);
                  }
                  mwork->entries--;

                  if(telnet_server_lv_id == -1)
                  {
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_DELSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_INSSERVER_BUTTON], win, NULL, GA_Disabled, TRUE, TAG_END);

                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  }
                  else
                  {
                     work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id);
                     
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Name, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Host, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm1, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm2, TAG_END);
                  }
               }
            }
            break;

         case CINS_TELNET_INSSERVER_BUTTON:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  memset(&tns, 0, sizeof(struct TelnetSite)); /* add blank to list */
                  CINS_InsertTelnetDataName(mwork->tnm_List, &tns, telnet_server_lv_id);
                  mwork->entries++;

                  work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id);       /* new entry will be nth entry */

                  InsertEntrySelect(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW], &work->tn_Site.Name, telnet_server_lv_id);

                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_NAME_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Name, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_HOST_STRING ], win, NULL, STRINGA_TextVal, work->tn_Site.Host, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM1_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm1, TAG_END);
                  SetGadgetAttrs((struct Gadget *)CINSGads[CINS_TELNET_COMM2_STRING], win, NULL, STRINGA_TextVal, work->tn_Site.Comm2, TAG_END);
               }
            }
            break;

         case CINS_TELNET_NAME_STRING:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_TELNET_NAME_STRING], (ULONG *)&str);
                  strcpy(work->tn_Site.Name, str);

                  if(name = (UBYTE *)FirstSelected(CINSGads[CINS_TELNET_SERVER_LISTVIEW]))
                  {
                     ReplaceEntry(win, CINSGads[CINS_TELNET_SERVER_LISTVIEW], name, work->tn_Site.Name);
                  }
               }
            }
            break;

         case CINS_TELNET_HOST_STRING:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_TELNET_HOST_STRING], (ULONG *)&str);
                  strcpy(work->tn_Site.Host, str);
               }
            }
            break;

         case CINS_TELNET_COMM1_STRING:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_TELNET_COMM1_STRING], (ULONG *)&str);
                  strcpy(work->tn_Site.Comm1, str);
               }
            }
            break;

         case CINS_TELNET_COMM2_STRING:
            if(mwork = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, telnet_list_lv_id))
            {
               if(work = (struct TelnetEntry *)CINS_FindOrdinal(mwork->tnm_List, telnet_server_lv_id))
               {
                  GetAttr(STRINGA_TextVal, CINSGads[CINS_TELNET_COMM2_STRING], (ULONG *)&str);
                  strcpy(work->tn_Site.Comm2, str);
               }
            }
            break;

         case CINS_TELNET_QUIT_BUTTON:
         case WMHI_CLOSEWINDOW:
            WindowClose(WO_CINSTelnetWin);
            win = NULL;
            break;
      }
   }

   return (win);
}

BOOL load_config(void)
{
   BPTR  fp;

	memset(&Config, 0, sizeof(struct CINSCfg));

   if((fp = Open(CINSCFG, MODE_OLDFILE)) && (Read(fp, &Config, sizeof(struct CINSCfg))))
      Close(fp);

   Config.AE     = NULL; /* Explicitly clear these here */
   Config.archie = NULL;
   Config.ftp    = NULL;
   Config.irc    = NULL;
   Config.telnet = NULL;

   if(!(Config.AE = AllocVec(sizeof(struct CINSAccEntry) * 32, MEMF_CLEAR)))
   {
      GetOut();
   }

   if(fp = Open(CINSACC, MODE_OLDFILE))
   {
      Read(fp, Config.AE, sizeof(struct CINSAccEntry) * 32);
      Close(fp);
   }

   return TRUE;
}

int load_archie_config(struct List **list)
{
   struct ArchieSite as;

   int entries = 0;

   if(*list = CINS_InitList())
   {
      BPTR fp;

      if(fp = Open(CINSARCHIECFG, MODE_OLDFILE))
      {
         while(FGets(fp, as.Site, sizeof(as.Site)))
         {
            as.Site[strlen(as.Site)-1] = '\0';
            CINS_AddArchieName(*list, &as);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

void save_archie_config(struct List *list)
{
   BPTR fp;

   if(list)
   {
      struct ArchieEntry *work = (struct ArchieEntry *)list->lh_Head;

      if(!IsListEmpty(list) && (fp = Open(CINSARCHIECFG, MODE_NEWFILE)))
      {
         while(work && work->arc_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->arc_Site.Site);
            work = (struct ArchieEntry *)work->arc_Node.ln_Succ;
         }
         Close(fp);
      }
      if(IsListEmpty(list))
      {
         DeleteFile(CINSARCHIECFG);
      }
   }
}

int load_ftp_config(struct List **list)
{
   struct FTPMSite ftpms;

   int entries = 0;

   if(*list = CINS_InitList())
   {
      BPTR fp;

      if(fp = Open(CINSFTPCFG, MODE_OLDFILE))
      {
         while(FGets(fp, ftpms.Menu, sizeof(ftpms.Menu)))
         {
            ftpms.Menu[strlen(ftpms.Menu)-1] = '\0';
            CINS_AddFTPName(*list, &ftpms);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

int load_ftp_data_config(struct List **list, int i)
{
   struct FTPSite ftps;

   int entries = 0;

   char buffer [ 256 ] = "";

   if(*list = CINS_InitList())
   {
      BPTR fp;

      sprintf(buffer, CINSFTPCFGPAT, i + 1);

      if(fp = Open(buffer, MODE_OLDFILE))
      {
         while(FGets(fp, ftps.Name, sizeof(ftps.Name)))
         {
            ftps.Name [strlen(ftps.Name) -1] = '\0';
            FGets(fp, ftps.Host , sizeof(ftps.Host));
            ftps.Host [strlen(ftps.Host) -1] = '\0';
            FGets(fp, ftps.Comm1, sizeof(ftps.Comm1));
            ftps.Comm1[strlen(ftps.Comm1)-1] = '\0';
            FGets(fp, ftps.Comm2, sizeof(ftps.Comm2));
            ftps.Comm2[strlen(ftps.Comm2)-1] = '\0';

            CINS_AddFTPDataName(*list, &ftps);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

void save_ftp_config(struct List *list)
{
   BPTR fp;

   int i = 1;

   if(list)
   {
      struct FTPMenu *work = (struct FTPMenu *)list->lh_Head;

      if(!IsListEmpty(list) && (fp = Open(CINSFTPCFG, MODE_NEWFILE)))
      {
         while(work && work->ftpm_Node.ln_Succ)
         {
            save_ftp_data_config(work->ftpm_List, i);

            FPrintf(fp, "%s\n", work->ftpm_Site.Menu);
            work = (struct FTPMenu *)work->ftpm_Node.ln_Succ;
            i++;
         }
         Close(fp);
      }
      if(IsListEmpty(list))
      {
         DeleteFile(CINSFTPCFG);
      }
   }
}

void save_ftp_data_config(struct List *list, int i)
{
   BPTR fp;

   char buffer [ 256 ] = "";

   sprintf(buffer, CINSFTPCFGPAT, i);

   if(list && !IsListEmpty(list) && (fp = Open(buffer, MODE_NEWFILE)))
   {
      struct FTPEntry *work = (struct FTPEntry *)list->lh_Head;

      while(work && work->ftp_Node.ln_Succ)
      {
         FPrintf(fp, "%s\n", work->ftp_Site.Name);
         FPrintf(fp, "%s\n", work->ftp_Site.Host);
         FPrintf(fp, "%s\n", work->ftp_Site.Comm1);
         FPrintf(fp, "%s\n", work->ftp_Site.Comm2);
         work = (struct FTPEntry *)work->ftp_Node.ln_Succ;
      }
      Close(fp);
   }
   else
   {
      DeleteFile(buffer);
   }
}

int load_irc_config(struct List **list)
{
   struct IRCSite ircs;

   int entries = 0;

   if(*list = CINS_InitList())
   {
      BPTR fp;

      if(fp = Open(CINSIRCCFG, MODE_OLDFILE))
      {
         while(FGets(fp, ircs.Site, sizeof(ircs.Site)))
         {
            FGets(fp, ircs.Host, sizeof(ircs.Host));
            ircs.Site[strlen(ircs.Site)-1] = '\0';
            ircs.Host[strlen(ircs.Host)-1] = '\0';
            CINS_AddIRCName(*list, &ircs);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

void save_irc_config(struct List *list)
{
   BPTR fp;

   if(list)
   {
      struct IRCEntry *work = (struct IRCEntry *)list->lh_Head;

      if(!IsListEmpty(list) && (fp = Open(CINSIRCCFG, MODE_NEWFILE)))
      {
         while(work && work->irc_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->irc_Site.Site);
            FPrintf(fp, "%s\n", work->irc_Site.Host);
            work = (struct IRCEntry *)work->irc_Node.ln_Succ;
         }
         Close(fp);
      }
      if(IsListEmpty(list))
      {
         DeleteFile(CINSIRCCFG);
      }
   }
}

int load_telnet_config(struct List **list)
{
   struct TelnetMSite tnms;

   int entries = 0;

   if(*list = CINS_InitList())
   {
      BPTR fp;

      if(fp = Open(CINSTELNETCFG, MODE_OLDFILE))
      {
         while(FGets(fp, tnms.Menu, sizeof(tnms.Menu)))
         {
            tnms.Menu[strlen(tnms.Menu)-1] = '\0';
            CINS_AddTelnetName(*list, &tnms);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

int load_telnet_data_config(struct List **list, int i)
{
   struct TelnetSite tns;

   int entries = 0;

   char buffer [ 256 ] = "";

   if(*list = CINS_InitList())
   {
      BPTR fp;

      sprintf(buffer, CINSTELNETCFGPAT, i + 1);

      if(fp = Open(buffer, MODE_OLDFILE))
      {
         while(FGets(fp, tns.Name, sizeof(tns.Name)))
         {
            tns.Name [strlen(tns.Name) -1] = '\0';
            FGets(fp, tns.Host , sizeof(tns.Host));
            tns.Host [strlen(tns.Host) -1] = '\0';
            FGets(fp, tns.Comm1, sizeof(tns.Comm1));
            tns.Comm1[strlen(tns.Comm1)-1] = '\0';
            FGets(fp, tns.Comm2, sizeof(tns.Comm2));
            tns.Comm2[strlen(tns.Comm2)-1] = '\0';

            CINS_AddTelnetDataName(*list, &tns);
            entries++;
         }
         Close(fp);
      }
   }

   return (entries);
}

void save_telnet_config(struct List *list)
{
   BPTR fp;

   int i = 1;

   if(list)
   {
      struct TelnetMenu *work = (struct TelnetMenu *)list->lh_Head;

      if(!IsListEmpty(list) && (fp = Open(CINSTELNETCFG, MODE_NEWFILE)))
      {
         while(work && work->tnm_Node.ln_Succ)
         {
            save_telnet_data_config(work->tnm_List, i);

            FPrintf(fp, "%s\n", work->tnm_Site.Menu);
            work = (struct TelnetMenu *)work->tnm_Node.ln_Succ;
            i++;
         }
         Close(fp);
      }
      if(IsListEmpty(list))
      {
         DeleteFile(CINSTELNETCFG);
      }
   }
}

void save_telnet_data_config(struct List *list, int i)
{
   BPTR fp;

   char buffer [ 256 ] = "";

   sprintf(buffer, CINSTELNETCFGPAT, i);

   if(list && !IsListEmpty(list) && (fp = Open(buffer, MODE_NEWFILE)))
   {
      struct TelnetEntry *work = (struct TelnetEntry *)list->lh_Head;

      while(work && work->tn_Node.ln_Succ)
      {
         FPrintf(fp, "%s\n", work->tn_Site.Name);
         FPrintf(fp, "%s\n", work->tn_Site.Host);
         FPrintf(fp, "%s\n", work->tn_Site.Comm1);
         FPrintf(fp, "%s\n", work->tn_Site.Comm2);
         work = (struct TelnetEntry *)work->tn_Node.ln_Succ;
      }
      Close(fp);
   }
   else
   {
      DeleteFile(buffer);
   }
}

void GUI_pre_save(struct Gadget **CINSGads)
{
   STRPTR temp;
   LONG   tint;

   GetAttr(STRINGA_LongVal, CINSGads[   CINS_FTP_INTEGER], (ULONG *)&tint);
   Config.FTPin  = tint;
   GetAttr(STRINGA_LongVal, CINSGads[  CINS_MENU_INTEGER], (ULONG *)&tint);
   Config.Menu   = tint;
   GetAttr(STRINGA_LongVal, CINSGads[   CINS_LOG_INTEGER], (ULONG *)&tint);
   Config.Log    = tint;
   GetAttr(STRINGA_LongVal, CINSGads[CINS_ACCESS_INTEGER], (ULONG *)&tint);
   Config.Access = tint;

   GetAttr(GA_Selected, CINSGads[CINS_USERDB_CHECK], (ULONG *)&tint);
   Config.AddNewToDB = tint;

   GetAttr(STRINGA_TextVal, CINSGads[CINS_FILEDIR_STRING], (ULONG *)&temp);
   strcpy(Config.UserDir, temp);
   GetAttr(STRINGA_TextVal, CINSGads[    CINS_URL_STRING], (ULONG *)&temp);
   strcpy(Config.URL    , temp);
   GetAttr(STRINGA_TextVal, CINSGads[    CINS_IRC_STRING], (ULONG *)&temp);
   strcpy(Config.IRC    , temp);
   GetAttr(STRINGA_TextVal, CINSGads[ CINS_ARCHIE_STRING], (ULONG *)&temp);
   strcpy(Config.Archie , temp);
   GetAttr(STRINGA_TextVal, CINSGads[ CINS_GOPHER_STRING], (ULONG *)&temp);
   strcpy(Config.Gopher , temp);

   save_config();

   if(Config.archie)
      save_archie_config(Config.archie);
   if(Config.ftp)
      save_ftp_config(Config.ftp);
   if(Config.irc)
      save_irc_config(Config.irc);
   if(Config.telnet)
      save_telnet_config(Config.telnet);
}


void save_config(void)
{
   BPTR fp;

   char *p;

   char *errstr = "Error writing "CINSCFG"!\n";

   int   i;

   if(!(p = strchr(Config.UserDir,'%')) && (*(p+1) != 's'))
   {
      i = strlen(Config.UserDir);
      if((Config.UserDir[i-1] != '/') && (Config.UserDir[i-1] != ':'))
         strcat(Config.UserDir,"/");
      strcat(Config.UserDir,"%s");
   }
   i = strlen(Config.UserDir);
   if((Config.UserDir[i-1] == '/') || (Config.UserDir[i-1] == ':'))
      Config.UserDir[i-1] = 0;

   if(fp = Open(CINSCFG, MODE_NEWFILE))
   {
      if(!Write(fp, &Config, sizeof(struct CINSCfg)))
      {
         if(CNetCBase)
            CNC_PutText(errstr);
         else
            Printf(errstr);
      }
      Close(fp);
   }

   if(fp = Open(CINSACC, MODE_NEWFILE))
   {
      FWrite(fp, Config.AE, sizeof(struct CINSAccEntry), 32);
      Close(fp);
   }
}

void GetOut( void )
{
   if(CNetBase)
   {
      CloseLibrary(CNetBase);
   }
   if(Config.AE)
   {
      FreeVec(Config.AE);
   }

   if(CNetCBase)
   {
      CNC_ShutDown(NULL);
      CNC_DisposeContext();
      CloseLibrary(CNetCBase);
   }
   exit(0);
}

void free_data_list(struct List **list)
{
   struct Node *work,
               *temp;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         if(((struct FTPMenu *)work)->ftpm_List)
         {
            free_list(&((struct FTPMenu *)work)->ftpm_List);
         }
         temp = work->ln_Succ;
         FreeVec(work);
         work = temp;
      }
      FreeVec(*list);

      *list = NULL;
   }
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
