/*
** $VER: bbgopher.c 1.0 (20.4.95)
** Copyright (c) 1995 Jason M. Weber (Amiga port)
** Copyright (c) 1993 Mark Morley (original BBGopher for UNIX)
** Freely distributable
*/

#include <netdb.h>
#include <fcntl.h>

#include "bbgopher.h"

const char *version = "$VER: BBGopherCNet 1.0 "__AMIGADATE__;

void LoadError(void);
void ShutDown(char *spawn);
void            GetOut( void );

void checkcarrier(void);
void err(char *err);
void bb_main(void);

struct MsgPort         *replyp; /* Some communication details ...       */
struct CPort           *cport;
struct CMessage        cmess;
struct MainPort        *myp;            /* Pointer to CNet port--ALL info!      */
struct PortData        *z;
char                   **bm;
struct Library         *CNetBase = NULL;
struct Library         *CNet4Base = NULL;

void main( int argc, char **argv )
{
        Forbid();
        if(argc > 1)
                cport = (struct CPort *)FindPort( argv[1] );
        Permit();

        if( argc<2 || !(cport) ) {
                printf("This is a CNet C program.\n");
                exit(0);
                }

        if( !(replyp = CreatePort( 0,0 )))
                exit(0);

        cmess.cn_Message.mn_ReplyPort   = replyp;
        cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
        cmess.cn_Message.mn_Node.ln_Name= "cstuff";

        // CNet version check
        if( cport->ack != 30 ) {        // change "30" to be compatible with only pre 4.13 systems
                cport->ack = 1;
                LoadError();
                }

        cport->ack = 0;

        z    = cport->zp;
        myp  = cport->myp;
        bm   = z->bm;

        if( !(CNetBase = OpenLibrary( "cnet.library", 4 )) ) // <- NOTE version 4 - can be changed to 3 to make compatible with older CNet releases!
                LoadError();
        if( !(CNet4Base = OpenLibrary( "cnet4.library", 4 )) )
                LoadError();

/************************************************************************
 *           End of CNet setup - YOUR CUSTOM CODE STARTS HERE           *
 ************************************************************************/

   if (!Setup())
      PutText("Warning: no configuration file found, using defaults.\n");

   if (z->npitems > 2)
   {
      strcpy(DefHost, z->pitem[1]);
      if (z->npitems == 3)
         DefPort = atoi(z->pitem[2]);
   }

   if (argc > 2)
   {
      strcpy(DefHost, argv[2]);
      if (argc > 3)
         DefPort = atoi(argv[3]);
   }

   bb_main();

        /* exit back to CNet - always call GetOut() to exit back to CNet! */
   GetOut();
}

int ValidNumber(int number, char *string)
{
   int   n1,
         n2;
   BOOL  r;

   while (*string)
   {
      r = FALSE;
      while (*string == ' ' || *string == '\t' || *string == ',')
         string++;
      n1 = 0;
      n2 = 0;
      if (*string >= '0' && *string <= '9')
      {
         n1 = atoi(string);
         while (*string >= '0' && *string <= '9')
            string++;
      }
      if (*string == '-')
      {
         r = TRUE;
         string++;
         n2 = atoi(string);
         while (*string >= '0' && *string <= '9')
            string++;
      }
      if (r)
      {
         if ((number >= n1) && (number <= n2))
            return 1;
      }
      else
      {
         if (number == n1)
           return 1;
      }
   }
   return 0;
}

int CheckUser(char type)
{
   BPTR fp;
   char line [ 80 ];
   char *uid;
   int  i;

   for (i = 0; KnownTypes[i].type; i++)
   {
      if (KnownTypes[i].type == type)
         break;
   }
   if (KnownTypes[i].type == 0)
      return 0;
   if (!(fp = Open(KnownTypes[i].acf,MODE_OLDFILE)))
      return 1;
   uid = z->user1.UUCP;
   while(FGets(fp, line, 80) && (line[0] != 26))
   {
      line[strlen(line) - 1] = 0;
      if (strcmp(line, uid) == 0)
      {
         Close(fp);
         return 1;
      }
   }
   Close(fp);
   return 0;
}

int SupportedType(char type)
{
   int i;

   for (i = 0; KnownTypes[i].type; i++)
   {
      if (KnownTypes[i].type == type)
         return KnownTypes[i].supported;
   }
   return 0;
}

int newEntry(GO4ENTRY *ent, char type, char *display, char *selector,
             char *hostname, short port)
{
   if (display && (ent->display = (char *)malloc(strlen(display) + 1)) == 0)
      return 0;
   if (selector && (ent->selector = (char *)malloc(strlen(selector) + 1)) == 0)
   {
      free(ent->display);
      return 0;
   }
   if (hostname && (ent->hostname = (char *)malloc(strlen(hostname) + 1)) == 0)
   {
      free(ent->selector);
      free(ent->display);
      return 0;
   }
   ent->type = type;
   if (display)
      strcpy(ent->display, display);
   else
      ent->display = 0;
   if (selector)
      strcpy(ent->selector, selector);
   else
      ent->selector = 0;
   if (hostname)
      strcpy(ent->hostname, hostname);
   else
      ent->hostname = 0;
   ent->port = port;
   return 1;
}

