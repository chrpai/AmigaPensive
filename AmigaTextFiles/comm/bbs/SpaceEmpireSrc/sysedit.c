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
#include "io.h"
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include "sysedit.h"

struct Team *team;
struct Player *player;
int pn;

void main( int, char **);
extern long DOSBase;
void main(argc,argv)
int argc;
char *argv[];
  {
  char command[5];
  int i;
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
  command[0] = ' ';
  readfiles();
  printf("\f[0;0HThis is the Sysop Editor.\n");
  while(*command != 'Q')
    {
    printf("Main Menu: Enter Command(? for menu): ");
    (void)fgets(command,5,stdin);
    strtoup(command);
    switch(*command)
      {
      case '?':
      for(i=0;mmenu[i] !=NULL;i++)printf("%s\n",mmenu[i]);
      break;
      case 'A':
      Alliance();
      break;
      case 'P':
      Player();
      break;

      }

    }
  end();

  }
void Alliance()
  {
  char cmd[5];
  int i;
  cmd[0] = ' ';
  while(*cmd != 'Q')
    {
    printf("Alliance Menu: Enter Command(? for menu): ");
    (void)fgets(cmd,5,stdin);
    strtoup(cmd);
    switch(*cmd)
      {
      case '?':
      for(i=0;amenu[i] !=NULL;i++)printf("%s\n",amenu[i]);
      break;
      case 'D':
      dissolve_team();
      break;
      case 'L':
      list_alliances();
      break;
      case 'C':
      change_alliance();
      break;

      };

    };

  }

void dissolve_team()
  {
  int x,z;
  struct Player *px, *pz;
  char cnd[5];
  cnd[0] = ' ';
  while(*cnd != 'Q')
    {
    printf("Which team(? for list, Q to quit)? ");
    (void)fgets(cnd,5,stdin);
    strtoup(cnd);
    if(*cnd == '?')list_alliances();
    else if(*cnd != 'Q')
      {
      z = atoi(cnd);
      pz = &player[z];
      if( pz->team_num != z )
        {
        printf("that is not a valid team\n");
        }
      else
        {
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
            px->team_defense = 0;
            px->team_soldiers = 0;
            px->team_cruisers = 0;
            };
          };
        };
      };

    };

  }
void showplayers()
  {
  int cou;
  printf("\n");
  printf("Name                       User\n");
  printf("----                       ----\n");
  for(cou =  0; cou<(PLAYERS+1); cou++)
    {
    if((strcmp("X",player[cou].name))!=  0) printf("%d. %s      %-26s\n",cou,player[cou].pname, player[cou].name);

    };
  printf("\n");

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
      printf("%2d. %-20s  %-20s  %d\n",x,team[x].name,player[x].pname,count);

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
  printf("The %s Alliance Leader: %s\n",team[tn].name,player[tn].pname);
  printf(" Member Name         Soldiers Defense Stations Cruisers\n");
  for (an = 1; an < PLAYERS; an++)
    {
    pp = &player[an];
    if( tn == 0 || pp->team_num == tn)
      {
      printf("%-20s %8.8d     %8.8d    %8.d\n",pp->pname,pp->team_soldiers,pp->team_defense,pp->cruisers);
      ts += pp->team_soldiers;
      td += pp->team_defense;
      tc += pp->team_cruisers;
      };
    };
  printf("\n%-20s %8.8d     %8.8d    %8.d\n",tcc,ts,td,tc);

  }
void readfiles()
  {
  BPTR fd;
  long i;
  char ostring[80];
  sprintf(ostring,"SpcPlayer.DAT");
  fd = Open(ostring,MODE_READWRITE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Get_Record(fd,i,sizeof(struct Player),(char *)&player[i]);
    Close(fd);

    }
  else
    {
    printf("Unable to Create data files:Player");

    };
  sprintf(ostring,"Teams.DAT");
  fd = Open(ostring,MODE_READWRITE);
  if( fd != NULL )
    {
    for(i=0;i < PLAYERS;i++)Get_Record(fd,i,sizeof(struct Team),(char *)&team[i]);
    Close(fd);

    }
  else
    {
    printf("Unable to Create data files:Team");

    };

  }
void change_alliance()
  {
  char  nme[19];
  char comm[5];
  comm[0] = ' ';
  while(*comm != 'Q')
    {
    printf("Which Alliance(? for list, Q to quit)? ");
    (void)fgets(comm,5,stdin);
    strtoup(comm);
    if(*comm == '?')list_alliances();
    else if(*comm != 'Q')
      {
      which = atoi(comm);
      show_team(which);
      printf("Enter new name: ");
      (void)fgets(nme,19,stdin);
      strtoup(nme);
      strip_crlf(nme);
      (void)strcpy(team[which].name,nme);

      }

    }

  }
