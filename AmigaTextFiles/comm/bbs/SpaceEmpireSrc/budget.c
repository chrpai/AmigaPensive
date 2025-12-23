#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "space.h"
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "dos.h"
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include "extra.h"
#include "io.h"
extern struct Player *gpn;  /* global pointer to current player */
extern struct Team *team;
extern struct Player *player;
extern int pn;
extern int cov_work;          /* 1 = covert agents not paid enough */
extern int cov_eff;           /* efficiency based on amount of payment */
extern int sol_work;          /* 1 = soldiers not paid enough */
extern int sol_eff;           /* efficiency based on amount paid */
void news ( int to , int from , int msg );
void dissolve_team ( int z );
void savefiles ( void );
void scores ( void );
long Do_Forced_Sales(char *,long *,long *,long *,long ,long);
void Bankrupcy(long);

struct Budget
  {
  char name[28];   /* budget item name */
  long  need;      /* amount required */
  long  budg;      /* amount given */
  long hflag;      /* TRUE: this item affects harmony, FALSE: no affect */
  long mflag;      /* TRUE: this item affects morale, FALSE: no affect */

  };
struct Prices pitem[] =
  {
    { '1', "Soldier Troop.......",  250, 100 },
    { '2', "Fighter craft.......", 1800, 900 },
    { '3', "Defense station.....", 2000, 900 },
    { '4', "Heavy cruiser.......", 4000,1800 },
    { '5', "Intelligence Agent..", 5000,   0 },
    { '6', "Carrier.............", 8000,3000 },
    { '7', "Generic planet......",    0,   0 },
    { '8', "Governing planet....",    0,   0 },
  };

#define MAXBUD 9
struct Budget  bmoney[MAXBUD] =
  {
    { "Planets                    ", 0,0, TRUE,FALSE },
    { "Armed Forces               ", 0,0,FALSE, TRUE },
    { "Intelligence Agents        ", 0,0,FALSE,FALSE },
    { "Medical Research           ", 0,0, TRUE,FALSE },
    { "Plague Research            ", 0,0, TRUE, TRUE },
    { "Food for Military          ", 0,0,FALSE, TRUE },
    { "Food for People            ", 0,0, TRUE,FALSE },
    { "Food to Sell               ", 0,0,FALSE,FALSE },
    { "Food to Buy                ", 0,0,FALSE,FALSE }
  };