int delEntry(GO4ENTRY *ent)
{
   if (ent->hostname)
      free(ent->hostname);
   if (ent->selector)
      free(ent->selector);
   if (ent->display)
      free(ent->display);
   return 1;
}

char *TypeLookup(char type)
{
   int i;
   static char buf [ 10 ];

   for (i = 0; KnownTypes[i].type; i++)
   {
      if (KnownTypes[i].type == type)
         return KnownTypes[i].display;
   }
   sprintf(buf, " %c ", type);
   return buf;
}

int DisplayLookup(char *display)
{
   int i;

   for (i = 0; i < KnownTypes[i].type; i++)
   {
      if (!strcmp(KnownTypes[i].display, display))
         return i;
   }
   return -1;
}

int Connect(char *selector, char *hostname, short port)
{
   if (port == 0)
      port = DefPort;
   if (ValidNumber(port, BadPorts))
   {
      PutText("Sorry, but access to that service is not allowed.\n\n");
      return G_CONNECT;
   }
   if (hostname == 0)
      hostname = DefHost;
   if ((host = gethostbyname(hostname)) == 0)
      return G_HOST;
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      return G_SOCKET;
   bzero(&sain, sizeof(sain));
   sain.sin_family = AF_INET;
   sain.sin_port = htons(port);
   bcopy(host->h_addr, &sain.sin_addr, host->h_length);
   if (connect(sock, (struct sockaddr*)&sain, sizeof(sain)) < 0)
      return G_CONNECT;
   if (selector)
      write(sock, selector, strlen(selector));
   write(sock, "\r\n", 2);
   sd = fdopen(sock, "r");
   return G_OK;
}

void Disconnect(void)
{
   close(sock);
}

int Page(char *file)
{
   BPTR fp;
   char line [ 256 ];

   char ii;

   int  n = 0;

   if (!(fp = Open(file,MODE_OLDFILE)))
      return 0;
   if (ClrScreen)
      PutText("f1");
   else
      DoReturn();
   while(FGets(fp, line, 256) && (line[0] != 26))
   {
      PutText(line);
      if (++n == 22)
      {
         PutText("[MORE...]");
         *line = 0;
         n = 0;

         ii = OneKey();

         checkcarrier();
         
         if (ii == 'Q')
            break;
         DoReturn();
      }
   }
   Close(fp);
   return 1;
}

int GetData(char *file, int binary)
{
   BPTR  fp;
   int   l = 0,
         t = 0;
   char line [ 256 ];
   char ch;

   if (!(fp = Open(file,MODE_NEWFILE)))
      return G_CREATE;
   if (Twirly)
      PutText(" ");
   if (binary)
   {
      while((ch = getc(sd)) && (ch != 26))
      {
         FPutC(fp, ch);
         if (Twirly && ++l == 1024)
         {
            l = 0;
            PutText("\b");
            sprintf(z->ABuffer,"%c",TwirlyChars[t]);
            PutA();
            t++;
            if (t == NUMTWIRLYCHARS)
               t = 0;
         }
      }
   }
   else
   {
      while((fgets(line, 255, sd)) && (line[0] != 26))
      {
         l = strlen(line) - 1;
         if (line[l] == '\n' && line[l - 1] == '\r')
         {
            line[l - 1] = '\n';
            line[l] = 0;
         }
         if (*line == '.' && (line[1] == '\r' || line[1] == '\n'))
            break;
         FPrintf(fp, line);
         if (Twirly)
         {
            PutText("\b");
            sprintf(z->ABuffer,"%c",TwirlyChars[t]);
            PutA();
            t++;
            if (t == NUMTWIRLYCHARS)
               t = 0;
         }
      }
   }
   if (Twirly)
      PutText("\b \b");
   Close(fp);
   return G_OK;
}

int GetFile(char *selector, char *hostname, short port, char *type, int binary)
{
   char  line [ 81 ];
   int   rc;

   sprintf(z->ABuffer,"Local name for %s file? ",type);
   EnterLine(40,ELINE_INPUTBOX,z->ABuffer);

   strcpy(line,z->InBuffer);

   checkcarrier();

   DoReturn();
   line[strlen(line)] = 0;
   if (*line == 0)
      return G_OK;
   if ((rc = Connect(selector, hostname, port)) != G_OK)
      return rc;
   PutText("Retrieving file...");
   rc = GetData(line, binary);
   DoReturn();
   DoReturn();
   Disconnect();
   return rc;
}

