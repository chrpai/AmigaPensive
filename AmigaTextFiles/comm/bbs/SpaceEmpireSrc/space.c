/* Space Empire Part 1 */
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

struct Team *team;
struct Player *player;
int pn;
int Quick_Save = TRUE;   /* TRUE means we do not have to save */
int oremount,foodmount,defmount,fightmount,crusmount;
int today;
char    newsfile[80];
char oldnewsfile[80];
char   scorefile[80];
char hiscorefile[80];
int MOVES;
int MAXSCORE;
int MAXPROT;

char rnews[80];
void Get_CFG_Data(char *,FILE *,char *,int);
void Random_Death( void );
void Random_News(void);
void Daily_Maintenance(void);
void Random_Events(struct Player *);
extern long DOSBase;
void Reset_News(void);
int Do_Break(void);
void dissolve_team ( int z );
void readcfg ( void );
void showplayers ( int yourteam );
int getplayer ( int yourteam );
void strife ( int h );
int main ( int argc , char *argv []);
void showstats ( void );
void ruling_stage ( void );
long you_give ( char mstring [], long mneed );
void giveplanets ( long amount );
void buyitem ( int n );
void buying_stage ( void );
void battle_stage ( void );
void makefiles ( void );
void savefiles ( void );
void scores ( void );
int lord ( void );
void readfiles ( void );
void reset ( void );
void send_messages ( void );
void player_msg ( int to , int from , int msg , char *);
void news ( int to , int from , int msg );
void covert_stage ( void );
void spy_show ( int playn , int type );
void list_alliances ( void );
void show_team ( int tn );
void team_round ( void );
void takeplanets( long );
void sellitem( void );
void Shaddy( void );
void Hint( void );
void Intelligence(int,int,int, int);
void Weapons( void );
long Compute_Planet_Limit(void);
struct Task *mytask; /* Pointer to this task so we can name it */
char *mission[] =
  {
  "Spy on military  ",   "Spy on government ",
  "Spy on everything",   "Insurrection Aide",
  "Demoralize Troops",   "Demoralize Empire",
  "Steal Credits    ",   "Steal food       ",
  "Setup            ",   "Plague Infection ",
  "Spy on Alliance  "
  };

struct Prices Item[] =
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


#define MAXANSI 7
char *ansii[MAXANSI] =
  {
  "[0m",          /* reset, ansii off */
  "[31;40m",      /* Color  Forground   Backround */
  "[32;40m",      /* black     31          41     */
  "[33;40m",      /* white     32          42     */
  "[34;40m",      /* orange    33          43     */
  "[35;40m",
  "[36;40m",
  };
int cov_work =  0;     /* 1 = covert agents not paid enough */
int cov_eff;           /* efficiency based on amount of payment */
int sol_work =  0;     /* 1 = soldiers not paid enough */
int sol_eff;           /* efficiency based on amount paid */
int pop_increase;
int pop_migration;
int maxplanets;       /* max planets per turn  */
int pvalue;           /* price of current item */
int maxdraft;         /* total men drafted this turn */
char spcpath[80];
struct Player *gpn;  /* global pointer to current player */
/* */
void dissolve_team(z)
int z;
  {
  int x;
  register struct Player *px;
  news(z,z,9);
  for(x =  1; x <  PLAYERS; x++)
    {
    px = &player[x];
    if(px->team_num ==  z)
      {
      px->team_num   =  0;
      px->team_apply =  0;
      px->defense  +=  px->team_defense;
      px->soldiers +=  px->team_soldiers;
      px->cruisers +=  px->team_cruisers;
      px->team_defense   =  0;
      px->team_soldiers  =  0;
      px->team_cruisers  =  0;
      };

    };

  }
void readcfg()
  {
  char ostring[80];
  FILE *fp;
  sprintf(ostring,"%sSpcEmp.cfg",spcpath);
  if( (fp =  fopen(ostring,"r")) == NULL)
    {
    printf("You must create \"%s\" configuration file\n",ostring);
    printf("Space Empire could not open it.\n");
    end();

    };
  Get_CFG_Data("Current News filename",fp,newsfile,80);
  Get_CFG_Data("Yesterday's News filename",fp,oldnewsfile,80);
  Get_CFG_Data("Score filename",fp,scorefile,80);
  Get_CFG_Data("High Score filename",fp,hiscorefile,80);
  Get_CFG_Data("moves per day",fp,ostring,40);
  MOVES = atoi(ostring);
  Get_CFG_Data("Space Lord Score limit",fp,ostring,40);
  MAXSCORE = atoi(ostring);
  Get_CFG_Data("planet protection limit",fp,ostring,40);
  MAXPROT = atoi(ostring);
  }
void Get_CFG_Data(str,fp,buff,blen)
char *str; /* Item name for error messages */
FILE *fp;
char *buff;
int  blen;
  {
  if( fgets(buff,blen,fp) == NULL)
    {
    printf("Unable to read %s from the configuration file\n",str);
    end();
    };
  strip_crlf(buff);

  }

void showplayers(yourteam)
int yourteam;
  {
  int cou,totp;
  register struct Player *px;
  printf("\n");
  printf("    Name                     Planets    Score\n");
  printf("--- -----------------------  -------  ---------\n");
  for(cou =  1; cou < PLAYERS; cou++)
    {
    px = &player[cou];
    totp = px->orep + px->defensep + px->foodp + px->governp;
    if( strcmp("X",px->name) !=  0 && totp > 0)
      {
      if(yourteam ==  0 || px->team_num ==  yourteam )
        {
        printf("%2d. %-23s  %7ld  %9ld\n"
        ,cou,px->pname,totp,px->score);

        };

      };

    };
  printf("\n");

  }
int getplayer(yourteam)
int yourteam;
  {
  struct Player *px;
  char ostring[80];
  int playn;
  comeagain:
  printf("\nEnter empire's number ('?' to list players): ");
  (void)gets(ostring);
  if(ostring[0] ==  '?')
    {
    printf("LIST PLAYERS\n");
    if( yourteam == 1 )showplayers(gpn->team_num);
    else showplayers(0);
    goto comeagain;

    };
  playn = atoi(ostring);
  if(playn < 1 || playn > PLAYERS)
    {
    printf("NOBODY\n");
    return -1;

    }
  px = &player[playn];
  if((strcmp("X",px->name)) ==  0) goto comeagain;
  printf("%s\n",px->pname);
  if(yourteam ==  1)
    {
    if(gpn->team_num == 0 || px->team_num !=  gpn->team_num)
      {
      printf("That empire isn't part of your alliance.\n");
      goto comeagain;

      }

    }
  if(yourteam ==  2)
    {
    if(gpn->team_num != 0 && px->team_num ==  gpn->team_num)
      {
      printf("That empire is part of your alliance!\n");
      goto comeagain;

      }

    }
  if(playn ==  pn)
    {
    printf("You can't do that to yourself!\n");
    goto comeagain;

    }
  return playn;

  }
void strife(h)
int h;
  {
  struct Player *px = gpn;
  int rnum;
  if( h > -8 && h < 8) return;
  rnum = rnd(200);
  if( ( rnum > h && h > 0 ) || ( rnum > -h && h < 0 ) ) return;
  rnum = rnd(10) + 1;
  if( h <  0 )
    {
    switch(rnum)
      {
      case 1:
      printf("\nStudent protesters wreak havok in major cities.\n");
      break;
      case 2:
      printf("\nA high ranking official in your government was recently assassinated.\n");
      break;
      case 3:
      printf("\nPrisons become overpopulated with political prisoners.\n");
      break;
      case 4:
      printf("\nCivil strife errupts at frontier planets.\n");
      px->morale -=   1;
      break;
      case 5:
      printf("\nRebels confront military in scattered skirmishes.\n");
      px->morale -=   1;
      break;
      case 6:
      if(px->governp > 0)
        {
        printf("\nAnti-matter bomb detonated by rebels on a governing planet.\n");
        px->governp -= 1;

        }
      break;
      case 7:
      printf("\nInsurection increasing every day!.\n");
      px->morale -= 1;
      break;
      case 8:
      printf("\nCivil war breaks out across your empire!");
      printf("Your empire is shattered by revolt!\n");
      px->harmony  =  0;
      px->morale   =  0;
      rnum = rnd(1000);
      if(px->soldiers > 0) px->soldiers =  (px->soldiers * (1000+rnum))/(2000+rnum);
      if(px->defensep > 0) px->defensep =  (px->defensep * (1000+rnum))/(2000+rnum);
      if(px->fighters > 0) px->fighters =  (px->fighters * (1000+rnum))/(1000+rnum);
      if(px->carriers > 0) px->carriers =  (px->carriers * (1000+rnum))/(1000+rnum);
      if(px->orep     > 0) px->orep     =  (px->orep     * (1000+rnum))/(1500+rnum);
      if(px->foodp    > 0) px->foodp    =  (px->foodp    * (1000+rnum))/(1500+rnum);
      if(px->governp  > 0) px->governp  =  (px->governp  * (1000+rnum))/(1500+rnum);
      px->population =  ( px->population  * (1000+rnum))/(1500+rnum);
      px->food = 11 * px->population;
      news(0,pn,6);
      break;
      default:
      printf("\nProtests mount as people show their anger at your rule");
      }

    }
  else
    {
    switch(rnum)
      {
      case 1:
      rnum = px->population/100 + 1;
      printf("%ld people voluntier for your armed forces!\n",rnum);
      px->soldiers += rnum;
      break;
      case 2:
      printf("Work rate on your planets temporary increase!\n");
      oremount  += 1000;
      break;
      case 3:
      printf("Workers on your Farming planets produce more!\n");
      foodmount += 500;
      break;
      case 4:
      printf("Industries workers work over time!\n");
      defmount   += 1;
      crusmount  += 1;
      fightmount += 1;
      break;
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      rnum = rnd(50);
      printf("%ld People emmigrate to your country!\n",rnum);
      px->population += rnum;
      break;

      }

    }

  }
