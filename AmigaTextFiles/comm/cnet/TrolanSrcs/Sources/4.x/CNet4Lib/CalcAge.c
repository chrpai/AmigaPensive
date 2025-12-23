#include "cnet4lib.h"

int CalcAge(struct UserData *udata)
{
   int age;

   struct IsDate id;

   memset(&id, 0, sizeof(struct IsDate));

   CNetExplodeTime(CNetTime(), &id);

   age = id.Year - udata->Birthdate.Year;

   if(id.Month == udata->Birthdate.Month) /* age */
   {
      if(id.Date < udata->Birthdate.Date)
      {
         age--;
      }
   }
   else if(id.Month < udata->Birthdate.Month)
   {
      age--;
   }

   return (age);
}
