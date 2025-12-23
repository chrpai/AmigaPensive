/* Space Empire Random Events File */
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
void Random_Death( void );
int getplayer ( int yourteam );
void giveplanets ( long amount );
void player_msg ( int to , int from , int msg , char *);
void news ( int to , int from , int msg );
void Random_Events(struct Player *);
void Shaddy( void );
void Hint( void );
void Random_News(void);
extern long DOSBase;
#define MAXANSI 7
extern char *ansii[MAXANSI];
extern char spcpath[80];
extern char rnews[80];
extern struct Player *gpn;  /* global pointer to current player */
extern struct Team *team;
extern struct Player *player;
extern int pn;
extern struct Player *pz, *px;

void Shaddy()
  {
  int who,i;
  char strings[80];
  if( gpn->credits/75 < gpn->score)return;
  printf("A shaddy character walks up to you and offers to eliminate\n");
  if(YesNo("your enemy for you.  Do you wish to inquire about his offer") == 'y')
    {
    printf("He offers to use a new state of the art MWGR (Multi-Warhead Gamma Ray)\n");
    printf("weapon.  He tells you it is guarenteed to eliminate your enemy\n");
    printf("and make sure they aren't a problem anymore. It will void any protection\n");
    sprintf(strings,"The price is %d, Do you want to do it",(gpn->score*25));
    if(YesNo(strings) == 'y' )
      {
      rget:
      printf("\nThe Shaddy character would like to know who to eliminate...\n");
      if((who =  getplayer(2))!=  -1)
        {
        pz = &player[who];
        if(pz->protection > 0)
          {
          printf("\nThat player is still under protection.  Eliminate someone else.\n");
          goto rget;
          };
        sprintf(strings,"\nAre you sure you want to eliminate %s?",pz->pname);
        if( boolean(strings,1) == 2)goto rget;
        gpn->credits -= gpn->score*25;
        gpn->protection =    0;
        if(rnd(100) <= 15)
          {
          printf("The shaddy character has tricked you and launched the MWGR at YOU!\n");
          who = pn;
          pz = &player[who];
          }
        else
          {
          printf("Your sensors detect the MWGR heading towards your enemy...\n");
          player_msg(who,pn,14,"Missile");
          };
        if(rnd(100) <= 10)
           {
           printf("THE MWGR WAS A DUD!!!!!!!  No damage inflicted!!!!\n");
           return;
           }
        else
           {
           printf("MWGR heading towards target..........\n");
           Delay(75);
           printf("MWGR nearing target..................\n");
           Delay(75);
           printf("MWGR Inpact in ");
           for(i=10;i > 0;i--)
             {
             printf(".%d.",i);
             Delay(20);
             }
           if(rnd(100) <= 10)
             {
             printf("\nTHE MWGR WAS INTERCEPTED by defense systems.\nNo damage inflicted!!!!\n");
             return;
             }
           else
             {
             int sl, pop;
             printf("\nMWGR has caused mass devistation.  The enemy population are suffering\n");
             printf("from sever losses.\n");
             Delay(50);
             printf("Population Eliminated: %d   Soldiers: %d\n",(pz->population/10),(pz->soldiers/10));
             pz->population -= ( pop = pz->population/10);
             pz->soldiers   -= ( sl  = pz->soldiers/10);
             Missile_Attack(pn,who,"MWGR",sl,0,0,0,pop,0,0);
             news(who,pn,22);
             }
           }
         }
       }
    }
  }