int MailFile(char *filename)
{
   BPTR fi,
        fo;
   char fname [ 40 ],
        to    [ 81 ],
        sub   [ 81 ];
   char ch;

   sprintf(z->ABuffer,"To [%s]: ", z->user1.UUCP);
   EnterLine(40,ELINE_INPUTBOX,z->ABuffer);

   strcpy(to,z->InBuffer);

   checkcarrier();

   to[strlen(to)] = 0;
   if (*to == 0)
      strcpy(to, z->user1.UUCP);
   EnterLine(40,ELINE_INPUTBOX,"Subject: ");

   strcpy(sub,z->InBuffer);

   checkcarrier();

   DoReturn();
   sub[strlen(sub)] = 0;
   if (*sub == 0)
      strcpy(sub, "No Subject Specified");
   PutText("Mailing file...\n\n");
   sprintf(fname,"T:_Gopher.%d",z->InPort);
   if ((fo = Open(fname, MODE_NEWFILE)) == NULL)
      return G_CREATE;
   fi = Open(filename, MODE_OLDFILE);
   while((ch = FGetC(fi)) && (ch != -1))
      FPutC(fo, ch);
   Close(fi);
   Close(fo);
   sprintf(z->ABuffer,bm[2132],fname,z->user1.UUCP,z->user1.RealName,to,sub);
   system(z->ABuffer);
   DeleteFile(fname);
   return G_OK;
}

int SaveFile(char *filename)
{
   FILE *fi,
        *fo;
   char line [ 81 ],
        temp [ 81 ];
   char *l;
   char ch;

   EnterLine(30,ELINE_INPUTBOX,"Local name for TXT file? ");

   strcpy(line,z->InBuffer);

   checkcarrier();

   DoReturn();

   line[strlen(line)] = 0;
   if (*line == 0)
      return G_OK;
   if (*Directory)
   {
      l = (char *)rindex(line, '/');
      if (l)
         *l++ = 0;
      else
         l = line;
      *temp = 0;
      if (*Directory == '~')
      {
         sprintf(temp,"Mail:Users/%s/Files",z->user1.UUCP);
         strcat(temp, &Directory[1]);
      }
      else
         strcat(temp, Directory);
      if (temp[strlen(temp) - 1] != '/')
         strcat(temp, "/");
      strcat(temp, l);
      l = temp;
   }
   else
      l = line;
   PutText("Saving file...\n\n");
   if ((fo = fopen(l, "w")) == NULL)
      return G_CREATE;
   fi = fopen(filename, "r");
   for (ch = fgetc(fi); !feof(fi); ch = fgetc(fi))
      fputc(ch, fo);
   fclose(fi);
   fclose(fo);
   return G_OK;
}

int bb_ReadText(char *selector, char *hostname, short port)
{
   char  fname [  40 ],
         line  [ 256 ];

   int   rc;

   char  ii;

   if ((rc = Connect(selector, hostname, port)) != G_OK)
      return rc;
   PutText("Retrieving text...");
   sprintf(fname,"T:Gopher.%d",z->InPort);
   if ((rc = GetData(fname, 0)) != G_OK)
      return rc;
   DoReturn();
   Disconnect();
   if (*Pager == 0 || !strcmp(Pager, "default"))
      Page(fname);
   else
   {
      sprintf(line, Pager, fname);
      if (!strstr(Pager, "%s"))
      {
         strcat(line, " ");
         strcat(line, fname);
      }
      system(line);
   }
   PutText("\nPress ENTER to continue, M to mail, S to save: ");

   rc = G_OK;

   ii = OneKey();

   checkcarrier();

   switch(ii)
   {
      case 'M':
         PutText("Mail\n");
         rc = MailFile(fname);
         break;

      case 'S':
         PutText("Save\n");
         rc = SaveFile(fname);
         break;

      default:
         DoReturn();
         break;
   }
   DeleteFile(fname);
   return rc;
}

int DoSearch(char *selector, char *hostname, short port)
{
   char buf  [ 256 ],
        line [  81 ];
   int rc;

   EnterLine(60,ELINE_INPUTBOX,"Please enter your search criteria (you may use 'and' and 'or')\n: ");

   strcpy(line,z->InBuffer);

   checkcarrier();

   line[strlen(line)] = 0;
   if (*line == 0)
      return G_OK;
   sprintf(buf, "%s\t%s\r\n", selector, line);
   if ((rc = Connect(buf, hostname, port)) != G_OK)
      return rc;
   PutText("Searching...\n");
   rc = ReadMenu("Index Search Results", buf, hostname, port, 0, 0);
   Disconnect();
   return rc;
}

