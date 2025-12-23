#include <intuition/gadgetclass.h>
#include "CINS/CINS.h"

/* listview test */

void main(void);
struct List *init_labels(void);
struct List *init_data_labels(int file);
void free_list(struct List **list);
void free_data_list(struct List **list);
ULONG handle_IDCMP(struct Window *win,
                   struct Gadget *lvgads[]);
ULONG handle_gadget(struct IntuiMessage *imsg,
                   struct Gadget *lvgads[]);
ULONG handle_vanilla(struct IntuiMessage *imsg,
                     struct Gadget *lvgads[]);
struct Gadget *init_gadgets(struct Gadget **glist,
                            struct Gadget *lvgads[],
                            ULONG Top,
                            APTR vi);
void AddName(struct List *list,
             UBYTE *name);
void AddDataName(struct List *list,
            struct TelnetSite *tns);
void InsertName(struct List *list,
                struct Node *node,
                UBYTE *name);
void InsertDataName(struct List *list,
                    struct Node *node,
                    struct TelnetSite *tns);
#ifndef CNET4
struct Node *FindOrdinal(struct List *list,
              int node);
#endif
BOOL save_list(struct List *list,
               char *file);
BOOL save_data_list(struct List *list,
                   char *file);
LONG CountList(struct List *list);

struct IntuitionBase *IntuitionBase;
struct Library       *GadToolsBase;
struct GfxBase       *GfxBase;
struct Library       *CNetBase;

struct List  *lv_lab;
struct TextAttr Topaz80 = {"topaz.font", 8, 0, 0};

struct lv_label
{
   struct Node lv_Node;
   LONG  Serial;
   LONG  Entries;
   char  name [ 128 ];
   struct List *sitelist;
};

struct lv_sites
{
   struct Node ls_Node;
   LONG  Serial;
   struct TelnetSite tn_Site;
};

struct lv_label *lv_alab = NULL;
struct lv_sites *ls_alab = NULL;
LONG num_entries = 0;
LONG hi_num_entries = 0;
LONG num_data_entries = 0;
LONG lv_alab_num = -1;
LONG ls_alab_num = -1;

