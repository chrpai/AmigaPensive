
#ifndef CNET_VDE
#define CNET_VDE

struct VDEentry {
   char  text[48];

   short xpos,      /* where to print these things */
         ypos;

   long  min,       /* for numerix */
         max;       /* for numerix */

   short length,    /* for string entry, field size */
                    /* for bit boolean, which bit? */

           type; /*  0 = UBYTE 0/1
                     1 = ULONG BIT 0/1
                     2 = Text (char *)
                     3 = BYTE numeric
                     4 = short numeric
                     5 = USHORT numeric
                     6 = long numeric
                     7 = date
                     8 = flags
                     9 = ordered BBSTEXT list (UBYTE)
                     10= ordered BBSMENU list (UBYTE)
                     11= char **
                     12= UBYTE BIT 0/1 */

   long  offset;      /* position in structure */

   short screen0,   /* belongs-to screen */
         screen2;   /* goes-to screen, -1 for exit */

   short goup,      /* VDEentry for movement */
         godown,
         goleft,
         goright;

   UWORD ghost;
};


#endif
