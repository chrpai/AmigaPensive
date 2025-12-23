const char *_VERSION="\0$VER: Convert 1.0 (23.03.96)";

#include	<dos/dos.h>

FILE	*fp1,*fp2;

char	line[200],
		gamespfad1[23]="pfiles:flut/flut.games",
		gamespfad2[15]="ram:flut.games";

UBYTE	x,anzahl,xx,yy;

void main(void)
{
	if (fp1=fopen(gamespfad1,"r"))
	{
		if (fp2=fopen(gamespfad2,"w"))
		{
			while (fgets(line,sizeof(line),fp1))
			{
				anzahl=line[0]-48;
				xx=atoi(line + 2);
				yy=atoi(line + 5);

				if(anzahl>0 && yy>8)
				{
					for(x=0;x<anzahl*2+2;x++)
					{
						fgets(line,sizeof(line),fp1);
					}
					line[0]='0';
					line[1]=10;
					line[2]=0;
					anzahl=0;
				}

				if(anzahl>0)
				{
					strcpy(line+8,line+19);
					fputs(line,fp2);

					fgets(line,sizeof(line),fp1);
					fputs(line,fp2);
					fgets(line,sizeof(line),fp1);
					fputs(line,fp2);

					for(x=0;x<anzahl;x++)
					{
						fgets(line,sizeof(line),fp1);
						line[9]=32;
						line[10]=10;
						line[11]=0;
						fputs(line,fp2);
						fgets(line,sizeof(line),fp1);
						fputs(line,fp2);
					}
				}
				else
				{
					fputs(line,fp2);
				}
			}
			fclose(fp2);
		}
		fclose(fp1);
	}
}

