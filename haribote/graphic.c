/* グラフィック処理関係 */

#include "bootpack.h"
#include <stdio.h> 		//1. sprintf包含在内

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黒 */
		0xff, 0x00, 0x00,	/*  1:明るい赤 */
		0x00, 0xff, 0x00,	/*  2:明るい緑 */
		0xff, 0xff, 0x00,	/*  3:明るい黄色 */
		0x00, 0x00, 0xff,	/*  4:明るい青 */
		0xff, 0x00, 0xff,	/*  5:明るい紫 */
		0x00, 0xff, 0xff,	/*  6:明るい水色 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:明るい灰色 */
		0x84, 0x00, 0x00,	/*  9:暗い赤 */
		0x00, 0x84, 0x00,	/* 10:暗い緑 */
		0x84, 0x84, 0x00,	/* 11:暗い黄色 */
		0x00, 0x00, 0x84,	/* 12:暗い青 */
		0x84, 0x00, 0x84,	/* 13:暗い紫 */
		0x00, 0x84, 0x84,	/* 14:暗い水色 */
		0x84, 0x84, 0x84	/* 15:暗い灰色 */
	};
	unsigned char table2[216 * 3];
	int r, g, b;
	set_palette(0, 15, table_rgb);
	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
				table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
				table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
			}
		}
	}
	set_palette(16, 231, table2);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 割り込み許可フラグの値を記録する */
	io_cli(); 					/* 許可フラグを0にして割り込み禁止にする */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 割り込み許可フラグを元に戻す */
	return;
}

