/*
** CNdomo Configuration Utility
** by Kelly Cochran
** Last updated: Sat Oct 17 20:29:08 1998
*/

#include "structs.h"
#include "CNdomoConfig.h"

LONG list_lv_id    = -1;
LONG user_lv_id    = -1;
LONG poster_lv_id  = -1;
LONG pending_lv_id = -1;
LONG banned_lv_id  = -1;

void           gui_main(struct List *ll);
struct Window *OpenCNDMainWin(struct List *list, Object **CND_MainWO, Object **CND_Gads);
BOOL           handle_CND_Main(struct Window *win, struct List *list, Object *WO, Object **CND_Gads);
BOOL           load_config(struct List **ll);
BOOL parse_user_line(char *line, struct UserEntry *ue);
void AddListToList(struct List *list, struct ListEntry *le);
void AddUserToList(struct List *list, struct UserEntry *ue);
void free_config(struct List **list);
void save_config(struct List *list);
void rethink_CND_window(struct Window *win, struct ListEntryNode *len, Object **CND_Gads);
ULONG MakeRequest(struct Window *win, char *title, char *gads, char *string, ...);

void main(void)
{
   struct List *ll = NULL;

   if(CNetBase = OpenLibrary("cnet.library", 4))
   {
      if(BGUIBase = OpenLibrary(BGUINAME, BGUIVERSION))
      {
         if(load_config(&ll))
         {
            gui_main(ll);

            free_config(&ll);
         }
         CloseLibrary(BGUIBase);
      }
      CloseLibrary(CNetBase);
   }
}

void gui_main(struct List *ll)
{
   Object *CND_MainWO;
   Object *CND_Gads [ CND_NUM_OBJS ];

   struct Window *CND_Main;

   ULONG signal;

   BOOL done;

   if(CNDMsgPort = CreateMsgPort())
   {
      signal = 1 << CNDMsgPort->mp_SigBit;

      if(CND_Main = OpenCNDMainWin(ll, &CND_MainWO, CND_Gads))
      {
         do
         {
            Wait(signal);
            done = handle_CND_Main(CND_Main, ll, CND_MainWO, CND_Gads);
         } while(!done);

         if(CND_MainWO)    DisposeObject(CND_MainWO);
      }
      DeleteMsgPort(CNDMsgPort);
   }
}

