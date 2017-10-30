#include "byte_order.h"

int doendian(int c)
{
	int x = 1;
	int e = *(char*)&x;
	if (c == OP_LITTLEENDIAN) return !e;//当前使用小端格式，本机字节码是大端的
	if (c == OP_BIGENDIAN) return e;//当前使用大端格式，本机字节码是小端的
	if (c == OP_NATIVE) return 0;
	return 0;
}

void doswap(int swap, void *p, size_t n)
{
	if (swap)
	{
		char *a = (char*)p;
		int i, j;
		for (i = 0, j = n - 1, n = n / 2; n--; i++, j--)
		{
			char t = a[i]; a[i] = a[j]; a[j] = t;
		}
	}
}

const char* ByteString(const char *msg, const unsigned int len)
{
	if (len > 30000)
		return "";

	static char buf[65535] = { 0 };
	buf[0] = 0;
	for (unsigned int i = 0; i < len; i++){
		sprintf(buf, "%s%02x-", buf, msg[i] & 0xff);
	}
	return buf;
}
