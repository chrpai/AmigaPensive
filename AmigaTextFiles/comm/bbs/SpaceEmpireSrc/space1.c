/* Space Empire Part 1 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include "dos.h"
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <pragmas/dos_pragmas.h>
#include "space.h"
#include "extra.h"
#include "io.h"
extern long DOSBase;
void Military(int,int,int,int,int,char *,int,int,int,int);
void Team_Military(int,int,int,int,int,char *,int,int,int);
void showplayers ( int yourteam );
int getplayer ( int yourteam );
void giveplanets ( long amount );
void battle_stage ( void );
void send_messages ( void );
void player_msg ( int to , int from , int msg , char *);
void news ( int to , int from , int msg );
void list_alliances ( void );
void show_team ( int tn );
void team_round ( void );
void takeplanets( int );
void dissolve_team ( int z );
#define MAXANSI 7
extern char *ansii[MAXANSI];
extern int sol_work;     /* 1 = soldiers not paid enough */
extern int sol_eff;           /* efficiency based on amount paid */
extern char spcpath[80];
extern struct Player *gpn;  /* global pointer to current player */
extern struct Team *team;
extern struct Player *player;
extern int pn;
/* */

struct Player *pz, *px;
void battle_stage()
  {
  int z,z2,x;
  char zstring[80], *wl;
  long a_dsend,a_csend,a_ssend,ad_lost,ac_lost,as_lost;
  long f_send,s_send,c_send;
  long f_lost,s_lost,c_lost,cr_lost;
  long ed_lost,es_lost,ec_lost;
  long ewin,ywin,pcap,pcap2;
  long erat,rat, per;
  long fmax;
  double mval;
  struct Player *px;
  if(sol_work ==  1)
    {
    printf("\nYour armed forces refuse to fight since you didn't pay them enough.\n");
    return;

    }
  if(!((gpn->soldiers>0)||(gpn->fighters>0)||(gpn->cruisers>0))) return;
  reget:
  printf("\nYour armed forces want to know who they should attack this round...\n");
  if((z =  getplayer(2))!=  -1)
    {
    pz = &player[z];
    if(pz->protection > 0)
      {
      printf("\nThat player is still under protection.  Attack someone else.\n");
      goto reget;

      };
    sprintf(zstring,"\nAre you sure you want to attack %s?",pz->pname);
    if( boolean(zstring,1) == 2)goto reget;
    gpn->protection =    0;
    f_send =  0;
    s_send =  0;
    c_send =  0;
    if(gpn->soldiers > 0)
      {
      s_send = Request_Data("Send how many soldiers",gpn->soldiers,gpn->soldiers);

      };
    if( gpn->fighters > 0 && gpn->carriers > 0 )
      {
      printf("You have %ld carriers, and each may carry 100 fighters.\n",gpn->carriers);
      fmax =    (gpn->carriers * 100);
      if( fmax > gpn->fighters ) fmax =    gpn->fighters;
      f_send = Request_Data("Send how many fighters",fmax,fmax);

      };
    if(gpn->cruisers > 0 )
      {
      c_send = Request_Data("Send how many cruisers",gpn->cruisers,gpn->cruisers);

      };
    if( s_send == 0 && f_send == 0 && c_send == 0)
      {
      printf("What's a matter, you a conscientious objector?\n");
      goto reget;

      };
    printf("You are sending %d armies, %d fighters, and %d cruisers into battle\n",
    s_send, f_send, c_send);
    printf("Your army's morale is %s%s%s.\n"
    ,ansii[3],harmony_level(1,gpn->morale),ansii[0]);
    if( boolean("Are you sure this is what you wish to do?",1)==2)goto reget;
    per = ( 10 + sol_eff) * 10;
    if( sol_eff > 0 )
      {
      printf("You have treated your troops well, they get a %d %% fighting bonus\n",per);
      }
    else if( sol_eff < 0 )
      {
      printf("You have mistreated your troop's they fight at %d %% efficiency\n",per);
      }
    /* Base calculations */
    ywin =    c_send * 7 + f_send * 3 + s_send + gpn->defensep * 20;
    ywin = ( ywin * per) / 100;
    ewin =    pz->cruisers * 7 + pz->defense * 3;
    ewin +=   pz->soldiers + pz->defensep * 25 + 50;
    if( pz->plague > 0 && gpn->plague < 0 )
      {
      gpn->plague = pz->plague;
      };
    /* Mutual defense forces adjustment */
    a_ssend = a_csend = a_dsend = 0;
    if(pz->team_num !=  0)
      {
      for(z2=1; z2 < PLAYERS ; z2++)
        {
        px = &player[z2];
        if( px->team_num == pz->team_num )
          {
          a_ssend +=  px->team_soldiers;
          a_csend +=  px->team_cruisers;
          a_dsend +=  px->team_defense;
          };
        };
      if(a_ssend > 0 || a_csend > 0 || a_dsend > 0 )
        {
        printf("Defense forces from your enemy's alliance arrive!\n");
        ewin +=  a_csend * 5  + a_dsend * 2  + a_ssend;

        };

      };
    /* Morale adjustment */
    mval =    (double)(100 + gpn->morale) * 0.01;
    if(mval < 0.5) mval =    0.5;
    if(mval > 2.0) mval =    2.0;
    ywin =    (unsigned long)((double)ywin * mval);
    mval =    (double)(100 + pz->morale) * 0.01;
    if(mval < 0.5) mval =    0.5;
    if(mval > 2.0) mval =    2.0;
    ewin =    (unsigned long)((double)ewin * mval);
    /* Element of chance calculation based on population */
    z2 = ( pz->population / 5000 );
    if( z2 > 200)z2 = 200;
    if( z2 > 10)ewin += rnd( z2 );
    z2 = (gpn->population / 5000 );
    if( z2 > 200)z2 = 200;
    if( z2 > 50)ywin += rnd( z2 );
    printf(" Your forces are off to battle.");
    for(z2 =  0; z2 < 5; z2++)
      {
      printf(" .");
      Delay(10);

      }
    printf(" .");
    /* Resolution */
    if(ywin > ewin)
      {
      printf("\nYou were victorious!\n");
      player_msg(z,pn,1,"You were attacked!");
      news(z,pn,1);
      gpn->score_modifier +=  500;
      gpn->morale++ ;
      pz->morale--;
      wl = "Lost ";
      }
    else
      {
      printf("\nYou lost...\n");
      player_msg(z,pn,2,"You were attacked!");
      news(z,pn,2);
      gpn->morale-- ;
      gpn->score_modifier -= 100;
      pz->morale++;
      wl = "Won ";
      };
    erat = ( 100 * ywin ) / (ywin + ewin );  /* enemy lose percentage */
    rat  = ( 100 * ewin ) / (ywin + ewin );  /* attacker lose percentage */
    if( erat > 80) erat = 80;
    if( erat <  5) erat =  1;  /* special overwhelming forces adjustment */
    if(  rat > 80)  rat = 80;
    if(  rat <  5)  rat =  1;
    /*          Player loses */
    s_lost  =  ( s_send * rat + 99)/100;
    f_lost  =  ( f_send * rat + 99)/100;
    c_lost  =  ( c_send * rat + 99)/100;
    cr_lost =  f_lost/100;  /* lose one carrier for every 100 ftrs destroyed*/
    /*             enemy loses */
    ed_lost =  ( pz->defense  * erat + 99)/100;
    ec_lost =  ( pz->cruisers * erat + 99)/100;
    es_lost =  ( pz->soldiers * erat + 99)/100;
    /*           Adjustments made   */
    gpn->soldiers -= s_send;
    if( (gpn->S_used += ( s_send - s_lost )) < 0)gpn->S_used = 0;
    gpn->cruisers -= c_send;
    if( (gpn->C_used += ( c_send - c_lost )) < 0)gpn->C_used = 0;
    gpn->fighters -= f_send;
    if( (gpn->F_used += ( f_send - f_lost )) < 0)gpn->F_used = 0;
    gpn->carriers -= cr_lost;
    if( gpn->carriers < 0)gpn->carriers = 0;
    if( (pz->defense   -= ed_lost) < 0 ) pz->defense   = 0;
    if( (pz->soldiers  -= es_lost) < 0 )
      {
      pz->soldiers  = 0;
      pz->morale = 0;
      };
    if( (pz->cruisers  -= ec_lost) < 0 ) pz->cruisers  = 0;
    printf("You lost %ld soldiers, %ld fighters, and %ld cruisers.\n"
    ,s_lost,f_lost,c_lost);
    printf("Your enemy lost %ld soldiers, %ld defense stations, and %ld cruisers.\n"
    ,es_lost,ed_lost,ec_lost);

    ad_lost = 0;
    if( rnd(100) > 90 && pz->defensep > 0 )
      {
      ad_lost = rnd( pz->defensep + 1 );
      if( ad_lost > 3 )ad_lost = 3;
      if( pz->defensep < ad_lost)ad_lost = pz->defensep;
      if( ad_lost > 0 )
        {
        printf("You destroyed %d defense planets in the attack!\n",ad_lost);
        pz->defensep -= ad_lost;
        };
      };

    Military(pn,z,s_send,f_send,c_send,wl,es_lost,ed_lost,ad_lost,ec_lost);

    /*          Alliance loses  and adjustment         */
    if( pz->team_num != 0 && (a_ssend > 0 || a_csend > 0 || a_dsend > 0) )
      {
      ad_lost = ac_lost = as_lost = 0;
      for(x =  1; x <  PLAYERS; x++)
        {
        px = &player[x];
        if(px->team_num ==  pz->team_num)
          {
          if(px->team_defense > 0)
            {
            ed_lost =  ( px->team_defense * erat + 99 )/100;
            if( (px->team_defense -= ed_lost) < 0 )px->team_defense = 0;
            ad_lost += ed_lost;
            };
          if(px->team_cruisers > 0 )
            {
            ec_lost =  ( px->team_cruisers * erat + 99 )/100;
            if( (px->team_cruisers -= ec_lost) < 0 )px->team_cruisers = 0;
            ac_lost += ec_lost;
            };
          if(px->team_soldiers > 0 )
            {
            es_lost =  ( px->team_soldiers * erat + 99 )/100;
            if( (px->team_soldiers -= es_lost) < 0 )px->team_soldiers = 0;
            as_lost += es_lost;

            };
          Team_Military(pn,x,s_send,f_send,c_send,wl,es_lost,ed_lost,ec_lost);

          };

        };

      };
    if( a_ssend > 0 && a_csend > 0 && a_dsend > 0 )
      {
      printf("Your enemy's alliance lost %ld soldiers, %ld defense stations, and %ld cruisers.\n"
      ,as_lost,ad_lost,ac_lost);

      };
    if( ywin > ewin )
      {
      if( erat > 70 )
        {
        pcap = ( pz->food * erat ) / 2000;
        if( pcap > 0 )
          {
          printf("You capture %d food stores from %s\n",pcap,pz->pname);
          pz->food -= pcap;
          gpn->food += pcap;

          };
        pcap = ( pz->credits * erat ) / 2000;
        if( pcap > 0 )
          {
          printf("You capture %d credits from %s\n",pcap,pz->pname);
          pz->credits -= pcap;
          gpn->credits += pcap;

          };

        };
      pcap =    0;
      if(pz->defensep > 0)
        {
        pcap2 =  (  pz->defensep * erat )/1000 + 1;
        if( erat > 90 && pcap2 < 2 ) pcap2 = 2;
        if(pz->defensep < pcap2 )pcap2 = pz->defensep;
        pz->defensep -=  pcap2;
        pcap +=  pcap2;

        };
      if(pz->orep > 0)
        {
        pcap2 =  ( pz->orep * erat )/1000 + 1;
        if( erat > 90 && pcap2 < 2 ) pcap2 = 2;
        if(pz->orep < pcap2 )pcap2 = pz->orep;
        pz->orep -= pcap2;
        pcap += pcap2;

        };
      if(pz->foodp > 0)
        {
        pcap2 =  ( pz->foodp * erat)/1000 + 1;
        if( erat > 90 && pcap2 < 2 ) pcap2 = 2;
        if(pz->foodp < pcap2 )pcap2 = pz->foodp;
        pz->foodp -=  pcap2;
        pcap +=  pcap2;

        };
      if( ( (pz->foodp + pz->orep + pz->defensep) == 0 ) && pz->governp > 0)
        {
        pcap2 =  (  pz->governp * erat ) /2000 + 1;
        if( erat > 90 && pcap2 < 2 ) pcap2 = 2;
        if(pz->governp < pcap2 )pcap2 = pz->governp;
        pz->governp -= pcap2;
        pcap += pcap2;

        }
      printf("You captured %ld planets\n!",pcap);
      if( pz->governp  ==  0
      &&  pz->foodp    ==  0
      &&  pz->orep     ==  0
      &&  pz->defensep ==  0)
        {
        printf("You have crushed your enemy!\n");
        if(pz->credits > 0)
          {
          printf("You take all their credits!\n");
          gpn->credits = gpn->credits + pz->credits;

          }
        if(pz->food > 0)
          {
          printf("You take all their food!\n");
          gpn->food = gpn->food + pz->food;

          }
        news(z,pn,3);
        gpn->morale  += 2;
        gpn->harmony += 1;
        strcpy(pz->name,"X");
        strcpy(pz->pname,"X");
        px->team_apply = 0;
        if(pz->team_num ==  z )
          {
          dissolve_team(pz->team_num);

          };

        }
      giveplanets(pcap);
      printf("\n");

      }
    printf("\n");

    }

  }