main(argc,argv)
int argc;
char *argv[];
  {
  short i,last_unused;
  char  username[21], ostring[81], str[81];
  long taxes,mearned,c_built,f_built,d_built,fgrown;
  int coun,coun2;
  int noreset;
  int ptot,gtot;
  long deserters;
  float pval;
  long amigatime,dummy;
  struct tm *systime;
  if( argc != 3 && argc != 4 )
    {
    printf(" %s <username>  <directory> [ force ]\n",argv[0]);
    printf("       ^             ^          ^___ Any string\n");
    printf("       |             |___directory with config file\n");
    printf("       |_____________Username in quotes \n");
    printf(" See the Sysop docs for more information\n");
    exit(10);
    };
  player = calloc(PLAYERS+1,sizeof(struct Player));
  if( player == NULL )
    {
    System_Error("calloc","Unable to get memory for Player info");

    };
  team   = calloc(TEAMS+1, sizeof(struct Team));
  if( team == NULL )
    {
    System_Error("calloc","Unable to get memory for team info");

    };
  amigatime =  time(&dummy);
  systime =  localtime(&amigatime);
  today =  systime->tm_mday;
  if( onbreak(&Do_Break) )printf("Cannot set break for ^C");
  /*  */
  /* ARG1 =    User name, ARG2 =    door path */
  /*  */
  memset(username,'\0',21);
  memset(spcpath,'\0',80);
  if( argc == 2  || argv[1] == "" )
    {
    while( strlen(username) < 3 )
      {
      pprompt("\nWhat is your username?",username,20);
      strncpy(spcpath,argv[1],79);   /* copy the path to files */
      };
    }
  else
    {
    strncpy(username,argv[1],20);   /* copy the User Name*/
    strncpy(spcpath,argv[2],79);   /* copy the path to files */
    };
  strip_crlf(username);
  strtoup(username);
  if( boolean("Do you want ANSI color",2) == 2)
    {
    for(i=0;i<MAXANSI;i++)ansii[i] = "";
    };
  oremount   = 19000 +  rnd(1000);
  foodmount  =  3500 +  rnd(200) ;
  defmount   = 2;
  fightmount = 2;
  crusmount  = 1;
  printf("\n\n");
  printf(" +----------------------------------------+\n");
  printf(" |    Citadel Space Empire - Amiga V%04.4s  |\n",VERSION);
  printf(" |    (C) 1992-93 Custom Services         |\n");
  printf(" |    Freeware distribution only!         |\n");
  printf(" |    Created by Jon Radoff(Space Empire) |\n");
  printf(" |    Call 609-953-8159 for information   |\n");
  printf(" +----------------------------------------+\n");
  sprintf(ostring,"%sSpcPlayer.DAT",spcpath);
  readcfg();
  if( access(ostring,6) == -1 )
    {
    printf("\nCreating Space Empire's data files... ");
    makefiles();
    printf("Done.\n");

    }
  else    readfiles();
  noreset =  0;
  for(coun2 =  1; coun2 <  PLAYERS; coun2++)
    {
    if((strcmp("X",player[coun2].name)) ==  0) continue;
    if(player[coun2].lastplay ==    today)   noreset =  1;

    };
  if(noreset ==  0) Daily_Maintenance();
  last_unused = pn =  0;
  for(coun =  1; coun <  PLAYERS && pn == 0; coun++)
    {
    if( strcmp("X",player[coun].name) == 0)
      {
      last_unused = coun;
      }
    else if((strcmp(username,player[coun].name)) ==  0)
      {
      gpn = &player[coun];
      pn =  coun;
      printf("\nWelcome back, %s...\n",gpn->pname);

      };

    };
  if( pn != 0 )
    {
    if(gpn->moves ==  0)
      {
      printf("Sorry, you are out of turns for today...\n");
      goto gameover;

      };
    sprintf(ostring,"%sMessages%ld.TXT",spcpath,pn);
    if( access(ostring,0) == 0)
      {
      showfile(ostring);
      if( boolean("Do you wish to purge your messages?",2)==1) unlink(ostring);

      };

    }
  else /* new player */
    {
    pn =  last_unused;
    printf("\nHello, %s.  You must be new in the galaxy.\n",username);
    if( pn == 0 )
      {
      printf("Unfortunately, the galaxy is overpopulated at the moment.\n");
      printf("Empires come and go.  Keep trying each day!\n");
      end();

      };
    gotname:
    pprompt("\nWhat name shall the galaxy know you by? ",str,19);
    if(strlen(str)<3)
      {
      printf("\nCome on...  You can do better than that!\n");
      goto gotname;

      }
    strip_crlf(str);       /* strip off the newline and make all upper case */
    strtoup(str);
    for(coun =  1; coun <  PLAYERS; coun++)
      {
      if((strcmp(str,player[coun].pname)) ==  0)
        {
        printf("I doubt the other %s would appreciate that.  Pick another name.",str);
        goto gotname;

        }

      };
    gpn = &player[pn];
    strcpy(gpn->pname,str);
    strcpy(gpn->name,username);
    gpn->moves           =    MOVES;
    gpn->soldiers        =    0;
    gpn->fighters        =    0;
    gpn->cruisers        =    0;
    gpn->credits         =    250000;
    gpn->orep            =    2;
    gpn->foodp           =    1;
    gpn->defensep        =    0;
    gpn->food            =    7000;
    gpn->carriers        =    0;
    gpn->population      =    500;
    gpn->protection      =    20;
    gpn->defense         =    0;
    gpn->governp         =    1;
    gpn->score           =    0;
    gpn->score_modifier  =    0;
    gpn->morale          =    0;
    gpn->harmony         =    3;
    gpn->agents          =    0;
    gpn->space_lord_days =    7;
    gpn->team_num        =    0;
    gpn->team_apply      =    0;
    gpn->team_defense    =    0;
    gpn->team_soldiers   =    0;
    gpn->team_cruisers   =    0;
    gpn->plague          =    0;  /* no infection    */
    gpn->command_ship    =    0;  /* no command ship */
    gpn->bank_loan       =    0;  /* amount borrowed */
    gpn->bank_acct       =    0;  /* amount in account */
    gpn->S_used          =    0;
    gpn->C_used          =    0;
    gpn->F_used          =    0;
    gpn->inactive        =    0;
    gpn->I_used          =    0;
    gpn->lastplay        =  today;  /* mark as maint. run */
    savefiles();
    sprintf(ostring,"%sMessages%ld.TXT",spcpath,pn);
    unlink(ostring);
    printf("\nWell...  Here you are.  Just another new emperor with a hoard of cash, some\n");
    printf("food, and the massive population your father left you on these three measly\n");
    printf("planets.  What are you waiting for?  Start buying some weapons!  Do what\n");
    printf("nobody in your family has done before!  CONQUER!  DESTROY!  YOU can rule\n");
    printf("the universe if you TRY!\n");
    pause();
    };
  if((coun = lord()) ==  -1)
    {
    printf("There is still no emperor powerful enough to be Space Lord.\n");

    }
  else
    {
    if((player[coun].space_lord_days) < 1 )
      {
      printf("\n%s has ascended the iridium throne and is the new Lord of ",player[coun].pname);
      printf("the Galaxy.\nPlease notify your sysop to reset the game.\n");
      savefiles();
      end();

      };
    printf("\nThe current Space Lord is %s.  He has %ld days remaining before\n"
    ,player[coun].pname,player[coun].space_lord_days);
    printf("he may ascend the iridium throne!\n");

    };
  while( TRUE )
    {
    printf("1. Read Yesterday's news...........\n");
    printf("2. Read Today's news...............\n");
    printf("3. Information about current game..\n");
    printf("4. Show players....................\n");
    printf("5. Show Alliances..................\n");
    printf("6. Abdicate your empire............\n");
    printf("7. Read instructions...............\n");
    printf("8. Read latest changes.............\n");
    printf("9. Read your messages..............\n");
    printf("A. Cancel your protection..........\n");
    printf("B. Send Messages...................\n");
    printf("C. Continue(or hit return).........\n");
    printf("Q. Quit for now....................\n");
    hotkey("Which would you like:",ostring);
    switch ( *ostring )
      {
      case '1': showfile(oldnewsfile); break;
      case '2': showfile(newsfile);break;
      case '3':
      printf(" Citadel Space Empire %s\n",VERSION);
      printf(" You are allowed %ld turns per day\n",MOVES);
      printf(" You need a score greater than %ld to become space lord\n",MAXSCORE);
      printf(" New players have a maximum of %ld planets under protection\n",MAXPROT);
      break;
      case '4': showplayers(0); break;
      case '5': list_alliances(); break;
      case '6':
      if(boolean("Are you absolutely sure, no way back",2) == 1)
         {
         if( gpn->team_num !=  0 )
           {
           if( gpn->team_num == pn)
             {
             dissolve_team(pn);
             };
           gpn->team_num = 0;
           };
         Random_Death();
         news(pn,pn,23);
         gpn->team_apply = 0;
         strcpy(gpn->name,"X");
         strcpy(gpn->pname,"X");
         savefiles();
         end();
         };
      break;
      case '7':
      sprintf(ostring,"%sSpcEmp.DOC",spcpath);
      showfile(ostring);
      break;
      case '8':
      sprintf(ostring,"%sChanges.DOC",spcpath);
      showfile(ostring);
      break;
      case ' ':
      case 'c':
      case 'C': goto taketurns;
      case 'A':
      case 'a':
      if( boolean("Are your sure you wish to cancel protection?",2)==1)gpn->protection = 0;
      case 'B':
      case 'b':
      send_messages(); break;
      case 'q':
      case 'Q':
      savefiles();
      end();
      case '9':
      sprintf(ostring,"%sMessages%ld.TXT",spcpath,pn);
      if( access(ostring,0) == 0)
        {
        showfile(ostring);
        if( boolean("Do you wish to purge your messages?",2)==1) unlink(ostring);

        };
      break;
      default: printf("That is not one of your choices...\n");
      };
    };

  taketurns:
  news(pn,pn,13);
  news(pn,pn,21);
  while( gpn->moves-- > 0 )
    {
    maxplanets   =Compute_Planet_Limit();  /* planet purchase limit */
    maxdraft     = 0;  /* number of soldiers drafted today */
    gpn->inactive= 0;  /* reset the inactivity counter */
    ptot    = gpn->defensep + gpn->orep + gpn->foodp;
    gtot    = gpn->governp;
    if(gpn->protection >  0)
      {
      if( ptot + gtot > MAXPROT || gpn->protection-- <= 0 )
        {
        news(pn,pn,14);
        printf(" Your protection has ended\n");
        gpn->protection = 0;
        };
      };
    reset();  /* computer the scores */
    team_round();
    Random_Events(gpn);
    ptot    = ( ptot + 49 )/ 50;   /* must have 1 gov planet for every 50 */
    if( ptot < 1) ptot = 1;
    if( gtot < 1) gtot = 1;
    taxes   =    gpn->population * 100;
    if( ptot == gtot || (ptot + 1)  == gtot )
      {
      taxes   = (taxes / ptot)*gtot;
      }
    else if ( ptot < gtot )
      {
      printf("Excessive government costs reduce tax intake! \n");
      news(pn,pn,10); /* too much government */
      taxes = ( taxes / gtot ) * ptot;
      }
    else
      {
      printf("Lack of government control reduces tax intake! \n");
      news(pn,pn,11); /* too little government */
      taxes = ( taxes / ptot ) * ptot;

      }
    mearned =    gpn->orep*oremount;
    fgrown  =    gpn->foodp*foodmount;
    d_built =    gpn->defensep*defmount;
    f_built =    gpn->defensep*fightmount;
    c_built =    gpn->defensep*crusmount;
    gpn->credits  +=  mearned;
    gpn->food     +=  fgrown;
    gpn->credits  +=  taxes;
    gpn->defense  +=  d_built;
    gpn->fighters +=  f_built;
    gpn->cruisers +=  c_built;
    sol_work =  0;
    cov_work =  0;
    showstats();
    printf("Taxes Collected: %ld credits  Ore sold: %ld\n",taxes,mearned);
    printf("Food Grown: %ld kilotons  Food Reserve: %ld\n",fgrown,gpn->food);
    printf("Build Defense stations: %ld  Fighters: %ld  ",d_built,f_built);
    printf("Cruisers: %ld\n",c_built);
    if( ptot > (gpn->population >>3))
      {
      printf("You don't have enough people to work your planets!!!!\n");
      if(oremount    > 5000 )oremount    -= 1000;
      if(foodmount   >  800 )foodmount   -=  800;
      if(defmount    > 1    )defmount    -= 1;
      if(fightmount  > 1    )fightmount  -= 1;
      if(crusmount   > 1    )crusmount   -= 1;
      }
    pause();
    ruling_stage();
    buying_stage();
    covert_stage();
    if(gpn->protection > 0)
      {
      printf(" You have %ld turns of protection left\n",gpn->protection);
      printf(" Doing battle automatically cancels protection\n");

      };
    if( boolean("Do you wish to do battle today?",2) == 1)
      {
      printf(" You may fire missiles or do conventional battle\n");
      if( boolean("Do you wish to fire missiles?",2) == 1)
        {
        Weapons();
        }
      else battle_stage();

      };
    if(gpn->soldiers > 0L)
      {
      if(gpn->morale < -5)
        {
        deserters =    gpn->soldiers/(rnd(10) + 1);
        printf("%ld troops of soldiers have deserted.\n",deserters);
        gpn->soldiers -=  deserters;
        if( gpn->soldiers < 0 )
          {
          gpn->soldiers = 0;
          gpn->morale   = 0;
          };
        };

      };
    pval =    (float)(gpn->harmony) * 0.01;
    if(pval < -0.25)
      {
      pval =    -0.25;
      }
    else if(pval >  0.0)
      {
      pval =    0.25;
      };
    if(( ptot + gtot ) < (gpn->population >>3)) pval += 0.35;
    if( (gpn->population)/(ptot+gtot) > 100 )   pval -= 0.35;
    pop_increase = rnd(gpn->population/5 + 2 );
    if(pval <   0.0)
      {
      pop_increase  = (int) ( (float)(pop_increase >> 2) * pval );
      }
    else pop_increase *= pval;
    if( pop_increase > 0 )
      {
      printf("Your population grows by %ld million\n",pop_increase);
      }
    else if( pop_increase < 0 )
      {
      printf("Your population is unhappy and shrinks by %ld million\n",-1 * pop_increase);
      };
    gpn->population += pop_increase;
    pop_migration =  (int)( (float)(rnd(gpn->population/10+2)) * pval);
    gpn->population += pop_migration;
    if(pop_migration  > 0)
      {
      printf("Good times have attracted %ld million to your banner.\n",pop_migration);
      }
    else if(pop_migration  < 0)
      {
      printf("You people are unhappy and %ld million leave your rule.\n",(pop_migration*-1));
      };
    if( pop_increase + pop_migration == 0 )
      {
      printf("There is no change in your population\n");
      };
    strife(gpn->harmony);
    savefiles();
    showplayers(0);
    Hint();
    if( gpn->moves ==  0)
      {
      printf("Sorry, you are out of turns for today...\n");
      goto gameover;

      }
    if( boolean("Do you wish to continue?",1) == 2)break;

    };
  gameover:
  if(gpn->plague < -10)gpn->plague = -5;
  switch(gpn->moves)
    {
    case 0: printf("You are out of turns for today.\n");
    break;
    case 1: printf("You have one turn remaining for today.\n");
    break;
    default:
    printf("You have %ld turns remaining for today.\n",gpn->moves);
    break;

    }
  team_round();
  send_messages();
  scores();
  savefiles();
  end();

  }
