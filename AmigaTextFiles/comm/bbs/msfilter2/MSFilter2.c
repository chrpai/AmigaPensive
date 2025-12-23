#include <exec/types.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MSFilter.h"

struct pkt_hdr my_ibm_pkt;
struct pkt_hdr my_amy_pkt;

struct pak_msg_hdr my_ibm_msg;
struct pak_msg_hdr my_amy_msg;

UBYTE  msg_date[20];
UBYTE  msg_to[36];
UBYTE  msg_from[36];
UBYTE  msg_subject[72];

UBYTE pkt_name[80];
UBYTE pkt_path[80];
UBYTE log_file[128];
UBYTE file_name[128];
UBYTE t_file_name[128];
UBYTE config_file[128];

FILE *in = NULL;
FILE *out = NULL;
FILE *log = NULL;

#define MC 1001

/* #define DEBUG 0x01 */

UBYTE filter_from[MC][80];
UBYTE filter_to[MC][80];
UBYTE keep_to[MC][80];
UBYTE keep_from[MC][80];
UBYTE filter_subject[MC][80];
UBYTE keep_subject[MC][80];

struct FileInfoBlock __aligned my_info;
struct FileLock *my_lock;

char *version = "$VER: MSFilter 2.001beta (c) 1994-1997 by Doug McLean (19-Jul-1997)";

void main(int argc, char *argv[])
{
int numpkts = 0;
        printf ("%s\n",&version[6]);
        if (argc != 4)
        {
                printf ("USAGE: %s Inbound: Logs: Config_File_Name\n",argv[0]);
                exit (10);
        }
        sprintf (pkt_path,"%s",argv[1]);
        sprintf (log_file,"%sMSFilter.log",argv[2]);
        sprintf (config_file,"%s",argv[3]);
        if ((log=fopen(log_file,"ab"))==NULL)
        {
                printf ("Cannot open %s for log\n",log_file);
                exit (12);
        }
        if (read_config()==0)
        {
                printf ("Cannot open config file %s\n",config_file);
                fclose (log);
                exit (13);
        }
        if ((my_lock = (struct FileLock *)Lock(pkt_path,ACCESS_READ))==NULL)
        {
                printf ("Cannot get lock on %s\n",pkt_path);
                fclose (log);
                exit (11);
        }
        Examine (my_lock,&my_info);
        while (ExNext(my_lock,&my_info))
        {
           if (my_info.fib_DirEntryType >= 0) continue;
           if (strlen(my_info.fib_FileName)!=12)continue;
           if (stricmp(&my_info.fib_FileName[8],".pkt")) continue;
           sprintf (pkt_name,"%s",my_info.fib_FileName);
           sprintf (file_name,"%s%s",pkt_path,pkt_name);
           numpkts++;
           parse_pkt();
        }
        UnLock (my_lock);
        printf ("**** PROCESSED %d PACKETS THIS SESSION ****\n",numpkts);
        fprintf (log,"**** PROCESSED %d PACKETS THIS SESSION ****\n",numpkts);
        fclose (log);
        exit (0);
}

