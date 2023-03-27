#include <Windows.h>


#define BASE_ADDR_建筑物 0x00000287B0B76580
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

	// 高四字节code2 - 这个才是类型    低四字节大小 
	ULONG64  size;   
	//DWORD  code2;
	//DWORD  size;

} BigObject, _BigObject, * PBigObject;


typedef  struct GoodBase {
	ULONG64 number;
	ULONG64 detail;
	ULONG64 reserved;
} GoodBase, _GoodBase, * PGoodBase;


typedef  struct GoodDetail_0x61_图鉴 {
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
	ULONG64  desc; // 描述 +0x

	ULONG64 arr[22];

} GoodDetail_0x61_图鉴,  * PGoodDetail_0x61_图鉴;


typedef  struct GoodDetail_0xF8_抗性 {
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
	ULONG64  desc; // 描述 +0x

} GoodDetail_0xF8_抗性, * PGoodDetail_0xF8_抗性;


typedef  struct GoodDetail_0x44_不变表 {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0x44_不变表, * PGoodDetail_0x44_不变表;


typedef  struct GoodDetail_0x15_类型表 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
} GoodDetail_0x15_类型表, * PGoodDetail_0x15_类型表;


typedef  struct GoodDetail_0x20_可能是资源对象 {
	ULONG64 r1;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
} GoodDetail_0x20_可能是资源对象, * PGoodDetail_0x20_可能是资源对象;


typedef  struct GoodDetail_0x71 {
	ULONG64 r1;
	ULONG64 r2;

} GoodDetail_0x71, * PGoodDetail_0x71;

typedef  struct GoodDetail_0x1c6_建筑物 {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 desc;

} GoodDetail_0x1c6_建筑物, * PGoodDetail_0x1c6_建筑物;


typedef  struct GoodDetail_15C0AE_建筑物 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_15C0AE_建筑物, * PGoodDetail_15C0AE_建筑物;

typedef  struct GoodDetail_0x222_建筑物 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0x222_建筑物, * PGoodDetail_0x222_建筑物;

typedef  struct GoodDetail_A4B16_描述 {
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

} GoodDetail_A4B16_描述, * PGoodDetail_A4B16_描述;

typedef  struct GoodDetail_2aa_装备2 {
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

} GoodDetail_2aa_装备2, * PGoodDetail_2aa_装备2;



typedef  struct GoodDetail_9FB86_描述 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;


} GoodDetail_9FB86_描述, * PGoodDetail_9FB86_描述;


typedef  struct GoodDetail_0xa_技能 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 desc;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_0xa_技能, * PGoodDetail_0xa_技能;

typedef  struct GoodDetail_554_背包物品 {
	ULONG64 reversed;
	ULONG64 toBrief;
	ULONG64 r3;
	ULONG64  name;
	ULONG64  r5;
	ULONG64  r6;
	ULONG64  r7;
	ULONG64  desc; // 描述 +0x

} GoodDetail_554_背包物品, * PGoodDetail_554_背包物品;



typedef  struct StructDetail {
	ULONG64 reversed;
	ULONG64  name;
	ULONG64  type3;
	ULONG64  desc; // 描述 +0x

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