void ruling_stage(void);
void ruling_stage()
  {
  int  foodb,foods;  /* food buy and sell prices */
  long money;
  long m2;
  long m3, h1,h6,h7;
  int i,j;
  int tamt,amt,per,bamt,namt,famt;
  register struct Budget *bptr;
  char      cper='%';

  printf("\n");
  for(i=0;i<MAXBUD;i++)bmoney[i].budg = 0;
  /* planets */
  money  =  gpn->orep     * (  3100 - rnd(50)  );
  money +=  gpn->foodp    * (  1700 - rnd(200) );
  money +=  gpn->defensep * ( 24000 - rnd(500) );
  money +=  gpn->governp  * ( 32000 - rnd(500) );
  bmoney[0].need = money;
  /* military */
  money  = ( gpn->cruisers + gpn->team_cruisers + gpn->C_used ) * ( 153 - rnd(4) );
  money += ( gpn->defense  + gpn->team_defense )                * (  78 - rnd(5) );
  money += ( gpn->soldiers + gpn->team_soldiers + gpn->S_used ) * (  30 - rnd(3) );
  money += ( gpn->fighters + gpn->F_used )                      * (  88 - rnd(5) );
  money += ( gpn->carriers )                                    * (  90 - rnd(4) );
  bmoney[1].need = money;
  /* intellegence */
  money =  ( gpn->agents + gpn->I_used ) * ( 85 - rnd(10) );
  bmoney[2].need = money;
  /* medical research  */
  money = (gpn->population/10) * 15;
  bmoney[3].need = money;
  /* plague research */
  if( gpn->plague > 0 )
    {
    money =  gpn->population * 36;

    }
  else money = 0;
  bmoney[4].need = money;
  /* adjust for credits and set initial budget */
  tamt = gpn->credits;
  bamt = 0;
  for(i=0;i<5;i++)
    {
    if( gpn->protection <=0 )
      {
      bmoney[i].need += (int)( (float)bmoney[i].need * 0.15);

      }
    money = bmoney[i].need;
    if( money > tamt) money = tamt;
    tamt -= money;
    bmoney[i].budg = money;
    bamt += money;

    };
  /*  Do the Food allocation */
  /* people */
  bmoney[6].need = gpn->population *11;
  bmoney[6].budg = bmoney[6].need;
  famt = bmoney[6].need;
  /* military */
  bmoney[5].need = (gpn->soldiers + gpn->team_soldiers + gpn->S_used) * 10;
  bmoney[5].need+= gpn->cruisers + gpn->team_cruisers + gpn->C_used;
  bmoney[5].budg = bmoney[5].need;
  famt += bmoney[5].need;
  foods = (foodb = player[0].foodp)/2;
  bmoney[7].need = bmoney[7].budg = 0;
  bmoney[8].need = bmoney[8].budg = 0;
  if( famt > gpn->food )
    {
    bmoney[8].need = famt - gpn->food;
    printf("You will need to buy additional food\n");
    namt = gpn->credits - bamt;
    if( namt/foods -bmoney[8].need < 0 )
      {
      bmoney[8].need = namt/foods;
      bamt += namt;
      printf("You do not have enought money to buy the needed food\n");

      };

    };
  /* */
  /* Main Loop: Recompute everything */
  /* */
  do  /* until user is satified with the budget */
    {
    printf("  Budget for                              buy  sell Credits:%d\n"
    ,gpn->credits - bamt);
    printf("  %-25s food price: %3d   %3d food reserve:%d\n"
    ,gpn->pname,foodb,foods,gpn->food - famt);
    bamt = namt = tamt = 0;
    printf("  +-Budget item----------------+--Needed--+--Budget--+-Results----------\n");
    for(i=0; i<MAXBUD;i++)
      {
      /* make adjustments */
      bptr = &bmoney[i];
      if( i < 5)
        {
        namt += bptr->need;
        bamt += bptr->budg;

        };
      printf("%2d:%-28s|%10d|%10d|",i+1,bptr->name,bptr->need,bptr->budg);
      if( bptr->need > 0 )
        {
        amt = bptr->budg - bptr->need;
        if( bptr->need < 1000 )
          {
          per = (bptr->budg* 100) / (bptr->need);

          }
        else   per = bptr->budg / (bptr->need/100);
        if( amt >= 0 )
          {
          printf(" %10d  %3d%c \n",amt,per,cper);

          }
        else
          {
          printf("(%10d)(%3d%c)\n",-amt,per,cper);

          };

        }
      else printf(" ok\n");

      };
    printf("  +----------------------------+----------+----------+------------------\n");
    printf("   %-28s|%10d|%10d|","Totals",namt,bamt);
    amt = bamt - namt;
    if( namt > 0 )
      {
      if( namt < 1000 )
        {
        per = (bamt*100) / namt;

        }
      else  per = bamt / (namt/100);
      if( amt >= 0 )
        {
        printf(" %10d  %3d%c \n",amt,per,cper);

        }
      else if( amt < 0 )
        {
        printf("(%10d)  %3d%c\n",-amt,per,cper);

        };

      }
    else printf(" ok\n");
    j = Request_Data("Enter Budget Item to adjust",0,MAXBUD);
    switch (j)
      {
      case 0:  /* finish the budget up, apply the data if ok */
      bamt = 0;
      for(j=i=0;i<MAXBUD;i++)
        {
        bamt += bmoney[i].budg;
        if(bmoney[i].need > bmoney[i].budg)
          {
          j++;
          printf("Warning, %s is under budgeted\n",bmoney[i].name);

          };

        };
      if( bamt <= gpn->credits )
        {
        if( j > 0 )
          {
          if( boolean("Warning, You have negative budget items, are you sure?",2)==1)j=0;

          };

        }
      else
        {
        printf("You do not have that many credits.  There is no deficit spending!\n");
        if( boolean("Do you wish to file for bankrupcy?",2)==1)
          {
          Bankrupcy(bamt);
          j = 0;
          }
        else j = 1;
        };
      if( gpn->food < famt )
        {
        printf("You do not have enough food to feed your people.  Re-do your Food Buget!\n");
        j = 1;

        };
      break;
      case 6:  /* Military Food */
      case 7:  /* People Food */
      j--;
      famt -= bmoney[j].budg;
      namt  = bmoney[j].need;
      bmoney[j].budg = 0;
      if( namt > (gpn->food - famt) )
        {
        namt = gpn->food - famt;
        printf("Warning: You do not have enough food!\n");
        printf("you will have to reduce some other budget item to get more\n");
        printf("or buy additional food to balance your budget\n");

        };
      i = Request_Data(bmoney[j].name,namt,gpn->food - famt);
      bmoney[j].budg = i;
      famt += i;
      break;
      case 8:  /* Sell food */
      j--;
      m2 = bmoney[j].budg;
      famt -= m2;
      bmoney[j].budg = 0;
      m3    = player[0].foodp/2;
      bamt += m2 * m3;
      namt  = gpn->food - famt;
      if( namt <= 0 )
        {
        printf("Warning: You do not have enough food to sell!\n");
        printf("you will have to reduce some other budget item to get more\n");
        printf("or buy additional food to balance your budget\n");

        }
      else
        {
        i = Request_Data(bmoney[j].name,0,namt);
        bmoney[j].budg = i;
        famt += i;
        bamt -= i*m3;

        }
      break;
      case 9:  /* buy food */
      j--;
      m2 = bmoney[j].budg;
      famt += m2;                    /* reduce the food available */
      bmoney[j].budg = 0;
      m3    = player[0].foodp;
      bamt -= m2 * m3;   /* increase the cash reserve */
      namt  = (gpn->credits - bamt)/m3;
      i = Request_Data(bmoney[j].name,0,namt);
      bmoney[j].budg = i;
      famt -= i;
      bamt += i*m3;
      break;
      default:
      j--;
      bamt -= bmoney[j].budg;
      bmoney[j].budg = 0;
      amt = gpn->credits - bamt;   /* amount you have left in your budget */
      if( amt < bmoney[j].need)
        {
        printf("Warning: You do not have enough credits for this item!\n");
        printf("you will have to reduce some other budget item to get more credits\n");
        namt = amt;

        }
      else
        {
        namt = bmoney[j].need;

        };
      i = Request_Data(bmoney[j].name,namt,amt);
      bamt += (bmoney[j].budg = i);
      j = 1;

      };

    }
  while (j != 0 );
  amt = 0;
  for( j=0; j<MAXBUD;j++)
    {
    if(j < 7 )
      {
      i = Adjust_Factor(bmoney[j].budg, bmoney[j].need);
      if( bmoney[j].hflag) gpn->harmony += i;
      if( bmoney[j].mflag) gpn->morale  += i;
      amt += i;
      if( j < 5 )gpn->credits -= bmoney[j].budg;
      };
    switch (j)
      {
      case 1: if( i < 0 ) sol_work = 1;sol_eff = i;break;
      case 2: if( i < 0 ) cov_work = 1;cov_eff = i;break;
      case 3: if( gpn->plague <= 0 && i > 0)gpn->plague -= i; break;
      case 4:
      if( gpn->plague > 0 )
        {
        if( i <= 0 )
          {
          printf("Doctors complain about the lack of research funds\n");
          news(pn,pn,16);

          }
        else
          {
          printf("Doctors promise a cure soon...\n");
          news(pn,pn,17);

          };
        namt = (gpn->population * gpn->plague)/100 + rnd(10) + 1;
        printf("government census shows that %d people died from the plague\n",namt);
        gpn->population -= namt;
        namt = (gpn->soldiers * gpn->plague)/100 + rnd(10) + 1;
        if( namt > gpn->soldiers)namt =gpn->soldiers;
        gpn->soldiers -= namt;
        printf("government census shows that %d soldiers died from the plague\n",namt);
        if( gpn->soldiers == 0)gpn->morale = 0;
        if( i <= 0 )i = 1;  /* be nice and always cure part of it */
        gpn->plague -= i;
        if( gpn->population <= 0 )
          {
          printf("You have become very ill, you call for help, but nobody\n");
          printf("comes.  It seems that it was a bad choice to cut the medical\n");
          printf("budget...  You population has been decimated by the plague\n");
          news(pn,pn,19);
          if(gpn->team_num ==  pn)
            {
            dissolve_team(gpn->team_num);

            };
          strcpy(gpn->name,"X");
          strcpy(gpn->pname,"X");
          scores();
          savefiles();
          end();

          };
        if( gpn->plague < 0 )
          {
          printf("Good News!  Doctors found a cure for the plague!\n");
          gpn->harmony +=  1;
          gpn->morale  +=  1;
          news(pn,pn,18);

          };

        };
      case 7:
      m3 = player[0].foodp/2;
      gpn->credits += bmoney[j].budg*m3;
      case 5: /* military food */
      h6 = i;
      case 6: /* people food */
      gpn->food -= bmoney[j].budg;
      break;
      case 8:
      m3 = player[0].foodp;
      gpn->credits -= bmoney[j].budg*m3;
      gpn->food    += bmoney[j].budg;
      break;

      };

    };
  if( gpn->food < 0 )
    {
    gpn->food = 0;
    h6 -= 2;

    };
  if( gpn->credits < 0 )
    {
    h6 -= 2;
    gpn->credits = 0;

    };
  h1 =  amt;
  if( h6 < 0 )
    {
    h7 = rnd(1-h6);
    m2 = (gpn->soldiers * h7 +99)/100;
    printf(" %d mistreated troops desert your army\n",m2);
    gpn->soldiers -= m2;
    if( gpn->soldiers < 0)
      {
      gpn->team_soldiers +=gpn->soldiers;
      gpn->soldiers  = 0;
      if( gpn->team_soldiers < 0 )
        {
        gpn->S_used += gpn->team_soldiers;
        gpn->team_soldiers = 0;
        if( gpn->S_used < 0 )gpn->S_used = 0;

        };

      };

    };
  h7 = ( h1 +  h6 ) * 18 ;
  printf(" The pols show ");
  if( h7 >= 75 )
    {
    gpn->morale  += 1;
    gpn->harmony += 1;
    printf("an upward");

    }
  else
    {
    gpn->morale  -= 1;
    gpn->harmony -= 1;
    printf("a downward");

    };
  printf(" trend in your popularity as a ruler...\n");

  }
