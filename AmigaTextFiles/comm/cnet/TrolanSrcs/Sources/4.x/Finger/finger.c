#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

extern struct h_errno;

int main(int argc, char **argv);

int main(int argc, char **argv)
{
   char *name = NULL,
        *site;
   char *alist[1];
   int sd,
       i;
   ULONG ip;

   char buff [ 1024 ] = "";

   struct hostent     *he, tmp;
   struct sockaddr_in *sin;
   struct servent     *se;

   if(*argv[1])
      name = argv[1];

   if(site = strchr(argv[1],'@'))
   {
      *site = '\0';
      *site++;
   }

   if(!(sin = AllocVec(sizeof(struct sockaddr_in),MEMF_CLEAR)))
      return 0;

   he = gethostbyname(site);

   if(!he)
   {
      if(!(ip = inet_addr(site)))
      {
         Printf("Unknown host: %s\n",site);
         return 0;
      }
      tmp.h_name      = site;
      tmp.h_addr_list = alist;
      tmp.h_addr      = (char *)&ip;
      tmp.h_length    = sizeof(struct in_addr);
      tmp.h_addrtype  = AF_INET;
      tmp.h_aliases   = 0;
      he              = &tmp;
   }

   Printf("[%s]\n", he->h_name);

   se = getservbyname("finger", "tcp");

   if(!se)
   {
      Printf("finger/tcp: Unknown service\n");
      return 0;
   }

   if((sd = socket(AF_INET,SOCK_STREAM,0)) < 0)
      Printf("No socket!!\n");
   else
   {
      CopyMem(he->h_addr,&sin->sin_addr,sizeof(struct in_addr));
      sin->sin_port   = se->s_port;
      sin->sin_family = AF_INET;

      if((connect(sd,(struct sockaddr *)sin,sizeof(struct sockaddr_in))) < 0)
         Printf("Couldn't connect\n");
      else
      {
         i = strlen(name);
         name[i++] = '\n';
         name[i++] = '\0';
         if(send(sd,name,i,0) != i)
         {
            FreeVec(sin);
            return 1;
         }
            
         while((i = recv(sd, buff, sizeof(buff) - 1, 0)) > 0)
         {
            buff[i++] = '\0';
            Printf(buff);
         }
      }

      CloseSocket(sd);
   }

   if(sin)
      FreeVec(sin);

   return 0;
}
