#include <intuition/gadgetclass.h>
#include "CINS/CINS.h"

/* listview test */

void main(void);
struct List *init_list(void);
void free_list(struct List **list);
ULONG handle_IDCMP(struct Window *win,
                   struct Gadget *lvgads[],
                   ULONG lvitem);
ULONG handle_gadget(struct Window *win,
                   struct IntuiMessage imsg,
                   struct Gadget *lvgads[],
                   ULONG lvitem);
ULONG handle_vanilla(struct Window *win,
                     struct IntuiMessage imsg,
                     struct Gadget *lvgads[],
                     ULONG lvitem);
struct Gadget *init_gadgets(struct Gadget **glist,
                            struct Gadget *lvgads[],
                            ULONG Top,
                            APTR vi);
void AddName(struct List *list,
             struct IRCSite *ircs);
void InsertName(struct List *list,
                struct Node *node,
                struct IRCSite *ircs);
#ifndef CNET4
struct Node *FindOrdinal(struct List *list,
                         int node);
#endif
BOOL save_irc_data(struct List *list,
               char *file);

struct IntuitionBase *IntuitionBase;
struct Library       *GadToolsBase;
struct GfxBase       *GfxBase;
struct Library       *CNetBase = NULL;

struct List  *ircl = NULL;
struct TextAttr Topaz80 = {"topaz.font", 8, 0, 0};

struct IRCEntry *lv_alab = NULL;
LONG num_entries = 0;

struct WindowData
{
   struct Window *win;
   struct Gadget *glist;
   struct Menu   *mstrip;
};

void main(void)
{
   struct Screen *scr;
   struct Window *win;
   struct Gadget *lvgads[ 8 ],
                 *glist;

   APTR vi;
   ULONG signal;
   ULONG  done = -1;
   

   if(IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37))
   {
      if(GadToolsBase = OpenLibrary("gadtools.library", 37))
      {
         if(GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 37))
         {
            if(CNetBase = OpenLibrary("cnet.library", 4))
            {
               if(scr = LockPubScreen(NULL))
               {
                  if(vi = GetVisualInfo(scr, TAG_END))
                  {
                     if(glist = init_gadgets(&glist, lvgads, scr->Font->ta_YSize, vi))
                     {
                        if(win = OpenWindowTags(NULL,
                                       WA_Title,  "Test",
                                       WA_Width,  400,
                                       WA_Height, 150,
                                       WA_MinWidth, 30,
                                       WA_MinHeight, 50,
                                       WA_MaxHeight, ~0,
                                       WA_MaxWidth, ~0,
                                       WA_Gadgets, glist,
                                       WA_Flags, WFLG_DRAGBAR |
                                                 WFLG_CLOSEGADGET |
                                                 WFLG_SIZEGADGET |
                                                 WFLG_DEPTHGADGET |
                                                 WFLG_ACTIVATE,
                                       WA_IDCMP, IDCMP_CLOSEWINDOW |
                                                 LISTVIEWIDCMP |
                                                 IDCMP_REFRESHWINDOW |
                                                 IDCMP_VANILLAKEY |
                                                 IDCMP_RAWKEY,
                                       TAG_END))
                        {
                           GT_RefreshWindow(win, NULL);
                           while(done != -2)
                           {
                              if(IsListEmpty(ircl))
                              {
                                 GT_SetGadgetAttrs(lvgads[0], win, NULL,
                                                   GA_Disabled, TRUE,
                                                   TAG_END);
                              }
                              signal = Wait(1L << win->UserPort->mp_SigBit);
                              done = handle_IDCMP(win, lvgads, done);
                              Printf("done = %ld\n", done);
                           }
                        }
                        CloseWindow(win);
                     }
                     FreeGadgets(glist);
                     FreeVisualInfo(vi);
                  }
                  UnlockPubScreen(NULL, scr);
               }
               CloseLibrary(CNetBase);
            }
            CloseLibrary((struct Library *)GfxBase);
         }
         CloseLibrary(GadToolsBase);
      }
      CloseLibrary((struct Library *)IntuitionBase);
   }
   if(ircl)
      free_list(&ircl);
   return;
}