void showstats()
  {
  printf("\n");
  printf("\n%s%s's Empire \n",ansii[2],gpn->pname);
  if(gpn->score<0) gpn->score =  0;
  printf("%s----------------------------------------------------------------%s\n",ansii[6],ansii[0]);
  printf("%sScore:        %ld  ",ansii[2],gpn->score);
  printf("Turns:        %ld\n",gpn->moves);
  printf("Treasury:     %ld Credits  ",gpn->credits);
  printf("Population:   %ld Million (%s%s%s)\n"
  ,gpn->population,ansii[2],harmony_level(2,gpn->harmony),ansii[2]);
  printf("Food:         %ld Kilotons\n",gpn->food);
  printf("Agents:       %ld  Troop Status: %s%s%s\n"
  ,gpn->agents,ansii[3],harmony_level(1,gpn->morale),ansii[2]);
  printf("%sMilitary:     [       Troops =  %ld] [      Fighters =  %ld]\n",ansii[1],gpn->soldiers,gpn->fighters);
  printf("              [Def. Stations =  %ld] [Heavy Cruisers =  %ld]\n",gpn->defense,gpn->cruisers);
  printf("              [     Carriers =  %ld]\n",gpn->carriers);
  printf("Planets:      [Agricultural =  %ld] [   Mining =  %ld]\n",gpn->foodp,gpn->orep);
  printf("              [     Defense =  %ld] [Governing =  %ld]\n",gpn->defensep,gpn->governp);
  printf("%s----------------------------------------------------------------%s\n",ansii[6],ansii[0]);
  if(gpn->protection > 0)
    {
    printf("You have %ld turn(s) of protection remaining.",gpn->protection);

    }
  printf("\n");

  }
