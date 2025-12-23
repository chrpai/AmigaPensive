
/**************************************************************************
 * anything below this point is not implemented yet!
 **************************************************************************/


/**************************************************************************
 * ChannelList locking is to be done through semaphore ownership.
 * Please use LockChannelList() to lock it before accessing.
 * BE SURE to use UnlockChannelList() to unlock it after use.
 * See "LockChannelList()" for more info.
 * Other important commands: RemoveChannel(), AddChannel(), FindChannel(),
 * RenameChannel(), NewTopicChannel(), JoinChannel(), MakeChannelList()
 **************************************************************************/
struct ChannelList
	{
	struct SignalSemaphore cl_Sem;		/* for locking shared channel list */
	struct List cl_List;						/* list header for Channel List */
	};


struct ChannelNode
	{
	struct Node cn_Node;
	char cn_Name[25];							/* channel name */
	char cn_Topic[80];						/* Current topic */
	LONG cn_ChannelID;						/* unique channel ID - users joined in this channel/room will have this unique ID set in their PortVars->JoinedChannels list */
	LONG cn_UseCount;							/* number of users currently in channel */
	struct ChannelList *cn_HeadChannel;	/* pointer to the top of the channel list */
	struct ChannelNode *cn_NextChannel;	/* pointer to next channel in list */
	};