int ReadMenu(char *display, char *selector, char *hostname, short port, int connect, FILE *fd)
{
   char  line [ 1025 ];
   char  *e;
   GO4ENTRY ent;
   int   rc,
         t;

   if (connect)
   {
      if ((rc = Connect(selector, hostname, port)) != G_OK)
         return rc;
   }
   if (StackLen >= MAXSTACKLEN)
   {
      if (connect)
         Disconnect();
      return G_STACK;
   }
   if (BM)
   {
      StackLen--;
      delEntry(&Stack[StackLen]);
      StackLen--;
      BM = 0;
   }
   if (!newEntry(&Stack[StackLen], '1', display, selector, hostname, port))
   {
      if (connect)
         close(sock);
      return G_MEMORY;
   }
   StackLen++;
   MenuLen = 0;
   t = 0;
   if (Twirly)
      PutText(" ");
   if (fd == NULL)
      fd = sd;
   for (fgets(line, 1024, fd); !feof(fd); fgets(line, 1024, fd))
   {
      if (*line == '.' && (line[1] == '\r' || line[1] == '\n'))
         break;
      e = line;
      ent.type = *e++;
      ent.display = e;
      while (*e && *e != '\t')
         e++;
      if (*e)
         *e++ = 0;
      ent.selector = e;
      while (*e && *e != '\t')
         e++;
      if (*e)
         *e++ = 0;
      ent.hostname = e;
      while (*e && *e != '\t')
         e++;
      if (*e)
         *e++ = 0;
      ent.port = atoi(e);
      if (SkipType && !SupportedType(ent.type))
         continue;
      if (SkipACF && !CheckUser(ent.type))
         continue;
      if (!newEntry(&Menu[MenuLen], ent.type, ent.display, ent.selector, ent.hostname, ent.port))
      {
         Disconnect();
         return G_MEMORY;
      }
      MenuLen++;
      if (MenuLen == MAXMENULEN)
         break;
      if (Twirly)
      {
         PutText("\b");
         sprintf(z->ABuffer,"%c",TwirlyChars[t]);
         PutA();
         t++;
         if (t == NUMTWIRLYCHARS)
            t = 0;
      }
   }
   if (Twirly)
      PutText("\b \b");
   if (connect)
      Disconnect();
   return G_OK;
}

void SaveBookmark(int n)
{
   BPTR  fp;
   char  buf [ 256 ];

   sprintf(buf,"Mail:Users/%s/Files/",z->user1.UUCP);
   strcat(buf, BookMark);
   if ((fp = Open(buf,MODE_OLDFILE)) == 0)
      return;
   Seek(fp,0,OFFSET_END);
   FPrintf(fp, "%c%s\t%s\t%s\t%d\n", Menu[n].type, Menu[n].display, Menu[n].selector, Menu[n].hostname, Menu[n].port);
   Close(fp);
   PutText("Bookmark saved...\n\n");
   return;
}

void DelBookmark(int n)
{
   BPTR  fp;
   int   i;
   char  buf [ 256 ];

   sprintf(buf,"Mail:Users/%s/Files/",z->user1.UUCP);
   strcat(buf, BookMark);

   if (n < MenuLen - 1)
      bcopy(&Menu[n + 1], &Menu[n], sizeof(GO4ENTRY) * (MenuLen - n - 1));
   MenuLen--;
   if ((fp = Open(buf, MODE_NEWFILE)) == 0)
      return;
   for (i = 0; i < MenuLen; i++)
      FPrintf(fp, "%c%s\t%s\t%s\t%d\n", Menu[i].type, Menu[i].display, Menu[i].selector, Menu[i].hostname, Menu[i].port);
   Close(fp);
   PutText("Bookmark deleted...\n\n");
   return;
}

int CompareEntries(GO4ENTRY *ent1, GO4ENTRY *ent2)
{
   return strcmp(ent1->display, ent2->display);
}

int ReadBookmarks(void)
{
   FILE  *fp;
   char  buf [ 256 ];

   sprintf(buf,"Mail:Users/%s/Files/",z->user1.UUCP);
   strcat(buf, BookMark);
   if ((fp = fopen(buf, "r")) == 0)
      return 0;
   ReadMenu("* Bookmarks *", 0, 0, 0, 0, fp);
   fclose(fp);
   qsort(Menu, MenuLen, sizeof(GO4ENTRY), CompareEntries);
   return 1;
}