ULONG handle_IDCMP(struct Window *win, struct Gadget *lvgads[], ULONG lvitem)
{
   struct IntuiMessage *msg,
                        imsg;

   while((lvitem != -2) && (msg = GT_GetIMsg(win->UserPort)))
   {
      CopyMem(msg, &imsg, sizeof(struct IntuiMessage));

      GT_ReplyIMsg(msg);

      switch(imsg.Class)
      {
         case IDCMP_CLOSEWINDOW:
            lvitem = -2;
            break;
         case IDCMP_GADGETUP:
            lvitem = handle_gadget(win, imsg, lvgads, lvitem);
            break;
         case IDCMP_RAWKEY:
         case IDCMP_VANILLAKEY:
            lvitem = handle_vanilla(win, imsg, lvgads, lvitem);
            break;
         case IDCMP_REFRESHWINDOW:
            GT_BeginRefresh(win);
            GT_EndRefresh(win, TRUE);
            break;
      }
   }
   return (lvitem);
}

ULONG handle_gadget(struct Window *win,
                    struct IntuiMessage imsg,
                    struct Gadget *lvgads[],
                    ULONG lvitem)
{
   struct IRCEntry *irce;
   struct IRCSite   ircs;
   struct Gadget   *gad;
   char *tmp;

   ULONG done = lvitem;

   gad = (struct Gadget *)imsg.IAddress;

   switch(gad->GadgetID)
   {
      case 1:  /* listview */
         lv_alab = irce = (struct IRCEntry *)FindOrdinal(ircl, imsg.Code);
         GT_SetGadgetAttrs(lvgads[1], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GTST_String, irce->irc_Site.Desc,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[6], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[7], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         done = imsg.Code;
         break;
      case 2:  /* name */
         tmp = ((struct StringInfo *)lvgads[1]->SpecialInfo)->Buffer;
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         strcpy(lv_alab->irc_Site.Site, tmp);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ircl,
                           TAG_END);
         break;
      case 3:  /* desc */
         tmp = ((struct StringInfo *)lvgads[1]->SpecialInfo)->Buffer;
         strcpy(lv_alab->irc_Site.Desc, ((struct StringInfo *)gad->SpecialInfo)->Buffer);
         break;
      case 4: /* save */
         SetWindowTitles(win, "Saving...", NULL);
         save_data(ircl, "CINS:Cfg/IRC");
         SetWindowTitles(win, "Test", NULL);
         break;
      case 5:  /* quit */
         done = -2;
         break;
      case 6:  /* add */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&ircs, 0, sizeof(struct IRCSite));
         AddName(ircl, &ircs);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, ircl,
                           GTLV_Selected, num_entries - 1,
                           GTLV_MakeVisible, num_entries - 1,
                           TAG_END);
         lv_alab = (struct IRCEntry *)FindOrdinal(ircl, num_entries - 1);
         GT_SetGadgetAttrs(lvgads[1], win, NULL,
                           GTST_String, lv_alab->irc_Site.Site,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GTST_String, lv_alab->irc_Site.Desc,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[6], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[7], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         done++;
         break;
      case 7:  /* delete */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         if(lv_alab != NULL)
         {
            if((lv_alab->irc_Node.ln_Succ)->ln_Succ == NULL)
            {
               irce = (struct IRCEntry *)lv_alab->irc_Node.ln_Pred;
               done--;
            }
            else
            {
               irce = (struct IRCEntry *)lv_alab->irc_Node.ln_Succ;
            }
            Remove((struct Node *)lv_alab);
            FreeVec(lv_alab);
            lv_alab = irce;
            num_entries--;
            GT_SetGadgetAttrs(lvgads[0], win, NULL,
                              GTLV_Labels, ircl,
                              TAG_END);
            if(!IsListEmpty(ircl))
            {
               GT_SetGadgetAttrs(lvgads[1], win, NULL,
                                 GTST_String, lv_alab->irc_Site.Site,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[2], win, NULL,
                                 GTST_String, lv_alab->irc_Site.Desc,
                                 TAG_END);
            }
         }
         break;
      case 8:  /* insert */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&ircs, 0, sizeof(struct IRCSite));
         InsertName(ircl, (struct Node *)lv_alab->irc_Node.ln_Pred, &ircs);
         lv_alab = (struct IRCEntry *)FindOrdinal(ircl, lvitem);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, ircl,
                           GTLV_Selected, lvitem,
                           GTLV_MakeVisible, lvitem,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GTST_String, lv_alab->irc_Site.Desc,
                           GA_Disabled, FALSE,
                           TAG_END);
         break;
   }

   if(IsListEmpty(ircl))
   {
      GT_SetGadgetAttrs(lvgads[1], win, NULL,
                        GA_Disabled, TRUE,
                        GTST_String, "",
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[2], win, NULL,
                        GA_Disabled, TRUE,
                        GTST_String, "",
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[6], win, NULL,
                        GA_Disabled, TRUE,
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[7], win, NULL,
                        GA_Disabled, TRUE,
                        TAG_END);
   }
   return (done);
}

