;  Editor Line Structure
 STRUCTURE ED_Node,0
	STRUCT	exedn_node,MLN_SIZE		; Minnode structure
	APTR	exedn_line			; the line entered
	BYTE	exedn_Flags			; Only eight flags allowed
	BYTE	exedn_KludgeFill00
	LABEL exedn_SIZE