void Hint()
  {
  char question[80];
  int ptot,price,x,verbose;
  if(gpn->score > gpn->credits)price = gpn->credits;
  if(gpn->score < gpn->credits)price = gpn->score;
  sprintf(question,"Pay %d for Galatic Adviser's help?",price);
  verbose =  boolean(question,2) == 2 ? FALSE : TRUE;
  x = 0;
  if( verbose )gpn->credits -= price;
  if( verbose )printf("The Galatic Advisor is evaluating your empire......\n");
  if( gpn->plague > 0)
    {
    if( verbose )
      {
      printf("The Galatic Advisor warns you that the plague is deadly and you\n");
      printf("should dedicate extra money to medical and plague research to try\n");
      printf("and stop it.\n");
      };
    x++;
    };
  ptot =   (gpn->orep + gpn->foodp + gpn->defensep + 49)/50 ;
  if( ptot != (gpn->governp + 1) && ptot != gpn->governp )
    {
    if( ptot < gpn->governp )
      {
      if( verbose)printf("The Galatic Advisor warns you that you have too many government planets\n");
      x++;
      }
    else
      {
      if(verbose)printf("The Galatic Advisor warns you that you have too few government planets\n");
      x++;
      };
    };
  if( ptot * 2 > gpn->defensep )
    {
    if( verbose )printf("The Galatic Advisor warns you that you could use some more defense planets\n");
    x++;
    };
  if(gpn->soldiers < ptot * 10 )
    {
    if( verbose )printf("The Galatic Advisor suggests you build up your military strength\n");
    x++;
    };
  if(gpn->harmony < 0)
    {
    if( verbose )
      {
      printf("The Galatic Advisor suggests you try and increase your population's\n");
      printf("happiness before revolts and civil war happen.\n");
      };
    x++;
    };
  if(gpn->morale < 0)
    {
    if( verbose )
      {
      printf("The Galatic Advisor suggests you try and increase your army's\n");
      printf("morale before they all desert you.\n");
      };
    x++;
    };
  if(x == 0)
    {
    if( verbose )
      {
      printf("The Galatic Advisor tells you your empire looks like it is in good\n");
      printf("shape.\n");
      }
    }
  else
    {
    if( !verbose )
      {
      if( x > 2)
        {
        printf("It might just be worth your while to find out all the things the\n");
        printf("he seems to think you need some help in certain areas!\n");
        }
      else printf("You might be sorry, his advice is pretty good for the price!\n");
      };
    };
  Random_Events(gpn);
  }

void Random_News()
  {
  switch (gpn->moves)
    {
    case 0: sprintf(rnews," Space Ball score: Duggers %d and Smackers %d",rand()%5,rand()%4);
    break;
    case 1: sprintf(rnews," Investigators find no evidence of foul play in crash");
    break;
    case 2: sprintf(rnews," government plans to hold special election, news at 11");
    break;
    case 3: sprintf(rnews," Foul play is suspected in last night's death");
    break;
    case 4: sprintf(rnews," Medical science is sure that a cure is around the corner");
    break;
    case 5: sprintf(rnews," Special investigator finds corruption in government");
    break;
    case 6: sprintf(rnews," Old laddy finds winning Galatic Lottery Ticket");
    break;
    case 7: sprintf(rnews," Space Ball score: Duggers %d and Smackers %d",rand()%5,rand()%4);
    break;
    default:
    sprintf(rnews," Census reports average city has %d crazies",gpn->moves);
    };
  }
void Random_Events(gpn)
struct Player *gpn;
  {
  int x;
  x = rnd(100) + 1;
  if( x <= 1 )  /* 2 % chance */
    {
    if( gpn->governp > 1 )
      {
      printf("A government planet has exploded!\n");
      gpn->governp -= 1;
      }
    }
  else if( x > 1 && x <= 6 )  /* 5 % chance */
    {
    if(gpn->credits > 1)
      {
      x = gpn->credits / ( 3 + rnd(4) );
      printf("Space pirates steal %d credits!\n",x++);
      gpn->credits -= x;
      };
    }
  else if( x > 6 && x <= 9 )  /* 3 % chance */
    {
    x = gpn->food / ( 3 + rnd(5) );
    if( x > 0 && gpn->food > 0)
      {
      printf("Rats eat part of your food!\n");
      gpn->food -= x;
      };
    }
  else if( x > 9 && x <= 11 ) /* 2 % chance */
    {
    x = rnd(10);
    if( x > 0 )
      {
      printf("Space exploration yields %d new planets!\n",x);
      giveplanets(x);
      }
    else printf("Space exploration finds only dead and uninhabitable planets.\n");
    }
  else if( x > 11 && x <= 13 ) /* 2 % chance */
    {
    x = gpn->population/ (gpn->orep + gpn->foodp + gpn->defensep + gpn->governp);
    printf("Deep Core Drilling has cause massive earthquakes and destroyed a");
    if( gpn->orep > 0 )
      {
      printf("n ore");
      gpn->orep--;
      }
    else if( gpn->foodp > 0)
      {
      printf(" food");
      gpn->foodp--;
      }
    else if( gpn->defensep > 0 )
      {
      printf(" defense");
      gpn->defensep--;
      };
    printf(" planet.\n");
    printf(" %d people were lost in the disaster.\n",x);
    gpn->population -= x;
    }
  else if( x > 14 && x <= 35)  /* 20 % chance */
    {
    Shaddy();
    }
  pause();
  }