BOOL handle_CND_Main(struct Window *win, struct List *list, Object *CND_MainWO, Object **CND_Gads)
{
   ULONG rc;

   BOOL done = FALSE;

   LONG temp;

   UBYTE *name,
         *str;

   BPTR lock;

   struct UserEntry      ue;
   struct UserEntryNode *uen;
   struct ListEntry      le;

   static struct ListEntryNode *len  = NULL,
                               *olen = NULL;
   static char   buff [ 128 ] = "";
   static int    New = 1;

   while((rc = HandleEvent(CND_MainWO)) != WMHI_NOMORE)
   {
      switch(rc)
      {
         case WMHI_CLOSEWINDOW:
         case CND_QUIT:
            done = TRUE;
            break;

         case CND_ABOUT:
            MakeRequest(win, "About CNDomoConfig", "Okay", 
               "\33cCNdomoConfig "CNDC_VERS"."CNDC_REVS"\n"
               "Copyright 1998\n"
               "Kelly Cochran\n\n"
               "Compile date: "__DATE__);
            break;

         case CND_SAVE:
            save_config(list);
            break;

         case CND_LIST_LV:
            if(list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DELETE     ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME       ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_FROM       ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_REPLYTO    ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ERRORTO    ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_PASSWD     ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_MAXPOSTSIZE], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_OWNER      ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ARTICLENUM ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTNUM  ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CND_Gads[CND_LIST_LV], (ULONG *)&list_lv_id);

            olen = len;

            if(len = (struct ListEntryNode *)FindOrdinal(list, list_lv_id))
            {
               if(olen != len)
               {
                  rethink_CND_window(win, len, CND_Gads);
               }
            }
            break;

         case CND_LIST_ADD:
            if(list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DELETE     ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME       ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_FROM       ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_REPLYTO    ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ERRORTO    ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_PASSWD     ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_MAXPOSTSIZE], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_OWNER      ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ARTICLENUM ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTNUM  ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&le, 0, sizeof(struct ListEntry));

            sprintf(le.le_Name, "New%ld", New++);
            le.le_Users   = init_list();
            le.le_Posters = init_list();
            le.le_Pending = init_list();
            le.le_Banned  = init_list();
            AddListToList(list, &le);

            sprintf(buff, LISTDIR"/%s", le.le_Name);

            if(lock = CreateDir(buff)) UnLock(lock);

            GetAttr(LISTV_NumEntries, CND_Gads[CND_LIST_LV], (ULONG *)&list_lv_id);

            if(len = (struct ListEntryNode *)FindOrdinal(list, list_lv_id))
            {
               AddEntrySelect(win, CND_Gads[CND_LIST_LV], &len->le.le_Name, LVAP_TAIL);

               rethink_CND_window(win, len, CND_Gads);
            }

            break;

         case CND_LIST_DELETE:
            RemoveSelected(win, CND_Gads[CND_LIST_LV]);

            if(len && *len->le.le_Name)
            {
               sprintf(buff, "Delete \""LISTDIR"/%s\" ALL FORCE QUIET", len->le.le_Name);
               SystemTags(buff, TAG_END);

               if(len->le.le_Users  ) free_list(&len->le.le_Users);
               if(len->le.le_Posters) free_list(&len->le.le_Posters);
               if(len->le.le_Pending) free_list(&len->le.le_Pending);
               if(len->le.le_Banned ) free_list(&len->le.le_Banned);

               if((len->le_Node.ln_Succ)->ln_Succ == NULL)
               {
                  RemoveNode(list, list_lv_id);
                  list_lv_id--;
               }
               else
               {
                  RemoveNode(list, list_lv_id);
               }
            }

            if(list_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DELETE     ], win, NULL, GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME       ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_FROM       ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_REPLYTO    ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ERRORTO    ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_PASSWD     ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_MAXPOSTSIZE], win, NULL, STRINGA_LongVal, 0 , GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_OWNER      ], win, NULL, STRINGA_TextVal, "", GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ARTICLENUM ], win, NULL, STRINGA_LongVal, 0 , GA_Disabled, TRUE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTNUM  ], win, NULL, STRINGA_LongVal, 0 , GA_Disabled, TRUE, TAG_END);

               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], win, NULL,
                  MX_DisableButton, 1L,
                  MX_DisableButton, 2L,
                  MX_DisableButton, 3L,
                  MX_DisableButton, 4L,
                  MX_DisableButton, 5L,
                  TAG_END);

               len = NULL;
            }
            else
            {
               if(len = (struct ListEntryNode *)FindOrdinal(list, list_lv_id))
               {
                  rethink_CND_window(win, len, CND_Gads);
               }
            }
            break;

         case CND_LIST_NAME:
            if(name = (UBYTE *)FirstSelected(CND_Gads[CND_LIST_LV]))
            {
               char buf1 [  64 ] = "";
               char buf2 [ 128 ] = "";

               strcpy(buf1, len->le.le_Name);   /* Save old name */
               GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_NAME], (ULONG *)&str);
               if(*str)
               {
                  strcpy(len->le.le_Name, str);
                  ReplaceEntry(win, CND_Gads[CND_LIST_LV], name, len->le.le_Name);

                  sprintf(buff, LISTDIR"/%s", buf1);
                  sprintf(buf2, LISTDIR"/%s", str);
                  Rename(buff, buf2);
               }
               else
               {
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME], win, NULL, STRINGA_TextVal, len->le.le_Name, TAG_END);
               }
            }
            break;

         case CND_LIST_FROM:
            GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_FROM], (ULONG *)&str);
            strcpy(len->le.le_From, str);
            break;

         case CND_LIST_REPLYTO:
            GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_REPLYTO], (ULONG *)&str);
            strcpy(len->le.le_ReplyTo, str);
            break;

         case CND_LIST_ERRORTO:
            GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_ERRORTO], (ULONG *)&str);
            strcpy(len->le.le_ErrorTo, str);
            break;

         case CND_LIST_PASSWD:
            GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_PASSWD], (ULONG *)&str);
            strcpy(len->le.le_Passwd, str);
            break;

         case CND_LIST_DIGESTSIZE:
            GetAttr(STRINGA_LongVal, CND_Gads[CND_LIST_DIGESTSIZE], (ULONG *)&temp);
            len->le.le_DigestSize = temp;
            break;

         case CND_LIST_ARTICLENUM:
            GetAttr(STRINGA_LongVal, CND_Gads[CND_LIST_ARTICLENUM], (ULONG *)&temp);
            len->le.le_Number = temp;
            break;

         case CND_LIST_DIGESTNUM:
            GetAttr(STRINGA_LongVal, CND_Gads[CND_LIST_DIGESTNUM], (ULONG *)&temp);
            len->le.le_DigestNum = temp;
            break;

         case CND_LIST_MAXPOSTSIZE:
            GetAttr(STRINGA_LongVal, CND_Gads[CND_LIST_MAXPOSTSIZE], (ULONG *)&temp);
            len->le.le_MaxPostSize = temp;
            break;

         case CND_LIST_OWNER:
            GetAttr(STRINGA_TextVal, CND_Gads[CND_LIST_OWNER], (ULONG *)&str);
            strcpy(len->le.le_Owner, str);
            break;

         case CND_FLAGS_CLOSED:
            len->le.le_Flags ^= LEF_CLOSED;

            SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], win, NULL,
               len->le.le_Flags & (LEF_MODERATED|LEF_CLOSED) ? MX_EnableButton:MX_DisableButton, 3L,
               TAG_END);
            break;

         case CND_FLAGS_MODERATED:
            len->le.le_Flags ^= LEF_MODERATED;

            SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], win, NULL,
               len->le.le_Flags & (LEF_MODERATED|LEF_CLOSED) ? MX_EnableButton:MX_DisableButton, 3L,
               TAG_END);
            break;

         case CND_FLAGS_DIGEST:
            len->le.le_Flags ^= LEF_DIGEST;

            SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTSIZE], win, NULL,
               GA_Disabled, !(len->le.le_Flags & LEF_DIGEST),
               TAG_END);
            break;

         case CND_FLAGS_CONFIRM:
            len->le.le_Flags ^= LEF_CONFIRM;

            SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], win, NULL,
               (len->le.le_Flags & LEF_CONFIRM) ? MX_EnableButton:MX_DisableButton, 4L,
               TAG_END);
            break;

         case CND_FLAGS_HANDLEADMIN:
            len->le.le_Flags ^= LEF_HANDLEADMIN;
            break;

         case CND_FLAGS_PRIVATE:
            len->le.le_Flags ^= LEF_PRIVATE;
            break;

         case CND_FLAGS_PREFIXLIST:
            len->le.le_Flags ^= LEF_PREFIXLIST;
            break;

         case CND_FLAGS_SETLISTFROM:
            len->le.le_Flags ^= LEF_SETLISTFROM;
            break;

         case CND_FLAGS_SENDSPOOL:
            len->le.le_Flags ^= LEF_SENDSPOOL;
            break;

         case CND_FLAGS_ARCHIVE:
            len->le.le_Flags ^= LEF_ARCHIVE;
            break;

         case CND_USER_LV:
            if(user_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CND_Gads[CND_USER_LV], (ULONG *)&user_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_USER_ADD:
            if(user_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ue, 0, sizeof(struct UserEntry));
            AddUserToList(len->le.le_Users, &ue);

            GetAttr(LISTV_NumEntries, CND_Gads[CND_USER_LV], (ULONG *)&user_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
            {
               AddEntrySelect(win, CND_Gads[CND_USER_LV], &uen->ue.ue_From, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_USER_DELETE:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
            {
               RemoveSelected(win, CND_Gads[CND_USER_LV]);
               if(uen && ((uen->ue_Node.ln_Succ)->ln_Succ == NULL))
               {
                  RemoveNode(len->le.le_Users, user_lv_id);
                  user_lv_id--;
               }
               else
               {
                  RemoveNode(len->le.le_Users, user_lv_id);
               }

               if(user_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_DELETE ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               }
               else
               {
                  if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
                  {
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
                  }
               }
            }
            break;

         case CND_USER_ADDRESS:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
            {
               if(name = (UBYTE *)FirstSelected(CND_Gads[CND_USER_LV]))
               {
                  GetAttr(STRINGA_TextVal, CND_Gads[CND_USER_ADDRESS], (ULONG *)&str);
                  strcpy(uen->ue.ue_From, str);
                  ReplaceEntry(win, CND_Gads[CND_USER_LV], name, uen->ue.ue_From);
               }
            }
            break;

         case CND_USER_PASSWD:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Users, user_lv_id))
            {
               GetAttr(STRINGA_TextVal, CND_Gads[CND_USER_PASSWD], (ULONG *)&str);
               strcpy(uen->ue.ue_Key, str);
            }
            break;

         case CND_POSTER_LV:
            if(poster_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CND_Gads[CND_POSTER_LV], (ULONG *)&poster_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_POSTER_ADD:
            if(poster_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ue, 0, sizeof(struct UserEntry));
            AddUserToList(len->le.le_Posters, &ue);

            GetAttr(LISTV_NumEntries, CND_Gads[CND_POSTER_LV], (ULONG *)&poster_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
            {
               AddEntrySelect(win, CND_Gads[CND_POSTER_LV], &uen->ue.ue_From, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_POSTER_DELETE:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
            {
               RemoveSelected(win, CND_Gads[CND_POSTER_LV]);
               if(uen && ((uen->ue_Node.ln_Succ)->ln_Succ == NULL))
               {
                  RemoveNode(len->le.le_Posters, poster_lv_id);
                  poster_lv_id--;
               }
               else
               {
                  RemoveNode(len->le.le_Posters, poster_lv_id);
               }

               if(poster_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_DELETE ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               }
               else
               {
                  if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
                  {
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
                  }
               }
            }
            break;

         case CND_POSTER_ADDRESS:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
            {
               if(name = (UBYTE *)FirstSelected(CND_Gads[CND_POSTER_LV]))
               {
                  GetAttr(STRINGA_TextVal, CND_Gads[CND_POSTER_ADDRESS], (ULONG *)&str);
                  strcpy(uen->ue.ue_From, str);
                  ReplaceEntry(win, CND_Gads[CND_POSTER_LV], name, uen->ue.ue_From);
               }
            }
            break;

         case CND_POSTER_PASSWD:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Posters, poster_lv_id))
            {
               GetAttr(STRINGA_TextVal, CND_Gads[CND_POSTER_PASSWD], (ULONG *)&str);
               strcpy(uen->ue.ue_Key, str);
            }
            break;

         case CND_PENDING_LV:
            if(pending_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CND_Gads[CND_PENDING_LV], (ULONG *)&pending_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_PENDING_ADD:
            if(pending_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ue, 0, sizeof(struct UserEntry));
            AddUserToList(len->le.le_Pending, &ue);

            GetAttr(LISTV_NumEntries, CND_Gads[CND_PENDING_LV], (ULONG *)&pending_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
            {
               AddEntrySelect(win, CND_Gads[CND_PENDING_LV], &uen->ue.ue_From, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
            }
            break;

         case CND_PENDING_DELETE:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
            {
               RemoveSelected(win, CND_Gads[CND_PENDING_LV]);
               if(uen && ((uen->ue_Node.ln_Succ)->ln_Succ == NULL))
               {
                  RemoveNode(len->le.le_Pending, pending_lv_id);
                  pending_lv_id--;
               }
               else
               {
                  RemoveNode(len->le.le_Pending, pending_lv_id);
               }

               if(pending_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_DELETE ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               }
               else
               {
                  if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
                  {
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], win, NULL, STRINGA_TextVal, uen->ue.ue_Key , TAG_END);
                  }
               }
            }
            break;

         case CND_PENDING_ADDRESS:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
            {
               if(name = (UBYTE *)FirstSelected(CND_Gads[CND_PENDING_LV]))
               {
                  GetAttr(STRINGA_TextVal, CND_Gads[CND_PENDING_ADDRESS], (ULONG *)&str);
                  strcpy(uen->ue.ue_From, str);
                  ReplaceEntry(win, CND_Gads[CND_PENDING_LV], name, uen->ue.ue_From);
               }
            }
            break;

         case CND_PENDING_PASSWD:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Pending, pending_lv_id))
            {
               GetAttr(STRINGA_TextVal, CND_Gads[CND_PENDING_PASSWD], (ULONG *)&str);
               strcpy(uen->ue.ue_Key, str);
            }
            break;

         case CND_BANNED_LV:
            if(banned_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            GetAttr(LISTV_LastClickedNum, CND_Gads[CND_BANNED_LV], (ULONG *)&banned_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Banned, banned_lv_id))
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
            }
            break;

         case CND_BANNED_ADD:
            if(banned_lv_id == -1)
            {
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_DELETE ], win, NULL, GA_Disabled, FALSE, TAG_END);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, GA_Disabled, FALSE, TAG_END);
            }

            memset(&ue, 0, sizeof(struct UserEntry));
            AddUserToList(len->le.le_Banned, &ue);

            GetAttr(LISTV_NumEntries, CND_Gads[CND_BANNED_LV], (ULONG *)&banned_lv_id);

            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Banned, banned_lv_id))
            {
               AddEntrySelect(win, CND_Gads[CND_BANNED_LV], &uen->ue.ue_From, LVAP_TAIL);
               SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
            }
            break;

         case CND_BANNED_DELETE:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Banned, banned_lv_id))
            {
               RemoveSelected(win, CND_Gads[CND_BANNED_LV]);
               if(uen && ((uen->ue_Node.ln_Succ)->ln_Succ == NULL))
               {
                  RemoveNode(len->le.le_Banned, banned_lv_id);
                  banned_lv_id--;
               }
               else
               {
                  RemoveNode(len->le.le_Banned, banned_lv_id);
               }

               if(banned_lv_id == -1)
               {
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
                  SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_DELETE ], win, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
               }
               else
               {
                  if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Banned, banned_lv_id))
                  {
                     SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], win, NULL, STRINGA_TextVal, uen->ue.ue_From, TAG_END);
                  }
               }
            }
            break;

         case CND_BANNED_ADDRESS:
            if(uen = (struct UserEntryNode *)FindOrdinal(len->le.le_Banned, banned_lv_id))
            {
               if(name = (UBYTE *)FirstSelected(CND_Gads[CND_BANNED_LV]))
               {
                  GetAttr(STRINGA_TextVal, CND_Gads[CND_BANNED_ADDRESS], (ULONG *)&str);
                  strcpy(uen->ue.ue_From, str);
                  ReplaceEntry(win, CND_Gads[CND_BANNED_LV], name, uen->ue.ue_From);
               }
            }
            break;
      }
   }

   return (done);
}

