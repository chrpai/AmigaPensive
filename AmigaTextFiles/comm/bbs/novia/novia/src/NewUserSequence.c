#include <novia/novia_global.h>
#include <novia/novia_funcs.h>
#include <pragma/noviasys_lib.h>

ULONG NewUserSequence()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char buffer[50]="                    ";
	char buffer2[50]="                    ";
	char local[10];
	char district[10];
	int counter;
	long temp;
	UBYTE ubyte;
	UWORD uword;
	ULONG ulong;
	BOOL Quit=FALSE,Quit2=FALSE;

				/*** COMPUTER TYPE ***/

	ShowText("work:novia/newuser/comp");
	ioprintf("Enter the # of a computer type.\n");
	Writeio(": ",-1);
	while(Quit==FALSE)
	{
		Getlong((ULONG *)&temp,0,1,0,0);
		if (temp>-1 && temp<5)
		{
			cport->LocalUser.ComputerType=*buffer;
			Quit=TRUE;
		}
		if (*buffer==13 | buffer==0)
		{
			cport->LocalUser.ComputerType=0;
			Quit=TRUE
		}
	}

				/*** TERMINAL LENGTH ***/

	for (counter=40;counter>7;counter--)
	{
		Writelong(counter);LF();
	}
	ShowText("work:novia/newuser/length");
	ioprintf("Enter the number of lines that your\n");
	ioprintf("terminal can display.\n");
	Writeio(": ",-1);
	ubyte=24;
	Getbyte((UBYTE *)buffer,&ubyte,2,0,0);
	if (*buffer==13 | *buffer==0)
	{cport->LocalUser.TermLength=24;}
	else
	{
		StrToLong((STRPTR)buffer,&temp);
		cport->LocalUser.TermLength=(char)temp
		}
	LF();

				/*** TERMINAL WIDTH ***/

	ShowText("work:novia/newuser/width");
	ioprintf("Enter the number of characters per line your terminal can display.\n");
	Writeio(": ",-1);
	ubyte=80;
	Getbyte(buffer,&ubyte,3,0,0);
	if (*buffer==13 | *buffer==0)
	{cport->LocalUser.TermWidth=80;}
	else
	{
		StrToLong((STRPTR)buffer,&temp);
		cport->LocalUser.TermWidth=(char)temp;
	}
	LF();LF();

				/*** ANSI-TEST !!!implementing later!!! ***/

	cport->LocalUser.TermANSI=1;
	cport->LocalUser.TermColors=8;
	cport->LocalUser.TermTabs=0;

	Termsettings(&cport->LocalUser,0);	// Show terminalsetting and
	LF();								// ask for change

				/*** COUNTRY CODE ***/
				
	ioprintf("You will now be asked to enter several personal data items.  Please respond\n");
	ioprintf("truthfully.  All users will be given access to view your Handle and your\n");
	ioprintf("City/Country.  You will be asked whether or not to keep the other information\n");
	ioprintf("private for System Operators' eyes only.\n\n");
	ioprintf("Please use the three letter country identifier that is commonly used in the.\n");
	ioprintf("Olympic games.\n\n");
	ShowText("work:novia/newuser/country");
	Quit=FALSE;
	while(Quit==FALSE)
	{
		LF();Writeio(":> ",-1);
		Getstring(buffer,"GER",3,0,0);
		if (*buffer==68)
		{ShowText("systext:sys.nationals.help");}
		else
		{
//			cport->LocalUser.AreaCode[0]=1;
			Quit=TRUE;
		}
	}
	LF();

				/*** USERNAME ***/

	ShowText("work:novia/newuser/handle");
	Quit=FALSE;
	while(Quit==FALSE)
	{
		Quit2=FALSE;
		while(Quit2==FALSE)
		{
			LF();ioprintf("Enter the handle that you wish to use on this system.\n");
			Writeio(":> ",-1);
			Getstring(buffer,cport->LocalUser.Handle,21,0,0);
			if (strlen((STRPTR)buffer)>4)
			{
				strcpy((STRPTR)cport->LocalUser.Handle,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(cport->LocalUser.Handle,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}

				/*** FRIST AND SECOND NAME ***/

	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("Please enter your real name.\n");
			Writeio(":> ",-1);
			Getstring(buffer,cport->LocalUser.RealName,41,0,0);
			if (strlen((STRPTR)buffer)>4)
			{
				strcpy((STRPTR)cport->LocalUser.RealName,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(cport->LocalUser.RealName,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}

	LF();Writeio("Do you want to keep your REAL NAME private ? ",-1);
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PRealName=TRUE;}
	else
		{cport->LocalUser.PRealName=FALSE;}
				
				/*** ADDRESS ***/

	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("\nPlease enter your Streetname and number.\n");
			Writeio(":> ",-1);
			Getstring(buffer,cport->LocalUser.Street,41,0,0);
			if (strlen((STRPTR)buffer)>4)
			{
				strcpy((STRPTR)cport->LocalUser.Street,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(cport->LocalUser.Street,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}

	LF();Writeio("Do you want to keep your Street private ? ",-1);
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PAddress=TRUE;}
	else
		{cport->LocalUser.PAddress=FALSE;}
				
				/*** CITY ***/

	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("\nPlease enter your City.\n");
			Writeio(":> ",-1);
			Getstring(buffer,cport->LocalUser.City,41,0,0);
			if (strlen((STRPTR)buffer)>2)
			{
				strcpy((STRPTR)cport->LocalUser.City,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(cport->LocalUser.City,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}

					/*** Postal Code ***/

	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("Please enter your PostalCode.\n");
			Writeio(":> ",-1);
			Getstring(buffer,cport->LocalUser.ZipCode,41,0,0);
			if (strlen((STRPTR)buffer)>2)
			{
				strcpy((STRPTR)cport->LocalUser.ZipCode,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(cport->LocalUser.ZipCode,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}

				/*** PHONE ***/

	ShowText("work:novia/newuser/phone");
	strcpy((STRPTR)district,(STRPTR)"06172");
	*local=0;
	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("Enter your COUNTRY/CITY CODES (++ prefix).\n");
			Writeio(":> ",-1);
			Getstring(buffer,district,8,0,0);
			if (strlen((STRPTR)buffer)<9)
			{
				strcpy((STRPTR)district,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		Quit2=FALSE;
		LF();
		while (Quit2==FALSE)
		{
			ioprintf("Enter the LOCAL part of your phone number.\n");
			Writeio(":> ",-1);
			Getstring(buffer,local,8,0,0);
			if (strlen((STRPTR)buffer)<12)
			{
				strcpy((STRPTR)local,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(district,-1);Writeio("-",1);Writeio(local,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}
	sprintf(cport->LocalUser.PhoneNo,"%s-%s",district,local);

	LF();Writeio("Do you want to keep your VOICE PHONE private ? ",-1);
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PPhoneno=TRUE;}
	else
		{cport->LocalUser.PPhoneno=FALSE;}

				/*** MODEM-NUMBER ***/

	ioprintf("Please enter your Modemnumber now.\n");
	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("Enter your COUNTRY/CITY CODES (++ prefix).\n");
			Writeio(":> ",-1);
			Getstring(buffer,district,8,0,0);
			if (strlen((STRPTR)buffer)<9)
			{
				strcpy((STRPTR)district,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		Quit2=FALSE;
		LF();
		while (Quit2==FALSE)
		{
			ioprintf("Enter the LOCAL part of your Modem number.\n");
			Writeio(":> ",-1);
			Getstring(buffer,local,8,0,0);
			if (strlen((STRPTR)buffer)<12)
			{
				strcpy((STRPTR)local,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(district,-1);Writeio("-",1);Writeio(local,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}
	sprintf(cport->LocalUser.ModemNo,"%s-%s",district,local);

	ioprintf("\nDo you want to keep your MODEM NUMBER private ? ");
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PModemno=TRUE;}
	else
		{cport->LocalUser.PModemno=FALSE;}

				/*** FAX-NUMBER ***/

	ioprintf("Please enter your Faxnumber now.\n");
	Quit=FALSE;
	LF();
	while (Quit==FALSE)
	{
		Quit2=FALSE;
		while (Quit2==FALSE)
		{
			ioprintf("\nEnter your COUNTRY/CITY CODES (++ prefix).\n");
			Writeio(":> ",-1);
			Getstring(buffer,district,8,0,0);
			if (strlen((STRPTR)buffer)<9)
			{
				strcpy((STRPTR)district,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		Quit2=FALSE;
		LF();
		while (Quit2==FALSE)
		{
			ioprintf("\n\nEnter the LOCAL part of your FAX number.\n");
			Writeio(":> ",-1);
			Getstring(buffer,local,8,0,0);
			if (strlen((STRPTR)buffer)<12)
			{
				strcpy((STRPTR)local,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writeio(district,-1);Writeio("-",1);Writeio(local,-1);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit=TRUE;
	}
	sprintf(cport->LocalUser.FaxNo,"%s-%s",district,local);

	LF();Writeio("Do you want to keep your FAX NUMBER private ? ",-1);
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PFaxno=TRUE;}
	else
		{cport->LocalUser.PFaxno=FALSE;}
		
				/*** BIRTHDAY ***/
	Quit2=FALSE;
	ioprintf("Your birthdate is required to grant you access to areas of this system which\n");
	ioprintf("require you to be over a certain minimum age, or under a certain maximum age.\n");
	ioprintf("The system operators can not accept responsibilty for a user's\n");
	ioprintf("mis-representation of his age.  Please answer truthfully.\n\n");
	ioprintf("Enter the year during which you were born:\n\n");
	while (Quit2==FALSE)
	{
		Quit=FALSE;
		while(Quit==FALSE)
		{
			Writeio(">: ",-1);
			Getword(&uword,0,4,0,0);
			if (uword>1800 && uword<3000)
			{
				cport->LocalUser.Birthday.year=(WORD)uword;
				Quit=TRUE;
			}
		}
	
		LF();
		ioprintf("01.....Jan     02.....Feb     03.....Mar     04.....Apr     05.....May\n");
		ioprintf("06.....Jun     07.....Jul     08.....Aug     09.....Sep     10.....Oct\n");  
		ioprintf("11.....Nov     12.....Dec\n\n");
		ioprintf("Enter the number of the month during which you were born:\n\n");
		Quit=FALSE;
		while(Quit==FALSE)
		{
			LF();Writeio(">: ",-1);
			Getbyte(&ubyte,0,4,0,0);
			if (ubyte>0 && ubyte<13)
			{
				cport->LocalUser.Birthday.month=(WORD)temp;
				Quit=TRUE;
			}
		}
		LF();
		ioprintf("Enter the day of the month on which you were born:\n");
		Quit=FALSE;
		while(Quit==FALSE)
		{
			Writeio(">: ",-1);
			Getbyte(&ubyte,0,4,0,0);
			if (ubyte>0 && ubyte<32)
			{
				cport->LocalUser.Birthday.day=(WORD)temp;
				Quit=TRUE;
			}
		}
		LF();LF();Writeio("Is ",-1);
		Writelong((ULONG)cport->LocalUser.Birthday.day);Writeio("-",1);
		Writelong((ULONG)cport->LocalUser.Birthday.month);Writeio("-",1);
		Writelong((ULONG)cport->LocalUser.Birthday.year);
		Writeio(" correct ? ",-1);
		if (Yesno(TRUE,0)==TRUE)Quit2=TRUE;
	}

	ioprintf("\nDo you want to keep your BIRTHDAY private ?\n");
	if (Yesno(TRUE,0)==TRUE)
		{cport->LocalUser.PBirthday=TRUE;}
	else
		{cport->LocalUser.PBirthday=FALSE;}

	Quit=FALSE;
	while (Quit==FALSE)
	{
		ioprintf("What is your gender? \n");
		ioprintf(" 0) Female                               1) Male\n");
		Writeio(":> ",-1);
		Getbyte(&ubyte,0,1,0,0);
		if (ubyte==0 | ubyte==1)
		{
			cport->LocalUser.Sex=*buffer;
			Quit=TRUE;
		}
	}

				/*** PASSWORD ***/

	ShowText("Work:novia/newuser/password");
	Quit=FALSE;
	while(Quit==FALSE)
	{
		Quit2=FALSE;
		while(Quit2==FALSE)
		{
			LF();Writeio("Enter the password that you would like to use.   ",-1);
			Writeio(":> ",-1);
			Getstring(buffer,0,20,0,0);
			if (strlen((STRPTR)buffer)>5)
			{
				strcpy((STRPTR)cport->LocalUser.PassWord,(STRPTR)buffer);
				Quit2=TRUE;
			}
		}
		LF();Writeio("Please Enter your Password too, for verification.",-1);
		Getstring(buffer2,0,20,0,0);
		if (FindArg((STRPTR)buffer,(STRPTR)buffer2)==0)
		{
			strcpy(cport->LocalUser.PassWord,buffer);
			Quit=TRUE;
		}
	}

/*	Writeln("
	Writeln(" 1) Handle  :  Campi                     2) Name    : *Thorsten Gehler
	Writeln(" 3) Address : *Frankfurter Landstr. 78   4) City    :  Bad Homburg
	Writeln(" 5) ZipCode :  61352                     6) Country :  GER
	Writeln(" 7) Voice#  : *06172-457063              8) Data#   : *06172-457063
	Writeln(" 9) BirthDay: *Mon  5-Apr-1976          10) Gender  :  Male
	Writeln("11) Organiz.:  No                       12) Password: *HELPMY
	Writeln("
	Writeln("Enter the # of the item to change, or press ENTER to continue.
	Writeln(": 
	Writeln("
	Writeln("There are NUMEROUS ways for you to customize your use of this system.  To
	Writeln("change your "system preferences" as they are called, use the EDIT PREFS
	Writeln("command once you are logged on.  For now, only the more important ones will
	Writeln("be introduced.
	Writeln("
	Writeln("Press RETURN if the time is correct.  Otherwise, press "=" to add an hour, or
	Writeln(""-" to subtract an hour until it is correct.
	Writeln("
	Writeln("Sat 27-Sep-1997 17:34 
	Writeln("
	Writeln("Often, it's hard to read text as fast as it scrolls off of your screen. The
	Writeln(""more mode" option will cause text to automatically PAUSE every time the screen
	Writeln("becomes full with new text.
	Writeln("
	Writeln("Do you want the "More?" mode enabled [Yes]? Yes
	Writeln("
	Writeln("Following are several questions whose answers will appear in a file which all
	Writeln("users have access to.  Note that you can change the answers to these questions
	Writeln("once your are logged on by using the EDIT FINGER command.
	Writeln("
	Writeln("Each question may be answered with up to 10 lines.  Only press RETURN at the
	Writeln("end of your response.
	Writeln("
	Writeln("What is your occupation?
	Writeln(":
	Writeln("
	Writeln("What is your occupation?
	Writeln(":.
	Writeln("
	Writeln("What type of computer equipment do you use?
	Writeln(":.
	Writeln("
	Writeln("What are some of your other interests?
	Writeln(":.
	Writeln("
	Writeln("How did you find out about our system?
	Writeln(":.
	Writeln("
	Writeln("Do you run a BBS?  If so, what's the name and number?
	Writeln(":.
	Writeln("
	Writeln("Following are several questions which only the System Operator will have access
	Writeln("to.  Your answers will be kept strictly confidential.
	Writeln("
	Writeln("Each question may be answered with up to 10 lines.  Only press RETURN at the
	Writeln("end of your response.
	Writeln("
	Writeln("Is there anything that hasn't been asked yet that you think the System
	Writeln("Operators should know about?
	Writeln(":.
	Writeln("
	Writeln("Filing your account information...
	Writeln("
	Writeln("
	Writeln("======================================
	Writeln("
	Writeln("Your Handle      --> Campi
	Writeln("Your Password    --> HELPMY
	Writeln("
	Writeln("======================================
	Writeln("
	Writeln("Make a note of these!  You will need them to signon to the system again.
	Writeln("
	Writeln("YES to continue [No]?Y
	Writeln("
	Writeln("
	Writeln("Note: this port will perform off-line maintenance in 625.1 minutes.
	Writeln("
	Writeln("@ Cleared for logon
	*/
	AddUser(&cport->LocalUser);
	return TRUE;
}
