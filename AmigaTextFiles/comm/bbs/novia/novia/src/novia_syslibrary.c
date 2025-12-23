#include <exec/types.h>

#include <stdlib.h>
#include <string.h>
#include <exec/lists.h>
#include <pragma/exec_lib.h>
#include <exec/memory.h>
#include <exec/tasks.h>
#include <exec/libraries.h>
#include <novia/novia_funcs.h>
#include <novia/novia_registers.h>
#include <novia/novia_portdata.h>

#include "protos/novia_dos_protos.h"
#include "protos/novia_io_protos.h"
#include "protos/novia_tcpip_protos.h"

extern SignalSemaphore *rt_semaphore;

char *id_string	= "noviasys.library $V1.1 (_DATE_) © 1999 by Thorsten Gehler";
char *libname		= "noviasys.library";

extern struct Library *NoviaSysBase;


struct NoviaDosProcess
{
	char *name;
	char *filename;
	struct PortData *cport;
	struct Task     *dostask;
	struct Task		 *rttask;
	struct Task		 *client_task;
	char isrunning;
	char canclose;
};


struct Library * __saveds ASM nslib_LibOpen(register __a6 struct Library *MyBase)
{
	struct Library *result = NULL;
	struct Task *owntask = FindTask(NULL);
	if (rt_semaphore)
	{
		if (rt_semaphore->ss_Owner)
		{
			struct PortData *cport = ((NoviaDosProcess *)rt_semaphore->ss_Owner->tc_UserData)->cport;
			owntask->tc_UserData = cport;
			if (cport)
			{
				NoviaSysBase->lib_OpenCnt++;
				NoviaSysBase->lib_Flags &= ~LIBF_DELEXP;
				result = NoviaSysBase;
				if (cport->ConPort)
					cport->ConPort->mp_SigTask 	= owntask;
				if (cport->LocalPort)
					cport->LocalPort->mp_SigTask	= owntask;
				if (cport->rexxport)
					cport->rexxport->mp_SigTask	= owntask;
				if (cport->TimePort)
					cport->TimePort->mp_SigTask	= owntask;
				if (cport->rtport)
					cport->rtport->mp_SigTask		= owntask;
				if (cport->nfs_port)
					cport->nfs_port->mp_SigTask	= owntask;
			}
			ReleaseSemaphore(rt_semaphore);
		}
	}
	return(result);
}

SEGLISTPTR __saveds ASM nslib_LibClose( register __a6 struct Library *MyBase)
{
	NoviaSysBase->lib_OpenCnt--;
	return(NULL);
}

SEGLISTPTR __saveds ASM nslib_LibExpunge( register __a6 struct Library *MyBase)
{
	if (NoviaSysBase->lib_OpenCnt)
		return NULL;

	// bye bye my nice library 

	Remove((Node *)NoviaSysBase);
	FreeMem(((char *)NoviaSysBase)-NoviaSysBase->lib_NegSize, NoviaSysBase->lib_NegSize + NoviaSysBase->lib_PosSize);
	return(NULL);
}

ULONG ASM nslib_LibExtfunc(void)
{
	return(NULL);
}

struct PortData *nslib_InitPort()
{
	printf("nslib_InitPort\n");
   return 0;
}

struct PortData *nslib_ClosePort()
{
	printf("nslib_ClosePort\n");
   return 0;
}

BOOL *nslib_HangingUP()
{
	printf("Lib HangingUP\n");
   return 0;
}

void InsertFunction(APTR base, ULONG Offset, APTR func)
{
	char  *address			= ((char *)base)-Offset;
	char  **func_address  = (char **)(address + 2);
	address[0]=0x4e;
	address[1]=0xf9;
	*func_address = (char *)func;
}