struct Window *OpenCNDMainWin(struct List *list, Object **CND_MainWO, Object **CND_Gads)
{
   struct Window *win = NULL;

   static ULONG cyc2page[] = { MX_Active, PAGE_Active, TAG_END };

   static struct NewMenu CNDMenu[] =
   {
      Title("Project"),
       Item("Save"     , "S", CND_SAVE),
       Item("About..." , "?", CND_ABOUT),
       ItemBar,
       Item("Quit"     , "Q", CND_QUIT),
      End
   };

   static UBYTE *TabLabels[] = { "Lists"  , "Flags"  ,
                                 "Users"  , "Posters",
                                 "Pending", "Banned" ,
                                  NULL };

   Object *pages;

   *CND_MainWO = WindowObject,
      WINDOW_Title       , "CNdomo Config",
      WINDOW_ScreenTitle , "CNdomo Config",
      WINDOW_AutoAspect  , TRUE,
      WINDOW_AutoKeyLabel, TRUE,
      WINDOW_MenuStrip   , CNDMenu,
      WINDOW_SharedPort  , CNDMsgPort,
      WINDOW_ScaleWidth  , 30,
      WINDOW_MasterGroup,
         VGroupObject, NormalOffset, NormalSpacing,
            StartMember,
               CND_Gads[CND_TABS] = Tabs(NULL, TabLabels, 0, CND_TABS),
            EndMember,
            StartMember,
               pages = PageObject,
                  /*
                  ** Main Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        StartMember,
                           VGroupObject,
                              StartMember,
                                 CND_Gads[CND_LIST_LV] = ListviewObject,
                                    LAB_Label, "_Lists",
                                    LAB_Place, PLACE_ABOVE,
                                    GA_ID, CND_LIST_LV,
                                 EndObject,
                              EndMember,
                              StartMember,
                                 HGroupObject,
                                    StartMember,
                                       CND_Gads[CND_LIST_ADD] = PrefButton("_Add", CND_LIST_ADD),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_LIST_DELETE] = PrefButton("_Delete", CND_LIST_DELETE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              StartMember,
                                 CND_Gads[CND_LIST_NAME] = TabString("_Name:", "", 32 - 1, CND_LIST_NAME), FixMinHeight,
                              EndMember,
                           EndObject, Weight(30),
                        EndMember,
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("List Options"),
                                 TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_LIST_OWNER] = TabString("_Owner:", "", 128, CND_LIST_OWNER), FixMinHeight,
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_LIST_FROM] = TabString("_From:", "", 128, CND_LIST_FROM), FixMinHeight,
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_LIST_REPLYTO] = TabString("_Reply-to:", "", 128, CND_LIST_REPLYTO), FixMinHeight,
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_LIST_ERRORTO] = TabString("_Errors-to:", "", 128, CND_LIST_ERRORTO), FixMinHeight,
                                    EndMember,
                                    StartMember,
                                       HGroupObject, NormalSpacing,
                                          StartMember,
                                             VGroupObject,
                                                StartMember,
                                                   CND_Gads[CND_LIST_DIGESTSIZE] = TabInteger("Min. _Digest size:", 0, 11, CND_LIST_DIGESTSIZE),
                                                EndMember,
                                                StartMember,
                                                   CND_Gads[CND_LIST_DIGESTNUM] = TabInteger("Current digest #:", 0, 5, CND_LIST_DIGESTNUM),
                                                EndMember,
                                             EndObject,
                                          EndMember,
                                          StartMember,
                                             VGroupObject,
                                                StartMember,
                                                   CND_Gads[CND_LIST_MAXPOSTSIZE] = TabInteger("_Max. Article size:", 0, 11, CND_LIST_MAXPOSTSIZE),
                                                EndMember,
                                                StartMember,
                                                   CND_Gads[CND_LIST_ARTICLENUM] = TabInteger("Current post #:", 0, 7, CND_LIST_ARTICLENUM),
                                                EndMember,
                                             EndObject,
                                          EndMember,
                                       EndObject, FixMinHeight,
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_LIST_PASSWD] = TabString("_Password:", "", 16, CND_LIST_PASSWD), FixMinHeight,
                                    EndMember,
                                 EndObject,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                     EndObject,      
                  /*
                  ** Flags Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        VarSpace(DEFAULT_WEIGHT),
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("Subscription Flags"), NormalSpacing,
                                 BOffset(GRSPACE_NARROW), TOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_CLOSED] = PrefCheckBox("_Closed:", 0, CND_FLAGS_CLOSED),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_PRIVATE] = PrefCheckBox("_Private:", 0, CND_FLAGS_PRIVATE),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_MODERATED] = PrefCheckBox("_Moderated:", 0, CND_FLAGS_MODERATED),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_CONFIRM] = PrefCheckBox("_Require Confirmation:", 0, CND_FLAGS_CONFIRM),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("List Flags"), NormalSpacing,
                                 BOffset(GRSPACE_NARROW), TOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_HANDLEADMIN] = PrefCheckBox("_Handle Admin Requests", 0, CND_FLAGS_HANDLEADMIN),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_DIGEST] = PrefCheckBox("_Digest:", 0, CND_FLAGS_DIGEST),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_PREFIXLIST] = PrefCheckBox("Prefix Subject w/_Listname:", 0, CND_FLAGS_PREFIXLIST),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_SETLISTFROM] = PrefCheckBox("Set _From: to list:", 0, CND_FLAGS_SETLISTFROM),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_SENDSPOOL] = PrefCheckBox("_Immediate delivery:", 0, CND_FLAGS_SENDSPOOL),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_FLAGS_ARCHIVE] = PrefCheckBox("_Archive sent items:", 0, CND_FLAGS_ARCHIVE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                        VarSpace(DEFAULT_WEIGHT),
                     EndObject,
                  /*
                  ** Users Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        StartMember,
                           VGroupObject,
                              StartMember,
                                 CND_Gads[CND_USER_LV] = ListviewObject,
                                    LAB_Label, "_Users",
                                    LAB_Place, PLACE_ABOVE,
                                    GA_ID, CND_USER_LV,
                                 EndObject,
                              EndMember,
                              StartMember,
                                 HGroupObject,
                                    StartMember,
                                       CND_Gads[CND_USER_ADD] = PrefButton("_Add", CND_USER_ADD),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_USER_DELETE] = PrefButton("_Delete", CND_USER_DELETE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              StartMember,
                                 CND_Gads[CND_USER_ADDRESS] = TabString("_Email Address:", "", 128 - 1, CND_USER_ADDRESS), FixMinHeight,
                              EndMember,
                           EndObject, Weight(30),
                        EndMember,
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("User Options"),
                                 TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_USER_PASSWD] = TabString("_Password:", "", 16 - 1, CND_USER_PASSWD),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                     EndObject,
                  /*
                  ** Posters Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        StartMember,
                           VGroupObject,
                              StartMember,
                                 CND_Gads[CND_POSTER_LV] = ListviewObject,
                                    LAB_Label, "Post Allowed _Users",
                                    LAB_Place, PLACE_ABOVE,
                                    GA_ID, CND_POSTER_LV,
                                 EndObject,
                              EndMember,
                              StartMember,
                                 HGroupObject,
                                    StartMember,
                                       CND_Gads[CND_POSTER_ADD] = PrefButton("_Add", CND_POSTER_ADD),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_POSTER_DELETE] = PrefButton("_Delete", CND_POSTER_DELETE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              StartMember,
                                 CND_Gads[CND_POSTER_ADDRESS] = TabString("_Email Address:", "", 128 - 1, CND_POSTER_ADDRESS), FixMinHeight,
                              EndMember,
                           EndObject, Weight(30),
                        EndMember,
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("Poster Options"),
                                 TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_POSTER_PASSWD] = TabString("_Password:", "", 16 - 1, CND_POSTER_PASSWD),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                     EndObject,
                  /*
                  ** Pending Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        StartMember,
                           VGroupObject,
                              StartMember,
                                 CND_Gads[CND_PENDING_LV] = ListviewObject,
                                    LAB_Label, "Pending _Users",
                                    LAB_Place, PLACE_ABOVE,
                                    GA_ID, CND_PENDING_LV,
                                 EndObject,
                              EndMember,
                              StartMember,
                                 HGroupObject,
                                    StartMember,
                                       CND_Gads[CND_PENDING_ADD] = PrefButton("_Add", CND_PENDING_ADD),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_PENDING_DELETE] = PrefButton("_Delete", CND_PENDING_DELETE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              StartMember,
                                 CND_Gads[CND_PENDING_ADDRESS] = TabString("_Email Address:", "", 128 - 1, CND_PENDING_ADDRESS), FixMinHeight,
                              EndMember,
                           EndObject, Weight(30),
                        EndMember,
                        StartMember,
                           VGroupObject,
                              VarSpace(DEFAULT_WEIGHT),
                              StartMember,
                                 VGroupObject, DefaultFrame, FrameTitle("Pending User Options"),
                                 TOffset(GRSPACE_NARROW), BOffset(GRSPACE_NARROW), NormalHOffset,
                                    StartMember,
                                       CND_Gads[CND_PENDING_PASSWD] = TabString("_Password:", "", 16 - 1, CND_PENDING_PASSWD),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              VarSpace(DEFAULT_WEIGHT),
                           EndObject,
                        EndMember,
                     EndObject,
                  /*
                  ** Banned Page
                  */
                  PageMember,
                     HGroupObject, NormalSpacing,
                        VarSpace(DEFAULT_WEIGHT),
                        StartMember,
                           VGroupObject,
                              StartMember,
                                 CND_Gads[CND_BANNED_LV] = ListviewObject,
                                    LAB_Label, "Banned _Users",
                                    LAB_Place, PLACE_ABOVE,
                                    GA_ID, CND_BANNED_LV,
                                 EndObject,
                              EndMember,
                              StartMember,
                                 HGroupObject,
                                    StartMember,
                                       CND_Gads[CND_BANNED_ADD] = PrefButton("_Add", CND_BANNED_ADD),
                                    EndMember,
                                    StartMember,
                                       CND_Gads[CND_BANNED_DELETE] = PrefButton("_Delete", CND_BANNED_DELETE),
                                    EndMember,
                                 EndObject, FixMinHeight,
                              EndMember,
                              StartMember,
                                 CND_Gads[CND_BANNED_ADDRESS] = TabString("_Email Address:", "", 128 - 1, CND_BANNED_ADDRESS), FixMinHeight,
                              EndMember,
                           EndObject, Weight(80),
                        EndMember,
                        VarSpace(DEFAULT_WEIGHT),
                     EndObject,
               EndObject,
            EndMember,
            StartMember,
               HGroupObject,
                  StartMember,
                     CND_Gads[CND_SAVE] = PrefButton("_Save", CND_SAVE),
                  EndMember,
                  StartMember,
                     CND_Gads[CND_QUIT] = PrefButton("_Quit", CND_QUIT),
                  EndMember,
               EndObject, FixMinHeight,
            EndMember,
         EndObject,
   EndObject;