int ProcessMenu(void)
{
   int   i,
         n,
         top = 0;
   char *s;
   char buf [ 256 ];

page:
   if (ClrScreen)
      PutText("f1");
   sprintf(z->ABuffer,"%s\n%s", Title, Stack[StackLen - 1].display);
   PutA();
   if (Stack[StackLen - 1].hostname)
   {
      sprintf(z->ABuffer," (%s)", Stack[StackLen - 1].hostname);
      PutA();
   }
   DoReturn();
   DoReturn();
   if (MenuLen == 0)
      PutText("There are no menu items available.\n");
   else for (i = 0, n = top; i < Lines && n < MenuLen; i++, n++)
   {
      if (Menu[n].type == 'i')
      {
         sprintf(z->ABuffer,"%3d %-.75s\n", n + 1, Menu[n].display);
         PutA();
      }
      else
      {
         sprintf(z->ABuffer,"%3d <%s> %-.69s\n", n + 1, TypeLookup(Menu[n].type), Menu[n].display);
         PutA();
      }
   }
   DoReturn();

loop:
   {
      *buf = 0;
      if (MenuLen == 0)
         EnterLine(5,ELINE_INPUTBOX,"Help, Quit or Previous: ");
      else
      {
         sprintf(z->ABuffer,"[1-%d], Help, Quit, %s, List, Previous, Up, or Down: ", MenuLen, BM ? "Delete" : "=, Bookmarks, Save");
         EnterLine(5,ELINE_INPUTBOX,z->ABuffer);
      }
      strcpy(buf,z->InBuffer);

      strupr(buf);

      checkcarrier();

      buf[strlen(buf)] = 0;

   }
   if (*buf == 'H')
   {
      if (*Pager == 0 || !strcmp(Pager, "default"))
         Page(HelpFile);
      else
      {
         sprintf(buf, Pager, HelpFile);
         if (!strcmp(Pager, "%s"))
         {
            strcat(buf, " ");
            strcat(buf, HelpFile);
         }
         system(buf);
      }
      PutText("\nPress any key to continueg1\n");
      goto page;
   }
   if (*buf == 'Q')
      return -2;
   if (!BM && *buf == '=')
   {
      for (s = buf; *s && *s != ' '; s++);
         n = atoi(s);
      if(n < 0 || n > MenuLen)
         PutText("You must specify a valid item number to save as a bookmark (eg: s 1).\n\n");
      else if (n == 0)
      {
         sprintf(z->ABuffer,"Type:     %s\n", TypeLookup(Stack[StackLen - 1].type));
         PutA();
         sprintf(z->ABuffer,"Title:    %s\n", Stack[StackLen - 1].display);
         PutA();
         sprintf(z->ABuffer,"Host:     %s\n", Stack[StackLen - 1].hostname);
         PutA();
         sprintf(z->ABuffer,"Port:     %d\n", Stack[StackLen - 1].port);
         PutA();
         sprintf(z->ABuffer,"Selector: %s\n\n", Stack[StackLen - 1].selector ? Stack[StackLen - 1].selector : "");
         PutA();
      }
      else
      {
         sprintf(z->ABuffer,"Type:     %s\n", TypeLookup(Menu[n - 1].type));
         PutA();
         sprintf(z->ABuffer,"Title:    %s\n", Menu[n - 1].display);
         PutA();
         sprintf(z->ABuffer,"Host:     %s\n", Menu[n - 1].hostname);
         PutA();
         sprintf(z->ABuffer,"Port:     %d\n", Menu[n - 1].port);
         PutA();
         sprintf(z->ABuffer,"Selector: %s\n\n", Menu[n - 1].selector ? Menu[n - 1].selector : "");
         PutA();
      }
      goto loop;
   }
   if (!BM && (*buf == 'B'))
      return -3;
   if (!BM && (*buf == 'S'))
   {
      for (s = buf; *s && *s != ' '; s++);
         n = atoi(s);
      if ((n < 1) || (n > MenuLen))
         PutText("You must specify a valid menu item number to save as a bookmark (eg: s 1).\n\n");
      else
         SaveBookmark(n - 1);
      goto loop;
   }
   if (BM && (*buf == 'D'))
   {
      for (s = buf; *s && *s != ' '; s++);
         n = atoi(s);
      if ((n < 1) || (n > MenuLen))
         PutText("You must specify a valid item number to delete. (eg: d 1)\n\n");
      else
         DelBookmark(n - 1);
      goto loop;
   }
   if (*buf == 'P')
      return -1;
   if (*buf == 'L')
      goto page;
   if ((*buf == 'D') || (*buf == 0))
   {
      top += Lines - 1;
      if (top >= MenuLen - 1)
         top = 0;
      goto page;
   }
   if (*buf == 'U')
   {
      if (top == 0)
         top = MenuLen - Lines;
      else
         top -= Lines - 1;
      if (top < 0)
         top = 0;
      goto page;
   }
   i = atoi(buf);
   if (i < 1 || i > MenuLen)
   {
      sprintf(z->ABuffer,"Please enter a command or a number between 1 and %d.\n\n", MenuLen);
      PutA();
      goto loop;
   }
   i--;
   if (Menu[i].type == 'i')
   {
      PutText("That menu item is informational only.\n\n");
      goto loop;
   }
   if (!SupportedType(Menu[i].type))
   {
      PutText("Sorry, that type of menu item isn't currently supported.\n\n");
      goto loop;
   }
   if (!CheckUser(Menu[i].type))
   {
      PutText("Sorry, but you don't have access to that item.\n\n");
      goto loop;
   }
   return i;
}