ULONG handle_vanilla(struct Window *win,
                     struct IntuiMessage imsg,
                     struct Gadget *lvgads[],
                     ULONG lvitem)
{
   struct IRCEntry *irce;
   struct IRCSite   ircs;

   ULONG done = lvitem;

   switch(imsg.Code)
   {
      case 'A':   /* Add */
      case 'a':
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&ircs, 0, sizeof(struct IRCSite));
         AddName(ircl, &ircs);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, ircl,
                           GTLV_Selected, num_entries - 1,
                           GTLV_MakeVisible, num_entries - 1,
                           TAG_END);
         lv_alab = (struct IRCEntry *)FindOrdinal(ircl, num_entries - 1);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GTST_String, lv_alab->irc_Site.Desc,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[6], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[7], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         ActivateGadget(lvgads[1], win, NULL);
         done = num_entries - 1;
         break;
      case 'D':   /* Delete */
      case 'd':
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         if(lv_alab != NULL)
         {
            if((lv_alab->irc_Node.ln_Succ)->ln_Succ == NULL)
            {
               irce = (struct IRCEntry *)lv_alab->irc_Node.ln_Pred;
               done--;
            }
            else
            {
               irce = (struct IRCEntry *)lv_alab->irc_Node.ln_Succ;
            }
            Remove((struct Node *)lv_alab);
            FreeVec(lv_alab);
            lv_alab = irce;
            num_entries--;
            if(!IsListEmpty(ircl))
            {
               GT_SetGadgetAttrs(lvgads[1], win, NULL,
                                 GTST_String, lv_alab->irc_Site.Site,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[2], win, NULL,
                                 GTST_String, lv_alab->irc_Site.Desc,
                                 TAG_END);
            }
         }
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ircl,
                           TAG_END);
         break;
      case 'E':   /* desc */
      case 'e':
         ActivateGadget(lvgads[2], win, NULL);
         break;
      case 'I':   /* insert */
      case 'i':
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&ircs, 0, sizeof(struct IRCSite));
         InsertName(ircl, (struct Node *)lv_alab->irc_Node.ln_Pred, &ircs);
         lv_alab = (struct IRCEntry *)FindOrdinal(ircl, lvitem);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, ircl,
                           GTLV_Selected, lvitem,
                           GTLV_MakeVisible, lvitem,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GTST_String, lv_alab->irc_Site.Desc,
                           GA_Disabled, FALSE,
                           TAG_END);
         ActivateGadget(lvgads[1], win, NULL);
         done = lvitem;
         break;
      case 'Q':   /* Quit */
      case 'q':
         done = -2;
         break;
      case 'R':
      case 'r':
         if(done == -1)
         {
            done = 0;
            irce = (struct IRCEntry *)FindOrdinal(ircl, done);
            GT_SetGadgetAttrs(lvgads[0], win, NULL,
                              GTLV_Selected, done,
                              TAG_END);
            GT_SetGadgetAttrs(lvgads[1], win, NULL,
                              GA_Disabled, FALSE,
                              TAG_END);
            GT_SetGadgetAttrs(lvgads[2], win, NULL,
                              GA_Disabled, FALSE,
                              GTST_String, irce->irc_Site.Desc,
                              TAG_END);
            GT_SetGadgetAttrs(lvgads[6], win, NULL,
                              GA_Disabled, FALSE,
                              TAG_END);
            GT_SetGadgetAttrs(lvgads[7], win, NULL,
                              GA_Disabled, FALSE,
                              TAG_END);
         }
         break;
      case 'S':   /* Save */
      case 's':
         SetWindowTitles(win, "Saving...", NULL);
         save_data(ircl, "CINS:Cfg/IRC");
         SetWindowTitles(win, "Test", NULL);
         break;
      case 'T':   /* Site */
      case 't':
         ActivateGadget(lvgads[1], win, NULL);
         break;
      case CURSORUP:
         if(done != -1)
         {
            if((imsg.Qualifier & IEQUALIFIER_LSHIFT) || (imsg.Qualifier & IEQUALIFIER_RSHIFT))
            {
               if(((LONG)done - 6) > 0)
               {
                  done -= 6;
               }
               else
               {
                  done = 0;
               }
            }
            else
            {
               done > 0 ? done-- : done;
            }
            GT_SetGadgetAttrs(lvgads[0], win, NULL,
                              GTLV_Selected, done,
                              GTLV_MakeVisible, done,
                              TAG_END);
            lv_alab = (struct IRCEntry *)FindOrdinal(ircl, done);
            GT_SetGadgetAttrs(lvgads[2], win, NULL,
                              GTST_String, lv_alab->irc_Site.Desc,
                              GA_Disabled, FALSE,
                              TAG_END);
         }
         break;
      case CURSORDOWN:
         if(done != -1)
         {
            if((imsg.Qualifier & IEQUALIFIER_LSHIFT) || (imsg.Qualifier & IEQUALIFIER_RSHIFT))
            {
               if((done + 6) < (num_entries - 1))
               {
                  done += 6;
               }
               else
               {
                  done = num_entries - 1;
               }
            }
            else
            {
               done < (num_entries - 1) ? done++ : 0;
            }
            GT_SetGadgetAttrs(lvgads[0], win, NULL,
                              GTLV_Selected, done,
                              GTLV_MakeVisible, done,
                              TAG_END);
            lv_alab = (struct IRCEntry *)FindOrdinal(ircl, done);
            GT_SetGadgetAttrs(lvgads[2], win, NULL,
                              GTST_String, lv_alab->irc_Site.Desc,
                              GA_Disabled, FALSE,
                              TAG_END);
         }
         break;
   }

   return (done);
}