   if(*CND_MainWO)
   {
      AddMap(CND_Gads[CND_TABS], pages, cyc2page);
      
      DoMethod(*CND_MainWO, WM_TABCYCLE_ORDER,
         CND_Gads[CND_LIST_NAME       ],
         CND_Gads[CND_LIST_OWNER      ],
         CND_Gads[CND_LIST_FROM       ],
         CND_Gads[CND_LIST_REPLYTO    ],
         CND_Gads[CND_LIST_ERRORTO    ],
         CND_Gads[CND_LIST_DIGESTSIZE ],
         CND_Gads[CND_LIST_MAXPOSTSIZE],
         CND_Gads[CND_LIST_DIGESTNUM  ],
         CND_Gads[CND_LIST_ARTICLENUM ],
         CND_Gads[CND_LIST_PASSWD     ],
         NULL);

      DoMethod(*CND_MainWO, WM_TABCYCLE_ORDER,
         CND_Gads[CND_USER_ADDRESS],
         CND_Gads[CND_USER_PASSWD ],
         NULL);

      DoMethod(*CND_MainWO, WM_TABCYCLE_ORDER,
         CND_Gads[CND_POSTER_ADDRESS],
         CND_Gads[CND_POSTER_PASSWD ],
         NULL);

      DoMethod(*CND_MainWO, WM_TABCYCLE_ORDER,
         CND_Gads[CND_PENDING_ADDRESS],
         CND_Gads[CND_PENDING_PASSWD ],
         NULL);

      /*
      ** Disable primary List gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME  ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DELETE], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_FROM       ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_REPLYTO    ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ERRORTO    ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_PASSWD     ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ARTICLENUM ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTNUM  ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_MAXPOSTSIZE], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTSIZE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_OWNER      ], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      /*
      ** Build initial List data
      */
      if(list)
      {
         struct ListEntryNode *len = (struct ListEntryNode *)list->lh_Head;

         while(len && len->le_Node.ln_Succ)
         {
            AddEntry(NULL, CND_Gads[CND_LIST_LV], &len->le.le_Name, LVAP_TAIL);
            len = (struct ListEntryNode *)len->le_Node.ln_Succ;
         }
      }

      /*
      ** Disable Page gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], NULL, NULL,
         MX_DisableButton, 1L,
         MX_DisableButton, 2L,
         MX_DisableButton, 3L,
         MX_DisableButton, 4L,
         MX_DisableButton, 5L,
         TAG_END);

      /*
      ** Disable user List gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      /*
      ** Disable poster List gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      /*
      ** Disable pending List gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      /*
      ** Disable banned List gadgets
      */
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], NULL, NULL, GA_Disabled, TRUE, TAG_END);
      SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);

      win = WindowOpen(*CND_MainWO);
   }

   return (win);
}