int Setup(void)
{
   BPTR  fp;
   char  line [ 91 ];
   char  *e,
         *val,
         *var;
   int   i;

   strcpy(Title,     DEFAULTTITLE);
   strcpy(Pager,     DEFAULTPAGER);
   strcpy(Telnet,    DEFAULTTELNET);
   strcpy(TN3270,    DEFAULTTN3270);
   strcpy(Directory, DEFAULTDIR);
   *BadPorts = 0;
   if(strlen(DefHost) < 1)
      strcpy(DefHost,DEFAULTHOST);
   if(!DefPort)
      DefPort   = DEFAULTPORT;
   Lines     = DEFAULTLINES;
   Twirly    = 1;
   ClrScreen = 0;
   SkipACF   = 0;
   SkipType  = 1;
   if (!(fp = Open(CONFIGPATH,MODE_OLDFILE)))
      return 0;
   while(FGets(fp, line, 90) && (line[0] != 26))
   {
      e = (char *)rindex(line, '#');
      if (e)
         *e-- = 0;
      else
      {
         e = line + strlen(line) - 1;
         *e-- = 0;
      }
      for (var = line; *var == ' ' || *var == '\t'; var++);
      for (val = var; *val && *val != ' ' && *val != '\t'; val++);
      if (*val)
         *val++ = 0;
      while (*val == ' ' || *val == '\t')
         val++;
      if (*val == '\"')
         val++;
      while (*e == ' ' || *e == '\t')
         *e-- = 0;
      if (*e == '\"')
         *e = 0;
      if (*var == 0 || *var == '#')
         continue;
      if (!strcmp(var, "TITLE"))
         strcpy(Title, val);
      else if (!strcmp(var, "PORT"))
      {
         if(!DefPort)
            DefPort = atoi(val);
      }
      else if (!strcmp(var, "SERVER"))
      {
         if(strlen(DefHost) < 1)
            strcpy(DefHost,val);
      }
      else if (!strcmp(var, "BADPORTS"))
         strcpy(BadPorts, val);
      else if (!strcmp(var, "HELPFILE"))
         strcpy(HelpFile, val);
      else if (!strcmp(var, "PAGER"))
         strcpy(Pager, val);
      else if (!strcmp(var, "WORKDIR"))
         sprintf(Directory,"Mail:Users/%s/Files/",z->user1.UUCP);
      else if (!strcmp(var, "BOOKMARK"))
         strcpy(BookMark, val);
      else if (!strcmp(var, "TELNET"))
         strcpy(Telnet, val);
      else if (!strcmp(var, "TN3270"))
         strcpy(TN3270, val);
      else if (!strcmp(var, "LINES"))
         Lines = atoi(val);
      else if (!strcmp(var, "TWIRLY"))
      {
         strupr(val);
         Twirly = (*val == 'Y' || *val == '1');
      }
      else if (!strcmp(var, "CLEAR"))
      {
         strupr(val);
         ClrScreen = (*val == 'Y' || *val == '1');
      }
      else if (!strcmp(var, "SKIPACF"))
      {
         strupr(val);
         SkipACF = (*val == 'Y' || *val == '1');
      }
      else if (!strcmp(var, "SKIPTYPE"))
      {
         strupr(val);
         SkipType = (*val == 'Y' || *val == '1');
      }
      else if (!strncmp(var, "ACF", 3) && (i = DisplayLookup(&var[3])) != -1)
         strcpy(KnownTypes[i].acf, val);
      else
      {
         sprintf(z->ABuffer,"Warning: unrecognized option in configuration file (%s)\n", var);
         PutA();
      }
   }
   Close(fp);
   return 1;
}

