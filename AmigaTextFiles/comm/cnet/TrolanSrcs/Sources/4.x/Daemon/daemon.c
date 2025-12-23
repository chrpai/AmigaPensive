#include <errno.h>
#ifndef WIN32
 #include <netdb.h>
 #include <sys/types.h>
 #include <sys/socket.h>
#else
 #include <winsock.h>
#endif


int main(int argc, char **argv);

int main(int argc, char **argv)
{
   struct sockaddr_in  sin,
                       from;
   struct sockaddr     ir;
   struct hostent      *he;
   struct servent      *se;
   
   int sd,
       nsd,
       i;

   int SIZE = sizeof(struct in_addr);
   int ONE  = 1;

   char buff [ 1024 ] = "";

   se = getservbyname("test", "tcp");

   if(!se)
   {
      printf("%s: tcp/test service unknown\n", argv[0]);
      return 5;
   }

   if(sd = socket(AF_INET, SOCK_STREAM, 0))
   {
      printf("Socket opened.\n");

      memset(&sin, 0, sizeof(sin));
      sin.sin_family = AF_INET;
      sin.sin_port   = se->s_port;
      if(!bind(sd, (struct sockaddr *)&sin, sizeof(sin)))
      {
         printf("Socket bound.\n");
         if(!listen(sd, 32))
         {
            printf("Listening opened.\n");
            do
            {
               errno = 0;
               nsd = accept(sd, &ir, &SIZE);

            } while(nsd < 0);

            if(nsd < 0)
            {
               closesocket(sd);
               return 10;
            }
            if(nsd > 0)
               send(nsd, "Input: ", 7, 0);

            while((i = recv(nsd, buff, sizeof(buff) - 1, 0)) > 0)
            {
               buff[i++] = '\0';
               if(!strcmp(buff,"HELO"))
               {
                  send(nsd, "Hello to you too!\n", 19, 0);
               }
               else if(toupper(*buff) == 'Q')
               {
                  send(nsd, "Goodbye\n", 8, 0);
                  closesocket(nsd);
               }
               else
                  printf("Recieved: %s\n", buff);
            }
         }
      }
      closesocket(sd);
   }

   return 0;
}
