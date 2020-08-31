#pragma once

/**
 * Code by C
 * @version 1.35 2020-08-12
 * @author MPX
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<conio.h>
#include<ctype.h>
#include<errno.h>
#include<tchar.h>

#include "Btree.h"
#include "GetPlate.h"

#define MAXLEN 10
#define MAXTIME 3

FILE* fp2;
FILE* fp3;
FILE* fp4;
FILE* fp5; //注册表文件
FILE* fp6; //注册表修改临时文件

//登录
int DT_Login(char* username, char* password);
//代码数据库界面
int DT_Database_Func();
//数据库文件读取函数
int DT_DatabaseFile_Read(TCHAR* db_name, BTree* T);
//数据库注册表文件读取函数
int DT_SettingFile_Read(TCHAR* db_name, char* file_name);
//数据库文件重写函数
int DT_DatabaseFile_ReWrite(TCHAR* db_name, BTree* T);
//创建数据库
int DT_Database_Create(TCHAR* db_name);
//删除数据库
int DT_Database_Delete(TCHAR* db_name);
//调用数据库
int DT_Database_Open(TCHAR* db_name);
//创建代码数据文件
int DT_Table_Create(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node);
//删除代码数据文件
int DT_Table_Delete(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node);
//查询代码数据文件
int DT_Data_Select(char* db_url, BTree* T, BTree_node* node); 
//编码转换
int DT_AutoCode(char RID[]); 


int DT_Login(char* username, char* password)
{
	int i = 0, flag = 0;
	char ch = 0;
	TCHAR true_password[] = TEXT("111"), true_username[] = TEXT("111");

	if (!_tcscmp(username, true_username))  //CASE_1：用户名输入正确
	{
		if (!_tcscmp(password, true_password))  //CASE_1_1：密码输入正确
			return 1;
		else  //CASE_1_2：密码输入错误
			return -1;
	}
	else  //CASE_2：用户名输入错误
		return -2;
}


int DT_DatabaseFile_Read(TCHAR* db_name, BTree* T)
{
	char db_url[18], SID[10], haven[16];

	errno_t err;
	FILE* fp;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("代码库.txt") + 1, "代码库.txt");

	BTree_CreateTree(&T, 3);

	if ((err = fopen_s(&fp, db_url, "r")) == 0)
	{
		fseek(fp, 0L, SEEK_SET);
		while (fgets(haven, 16, fp) != NULL)  //读取数据库文件中的文件名，在内存中生成B树
		{
			_tcscpy_s(SID, _tcslen(haven) + 1, haven);  //分离尾缀
			int key = atoi(SID);
			BTree_Insert(&T, key);
		}
		fclose(fp);
		return 0;
	}
	
}

int DT_SettingFile_Read(TCHAR* db_name, char* file_name)
{
	char setting[45];

	errno_t err;
	FILE* fp;

	_tcscpy_s(setting, _tcslen(db_name) + 1, db_name);
	strcat_s(setting, strlen(setting) + strlen("代码库程序注册表.txt") + 1, "代码库程序注册表.txt");

	if((err = fopen_s(&fp, setting, "r")) == 0)
	{
		GetFileString(0, 2000, file_name, fp);
	}

	return 0;
}

int DT_DatabaseFile_ReWrite(TCHAR* db_name, BTree* T)
{
	char db_url[18];
	errno_t err;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("代码库.txt") + 1, "代码库.txt");

	err = fopen_s(&fp3, "db_temp.txt", "w");  //更新数据库文件内容
	BTree_Traverse(&T, T->root, 0);
	remove(db_url);
	fclose(fp3);
	rename("db_temp.txt", db_url);

	return 0;
}

int DT_Database_Create(TCHAR* db_name)
{
	char db_url[18], setting[30];
	errno_t err, err1;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("代码库") + 1, "代码库");

	strcpy_s(setting, strlen(db_url) + 1, db_url);
	strcat_s(setting, strlen(setting) + strlen("程序注册表.txt") + 1, "程序注册表.txt");

	if ((err = fopen_s(&fp, setting, "r")) != 0)						//	检查注册表文件是否存在，防止丢失数据
	{
		err1 = fopen_s(&fp5, setting, "w");
		fclose(fp5);
	}
	else
		return -1;
	
	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	if ((err = fopen_s(&fp, db_url, "r")) != 0)							//	检查库文件是否存在
	{
		err = fopen_s(&fp, db_url, "w");
		fclose(fp);
		return 1;
	}
	else
		return -1;
}

int DT_Database_Delete(TCHAR* db_name)
{
	char db_url[18], haven[18], setting[30];
	errno_t err;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("代码库") + 1, "代码库");

	strcpy_s(setting, strlen(db_url) + 1, db_url);
	strcat_s(setting, strlen(setting) + strlen("程序注册表.txt") + 1, "程序注册表.txt");
	remove(setting);

	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	err = fopen_s(&fp, db_url, "r");  //对相关的库进行遍历删除
	while (fscanf_s(fp, "%s", haven, 18) != EOF) remove(haven);  //使用了fscanf函数用于防止程序读入\n造成错误
	fclose(fp);

	if (!(remove(db_url)))
		return 1;
	else
		return -1;
}

int DT_Database_Open(TCHAR* db_name)
{
	char db_url[18];
	errno_t err;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("代码库") + 1, "代码库");
	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	if ((err = fopen_s(&fp, db_url, "r")) == 0)  //判断数据库是否存在
	{
		fclose(fp);
		return 1;
	}
	else
		return -1;
}

int DT_Table_Create(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node) 
{
	int n = 0, i = 0, line;
	char db_url[18], Rid[14], file_url[45], code[200], setting[45];
	char* p;
	errno_t err, err1;

	strcpy_s(Rid, strlen("TEST") + 1, "TEST");
	_tcscat_s(Rid, strlen(Rid) + _tcslen(rid) + 1, rid);
	int key = DT_AutoCode(rid);

	_tcscpy_s(setting, _tcslen(db_name) + 1, db_name);
	strcat_s(setting, strlen(setting) + strlen("代码库程序注册表.txt") + 1, "代码库程序注册表.txt");

	strcpy_s(file_url, strlen(db_url) + 1, db_url);
	strcat_s(file_url, strlen(file_url) + strlen("-") + 1, "-");
	strcat_s(file_url, strlen(file_url) + strlen(Rid) + 1, Rid);
	strcat_s(file_url, strlen(file_url) + strlen(file_name) + 1, file_name);
	strcat_s(file_url, strlen(file_url) + strlen(".txt") + 1, ".txt");

	//将文件全名写入注册表文件
	err = fopen_s(&fp5, setting, "a+");
	fprintf(fp5, "%s\n", file_url);
	fclose(fp5);

	//将剪贴板数据写入代码数据文件
	if (BTree_Search(&T, node, key, 0) == -1)
	{
		BTree_Insert(T, key);
		err1 = fopen_s(&fp2, file_url, "w");  //以只写方式打开，若文件不存在则建立文件
		//fprintf(fp2, "%s\n", code_data);
		GetPlateString(fp2);
		fclose(fp2);
		return 1;
	}
	else
		return -1;
}

int DT_Table_Delete(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node)
{
	int n = 0, i = 0, line;
	char db_url[18], Rid[14], file_url[45], haven[45], code[200], setting[45];
	char* p;
	errno_t err, err1;

	strcpy_s(Rid, strlen("TEST") + 1, "TEST");
	_tcscat_s(Rid, strlen(Rid) + _tcslen(rid) + 1, rid);
	int key = DT_AutoCode(rid);

	_tcscpy_s(setting, _tcslen(db_name) + 1, db_name);
	strcat_s(setting, strlen(setting) + strlen("代码库程序注册表.txt") + 1, "代码库程序注册表.txt");

	strcpy_s(file_url, strlen(db_url) + 1, db_url);
	strcat_s(file_url, strlen(file_url) + strlen("-") + 1, "-");
	strcat_s(file_url, strlen(file_url) + strlen(Rid) + 1, Rid);
	strcat_s(file_url, strlen(file_url) + strlen(file_name) + 1, file_name);
	strcat_s(file_url, strlen(file_url) + strlen(".txt") + 1, ".txt");

	BTree_Print(&T, T->root, 0);
	if (BTree_Search(&T, node, key, 0) != -1)
	{

		err = fopen_s(&fp5, setting, "r");
		err1 = fopen_s(&fp6, "_temp.txt", "w");
		while (fscanf_s(fp5, "%s\n", haven, 45) != EOF)
		{
			if (strcmp(haven, file_url) == 0)
			{
				continue;
			}
			else fprintf(fp6, "%s\n", haven);
		}
		fclose(fp5);
		remove(setting);
		fclose(fp6);  //先关闭文件，后修改名字
		rename("_temp.txt", setting);

		if (!(remove(file_url)))  //若删除成功则返回0，此处还没有进行测试
			return 1;
		else  //删除失败
			return -1;
		
	}
}


int DT_Data_Select(char* db_url, BTree* T, BTree_node* node)
{
	system("cls");
	int flag = 0, flag1;
	int a;
	char* p;
	char research_id[10] = { 0 };
	char Rid[14], file_name[30], file_url[45], code[1000];
	errno_t err;

	printf("-----------------------------------\n");
	printf("查找代码数据\n");
	printf("-----------------------------------\n");
	printf("标准格式：202001-01\n");
	printf("请按照格式输入代码数据文件的编号：");

	scanf_s("%s", research_id, 10);
	strcpy_s(Rid, strlen("TEST") + 1, "TEST");
	strcat_s(Rid, strlen(Rid) + strlen(research_id) + 1, research_id);
	int key = DT_AutoCode(research_id);

	printf("请输入代码数据文件的标题（10字以内）：");
	scanf_s("%s", file_name, 30);
	strtok_s(db_url, "代码库", &p);
	strcpy_s(file_url, strlen(db_url) + 1, db_url);
	strcat_s(file_url, strlen(file_url) + strlen("-") + 1, "-");
	strcat_s(file_url, strlen(file_url) + strlen(Rid) + 1, Rid);
	strcat_s(file_url, strlen(file_url) + strlen(file_name) + 1, file_name);
	strcat_s(file_url, strlen(file_url) + strlen(".txt") + 1, ".txt");

	if (BTree_Search(&T, node, key, 0) != -1)
	{
		printf("-----------------------------------\n");
		printf("代码数据文件存在！\n");
		printf("B树查询编号 SID ：%d\n", key);
		printf("代码数据文件名 RID ：%s\n", file_url);
		printf("-----------------------------------\n");
		if ((err = fopen_s(&fp2, file_url, "r") == 0))
		{
			while (fgets(code, 1000, fp2) != NULL)
			{
				printf("%s", code);
			}
			printf("查询代码成功！\n");
			//fclose(fp2);
			printf("-----------------------------------\n");

			printf("是否需要复制代码数据?\n1.需要复制\n2.不需要\n");
			printf("-----------------------------------\n");
			printf("请输入您的选择：");
			scanf_s("%d", &flag1);
			if (flag1 == 1) StringAutoCopy(fp2);

			fclose(fp2);
			printf("按下任意键继续......");
			_getch();
			return 0;
		}
		else
		{
			system("cls");
			printf("-----------------------------------\n");
			printf("查询代码数据文件出错！\n");
			printf("-----------------------------------\n");
			printf("您有以下选择：\n");
			printf("1.重新查询\n2.结束查询\n3.退出程序\n");
			printf("-----------------------------------\n");
			printf("请输入您的选择：");
			scanf_s("%d", &flag);
			if (flag == 1) return 1;
			if (flag == 2) return 0;
			if (flag == 3) exit(0);
			else
			{
				printf("没有该选项\n结束查询");
				return 0;
			}
		}
	}
	system("cls");
	printf("-----------------------------------\n");
	printf("该代码数据文件不存在于该数据库！\n");
	printf("-----------------------------------\n");
	printf("您有以下选择：\n");
	printf("1.查询其他\n2.取消查询\n3.退出程序\n");
	printf("-----------------------------------\n");
	printf("请输入您的选择：");
	scanf_s("%d", &flag);
	if (flag == 1) return 1;
	if (flag == 2) return 0;
	if (flag == 3) exit(0);
	else
	{
		printf("-----------------------------------\n");
		printf("没有该选项\n结束查询");
		Sleep(1000);
		return 0;
	}
}


int DT_AutoCode(char RID[])  //----自动编码----//  用户输入查询编号，自动将查询编号 RID 转换为搜索编号 SID
{
	int i,j;
    for(i = 0, j = 0; *(RID + i) != '\0'; i++)
    {
		if (*(RID + i) == '-')
			continue;
        else
        {
			*(RID + j) = *(RID + i);
            j++;
        }
    }
	*(RID + j) = '\0';

	int SID = atoi(RID);
	return SID;
}