void bb_main(void)
{
   int ok;
   int rc;
   int rc2;
   char buf[10];

   SetDoing("Gopher");

   PutText("f1");
   PutText("\nBBGopher 1.6 - Written by Mark Morley (September '93)");
   DoReturn();
   PutText("BBGopher/amiga 1.0 - Ported by Jason M. Weber (20.4.95)");
   DoReturn();
   PutText("BBGopher/CNet  1.0 - Ported by Kelly Cochran  (16.11.96)");
   DoReturn();
   DoReturn();

   StackLen = 0;

   rc = ReadMenu("Default Gopher Server", 0, DefHost, DefPort, 1, 0);
   if (rc != G_OK)
   {
      sprintf(z->ABuffer,"Sorry, but I was unable to connect to %s on port %d\n", DefHost, DefPort);
      PutA();
      GetOut();
   }
   BM = 0;
   ok = 1;
   while (ok)
   {
      rc = ProcessMenu();
      switch (rc)
      {
         case -1:
            BM = 0;
            if (StackLen > 1)
            {
               StackLen--;
               delEntry(&Stack[StackLen]);
               StackLen--;
               ReadMenu(Stack[StackLen].display, Stack[StackLen].selector, Stack[StackLen].hostname, Stack[StackLen].port, 1, 0);
            }
            break;
         case -2:
            ok = 0;
            break;
         case -3:
            if (BM == 0)
            {
               ReadBookmarks();
               BM = 1;
            }
            break;
         default:
            if (BM && StackLen > 1)
            {
               StackLen--;
               delEntry(&Stack[StackLen]);
            }
            BM = 0;
            switch (Menu[rc].type)
            {
               case '0':
                  rc2 = bb_ReadText(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port);
                  break;
               case '1':
                  rc2 = ReadMenu(Menu[rc].display, Menu[rc].selector, Menu[rc].hostname, Menu[rc].port, 1, 0);
                  break;
               case '4':
                  rc2 = GetFile(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port, "Mac HQX", 1);
                  break;
               case '5':
                  rc2 = GetFile(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port, "DOS binary", 1);
                  break;
               case '6':
                  rc2 = GetFile(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port, "UU encoded", 0);
                  break;
               case '7':
                  rc2 = DoSearch(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port);
                  break;
               case '8':
                  if (*Menu[rc].selector)
                  {
                     sprintf(z->ABuffer,"Try logging in as '%s'\n\n", Menu[rc].selector);
                     PutA();
                  }
                  sprintf(buf, "%s %s %d", Telnet, Menu[rc].hostname, Menu[rc].port ? Menu[rc].port : 23);
                  system(buf);
                  rc2 = G_OK;
                  break;
               case '9':
                  rc2 = GetFile(Menu[rc].selector, Menu[rc].hostname, Menu[rc].port, "binary", 1);
                  break;
               case 'T':
                  sprintf(buf, "%s %s %d", TN3270, Menu[rc].hostname, Menu[rc].port ? Menu[rc].port : 23);
                  system(buf);
                  rc2 = G_OK;
                  break;
            }
            if (rc2 != G_OK)
            {
               PutText("ERROR: ");
               switch (rc2)
               {
                  case G_HOST:
                     sprintf(z->ABuffer,"Couldn't resolve hostname '%s'\n", Menu[rc].hostname);
                     PutA();
                     break;
                  case G_SOCKET:
                     PutText("Socket error!\n");
                     break;
                  case G_CONNECT:
                     sprintf(z->ABuffer,"Couldn't connect to %s\n", Menu[rc].hostname);
                     PutA();
                     break;
                  case G_CREATE:
                     PutText("Couldn't create local file.\n");
                     break;
                  default:
                     sprintf(z->ABuffer,"Code = %d\n", rc2);
                     PutA();
                     break;
               }
               DoReturn();
            }
            break;
      }
   }
   while (MenuLen)
   {
      MenuLen--;
      delEntry(&Menu[MenuLen]);
   }
   while (StackLen)
   {
      StackLen--;
      delEntry(&Stack[StackLen]);
   }
}

/**************************************************************************
 *       Routine called if Load error (wrong CNet version, etc,...        *
 **************************************************************************/
void LoadError( void )
{
        DeletePort( replyp );
        exit(0);
}

/************************************************************************
 *                           Generic EXIT code                          *
 ************************************************************************/
void GetOut( void )
{
        ShutDown( NULL );
        DeletePort( replyp );
        exit(0);
}


/**************************************************************************
 *                         another file to run?                           *
 **************************************************************************/
void ShutDown( char *spawn )
{
        /* spawn = full path/file to run */
        if( spawn )
                strcpy( z->CSpawn, spawn );

        CallHost( 0 );
}


void CallHost( UBYTE c )
{
        cmess.command = c;
        PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
        WaitPort( replyp );
        GetMsg  ( replyp );
}

void PutText( char *text )
{
        cmess.arg1 = (ULONG)text;       /* text to print                */
        CallHost( 1 );
}

void PutA( void )
{
        PutText( z->ABuffer );
}


/**************************************************************************
 *    ENTERLINE FLAGS:                                                    *
 *      1: All capitalized                                                *
 *      2: FILENAME.  Don't allow =":; or asterisk                        *
 *      4: Begin with existing z->InBuffer                                *
 *      8: Chop leading spaces                                            *
 *     16: Force 1st letter of word caps                                  *
 *     32: Force all others lower case                                    *
 *     64: Numeric input only                                             *
 *    128: Print input box (.)                                            *
 *    256: DO allow MCI                                                   *
 *    512: HANDLES/SPECIAL.  Don't allow ^_`{|}~@                         *
 *   1024: Exit for . or / at beginning of line                           *
 *   2048: Exit for backspace at beginning of line                        *
 *   4096: Do not allow OLM's to appear while editing                     *
 *   8192: Allow Chat break in at this prompt. COMMAND PROMPT.            *
 *  16384: Don't allow SPACE, either                                      *
 *  32768: DON'T ALLOW MOVEMENT                                           *
 *  65536: Don't allow forward slash                                      *
 **************************************************************************/
