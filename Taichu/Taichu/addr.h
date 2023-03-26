#include <Windows.h>


#define BASE_ADDR_建筑物 0x00000287B0B76580
#define BASE_ADDR_PACKAGE 0x1E3F3B6A3C0

#define USER_ADDRESS_START 0x400000
#define USER_ADDRESS_END 0x00007fffffffffff
#define USER_ADDRESS_LEN ( (USER_ADDRESS_END - USER_ADDRESS_START) / 8)



typedef  struct GoodBase {
	ULONG64 number;
	ULONG64 detail;
	ULONG64 reserved;
} GoodBase, _GoodBase, * PGoodBase;

typedef  struct GoodDetail {
	ULONG64 reversed;
	ULONG64 toBrief;
	ULONG64 type3;
	ULONG64  name;
	ULONG64  type5;
	ULONG64  type6;
	ULONG64  type7;
	ULONG64  desc; // 描述 +0x

	ULONG64 buf[28];

} GoodDetail, _GoodDetail, * PGoodDetail;

typedef  struct GoodDetail_A4947_图鉴 {
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

} GoodDetail_A4947_图鉴,  * PGoodDetail_A4947_图鉴;


typedef  struct GoodDetail_A44394_禁用 {
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

} GoodDetail_A44394_禁用, * PGoodDetail_A44394_禁用;


typedef  struct GoodDetail_15C50E_不变表 {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_15C50E_不变表, * PGoodDetail_15C50E_不变表;

typedef  struct GoodDetail_26E292_建筑物 {
	ULONG64 reversed;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 desc;

} GoodDetail_26E292_建筑物, * PGoodDetail_26E292_建筑物;


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

typedef  struct GoodDetail_15C102_建筑物 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_15C102_建筑物, * PGoodDetail_15C102_建筑物;

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

typedef  struct GoodDetail_272CA3_家具 {
	ULONG64 r1;
	ULONG64 name;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;
	ULONG64 r9;
	ULONG64 r10;
	ULONG64 kind;
	ULONG64 r[14];

} GoodDetail_272CA3_家具, * PGoodDetail_272CA3_家具;



typedef  struct GoodDetail_9FB86_描述 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 r3;
	ULONG64 r4;
	ULONG64 r5;


} GoodDetail_9FB86_描述, * PGoodDetail_9FB86_描述;


typedef  struct GoodDetail_9C31E_技能 {
	ULONG64 name;
	ULONG64 r2;
	ULONG64 desc;
	ULONG64 r4;
	ULONG64 r5;
	ULONG64 r6;
	ULONG64 r7;
	ULONG64 r8;

} GoodDetail_C31E_技能, * PGoodDetail_C31E_技能;

typedef  struct GoodDetail_A4718_背包物品 {
	ULONG64 reversed;
	ULONG64 toBrief;
	ULONG64 r3;
	ULONG64  name;
	ULONG64  r5;
	ULONG64  r6;
	ULONG64  r7;
	ULONG64  desc; // 描述 +0x

} GoodDetail_A4718_背包物品, * PGoodDetail_A4718_背包物品;



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

typedef struct BigObject {
	ULONG64  magic;
	ULONG64  r2;
	ULONG64  r3;
	ULONG64  code;
	ULONG64  r5;
	ULONG64  r6;
	ULONG64  arr;


 	ULONG64  size;   // 高四字节code2 低四字节大小
	//DWORD  code2;
	//DWORD  size;

} BigObject, _BigObject, * PBigObject;

void PrintPackages(ULONG64 base);
void PrintStructs(ULONG64 base);
void PrintRamdoms(ULONG64 arr, ULONG64 siz, ULONG64 code);

void FindAllObjects(ULONG64 exp);
void FindAllObjectsULONG64(ULONG64 exp);

void FindAllObjectsWithCharCode(PUCHAR code, ULONG64 length);