void rethink_CND_window(struct Window *win, struct ListEntryNode *len, Object **CND_Gads)
{
   struct UserEntryNode *uen;

   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_NAME], win, NULL, STRINGA_TextVal, len->le.le_Name, TAG_END);

   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_FROM       ], win, NULL, STRINGA_TextVal, len->le.le_From       , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_REPLYTO    ], win, NULL, STRINGA_TextVal, len->le.le_ReplyTo    , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ERRORTO    ], win, NULL, STRINGA_TextVal, len->le.le_ErrorTo    , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_PASSWD     ], win, NULL, STRINGA_TextVal, len->le.le_Passwd     , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_DIGESTNUM  ], win, NULL, STRINGA_LongVal, len->le.le_DigestNum  , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_ARTICLENUM ], win, NULL, STRINGA_LongVal, len->le.le_Number     , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_MAXPOSTSIZE], win, NULL, STRINGA_LongVal, len->le.le_MaxPostSize, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_LIST_OWNER      ], win, NULL, STRINGA_TextVal, len->le.le_Owner      , TAG_END);

   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_TABS], win, NULL,
      MX_EnableButton , 1L,
      MX_EnableButton , 2L,
      (len->le.le_Flags & (LEF_MODERATED|LEF_CLOSED)) ? MX_EnableButton:MX_DisableButton, 3L,
      (len->le.le_Flags & LEF_CONFIRM) ? MX_EnableButton:MX_DisableButton, 4L,
      MX_EnableButton , 5L,
      TAG_END);

   /*
   ** Flags page
   */
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_CLOSED     ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_CLOSED     , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_PRIVATE    ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_PRIVATE    , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_MODERATED  ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_MODERATED  , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_CONFIRM    ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_CONFIRM    , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_HANDLEADMIN], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_HANDLEADMIN, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_DIGEST     ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_DIGEST     , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_PREFIXLIST ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_PREFIXLIST , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_SETLISTFROM], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_SETLISTFROM, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_SENDSPOOL  ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_SENDSPOOL  , TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_FLAGS_ARCHIVE    ], NULL, NULL, GA_Selected, len->le.le_Flags & LEF_ARCHIVE    , TAG_END);

   /*
   ** User related pages
   */
   user_lv_id    = -1;
   poster_lv_id  = -1;
   pending_lv_id = -1;
   banned_lv_id  = -1;

   ClearList(NULL, CND_Gads[CND_USER_LV]);
   if(uen = (struct UserEntryNode *)len->le.le_Users->lh_Head)
   {
      while(uen && uen->ue_Node.ln_Succ)
      {
         AddEntry(NULL, CND_Gads[CND_USER_LV], &uen->ue.ue_From, LVAP_TAIL);
         uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
      }
   }
   RefreshList(win, CND_Gads[CND_USER_LV]);

   ClearList(NULL, CND_Gads[CND_POSTER_LV]);
   if(uen = (struct UserEntryNode *)len->le.le_Posters->lh_Head)
   {
      while(uen && uen->ue_Node.ln_Succ)
      {
         AddEntry(NULL, CND_Gads[CND_POSTER_LV], &uen->ue.ue_From, LVAP_TAIL);
         uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
      }
   }
   RefreshList(win, CND_Gads[CND_POSTER_LV]);

   ClearList(NULL, CND_Gads[CND_PENDING_LV]);
   if(uen = (struct UserEntryNode *)len->le.le_Pending->lh_Head)
   {
      while(uen && uen->ue_Node.ln_Succ)
      {
         AddEntry(NULL, CND_Gads[CND_PENDING_LV], &uen->ue.ue_From, LVAP_TAIL);
         uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
      }
   }
   RefreshList(win, CND_Gads[CND_PENDING_LV]);

   ClearList(NULL, CND_Gads[CND_BANNED_LV]);
   if(uen = (struct UserEntryNode *)len->le.le_Banned->lh_Head)
   {
      while(uen && uen->ue_Node.ln_Succ)
      {
         AddEntry(NULL, CND_Gads[CND_BANNED_LV], &uen->ue.ue_From, LVAP_TAIL);
         uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
      }
   }
   RefreshList(win, CND_Gads[CND_BANNED_LV]);

   /*
   ** Disable user List gadgets
   */
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_ADDRESS], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_USER_PASSWD ], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

   /*
   ** Disable poster List gadgets
   */
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_ADDRESS], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_POSTER_PASSWD ], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

   /*
   ** Disable pending List gadgets
   */
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_ADDRESS], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_PENDING_PASSWD ], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);

   /*
   ** Disable banned List gadgets
   */
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_ADDRESS], NULL, NULL, GA_Disabled, TRUE, STRINGA_TextVal, "", TAG_END);
   SetGadgetAttrs((struct Gadget *)CND_Gads[CND_BANNED_DELETE ], NULL, NULL, GA_Disabled, TRUE, TAG_END);
}