struct Gadget *init_gadgets(struct Gadget **glist,
                            struct Gadget *lvgads[],
                            ULONG Top,
                            APTR vi)
{
   struct NewGadget ng;
   struct Gadget    *gad,
                    *gcon;

   gad = gcon = CreateContext(glist);

   ng.ng_VisualInfo = vi;
   ng.ng_TextAttr   = &Topaz80;
   ng.ng_LeftEdge   = 80;
   ng.ng_Width      = 300;
   ng.ng_Height = 12;
   ng.ng_GadgetID = 2;
   ng.ng_GadgetText = "Si_te: ";
   ng.ng_Flags      = PLACETEXT_LEFT;
   
   lvgads [ 1 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_TopEdge    = 17 + Top;
   ng.ng_Height     = 72;
   ng.ng_GadgetID   = 1;
   ng.ng_GadgetText = "I_RC Servers";
   ng.ng_Flags      = PLACETEXT_ABOVE;

   ircl = init_list();

   lvgads [ 0 ] = gad = CreateGadget(LISTVIEW_KIND, gad, &ng,
                                     GTLV_Labels, ircl,
                                     GTLV_ShowSelected, lvgads[ 1 ],
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_TopEdge += 72 + 1;
   ng.ng_Height = 12;
   ng.ng_GadgetID = 3;
   ng.ng_GadgetText = "D_esc: ";
   ng.ng_Flags      = PLACETEXT_LEFT;

   lvgads [ 2 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_TopEdge += 12;
   ng.ng_GadgetID = 4;
   ng.ng_Width = 60;
   ng.ng_GadgetText = "_Save";
   ng.ng_Flags = PLACETEXT_IN;

   lvgads [ 3 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 5;
   ng.ng_GadgetText = "_Quit";

   lvgads [ 4 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 6;
   ng.ng_GadgetText = "_Add";

   lvgads [ 5 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 7;
   ng.ng_GadgetText = "_Delete";

   lvgads [ 6 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 8;
   ng.ng_GadgetText = "_Insert";

   lvgads [ 7 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   return(gcon);
}

struct List *init_list(void)
{
   BPTR fp;

   struct IRCSite ircs;

   memset(&ircs, 0, sizeof(struct IRCSite));

   if(ircl = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(ircl);

      if(fp = Open("CINS:Cfg/IRC", MODE_OLDFILE))
      {
         while(FGets(fp, ircs.Site, sizeof(ircs.Site)))
         {
            FGets(fp, ircs.Desc, sizeof(ircs.Desc));
            ircs.Site[strlen(ircs.Site)-1] = '\0';
            ircs.Desc[strlen(ircs.Desc)-1] = '\0';
            AddName(ircl, &ircs);
         }
         Close(fp);
      }
      return (ircl);
   }
   return NULL;
}

void free_list(struct List **list)
{
   struct Node *temp;
   struct Node *work;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         temp = work->ln_Succ;
         FreeVec(work);
         work = temp;
      }
      FreeVec(*list);

      *list = NULL;
   }
}

void AddName(struct List *list, struct IRCSite *ircs)
{
   struct IRCEntry *irc;

   if(irc = (struct IRCEntry *)AllocVec(sizeof(struct IRCEntry), MEMF_CLEAR))
   {
      CopyMem(ircs, (APTR)&irc->irc_Site, sizeof(struct IRCSite));
      irc->irc_Node.ln_Name = irc->irc_Site.Site;
      AddTail((struct List *)list, (struct Node *)irc);
      num_entries++;
   }
}

void InsertName(struct List *list, struct Node *node, struct IRCSite *ircs)
{
   struct IRCEntry *irc;

   if(irc = (struct IRCEntry *)AllocVec(sizeof(struct IRCEntry), MEMF_CLEAR))
   {
      CopyMem(ircs, (APTR)&irc->irc_Site, sizeof(struct IRCSite));
      irc->irc_Node.ln_Name = irc->irc_Site.Site;
      Insert(list, (struct Node *)irc, node);
      num_entries++;
   }
}

#ifndef CNET4
struct Node *FindOrdinal(struct List *list, int node)
{
   struct Node *tmp;

   tmp = (struct Node *)list->lh_Head;

   while(node && (tmp = (struct Node *)tmp->lv_Node.ln_Succ))
   {
      node--;
   }

   return (tmp);
}
#endif

BOOL save_irc_data(struct List *list, char *file)
{
   struct IRCEntry *work;
   struct IRCEntry *next;

   BPTR fp;

   if(!IsListEmpty(list) && (fp = Open(file, MODE_NEWFILE)))
   {
      work = (struct IRCEntry *)list->lh_Head;

      while(next = (struct IRCEntry *)work->irc_Node.ln_Succ)
      {
         FPrintf(fp, "%s\n", work->irc_Site.Site);
         FPrintf(fp, "%s\n", work->irc_Site.Desc);
         work = next;
      }
      Close(fp);
   }
   if(IsListEmpty(list))
   {
      DeleteFile(file);
   }
   return TRUE;
}
