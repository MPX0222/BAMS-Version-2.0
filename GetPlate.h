#pragma once

// code by C

/**
 * Code by C
 * @version 1.35 2020-08-12
 * @author MPX
*/

#include<stdio.h>
#include<ctype.h>
#include<windows.h> //��Ҫʹ�õ�ϵͳ�ṩ�ļ��а�clipboard

#define N 2000

//�Զ���ȡ����������
int GetPlateString(FILE* fp);
//�Զ���ȡ���ƴ��뵽������
int StringAutoCopy(FILE* fp);
//��ȡ�ļ���������
char* GetFileString(int m, int n, char* mystring, FILE* fp);


int GetPlateString(FILE* fp)
{
	char* pbuf = NULL;
	int i, count = 0;//count ��ʾ���ʵ�����

	HANDLE hclip;//���а���

	//�򿪼��а壬��ȡ���������

	if (OpenClipboard(NULL) == 0)
	{
		printf("fail to open the Plate\n");
		return -1;
	}

	//���а嵱�е������ǲ����ı����͵�
	if (!IsClipboardFormatAvailable(CF_TEXT))
	{
		printf("���а嵱�е��������Ͳ�ƥ��!\n");
		CloseClipboard();
		return -1;
	}

	//��ȡ���а����������
	hclip = GetClipboardData(CF_TEXT);
	pbuf = (char*)GlobalLock(hclip);  //����������һ��VOID���͵�ָ��
	GlobalUnlock(hclip);  //����

	fprintf(fp, "%s", pbuf);
	printf("%s", pbuf);

	CloseClipboard();//�رռ��а壬��Ȼ���������޷�����0ʹ�ü��а�

	return 0;
}

int StringAutoCopy(FILE* fp)
{
    if (!OpenClipboard(NULL) || !EmptyClipboard())
    {
        printf("�򿪻���ռ��а����\n");
        return;
    }

    HGLOBAL hMen;
    char strText[10000];

    GetFileString(0, 2000, strText, fp);

    // ����ȫ���ڴ�    
    hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(strText) + 1) * sizeof(char)));

    if (!hMen)
    {
        printf("����ȫ���ڴ����\n");
        // �رռ��а�    
        CloseClipboard();
        return;
    }

    // �����ݿ�����ȫ���ڴ���   
    // ��ס�ڴ���    
    LPSTR lpStr = (LPSTR)GlobalLock(hMen);

    // �ڴ渴��   
    memcpy(lpStr, strText, ((strlen(strText)) * sizeof(char)));
    // �ַ�������    
    lpStr[strlen(strText)] = (TCHAR)0;
    // �ͷ���    
    GlobalUnlock(hMen);

    // ���ڴ��е����ݷŵ����а���   
    SetClipboardData(CF_TEXT, hMen);
    CloseClipboard();
    printf("���Ƴɹ���");

	return 0;
}


char* GetFileString(int m, int n, char* mystring, FILE* fp) {
    int i;
    char str[N];
    *mystring = '\0';

    fseek(fp, 0L, SEEK_SET);
    for (i = 1; i < m; i++) {
        if (fgets(str, N, fp) == NULL) {
            //printf("�ļ����Ȳ���");
            fclose(fp);
            return NULL;
        }
    }

    for (i = 1; i < n; i++) {
        if (fgets(str, N, fp) == NULL) {
            //printf("�ļ����Ȳ���");
            fclose(fp);
            return NULL;
        }
        strcat_s(mystring, strlen(mystring) + strlen(str) + 1, str);
    }
    fclose(fp);
    return mystring;
}