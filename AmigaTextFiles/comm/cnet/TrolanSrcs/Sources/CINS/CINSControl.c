/*
** CINS Control / Management Program
** by Kelly Cochran
** Last Updated: Tue Sep  9 22:24:11 1997
*/

#include "CINSControl.h"

struct Library      *GadToolsBase = NULL;
struct CINSMainPort cmp;

struct NewMenu CINS_Menu[] =
{
   {NM_TITLE, "Project"    ,  0 , 0, 0, 0},
   {NM_ITEM , "Reload Text", "R", 0, 0, 0},
   {NM_ITEM , "About..."   , "?", 0, 0, 0},
   {NM_ITEM , "Quit"       , "Q", 0, 0, 0},
   {NM_END  , NULL         ,  0 , 0, 0, 0}
};

int  main(int argc, char **argv);
BOOL Handle_IDCMP(struct Window *win);
BOOL load_text(void);
void free_text(void);
BOOL reload_text(void);
int  do_ez_req(UBYTE *Title, UBYTE *Body, UBYTE *Buttons);
void main_loop(void);

int main(int argc, char **argv)
{
   memset(&cmp, 0, sizeof(struct CINSMainPort));

   if(GadToolsBase = OpenLibrary("gadtools.library", 37))
   {
      if(cmp.win = OpenWindowTags(NULL,
                              WA_Title    , "CINS Control",
                              WA_Width    , 300,
                              WA_Height   , 50,
                              WA_MinWidth , 300,
                              WA_MinHeight, 50,
                              WA_MaxWidth , 300,
                              WA_MaxHeight, ~0,
                              WA_Flags    , WFLG_CLOSEGADGET  |
                                            WFLG_SIZEGADGET   |
                                            WFLG_DEPTHGADGET  |
                                            WFLG_NEWLOOKMENUS |
                                            WFLG_DRAGBAR      |
                                            WFLG_SIZEBRIGHT   |
                                            WFLG_ACTIVATE,
                              WA_IDCMP    , IDCMP_CLOSEWINDOW |
                                            IDCMP_MENUPICK    |
                                            IDCMP_NEWSIZE,
                              TAG_END))
      {
         if(cmp.menu = CreateMenus(CINS_Menu, NULL))
         {
            if(cmp.vi = GetVisualInfo(cmp.win->WScreen, NULL))
            {
               if(LayoutMenus(cmp.menu, cmp.vi, GTMN_NewLookMenus, TRUE))
               {
                  if(SetMenuStrip(cmp.win, cmp.menu))
                  {
                     main_loop();
                     ClearMenuStrip(cmp.win);
                  }
               }
               FreeVisualInfo(cmp.vi);
            }
            FreeMenus(cmp.menu);
         }
         CloseWindow(cmp.win);
      }
      CloseLibrary(GadToolsBase);
   }

   return 0;
}

void main_loop(void)
{
   LONG signals;
   BOOL done    = FALSE;

   if(load_text())
   {
      while(!done)
      {
         signals = Wait(1L << cmp.win->UserPort->mp_SigBit);
         if(signals & (1L << cmp.win->UserPort->mp_SigBit))
         {
            done = Handle_IDCMP(cmp.win);
         }
      }
      free_text();
   }
}

BOOL Handle_IDCMP(struct Window *win)
{
   struct IntuiMessage *imsg;

   BOOL  done = FALSE;
   ULONG class;
   ULONG code;
   USHORT menuNumber;
   USHORT menuNum;
   USHORT itemNum;

   imsg = (struct IntuiMessage *)GetMsg(win->UserPort);

   class = imsg->Class;
   code  = menuNumber = imsg->Code;

   menuNum = MENUNUM(menuNumber);
   itemNum = ITEMNUM(menuNumber);

   ReplyMsg((struct Message *)imsg);

   switch(class)
   {
      case IDCMP_NEWSIZE:
         break;
      case IDCMP_CLOSEWINDOW:
         done = TRUE;
         break;
      case IDCMP_MENUPICK:
         switch(menuNum)
         {
            case 0:
               switch(itemNum)
               {
                  case 0:
                     Printf("reload_text()\n");
                     break;
                  case 1:
                     do_ez_req("", "CINS Control ("__DATE__")", "");
                     break;
                  case 2:
                     done = TRUE;
                     break;
               }
         }
         break;
   }

   return (done);
}

int do_ez_req(UBYTE *Title, UBYTE *Body, UBYTE *Buttons)
{
   struct EasyStruct ez_req =
   {
      sizeof(struct EasyStruct),
      NULL,
      NULL,
      NULL,
      NULL
   };

   if(Title && *Title)
   {
      ez_req.es_Title = Title;
   }
   else
   {
      ez_req.es_Title = "CINS Control";
   }

   if(Body && *Body)
   {
      ez_req.es_TextFormat = Body;
   }

   if(Buttons && *Buttons)
   {
      ez_req.es_GadgetFormat = Buttons;
   }
   else
   {
      ez_req.es_GadgetFormat = "Okay";
   }

   return (EasyRequest(cmp.win, &ez_req, NULL, NULL));
}

BOOL load_text(void)
{
   BPTR fp;

   return TRUE;
}

void free_text(void)
{
}

BOOL reload_text(void)
{
}