BOOL load_config(struct List **list)
{
   BPTR lock;
   BPTR fp;

   struct ListEntry le;
   struct UserEntry ue;

   char buff [ 128 ] = "";

   if(*list = init_list())
   {
      if(FileExists(LISTDIR))
      {
         struct FileInfoBlock *fib;

         if(fib = AllocDosObject(DOS_FIB, NULL))
         {
            if(lock = Lock(LISTDIR, ACCESS_READ))
            {
               if(Examine(lock, fib))
               {
                  while(ExNext(lock, fib))
                  {
                     if(fib->fib_DirEntryType > 0)
                     {
                        sprintf(buff, LISTDIR"/%s/Config", fib->fib_FileName);

                        if(fp = Open(buff, MODE_OLDFILE))
                        {
                           Read(fp, &le, sizeof(struct ListEntry));
                           Close(fp);
                           strcpy(le.le_Name, fib->fib_FileName);  /* Set name */

                           if(le.le_Users = init_list())
                           {
                              sprintf(buff, LISTDIR"/%s/Users", le.le_Name);

                              if(fp = Open(buff, MODE_OLDFILE))
                              {
                                 while(FGets(fp, buff, sizeof(buff)))
                                 {
                                    if(parse_user_line(buff, &ue))
                                    {
                                       AddUserToList(le.le_Users, &ue);
                                    }
                                 }
                                 Close(fp);
                              }
                           }

                           if(le.le_Posters = init_list())
                           {
                              sprintf(buff, LISTDIR"/%s/AllowPost", le.le_Name);

                              if(fp = Open(buff, MODE_OLDFILE))
                              {
                                 while(FGets(fp, buff, sizeof(buff)))
                                 {
                                    if(parse_user_line(buff, &ue))
                                    {
                                       AddUserToList(le.le_Posters, &ue);
                                    }
                                 }
                                 Close(fp);
                              }
                           }

                           if(le.le_Pending = init_list())
                           {
                              sprintf(buff, LISTDIR"/%s/Pending", le.le_Name);

                              if(fp = Open(buff, MODE_OLDFILE))
                              {
                                 while(FGets(fp, buff, sizeof(buff)))
                                 {
                                    if(parse_user_line(buff, &ue))
                                    {
                                       AddUserToList(le.le_Pending, &ue);
                                    }
                                 }
                                 Close(fp);
                              }
                           }

                           if(le.le_Banned = init_list())
                           {
                              sprintf(buff, LISTDIR"/%s/Banned", le.le_Name);

                              if(fp = Open(buff, MODE_OLDFILE))
                              {
                                 while(FGets(fp, buff, sizeof(buff)))
                                 {
                                    if(parse_user_line(buff, &ue))
                                    {
                                       AddUserToList(le.le_Banned, &ue);
                                    }
                                 }
                                 Close(fp);
                              }
                           }

                           AddListToList(*list, &le);
                        }
                     }
                  }
               }
               UnLock(lock);
            }
            FreeDosObject(DOS_FIB, fib);
         }
      }
      else
      {
         if(lock = CreateDir(MAINDIR)) UnLock(lock);
         if(lock = CreateDir(LISTDIR)) UnLock(lock);
      }
   }
   else
   {
      return FALSE;
   }
   return TRUE;
}

