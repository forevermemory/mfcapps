#include <Windows.h>


#define BASE_ADDR_������ 0x00000287B0B76580
#define BASE_ADDR_PACKAGE 0x1E3F3B6A3C0

#define USER_ADDRESS_START 0x400000
#define USER_ADDRESS_END 0x00007fffffffffff
#define USER_ADDRESS_LEN ( (USER_ADDRESS_END - USER_ADDRESS_START) / 8)


typedef struct BigObject {
	ULONG64  magic;
	ULONG64  r2;
	ULONG64  r3;
	ULONG64  code;
	ULONG64  r5;
	ULONG64  r6;
	ULONG64  arr;

	// �����ֽ�code2 - �����������    �����ֽڴ�С 
	ULONG64  size;   
	//DWORD  code2;
	//DWORD  size;

} BigObject, _BigObject, * PBigObject;


typedef  struct GoodBase {
	ULONG64 number;
	ULONG64 detail;
	ULONG64 reserved;
} GoodBase, _GoodBase, * PGoodBase;


typedef  struct GoodDetail_0x61_ͼ�� {
	ULONG64 reversed;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;
	ULONG64 name;
	ULONG64 r10;
	ULONG64  desc; // ���� +0x

	ULONG64 arr[22];

} GoodDetail_0x61_ͼ��,  * PGoodDetail_0x61_ͼ��;


typedef  struct GoodDetail_0xF8_���� {
	ULONG64 reversed;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;
	ULONG64 name;
	ULONG64 r10;
	ULONG64  desc; // ���� +0x

} GoodDetail_0xF8_����, * PGoodDetail_0xF8_����;


typedef  struct GoodDetail_0x44_����� {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0x44_�����, * PGoodDetail_0x44_�����;


typedef  struct GoodDetail_0x15_���ͱ� {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
} GoodDetail_0x15_���ͱ�, * PGoodDetail_0x15_���ͱ�;


typedef  struct GoodDetail_0x20_��������Դ���� {
	ULONG64 r1;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
} GoodDetail_0x20_��������Դ����, * PGoodDetail_0x20_��������Դ����;


typedef  struct GoodDetail_0x71 {
	ULONG64 r1;
	ULONG64 r2;

} GoodDetail_0x71, * PGoodDetail_0x71;

typedef  struct GoodDetail_0x1c6_������ {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 desc;

} GoodDetail_0x1c6_������, * PGoodDetail_0x1c6_������;


typedef  struct GoodDetail_15C0AE_������ {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_15C0AE_������, * PGoodDetail_15C0AE_������;

typedef  struct GoodDetail_0x222_������ {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0x222_������, * PGoodDetail_0x222_������;

typedef  struct GoodDetail_A4B16_���� {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;
	ULONG64 r9;
	ULONG64 r10;
	ULONG64 r11;
	ULONG64 r12;
	ULONG64 r13;
	ULONG64 r14;

} GoodDetail_A4B16_����, * PGoodDetail_A4B16_����;

typedef  struct GoodDetail_2aa_װ��2 {
	ULONG64 r1;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 desc;
	ULONG64 r9;
	ULONG64 r10;
	ULONG64 kind;
	ULONG64 r[14];

} GoodDetail_2aa_װ��2, * PGoodDetail_2aa_װ��2;



typedef  struct GoodDetail_9FB86_���� {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;


} GoodDetail_9FB86_����, * PGoodDetail_9FB86_����;


typedef  struct GoodDetail_0xa_���� {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 desc;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0xa_����, * PGoodDetail_0xa_����;

typedef  struct GoodDetail_554_������Ʒ {
	ULONG64 reversed;
	ULONG64 toBrief;
	ULONG64 r3;
	ULONG64  name;
	ULONG64  r5;
	ULONG64  r6;
	ULONG64  r7;
	ULONG64  desc; // ���� +0x

} GoodDetail_554_������Ʒ, * PGoodDetail_554_������Ʒ;



typedef  struct StructDetail {
	ULONG64 reversed;
	ULONG64  name;
	ULONG64  type3;
	ULONG64  desc; // ���� +0x

	ULONG64 buf[5];

} StructDetail, _StructDetail, * PStructDetail;

typedef  struct CodeThread {
	UCHAR code[8];
	ULONG64  length;
} CodeThread, * PCodeThread;

void PrintPackages(ULONG64 base);
void PrintStructs(ULONG64 base);
void PrintRamdoms(ULONG64 arr, ULONG64 siz, ULONG64 code);

void FindAllObjects(ULONG64 exp);
void FindAllObjectsULONG64(ULONG64 exp);

void FindAllObjectsWithCharCode(PUCHAR code, ULONG64 length);