void Bankrupcy(need)
long need;                /* amount of cash to generate */
  {
  short value=4;
  long amt;
  printf("Hear ye. Hear ye. First Galactic Bankrupcy court is in session!\n");
  printf("His Honorable Judge Ebenezer Scrooge presiding!\n");
  printf("Rumor has it that one of the ten judges is crooked!\n");
  if( boolean("Do you want to attempt to bribe the judge?\n",2)==1)
    {
    need += 10000;
    printf("You slip the judge a bribe in the court snack bar\n");
    printf("The bribe of 10,000 credits is added to your debt\n");
    if( rnd(100) < 10 )
      {
      value = 1;

      }
    else
      {
      printf("Opps... A sting!  The judge gloats over you as the galactic agents read you\n");
      printf("your rights.  He also says that you will pay dearly for the insult!\n");
      value = 8;

      };

    };
  printf("The judge rules that you should pay %d cents on the dollar!",value*10);
  printf("Items in your inventory will be sold for you:\n");
  gpn->credits += need;
  need = need *10/value;
  amt  = 0;
  need -= Do_Forced_Sales("Soldiers"        ,&gpn->soldiers,&gpn->S_used,&gpn->team_soldiers,need,pitem[0].sprice);
  need -= Do_Forced_Sales("fighters"        ,&gpn->fighters,&gpn->F_used,&amt               ,need,pitem[1].sprice);
  need -= Do_Forced_Sales("Defense stations",&gpn->defense ,&amt        ,&gpn->team_defense ,need,pitem[2].sprice);
  need -= Do_Forced_Sales("Heavy cruisers"  ,&gpn->cruisers,&gpn->C_used,&gpn->team_cruisers,need,pitem[3].sprice);
  need -= Do_Forced_Sales("Carriers"        ,&gpn->carriers,&amt        ,&amt               ,need,pitem[4].sprice);
  if( need > 0 )
    {
    printf("The Judge has ruled that you are so incompetent that you should\n");
    printf("be removed from office.  However, he realizes that he cannot do that\n");
    printf("and reduces your debt to your assets.  Morale is at an all time low for your country\n");
    gpn->morale -= ( need>>4);
    };
  gpn->harmony = 0;
  gpn->morale -= 20;
  printf("The population is very unhappy with your management!\n");
  }
long Do_Forced_Sales(str,item,used,ally,Amt,price)
long *item,*used,*ally,Amt,price;
char *str;
  {
  int total,cash;
  if( Amt <= 0 )return 0;
  total = *item + *used + *ally;
  if( total <= 0)
    {
    cash = *item =  *used =  *ally = 0;
    printf(" Not enough %s, no funds generated\n",str);

    }
  else
    {
    cash  = *item * price;
    cash += *used * price;
    cash += *ally * price;
    printf(" The liquidated your %s and generated %d cash\n",str,cash);

    };
  return cash;

  }