int Do_Break()
  {
  printf("\n ^C - ***Break***\n ");
  savefiles();
  return(1);

  }

long you_give(mstring,mneed)
char mstring[];
long mneed;
  {
  char string[80];
  long tmpn;
  long mavailable;
  mavailable =  mneed;
  getmoney:
  if(gpn->credits < mneed) mavailable =  gpn->credits;
  sprintf(string,"%s requires %ld credits.:",mstring,mneed);
  tmpn = Request_Data(string,mavailable,gpn->credits);
  /*  */
  /* check range to make sure the user does not do something stupid! */
  /*  */
  if( (mneed * 9)/10 > tmpn || tmpn > (mneed *11)/10 )
    {
    if( boolean("Are you sure that is what you want to give?",2)==2)goto getmoney;

    };
  return tmpn;

  }
void giveplanets(amount)
long amount;
  {
  char ostring[80],c, *s;
  long amt;
  int p_i;
  p_i =    (int)amount;
  if( p_i > 100 )p_i =    100;
  if( p_i <   0 )p_i =    100;
  pop_increase =    pop_increase + p_i;
  while(amount !=  0)
    {
    printf("\nAgriculture: %ld, Ore mining: %ld, Defense: %ld",
      gpn->foodp, gpn->orep, gpn->defensep);
    printf("\nYou can take %ld more planets.",amount);
    hotkey("\n[A]gricultural, [O]re mining, or [D]efense? ",ostring);
    c = ostring[0];
    if( c != 'A' && c != 'O' && c != 'D' )continue;
    switch( c )
      {
      case 'A': s = "Agricultural"; break;
      case 'O': s = "Ore mining"; break;
      case 'D': s = "Defense"; break;

      }
    sprintf(ostring,"How many %s planets do you want? ",s);
    amt = Request_Data(ostring,amount,amount);
    amount -=  amt;
    switch( c )
      {
      case 'A': gpn->foodp    +=  amt; break;
      case 'O': gpn->orep     +=  amt; break;
      case 'D': gpn->defensep +=  amt; break;

      };

    };

  }
void buyitem(n)
int n;
  {
  long hfactor,price;
  long amount;
  long maxval;
  long ellig;
  switch(n)
    {
    case 1: printf("Soldier troop\n");     break;
    case 2: printf("Fighter craft\n");     break;
    case 3: printf("Defense station\n");   break;
    case 4: printf("Heavy cruiser\n");     break;
    case 5: printf("Intelligence agent\n");break;
    case 6: printf("Carrier\t\n");         break;
    case 7: printf("Generic planet\t\n");  break;
    case 8: printf("Governing planet\n");  break;
    default : printf("Unimplemented function!\n");
    return;
    }
  price = Item[n-1].bprice;
  if(price > gpn->credits)
    {
    printf("You don't have enough credits to buy even one!\n");
    return;

    }
  buyagain:
  printf("\n");
  maxval =  (gpn->credits/price);
  if( n == 7 || n == 8 )
    {
    if( maxval > maxplanets) maxval = maxplanets;
    };
  if(n ==  1)
    {
    ellig = (gpn->population * 1000) - gpn->soldiers;
    printf("There are currently %ld eligible men in your population\n",ellig);
    ellig =  ( ellig / 150 ) - maxdraft;
    if( ellig < 0 ) ellig = 0;
    printf("You may draft up to %ld additional troops.\n",ellig);
    if(ellig < maxval) maxval =  ellig;

    }
  amount = Request_Data("how many do you wish",0,maxval);
  if(amount <= 0) return;
  gpn->credits  -=  price * amount;
  switch(n)
    {
    case 1:
    hfactor = (gpn->morale * gpn->soldiers);
    gpn->soldiers +=  amount;
    gpn->morale = hfactor/( gpn->soldiers + 1);
    maxdraft += amount;
    break;
    case 2: gpn->fighters +=  amount;
    break;
    case 3: gpn->defense  +=  amount;
    break;
    case 4: gpn->cruisers +=  amount;
    break;
    case 5: gpn->agents   +=  amount;
    break;
    case 6: gpn->carriers +=  amount;
    break;
    case 7: giveplanets(amount);
    maxplanets -=amount;
    break;
    case 8: gpn->governp  +=  amount;
    maxplanets -=amount;
    break;

    }

  }
void buying_stage()
  {
  char ostring[80];
  int n,i,m, amt;
  long totplanets;
  do
    {
    totplanets =  gpn->orep+gpn->foodp+gpn->defensep;
    pvalue =  21000 - rnd(2000) +(500L*(totplanets));  /* adjust with each purchase! */
    Item[6].bprice = pvalue;             /* generic planets price */
    Item[6].sprice =  10000;
    Item[7].bprice = ( pvalue * 3 )/ 2;  /* government planets price */
    Item[7].sprice =  15000;
    Item[0].owned  = gpn->soldiers;
    Item[1].owned  = gpn->fighters;
    Item[2].owned  = gpn->defense;
    Item[3].owned  = gpn->cruisers;
    Item[4].owned  = gpn->agents;
    Item[5].owned  = gpn->carriers;
    Item[6].owned  = totplanets;
    Item[7].owned  = gpn->governp;
    printf("\nGovernment spending\n");
    printf("   Item                      P r i c e         Can     You\n");
    printf("#  Name                  Purchase Selling      Buy     Own\n");
    printf("----------------------------------------------------------\n");
    m = ( maxplanets > 0 ) ? 8 : 6;
    for( i=0; i < m;i++)
      {
      amt = gpn->credits / Item[i].bprice;
      if(i ==  1)
       {
       long ellig;
       ellig = (gpn->population * 1000) - gpn->soldiers;
       ellig =  ( ellig / 150 ) - maxdraft;
       if( ellig < 0 ) ellig = 0;
       if( amt > ellig ) amt = ellig;
       };
      if( i > 5 && amt > maxplanets) amt = maxplanets;
      printf("%c) %s %8ld   %6ld  %6ld  %6ld\n"
      ,Item[i].code,Item[i].name,Item[i].bprice,Item[i].sprice,amt,Item[i].owned);
      };
    printf("----------------------------------------------------------\n");
    printf("You have %ld credits and %ld turns left.\n",gpn->credits,gpn->moves);
    hotkey("Which would you like to buy? (S to Sell) ",ostring);
    switch (ostring[0])
      {
      case 'S':
      case 's': sellitem();break;
      case ' ': n = 0;break;
      case '?': n = 1;break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6': n = ostring[0] - '0'; buyitem(n); break;
      case '7':
      case '8':
       if( maxplanets > 0 )
         {
         n = ostring[0] - '0'; buyitem(n); break;
         }
       else
         {
         printf("You have already reached your limit on planets\n");
         };
      default:
      printf("That is not one of your choices, please try again\n");
      n = 1;

      };

    }
  while (n!=  0);
  printf("\n");

  }
