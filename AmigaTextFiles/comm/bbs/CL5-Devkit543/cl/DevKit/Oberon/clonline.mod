(* --------------------------------------------------------------------------
 :Program.       clonline.mod
 :Contents.      Interface zur clonline.library
 :Author.        Frank Schummertz (fs)
 :Address.       Katernberger Schulweg 35, 42113 Wuppertal
 :Phone.         0202 / 761780
 :Copyright.     fs
 :Language.      Oberon2
 :Translator.    AmigaOberon 3.00d
 :Version.       0.1
 :Support.       
 :Imports.       
 :Remarks.       für clonline.library v2.3 (c) Mathis Mischler
 :Usage.         
 :Bugs.          
-------------------------------------------------------------------------- *)

MODULE CLOnline;

IMPORT
  sys : SYSTEM,
  e   : Exec,
  i   : Intuition,
  l   : Locale,
  t   : Timer,
  u   : Utility;


VAR
  clonline * : e.LibraryPtr;

CONST
  clonlinename    * = "CONNECTLINE:libs/clonline.library";
  clonlineversion * = 2;

(* Definition of a ANSI-Select Entry *)
TYPE
  EntryNodePtr * = POINTER TO EntryNode;
  EntryNode * = STRUCT (n : e.MinNode)
    data    * : ARRAY 80 OF CHAR;  (* String to display *)
    bold    * : BOOLEAN;
    inverse * : BOOLEAN;
  END;

(* Colour Definitions for CLONL_Color Command *)

CONST
  Black  * = 0;
  Red    * = 1;
  Green  * = 2;
  Yellow * = 3;
  Blue   * = 4;
  Pink   * = 5;
  Cyan   * = 6;
  White  * = 7;

(* Style Definitions for CLONL_Color Command *)

  normal     * = 0;
  bold       * = 1;
  kursiv     * = 3;
  underlined * = 4;
  inverse    * = 7;

(* Definitions of types for CLONL_BlueRead() *)

  ASCII  * = 0;
  NUMBER * = 1;
  BOOL   * = 2;
  SECRET * = 3;

(* Definitions for CLONL_GetKey () *)

  CTRLA * = 1;
  CTRLB * = 2;
  CTRLC * = 3;
  CTRLD * = 4;
  CTRLE * = 5;
  CTRLF * = 6;
  BS    * = 8;
  TAB   * = 9;
  LF    * = 10;
  CR    * = 13;
  CTRLK * = 11;
  CTRLN * = 14;
  CTRLR * = 18;
  CTRLX * = 24;
  CTRLZ * = 26;
  ESC   * = 27;
  DEL   * = 127;
  CSI   * = 155;

  BREAK * = 1024;
  UP    * = 1025;
  DOWN  * = 1026;
  RIGHT * = 1027;
  LEFT  * = 1028;
  EOF   * = 1029;

(* Definitons for Mode for ANSI-Requester *)

  SELECT * = 0; (* To select multiple entrys by marking *)
  CHOOSE * = 1; (* To choose ONE entry by cursor left *)
  INHALT * = 2; (* Special - internal *)

(*
   Definition of range for CLONL_FreeRange() and CLONL_ParseExpression()
*)

TYPE
  RangePtr * = POINTER TO Range;
  Range * = STRUCT
    from * : LONGINT;
    to   * : LONGINT;
    next * : RangePtr;
  END;

(*
   Definition of Shortmenu for CLONL_AddShortMenu, CLONL_ShortMenuSelect(), CLONL_FreeShortMenu
*)

  ShortmenuPtr * = POINTER TO Shortmenu;
  Shortmenu * = STRUCT
    entry  * : ARRAY 40 OF CHAR;
    hotkey * : CHAR;
    next   * : ShortmenuPtr;
  END;

(*
   Definition for one Mask Entry for CLONL_AddGadget, CLONL_GetGagdet, CLONL_InputMask()
*)

  MaskNodePtr * = POINTER TO MaskNode;
  MaskNode * = STRUCT (n : e.MinNode)
    MaskType   * : LONGINT;
    identifier * : ARRAY 40 OF CHAR;
    buffer     * : e.APTR;
    buffersize * : LONGINT;
  END;
    

(* typedef struct MaskNode MaskNode; *)

(* Mask-List Definition *)

TYPE
  Mask * = e.MinList;


PROCEDURE Open * {clonline,-30} (catalogname {8} : ARRAY OF CHAR):BOOLEAN;
PROCEDURE Close * {clonline,-36} ();
PROCEDURE FlushInput * {clonline,-42} ();
PROCEDURE FlushOutput * {clonline,-48} ();
PROCEDURE UpdateUser * {clonline,-54} ();
PROCEDURE Normal * {clonline,-60} ();
PROCEDURE DeleteEOL * {clonline,-66} ();
PROCEDURE DeleteLine * {clonline,-72} ();
PROCEDURE InsertLine * {clonline,-78} ();
PROCEDURE BlueMode * {clonline,-84} ();
PROCEDURE GotoXY * {clonline,-90} (x {0}, y {1}: LONGINT);
PROCEDURE Color * {clonline,-96} (col {0}   : LONGINT;
                                  bcol {1}  : LONGINT;
                                  style {2} : LONGINT);
PROCEDURE ClearScreen * {clonline,-102} ();
PROCEDURE BlueRead * {clonline,-108} (VAR str  {8} : ARRAY OF CHAR;
                                      def  {9} : ARRAY OF CHAR;
                                      len  {0} : LONGINT;
                                      type {1} : LONGINT;
                                      mask {2} : BOOLEAN); (* :e.STRPTR; *)