int EnterLine( UBYTE len, ULONG flags, char *prompt )
{
        cmess.arg1 = (ULONG)len;        /* how many chars max to input  */
        cmess.arg2 = (ULONG)flags;      /* 1=UpperCase                  */
        cmess.arg3 = (ULONG)prompt;     /* text to print before input   */
        CallHost( 2 );                  /* result is in z->InBuffer     */
        return( (int)strlen( z->InBuffer ));
}

/**************************************************************************
 *                       Stop until a key is pressed                      *
 **************************************************************************/
char OneKey( void )
{
        CallHost( 3 );
        return( (char)cmess.result );   /* returns key pressed */
}

/**************************************************************************
 *                 Check z->InBuffer for Chat, OLM, etc                   *
 **************************************************************************/
long CommonCommands( void )
{
        CallHost( 5 );
        return( (long)cmess.result );
}

/**************************************************************************
 *
 **************************************************************************/
UBYTE ReadFile( char *path, UBYTE flags )
{
        cmess.arg1 = (ULONG)path;
        cmess.arg2 = (ULONG)flags;          /* 1 = print File Not Found */
        CallHost( 6 );
        return( (UBYTE)cmess.result );      /* returns FALSE if File Not Found */
}

/**************************************************************************
 *        Sets the "Action" or "Where" field for the user's port          *
 *      ** remember to retain and restore the previous z->DOING **        *
 **************************************************************************/
void SetDoing( char *what )
{
        cmess.arg1 = (ULONG)what;
        CallHost( 7 );
}


/**************************************************************************
 *Invokes the CNet editor according to user's preference of line or Visual*
 **************************************************************************/
void CallEditor( short max, short inlines )
{
        cmess.arg1 = (ULONG)max;        /* Maximum number of lines (250)*/
        cmess.arg2 = (ULONG)inlines;    /* TRUE/FALSE use existing _edbuff? */
        CallHost( 8 );
}

/**************************************************************************
 *  Construct a date in CNet format - like that seen in the port titlebar *
 **************************************************************************/
void MakeDate( struct IsDate *date, char *output )
{
        cmess.arg1 = (ULONG)date;
        cmess.arg2 = (ULONG)output;
        CallHost( 10 );
}

/**************************************************************************
 *  Forces CNet to flush/check it's buffers and parse/update the keyboard *
 *  buffers                                                               *
 **************************************************************************/
void CheckFlowControl( void )
{
        CallHost( 21 );
}

/**************************************************************************
 * Print the prompt string passed as "a" and return the user's            *
 * YES or NO equivalent result.  1=YES, 2=NO                              *
 **************************************************************************/
UBYTE PutQ( char *a )
{
        PutText( a );
        return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}

/**************************************************************************
 * Isn't it obvious enough? ;-)
 **************************************************************************/
void DoReturn( void )
{
        PutText("\n");
}

/**************************************************************************
 * Create the editor filename used for the current port and place it      *
 * in the string array passed as "path"                                   *
 **************************************************************************/
void MakeEd( char *path )
{
        sprintf( path, "%s_edbuff%d", myp->gc.ZIPpath, z->InPort );
}


/**************************************************************************
 * Delete the editor file                                                 *
 **************************************************************************/
void DeleteEd( void )
{
        char    filename[80];

        MakeEd    ( filename ) ;
        DeleteFile( filename ) ;
}

/**************************************************************************
 * Open the file used for the editor and return the filehandle (BPTR)     *
 **************************************************************************/
BPTR OpenEd( long mode )
{
        char    filename[80];

        MakeEd( filename );

        return Open( filename, mode );
}

/**************************************************************************
 * insert the contents of the file belonging to filehandle "fp" into      *
 * the current port's editor file                                         *
 **************************************************************************/
void PrepEditor( BPTR fp )
{
        BPTR    kp;
        char    buff[100];

        if( fp ) {
                if( kp = OpenEd( MODE_NEWFILE ) ) {
                        while( FGets( fp, buff, 82 ) && buff[0]!=26 )
                               FPuts( kp, buff     ) ;

                        Close( kp );
                }
        }
        else    DeleteEd();
}

/**************************************************************************
 * Save the contents of the current port's editor file into the file      *
 * belonging to filehandle "fp"                                           *
 **************************************************************************/
void SaveEditor( BPTR fp, UBYTE eof )
{
        BPTR    kp;
        char    buff[100];

        if( kp = OpenEd( MODE_OLDFILE ) ) {
                while( FGets( kp, buff, 82 ) && buff[0]!=26 )
                       FPuts( fp, buff     ) ;

                Close( kp );

                DeleteEd();
        }

        if( eof ) FPuts( fp, "\032\n" );
}

void checkcarrier(void)
{
   if( !z->Carrier || z->Dumped || !z->TimeLeft )
      GetOut();
   return;
}

void err(char *error)
{
   PutText(error);
   GetOut();
}
