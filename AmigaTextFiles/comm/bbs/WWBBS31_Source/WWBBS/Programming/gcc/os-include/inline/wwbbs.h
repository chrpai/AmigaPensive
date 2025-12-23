#ifndef _INLINE_WWBBS_H
#define _INLINE_WWBBS_H
#endif

#include <sys/cdefs.h>
#include <inline/stubs.h>

#ifndef BASE_EXT_DECL
#define BASE_EXT_DECL
#endif
#ifndef BASE_PAR_DECL
#define BASE_PAR_DECL
#endif
#ifndef BASE_NAME
#define BASE_NAME WorldWideBase
#endif

extern __inline BOOL 
IsRange (BASE_PAR_DECL BYTE * range, ULONG num)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = range;
  register ULONG d0 __asm("d0") = num;
  __asm __volatile ("jsr a6@(-258)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0)
  : "a0", "d0",  "memory");
  return _res;
}
extern __inline void
LogEntry (BASE_PAR_DECL BYTE * id, BYTE * log, BYTE * text)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = id;
  register BYTE * a1 __asm("a1") = log;
  register BYTE * a2 __asm("a2") = text;
  __asm __volatile ("jsr a6@(-252)"
  : /* no output */
  : "r" (a6), "r" (a0), "r" (a1), "r" (a2)
  : "a0", "a1", "a2",  "memory");
}
extern __inline void
SendUpdate (BASE_PAR_DECL UBYTE cmd, ULONG id)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register UBYTE d0 __asm("d0") = cmd;
  register ULONG a0 __asm("a0") = id;
  __asm __volatile ("jsr a6@(-246)"
  : /* no output */
  : "r" (a6), "r" (d0), "r" (a0)
  : "a0", "d0",  "memory");
}
extern __inline void
SetUpdatePort (BASE_PAR_DECL struct MsgPort * port)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct MsgPort * a0 __asm("a0") = port;
  __asm __volatile ("jsr a6@(-240)"
  : /* no output */
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
}
extern __inline ULONG
TimerCommand (BASE_PAR_DECL BYTE * id, UBYTE cmd, struct TagItem * tags)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = id;
  register UBYTE d0 __asm("d0") = cmd;
  register struct TagItem * a1 __asm("a1") = tags;
  __asm __volatile ("jsr a6@(-234)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0), "r" (a1)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline ULONG
SerialCommand (BASE_PAR_DECL BYTE * id, UBYTE cmd, struct TagItem * tags)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = id;
  register UBYTE d0 __asm("d0") = cmd;
  register struct TagItem * a1 __asm("a1") = tags;
  __asm __volatile ("jsr a6@(-228)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0), "r" (a1)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline ULONG
ConsoleCommand (BASE_PAR_DECL BYTE * id, UBYTE cmd, struct TagItem * tags)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = id;
  register UBYTE d0 __asm("d0") = cmd;
  register struct TagItem * a1 __asm("a1") = tags;
  __asm __volatile ("jsr a6@(-222)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0), "r" (a1)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline ULONG
NodeCommand (BASE_PAR_DECL BYTE * id, UBYTE cmd, struct TagItem * tags)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = id;
  register UBYTE d0 __asm("d0") = cmd;
  register struct TagItem * a1 __asm("a1") = tags;
  __asm __volatile ("jsr a6@(-216)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0), "r" (a1)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline ULONG
GetPreviousFile (BASE_PAR_DECL APTR gnode, ULONG id)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register ULONG d0 __asm("d0") = id;
  __asm __volatile ("jsr a6@(-210)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0)
  : "a0", "d0",  "memory");
  return _res;
}
extern __inline ULONG
GetNextFile (BASE_PAR_DECL APTR gnode, ULONG id)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register ULONG d0 __asm("d0") = id;
  __asm __volatile ("jsr a6@(-204)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0)
  : "a0", "d0",  "memory");
  return _res;
}
extern __inline ULONG
RemFile (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-198)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
AddFile (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-192)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
SetFile (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-186)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
GetFile (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-180)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline BOOL
GetFileGroupDirectory (BASE_PAR_DECL APTR gnode, BYTE * buffer, LONG len)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register BYTE * a1 __asm("a1") = buffer;
  register LONG d0 __asm("d0") = len;
  __asm __volatile ("jsr a6@(-174)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1), "r" (d0)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline void
CloseFileGroup (BASE_PAR_DECL APTR gnode)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  __asm __volatile ("jsr a6@(-168)"
  : /* no output */
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
}
extern __inline APTR
OpenFileGroup (BASE_PAR_DECL BYTE * path, BYTE * name, LONG type)
{
  BASE_EXT_DECL
  register APTR _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = path;
  register BYTE * a1 __asm("a1") = name;
  register LONG d0 __asm("d0") = type;
  __asm __volatile ("jsr a6@(-162)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1), "r" (d0)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline ULONG
GetPreviousMessage (BASE_PAR_DECL APTR gnode, ULONG id)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register ULONG d0 __asm("d0") = id;
  __asm __volatile ("jsr a6@(-156)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0)
  : "a0", "d0",  "memory");
  return _res;
}
extern __inline ULONG
GetNextMessage (BASE_PAR_DECL APTR gnode, ULONG id)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register ULONG d0 __asm("d0") = id;
  __asm __volatile ("jsr a6@(-150)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (d0)
  : "a0", "d0",  "memory");
  return _res;
}
extern __inline BOOL
RemMessage (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-144)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
AddMessage (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-138)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
SetMessage (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-132)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline ULONG
GetMessage (BASE_PAR_DECL APTR gnode, struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  register struct TagItem * a1 __asm("a1") = tags_orig;
  __asm __volatile ("jsr a6@(-126)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1)
  : "a0", "a1",  "memory");
  return _res;
}
extern __inline void
CloseMessageGroup (BASE_PAR_DECL APTR gnode)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register APTR a0 __asm("a0") = gnode;
  __asm __volatile ("jsr a6@(-120)"
  : /* no output */
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
}
extern __inline APTR
OpenMessageGroup (BASE_PAR_DECL BYTE * path, BYTE * name, LONG type)
{
  BASE_EXT_DECL
  register APTR _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register BYTE * a0 __asm("a0") = path;
  register BYTE * a1 __asm("a1") = name;
  register LONG d0 __asm("d0") = type;
  __asm __volatile ("jsr a6@(-114)"
  : "=r" (_res)
  : "r" (a6), "r" (a0), "r" (a1), "r" (d0)
  : "a0", "a1", "d0",  "memory");
  return _res;
}
extern __inline VOID
UnLoadUser (BASE_PAR_DECL struct TagItem * tag_orig)
{
  BASE_EXT_DECL
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tag_orig;
  __asm __volatile ("jsr a6@(-108)"
  : /* no output */
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
}
extern __inline BOOL
LoadUser (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-102)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
RemUser (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-96)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
AddUser (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-90)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
SetUser (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-84)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
GetUser (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-78)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
RemStatus (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-72)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
AddStatus (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-66)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
SetStatus (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-60)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
GetStatus (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-54)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
RemConfig (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-48)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline BOOL
AddConfig (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register BOOL _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-42)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
SetConfig (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-36)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
extern __inline ULONG
GetConfig (BASE_PAR_DECL struct TagItem * tags_orig)
{
  BASE_EXT_DECL
  register ULONG _res  __asm("d0");
  register struct Library * a6 __asm("a6") = BASE_NAME;
  register struct TagItem * a0 __asm("a0") = tags_orig;
  __asm __volatile ("jsr a6@(-30)"
  : "=r" (_res)
  : "r" (a6), "r" (a0)
  : "a0",  "memory");
  return _res;
}
