// NpkParser.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <Windows.h>
#include "define.h"
#pragma warning(disable:4996)

const char* fname = "D:\\sprite_map_npc_20_illusion.NPK";
//const char * fname = "D:\\A金币闪光.NPK";


void parseImg(char * filebuff, PNPK_Index item)
{
    PImgF_Header imgHeader = (PImgF_Header)((UINT64)filebuff + item->offset);

    printf("\t flag:%s, 大小:%d, %d-%d, 索引数量:%d\n",
        imgHeader->flag,
        imgHeader->index_size,

        imgHeader->unknown1,
        imgHeader->unknown2,
        imgHeader->index_count);

    for (size_t i = 0; i < imgHeader->index_count; i++)
    {
        PPngF_Type pType = (PPngF_Type)
    }
}

int main()
{
    printf("helloworld\n");

    PNPK_Header header;
    PNPK_Index items;

    FILE* fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("open file fail\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    char * filebuff = (char * )malloc(len);
    if(filebuff == NULL)
    {
        printf("malloc fail\n");
        goto FAIL;
    }
    memset(filebuff, 0, len);

    rewind(fp); // 移动文件指针到开头
    fread(filebuff, 1, len, fp);

    // 解析
    header = (PNPK_Header)filebuff;
    if (strcmpi(header->flag, HeaderFlag) != 0)
    {
        printf("不是有效的npk文件\n");
        goto FAIL;
    }

    printf("子文件数量:%d\n", header->count);

    items = (PNPK_Index)((UINT64)filebuff + sizeof(NPK_Header));
    for (size_t i = 0; i < header->count; i++)
    {
        for (size_t j = 0; j < MAX_FILENAME_LENGTH; j++)
        {
            items->data[j] ^= FileNameFlag[j];
        }
        printf("offset:%d , size:%d, name:%s \n", items->offset, items->size, items->data);

        parseImg(filebuff, items);
        items++;
    }


FAIL:
    fclose(fp);
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
