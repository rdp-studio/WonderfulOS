#include "apilib.h"

void HariMain(void)
{
	char *buf;
	int win;

	api_initmalloc();
	buf = api_malloc(150 * 50 * 4);
	win = api_openwin(buf, 185, 50, -1, "Ver");
	api_putstrwin(win, 28, 28, 0 /* 崟 */, 12, "W OS Beta");
	for (;;) {
		if (api_getkey(1) == 0x0a) {
			break; /* Enter側傜break; */
		}
	}
	api_end();
}