void Military(from,to,as,af,ac,wl,sl,dl,dpl,dc)
int from,to,as,af,ac,sl,dl,dpl,dc;
char *wl;
  {
  char filename[80], msg[80];
  sprintf(filename,"%sMessages%d.txt",spcpath,to);
  optlog(" ",filename);
  sprintf(msg,"From: Defense Coordinator");
  optlog(msg,filename);
  sprintf(msg,"  To: %s",player[to].pname);
  optlog(msg,filename);
  sprintf(msg,"Subject: %s Battle",wl);
  optlog(msg,filename);
  sprintf(msg,"It is my wish to report to you that our glorious empire");
  optlog(msg,filename);
  sprintf(msg,"was attacked by %s.   In the combat we lost"
  ,player[from].pname);
  optlog(msg,filename);
  sprintf(msg,"%d soldiers, %d defense stations, and %d cruisers",sl,dl,dc);
  optlog(msg,filename);
  if( dpl > 0 )
    {
    sprintf(msg,
    "In the attack, the enemy managed to destroy %d defense planets",dpl);
    optlog(msg,filename);
    };
  sprintf(msg,"It was reported that we were attacked by");
  optlog(msg,filename);
  sprintf(msg,"%d soldiers, %d fighters, and %d cruisers\n",as,af,ac);
  optlog(msg,filename);
  }

void Team_Military(from,to,as,af,ac,wl,sl,dl,dc)
int from,to,as,af,ac,sl,dl,dc;
char *wl;
  {
  char filename[80], msg[80];
  sprintf(filename,"%sMessages%d.txt",spcpath,to);
  optlog(" ",filename);
  sprintf(msg,"From: Alliance Defense Coordinator");
  optlog(msg,filename);
  sprintf(msg,"  To: %s",player[to].pname);
  optlog(msg,filename);
  sprintf(msg,"It is my wish to report to you that your glorious team");
  optlog(msg,filename);
  sprintf(msg,"was attacked by %s.   In the combat we lost"
  ,player[from].pname);
  optlog(msg,filename);
  sprintf(msg,"%d soldiers, %d defense stations, and %d cruisers",sl,dl,dc);
  optlog(msg,filename);
  sprintf(msg,"It was reported that we were attacked by");
  optlog(msg,filename);
  sprintf(msg,"%d soldiers, %d fighters, and %d cruisers\n",as,af,ac);
  optlog(msg,filename);
  }
