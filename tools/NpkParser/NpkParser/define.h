
#include <Windows.h>


#define MAX_FILENAME_LENGTH 256
#define MAX_HEADER_FLAG 16

char FileNameFlag[MAX_FILENAME_LENGTH] = "puchikon@neople dungeon and fighter DNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNFDNF";
char HeaderFlag[MAX_HEADER_FLAG] = "NeoplePack_Bill";
char ImgFlag[MAX_HEADER_FLAG] = "Neople Img File";

//IMAGE_SIGNATURE = 'Neople Image File'
//IMG_SIGNATURE = 'Neople Img File'

// 16�ֽ�magic
// 4�ֽ�������          NPK_Header
// 4 4 256 �ṹ������   NPK_Index
// ....
// 
// (16 + 4 + 264 * n ) / 17 * 17  
// 32�ֽڣ���ǰ�� ȡ����(�����ֽ����� / 17) * 17 ��ô���ֽڣ�����sha256

// data  
// ImgF_Header 
//	 char flag[MAX_HEADER_FLAG]; // �ļ���ʯ"Neople Img File"
//	 UINT32 index_size;	// �������С�����ֽ�Ϊ��λ
//	 UINT32 unknown1;
//	 UINT32 unknown2;
//	 UINT32 index_count;// ��������Ŀ
//	 UINT32 dwType; //Ŀǰ��֪�������� 0x0E(1555��ʽ) 0x0F(4444��ʽ) 0x10(8888��ʽ) 0x11(�������κ����ݣ�������ָ����ͬ��һ֡)
//	 UINT32 dwCompress; // Ŀǰ��֪�������� 0x06(zlibѹ��) 0x05(δѹ��)



typedef struct _NPK_Header
{
    char flag[MAX_HEADER_FLAG]; // 16�ֽ�magic
    UINT32 count;      // 4�ֽ� ������
}NPK_Header , *PNPK_Header;

typedef struct _NPK_Index {
    UINT32 offset;  // �����ļ���ͷƫ��
    UINT32 size;    // �ļ���С 
    char data[MAX_FILENAME_LENGTH]; // �ļ��� ��FileNameFlag��򼴿�

} NPK_Index, * PNPK_Index;



typedef struct _ImgF_Header
{
	char flag[MAX_HEADER_FLAG]; // �ļ���ʯ"Neople Img File"
	UINT32 index_size;	// �������С�����ֽ�Ϊ��λ
	UINT32 unknown1;
	UINT32 unknown2;
	UINT32 index_count;// ��������Ŀ


}ImgF_Header, *PImgF_Header;




typedef struct _PngF_Type
{
	// Ŀǰ��֪�������� 0x0E(1555��ʽ) 0x0F(4444��ʽ) 0x10(8888��ʽ) 0x11(�������κ����ݣ�������ָ����ͬ��һ֡)
	UINT32 color_model;
	UINT32 dwCompress; // Ŀǰ��֪�������� 0x06(zlibѹ��) 0x05(δѹ��)

	UINT32 width;        // ���
	UINT32 height;       // �߶�
	UINT32 size;         // ѹ��ʱsizeΪѹ�����С��δѹ��ʱsizeΪת����8888��ʽʱռ�õ��ڴ��С
	UINT32 key_x;        // X�ؼ��㣬��ǰͼƬ����ͼ�е�X����
	UINT32 key_y;        // Y�ؼ��㣬��ǰͼƬ����ͼ�е�Y����
	UINT32 max_width;    // ��ͼ�Ŀ��
	UINT32 max_height;   // ��ͼ�ĸ߶ȣ��д�������Ϊ�˶��뾫��

}PngF_Type , *PPngF_Type;