/* Additions to Space Empire, Including Chemical and Nuclear Weapons */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
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
#include "space.h"
#include "extra.h"
#include "io.h"
void Missile_Attack(int,int,char *,int,int,int,int,int,int,int);
void Weapons( void );
void Terminal ( char *string , int size );
void strip_crlf ( char *string );
void Chemical_Fire( int );
void Nuclear_Fire( int );
void Convent_Fire( int );
int getplayer ( int yourteam );
int boolean ( char astring [], int yndef );
void player_msg ( int to , int from , int msg , char string []);
void news ( int to , int from , int msg );
extern long DOSBase;
#define MAXANSI 7
extern char *ansii[MAXANSI];
extern struct Player *gpn;  /* global pointer to current player */
extern struct Player *player;
int fired;
extern int pn;

void Weapons()
  {
  int num,c1,c2,c3;
  gpn = &player[pn];
  fired = 0;
  num = gpn->score;
  if( num < 1)num = 10;
  c1 = 4*num+ rnd(1000);
  c2 = 5*num+ rnd(1000);
  c3 = 8*num+ rnd(1000);
  while( num != 0 && fired != 1)
    {
    printf("\nSpecial Weapons Menu                Cost\n");
    printf("--------------------------------------------\n");
    printf("0. Exit                               Free\n");
    printf("1. %sChemical Warfare                   %s%d%s\n",ansii[2],ansii[3],c1,ansii[0]);
    printf("2. %sConventional Missile Strike        %s%d%s\n",ansii[2],ansii[3],c3,ansii[0]);
    printf("3. %sThermonuclear Missile Strike       %s%d%s\n",ansii[2],ansii[3],c2,ansii[0]);
    printf("--------------------------------------------\n");
    num = Request_Data("Enter choice(0 to exit)",0,3);
    switch( num )
      {
      case 0: break;
      case 1: Chemical_Fire(c1); break;
      case 3: Nuclear_Fire(c2);  break;
      case 2: Convent_Fire(c3);  break;
      default: printf("Invalid option, try again...\n");
      }
    }
  }
void Chemical_Fire(cost)
int cost;
  {
  int z,defplan,i;
  char zstring[80];
  struct Player *px;
  gpn = &player[pn];
  if( ! (cost > gpn->credits) )
    {
    reget:
    if((z =  getplayer(2))!=  -1)
      {
      px = &player[z];
      if(px->protection > 0)
        {
        printf("\nThat player is still under protection.  Attack someone else.\n");
        goto reget;
        };

      sprintf(zstring,"\nAre you sure you want to attack %s?",px->pname);
      if( boolean(zstring,1) == 2)goto reget;
      fired = 1;
      gpn->protection =    0;
      gpn->credits   -= cost;
      printf("Launching a chemical warheaded missile.....\n"); Delay(75);
      printf("Computing best location to hit.....\n"); Delay(125);
      printf("Chemical Missile's Target is Locked into the Computer...\n"); Delay(75);
      printf("Missile impact in:    ");  Delay(25);
      printf("10"); Delay(25);
      printf("\b\b  ");
      for(i=9;i > 0;--i)
        {
        printf("\b%d",i);
        Delay(20);
        }
      defplan = (px->defense*10) / (px->orep + px->foodp + px->defensep + px->governp + 1);
      if(defplan > 900)defplan = 900;
      if( rnd(1000) > defplan)
        {
        int pop;
        printf("\n Missile has succesfully impacted...\n");
        printf("                    Population   Food Supply \n");
        printf("Damages inflicted:        %d             %d\n", (px->population/10+1), (px->food/10+1));
        px->population -= ( pop = (px->population/10 + 1));
        px->food       -= px->food/10 + 1;
        px->plague += 1;
        Missile_Attack(pn,z,"Chemical",0,0,0,0,pop,0,0);

        }
      else
        {
        printf("\nSorry missile was intercepted... No damages inflicted\n");
        }
      player_msg(z,pn, 14, "Missile Launched");
      news(z,pn, 22);
      }

    }
  else
    {
    printf("You do not have enough credits......\n");
    }
  }

