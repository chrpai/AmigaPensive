#ifndef CNET_CHARGES
#define CNET_CHARGES

struct   ChargeType
   {
   struct   ChargeType  *next;

   short                id;

   short                schedule;
   short                type;
   short                units;

   ULONG                fcredits;
   ULONG                bcredits;
   };

struct   ChargeSet
   {
   struct   ChargeSet   *next;
   short                schedule;
   short                ACC[32][80];
   };

#endif