void Random_Death()
  {
  if(gpn->harmony > 5 && gpn->harmony < 200)
    {
    printf("You look at yourself in the mirror and try to talk yourself out\n");
    printf("of it, but you just can't do it.  Your empire is falling to pieces\n");
    printf("because of you, and you can't live with yourself knowing that.  You open\n");
    printf("your dresser drawer and pull out a laser pistol.  You pull the trigger\n");
    printf("and the last thing you hear is a distant, \"NOOOOOOOOOO!!\".\n");
    printf("Your empire holds a funeral and mourns deaply at your loss.\n");
    }
  else if(gpn->harmony < -20)
    {
    printf("Your empire isn't looking good these days and your people are very\n");
    printf("upset with you.  You grab your old fashon 9mm and put it up to\n");
    printf("your head.  You think for a quick moment and your finger slips.\n");
    printf("When your people hear the news they rejoice as a cruel and evil\n");
    printf("Emperior dies a dishonerable death.\n");
    }
  else if(gpn->harmony > 200)
    {
    printf("You come back from a hard day's work of ruling your empire.  You are\n");
    printf("tired, and weak of old age.  You muscles and bones can barely support\n");
    printf("you anymore.  You think about how you are going to rule your empire\n");
    printf("like this.  After a night's rest, you decide that you can't go on like\n");
    printf("this.  You love your empire, and they love you, but you don't want them\n");
    printf("to begin to hate you because of your old age.  You take an overdose of\n");
    printf("your sleeping pills, and fall asleep, never to awaken to the world again.\n");
    printf("Your empire is devestated by you actions, and holds a very large funeral.\n");
    printf("Many people hold a cerimonies, trying to raise you back from your grave,\n");
    printf("but none of them are successful, and your empire falls to ruins.\n");
    }

  else
    {
    printf("You wake up one morning, and notice in the mirror that you are no\n");
    printf("longer the young person you used to be.  You used to enjoy waking\n");
    printf("up each morning, and bringing your empire to victory after victory,\n");
    printf("but now it is different.  Instead of enjoying victory, you start\n");
    printf("to enjoy the loses because there is less work to do, and you don't\n");
    printf("have to worry about celebrating.  So you decide that the best thing\n");
    printf("for you and your empire, would be if you didn't have the thrown any\n");
    printf("more.  You leave a note to put someone more youthful in charge, and\n");
    printf("then you open up your dresser drawer.  Out you pull a common hand gun,\n");
    printf("and then you put it to your head, pulling the trigger.  All you feel\n");
    printf("is a acute pain, and then nothing.\n");
    }
  }

void Missile_Attack(from,to,mtype,sl,dl,dpl,dc,pop,carr,ftrs)
int from;    /* attacker */
int to;      /* attacked */
char *mtype; /* missile type*/
int sl;      /* soldiers lost */
int dl;      /* defense stations lost */
int dpl;     /* defense planets lost */
int dc;      /* defending cruisers lost */
int pop;     /* population lost */
int carr;    /* carriers lost */
int ftrs;    /* fighters lost */
  {
  char filename[80], msg[80];
  sprintf(filename,"%sMessages%d.txt",spcpath,to);
  optlog(" ",filename);
  sprintf(msg,"From: Defense Coordinator"); optlog(msg,filename);
  sprintf(msg,"  To: %s",player[to].pname); optlog(msg,filename);
  sprintf(msg,"Subject: Missile attack by %s",player[from].pname); optlog(msg,filename);
  sprintf(msg,"It is my wish to report to you that our glorious empire"); optlog(msg,filename);
  sprintf(msg,"was attacked by  %s missiles.",mtype); optlog(msg,filename);
  sprintf(msg,"Our defenses were strong but we lost:");optlog(msg,filename);
  sprintf(msg,"%d soldiers, %d defense stations, and %d cruisers",sl,dl,dc);
  optlog(msg,filename);
  if( dpl > 0 )
    {
    sprintf(msg,"In the attack, the enemy managed to destroy %d defense planets",dpl); optlog(msg,filename);
    };
  if( pop > 0 )
    {
    sprintf(msg,"We also regret to inform you that %d population were lost",pop);
    }
  else
    {
    sprintf(msg,"Fortunately, no population loses were incurred");
    };
  optlog(msg,filename);
  }
