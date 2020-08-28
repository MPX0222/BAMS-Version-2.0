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

FILE* fp;
FILE* fp2;
FILE* fp3;
FILE* fp4;
FILE* fp5; //注册表文件
FILE* fp6; //注册表修改临时文件

//登录
int DT_Login(char* username, char* password);
//代码数据库界面
int DT_Database_Func();
//代码数据表界面
int DT_Table_Func(TCHAR* db_name);
//创建数据库
int DT_Database_Create(TCHAR* db_name);
//删除数据库
int DT_Database_Delete(TCHAR* db_name);
//调用数据库
int DT_Database_Open(TCHAR* db_name);
//创建代码数据文件
int DT_Table_Create(char* db_url, BTree* T, BTree_node* node, char* setting);
//删除代码数据文件
int DT_Table_Delete(char* db_url, BTree* T, BTree_node* node, char* seeting);
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

int DT_Table_Create(char* db_url, BTree* T, BTree_node* node, char* setting)  //----数据表创建模块（四级界面）----//  用户根据提示创建数据表
{
	system("cls");
	int flag = 0, n = 0, i = 0, line;
	char research_id[10] = { 0 };
	char Rid[14], file_name[30], file_url[45], code[200];
	char* p;
	errno_t err;

	
	printf("-----------------------------------\n");
	printf("新建代码数据文件\n");
	printf("-----------------------------------\n");
	printf("标准格式：202001-01\n");
	printf("请按照格式输入代码数据文件的编号：");
	rewind(stdin);
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

	err = fopen_s(&fp5, setting, "a+");
	fprintf(fp5, "%s\n", file_url);
	fclose(fp5);

	if (BTree_Search(&T, node, key, 0) == -1)
	{
		BTree_Insert(T, key);
		err = fopen_s(&fp2, file_url, "w");  //以只写方式打开，若文件不存在则建立文件
		printf("-----------------------------------\n");
		printf("请输入数据（可复制）:\n");
		printf("----------------------------------\n");
		GetPlateString(fp2);
		fclose(fp2);
		fflush(stdin);
		printf("创建成功\n");
		printf("-----------------------------------\n");
		printf("按下任意键继续......");
		_getch();
		return 0;
	}
	else
	{
		system("cls");
		printf("-----------------------------------\n");
		printf("该数据表名称已经存在，或命名错误\n");
		printf("-----------------------------------\n");
		printf("您有以下选择:\n");
		printf("1.重新创建数据表\n2.取消创建数据表\n3.退出程序\n");
		printf("-----------------------------------\n");
		printf("请输入您的选择：");
		scanf_s("%d", &flag);
		if (flag == 1) return 1;  //CASE_1：重新创建
		if (flag == 2) return 0;  //CASE_2：取消创建
		if (flag == 3) exit(0);  //CASE_3：退出程序
		else
		{
			printf("-----------------------------------\n");
			printf("选项不存在！\n取消创建数据表");
			Sleep(2000);
			return 0;
		}
	}
}

int DT_Table_Delete(char* db_url, BTree* T, BTree_node* node, char* setting)  //----数据表删除模块----//
{
	system("cls");
	int flag;
	errno_t err, err1;
	char research_id[10] = { 0 };
	char Rid[14], file_name[30], file_url[45], haven[45], code[200];
	char* p;

	printf("-----------------------------------\n");
	printf("删除代码数据文件\n");
	printf("-----------------------------------\n");
	printf("标准格式：202001-01\n");
	printf("请按照格式输入代码数据文件的编号：");
	rewind(stdin);
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

	BTree_Print(&T, T->root, 0);
	if (BTree_Search(&T, node, key, 0) != -1)
	{
		printf("-----------------------------------\n");
		printf("该代码数据文件存在！\n");
		printf("B树查询编号 SID ：%d\n", key);
		printf("代码数据文件名 RID ：%s\n", file_url);
		printf("-----------------------------------\n");

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
		{
			BTree_DeleteTree(T, key);
			printf("删除成功！\n");
			printf("-----------------------------------\n");
			printf("按下任意键继续......");
			_getch();
			return 0;
		}
		else  //删除失败
		{
			system("cls");
			printf("-----------------------------------\n");
			printf("删除代码数据文件出错！\n");
			printf("-----------------------------------\n");
			printf("您有以下选择：\n");
			printf("1.重新删除\n2.结束删除\n3.退出程序\n");
			printf("-----------------------------------\n");
			printf("请输入您的选择：");
			scanf_s("%d", &flag);
			if (flag == 1) return 1;
			if (flag == 2) return 0;
			if (flag == 3) exit(0);
			else
			{
				printf("-----------------------------------\n");
				printf("没有该选项\n结束删除");
				//Sleep(1000);
				return 0;
			}
		}
	}
	  //CASE_2:该数据表不在数据库内
	system("cls");
	printf("-----------------------------------\n");
	printf("该代码数据文件不存在于库！\n");
	printf("-----------------------------------\n");
	printf("您有以下选择：\n");
	printf("1.删除其他\n2.取消删除\n3.退出程序\n");
	printf("-----------------------------------\n");
	printf("请输入您的选择：");
	scanf_s("%d", &flag);
	if (flag == 1) return 1;
	if (flag == 2) return 0;
	if (flag == 3) exit(0);
	else
	{
		printf("-----------------------------------\n");
		printf("没有该选项\n取消删除");
		Sleep(2000);
		return 0;
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
