#include "stdafx.h"
#include "d2itemtool.h"
#include <stdlib.h>
#include <stdio.h>

struct t_xhg_node
{
	char name[64];
	int identified;
	int ethereal;
	int drop_level;
	int type_count;
	char type[16][4];
	int quality;
	int param1;
	int param2;
	int quantity;
	int bonus; 
};

struct t_xhg_store
{
	int count;
	t_xhg_node list[128];
};


static t_xhg_store xhg_store;

_declspec(dllexport) int xhg_readcsv(char* filename)
{
	xhg_store.count = 0;
	FILE* fp;
	fp = fopen(filename, "r");
	if (!fp) return -1;
	char buf[512];
	int c = -1;
	while (!feof(fp))
	{
		fgets(buf, 512, fp);
		int len = strlen(buf);
		if (len < 1) continue;
		if (memcmp(buf, "NAME,", 5) == 0)
		{
			c = 0;
			continue;
		}
		int last = 0;
		int n = 0;
		for (int i = 0; i <= len; i ++)
		{
			if (buf[i] == ',' || buf[i] == 0)
			{
				char col[64];
				memcpy(col, &buf[last], i - last);
				col[i - last] = 0;
				switch (n)
				{
				case 0:
					strcpy(xhg_store.list[c].name, col);
					break;
				case 1:
					xhg_store.list[c].identified = strtol(col, NULL, 10);
					break;
				case 2:
					xhg_store.list[c].ethereal = strtol(col, NULL, 10);
					break;
				case 3:
					xhg_store.list[c].drop_level = strtol(col, NULL, 10);
					break;
				case 4:
					xhg_store.list[c].type_count = 0;
					for (int k = 0; k <= (int)strlen(col); k ++)
					{
						if (col[k] == ' ' || col[k] == 0)
						{
							memcpy(xhg_store.list[c].type[xhg_store.list[c].type_count], &col[k - 3], 3);
							xhg_store.list[c].type[xhg_store.list[c].type_count++][4] = 0;
						}
					}
					break;
				case 5:
					xhg_store.list[c].quality = strtol(col, NULL, 10);
					break;
				case 6:
					xhg_store.list[c].param1 = strtol(col, NULL, 16);
					break;
				case 7:
					xhg_store.list[c].param2 = strtol(col, NULL, 16);
					break;
				case 8:
					xhg_store.list[c].quantity = strtol(col, NULL, 10);
					break;
				case 9:
					xhg_store.list[c].bonus = strtol(col, NULL, 10);
					break;
				}
				last = i + 1;
				n ++;
			}
		}
		c ++;
	}
	xhg_store.count = c;
	return xhg_store.count;
}

int xhg_calc_xchgitems(const t_d2_item_list* src, int* ret, int* count)
{
	int mchs[128];
	int mch_item[128][64];
	memset(mchs, 0, sizeof(int) * 128);
	for (int i = 0; i < src->count; i ++)
	{
		int curr_mch = -1;
		if (src->items[i].stored_loc != STORE_LOC_INVENTORY && src->items[i].stored_loc != STORE_LOC_STASH) continue;
		for (int j = 0; j < xhg_store.count; j ++)
		{
			if (xhg_store.list[j].identified != -1 && src->items[i].identified != xhg_store.list[j].identified) continue;
			if (xhg_store.list[j].ethereal != -1 && src->items[i].ethereal != xhg_store.list[j].ethereal) continue;
			if (xhg_store.list[j].drop_level != -1)
			{
				if (src->items[i].simple) continue;
				if (src->items[i].info_format.extended_format.drop_level != xhg_store.list[j].drop_level) continue;
			}
			int found = -1;
			for (int k = 0; k < xhg_store.list[j].type_count; k ++)
			{
				if (memcmp(xhg_store.list[j].type[k], src->items[i].info_format.type, 3) == 0)
				{
					found = k;
					break;
				}
			}
			if (found == -1) continue;
			if (xhg_store.list[j].quality != -1)
			{
				if (src->items[i].simple) continue;
				if (src->items[i].info_format.extended_format.quality != xhg_store.list[j].quality) continue;
				switch (xhg_store.list[j].quality)
				{
				case 5:
					if (xhg_store.list[j].param1 != -1 && xhg_store.list[j].param1 != src->items[i].info_format.extended_format.set_id) continue;
					break;
				case 7:
					if (xhg_store.list[j].param1 != -1 && xhg_store.list[j].param1 != src->items[i].info_format.extended_format.unique_id) continue;
					break;
				}
			}
			mch_item[j][mchs[j]++] = i;
			curr_mch = j;
		}
	}
	int total_bonus = 0;
	int xhg_list[256];
	int pos = 0;
	for (int i = 0; i < xhg_store.count; i ++)
	{
		while (mchs[i] >= xhg_store.list[i].quantity)
		{
			for (int j = 0; j < xhg_store.list[i].quantity; j ++)
			{
				xhg_list[pos++] = mch_item[i][--mchs[i]];
			}
			total_bonus += xhg_store.list[i].bonus;
		}
	}
	*count = pos;
	memcpy(ret, xhg_list, sizeof(int) * pos);
	return total_bonus;
}

_declspec(dllexport) int xhg_xchg(char* filename)
{
	t_d2_item_list items;
	t_d2_item_list items_xchg;
	if (item_readlist(filename, &items) == -1) return -1;
	int xhg_item[256];
	int xhg_item_count;
	int bonus = xhg_calc_xchgitems(&items, xhg_item, &xhg_item_count);
	items_xchg.majic[0] = 'J';
	items_xchg.majic[1] = 'M';
	items_xchg.count = items.count - xhg_item_count;
	int xhg_count = 0;
	for (int i = 0; i < items.count; i ++)
	{
		int found = -1;
		for (int j = 0; j < xhg_item_count; j ++)
		{
			if (xhg_item[j] == i) found = j;
		}
		if (found == -1)
		{
			memcpy((void*)&items_xchg.items[xhg_count++], (void*)&items.items[i], sizeof(t_d2_item_struct));
		}
	}
	if (item_overwritelist(filename, &items_xchg)) goto fail;
	return bonus;
fail:
	return -1;
}