void CreateNoviaLibrary()
{
	char *base;
	ULONG negsize = 600;
	ULONG possize = sizeof(Library) + strlen(id_string) + 1 + strlen(libname) + 1;
	
	char *l_id_string;
	char *l_libname;

/*	static APTR func_table[] =
	{
		nslib_LibOpen,				// -6
		nslib_LibClose,			// -12
		nslib_LibExpunge,			// -18 4ef9
		nslib_LibExtfunc,			// -24
		nslib_LibExtfunc,			// -30
		nslib_InitPort,			// -36
		(APTR)~1
	};*/

	if ((base = AllocMem(negsize+possize,MEMF_ANY|MEMF_CLEAR)))
	{
		printf("create noviasys.library: 0x%x\n",NoviaSysBase);
		NoviaSysBase = (Library *)(base+negsize);

		InsertFunction(NoviaSysBase,	6,	 nslib_LibOpen);
		InsertFunction(NoviaSysBase,	12, nslib_LibClose);				// -12
		InsertFunction(NoviaSysBase,	18, nslib_LibExpunge);			// -18
		InsertFunction(NoviaSysBase,	24, nslib_LibExtfunc);			// -24
		InsertFunction(NoviaSysBase,	30, nslib_LibExtfunc);			// -30
		InsertFunction(NoviaSysBase,	36, nslib_InitPort);				// -36

		InsertFunction(NoviaSysBase,  60, nslib_ShowText);
		InsertFunction(NoviaSysBase,  66, nslib_ShowPart);
		InsertFunction(NoviaSysBase,  72, nslib_SystemMessage);
		InsertFunction(NoviaSysBase,  78, nslib_SetColor);
		InsertFunction(NoviaSysBase,  84, nslib_SetBColor);
		InsertFunction(NoviaSysBase,  90, nslib_space);
		InsertFunction(NoviaSysBase,  96, nslib_cleft);
		InsertFunction(NoviaSysBase, 102, nslib_cright);
		InsertFunction(NoviaSysBase, 108, nslib_LF);
		InsertFunction(NoviaSysBase, 114, nslib_Getstring);
		InsertFunction(NoviaSysBase, 120, nslib_Getbyte);
		InsertFunction(NoviaSysBase, 126, nslib_Getword);
		InsertFunction(NoviaSysBase, 132, nslib_Getlong);
		InsertFunction(NoviaSysBase, 138, nslib_Writeio);
		InsertFunction(NoviaSysBase, 144, nslib_Writelong);
		InsertFunction(NoviaSysBase, 150, nslib_CSI);
		InsertFunction(NoviaSysBase, 156, nslib_JumpXY);
		InsertFunction(NoviaSysBase, 162, nslib_CLS);
		InsertFunction(NoviaSysBase, 168, nslib_Getline);
		InsertFunction(NoviaSysBase, 174, nslib_Yesno);
		InsertFunction(NoviaSysBase, 180, nslib_AskKey);
		InsertFunction(NoviaSysBase, 186, nslib_GetChar);
		InsertFunction(NoviaSysBase, 192, nslib_WaitKey);
		InsertFunction(NoviaSysBase, 198, nslib_ChangeScreenStatus);
		InsertFunction(NoviaSysBase, 204, nslib_ErrorMessage);
		InsertFunction(NoviaSysBase, 210, nslib_GetCurserPos);
		InsertFunction(NoviaSysBase, 216, nslib_FreeMsg);
		InsertFunction(NoviaSysBase, 222, nslib_ReturnMsg);
		InsertFunction(NoviaSysBase, 228, nslib_ClearIOBuffer);
		InsertFunction(NoviaSysBase, 234, nslib_vioprintf);
		InsertFunction(NoviaSysBase, 240, nslib_PrintDosError);
		InsertFunction(NoviaSysBase, 246, nslib_execute_nsl);
		InsertFunction(NoviaSysBase, 252, nslib_Datecode2Date); 
		InsertFunction(NoviaSysBase, 258, nslib_GetTime);
		InsertFunction(NoviaSysBase, 264, nslib_DateToString);

		InsertFunction(NoviaSysBase, 270, nslib_CalcDatecode);
		InsertFunction(NoviaSysBase, 276, nslib_StringToDate);
		InsertFunction(NoviaSysBase, 282, nslib_SearchDateKey);
		InsertFunction(NoviaSysBase, 288, nslib_getstr);


		InsertFunction(NoviaSysBase, 294, nslib_closeroot);
		InsertFunction(NoviaSysBase, 300, nslib_openroot);				// basic dos functions
		InsertFunction(NoviaSysBase, 306, nslib_parrentdir);
		InsertFunction(NoviaSysBase, 312, nslib_changedir);
		InsertFunction(NoviaSysBase, 318, nslib_mkdir);
		InsertFunction(NoviaSysBase, 324, nslib_closedir);
		InsertFunction(NoviaSysBase, 330, nslib_delete);
		InsertFunction(NoviaSysBase, 336, nslib_editflags);
		InsertFunction(NoviaSysBase, 342, nslib_info);
		InsertFunction(NoviaSysBase, 348, nslib_rename);
		InsertFunction(NoviaSysBase, 354, nslib_getlist);

		InsertFunction(NoviaSysBase, 360, nslib_load_ndos_object);
		InsertFunction(NoviaSysBase, 366, nslib_save_ndos_object);

		InsertFunction(NoviaSysBase, 372, nslib_list);
		InsertFunction(NoviaSysBase, 378, nslib_execute);
		InsertFunction(NoviaSysBase, 384, nslib_get_ieb);

		InsertFunction(NoviaSysBase, 390, nslib_Monitor_open);
		InsertFunction(NoviaSysBase, 396, nslib_Monitor_close);
		InsertFunction(NoviaSysBase, 402, nslib_opengui);
		InsertFunction(NoviaSysBase, 408, nslib_closegui);
		InsertFunction(NoviaSysBase, 414, nslib_getmainportconfig);

		InsertFunction(NoviaSysBase, 420, nslib_addmail);
		InsertFunction(NoviaSysBase, 426, nslib_addreply);
		InsertFunction(NoviaSysBase, 432, nslib_additementry);
		InsertFunction(NoviaSysBase, 438, nslib_additementry2dir);
		InsertFunction(NoviaSysBase, 444, nslib_addmail2dir);
		InsertFunction(NoviaSysBase, 450, nslib_readmessage);
		InsertFunction(NoviaSysBase, 456, nslib_uucp_sendmail);
		InsertFunction(NoviaSysBase, 462, nslib_uucp_getmails);
//		InsertFunction(NoviaSysBase, 468, nslib_uucp_startmailer);

//		InsertFunction(NoviaSysBase, 474, nslib_DoNFSPacket);
//		InsertFunction(NoviaSysBase, 480, nslib_SendNFSPacket);
//		InsertFunction(NoviaSysBase, 486, nslib_WaitNFSPacket
//		InsertFunction(NoviaSysBase, 492, nslib_AbortNFSPacket
//		InsertFunction(NoviaSysBase, 498, nslib_ReplyNDSPacket
		InsertFunction(NoviaSysBase, 504, nslib_readmailbox);

		InsertFunction(NoviaSysBase, 510, nslib_loadselectlist);
		InsertFunction(NoviaSysBase, 516, nslib_saveselectlist);
		InsertFunction(NoviaSysBase, 522, nslib_tstselect);

		InsertFunction(NoviaSysBase, 528, nslib_SearchUser);
		InsertFunction(NoviaSysBase, 534, nslib_FindUser);
		InsertFunction(NoviaSysBase, 540, nslib_GetUserByIP);
		InsertFunction(NoviaSysBase, 546, nslib_SaveUser);
		InsertFunction(NoviaSysBase, 552, nslib_AddUser);
		InsertFunction(NoviaSysBase, 558, nslib_LoadUser);
		InsertFunction(NoviaSysBase, 564, nslib_CheckPassword);

		InsertFunction(NoviaSysBase, 570, nslib_CopyUUCPName);
		InsertFunction(NoviaSysBase, 576, nslib_GetUUCPMonth);
		InsertFunction(NoviaSysBase, 582, nslib_ConvertUUCPDate);
		InsertFunction(NoviaSysBase, 588, nslib_sendmail);
		InsertFunction(NoviaSysBase, 594, nslib_uucp_import_mail);
//		InsertFunction(NoviaSysBase, 600, 

		l_id_string = ((char *)NoviaSysBase + sizeof(Library));
		l_libname   = l_id_string + strlen(id_string) + 1;
		strcpy(l_id_string,id_string);
		strcpy(l_libname,libname);
		NoviaSysBase->lib_Node.ln_Type = NT_LIBRARY;
		NoviaSysBase->lib_Node.ln_Name = l_libname;
		NoviaSysBase->lib_pad			 = 0;
		NoviaSysBase->lib_NegSize		 = negsize;
		NoviaSysBase->lib_PosSize		 = possize;
		NoviaSysBase->lib_Flags			 = LIBF_CHANGED | LIBF_SUMUSED;
		NoviaSysBase->lib_Version		 = 1.0;
		NoviaSysBase->lib_Revision		 = 1.0;
		NoviaSysBase->lib_IdString		 = l_id_string;

		// *** online at last!
		AddLibrary(NoviaSysBase);
	}
	return;
}



void RemoveNoviaLibrary(struct Library *lib)
{
//	RemLibrary(lib);
	Remove((Node *)NoviaSysBase);
	FreeMem(((char *)NoviaSysBase)-NoviaSysBase->lib_NegSize, NoviaSysBase->lib_NegSize + NoviaSysBase->lib_PosSize);
}