void savefiles()
  {
  BPTR fd;
  long i;
  char ostring[80];
  sprintf(ostring,"SpcPlayer.DAT");
  fd = Open(ostring,MODE_READWRITE);
  if( fd != NULL )
    {
    printf("Writing Player files, please be patient.....");
    for(i=0;i < 26;i++)Put_Record(fd,i,sizeof(struct Player),(char *)&player[i]);
    Close(fd);
    printf("Done!\n");

    }
  else
    {
    System_Error("Unable to Create data files","Player");

    };
  sprintf(ostring,"Teams.DAT");
  fd = Open(ostring,MODE_READWRITE);
  if( fd != NULL )
    {
    printf("Writing Team files, please be patient.....");
    for(i=0;i < 26;i++)Put_Record(fd,i,sizeof(struct Team),(char *)&team[i]);
    Close(fd);
    printf("Done!\n");

    }
  else
    {
    System_Error("Unable to Create data files","Team");

    };

  }
void Player()
  {
  char cmd[5];
  int i;
  cmd[0] = ' ';
  while(*cmd != 'Q')
    {
    printf("Player Menu: Enter Command(? for menu): ");
    (void)fgets(cmd,5,stdin);
    strtoup(cmd);
    switch(*cmd)
      {
      case '?':
      for(i=0;pmenu[i] !=NULL;i++)printf("%s\n",pmenu[i]);
      break;
      case 'D':
      case 'R':
      remove_player();
      break;
      case 'L':
      showplayers();
      break;
      case 'M':
      modify_player();
      break;

      };

    };

  }
void remove_player()
  {
  char comm[5];
  comm[0] = ' ';
  while(*comm != 'Q')
    {
    printf("Which Player(? for list, Q to quit)? ");
    (void)fgets(comm,5,stdin);
    strtoup(comm);
    if(*comm == '?')showplayers();
    else if(*comm != 'Q')
      {
      which = atoi(comm);
      showstats(which);
      if( boolean("Are you sure?",2) == 1)
        {
        strcpy(player[which].name,"X");
        savefiles();
        }

      }

    }

  }
void showstats(pn)
int pn;
  {
  if(player[pn].score<0) player[pn].score =  0;
  printf("%s's Empire   Protection: %d\n",player[pn].pname,player[pn].protection);
  printf("     Score:  %ld  PS: %d       ID: %d\n",player[pn].score,player[pn].plague,player[pn].inactive);
  printf("     Turns:  %d  SLD: %d Treasury: %ld\n",player[pn].moves,player[pn].space_lord_days,player[pn].credits);
  printf("Population:  %d   %s, %d\n",player[pn].population,harmony_level(2,player[pn].harmony),player[pn].harmony);
  printf("      Food:  %ld  IA: %ld      TM: %s, %d\n",player[pn].food,player[pn].agents,harmony_level(1,player[pn].morale),player[pn].morale);
  printf(" S: %ld   F: %ld   DS: %ld   HC: %ld   C: %ld\n",player[pn].soldiers,player[pn].fighters,player[pn].defense,player[pn].cruisers,player[pn].carriers);
  printf("TS: %ld TDS: %ld  THC: %ld\n",player[pn].team_soldiers,player[pn].team_defense,player[pn].team_cruisers);
  printf("US: %ld  UF: %ld  UHC: %ld  UIA: %d\n",player[pn].S_used,player[pn].F_used,player[pn].C_used,player[pn].I_used);
  printf("FP: %ld  OP: %ld DP: %ld GP: %ld\n",player[pn].foodp,player[pn].orep,player[pn].defensep,player[pn].governp);
  }

int boolean(astring,yndef)
char astring[];
int yndef;
  {
  char ostring[80];
  int retv =  0;
  reget:
  printf(astring);
  if(yndef ==  1) printf(" [Y/n] ");
  if(yndef ==  2) printf(" [y/N] ");
  hotkey("",ostring);
  if(ostring[0] != 'Y' && ostring[0] != 'N' && ostring[0] != ' ') goto reget;
  switch(yndef)
    {
    case 1: if(ostring[0] ==  'N') retv =  2; else retv =  1; break;
    case 2: if(ostring[0] ==  'Y') retv =  1; else retv =  2; break;

    }
  return retv;

  }
void hotkey(mstring,ostring)
char mstring[];
char *ostring;
  {
  printf(mstring);
  *ostring =    getchar();
  if( *ostring != '\n')while ( getchar() !=   '\n');
  if( ostring[0] >='a' && ostring[0] <= 'z')
    {
    ostring[0] = (char) ( (int)ostring[0] - 32 );

    }
  else if( ostring[0] == '\n') ostring[0] = ' ';

  }

