
extern struct Library *CNetBase;

BOOL IsSubOp(struct UserData *udata, struct SubboardType4 *sub)
{
   int x,
       acc = IDToAccount(udata->IDNumber);

   for(x = 0; x < 6; x++)
   {
      if((sub->SubOpAccs[x] == acc) &&
         (sub->SubOpIDs [x] == udata->IDNumber))
      {
         return TRUE;
      }
   }

   return FALSE;
}