void AddListToList(struct List *list, struct ListEntry *le)
{
   struct ListEntryNode *len;

   if(len = (struct ListEntryNode *)AllocVec(sizeof(struct ListEntryNode), MEMF_CLEAR))
   {
      CopyMem(le, (APTR)&len->le, sizeof(struct ListEntry));
      len->le_Node.ln_Name = len->le.le_Name;
      AddTail((struct List *)list, (struct Node *)len);
   }
}

void AddUserToList(struct List *list, struct UserEntry *ue)
{
   struct UserEntryNode *uen;

   if(uen = (struct UserEntryNode *)AllocVec(sizeof(struct UserEntryNode), MEMF_CLEAR))
   {
      CopyMem(ue, (APTR)&uen->ue, sizeof(struct UserEntry));
      uen->ue_Node.ln_Name = uen->ue.ue_From;
      AddTail((struct List *)list, (struct Node *)uen);
   }
}

void free_config(struct List **list)
{
   if(list && *list)
   {
      struct ListEntryNode *len = (struct ListEntryNode *)(*list)->lh_Head;

      while(len && len->le_Node.ln_Succ)
      {
         if(len->le.le_Users  ) free_list(&len->le.le_Users);
         if(len->le.le_Posters) free_list(&len->le.le_Posters);
         if(len->le.le_Pending) free_list(&len->le.le_Pending);
         if(len->le.le_Banned ) free_list(&len->le.le_Banned);
         len = (struct ListEntryNode *)len->le_Node.ln_Succ;
      }
      free_list(list);
   }
}

