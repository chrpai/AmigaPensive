#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <stdio.h>
#include <string.h>
#include <novia/novia_portdata.h>
#include <novia/novia_config.h>
#include <novia/novia_mail.h>

struct Library *NoviaSysBase;

LONG nslib_forward_uucp_message(INet_MailHeader *header, BPTR fh_text, ULONG UserID);

void ioprintf(const char *string, ...)
{
	char *buffer=AllocVec(10000,MEMF_ANY|MEMF_CLEAR);
	if (buffer)
	{
		vsprintf(buffer, string, unsigned int(&string + 1));
		Writeio(buffer,-1);
		FreeVec(buffer);
	}
}

int mystrcmp(char *str1, char *str2)
{
	char *ptr1 = str1;
	char *ptr2 = str2;
	int result = TRUE;
	while (isspace(*ptr1))
		ptr1++;
	while (result && *ptr1 && *ptr2)
	{
		if (toupper(*ptr1) == toupper(*ptr2))
		{
			ptr1++; ptr2++;
		}
		else
		{
			if (!isspace(*ptr1) | *ptr2 != 0)
				result = FALSE;
		}
	}
	return(result);
}


void getuser_from_host(char *src, char *username)
{
	char *ptr			= src;
	char *usernameptr	= username;
	while (*ptr && *ptr !='@')
		ptr++;
	if (*ptr)
	{
		while (ptr >= src && !isspace(*ptr) && *ptr != '<')
			ptr--;
		if (isspace(*ptr) | *ptr == '<')
			ptr++;
		while (*ptr && *ptr !='@')
			*usernameptr++ = *ptr++;
		*usernameptr = 0;
	}
}


void main()
{
	if ((NoviaSysBase = OpenLibrary("noviasys.library", 0)))
	{
		struct MainPortConfig *mainportconfig;
		struct PortData       *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
		if ((mainportconfig = getmainportconfig()))
		{
			struct INet_POP3Server *server = (struct INet_POP3Server *)mainportconfig->POP3ServerList.mlh_Head;
			struct INet_MailHeader *header;
			if ((header = AllocVec(sizeof(INet_MailHeader), MEMF_ANY)))
			{
				char buffer[128];
				BPTR fh_index;
				BPTR fh_text;
				char	*fullpath;
				ULONG length_fp;
				char to_found = FALSE;
				char to[80];
				ULONG userid;

				while (server->ln_Succ)
				{
					uucp_getmails(server);
					fullpath		= server->InboundPath;
					length_fp	= strlen(fullpath);

					if (length_fp)
					{
						if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
							sprintf(buffer,"%s.index",fullpath);
						else
							sprintf(buffer,"%s/.index",fullpath);
						if ((fh_index = Open(buffer, MODE_READWRITE)))
						{
							while (Read(fh_index, header, sizeof(INet_MailHeader)) == sizeof(INet_MailHeader))
							{
								if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
									sprintf(buffer,"%s%s",fullpath, header->filename);
								else
									sprintf(buffer,"%s/%s",fullpath, header->filename);
								ioprintf("error: %d\n",ndos_uucp_import_mail(buffer));
							}
							Close(fh_index);
						}
						else
							ioprintf("can't open inbound path.\n");
					}
					else
						ioprintf("inbound path not set\n");
					server = server->ln_Succ;
				}
				FreeVec(header);
			}
		}
	}
}



