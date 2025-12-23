#include <exec/types.h>
#include <exec/exec.h>
#include <dos/exall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include "logmaint_rev.h"

char *ver=VERSTAG;

#define DAYS_DEFAULT 7

BYTE *Template="Days/N";

void main()
	{
		struct RDArgs *RDArgs;
		LONG ArgArray[]={DAYS_DEFAULT};
		if(!(RDArgs=ReadArgs(Template,ArgArray,NULL)))
			exit(RETURN_FAIL);
		{
			ULONG days;
			struct DateStamp CurrentDate;
			days=(ULONG) *((LONG *) ArgArray[0]);
			DateStamp(&CurrentDate);
			{
				BPTR lock;
				if(lock=Lock("WWBBS:Logs",SHARED_LOCK))
					{
						struct ExAllControl *eac;
						struct ExAllData *ead,*eadata;
						int more;
						if(eac=AllocDosObject(DOS_EXALLCONTROL,NULL))
							{
								eac->eac_LastKey=0;
								if(eadata=AllocMem(sizeof(struct ExAllData)*16,MEMF_CLEAR))
									{
										do
											{
												more=ExAll(lock,eadata,sizeof(struct ExAllData)*16,ED_NAME,eac);
												if((!more) && (IoErr()!=ERROR_NO_MORE_ENTRIES))
													break;
												if(eac->eac_Entries==0)
													continue;
												{
													ead=eadata;
													do
														{
															{
																BPTR fh_in,fh_out;
																char filename[256],buff[256];
																int ok=FALSE;
																sprintf(filename,"WWBBS:Logs/%s",ead->ed_Name);
																if(fh_in=Open(filename,MODE_OLDFILE))
																	{
																		if(fh_out=Open("T:logmaint",MODE_NEWFILE))
																			{
																				while(FGets(fh_in,buff,255))
																					FPuts(fh_out,buff);
																				ok=TRUE;
																				Close(fh_out);
																			}
																		Close(fh_in);
																	}
																if(ok)
																	{
																		struct DateTime dt;
																		char date[9];
																		if(fh_in=Open("T:logmaint",MODE_OLDFILE))
																			{
																				if(fh_out=Open(filename,MODE_NEWFILE))
																					{
																						while(FGets(fh_in,buff,255))
																							{
																								sscanf(buff,"%s",date);
																								dt.dat_Format=FORMAT_USA;
																								dt.dat_Flags=NULL;
																								dt.dat_StrDate=date;
																								dt.dat_StrTime=NULL;
																								StrToDate(&dt);
																								if(dt.dat_Stamp.ds_Days+days>=CurrentDate.ds_Days)
																									FPuts(fh_out,buff);
																							}
																						Close(fh_out);
																					}
																				Close(fh_in);
																			}
																		DeleteFile("T:logmaint");
																	}
															}
															ead=ead->ed_Next;
														} while(ead);
												}
											} while(more);
										FreeMem(eadata,sizeof(struct ExAllData)*16);
									}
								FreeDosObject(DOS_EXALLCONTROL,eac);
							}
						UnLock(lock);
					}
			}
		}
		exit(RETURN_OK);
	}