void Nuclear_Fire(cost)
int cost;
  {
  int z,defplan,i;
  char zstring[80];
  struct Player *px;
  gpn = &player[pn];
  if( ! ( cost > gpn->credits) )
    {
    reget:
    if((z =  getplayer(2))!=  -1)
      {
      px = &player[z];
      if(px->protection > 0)
        {
        printf("\nThat player is still under protection.  Attack someone else.\n");
        goto reget;
        };

      sprintf(zstring,"\nAre you sure you want to attack %s?",px->pname);
      if( boolean(zstring,1) == 2)goto reget;
      fired = 1;
      gpn->credits   -= cost;
      gpn->protection =    0;
      printf("Launching a nuclear missile.....\n");     Delay(75);
      printf("Computing best location to hit.....\n");  Delay(125);
      printf("Nuclear Missile's Target is Locked into the Computer...\n"); Delay(75);
      printf("Missile impact in:    ");
      printf("10"); Delay(25);
      printf("\b\b  ");
      for(i=9;i > 0;--i)
        {
        printf("\b%d",i);
        Delay(20);
        }
      defplan = (px->defense*10) / (px->orep + px->foodp + px->defensep + px->governp);
      if(defplan > 900)defplan = 900;
      if( rnd(1000) > defplan)
        {
        int pop;
        printf("\n Missile has succesfully impacted...\n");
        printf("                    Population  \n");
        printf("Damages inflicted:       %d\n", (px->population/15+1));
        px->population -= (pop = px->population/15 + 1);
        Missile_Attack(pn,z,"Nuclear",0,0,0,0,pop,0,0);

        }
      else
        {
        printf("\nSorry missile was intercepted... No damages inflicted\n");
        }
      player_msg(z,pn, 14, "Missile Launched");
      news(z,pn, 22);
      }

    }
  else
    {
    printf("You do not have enough credits......\n");
    }
  }

void Convent_Fire(cost)
int cost;
  {
  int z,defplan,i;
  char zstring[80];
  struct Player *px;
  gpn = &player[pn];
  if( ! (cost > gpn->credits) )
    {
    reget:
    if((z =  getplayer(2))!=  -1)
      {
      px = &player[z];
      if(px->protection > 0)
        {
        printf("\nThat player is still under protection.  Attack someone else.\n");
        goto reget;
        };

      sprintf(zstring,"\nAre you sure you want to attack %s?",px->pname);
      if( boolean(zstring,1) == 2)goto reget;
      fired = 1;
      gpn->credits   -= cost;
      gpn->protection =    0;
      printf("Launching a conventional nuclear missile.....\n"); Delay(75);
      printf("Computing best location to hit.....\n");  Delay(125);
      printf("Conventional Missile's Target is Locked into the Computer...\n");  Delay(75);
      printf("Missile impact in:    ");
      printf("10"); Delay(25);
      printf("\b\b  ");
      for(i=9;i > 0;--i)
        {
        printf("\b%d",i);
        Delay(20);
        }
      defplan = (px->defense*10) / (px->orep + px->foodp + px->defensep + px->governp);
      if(defplan > 900)defplan = 900;
      if( rnd(1000) > defplan)
        {
        int ft,ds,cr,ca;
        px->fighters -= (ft=px->fighters/10);
        px->defense  -= (ds=px->defense/10);
        px->cruisers -= (cr=px->cruisers/10);
        px->carriers -= (ca=px->carriers/10);
        printf("\n Missile has succesfully impacted...\n");
        printf("                    Fighters  Defense  Cruisers  Carriers\n");
        printf("Damages inflicted:  %8d  %7d  %8d  %8d\n", ft, ds, cr, ca);
        Missile_Attack(pn,z,"ThermoNuclear",0,ds,0,cr,0,ca,ft);
        }
      else
        {
        printf("\nSorry missile was intercepted... No damages inflicted\n");
        }
      player_msg(z,pn, 14, "Missile Launched");
      news(z,pn, 22);
      }

    }
  else
    {
    printf("You do not have enough credits......\n");
    }
  }
