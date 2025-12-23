struct pkt_hdr {
   unsigned short p_orig_node;
   unsigned short p_dest_node;
   unsigned short p_year;
   unsigned short p_month;
   unsigned short p_day;
   unsigned short p_hour;
   unsigned short p_min;
   unsigned short p_sec;
   unsigned short p_baud;
   unsigned short p_type;
   unsigned short p_orig_net;
   unsigned short p_dest_net;
   unsigned char  p_pcode;
   unsigned char  p_psernum;
   unsigned char  p_password[8];
   unsigned short p_orig_zone;
   unsigned short p_dest_zone;
   unsigned char  p_fill[20];
};

struct pak_msg_hdr {
   unsigned short m_type;
   unsigned short m_orig_node;
   unsigned short m_dest_node;
   unsigned short m_orig_net;
   unsigned short m_dest_net;
   unsigned short m_attribute;
   unsigned short m_cost;
/*
   unsigned char  m_date[20];
   unsigned char  m_to[36];
   unsigned char  m_from[36];
   unsigned char  m_subject[72];
*/
};

#define Private             1
#define Crash               2
#define Received            4
#define Sent                8
#define FileAttached        16
#define InTransit           32
#define Orphan              64
#define KillSent            128
#define Local               256
#define HoldForPickup       512
#define FFileRequest        2048
#define ReturnRectRequest   4096
#define IsReturnRect        8192
#define AuditRequest        16384
#define FileUpdateReq       32768