void save_config(struct List *list)
{
   BPTR fp;

   char buff [ 256 ] = "";

   struct UserEntryNode *uen;
   struct ListEntryNode *len = (struct ListEntryNode *)list->lh_Head;

   while(len && len->le_Node.ln_Succ)
   {
      sprintf(buff, LISTDIR"/%s/Config", len->le.le_Name);

      if(fp = Open(buff, MODE_NEWFILE))
      {
         Write(fp, &len->le, sizeof(struct ListEntry));
         Close(fp);

         if(len->le.le_Users)
         {
            uen = (struct UserEntryNode *)len->le.le_Users->lh_Head;

            sprintf(buff, LISTDIR"/%s/Users", len->le.le_Name);

            if(IsListEmpty(len->le.le_Users))
            {
               DeleteFile(buff);
            }
            else if(fp = Open(buff, MODE_NEWFILE))
            {
               while(uen && uen->ue_Node.ln_Succ)
               {
                  FPrintf(fp, "%s:%s\n", uen->ue.ue_From, uen->ue.ue_Key);
                  uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
               }
               Close(fp);
            }
         }

         if(len->le.le_Posters)
         {
            uen = (struct UserEntryNode *)len->le.le_Posters->lh_Head;

            sprintf(buff, LISTDIR"/%s/AllowPost", len->le.le_Name);

            if(IsListEmpty(len->le.le_Posters))
            {
               DeleteFile(buff);
            }
            else if(fp = Open(buff, MODE_NEWFILE))
            {
               while(uen && uen->ue_Node.ln_Succ)
               {
                  FPrintf(fp, "%s:%s\n", uen->ue.ue_From, uen->ue.ue_Key);
                  uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
               }
               Close(fp);
            }
         }

         if(len->le.le_Pending)
         {
            uen = (struct UserEntryNode *)len->le.le_Pending->lh_Head;

            sprintf(buff, LISTDIR"/%s/Pending", len->le.le_Name);

            if(IsListEmpty(len->le.le_Pending))
            {
               DeleteFile(buff);
            }
            else if(fp = Open(buff, MODE_NEWFILE))
            {
               while(uen && uen->ue_Node.ln_Succ)
               {
                  FPrintf(fp, "%s:%s\n", uen->ue.ue_From, uen->ue.ue_Key);
                  uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
               }
               Close(fp);
            }
         }

         if(len->le.le_Banned)
         {
            uen = (struct UserEntryNode *)len->le.le_Banned->lh_Head;

            sprintf(buff, LISTDIR"/%s/Banned", len->le.le_Name);

            if(IsListEmpty(len->le.le_Banned))
            {
               DeleteFile(buff);
            }
            else if(fp = Open(buff, MODE_NEWFILE))
            {
               while(uen && uen->ue_Node.ln_Succ)
               {
                  FPrintf(fp, "%s:%s\n", uen->ue.ue_From, uen->ue.ue_Key);
                  uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ;
               }
               Close(fp);
            }
         }
      }

      len = (struct ListEntryNode *)len->le_Node.ln_Succ;
   }
}

BOOL parse_user_line(char *line, struct UserEntry *ue)
{
   char *tmp;

   if(!*line)
      return FALSE;

   memset(ue, 0, sizeof(struct UserEntry));

   line[strlen(line) - 1] = '\0';

   if(tmp = strchr(line, ':'))
   {
      *tmp = '\0';
      tmp++;
      strncpy(ue->ue_From, line, sizeof(ue->ue_From) - 1);
   }

   if(*tmp)
   {
      strncpy(ue->ue_Key, tmp, sizeof(ue->ue_Key) - 1);
   }

   return TRUE;
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
