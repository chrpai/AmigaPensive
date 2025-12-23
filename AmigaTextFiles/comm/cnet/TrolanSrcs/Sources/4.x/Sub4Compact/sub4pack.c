/*
** Subboards4 Compressor
** by Kelly Cochran
** Wed Jul 30 17:21:24 1997
*/

#define SUB4_KILLED   0x80

struct KilledSub
{
   struct Node ks_Node;
   char Name [ 128 ];
   int  Subboard;
   int  Serial;
};

char *version = "\0$VER: 0.1 Sub4Pack";

void AddToList(struct List *list, char *name, int phys, int serial);
struct List *init_list(void);
void free_list(struct List *list);
void PrintKilled(struct List *list);

void main(void)
{
   BPTR fp;

   struct MainPort      *myp;
   struct SubboardType4  sub;
   struct List          *list;
   int i = 0,
       x = 0,
       y = 1;

   Forbid();
   if(myp = (struct MainPort *)FindPort("cnetport"))
   {
      Permit();
      if(fp = Open("SysData:subboards4", MODE_OLDFILE))
      {
         Printf("Checking for deleted subboards...\n");
         Flush(Output());

         if(list = init_list())
         {
            while(Read(fp, &sub, sizeof(struct SubboardType4)) == sizeof(struct SubboardType4))
            {
               if(sub.Marker & SUB4_KILLED)
               {
                  Printf("*");
                  AddToList(list, (char *)sub.ZeroPath, i, y++);
                  x++;
               }
               else
               {
                  Printf(".");
               }
               Flush(Output());
               i++;
            }
            Printf("\n%ld deleted subboard%s found.\n", x, x ? "":"s");
            if(!IsListEmpty(list))
            {
               PrintKilled(list);
            }
            Close(fp);

            free_list(list);
         }
      }
      else
      {
         Printf("SysData:subboards4 not found.\n");
      }
   }
   else
   {
      Permit();
   }
}

struct List *init_list(void)
{
   struct List *work;

   if(work = AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(work);
   }

   return (work);
}

void AddToList(struct List *list, char *name, int phys, int serial)
{
   struct KilledSub *work;

   if(work = AllocVec(sizeof(struct KilledSub), MEMF_CLEAR))
   {
      work->Subboard        = phys;
      strcpy(work->Name, name);
      work->ks_Node.ln_Name = work->Name;
      work->Serial          = serial;
      AddTail(list, (struct Node *)work);
   }
}

void free_list(struct List *list)
{
   struct Node *work;
   struct Node *temp;

   work = list->lh_Head;

   while(work && work->ln_Succ)
   {
      temp = work->ln_Succ;
      FreeVec(work);
      work = temp;
   }

   FreeVec(list);
}

void PrintKilled(struct List *list)
{
   struct KilledSub *work;
   struct KilledSub *temp;

   work = (struct KilledSub *)list->lh_Head;

   Printf("\nKilled Subboards:\n");

   while(work && work->ks_Node.ln_Succ)
   {
      temp = (struct KilledSub *)work->ks_Node.ln_Succ;
      Printf("[%3ld] [%3ld] %s\n", work->Subboard, work->Serial, work->Name);
      work = temp;
   }
   Printf("\n");
}