read_config()
{
FILE *cfg = NULL;
char inbuff[128];
int i_f = 0;
int i_t = 0;
int i_kt = 0;
int i_kf = 0;
int i_s = 0;
int i_sk = 0;
        fprintf (log,"\n------------------------------------------------------------\n");
        fprintf (log,"%s\nCurrent configuration:\n",&version[6]);
        if ((cfg=fopen(config_file,"rb"))==NULL) return (0);
        while (fgets(inbuff,80,cfg))
        {
                strip_n(inbuff);
                if (strlen(inbuff) < 4)
                {
                        fprintf (log,"   ???????? SHORT LINE: %s\n",inbuff);
                        continue;
                }
                inbuff[79]=0x00;
                strlwr(inbuff);
                switch (inbuff[0])
                {
                    case ';': /* comment line */
                        fprintf (log,"   ******** %s\n",&inbuff[1]);
                        break;
                    case '<': /* filter from */
                        if (i_f > (MC-2))
                        {
                           fprintf (log,"   WARNING: From buffer full, [%s] not filtered\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (filter_from[i_f],"%s",&inbuff[1]);
                           fprintf (log,"   [ < ] Filter From    [%s*]\n",filter_from[i_f]);
                           i_f++;
                        }
                        break;
                    case '>': /* filter to */
                        if (i_t > (MC-2))
                        {
                           fprintf (log,"   WARNING: To buffer full, [%s] not filtered\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (filter_to[i_t],"%s",&inbuff[1]);
                           fprintf (log,"   [ > ] Filter To      [%s*]\n",filter_to[i_t]);
                           i_t++;
                        }
                        break;
                    case '+': /* keep to */
                        if (i_kt > (MC-2))
                        {
                           fprintf (log,"   WARNING: Keep to buffer full, [%s] not kept\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (keep_to[i_kt],"%s",&inbuff[1]);
                           fprintf (log,"   [ + ] Keep To        [%s*]\n",keep_to[i_kt]);
                           i_kt++;
                        }
                        break;
                    case '&': /* keep from */
                        if (i_kf > (MC-2))
                        {
                           fprintf (log,"   WARNING: Keep from buffer full, [%s] not kept\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (keep_from[i_kf],"%s",&inbuff[1]);
                           fprintf (log,"   [ & ] Keep From      [%s*]\n",keep_from[i_kf]);
                           i_kf++;
                        }
                        break;
                    case '=': /* filter subject */
                        if (i_s > (MC-2))
                        {
                           fprintf (log,"   WARNING: Subject buffer full, [%s] not filtered\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (filter_subject[i_s],"%s",&inbuff[1]);
                           fprintf (log,"   [ = ] Filter Subject [*%s*]\n",filter_subject[i_s]);
                           i_s++;
                        }
                        break;
                    case '$': /* keep subject */
                        if (i_sk > (MC-2))
                        {
                           fprintf (log,"   WARNING: Keep subject buffer full, [%s] not kept\n",&inbuff[1]);
                        }
                        else
                        {
                           sprintf (keep_subject[i_sk],"%s",&inbuff[1]);
                           fprintf (log,"   [ $ ] Keep Subject   [*%s*]\n",keep_subject[i_sk]);
                           i_sk++;
                        }
                        break;
                        
                    default:
                        fprintf (log,"   ???????? CONFIG ERROR: %s\n",inbuff);
                        break;
                }
        }
        fclose (cfg);
        filter_from[MC-1][0] = 0x00;
        filter_to[MC-1][0] = 0x00;
        keep_to[MC-1][0] = 0x00;
        keep_from[MC-1][0] = 0x00;
        filter_subject[MC-1][0] = 0x00;
        fprintf (log,"------------------------------------------------------------\n");
        return (1);
}

strip_n(char *buffer)
{
        while (*buffer!=0x00)
        {
                if (*buffer == '\n') *buffer = 0x00;
                if (*buffer == '\r') *buffer = 0x00;
                buffer++;
        }
        return (0);
}

parse_pkt()
{
int num_msgs = 0;
int ch;
int i;
int pkt_filtered = 0;
int fr;
        if ((in = fopen(file_name,"rb"))==NULL)
        {
                printf ("Cannot open %s for input\n",file_name);
                return (0);
        }
        sprintf (t_file_name,"%s.msfilter",file_name);
        if ((out = fopen(t_file_name,"wb"))==NULL)
        {
                printf ("Cannot open %s for output\n",t_file_name);
                fprintf (log,"Cannot open %s for output\n",t_file_name);
                fclose (out);
                return (0);
        }
        if (fread((char *)&my_ibm_pkt,sizeof(my_ibm_pkt),1,in)!=1)
        {
                fclose (in);
                printf ("Bad header for packet %s\n",file_name);
                fprintf (log,"Bad header for packet %s\n",file_name);
                return (0);
        }
        fwrite ((char *)&my_ibm_pkt,sizeof(my_ibm_pkt),1,out);
        
        pkt_ibm_to_amy(&my_ibm_pkt,&my_amy_pkt);

        print_pkt_hdr(&my_amy_pkt);
        printf ("Working on packet %s\n",pkt_name);
        
        while (fread((char *)&my_ibm_msg,sizeof(my_ibm_msg),1,in)==1)
        {
            for (i = 0; i < 20; i++)
            {
                ch = fgetc(in);
                if (ch==EOF) goto eop;
                msg_date[i] = ch;
                if (ch == 0x00) break;
            }
            for (i = 0; i < 36; i++)
            {
                ch = fgetc(in);
                if (ch==EOF) goto eop;
                msg_to[i] = ch;
                if (ch == 0x00) break;
            }
            for (i = 0; i < 36; i++)
            {
                ch = fgetc(in);
                if (ch==EOF) goto eop;
                msg_from[i] = ch;
                if (ch == 0x00) break;
            }
            for (i = 0; i < 72; i++)
            {
                ch = fgetc(in);
                if (ch==EOF) goto eop;
                msg_subject[i] = ch;
                if (ch == 0x00) break;
            }
            msg_ibm_to_amy(&my_ibm_msg,&my_amy_msg);

            printf (" Message #%-5d...",num_msgs);

            if ((fr =filter_msg())==0)
            {
                write_ibm_msg();
                printf ("...Ok      \n");
            }
            else
            {
                pkt_filtered++;
                print_msg_hdr(&my_amy_msg,num_msgs,fr);
                log_filtered_msg();
                printf ("...Filtered\n\n");
            }
            printf ("\033[1A");
            num_msgs++;
        }
eop:
        fputc(0x00,out);fputc(0x00,out);
        fclose (in);
        fclose (out);
        printf ("\nEnd of packet\n");
        printf ("PACKET CONTAINED %d MESSAGES\n",num_msgs);
        fprintf (log,"PACKET CONTAINED %d MESSAGES\n",num_msgs);
        if (pkt_filtered == 0)
        {
                printf ("NO MESSAGES FILTERED IN THIS PACKET\n");
                fprintf (log,"NO MESSAGES FILTERED IN THIS PACKET\n\n");
                DeleteFile (t_file_name);
        }
        else if (pkt_filtered != num_msgs)
        {
                printf ("TOTAL OF %d MESSAGES FILTERED FROM THIS PACKET\n",pkt_filtered);
                fprintf (log,"TOTAL OF %d MESSAGES FILTERED FROM THIS PACKET\n\n",pkt_filtered);
                DeleteFile (file_name);
                Rename (t_file_name,file_name);
        }
        else
        {
                printf ("TOTAL OF %d MESSAGES FILTERED FROM THIS PACKET\n",pkt_filtered);
                printf ("ALL MESSAGES IN PACKET WERE FILTERED. PACKET DELETED\n");
                fprintf (log,"TOTAL OF %d MESSAGES FILTERED FROM THIS PACKET\n",pkt_filtered);
                fprintf (log,"ALL MESSAGES IN PACKET WERE FILTERED. PACKET DELETED\n\n");
                DeleteFile (file_name);
                DeleteFile (t_file_name);
        }
        printf ("\n");
        return (0);        
}

filter_msg()
{
int i;
UBYTE msg_temp[80];

/*** check keep strings first. probably keep most based on to ***/

        strcpy (msg_temp,msg_to);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** To: [%s]  Temp: [%s]\n",msg_to,msg_temp);
#endif
        while (keep_to[i][0]!=0x00)
        {
                if (strstr(msg_temp,keep_to[i])) return (0);
                i++;
        }
        strcpy (msg_temp,msg_from);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** From: [%s]  Temp: [%s]\n",msg_from,msg_temp);
#endif
        while (keep_from[i][0]!=0x00)
        {
                if (strstr(msg_temp,keep_from[i])) return (0);
                i++;
        }
        strcpy (msg_temp,msg_subject);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** Subject: [%s]  Temp: [%s]\n",msg_subject,msg_temp);
#endif
        while (keep_subject[i][0]!=0x00)
        {
                if (strstr(msg_temp,keep_subject[i])) return (0);
                i++;
        }

/***** check filters. probably reject more on from than subject
       so check it first. if more rejects are expected due to
       subject, then put that section first **********/

        strcpy (msg_temp,msg_from);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** From: [%s]  Temp: [%s]\n",msg_from,msg_temp);
#endif
        while (filter_from[i][0]!=0x00)
        {
#ifdef DEBUG
          printf ("  I: %d  From: [%s]  Filter: [%s]\n",i,msg_temp,filter_from[i]);
#endif
                if (strstr(msg_temp,filter_from[i])) return (1);
                i++;                
        }
        strcpy (msg_temp,msg_subject);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** Subject: [%s]  Temp: [%s]\n",msg_subject,msg_temp);
#endif
        while (filter_subject[i][0]!=0x00)
        {
#ifdef DEBUG
          printf ("  I: %d  Subject: [%s]  Filter: [%s]\n",i,msg_temp,filter_subject[i]);
#endif
                if (strstr(msg_temp,filter_subject[i])) return (3);
                i++;
        }
        strcpy (msg_temp,msg_to);
        strlwr (msg_temp);
        i = 0;
#ifdef DEBUG
         printf ("** To: [%s]  Temp: [%s]\n",msg_to,msg_temp);
#endif
        while (filter_to[i][0]!=0x00)
        {
#ifdef DEBUG
          printf ("  I: %d  To: [%s]  Filter: [%s]\n",i,msg_temp,filter_to[i]);
#endif
                if (strstr(msg_temp,filter_to[i])) return (2);
                i++;
        }
        return (0);
}

log_filtered_msg()
{
int ch;
int i = 0;
      fprintf (log,"- - - - - - - - - - - - - - - Message Text - - - - - - - - - - - - - - - -\n");
       while ((ch = fgetc(in))!=0x00)
       {
            if (ch==EOF) break;
            if ((ch >= 32) && (ch <= 126))
            {
                fputc (ch,log);
                i++;
            }
            else if ((ch == '\r')||(ch == '\n')) i = 80;
            else
            {
                fprintf (log,"[0x%x]",ch);
                i+=6;
            }
            if (i > 77)
            {
                fputc('\n',log);
                i = 0;
            }
       }
       fprintf (log,"- - - - - - - - - - - - - - End Message Text - - - - - - - - - - - - - - -\n");
       return(0);
}

write_ibm_msg()
{
int ch;
        fwrite ((char *)&my_ibm_msg,sizeof(my_ibm_msg),1,out);
        fprintf (out,"%s",msg_date);fputc(0x00,out);
        fprintf (out,"%s",msg_to);fputc(0x00,out);
        fprintf (out,"%s",msg_from);fputc(0x00,out);
        fprintf (out,"%s",msg_subject);fputc(0x00,out);
        while ((ch = fgetc(in))!=0x00)
        {
            if (ch==EOF) break;
            fputc(ch,out);
        }
        fputc(0x00,out);
        return (0);
}

msg_ibm_to_amy(struct pak_msg_hdr *ibm,struct pak_msg_hdr *amy)
{
        amy->m_type = ((ibm->m_type & 0xff00) >> 8) | ((ibm->m_type & 0x00ff) << 8);
        amy->m_orig_node = ((ibm->m_orig_node & 0xff00) >> 8) | ((ibm->m_orig_node & 0x00ff) << 8);
        amy->m_dest_node = ((ibm->m_dest_node & 0xff00) >> 8) | ((ibm->m_dest_node & 0x00ff) << 8);
        amy->m_orig_net = ((ibm->m_orig_net & 0xff00) >> 8) | ((ibm->m_orig_net & 0x00ff) << 8);
        amy->m_dest_net = ((ibm->m_dest_net & 0xff00) >> 8) | ((ibm->m_dest_net & 0x00ff) << 8);
        amy->m_attribute = ((ibm->m_attribute & 0xff00) >> 8) | ((ibm->m_attribute & 0x00ff) << 8);
        amy->m_cost = ((ibm->m_cost & 0xff00) >> 8) | ((ibm->m_cost & 0x00ff) << 8);
        return (0);
}

pkt_ibm_to_amy(struct pkt_hdr *ibm,struct pkt_hdr *amy)
{
int i;
        amy->p_orig_node = ((ibm->p_orig_node & 0xff00) >> 8) | ((ibm->p_orig_node & 0x00ff) << 8);
        amy->p_dest_node = ((ibm->p_dest_node & 0xff00) >> 8) | ((ibm->p_dest_node & 0x00ff) << 8);
        amy->p_year = ((ibm->p_year & 0xff00) >> 8) | ((ibm->p_year & 0x00ff) << 8);
        amy->p_month = ((ibm->p_month & 0xff00) >> 8) | ((ibm->p_month & 0x00ff) << 8);
        amy->p_day = ((ibm->p_day & 0xff00) >> 8) | ((ibm->p_day & 0x00ff) << 8);
        amy->p_hour = ((ibm->p_hour & 0xff00) >> 8) | ((ibm->p_hour & 0x00ff) << 8);
        amy->p_min = ((ibm->p_min & 0xff00) >> 8) | ((ibm->p_min & 0x00ff) << 8);
        amy->p_sec = ((ibm->p_sec & 0xff00) >> 8) | ((ibm->p_sec & 0x00ff) << 8);
        amy->p_baud = ((ibm->p_baud & 0xff00) >> 8) | ((ibm->p_baud & 0x00ff) << 8);
        amy->p_orig_net = ((ibm->p_orig_net & 0xff00) >> 8) | ((ibm->p_orig_net & 0x00ff) << 8);
        amy->p_dest_net = ((ibm->p_dest_net & 0xff00) >> 8) | ((ibm->p_dest_net & 0x00ff) << 8);
        amy->p_orig_zone = ((ibm->p_orig_zone & 0xff00) >> 8) | ((ibm->p_orig_zone & 0x00ff) << 8);
        amy->p_dest_zone = ((ibm->p_dest_zone & 0xff00) >> 8) | ((ibm->p_dest_zone & 0x00ff) << 8);
        amy->p_type = ((ibm->p_type & 0xff00) >> 8) | ((ibm->p_type & 0x00ff) << 8);
        amy->p_pcode = ibm->p_pcode;
        amy->p_psernum = ibm->p_psernum;
        sprintf (amy->p_password,"%s",ibm->p_password);
        for (i = 0; i < 20; i++) amy->p_fill[i] = ibm->p_fill[i];
        return (0);        
}

print_pkt_hdr(struct pkt_hdr *pkt)
{
int i;
        fprintf (log,"Packet Header for %s :\n",pkt_name);
        fprintf (log,"  Orig Node  : %d:%d/%d\n",pkt->p_orig_zone,pkt->p_orig_net,pkt->p_orig_node);
        fprintf (log,"  Dest Node  : %d:%d/%d\n",pkt->p_dest_zone,pkt->p_dest_net,pkt->p_dest_node);
        fprintf (log,"  Date       : Year:%-4d   Month:%-4d Day:%-4d\n               Hour:%-4d   Min  :%-4d Sec:%-4d\n",pkt->p_year,pkt->p_month,pkt->p_day,pkt->p_hour,pkt->p_min,pkt->p_sec);
        fprintf (log,"  Baud       : %d\n",pkt->p_baud);
        fprintf (log,"  Type       : %d\n",pkt->p_type);
        fprintf (log,"  Prod Code  : %d\n",pkt->p_pcode);
        fprintf (log,"  Serial Num : %d\n",pkt->p_psernum);
        fprintf (log,"  Password   : %s\n",pkt->p_password);
        fprintf (log,"  Filler     :");
        for (i = 0; i < 10; i++) fprintf (log," 0x%-2x",pkt->p_fill[i]);
        fprintf (log,"\n              ");
        for (i = 10; i < 20; i++) fprintf (log," 0x%-2x",pkt->p_fill[i]);
        fprintf (log,"\n");
        return (0);
}

print_msg_hdr(struct pak_msg_hdr *msg,int num_msgs, int fr)
{
UBYTE *reasons[] = {
  "OOPS, NOT FILTERED",
  "WHO IT IS FROM",
  "WHO IT IS TO",
  "THE SUBJECT"
};
        fprintf (log,"  Header For Message #%d\n",num_msgs);
        fprintf (log,"     FILTERED BECAUSE OF %s\n",reasons[fr]);
        fprintf (log,"     Type      : %d\n",msg->m_type);
        fprintf (log,"     Orig Node : %d/%d\n",msg->m_orig_net,msg->m_orig_node);
        fprintf (log,"     Dest Node : %d/%d\n",msg->m_dest_net,msg->m_dest_net);
        fprintf (log,"     Flags     : 0x%x\n",msg->m_attribute);
        fprintf (log,"     Cost      : %d\n",msg->m_cost);
        fprintf (log,"     Date      : %s\n",msg_date);
        fprintf (log,"     To        : %s\n",msg_to);
        fprintf (log,"     From      : %s\n",msg_from);
        fprintf (log,"     Subject   : %s\n",msg_subject);
        return (0);
}