char *harmony_level(type,level)
int type,level;
  {
  switch (type)
    {
    case  1:    /* Morale */
    if( level >=   -20 && level <= 20 ) return "average";
    if( level >  1000) return "Elite Guards";
    if( level >   400) return "Elite Trained";
    if( level >   200) return "very high";
    if( level >    80) return "high";
    if( level >    10) return "above average";
    if( level < -1000) return "not trained";
    if( level <  -400) return "very poorly trained";
    if( level <  -200) return "very low";
    if( level <   -80) return "low";
    if( level <   -10) return "below average";
    break;
    case  2:    /* Population Harmony */
    if( level >=   -20  &&  level <= 20 ) return "Satisfied";
    if( level >  1000 ) return "Exalted";
    if( level >   400 ) return "Extremely Happy";
    if( level >   200 ) return "Very Happy";
    if( level >   100 ) return "Boisterous";
    if( level >    80 ) return "Happy";
    if( level >    40 ) return "Jolly";
    if( level >    10 ) return "Content";
    if( level < -1000 ) return "Anarchy";
    if( level < - 400 ) return "Revolting";
    if( level <  -200 ) return "Enraged";
    if( level <   -80 ) return "Restless";
    if( level <   -40 ) return "Protesting";
    if( level <   -10 ) return "Discontent";
    break;
    }

  }
void modify_player()
  {
  char nme[19];
  char comm[5];
  char commd[5];
  comm[0]  = ' ';
  commd[0] = ' ';
  while(*comm != 'Q')
    {
    printf("Which Player(? for list, Q to quit)? ");
    (void)fgets(comm,5,stdin);
    strtoup(comm);
    if(*comm == '?')showplayers();
    else if(*comm != 'Q')
      {
      which = atoi(comm);
      showstats(which);
      while(*commd != 'Q')
        {
        printf("You may modify the following:\n");
        printf(" 1.  Empire Name           2.  Player Name\n");
        printf(" 3.  Turns Left            4.  Credits\n");
        printf(" 5.  Population            6.  Harmony (People)\n");
        printf(" 7.  Military              8.  Morale (Military)\n");
        printf(" 9.  Planets              10.  Protection\n");
        printf("11.  Food                 12.  Covert Agents\n");
        printf("13.  Plague Status        14.  Inactive Days\n");
        printf(" S.  Empire Status         Q.  Quit\n");
        printf("Which one? ");
        (void)fgets(commd,5,stdin);
        strtoup(commd);
        if(*commd == 'S' || *commd == 'Q')
          {
          if(*commd == 'S')showstats(which);
          }
        else
          {
          switch(atoi(commd))
            {
            case 1:
            printf("Old name %s\nEnter new name: ",player[which].pname);
            (void)fgets(nme,19,stdin);
            strtoup(nme);
            strip_crlf(nme);
            (void)strcpy(player[which].pname,nme);
            break;
            case 2:
            printf("Old name %s\nEnter new name: ",player[which].name);
            (void)fgets(nme,19,stdin);
            strtoup(nme);
            strip_crlf(nme);
            (void)strcpy(player[which].name,nme);
            break;
            case 3:
            printf("Current turns: %d, Enter new amount: ",player[which].moves);
            (void)fgets(nme,19,stdin);
            player[which].moves = atoi(nme);
            break;
            case 4:
            printf("Current Credits: %d, Enter new amount: ",player[which].credits);
            (void)fgets(nme,19,stdin);
            player[which].credits = atoi(nme);
            break;
            case 5:
            printf("Current Population: %d million\nEnter new amount: ",player[which].population);
            (void)fgets(nme,19,stdin);
            player[which].population = atoi(nme);
            break;
            case 6:
            printf("Current Harmony: %d, Enter new amount: ",player[which].harmony);
            (void)fgets(nme,19,stdin);
            player[which].harmony = atoi(nme);
            break;
            case 7:
            military();
            break;
            case 8:
            printf("Current Morale: %d, Enter new amount: ",player[which].morale);
            (void)fgets(nme,19,stdin);
            player[which].morale = atoi(nme);
            break;
            case 9:
            planets();
            break;
            case 10:
            printf("Current Protection: %d, Enter new amount: ",player[which].protection);
            (void)fgets(nme,19,stdin);
            player[which].protection = atoi(nme);
            break;
            case 11:
            printf("Current Food: %d, Enter new amount: ",player[which].food);
            (void)fgets(nme,19,stdin);
            player[which].food = atoi(nme);
            break;
            case 12:
            printf("Current Covert Agents: %d, Enter new amount: ",player[which].agents);
            (void)fgets(nme,19,stdin);
            player[which].agents = atoi(nme);
            break;
            case 13:
            printf("Current Plague Status: %d, Enter new amount: ",player[which].plague);
            (void)fgets(nme,19,stdin);
            player[which].plague = atoi(nme);
            break;
            case 14:
            printf("Current Inactive Days: %d, Enter new amount: ",player[which].inactive);
            (void)fgets(nme,19,stdin);
            player[which].inactive = atoi(nme);
            break;
            }

          }

        }

      }

    }

  }