void main(void)
{
   struct Screen *scr;
   struct Window *win;
   struct Gadget *lvgads[ 15 ],
                 *glist;

   APTR vi;
   ULONG signal;
   BOOL  done = FALSE;

   if(IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37))
   {
      if(GadToolsBase = OpenLibrary("gadtools.library", 37))
      {
         if(GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 37))
         {
            if(CNetBase = OpenLibrary("cnet.library", 3))
            {
               if(scr = LockPubScreen(NULL))
               {
                  if(vi = GetVisualInfo(scr, TAG_END))
                  {
                     if(glist = init_gadgets(&glist, lvgads, scr->Font->ta_YSize, vi))
                     {
                        if(win = OpenWindowTags(NULL,
                                       WA_Title,  "Test",
                                       WA_Width,  600,
                                       WA_Height, 140,
                                       WA_MinWidth, 70,
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
                                                 IDCMP_VANILLAKEY,
                                       TAG_END))
                        {
                           GT_RefreshWindow(win, NULL);
                           while(!done)
                           {
                              if(IsListEmpty(lv_lab))
                                 GT_SetGadgetAttrs(lvgads[0], win, NULL,
                                                   GA_Disabled, TRUE,
                                                   TAG_END);
                              signal = Wait(1L << win->UserPort->mp_SigBit);
                              done = handle_IDCMP(win, lvgads);
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
   if(lv_lab)
   {
      free_list(&lv_lab);
   }
   return;
}

ULONG handle_IDCMP(struct Window *win, struct Gadget *lvgads[])
{
   struct IntuiMessage *msg,
                        imsg;

   BOOL done = FALSE;

   while(!done && (msg = GT_GetIMsg(win->UserPort)))
   {
      CopyMem(msg, &imsg, sizeof(struct IntuiMessage));

      GT_ReplyIMsg(msg);

      switch(imsg.Class)
      {
         case IDCMP_CLOSEWINDOW:
            done = TRUE;
            break;
         case IDCMP_GADGETUP:
            done = handle_gadget(&imsg, lvgads);
            break;
         case IDCMP_VANILLAKEY:
            done = handle_vanilla(&imsg, lvgads);
            break;
         case IDCMP_REFRESHWINDOW:
            GT_BeginRefresh(win);
            GT_EndRefresh(win, TRUE);
            break;
      }
   }
   return (done);
}

ULONG handle_gadget(struct IntuiMessage *imsg,
                    struct Gadget *lvgads[])
{
   struct Window   *win = imsg->IDCMPWindow;
   struct lv_label *lab;
   struct lv_sites *lsi;
   struct Gadget   *gad;
   struct TelnetSite tns;
   char *tmp;

   BOOL done = FALSE;

   gad = (struct Gadget *)imsg->IAddress;

   switch(gad->GadgetID)
   {
      case 1:  /* listview */
         if(lv_alab_num == imsg->Code)
            break;
         lv_alab_num = imsg->Code;
         lv_alab = (struct lv_label *)FindOrdinal(lv_lab, lv_alab_num);
         GT_SetGadgetAttrs(lvgads[1], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[6], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[7], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, NULL,
                           TAG_END);
         if(!IsListEmpty(lv_alab->sitelist))
         {
            GT_SetGadgetAttrs(lvgads[8], win, NULL,
                              GA_Disabled, FALSE,
                              GTLV_Labels, lv_alab->sitelist,
                              TAG_END);
         }
         else
            GT_SetGadgetAttrs(lvgads[8], win, NULL,
                              GA_Disabled, TRUE,
                              TAG_END);
         ls_alab = NULL;
         ls_alab_num = -1;
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[9], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[10], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[11], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[12], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[13], win, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[14], win, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         break;
      case 2:  /* name */
         tmp = ((struct StringInfo *)lvgads[1]->SpecialInfo)->Buffer;
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         strcpy(lv_alab->name, tmp);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, lv_lab,
                           TAG_END);
         break;
      case 3:  /* name-lv */
         tmp = ((struct StringInfo *)lvgads[2]->SpecialInfo)->Buffer;
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         strcpy(ls_alab->tn_Site.Site, tmp);
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, lv_alab->sitelist,
                           TAG_END);
         break;
      case 4: /* save */
         SetWindowTitles(win, "Saving...", NULL);
         save_list(lv_lab, "CINS:Telnet/Menu");
         SetWindowTitles(win, "Test", NULL);
         break;
      case 5:  /* quit */
         done = TRUE;
         break;
      case 6:  /* add */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         AddName(lv_lab, "");
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, lv_lab,
                           GTLV_Selected, num_entries - 1,
                           GTLV_MakeVisible, num_entries - 1,
                           TAG_END);
         lv_alab_num = num_entries - 1;
         lv_alab = (struct lv_label *)FindOrdinal(lv_lab, lv_alab_num);
         GT_SetGadgetAttrs(lvgads[1], win, NULL,
                           GTST_String, lv_alab->name,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[6], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[7], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[12], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         ActivateGadget(lvgads[1], win, NULL);
         break;
      case 7:  /* delete */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         if(lv_alab != NULL)
         {
            if((lv_alab->lv_Node.ln_Succ)->ln_Succ == NULL)
            {
               lab = (struct lv_label *)lv_alab->lv_Node.ln_Pred;
               lv_alab_num--;
            }
            else
            {
               lab = (struct lv_label *)lv_alab->lv_Node.ln_Succ;
            }
            if(lv_alab->sitelist)
            {
               GT_SetGadgetAttrs(lvgads[8], win, NULL,
                                 GTLV_Labels, NULL,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[9], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[10], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[11], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[2], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
               free_data_list(&lv_alab->sitelist);
               ls_alab = NULL;
               ls_alab_num = -1;
            }
            Remove((struct Node *)lv_alab);
            FreeVec(lv_alab);
            lv_alab = lab;
            num_entries--;
            GT_SetGadgetAttrs(lvgads[0], win, NULL,
                              GTLV_Labels, lv_lab,
                              TAG_END);
            GT_SetGadgetAttrs(lvgads[8], win, NULL,
                              GTLV_Labels, lv_alab->sitelist,
                              GA_Disabled, FALSE,
                              TAG_END);
            if(IsListEmpty(lv_alab->sitelist))
               GT_SetGadgetAttrs(lvgads[8], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
            if(!IsListEmpty(lv_lab))
            {
               GT_SetGadgetAttrs(lvgads[1], win, NULL,
                                 GTST_String, lv_alab->name,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[2], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
            }
         }
         break;
      case 8:  /* insert */
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         InsertName(lv_lab, (struct Node *)lv_alab->lv_Node.ln_Pred, "");
         lv_alab = (struct lv_label *)FindOrdinal(lv_lab, lv_alab_num);
         GT_SetGadgetAttrs(lvgads[0], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, lv_lab,
                           GTLV_Selected, lv_alab_num,
                           GTLV_MakeVisible, lv_alab_num,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[9], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[10], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[11], win, NULL,
                           GA_Disabled, TRUE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GA_Disabled, TRUE,
                           TAG_END);
         break;
      case 9:
         if(ls_alab_num == imsg->Code)
            break;
         ls_alab_num = imsg->Code;
         ls_alab = (struct lv_sites *)FindOrdinal(lv_alab->sitelist, ls_alab_num);
         GT_SetGadgetAttrs(lvgads[2], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[9], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, ls_alab->tn_Site.Comm1,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[10], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, ls_alab->tn_Site.Comm2,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[11], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, ls_alab->tn_Site.Comm3,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[13], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[14], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         break;
      case 10:
         strcpy(ls_alab->tn_Site.Comm1, ((struct StringInfo *)gad->SpecialInfo)->Buffer);
         break;
      case 11:
         strcpy(ls_alab->tn_Site.Comm2, ((struct StringInfo *)gad->SpecialInfo)->Buffer);
         break;
      case 12:
         strcpy(ls_alab->tn_Site.Comm3, ((struct StringInfo *)gad->SpecialInfo)->Buffer);
         break;
      case 13: /* add-sub */
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&tns, 0, sizeof(struct TelnetSite));
         AddDataName(lv_alab->sitelist, &tns);
         ls_alab_num = lv_alab->Entries = CountList(lv_alab->sitelist);
         ls_alab_num--;
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, lv_alab->sitelist,
                           GTLV_Selected, ls_alab_num,
                           GTLV_MakeVisible, ls_alab_num,
                           TAG_END);
         ls_alab = (struct lv_sites *)FindOrdinal(lv_alab->sitelist, ls_alab_num);
         GT_SetGadgetAttrs(lvgads[9], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[10], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[11], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[13], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[14], win, NULL,
                           GA_Disabled, FALSE,
                           TAG_END);
         ActivateGadget(lvgads[2], win, NULL);
         break;
      case 14: /* insert-sub */
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         memset(&tns, 0, sizeof(struct TelnetSite));
         InsertDataName(lv_alab->sitelist, (struct Node *)ls_alab->ls_Node.ln_Pred, &tns);
         ls_alab = (struct lv_sites *)FindOrdinal(lv_alab->sitelist, ls_alab_num);
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GA_Disabled, FALSE,
                           GTLV_Labels, lv_alab->sitelist,
                           GTLV_Selected, ls_alab_num,
                           GTLV_MakeVisible, ls_alab_num,
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[9], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[10], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         GT_SetGadgetAttrs(lvgads[11], win, NULL,
                           GA_Disabled, FALSE,
                           GTST_String, "",
                           TAG_END);
         break;
      case 15: /* delete-sub */
         GT_SetGadgetAttrs(lvgads[8], win, NULL,
                           GTLV_Labels, ~0,
                           TAG_END);
         if(ls_alab != NULL)
         {
            if((ls_alab->ls_Node.ln_Succ)->ln_Succ == NULL)
            {
               lsi = (struct lv_sites *)ls_alab->ls_Node.ln_Pred;
               ls_alab_num--;
            }
            else
               lsi = (struct lv_sites *)ls_alab->ls_Node.ln_Succ;
            Remove((struct Node *)ls_alab);
            FreeVec(ls_alab);
            ls_alab = lsi;
            if(!IsListEmpty(lv_alab->sitelist))
            {
               GT_SetGadgetAttrs(lvgads[8], win, NULL,
                                 GTLV_Labels, lv_alab->sitelist,
                                 TAG_END);
               lv_alab->Entries = CountList(lv_alab->sitelist);
               GT_SetGadgetAttrs(lvgads[9], win, NULL,
                                 GTST_String, ls_alab->tn_Site.Comm1,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[10], win, NULL,
                                 GTST_String, ls_alab->tn_Site.Comm2,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[11], win, NULL,
                                 GTST_String, ls_alab->tn_Site.Comm3,
                                 TAG_END);
            }
            else
            {
               GT_SetGadgetAttrs(lvgads[2], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[8], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTLV_Labels, NULL,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[9], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[10], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[11], win, NULL,
                                 GA_Disabled, TRUE,
                                 GTST_String, "",
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[13], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
               GT_SetGadgetAttrs(lvgads[14], win, NULL,
                                 GA_Disabled, TRUE,
                                 TAG_END);
            }
         }
         break;
   }

   if(IsListEmpty(lv_lab))
   {
      GT_SetGadgetAttrs(lvgads[1], win, NULL,
                        GA_Disabled, TRUE,
                        GTST_String, "",
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[6], win, NULL,
                        GA_Disabled, TRUE,
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[7], win, NULL,
                        GA_Disabled, TRUE,
                        TAG_END);
      GT_SetGadgetAttrs(lvgads[8], win, NULL,
                        GA_Disabled, TRUE,
                        TAG_END);
   }
   return (done);
}

ULONG handle_vanilla(struct IntuiMessage *imsg,
                     struct Gadget *lvgads[])
{
   BOOL done = FALSE;

   switch(imsg->Code)
   {
      case 'Q':   /* Quit */
      case 'q':
         done = TRUE;
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
   ng.ng_LeftEdge   = 5;
   ng.ng_Width      = 300;
   ng.ng_Height = 12;
   ng.ng_GadgetID = 2;
   ng.ng_GadgetText = "";
   ng.ng_Flags      = PLACETEXT_LEFT;
   
   lvgads [ 1 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_TopEdge    = 15 + Top;
   ng.ng_Height     = 72;
   ng.ng_GadgetID   = 1;
   ng.ng_GadgetText = "";
   ng.ng_Flags      = PLACETEXT_ABOVE;

   if(!(lv_lab = init_labels()))
      return (gcon);

   hi_num_entries = num_entries;

   lvgads [ 0 ] = gad = CreateGadget(LISTVIEW_KIND, gad, &ng,
                                     GTLV_Labels, lv_lab,
                                     GTLV_ShowSelected, lvgads[ 1 ],
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_Width  = 240;
   ng.ng_Height = 12;
   ng.ng_GadgetID = 3;
   ng.ng_GadgetText = "";
   ng.ng_Flags      = PLACETEXT_LEFT;
   
   lvgads [ 2 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 300;
   ng.ng_Height    = 72;
   ng.ng_GadgetText = "";
   ng.ng_GadgetID  = 9;

   lvgads [ 8 ] = gad = CreateGadget(LISTVIEW_KIND, gad, &ng,
                                    GTLV_ShowSelected, lvgads[ 2 ],
                                    GA_Disabled, TRUE,
                                    TAG_END);

   ng.ng_LeftEdge = 5;
   ng.ng_TopEdge = 3 + Top;
   ng.ng_GadgetID = 4;
   ng.ng_Height = 12;
   ng.ng_Width = 60;
   ng.ng_GadgetText = "Save";
   ng.ng_Flags = PLACETEXT_IN;

   lvgads [ 3 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 5;
   ng.ng_GadgetText = "_Quit";

   lvgads [ 4 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GT_Underscore, '_',
                                     TAG_END);

   ng.ng_LeftEdge += 60;
   ng.ng_GadgetID = 6;
   ng.ng_GadgetText = "Add";

   lvgads [ 5 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     TAG_END);

   ng.ng_LeftEdge += 120;
   ng.ng_GadgetID = 7;
   ng.ng_GadgetText = "Delete";

   lvgads [ 6 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_LeftEdge -= 60;
   ng.ng_GadgetID = 8;
   ng.ng_GadgetText = "Insert";

   lvgads [ 7 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_TopEdge += 85;
   ng.ng_LeftEdge += 120;
   ng.ng_Width = 240;
   ng.ng_GadgetID = 10;
   ng.ng_Flags = PLACETEXT_LEFT;
   ng.ng_GadgetText = "Hostname:";

   lvgads [ 9 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_TopEdge += 12;
   ng.ng_GadgetText = "Comment #1:";
   ng.ng_GadgetID = 11;

   lvgads [ 10 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_TopEdge += 12;
   ng.ng_GadgetText = "Comment #2:";
   ng.ng_GadgetID = 12;

   lvgads [ 11 ] = gad = CreateGadget(STRING_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_TopEdge = 3 + Top;
   ng.ng_GadgetText = "Add";
   ng.ng_Flags = PLACETEXT_IN;
   ng.ng_Width = 80;
   ng.ng_GadgetID = 13;

   lvgads [ 12 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_LeftEdge += 80;
   ng.ng_GadgetText = "Insert";
   ng.ng_GadgetID = 14;

   lvgads [ 13 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);

   ng.ng_LeftEdge += 80;
   ng.ng_GadgetText = "Delete";
   ng.ng_GadgetID = 15;

   lvgads [ 14 ] = gad = CreateGadget(BUTTON_KIND, gad, &ng,
                                     GA_Disabled, TRUE,
                                     TAG_END);
   return(gcon);
}

struct List *init_labels(void)
{
   struct List *list = NULL;
   struct lv_label *lab;

   BPTR fp;
   LONG i;

   char buff [ 128 ] = "";

   if(list = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(list);

      if(fp = Open("CINS:Telnet/Menu", MODE_OLDFILE))
      {
         while(FGets(fp, buff, 128))
         {
            if(*buff)
            {
               buff[strlen(buff) - 1] = '\0';
               AddName(list, buff);
            }
         }
         Close(fp);
      }
   }
   for(i = 0; i < num_entries; i++)
   {
      lab = (struct lv_label *)FindOrdinal(list, i);
      lab->sitelist = init_data_labels(i + 1);
      lab->Entries  = CountList(lab->sitelist);
   }

   return (list);
}

struct List *init_data_labels(int file)
{
   struct List *list = NULL;
   struct TelnetSite tns;

   char buff [ 128 ] = "";

   BPTR fp;

   memset(&tns, 0, sizeof(struct TelnetSite));

   if(list = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(list);

      sprintf(buff, "CINS:Telnet/%ld", file);

      if(fp = Open(buff, MODE_OLDFILE))
      {
         while(FGets(fp, tns.Site, sizeof(tns.Site)))
         {
            tns.Site[strlen(tns.Site) - 1] = '\0';
            FGets(fp, tns.Comm1, sizeof(tns.Comm1));
            tns.Comm1[strlen(tns.Comm1) - 1] = '\0';
            FGets(fp, tns.Comm2, 128);
            tns.Comm2[strlen(tns.Comm2) - 1] = '\0';
            FGets(fp, tns.Comm3, 128);
            tns.Comm3[strlen(tns.Comm3) - 1] = '\0';
            AddDataName(list, &tns);
         }
         Close(fp);
      }
   }
   return (list);
}

void free_list(struct List **list)
{
   struct Node *work;
   struct Node *next;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         if(((struct lv_label *)work)->sitelist)
         {
            free_data_list(&((struct lv_label *)work)->sitelist);
         }
         next = work->ln_Succ;
         FreeVec(work);
         work = next;
      }
      FreeVec(*list);

      *list = NULL;
   }
}

void free_data_list(struct List **list)
{
   struct Node *work;
   struct Node *next;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         next = work->ln_Succ;
         FreeVec(work);
         work = next;
      }
      FreeVec(*list);

      *list = NULL;
   }
}

void AddName(struct List *list, UBYTE *name)
{
   struct lv_label *lab;

   if(lab = (struct lv_label *)AllocVec(sizeof(struct lv_label), MEMF_CLEAR))
   {
      strcpy(lab->name, name);
      lab->lv_Node.ln_Name = lab->name;
      lab->Serial = num_entries++;
      lab->sitelist = init_data_labels(NULL);
      AddTail((struct List *)list, (struct Node *)lab);
   }
}

void AddDataName(struct List *list, struct TelnetSite *tns)
{
   struct lv_sites *lab;

   if(lab = (struct lv_sites *)AllocVec(sizeof(struct lv_sites), MEMF_CLEAR))
   {
      CopyMem(tns, (APTR)&lab->tn_Site, sizeof(struct TelnetSite));
      lab->ls_Node.ln_Name = lab->tn_Site.Site;
      lab->Serial = num_data_entries++;
      AddTail((struct List *)list, (struct Node *)lab);
   }
}

void InsertName(struct List *list, struct Node *node, UBYTE *name)
{
   struct lv_label *lab;

   if(lab = (struct lv_label *)AllocVec(sizeof(struct lv_label), MEMF_CLEAR))
   {
      strcpy(lab->name, name);
      lab->lv_Node.ln_Name = lab->name;
      lab->Serial = hi_num_entries++;
      lab->sitelist = init_data_labels(hi_num_entries);
      Insert(list, (struct Node *)lab, node);
      num_entries++;
   }
}

void InsertDataName(struct List *list, struct Node *node, struct TelnetSite *tns)
{
   struct lv_sites *lab;

   if(lab = (struct lv_sites *)AllocVec(sizeof(struct lv_sites), MEMF_CLEAR))
   {
      CopyMem(tns, (APTR)&lab->tn_Site, sizeof(struct TelnetSite));
      lab->ls_Node.ln_Name = lab->tn_Site.Site;
      lab->Serial = num_data_entries++;
      Insert(list, (struct Node *)lab, node);
   }
}

#ifndef CNET4
struct Node *FindOrdinal(struct List *list, int node)
{
   struct Node *tmp;

   tmp = list->lh_Head;

   while(node && (tmp = tmp->ln_Succ))
   {
      node--;
   }

   return (tmp);
}
#endif

BOOL save_list(struct List *list, char *file)
{
   struct lv_label *work;
   struct lv_label *next;

   LONG seq = 1;

   char buff [ 128 ] = "";

   BPTR fp;

   if(!IsListEmpty(list) && (fp = Open(file, MODE_NEWFILE)))
   {
      work = (struct lv_label *)list->lh_Head;

      while(next = (struct lv_label *)work->lv_Node.ln_Succ)
      {
         FPrintf(fp, "%s\n", work->name);
         if(work->sitelist)
         {
            sprintf(buff, "CINS:Telnet/%ld", seq);
            if(save_data_list(work->sitelist, buff))
               seq++;
         }
         work = next;
      }
      Close(fp);
   }
   if(IsListEmpty(list))
      DeleteFile(file);
   for(; seq < hi_num_entries + 1; seq++)
   {
      sprintf(buff, "CINS:Telnet/%ld", seq);
      DeleteFile(buff);
   }
   return TRUE;
}

LONG CountList(struct List *list)
{
   struct Node *temp;

   LONG number = 0;

   temp = list->lh_Head;

   while(temp && (temp = temp->ln_Succ))
      number++;

   return (number);
}

BOOL save_data_list(struct List *list, char *file)
{
   struct lv_sites *work;
   struct lv_sites *next;

   BPTR fp;

   if(!IsListEmpty(list) && (fp = Open(file, MODE_NEWFILE)))
   {
      work = (struct lv_sites *)list->lh_Head;

      while(next = (struct lv_sites *)work->ls_Node.ln_Succ)
      {
         FPrintf(fp, "%s\n", work->tn_Site.Site);
         FPrintf(fp, "%s\n", work->tn_Site.Comm1);
         FPrintf(fp, "%s\n", work->tn_Site.Comm2);
         FPrintf(fp, "%s\n", work->tn_Site.Comm3);
         work = next;
      }
      Close(fp);
   }
   if(IsListEmpty(list))
   {
      DeleteFile(file);
      return FALSE;
   }
   return TRUE;
}