void boxfill8(unsigned int *vram, int xsize, unsigned int c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

int read_wallpaper_32 (unsigned char *filename, int x, int y, int *fat, unsigned int *vram)
{
	int i, j, x0, y0, fsize, info[4];
	unsigned char *filebuf, r, g, b;
	struct RGB *picbuf;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo;
	struct DLL_STRPICENV *env;
	finfo = file_search("back.jpg", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0) {
		return -1;
	}
	fsize = finfo->size;
	filebuf = (unsigned char *) memman_alloc_4k(memman, fsize);
	filebuf = file_loadfile2(finfo->clustno, &fsize, fat);
	env = (struct DLL_STRPICENV *) memman_alloc_4k(memman, sizeof(struct DLL_STRPICENV));
	info_JPEG(env, info, fsize, filebuf);
	picbuf = (struct RGB *) memman_alloc_4k(memman, info[2] * info[3] * sizeof(struct RGB));
	decode0_JPEG(env, fsize, filebuf, 4, (unsigned char *) picbuf, 0);
	x0 = (int) ((x - info[2]) / 2);
	y0 = (int) ((y - info[3]) / 2);
	for (i = 0; i < info[3]; i++) {
		for (j = 0; j < info[2]; j++) {
		r = picbuf[i * info[2] + j].r;
		g = picbuf[i * info[2] + j].g;
		b = picbuf[i * info[2] + j].b;
		vram[(y0 + i) * x + (x0 + j)] = b | g << 8 | r << 16 | 0x00 << 24;
		}
	}
}

/*void init_screen8(int *vram, int x, int y, int bc)
{
	char s[100];
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	sprintf(s, "DrawMode = 0x%05x", bc);
	putfonts8_asc(vram, x, 8, 8, COL8_FFFFFF, s);
	return;
}
*/

void init_screen8(int *vram, int x, int y)
{
int *fat, i;
struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
fat = (int *) memman_alloc_4k(memman, 4 * 2880);
file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
read_wallpaper_32("back.jpg", x, y, fat, vram);
memman_free_4k(memman, (int) fat, 4 * 2880);
//boxfill8(vram, x, 0x00FAFAFA, 0, y - 28, x - 1, y - 28);
//boxfill8(vram, x, COL8_FFFFFF, 0, y - 27, x - 1, y - 27);
boxfill8(vram, x, 0x00F5F5F5, 0, y - 28, x - 1, y - 1);
/*boxfill8(vram, x, COL8_FFFFFF, 3, y - 24, 59, y - 24);
boxfill8(vram, x, COL8_FFFFFF, 2, y - 24, 2, y - 4);
boxfill8(vram, x, COL8_848484, 3, y - 4, 59, y - 4);
boxfill8(vram, x, COL8_848484, 59, y - 23, 59, y - 5);
boxfill8(vram, x, COL8_000000, 2, y - 3, 59, y - 3);
boxfill8(vram, x, COL8_000000, 60, y - 24, 60, y - 3);
boxfill8(vram, x, COL8_848484, x - 47, y - 24, x - 4, y - 24);
boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y - 4);
boxfill8(vram, x, COL8_FFFFFF, x - 47, y - 3, x - 4, y - 3);
boxfill8(vram, x, COL8_FFFFFF, x - 3, y - 24, x - 3, y - 3); */
return;
}

void putfont8(int *vram, int xsize, int x, int y, int c, char *font)
{
	int i;
	int *p;
	char d; /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}
void putfont32(int *vram, int xsize, int x, int y, int c, char *font1, char *font2)
{
    int i,k,j,f;
    int *p;
	char d;
    j=0;
    p=vram+(y+j)*xsize+x;
    j++;
    //????
    for(i=0;i<16;i++)
    {
        for(k=0;k<8;k++)
        {
            if(font1[i]&(0x80>>k))
            {
                p[k+(i%2)*8]=c;
            }
        }
        for(k=0;k<8/2;k++)
        {
            f=p[k+(i%2)*8];
            p[k+(i%2)*8]=p[8-1-k+(i%2)*8];
            p[8-1-k+(i%2)*8]=f;
        }
        if(i%2)
        {
            p=vram+(y+j)*xsize+x;
            j++;
        }
    }
    //????
    for(i=0;i<16;i++)
    {
        for(k=0;k<8;k++)
        {
            if(font2[i]&(0x80>>k))
            {
                p[k+(i%2)*8]=c;
            }
        }
        for(k=0;k<8/2;k++)
        {
            f=p[k+(i%2)*8];
            p[k+(i%2)*8]=p[8-1-k+(i%2)*8];
            p[8-1-k+(i%2)*8]=f;
        }
        if(i%2)
        {
            p=vram+(y+j)*xsize+x;
            j++;
        }
    }
    return;
}


void putfonts8_asc(int *vram, int xsize, int x, int y, int c, unsigned char *s)
{
	extern char hankaku[4096];
	struct TASK *task = task_now();
	char *nihongo = (char *) *((int *) 0x0fe8), *font;
	int k, t;

	if (task->langmode == 0) {
		for (; *s != 0x00; s++) {
			putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
			x += 8;
		}
	}
	if (task->langmode == 1) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if ((0x81 <= *s && *s <= 0x9f) || (0xe0 <= *s && *s <= 0xfc)) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				if (0x81 <= task->langbyte1 && task->langbyte1 <= 0x9f) {
					k = (task->langbyte1 - 0x81) * 2;
				} else {
					k = (task->langbyte1 - 0xe0) * 2 + 62;
				}
				if (0x40 <= *s && *s <= 0x7e) {
					t = *s - 0x40;
				} else if (0x80 <= *s && *s <= 0x9e) {
					t = *s - 0x80 + 63;
				} else {
					t = *s - 0x9f;
					k++;
				}
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );	/* 左半分 */
				putfont8(vram, xsize, x    , y, c, font + 16);	/* 右半分 */
			}
			x += 8;
		}
	}
	if (task->langmode == 2) {
    for (; *s != 0x00; s++) {
        if (task->langbyte1 == 0) {
            if (0xa1 <= *s && *s <= 0xfe) {
                task->langbyte1 = *s;
            } else {
                putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
            }
        } else {
            k = task->langbyte1 - 0xa1;
            t = *s - 0xa1;
            task->langbyte1 = 0;
            font = nihongo + (k * 94 + t) * 32;
            putfont32(vram,xsize,x-8,y,c,font,font+16);    
        }
        x += 8;
    }
}
	return;
}

void init_mouse_cursor8(int *mouse, int bc)
//2. 原先? void init_mouse_cursor8(char *mouse, char bc)
/* マウスカーソルを準備（16x16） */
{
	static char cursor[16][16] = {
		"*...............",
		"**..............",
		"*O*.............",
		"*OO*............",
		"*OOO*...........",
		"*OOOO*..........",
		"*OOOOO*.........",
		"*OOOOOO*........",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOOO*****.....",
		"*OO**OO*........",
		"*O*.*OO*........",
		"**...*OO*.......",
		"*....*OO*.......",
		"......**........"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(int *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, int *buf, int bxsize)
//3. 
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}