void makefiles()
  {
  int x;
  char ostring[80];
  Quick_Save = FALSE;
  unlink(newsfile);
  unlink(oldnewsfile);
  for(x =  0; x < PLAYERS; x++)
    {
    player[x].team_apply = 0;
    strcpy(player[x].name,"X");
    strcpy(team[x].name,"X");
    sprintf(ostring,"%sMessages%ld.TXT",spcpath,pn);
    unlink(ostring);
    };
  savefiles();
  optlog("The Spacer's Digest. There is no old news",oldnewsfile);
  optlog("The Spacer's Digest.  \"The paper that brings it home!\"",newsfile);

  }
void savefiles()
  {
  BPTR fd;
  long i;
  char ostring[80];
  char ebuf[80];
  if( Quick_Save )return;
  sprintf(ostring,"%sSpcPlayer.DAT",spcpath);
  fd = Open(ostring,MODE_NEWFILE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Put_Record(fd,i,sizeof(struct Player),(char *)&player[i]);
    Close(fd);

    }
  else
    {
    sprintf(ebuf,"Unable to Create %s (err:%ld)",ostring,IoErr());
    System_Error(ebuf,ostring);

    };
  sprintf(ostring,"%sTeams.DAT",spcpath);
  fd = Open(ostring,MODE_NEWFILE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Put_Record(fd,i,sizeof(struct Team),(char *)&team[i]);
    Close(fd);

    }
  else
    {
    sprintf(ebuf,"Unable to Create %s (err:%ld)",ostring,IoErr());
    System_Error(ebuf,ostring);

    };

  }
void scores()
  {
  struct Player *pc;
  int cou, totp;
  FILE *fp;
  struct tm *tp;
  long t;
  reset();   /* recompute the scores */
  time(&t);
  tp = localtime(&t);
  fp =  fopen(scorefile,"w");
  fprintf(fp,"Space Empire V%04.4s list of scores: %s",VERSION,asctime(tp));
  fputs(" --------------------------------------------\n",fp);
  fputs(" Name                  Planets          Score\n",fp);
  fputs(" ----                  -------       --------\n",fp);
  for(cou =  0; cou<(PLAYERS+1); cou++)
    {
    pc   = &player[cou];
    totp = pc->orep + pc->defensep + pc->foodp + pc->governp;
    if((strcmp("X",pc->name))!=  0 && totp > 0 )
      {
      fprintf(fp," %-20s  %7ld       %8ld\n",pc->pname,totp,pc->score);

      }

    }
  fputs("\n",fp);

  if((cou =lord()) ==  -1)
    {
    fputs("\nThere is still no emperor powerful enough to be Space Lord.\n\n",fp);

    }
  else
    {
    if((player[cou].space_lord_days)<1)
      {
      fprintf(fp,"\n%s has ascended the iridium throne and is the new Lord of\n",player[cou].pname);
      fputs("the Galaxy.  Please notify your sysop to reset the game.\n\n",fp);

      }
    else
      {
      fprintf(fp,"\nThe current Space Lord is %s.  He has %ld days remaining before\n",player[cou].pname,player[cou].space_lord_days);
      fputs(" he may ascend the iridium throne!\n\n",fp);

      }

    }
  fclose(fp);

  }
int lord() /* Returns the number of the space lord */
  {
  int z;
  long high_score;
  int sl;
  struct Player *pz;
  high_score =  0;
  sl =  -1;
  for(z =  1; z <  PLAYERS; z++)
    {
    pz = &player[z];
    if(pz->score > MAXSCORE && strcmp(pz->name,"X") != 0 )
      {
      if( pz->score > high_score)
        {
        high_score =  pz->score;
        sl = z;

        };

      };

    };
  return sl;

  }
void readfiles()
  {
  BPTR fd;
  long i;
  char ostring[80];
  char ebuf[80];
  sprintf(ostring,"%sSpcPlayer.DAT",spcpath);
  fd = Open(ostring,MODE_OLDFILE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Get_Record(fd,i,sizeof(struct Player),(char *)&player[i]);
    Close(fd);

    }
  else
    {
    sprintf(ebuf,"Unable to read %s(err:%ld)",ostring,IoErr());
    System_Error(ebuf,ostring);

    };
  sprintf(ostring,"%sTeams.DAT",spcpath);
  fd = Open(ostring,MODE_OLDFILE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Get_Record(fd,i,sizeof(struct Team),(char *)&team[i]);
    Close(fd);

    }
  else
    {
    sprintf(ebuf,"Unable to read %s(err:%ld)",ostring,IoErr());
    System_Error(ebuf,ostring);

    };
  Quick_Save = FALSE;
  }
void reset() /* turn  maintenance  */
  {
  int z;
  long score;
  long cs, fs;
  struct Player *pz;
  for(z =  1; z < PLAYERS; z++)
    {
    pz = &player[z];
    /* Calculate scores */
    if( strcmp(pz->name,"X") == 0 )
      {
      score = 0;

      }
    else
      {
      score = 80 * (pz->defensep + pz->orep + pz->foodp + pz->governp);
      score += ( pz->population ) /  100;
      score += ( pz->credits    ) /10000;
      score += ( pz->soldiers + pz->team_soldiers);
      score += ( pz->cruisers + pz->team_cruisers) * 3;
      score +=   pz->team_defense;
      cs     =   pz->carriers;
      fs     =   pz->fighters;
      if( cs* 100 > fs)cs = (fs+99)/100;
      if( (fs+99)/100 > cs )fs = cs * 100;
      score +=   (cs * fs )>>1;  /* lower bonus for ftrs/carriers */
      score +=   pz->defense;
      score +=   pz->agents  * 2;
      score +=   pz->carriers * 3;
      score +=   pz->score_modifier;
      if(score < 0) pz->score =  0;

      };
    pz->score =    score;

    }

  }
void player_msg(to,from,msg,string)
int to,from,msg;
char string[];
  {
  char ostring[80],zstring[80];
  struct Player *pf, *pt;
  pt = &player[to];
  pf = &player[from];
  sprintf(zstring,"%sMessages%ld.TXT",spcpath,to);
  switch(msg)
    {
    case 1: sprintf(ostring,"%s defeated you!",pf->pname);
    break;
    case 2:	sprintf(ostring,"You repelled an attack from %s!",pf->pname);
    break;
    case 5: sprintf(ostring,"%s sent covert agents to your empire!",pf->pname);
    break;
    case 7: sprintf(ostring,"%s accepted your application.",pf->pname);
    break;
    case 10:sprintf(ostring,"%s rejected your application.",pf->pname);
    break;
    case 11:sprintf(ostring,"%s: %s",pf->pname,string);
    break;
    case 12:sprintf(ostring,"You were expelled from the %s alliance.",team[from].name);
    break;
    case 14: sprintf(ostring,"You have detected a missile launched at your empire!!\n");
    break;
    default: sprintf(ostring," Player:%s(%ld) (%ld) (%ld) System Error",pt->name,to,from,msg);

    }
  optlog(ostring,zstring);

  }
