#ifndef BYTEORDER_H__
#define BYTEORDER_H__

#include "stdio.h"

/************************************************************************/
/* 字节序判断，实现大端小端排序                                                                     */
/************************************************************************/

#define	OP_LITTLEENDIAN	'<'		/* little endian */
#define	OP_BIGENDIAN	'>'		/* big endian */
#define	OP_NATIVE	'='		/* native endian */

int doendian(int c);

void doswap(int swap, void *p, size_t n);

const char* ByteString(const char *msg, const unsigned int len);

#endif//BYTEORDER_H__