LONG nslib_forward_uucp_message(INet_MailHeader *header, BPTR fh_text, ULONG UserID)
{
	LONG result = NULL;
	struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
	if (cport && fh_text && UserID)
	{
		char buffer[256];
		char quit = FALSE;
		struct NewMail   *newmail;
		if ((newmail = AllocVec(sizeof(struct NewMail), MEMF_ANY | MEMF_CLEAR)))
		{
			if (Seek(fh_text, 0, OFFSET_BEGINNING) != -1)
			{
				newmail->ToUser		= UserID;
				newmail->PrivateMail = TRUE;
				newmail->MailType		= MAILTYPE_UUCP;
				newmail->NetworkType = MAILTYPE_UUCP;
	
				newmail->ItemType		= 0;
				while (!quit && FGets(fh_text, buffer, 255))
				{	
//					ioprintf("read: %s",buffer);
					if (mystrcmp(buffer, "From:") )
					{
						if (!newmail->FromRealName && !newmail->From)
						{
							if ((newmail->FromRealName = AllocVec(80, MEMF_ANY)))
							{
								if ((newmail->From = AllocVec(80, MEMF_ANY)))
								{
									CopyUUCPName(buffer, newmail->FromRealName, newmail->From);
									printf("FROM: <%s> <%s>\n",newmail->FromRealName, newmail->From);
								}
								else
								{
									FreeVec(newmail->FromRealName);
									newmail->FromRealName = NULL;
								}
							}
						}
					}
					else if (mystrcmp(buffer, "X-Envelope-To:") )
					{
						if (!newmail->ToRealName && !newmail->To)
						{
							if ((newmail->ToRealName = AllocVec(80, MEMF_ANY)))
							{
								if ((newmail->To = AllocVec(80, MEMF_ANY)))
								{
									CopyUUCPName(buffer, newmail->ToRealName, newmail->To);
									printf("X-Envelope-To: <%s> <%s>\n",newmail->ToRealName, newmail->To);
								}
								else
								{
									FreeVec(newmail->ToRealName);
									newmail->ToRealName = NULL;
								}
							}
						}
					}
					else if (mystrcmp(buffer, "Organization:"))
					{
						if (!newmail->Organization)
						{
							if ((newmail->Organization = AllocVec(sizeof(buffer), MEMF_ANY | MEMF_CLEAR)))
							{
								char *ptr = buffer;
								char *org = newmail->Organization;
								while (*ptr && *ptr != ':')
									ptr++;
								if (*ptr)
								{
									ptr++;
									while (isspace(*ptr))
										ptr++;
									while (*ptr && *ptr != 13 && *ptr != 10)
										*org++ = *ptr++;
									*org = 0;
								}
								else
								{
									FreeVec(newmail->Organization);
									newmail->Organization = 0;
								}
							}
						}
					}
					else if (mystrcmp(buffer, "Date:"))
					{
						Date *date = &newmail->SendDate;
						ConvertUUCPDate(buffer, &newmail->SendDate);
					}
					else if (mystrcmp(buffer, "Subject:"))
					{
						if (!strlen(newmail->subject))
						{
							char *ptr = buffer;
							while (*ptr && *ptr != ':')
								ptr++;
							if (*ptr)
							{
								ptr++;
								while (isspace(*ptr))
									ptr++;
								strncpy(newmail->subject,ptr,120);
							}
						}
						printf("SUBJECT: >%s>\n",newmail->subject);
					}
					if (*buffer == 10 | *buffer == 13)
					{
						quit = TRUE;
					}
				}
				if ((newmail->text = AllocVec(header->size, MEMF_ANY | MEMF_CLEAR)))
				{
					char *textptr = newmail->text;
					char *endptr  = newmail->text + (header->size - 1);
					char *bufptr;
					quit = FALSE;
					if (Seek(fh_text, 0, OFFSET_BEGINNING) != -1)
					{
						while (!quit)
						{
							
							if (FGets(fh_text, buffer, 255))
							{
								bufptr = buffer;
								while (*bufptr && *bufptr != 13 && *bufptr != 10)
									*textptr++ = *bufptr++;
								if (*bufptr == 10 | *bufptr == 13)
									*textptr++ = 10;
							}
							else
								quit = TRUE;
						}
						*textptr = 0;
						newmail->length = textptr - newmail->text;
						ioprintf("send message to <%s> %s, %d bytes.\n",newmail->ToRealName, newmail->To, newmail->length);
						ndos_sendmail(newmail);
					}
				}
			}
		}
	}
	return (result);
}