void news(to,from,msg)
int to,from,msg;
  {
  char ostring[80];
  struct Player *pt,*pf;
  pt = &player[to];
  pf = &player[from];
  switch(msg)
    {
    case 1: sprintf(ostring," %s defeated %s!",pf->pname,pt->pname);
    break;
    case 2: sprintf(ostring," %s repelled an attack from %s!",pt->pname,pf->pname);
    break;
    case 3: sprintf(ostring," Obituary: %s crushed by %s!",pt->pname,pf->pname);
    break;
    case 4: sprintf(ostring," %s was caught spying on %s!",pt->pname,pf->pname);
    break;
    case 5: sprintf(ostring," %s was caught spying on %s!",pt->pname,pf->pname);
    break;
    case 6: sprintf(ostring," %s was torn in half by civil war!",pf->pname);
    break;
    case 7: sprintf(ostring," %s joined the %s alliance.",pt->pname,team[from].name);
    break;
    case 8: sprintf(ostring," %s withdrew from the %s alliance.",pf->pname,team[to].name);
    break;
    case 9: sprintf(ostring," The %s alliance was dissolved.",team[to].name);
    break;
    case 10:sprintf(ostring," %s has excessive government costs!",pf->pname);
    break;
    case 11:sprintf(ostring," Income lost to %s lack of government control!",pf->pname);
    break;
    case 12:sprintf(ostring," %s was expelled from the %s alliance.",pt->pname,team[from].name);
    break;
    case 13:sprintf(ostring," %s played today!",pt->pname);
    break;
    case 14:sprintf(ostring," %s has left the federation of planet's protection",pt->pname);
    break;
    case 15:sprintf(ostring," The %s has contracted a strange plague.",pt->pname);
    break;
    case 16:sprintf(ostring," Plague is rampant for %s",pt->pname);
    break;
    case 17:sprintf(ostring," Doctors report a cure is close for %s's plague",pt->pname);
    break;
    case 18:sprintf(ostring," Doctors rejoicing the plague is cured for %s",pt->pname);
    break;
    case 19:sprintf(ostring," Obituary: %s died after a long illness",pt->pname);
    break;
    case 20:sprintf(ostring," %s is missing in action....",pt->pname);
    break;
    case 21:
    Random_News();
    sprintf(ostring," %s",rnews);
    break;
    case 22: sprintf(ostring,"A Missile was launched at %s",pt->pname);
    break;
    case 23: sprintf(ostring,"%s has abdicated their thrown.",pt->pname);
    break;
    default: sprintf(ostring," Player:%s(%ld) (%ld) (%ld) System Error",pt->name,to,from,msg);

    };
  optlog(ostring,newsfile);

  }
void covert_stage()
  {
  char ostring[80];
  int chance;
  int z,q,r,code;
  int price;
  int spysend;
  int cmod,cmod2;
  struct Player *pz;
  q = 1;
  while(q == 1)
    {
    if(gpn->agents < 1) return;
    recov:
    if( boolean("Conduct covert activities?",2)==2)return;
    if(cov_work ==  1)
      {
      printf("\nYour covert agency refuses to work since you didn't pay them");
      printf("enough.\n");
      return;

      };
    if( gpn->governp == 0 )
      {
      printf(" Your covert agency cannot function without some government control\n");
      return;

      };
    if((z =  getplayer(2))!=  -1)
      {
      pz = &player[z];
      chance =  50;
      if(pz->agents > gpn->agents)
        {
        cmod2 =    (int)(( (double)(gpn->agents+1)/(double)(pz->agents+1)  )*20.0);
        chance =    chance - cmod2;

        }
      else
        {
        cmod2 =    (int)(( (double)(pz->agents+1)/(double)(gpn->agents+1)  )*20.0);
        chance =    chance + cmod2;

        }
      remis:
      printf("\nMission types:\n");
      printf(" 1.  Spy on military.........5000\n");
      printf(" 2.  Spy on government.......5000\n");
      printf(" 3.  Spy on Both............12000\n");
      printf(" 4.  Insurection Aide......100000\n");
      printf(" 5.  Demoralize Troops.....110000\n");
      printf(" 6.  Demoralize Empire.....220000\n");
      printf(" 7.  Steal Credits.........150000\n");
      printf(" 8.  Steal Food............150000\n");
      printf(" 9.  Setup.................200000\n");
      printf(" A.  Biological Warfare....500000\n");
      printf(" B.  Spy on Alliance........20000\n");
      printf("Return to quit covert actions\n");
      pprompt("Choice: ",ostring,19);
      if( ostring[0] == ' ' )return;
      if(gpn->protection > 0  &&
        ( *ostring != '1' && *ostring != '2'  && *ostring != '3') )
        {
        if(boolean("You are under protection.  Do you wish to void it? ",2) == 2)goto remis;
        gpn->protection = 0;
        }
      switch(ostring[0])
        {
        case  '1':
        case  '2': cmod =  5; price =   5000; break;
        case  '3': cmod =  0; price =  12000; break;
        case  '4': cmod = -5; price = 100000; break;
        case  '5': cmod = -6; price = 110000; break;
        case  '6': cmod = -4; price = 220000; break;
        case  '7':
        case  '8': cmod = -9; price = 150000; break;
        case  '9': cmod =-15; price = 200000; break;
        case  'a':
        case  'A': cmod =-25; price = 500000; break;
        case  'b':
        case  'B': cmod =-10; price =  20000; break;
        default:
        printf("That is not one of your choices\n");
        goto remis;
        }
        switch(ostring[0])
          {
          case 'a' :
          case 'A' : code = 9;break;
          case 'b' :
          case 'B' : code = 10;break;
          default: code = ostring[0] - '1';
          };

      chance =    chance + cmod + cov_eff;
      if(chance < 10) chance =  10;
      if(chance > 80) chance =  80;
      printf("\nYou have a base %ld%% chance of success with %s.\n",chance,pz->pname);
      printf("For every additional agent you send, your chance increases 1% to a maximum\n");
      printf("of 90%.  If the player has covert agents, some will be assigned\n");
      printf("to counter-intellegence, there is always a chance they will stop you\n");
      spysend = Request_Data("Send how many agents",gpn->agents,gpn->agents);
      if(gpn->credits < price )
        {
        printf("You do not have enough credits to pay for that operation\n");
        goto remis;
        };
      if(spysend < 1)
        {
        goto recov;

        }
      gpn->credits -= price;
      chance =    chance + spysend;
      if(chance > 90) chance =  90;
      r = pz->agents/20;
      if( pz->agents > 0  && pz->governp > 0) chance -= r;
      if( r > spysend ) r = spysend;
      pz->I_used += r;
      pz->I_used -= r;
      if(chance <  10 ) chance =  10;
/*
** if a player is under protection you automatically fail
** Also you have one chance in 1000 you will fail the same way
*/
      if(pz->protection > 0 || rnd(1000) < 5 )chance = -1;
      if( (rnd(100)) < chance )
        {
        printf("\nMission accomplished!\n");
        spy_show(z,code);
        gpn->score_modifier =    gpn->score_modifier + 10;
        gpn->I_used += spysend;
        }
      else
        {
        if(chance == -1)
          {
          printf("Galactic Intellegence Forces arrest your agents\n");
          }
        else
          {
          printf("You were caught!  All the agents you sent were executed!\n");

          };
        Intelligence(pn,z,code,spysend); /* inform the user attacked */
        gpn->harmony -=      1;
        news(pn,z,5);   /* all spys caught! */
        player_msg(z,pn,5,"Bad News!");
        gpn->score_modifier -=   5;

        };
      gpn->agents  -=  spysend;

      }

    }

  }
void spy_show(playn,type)
int playn;
int type;
  {
  int z;
  char ostring[80];
  struct Player *pp;
  pp = &player[playn];
  type++;
  if((type ==  1)||(type ==  3))
    {
    printf("[       Troops =  %ld] [      Fighters =  %ld]\n",pp->soldiers,pp->fighters);
    printf("[Def. Stations =  %ld] [Heavy Cruisers =  %ld]\n",pp->defense,pp->cruisers);
    printf("[     Carriers =  %ld]\n",pp->carriers);
    printf("Their army exhibits %s morale.\n",harmony_level(1,pp->morale));

    };
  if(type ==  2 || type ==  3)
    {
    printf("\n%s:",pp->pname);
    printf("Treasury:     %ld Credits\n",pp->credits);
    printf("Population:   %ld Million (%s%s%s)\n",gpn->population
    ,ansii[6],harmony_level(2,pp->harmony),ansii[0]);
    printf("Food:         %ld Kilotons\n",pp->food);
    printf("Agents:       %ld\n",pp->agents);
    printf("[Agricultural =  %ld] [   Mining =  %ld]\n",pp->foodp,pp->orep);
    printf("[     Defense =  %ld] [Governing =  %ld]\n",pp->defensep,pp->governp);
    if( pp->plague > 0)
      {
      printf("The population seems to be suffering from a plague infection.\n");
      switch (pp->plague)
        {
        case 0:
        case 1:
        case 2:  printf("The infection is not serious\n");break;
        case 3:
        case 4:
        case 5:  printf("It looks like a major infection\n");break;
        default: printf("A deadly disease is attacking this empire\n");
        };
      };
    };
  if(type == 4 || type == 6)
    {
    pp->harmony -= 5;
    }
  else if(type == 5 || type == 6)
    {
    pp->morale  -= 5;
    }
  else if(type == 7)
    {
    gpn->credits += (type = pp->credits/10 + 1);
    pp->credits  -= pp->credits/10 + 1;
    if( pp->credits < 0 ) pp->credits = 0;
    printf("Your agents manage to steal %ld credits\n",type);
    }
  else if(type == 8)
    {
    gpn->food += (type = pp->food/10 + 1);
    pp->food  -= pp->food/10 + 1;
    if( pp->food < 0 ) pp->food = 0;
    printf("Your agents manage to steal %ld food\n",type);
    }
  else if(type == 9)
    {
    printf("Who do you wish to blame?\n");
    if((z =  getplayer(2))==  -1)z = pn;
    player_msg(playn,z,1, "Bad News!");
    news(playn,z,1);

    }
  else if( type == 10 )
    {
    printf("Your agents infiltrate and spread a bioengineered virus\n");
    news(playn,z,15);
    pp->plague += 4;
    if( pp->plague > 8)
      {
      pp->plague = 8;
      printf("It looks like the plague is at it's maximum effect\n");
      };
    }
  else  if( type == 11 )
    {
    relist:
    list_alliances();
    pprompt("\nWhich alliance do you wish to spy on? ",ostring,4);
    sscanf(ostring,"%ld",&z);
    if( z < 1 ||  z > PLAYERS ) goto relist;
    if(player[z].team_num != z)
      {
      printf("\nThat is not an alliance.\n");
      goto relist;

      }
    else show_team(z);
    };
  }