void military()
  {
  char c[3];
  char nam[19];
  c[0] = ' ';
  while(*c != 'Q')
    {
    printf("Modify which:\n");
    printf(" 1.  Soldiers           2.  Fighters\n");
    printf(" 3.  Cruisers           4.  Team Troops\n");
    printf(" 5.  Team Cruisers      6.  Defense Stations\n");
    printf(" 7.  Team Defense       8.  Used Soldiers\n");
    printf(" 9.  Used Cruisers     10.  Used Fighters\n");
    printf("11.  Used Covert        Q.  Quit\n");
    printf("Which one? ");
    (void)fgets(c,3,stdin);
    strtoup(c);
    if(*c == 'Q')printf("");
    else
      {
      switch(atoi(c))
        {
        case 1:
        printf("Current Soldiers: %d, Enter new amount: ",player[which].soldiers);
        (void)fgets(nam,19,stdin);
        player[which].soldiers = atoi(nam);
        break;
        case 2:
        printf("Current Fighters: %d, Enter new amount: ",player[which].fighters);
        (void)fgets(nam,19,stdin);
        player[which].fighters = atoi(nam);
        break;
        case 3:
        printf("Current Cruisers: %d, Enter new amount: ",player[which].cruisers);
        (void)fgets(nam,19,stdin);
        player[which].cruisers = atoi(nam);
        break;
        case 4:
        printf("Current Soldiers: %d, Enter new amount: ",player[which].team_soldiers);
        (void)fgets(nam,19,stdin);
        player[which].team_soldiers = atoi(nam);
        break;
        case 5:
        printf("Current Cruisers: %d, Enter new amount: ",player[which].team_cruisers);
        (void)fgets(nam,19,stdin);
        player[which].team_cruisers = atoi(nam);
        break;
        case 6:
        printf("Current Defense Stations: %d, Enter new amount: ",player[which].defense);
        (void)fgets(nam,19,stdin);
        player[which].defense = atoi(nam);
        break;
        case 7:
        printf("Current Defense Stations: %d, Enter new amount: ",player[which].team_defense);
        (void)fgets(nam,19,stdin);
        player[which].team_defense = atoi(nam);
        break;
        case 8:
        printf("Current Used Soldiers: %d, Enter new amount: ",player[which].S_used);
        (void)fgets(nam,19,stdin);
        player[which].S_used = atoi(nam);
        break;
        case 9:
        printf("Current Used Fighters: %d, Enter new amount: ",player[which].F_used);
        (void)fgets(nam,19,stdin);
        player[which].F_used = atoi(nam);
        break;
        case 10:
        printf("Current Used Cruisers: %d, Enter new amount: ",player[which].C_used);
        (void)fgets(nam,19,stdin);
        player[which].C_used = atoi(nam);
        break;
        case 11:
        printf("Current Used Agents: %d, Enter new amount: ",player[which].I_used);
        (void)fgets(nam,19,stdin);
        player[which].I_used = atoi(nam);
        break;
        }

      }

    }

  }
void planets()
  {
  char c[5];
  char nam[19];
  c[0] = ' ';
  while(*c != 'Q')
    {
    printf("Modify which:\n");
    printf("1.  Food        2.  Ore\n");
    printf("3.  Defense     4.  government\n");
    printf("Q.  Quit\n");
    printf("Which one? ");
    (void)fgets(c,5,stdin);
    strtoup(c);
    if(*c == 'Q')printf("");
    else
      {
      switch(atoi(c))
        {
        case 1:
        printf("Current Food Planets: %d, Enter new amount: ",player[which].foodp);
        (void)fgets(nam,19,stdin);
        player[which].foodp = atoi(nam);
        break;
        case 2:
        printf("Current Ore Planets: %d, Enter new amount: ",player[which].orep);
        (void)fgets(nam,19,stdin);
        player[which].orep = atoi(nam);
        break;
        case 3:
        printf("Current Defense Planets: %d, Enter new amount: ",player[which].defensep);
        (void)fgets(nam,19,stdin);
        player[which].defensep = atoi(nam);
        break;
        case 4:
        printf("Current government Planets: %d, Enter new amount: ",player[which].governp);
        (void)fgets(nam,19,stdin);
        player[which].governp = atoi(nam);
        break;

        }

      }

    }

  }
void end()
  {
  char ans[3];
  printf("Save Files? ");
  (void)fgets(ans,2,stdin);
  strtoup(ans);
  if(*ans == 'Y')savefiles();
  exit(0);
  }
