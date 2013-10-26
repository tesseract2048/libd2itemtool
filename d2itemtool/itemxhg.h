#ifndef H_INCLUDE_ITEMXHG

#define H_INCLUDE_ITEMXHG

int xhg_readcsv(char* filename);
int xhg_calc_xchgitems(const t_d2_item_list* src, int* ret, int* count);
int xhg_xchg(char* filename);

#endif