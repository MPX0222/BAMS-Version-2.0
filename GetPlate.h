#pragma once

// code by C

/**
 * Code by C
 * @version 1.35 2020-08-12
 * @author MPX
*/

#include<stdio.h>
#include<ctype.h>
#include<windows.h> //需要使用到系统提供的剪切板clipboard

#define N 2000

//自动读取剪贴板数据
int GetPlateString(FILE* fp);
//自动读取复制代码到剪贴板
int StringAutoCopy(FILE* fp);
//读取文件多行内容
char* GetFileString(int m, int n, char* mystring, FILE* fp);


int GetPlateString(FILE* fp)
{
	char* pbuf = NULL;
	int i, count = 0;//count 表示单词的数量

	HANDLE hclip;//剪切板句柄

	//打开剪切板，获取里面的数据

	if (OpenClipboard(NULL) == 0)
	{
		printf("fail to open the Plate\n");
		return -1;
	}

	//剪切板当中的数据是不是文本类型的
	if (!IsClipboardFormatAvailable(CF_TEXT))
	{
		printf("剪切板当中的数据类型不匹配!\n");
		CloseClipboard();
		return -1;
	}

	//获取剪切板里面的数据
	hclip = GetClipboardData(CF_TEXT);
	pbuf = (char*)GlobalLock(hclip);  //加锁，返回一个VOID类型的指针
	GlobalUnlock(hclip);  //解锁

	fprintf(fp, "%s", pbuf);
	printf("%s", pbuf);

	CloseClipboard();//关闭剪切板，不然其他程序无法正常0使用剪切板

	return 0;
}

int StringAutoCopy(FILE* fp)
{
    if (!OpenClipboard(NULL) || !EmptyClipboard())
    {
        printf("打开或清空剪切板出错！\n");
        return;
    }

    HGLOBAL hMen;
    char strText[10000];

    GetFileString(0, 2000, strText, fp);

    // 分配全局内存    
    hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(strText) + 1) * sizeof(char)));

    if (!hMen)
    {
        printf("分配全局内存出错！\n");
        // 关闭剪切板    
        CloseClipboard();
        return;
    }

    // 把数据拷贝考全局内存中   
    // 锁住内存区    
    LPSTR lpStr = (LPSTR)GlobalLock(hMen);

    // 内存复制   
    memcpy(lpStr, strText, ((strlen(strText)) * sizeof(char)));
    // 字符结束符    
    lpStr[strlen(strText)] = (TCHAR)0;
    // 释放锁    
    GlobalUnlock(hMen);

    // 把内存中的数据放到剪切板上   
    SetClipboardData(CF_TEXT, hMen);
    CloseClipboard();
    printf("复制成功！");

	return 0;
}


char* GetFileString(int m, int n, char* mystring, FILE* fp) {
    int i;
    char str[N];
    *mystring = '\0';

    fseek(fp, 0L, SEEK_SET);
    for (i = 1; i < m; i++) {
        if (fgets(str, N, fp) == NULL) {
            //printf("文件长度不足");
            fclose(fp);
            return NULL;
        }
    }

    for (i = 1; i < n; i++) {
        if (fgets(str, N, fp) == NULL) {
            //printf("文件长度不足");
            fclose(fp);
            return NULL;
        }
        strcat_s(mystring, strlen(mystring) + strlen(str) + 1, str);
    }
    fclose(fp);
    return mystring;
}