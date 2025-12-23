; Item struct

 STRUCTURE NewPost_Data,0			; New Sub-board layout
	STRUCT	expd_Subject,81			; The Message Subject
	BYTE	expd_KludgeFill00

	LONG	expd_Date_Sent			; Date Message Was Sent
	LONG	expd_LR_Date			; Last Response Date
	
	LONG	expd_Post_ID			; ID number of the post
	LONG	expd_Responses			; # of responses this article has
	LONG	expd_Times_Used			; Time message has been read/downloaded
	LONG	expd_Bytes			; Filesize if a file
	LONG	expd_Ext_Flags			; Flags, use POST_xxx

	LONG	expd_DescPos			; The position of the short description
	LONG	expd_From_ID			; The Senders User ID
	LONG	expd_From_Serial_Num		; Senders Serial Number

	LONG	expd_Credits_Received		; The credits given to uploader
	LONG	expd_expansion

	LONG	expd_FirstResponse		; The first response on disk
	LONG	expd_LastResponse		; The last response on disk
	LONG	expd_LastAccessed		; Last Date file was downloaded 
	LONG	expd_DownloadBonus		; The % of filesize returned to uploader

	LONG	expd_expansion1

	LONG	expd_BestCPS			; The best dl CPS rate for this file

	DOUBLE	expd_ItemCharge			; The charge for the item
	STRUCT	expd_exp,4*2
	LABEL expd_SIZE
