/* BBSPrefs */

extern struct List BBS_List;

struct Node BBS_Nodes[]={
	{ &BBS_Nodes[1],(struct Node *) &BBS_List.lh_Head,0,0,"Access Groups" },
	{ &BBS_Nodes[2],&BBS_Nodes[0],0,0,"Archivers" },
	{ &BBS_Nodes[3],&BBS_Nodes[1],0,0,"Editors" },
	{ &BBS_Nodes[4],&BBS_Nodes[2],0,0,"File Bases" },
	{ &BBS_Nodes[5],&BBS_Nodes[3],0,0,"Menus" },
	{ &BBS_Nodes[6],&BBS_Nodes[4],0,0,"Message Bases" },
	{ &BBS_Nodes[7],&BBS_Nodes[5],0,0,"News" },
	{ &BBS_Nodes[8],&BBS_Nodes[6],0,0,"Nodes" },
	{ &BBS_Nodes[9],&BBS_Nodes[7],0,0,"Protocols" },
	{ (struct Node *) &BBS_List.lh_Tail,&BBS_Nodes[8],0,0,"System" }
};

struct List BBS_List={
	&BBS_Nodes[0],
	NULL,
	&BBS_Nodes[8],
	0,0
};

ProjectDefinition(bbsprefs_trwintags)
	{
		WindowID(1),WindowPosition(TRWP_CENTERDISPLAY),
		WindowTitle("BBS Preferences"),

		BeginMenu("Project"),
			MenuItem("?_About...",101),
			ItemBarlabel,
			MenuItem("Q_Quit",102),

		VertGroupA,
			Space,
			HorizGroupA,
				Space,
				ListSel(&BBS_List,1,0),TRAT_MinWidth,32,TRAT_MinHeight,10,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

/* Prefs */

struct BufferNode {
	struct Node bn_Node;
	BYTE bn_Buffer[41];
};

BYTE path_buff[33];

#define listprefs_trwintags NamedFrameBox("List"),HorizGroupA,Space,VertGroupA,Space,ListSS(NULL,51,0,~0),StringGadget(NULL,52),Space,HorizGroupEA,Button("_Add",55),Button("D_elete",56),EndGroup,HorizGroupEA,Button("Move _Up",57),Button("Move _Down",58),EndGroup,Space,EndGroup,Space,EndGroup
#define treeprefs_trwintags NamedFrameBox("Tree"),HorizGroupA,Space,VertGroupA,Space,TextBox(NULL,59,32),ListSS(NULL,51,0,~0),StringGadget(NULL,52),Space,HorizGroupEA,Button("Parent",53),Button("Child",54),EndGroup,HorizGroupEA,Button("_Add",55),Button("D_elete",56),EndGroup,HorizGroupEA,Button("Move _Up",57),Button("Move _Down",58),EndGroup,Space,EndGroup,Space,EndGroup

#define BeginListPrefs(title,id) WindowID(id),WindowPosition(TRWP_CENTERDISPLAY),WindowTitle(title),BeginMenu("Project"),MenuItem("Q_Quit",101),HorizGroupEA,Space,VertGroupA,Space,listprefs_trwintags,Space,EndGroup,Space,VertGroupA,Space,VertGroupSA,NamedFrameBox("Fields"),HorizGroupA,Space,VertGroupA
#define BeginTreePrefs(title,id) WindowID(id),WindowPosition(TRWP_CENTERDISPLAY),WindowTitle(title),BeginMenu("Project"),MenuItem("Q_Quit",101),HorizGroupEA,Space,VertGroupA,Space,treeprefs_trwintags,Space,EndGroup,Space,VertGroupA,Space,VertGroupSA,NamedFrameBox("Fields"),HorizGroupA,Space,VertGroupA
#define EndPrefs EndGroup,Space,EndGroup,Space,EndGroup,Space,EndGroup,Space,EndGroup
#define PrefsStringGadget(text,id,maxchars) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,StringGadget(NULL,id),TRAT_Value,maxchars,EndGroup,EndGroup
#define PrefsGetFileStringGadget(text,id,maxchars) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,StringGadget(NULL,id),TRAT_Value,maxchars,GetFileButton(id+1),EndGroup,EndGroup
#define PrefsGetDrawerStringGadget(text,id,maxchars) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,StringGadget(NULL,id),TRAT_Value,maxchars,GetDrawerButton(id+1),EndGroup,EndGroup
#define PrefsCycleGadget(text,labels,id) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,CycleGadget(labels,0,id),EndGroup,EndGroup
#define PrefsCheckboxGadget(text,id) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,CheckBox(id),EndGroup,EndGroup
#define PrefsTextGadget(text,id,minwidth) HorizGroupEAC,HorizGroupSAC,Space,TextN(text),EndGroup,Space,TextBox(NULL,id,minwidth),EndGroup
#define PrefsGetEntryTextGadget(text,id,minwidth) HorizGroupEAC,HorizGroupSAC,Space,TextN(text),EndGroup,Space,HorizGroupAC,_TextBox,HorizGroupAC,TextBox(NULL,id,minwidth),EndGroup,GetEntryButton(id+1),EndGroup,EndGroup

/* AccessGroupPrefs */

BYTE ag_days_buff[11];
BYTE ag_sessiontimelimit_buff[6],ag_inactivitytimelimit_buff[6];
BYTE ag_accesslevel_buff[4],ag_ratiobyte_buff[4],ag_ratiofile_buff[4];

ProjectDefinition(accessgroupprefs_trwintags)
	{
		BeginListPrefs("Access Group Preferences",1),
			Space,
			PrefsStringGadget("Access _Level",1,3),
			Space,
			PrefsStringGadget("_Session Time Limit",2,5),
			Space,
			PrefsStringGadget("_Inactivity Time Limit",3,5),
			Space,
			PrefsStringGadget("Ratio _Byte",4,3),
			Space,
			PrefsStringGadget("Ratio _File",5,3),
			Space,
			PrefsStringGadget("Da_ys",6,10),
			Space,
		EndPrefs,

		EndProject
	};

/* ArchiverPrefs */

BYTE arc_extension[4],arc_addcommand[256],arc_extractcommand[256],arc_viewcommand[256];

ProjectDefinition(archiverprefs_trwintags)
	{
		BeginListPrefs("Archiver Preferences",2),
			Space,
			PrefsStringGadget("Ex_tension",1,3),
			Space,
			PrefsStringGadget("Add _Command",2,255),
			Space,
			PrefsStringGadget("E_xtract Command",3,255),
			Space,
			PrefsStringGadget("_View Command",4,255),
			Space,
		EndPrefs,

		EndProject
	};

/* EditorPrefs */

BYTE *type_edlabels[]={ "Standard","Visual","Graphical",NULL };

BYTE ed_command[256];
BYTE ed_stack_buff[11];

ProjectDefinition(editorprefs_trwintags)
	{
		BeginListPrefs("Editor Preferences",4),
			Space,
			PrefsCycleGadget("_Type",type_edlabels,1),
			Space,
			PrefsGetFileStringGadget("_Command",2,255),
			Space,
			PrefsStringGadget("_Stack",4,10),
			Space,
		EndPrefs,

		EndProject
	};

/* FileBasePrefs */

BYTE default_directory[256];

BYTE fb_accessrange[21],fb_group[41],fb_directory[256];
BYTE fb_diskspacerequired[11],fb_days[11];

ProjectDefinition(filebaseprefs_trwintags)
	{
		BeginTreePrefs("File Base Preferences",5),
			Space,
			PrefsStringGadget("Access _Range",1,20),
			Space,
			PrefsStringGadget("_Group",2,40),
			Space,
			PrefsGetDrawerStringGadget("Direc_tory",3,255),
			Space,
			PrefsStringGadget("Disk _Space Req (K)",5,10),
			Space,
			PrefsStringGadget("Da_ys",6,10),
			Space,
			PrefsCheckboxGadget("Read _Only",7),
			Space,
			PrefsCheckboxGadget("Auto _Join",8),
			Space,
		EndPrefs,

		EndProject
	};

/* Menu Prefs */

BYTE mn_accessrange[21],mn_character[2],mn_defaultchar[2],mn_number[2],mn_prompt[64],mn_command[64];

ProjectDefinition(menuprefs_trwintags)
	{
		BeginTreePrefs("Menu Preferences",6),
			Space,
			PrefsStringGadget("Access _Range",1,20),
			Space,
			PrefsStringGadget("_Character",2,1),
			Space,
			PrefsStringGadget("De_fault",3,1),
			Space,
			PrefsStringGadget("_Number",4,1),
			Space,
			PrefsStringGadget("_Prompt",5,63),
			Space,
			PrefsStringGadget("Co_mmand",6,63),
			Space,
		EndPrefs,

		EndProject
	};

/* MessageBasePrefs */

BYTE default_quote_header[81];

BYTE mb_accessrange[21],mb_group[21],mb_quoteheader[81],mb_days[11];

ProjectDefinition(messagebaseprefs_trwintags)
	{
		BeginTreePrefs("Message Base Preferences",7),
			Space,
			PrefsStringGadget("Access _Range",1,20),
			Space,
			PrefsStringGadget("_Group",2,20),
			Space,
			PrefsStringGadget("_Quote Header",3,80),
			Space,
			PrefsStringGadget("_Days",6,10),
			Space,
			PrefsCheckboxGadget("Read _Only",4),
			Space,
			PrefsCheckboxGadget("Auto _Join",5),
			Space,
		EndPrefs,

		EndProject
	};

/* NewsPrefs */

BYTE date_buff[64];

BYTE nws_accessrange[21],nws_file[256];

ProjectDefinition(newsprefs_trwintags)
	{
		BeginListPrefs("News Preferences",8),
			Space,
			PrefsStringGadget("Access _Range",1,20),
			Space,
			PrefsGetFileStringGadget("_File",2,255),
			Space,
			PrefsTextGadget("Date",4,17),
			Space,
		EndPrefs,

		EndProject
	};

/* NodePrefs */

BYTE default_node_command[256];

BYTE *type_ndlabels[]={ "Local","Remote",NULL };

BYTE nd_accessrange[21];
BYTE nd_device[33];
BYTE nd_unit[11],nd_buffersize[11],nd_baud[11];
BYTE nd_command[256];

ProjectDefinition(nodeprefs_trwintags)
	{
		BeginListPrefs("Node Preferences",9),
			Space,
			PrefsStringGadget("Access _Range",1,20),
			Space,
			PrefsCycleGadget("_Type",type_ndlabels,2),
			Space,
			PrefsGetFileStringGadget("De_vice",3,32),
			Space,
			PrefsStringGadget("U_nit",5,10),
			Space,
			PrefsStringGadget("Buffer _Size",6,10),
			Space,
			PrefsStringGadget("_Baud",7,10),
			Space,
			PrefsCheckboxGadget("_Handshaking",8),
			Space,
			PrefsCheckboxGadget("Aut_o Baud",9),
			Space,
			PrefsCheckboxGadget("_Front End",10),
			Space,
			PrefsStringGadget("_Command",11,255),
			Space,
		EndPrefs,

		EndProject
	};

/* ProtocolPrefs */

BYTE pr_sendcommand[256],pr_receivecommand[256];

ProjectDefinition(protocolprefs_trwintags)
	{
		BeginListPrefs("Protocol Preferences",10),
			Space,
			PrefsStringGadget("_Send Command",1,255),
			Space,
			PrefsStringGadget("_Receive Command",2,255),
			Space,
			PrefsCheckboxGadget("_Batch",3),
			Space,
			PrefsCheckboxGadget("Bidirec_tional",4),
			Space,
		EndPrefs,

		EndProject
	};

/* SystemPrefs */

BYTE sys_inputcolor[81],sys_headercolor[81],sys_outputcolor[81],sys_promptcolor[81],sys_systemcolor[81],sys_fileiddizextractor[81];

ProjectDefinition(systemprefs_trwintags)
	{
		WindowID(11),
			WindowPosition(TRWP_CENTERDISPLAY),
			WindowTitle("System Preferences"),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				PrefsStringGadget("_Header Color",5,80),
				Space,
				PrefsStringGadget("_Input Color",6,80),
				Space,
				PrefsStringGadget("_Output Color",7,80),
				Space,
				PrefsStringGadget("_Prompt Color",8,80),
				Space,
				PrefsStringGadget("_System Color",9,80),
				Space,
				PrefsStringGadget("_File ID Diz Extractor",12,80),
				Space,
				PrefsCheckboxGadget("Only _Real Names",11),
				Space,
			EndGroup,
			Space,
		EndGroup,
	};

/* Additional Prefs */

#define PREFS_ListMode (1<<0)
#define PREFS_TreeMode (1<<1)

ULONG prefs_flags[]={
	PREFS_ListMode, /* Access Groups */
	PREFS_ListMode, /* Archivers */
	PREFS_ListMode, /* Editors */
	PREFS_ListMode|PREFS_TreeMode, /* File Bases */
	PREFS_ListMode|PREFS_TreeMode, /* Menus */
	PREFS_ListMode|PREFS_TreeMode, /* Message Bases */
	PREFS_ListMode, /* News */
	PREFS_ListMode, /* Nodes */
	PREFS_ListMode, /* Protocols */
	NULL /* System */
};

APTR prefs_trwintags[]={
	accessgroupprefs_trwintags,
	archiverprefs_trwintags,
	editorprefs_trwintags,
	filebaseprefs_trwintags,
	menuprefs_trwintags,
	messagebaseprefs_trwintags,
	newsprefs_trwintags,
	nodeprefs_trwintags,
	protocolprefs_trwintags,
	systemprefs_trwintags
};

BYTE *prefs_path[]={
	"AccessGroups",
	"Archivers",
	"Editors",
	"FileBases",
	"Menus",
	"MessageBases",
	"News",
	"Nodes",
	"Protocols",
	""
};

ULONG prefs_type[]={
	CFGTYP_AccessGroup,
	CFGTYP_Archiver,
	CFGTYP_Editor,
	CFGTYP_FileBase,
	CFGTYP_Menu,
	CFGTYP_MessageBase,
	CFGTYP_News,
	CFGTYP_Node,
	CFGTYP_Protocol,
	CFGTYP_System
};
