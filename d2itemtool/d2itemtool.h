#ifndef H_INCLUDE_D2ITEMTOOL

#define H_INCLUDE_D2ITEMTOOL

#define STORE_LOC_INVENTORY 1
#define STORE_LOC_STASH 5
#define STASH_COLUMN 6
#define STASH_ROW 8
#define STASH_COLUMN_NONEXP 6
#define STASH_ROW_NONEXP 4
#define INVENTORY_COLUMN 10
#define INVENTORY_ROW 4
#define CHAR_FLAG_EXPANSION 0x20
#pragma pack(push)

#pragma pack(1)

struct t_d2_prop
{
	int code;
	char* desc;
	int param_count;
	int param_len[4];
};

struct t_d2_item_prop
{
	unsigned short prop_code;
	unsigned int prop_args[4];
};

struct t_d2_item_prop_list
{
	int count;
	t_d2_item_prop props[64];
};

struct t_d2_item_ear_struct
{
	unsigned char ear_class;
	unsigned char ear_level;
	unsigned char player[19];
};

struct t_d2_item_extended_struct
{
	unsigned char gems;
	unsigned int guid;
	unsigned char drop_level;
	unsigned char quality;
	unsigned char vargfx;
	unsigned char n_vargfx;
	unsigned char has_class;
	unsigned short class_info;
	//quality 1 stuff
	unsigned char low_quality;
	//quality 3 stuff
	unsigned char high_quality;
	//quality 4 stuff
	unsigned short magic_prefix;
	unsigned short magic_suffix;
	//quality 5 stuff
	unsigned short set_id;
	//quality 6, 8 stuff
	unsigned char rare_name_1;
	unsigned char rare_name_2;
	unsigned char has_prefix_1;
	unsigned short prefix_1;
	unsigned char has_suffix_1;
	unsigned short suffix_1;
	unsigned char has_prefix_2;
	unsigned short prefix_2;
	unsigned char has_suffix_2;
	unsigned short suffix_2;
	unsigned char has_prefix_3;
	unsigned short prefix_3;
	unsigned char has_suffix_3;
	unsigned short suffix_3;
	//quality 7 stuff
	unsigned short unique_id;
	//pop
	unsigned short rune_word;
	unsigned char inscribed_name[16];
	unsigned char spell_id;
};

struct t_d2_item_specified_struct
{
	unsigned short def;
	unsigned char max_dur;
	unsigned short cur_dur;
	unsigned char socket_num;
	unsigned short quantity;
	//quality 5 stuff
	unsigned char has_set_prop[5];
	//pop
	t_d2_item_prop_list props;
	t_d2_item_prop_list set_props[5];
	t_d2_item_prop_list rw_props;
};

struct t_d2_item_info_struct
{
	char type[5];
	t_d2_item_extended_struct extended_format;
	unsigned char has_rand;
	unsigned int pad1;
	unsigned int pad2;
	unsigned int pad3;
	t_d2_item_specified_struct specified_format;
};

struct t_d2_item_struct
{
	unsigned char majic[2];
	unsigned char quest_item;
	unsigned char unk21;
	unsigned char identified;
	unsigned char unk25;
	unsigned char unk30;
	unsigned char duplicated;
	unsigned char socketed;
	unsigned char unk34;
	unsigned char illegal;
	unsigned char unk37;
	unsigned char ear;
	unsigned char starter;
	unsigned char unk42;
	unsigned char unk44;
	unsigned char simple;
	unsigned char ethereal;
	unsigned char unk47;
	unsigned char inscribed;
	unsigned char unk51;
	unsigned char rune_word;
	unsigned char unk53;
	unsigned char unk60;
	unsigned char unk70;
	unsigned char location;
	unsigned char position;
	unsigned char col;
	unsigned char row;
	unsigned char stored_loc;
	t_d2_item_ear_struct ear_format;
	t_d2_item_info_struct info_format;
};

struct t_d2_item_list
{
	unsigned char majic[2];
	unsigned short count;
	unsigned char char_flag;
	unsigned short real_count;
	t_d2_item_struct items[128];
};

struct t_d2_item_slot
{
	int stored_loc;
	int x;
	int y;
};

struct t_d2_item_type
{
	int catalog;
	char type[4];
	int stackable;
	int inv_width;
	int inv_height;
	int unique;
};

#pragma pack(pop)

int item_readlist(char* filename, t_d2_item_list* dst);
int item_overwritelist(char* filename, const t_d2_item_list* items);

#endif