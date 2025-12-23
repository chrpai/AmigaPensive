#ifndef PROTO_DDDOOR_H
#define PROTO_DDDOOR_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif /* EXEC_TYPES_H */

#include <clib/dddoor_protos.h>
#if defined(__OPTIMIZE__) && !defined(__NOINLINES__)
#include <inline/dddoor.h>
#endif
#ifndef __NOLIBBBASE__
extern struct Library *DDBase;
#endif

#endif /* PROTO_DDDOOR_H */
