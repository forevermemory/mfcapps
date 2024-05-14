
#include <Windows.h>


#define MAX_FILENAME_LENGTH 256
#define MAX_HEADER_FLAG 16

char FileNameFlag[MAX_FILENAME_LENGTH] = "puchikon@neople dungeon and fighter DNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNF";
char HeaderFlag[MAX_HEADER_FLAG] = "NeoplePack_Bill";
char ImgFlag[MAX_HEADER_FLAG] = "Neople Img File";

//IMAGE_SIGNATURE = 'Neople Image File'
//IMG_SIGNATURE = 'Neople Img File'

// 16字节magic
// 4字节子数量          NPK_Header
// 4 4 256 结构体数组   NPK_Index
// ....
// 
// (16 + 4 + 264 * n ) / 17 * 17  
// 32字节，是前面 取整数(所有字节数量 / 17) * 17 这么多字节，再算sha256

// data  
// ImgF_Header 
//	 char flag[MAX_HEADER_FLAG]; // 文件标石"Neople Img File"
//	 UINT32 index_size;	// 索引表大小，以字节为单位
//	 UINT32 unknown1;
//	 UINT32 unknown2;
//	 UINT32 index_count;// 索引表数目
//	 UINT32 dwType; //目前已知的类型有 0x0E(1555格式) 0x0F(4444格式) 0x10(8888格式) 0x11(不包含任何数据，可能是指内容同上一帧)
//	 UINT32 dwCompress; // 目前已知的类型有 0x06(zlib压缩) 0x05(未压缩)



typedef struct _NPK_Header
{
    char flag[MAX_HEADER_FLAG]; // 16字节magic
    UINT32 count;      // 4字节 子数量
}NPK_Header , *PNPK_Header;

typedef struct _NPK_Index {
    UINT32 offset;  // 距离文件开头偏移
    UINT32 size;    // 文件大小 
    char data[MAX_FILENAME_LENGTH]; // 文件名 和FileNameFlag异或即可

} NPK_Index, * PNPK_Index;



typedef struct _ImgF_Header
{
	char flag[MAX_HEADER_FLAG]; // 文件标石"Neople Img File"
	UINT32 index_size;	// 索引表大小，以字节为单位
	UINT32 unknown1;
	UINT32 unknown2;
	UINT32 index_count;// 索引表数目


}ImgF_Header, *PImgF_Header;




typedef struct _PngF_Type
{
	// 目前已知的类型有 0x0E(1555格式) 0x0F(4444格式) 0x10(8888格式) 0x11(不包含任何数据，可能是指内容同上一帧)
	UINT32 color_model;
	UINT32 dwCompress; // 目前已知的类型有 0x06(zlib压缩) 0x05(未压缩)

	UINT32 width;        // 宽度
	UINT32 height;       // 高度
	UINT32 size;         // 压缩时size为压缩后大小，未压缩时size为转换成8888格式时占用的内存大小
	UINT32 key_x;        // X关键点，当前图片在整图中的X坐标
	UINT32 key_y;        // Y关键点，当前图片在整图中的Y坐标
	UINT32 max_width;    // 整图的宽度
	UINT32 max_height;   // 整图的高度，有此数据是为了对齐精灵

}PngF_Type , *PPngF_Type;