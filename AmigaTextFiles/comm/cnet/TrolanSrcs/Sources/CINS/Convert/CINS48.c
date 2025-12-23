/*
** CINS Configuration file conversion utility
** by Kelly Cochran
** Last updated:
*/

#include "/CINS.h"

void main(void)
{
    struct CINSCfg Config;

    BPTR fp;

    char *tmp;

    char buf1 [ 256 ],
         buf2 [ 256 ];

    memset(&Config, 0, sizeof(struct CINSCfg));

    if(fp = Open(CINSCFG, MODE_OLDFILE))
    {
        Printf("Checking for old format CINS.cfg file....");
        FGets(fp, buf1, sizeof(buf1));
        if(*buf1 == '#')
            Printf("found!\n");
        else
        {
            Printf("not found.  You do not need to run this program.\n");
            return;
        }
        Printf("Converting old format file..\n");
        while(FGets(fp,buf1,256) != NULL)
        {
            if(*buf1 != '#')
            {
                strmid(buf1,buf2,1,4);

                if(!strcmp(buf2,"ACCE"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Access = atoi(tmp);
                   Printf("Access level minimum..\n");
                }
                else if(!strcmp(buf2,"FDIR"))
                {
                   strmid(buf1,buf2,6,strlen(buf1));
                   stccpy(Config.UserDir,buf2,strlen(buf2));
                   Printf("UserDir..\n");
                }
                else if(!strcmp(buf2,"FTP="))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.FTPin = atoi(tmp);
                   Printf("Outbound FTP limit..\n");
                }
                else if(!strcmp(buf2,"MENU"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Menu = atoi(tmp);
                   Printf("BBSMenu..\n");
                }
                else if(!strcmp(buf2,"LINE"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Line = atoi(tmp);
                   Printf("BBSText..\n");
                }
                else if(!strcmp(buf2,"RATE"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Rate = atol(tmp);
                   Printf("Accounting rate..\n");
                }
                else if(!strcmp(buf2,"URL="))
                {
                   strmid(buf1,buf2,5,strlen(buf1));
                   stccpy(Config.URL,buf2,strlen(buf2));
                   Printf("Default home page..\n");
                }
                else if(!strcmp(buf2,"IRC="))
                {
                   strmid(buf1,buf2,5,strlen(buf1));
                   stccpy(Config.IRC,buf2,strlen(buf2));
                   Printf("Default IRC server..\n");
                }
                else if(!strcmp(buf2,"ARCH"))
                {
                   strmid(buf1,buf2,8,strlen(buf1));
                   stccpy(Config.Archie,buf2,strlen(buf2));
                   Printf("Default archie server..\n");
                }
                else if(!strcmp(buf2,"GOPH"))
                {
                   strmid(buf1,buf2,8,strlen(buf1));
                   stccpy(Config.Gopher,buf2,strlen(buf2));
                   Printf("Default gopher server..\n");
                }
                else if(!strcmp(buf2,"LOG="))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Log = atoi(tmp);
                   Printf("Log ID..\n");
                }
                else if(!strcmp(buf2,"FREE"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Free = atol(tmp);
                   Printf("User directory maximum..\n");
                }
                else if(!strcmp(buf2,"UPLO"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.UserUp = atoi(tmp);
                   Printf("Allow uploads..\n");
                }
                else if(!strcmp(buf2,"CD=="))
                {
                    tmp = strchr(buf1,'=');
                    tmp++;
                    Config.CD = atoi(tmp);
                    Printf("CD allowed..\n");
                }
                else if(!strcmp(buf2,"DAYS"))
                {
                   tmp = strchr(buf1,'=');
                   tmp++;
                   Config.Days = atoi(tmp);
                   Printf("Purge days..\n");
                }
            }
        }
        Close(fp);
    }
    Rename(CINSCFG, CINSCFG".bak");
    if(fp = Open(CINSCFG, MODE_NEWFILE))
    {
        Printf("Writing new config file..");
        Write(fp, &Config, sizeof(struct CINSCfg));
        Close(fp);
        Printf("done.\n");
    }
}