void list_alliances()
  {
  int x,count,i;
  printf("\nNum Name                  Leader                Members\n");
  printf("--- ----                  ------                -------\n");
  for(x =  1; x <  PLAYERS; x++)
    {
    if(player[x].team_num == x)
      {
      for(count=0,i=1; i< PLAYERS; i++)if( player[i].team_num == x)count++;
      printf("%2d. %-20s  %-20s  %ld\n",x,team[x].name,player[x].pname,count);

      }

    }
  printf("\n");

  }
void show_team(tn)
int tn;
  {
  struct Player *pp;
  int ts,tc,td,an;
  char *tcc = "Totals for Alliance ";
  ts = tc = td = 0;
  printf("\nThe %s Alliance Leader: %s\n",team[tn].name,player[tn].pname);
  printf(" Member Name         Soldiers Defense Stations Cruisers\n");
  for (an = 1; an < PLAYERS; an++)
    {
    pp = &player[an];
    if( tn == 0 || pp->team_num == tn)
      {
      printf("%-20s %8d     %8d    %8d\n",pp->pname,pp->team_soldiers,pp->team_defense,pp->team_cruisers);
      ts += pp->team_soldiers;
      td += pp->team_defense;
      tc += pp->team_cruisers;
      };
    };
  printf("\n%-20s %8d     %8d    %8d\n",tcc,ts,td,tc);

  }
void team_round()
  {
  char ostring[80];
  int x;
  long putin;
  if( boolean("Conduct Alliance activities?",2)==2)return;
  printf("\nAlliance round\n-------- -----\n");
  if(gpn->team_num ==  0)
    {
    printf("\nYou are not a member of any alliance.\n");

    }
  else
    {
    if(pn!=  gpn->team_num)
      {
      printf("\nYou are a member of the %s alliance.\n",team[gpn->team_num].name);

      }
    else
      {
      printf("\nYou are the leader of the %s alliance.\n",team[gpn->team_num].name);
      for(x =  1; x <  PLAYERS; x++)
        {
        if(player[x].team_apply ==  pn && strcmp("X",player[x].name) !=  0)
          {
          player[x].team_apply =    0;
          printf("%s would like to join your alliance.\n",player[x].pname);
          if((boolean("\nLet him join?",2) ==  1))
            {
            player[x].team_num =    pn;
            player_msg(x,pn,7,"Diplomatic Inquiry");
            news(x,pn,7);

            }
          else
            {
            player_msg(x,pn,10,"Diplomatic Inquiry");

            }

          }

        }

      }

    }
  choices:
  printf("\n");
  if( (gpn->team_num!=  0) && (gpn->team_num ==  pn) )
    {
    printf("[D]issolve alliance\n");
    printf("[E]xpell empire from alliance\n");

    }
  if(gpn->team_num!=  0)
  printf("[I]nfo on your alliance\n");
  if(gpn->team_num ==  0)
  printf("[J]oin an alliance\n");
  printf("[L]ist existing alliances\n");
  printf("[Q]uit\n");
  if(gpn->team_num!= 0)printf("[R]eview defense forces\n");
  if(gpn->team_num== 0)printf("[S]tart a new alliance\n");
  if(gpn->team_num!= 0 && gpn->team_num!= pn)printf("[W]ithdraw from alliance\n");
  printf("\n");
  hotkey("Choice: ",ostring);
  switch(ostring[0])
    {
    case 'D':
    if(!( (gpn->team_num!=  0) && (gpn->team_num ==  pn) )) goto choices;
    printf("Dissolve alliance\n");
    if((boolean("Are you sure?",2) ==  1))
      {
      dissolve_team(pn);

      }
    break;
    case 'E':
    if(!( (gpn->team_num!=  0) && (gpn->team_num ==  pn) )) goto choices;
    printf("Expell empire from alliance");
    x =  getplayer(1);
    if(x ==  -1) break;
    if((boolean("Are you sure?",2) ==  1))
      {
      news(x,pn,12);
      player_msg(x,pn,12,"Diplomatic Setback");
      player[x].team_num =  0;
      player[x].team_apply =  0;
      player[x].defense  += player[x].team_defense;
      player[x].soldiers += player[x].team_soldiers;
      player[x].cruisers += player[x].team_cruisers;
      player[x].team_defense  = 0;
      player[x].team_soldiers = 0;
      player[x].team_cruisers = 0;

      }
    break;
    case 'I':
    if(gpn->team_num ==  0) goto choices;
    printf("Info on your alliance");
    show_team(gpn->team_num);
    break;
    case 'J':
    if(gpn->team_num!=  0) goto choices;
    printf("Join an alliance\n");
    if(gpn->team_apply!=  0)
      {
      printf("You have already applied to join an alliance.\n");
      if((boolean("Revoke your application?",2) ==  2)) goto choices;

      }
    relist:
    list_alliances();
    pprompt("\nWhich alliance do you wish to join? ",ostring,4);
    sscanf(ostring,"%ld",&x);
    if( x < 1 ||  x > PLAYERS ) break;
    if(player[x].team_num != x )
      {
      printf("\nThat is not an alliance.\n");
      goto relist;

      };
    gpn->team_apply =  x;
    printf("\nThe leader of the alliance will review your application.\n");
    break;
    case 'L':
    printf("List existing alliances");
    list_alliances();
    break;
    case 'Q': return; break;
    case 'R':
    if(gpn->team_num ==  0) goto choices;
    printf("Review defense forces");
    show_team(gpn->team_num);
    gpn->soldiers                     += gpn->team_soldiers;
    gpn->team_soldiers                 = 0;
    gpn->defense                      += gpn->team_defense;
    gpn->team_defense                  = 0;
    gpn->cruisers                     += gpn->team_cruisers;
    gpn->team_cruisers                 = 0;
    printf("\n");
    putin = Request_Data("How many of your soldier troops will you use?",gpn->soldiers,gpn->soldiers);
    gpn->team_soldiers                 =  putin;
    gpn->soldiers                     -=  putin;
    putin = Request_Data("How many of your defense stations will you use?",gpn->defense,gpn->defense);
    gpn->team_defense                 =  putin;
    gpn->defense                     -=  putin;
    putin = Request_Data("How many of your cruisers will you use?",gpn->cruisers,gpn->cruisers);
    gpn->team_cruisers                 =  putin;
    gpn->cruisers                     -=  putin;
    printf("\n");
    break;
    case 'S':
    if(gpn->team_num !=  0)
      {
      printf("You cannot start a new alliance, you are already part of one!\n");
      goto choices;

      };
    printf("Start new alliance");
    pprompt("\nWhat shall you call your alliance? ",team[pn].name,19);
    strtoup(team[pn].name);
    strip_crlf(team[pn].name);
    gpn->team_num =  pn;
    gpn->team_apply =  0;
    break;
    case 'W':
    if(!( (gpn->team_num !=  0) && (gpn->team_num !=  pn) )) goto choices;
    printf("Withdraw from alliance");
    if((boolean("Are you sure?",2) ==  1))
      {
      news(gpn->team_num,pn,8);
      gpn->team_num   =  0;
      gpn->team_apply =  0;
      gpn->defense   += gpn->team_defense;
      gpn->soldiers  += gpn->team_soldiers;
      gpn->cruisers  += gpn->team_cruisers;
      gpn->team_defense  = 0;
      gpn->team_soldiers = 0;
      gpn->team_cruisers = 0;

      }
    break;
    default: goto choices; break;

    }
  goto choices;

  }