PROCEDURE Wait * {clonline,-114} ():BOOLEAN;
PROCEDURE DrawTitle * {clonline,-120} ();
PROCEDURE Edit * {clonline,-126} (filename {8}: ARRAY OF CHAR):BOOLEAN;
PROCEDURE RequesterSelect * {clonline,-168} (entry {8} : e.MinListPtr;
                                             pos   {0} : LONGINT;
                                             title {9} : ARRAY OF CHAR;
                                             mode  {1} : LONGINT):EntryNodePtr;
PROCEDURE GetLocale * {clonline,-174} ():l.LocalePtr;
PROCEDURE GetCatalog * {clonline,-180} ():e.APTR;
PROCEDURE GetUserShell * {clonline,-186} ():e.APTR; (* eigentlich clusershellptr *)
PROCEDURE IsTTY * {clonline,-192} ():BOOLEAN;
PROCEDURE GetString * {clonline,-198} (entry {8}: ARRAY OF CHAR):e.STRPTR;
PROCEDURE VPrintf * {clonline,-204} (format {8} : ARRAY OF CHAR;
                                     arg    {9} : ARRAY OF u.TagItem);
PROCEDURE Printf * {clonline,-204} (format {8}   : ARRAY OF CHAR;
                                    arg    {9}.. : e.APTR);
PROCEDURE VSPrintf * {clonline,-210} (VAR to {8} : ARRAY OF CHAR;
                                      format {9} : ARRAY OF CHAR;
                                      arg    {10}: ARRAY OF u.TagItem);
PROCEDURE SPrintf * {clonline,-210} (VAR to {8}   : ARRAY OF CHAR;
                                     format {9}   : ARRAY OF CHAR;
                                     arg    {10}..: e.APTR);
PROCEDURE DoCommand * {clonline,-216} (cmd   {8} : ARRAY OF CHAR;
                                       param {9} : ARRAY OF CHAR):LONGINT;
PROCEDURE SendMail * {clonline,-222} (abs      {8} : ARRAY OF CHAR;
                                      emp      {9} : ARRAY OF CHAR;
                                      bet      {10}: ARRAY OF CHAR;
                                      filename {11}: ARRAY OF CHAR;
                                      bezid    {12}: ARRAY OF CHAR):LONGINT;
PROCEDURE GetKey * {clonline,-228} ():LONGINT;
PROCEDURE FreeRange * {clonline,-240} (range {8} : RangePtr);
PROCEDURE ParseExpression * {clonline,-246} (parsepattern {8} : ARRAY OF CHAR;
                                             maximum      {0} : LONGINT):RangePtr;
PROCEDURE Transfer * {clonline,-252} (filename     {8} : ARRAY OF CHAR;
                                      sendfilename {9} : ARRAY OF CHAR):LONGINT;
PROCEDURE GetUserFullName * {clonline,-258} (email {8} : ARRAY OF CHAR):e.STRPTR;
PROCEDURE GetNameVal * {clonline,-264} (name {8} : ARRAY OF CHAR):LONGINT;
PROCEDURE IsKeyPressed * {clonline,-270} ():BOOLEAN;
PROCEDURE Continue * {clonline,-276} ():BOOLEAN;
PROCEDURE AddShortMenu * {clonline,-282} (oldmenu {8} : ShortmenuPtr;
                                          entry   {9} : ARRAY OF CHAR;
                                          hotkey  {0} : CHAR):ShortmenuPtr;
PROCEDURE FreeShortMenu * {clonline,-288} (menu {8} : ShortmenuPtr);
PROCEDURE ShortMenuSelect * {clonline,-294} (menu {8} : ShortmenuPtr):CHAR;
PROCEDURE TimeToStr * {clonline,-300} (time {0} : t.TimeVal;
                                       str  {8} : ARRAY OF CHAR):e.STRPTR;
PROCEDURE StrToTime * {clonline,-306} (str  {8} : ARRAY OF CHAR;
                                       time {9} : t.TimeVal):t.TimeVal;
PROCEDURE AddGadget * {clonline,-312} (mask       {8} : e.APTR; (* POINTER TO Mask !! *)
                                       masktype   {0} : LONGINT;
                                       identifier {9} : ARRAY OF CHAR;
                                       buffer     {10}: ARRAY OF CHAR;
                                       buffersize {1} : LONGINT);
PROCEDURE GetGadget * {clonline,-318} (mask       {8} : e.APTR; (* POINTER TO Mask !! *)
                                       identifier {9} : ARRAY OF CHAR):MaskNodePtr;
PROCEDURE InputMask * {clonline,-324} (mask  {8} : e.APTR; (* POINTER TO Mask !! *)
                                       title {9} : ARRAY OF CHAR):BOOLEAN; (* 144 9802 *)
PROCEDURE ShortMenuSelectVertical * {clonline,-330} (menu {8} : ShortmenuPtr):CHAR;


(* Defines for easyer textoutput *)
PROCEDURE MSG * (txt : ARRAY OF CHAR):e.STRPTR; BEGIN; RETURN GetString(txt); END MSG;
PROCEDURE PutMsg * (txt : ARRAY OF CHAR); BEGIN; Printf(MSG(txt)^, NIL); END PutMsg;


BEGIN

  clonline := e.OpenLibrary (clonlinename, clonlineversion);
  IF clonline = NIL THEN
    sys.SETREG (0, i.DisplayAlert(0, "\x00\x64\x14Achtung : clonline.library v2.3 fehlt\o\o",50));
    HALT(0)
  END;

CLOSE

  IF clonline # NIL THEN
    e.CloseLibrary (clonline);
  END;

END CLOnline.
