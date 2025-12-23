const char *_VERSION="\0$VER: Creator 1.2 (07.08.95)";

#include <dos/dos.h>
#include <stdio.h>

FILE	*fr1,*fr2,*fw1,*fw2;

char	indexpfad[81]="pfiles:cdnet/",
		treepfad[81]="pfiles:cdnet/",
		tree[81]="pfiles:cdnet/tree",
		createpfad[81]="makedir >nil: pfiles:cdnet/",
		cdfilespfad[81],
		index_name[81],
		leer[81]="                                                                                ",

		line_a[150],*p,*a[3],
		line_b[150];

BOOL	found;

UBYTE	x,y,i,j,k,

		ipos[3],ilen[3];

// 1: name dir
// 2: pfad cd
// 3: name index
// 4: pos name
// 5: len name
// 6: pos size
// 7: len size
// 8: pos desc
// 9: len desc

main(int argc,char **argv)
{
	if(argc!=10)
	{
		printf("\nCreator v1.2\nUsage: Creator <Dir-Name> <CD-Rom-Path> <Index-Name> <Pos_Name> <Len_Name> <Pos_Size> <Len_Size> <Pos_Desc> <Len_Desc>\n\nExample: Creator Aminet6 Aminet6:Aminet index 1 18 30 5 40 48\n\n");
	}
	else
	{
		for(i=0;i<3;i++)
		{
			ipos[i]=atoi(argv[i*2+4]);
			ilen[i]=atoi(argv[i*2+5]);

			if(ipos[i]>0)
			{
				a[i]=line_a+ipos[i]-1;
			}
			else
			{
				a[i]=leer;
			}
		}

		AddPart(indexpfad,argv[1],80);
		AddPart(treepfad,argv[1],80);
		AddPart(indexpfad,"index",80);
		AddPart(treepfad,"tree",80);

		strcpy(cdfilespfad,argv[2]);

		AddPart(createpfad,argv[1],80);

		system(createpfad);

		if (fr1=fopen(tree,"r"))
		{
			if (fw1=fopen(treepfad,"wb"))
			{
				if (fw2=fopen(indexpfad,"wb"))
				{
					while(fgets(line_a+1,sizeof(line_a)-1,fr1))
					{
						line_a[0]=' ';
						p=line_a+1;
						while(*p!=' ')
						{
							p++;
						}
						*p=0;
						strcpy(index_name,cdfilespfad);
						AddPart(index_name,line_a+1,80);
						AddPart(index_name,argv[3],80);

						if(line_a[5]!=' ' && line_a[5]!=0 && line_a[1]!=10 && strnicmp(line_a+1,"info",4)!=0)
						{
							if (fr2=fopen(index_name,"r"))
							{
								*p=' ';
								fputs(line_a,fw1);
								*p=10;
								p++;
								*p=0;
								line_a[0]='#';
								fputs(line_a,fw2);
								while(fgets(line_a,sizeof(line_a),fr2))
								{
									if(line_a[0]!='|')
									{
										for(i=0;i<3;i++)
										{
											if(ilen[i]>0)
											{
												line_a[ipos[i]+ilen[i]-1]=0;
											}
										}
										sprintf(line_b," %-22.22s %8.8s %-.46s\0",a[0],a[1],a[2]);
										i=strlen(line_b);
										if(i==79)
										{
											line_b[i-1]=10;
											line_b[i]=0;
										}
										fputs(line_b,fw2);
									}
								}
								fclose(fr2);
							}
						}
					}
					fclose(fw2);
				}
				fclose(fw1);
			}
			fclose(fr1);
		}
	}
}