void Reset_News()
  {
  FILE *in, *out;
  char  text[80];
  out =  fopen(oldnewsfile,"a");
  if( out == NULL )
    {
    System_Error("Reset_News","Unable to output to the logfile");

    };
  if( (in =  fopen(newsfile,"r")) ==  NULL )
    {
    printf("\nThe news file is missing, please notify your Sysop\n");
    pause();
    fclose(out);
    return;

    }
  else
    {
    while( fgets(text,79,in) !=   NULL )
      {
      text[79] =  '\0';
      strip_crlf(text);
      fprintf(out,"%s\n",text);

      };
    fclose(in);
    fclose(out);

    };
  (void)unlink(newsfile);
  optlog("The Spacer's Digest. Maintenance.",newsfile);

  }
void Daily_Maintenance()
  {
  int ptot, gtot;
  int l;
  int x;
  int coun2;
  char ostring[80];
  struct Player *px;
  printf("Performing Daily maintenance...\n");
  ptot = gtot = 0;
  for( coun2 = 1; coun2 < PLAYERS; coun2++)
    {
    if((strcmp("X",player[coun2].name)) ==  0) continue;
    px = &player[coun2];
    if( px->inactive++ > 3 )
      {
      news(coun2,coun2,20);
      if( px->protection-- == 0 )
        {
        news(coun2,coun2,14);
        px->protection = 0;
        };
      };
/* age the factors so nobody can get high and just sit on it */
/* everything moves toward zero at 3 % per day  */
    px->score_modifier = ( px->score_modifier * 97 ) / 100;
    px->harmony = ( px->harmony * 97 ) / 100;
    px->morale  = ( px->morale  * 97 ) / 100;
    ptot += px->foodp;
    gtot += ( px->foodp + px->defensep + px->governp + px->orep );
    px->lastplay  =  today;      /* mark as maint. run */
    px->moves    +=  MOVES;      /* set moves per day */
    if( px->moves > 2*MOVES) px->moves = 2*MOVES;   /* limit to 2 days */
    px->agents   += px->I_used;  /* restore agents */
    px->cruisers += px->C_used;  /* cruisers */
    px->soldiers += px->S_used;  /* soldiers */
    if( px->soldiers <= 0)px->morale = 0;
    px->fighters += px->F_used;  /* fighters */
    px->I_used = 0;
    px->S_used = 0;
    px->C_used = 0;
    px->F_used = 0;
    };
  if( ptot == 0 )ptot = 1;
  player[0].foodp =  ( ( gtot + ptot -1 ) / ptot ) ;
  if( player[0].foodp <  MINPRICE) player[0].foodp = MINPRICE;
  if( player[0].foodp > MAXPRICE) player[0].foodp =MAXPRICE;
  l = lord();
  if( l != -1 )
    {
    player[l].space_lord_days--;
    if( player[l].space_lord_days <= 0)
      {
      sprintf(ostring," %20s Has reached greatness with a score of %ld",
      player[l].pname,player[l].score);
      optlog(ostring,hiscorefile);
      if( player[l].team_num != 0 )
        {
        for(x =  1; x <  PLAYERS; x++)
          {
          if( x == l ) continue;
          px = &player[x];
          if( (px->team_num ==  player[l].team_num)  )
            {
            sprintf(ostring, " %s was on the %s team.",px->pname,
                           team[px->team_num].name);
            optlog(ostring,hiscorefile);
            };
          };
        };
      }
    else /* reset the other players */
      {
      for( x=1; x< PLAYERS;x++)
        {
        if( x != l ) player[x].space_lord_days = 7;
        };
      };
    };
  Reset_News();

  }

void sellitem()
  {
  char c,ostring[80];
  long price;
  short i;
  long amount;
  long maxval;
  hotkey("\nWhich would you like to sell? ",ostring);
  i = *ostring - '0';
  if( i > 0 && i < 9 )
    {
    price = Item[i-1].sprice;
    };
  switch(*ostring)
    {
    case '1':  maxval = gpn->soldiers;break;
    case '2':  maxval = gpn->fighters;break;
    case '3':  maxval = gpn->defense; break;
    case '4':  maxval = gpn->cruisers;break;
    case '5':  maxval = gpn->agents;  break;
    case '6':  maxval = gpn->carriers;break;
    case '7':  maxval = gpn->orep + gpn->foodp + gpn->defensep-1;break;
    case '8':  maxval = gpn->governp-1; break;
    default : printf("Unimplemented function!\n");
    return;
    break;
    }
  buyagain:
  c = *ostring;
  sprintf(ostring,"%s: sell how many?",Item[i-1].name);
  amount = Request_Data(ostring,0,maxval);
  gpn->credits  +=  price * amount;
  switch(c)
    {
    case '1': gpn->soldiers -=  amount;
    break;
    case '2': gpn->fighters -=  amount;
    break;
    case '3': gpn->defense  -=  amount;
    break;
    case '4': gpn->cruisers -=  amount;
    break;
    case '5': gpn->agents   -=  amount;
    break;
    case '6': gpn->carriers -=  amount;
    break;
    case '7': takeplanets(amount);
    break;
    case '8': gpn->governp  -=  amount;
    break;

    }

  }

void takeplanets(amount)
long amount;
  {
  char ostring[80],c;
  long amt;
  while(amount !=  0)
    {
    printf("\nYou can abandon %ld planets.",amount);
    hotkey("\n[A]gricultural, [O]re mining, or [D]efense? ",ostring);
    c = ostring[0];
    if( c != 'A' && c != 'O' && c != 'D' )continue;
    sprintf(ostring,"How many planets do you want to abandon? ");
    amt = Request_Data(ostring,amount,amount);
    switch( c )
      {
      case 'A':
      if( amt > gpn->foodp)
        {
        printf("You only abandon %ld agriculture planets\n",gpn->food);
        amt = gpn->foodp;
        };
      gpn->foodp    -=  amt; break;
      case 'O':
      if( amt > gpn->orep)
        {
        printf("You only abandon %ld ore planets\n",gpn->orep);
        amt = gpn->orep;
        };
      gpn->orep     -=  amt; break;
      case 'D':
     if( amt > gpn->defensep)
        {
        printf("You only abandon %ld defense planets\n",gpn->defensep);
        amt = gpn->defensep;
        };

      gpn->defensep -=  amt; break;

      };
    amount -=  amt;

    };

  }

void Intelligence(from,to,type, amt)
int from,to,type,amt;
  {
  char filename[80], msg[80];
  sprintf(filename,"%sMessages%ld.txt",spcpath,to);
  sprintf(msg,"From: Director of Counter Intellegence");
  optlog(msg,filename);
  sprintf(msg,"  To: %s",player[to].pname);
  optlog(msg,filename);
  sprintf(msg,"Subject: Captured Spies");
  optlog(msg,filename);
  sprintf(msg,"It is my wish to report to you that %ld agents from",amt);
  optlog(msg,filename);
  sprintf(msg,"%s were captured.  After an intensive interrogation,"
  ,player[from].pname);
  optlog(msg,filename);
  sprintf(msg,"we determined their mission was %s",mission[type]);
  optlog(msg,filename);
  }

long Compute_Planet_Limit()
  {
  long maxbuy;
  long  ctot;
  short cou;
  register struct Player *px;
  long big, small, top,bot;
  big = -1;
  small = 1000000;
  for(cou =  1; cou < PLAYERS; cou++)
    {
    px = &player[cou];
    if( strcmp("X",px->name) !=  0)
      {
      ctot = px->orep + px->defensep + px->foodp + px->governp;
      if( ctot < small)small = ctot;
      if( big  < ctot) big   = ctot;
      };
    };
  ctot = gpn->orep + gpn->defensep + gpn->foodp + gpn->governp;
  top = big - ctot ;
  bot = big - small;
  if( bot > 1 )
    {
    maxbuy = (40 * top)/bot + 10;
    if( maxbuy > 50)maxbuy = 50;
    if( maxbuy < 10)maxbuy = 10;
    }
  else maxbuy = 10;
  printf("The galactic council has allocated %ld planets for your purchase\n",maxbuy);
  return maxbuy;
  }
