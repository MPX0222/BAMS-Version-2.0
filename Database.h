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
FILE* fp5; //ע����ļ�
FILE* fp6; //ע����޸���ʱ�ļ�

//��¼
int DT_Login(char* username, char* password);
//�������ݿ����
int DT_Database_Func();
//���ݿ��ļ���ȡ����
int DT_DatabaseFile_Read(TCHAR* db_name, BTree* T);
//���ݿ�ע����ļ���ȡ����
int DT_SettingFile_Read(TCHAR* db_name, char* file_name);
//���ݿ��ļ���д����
int DT_DatabaseFile_ReWrite(TCHAR* db_name, BTree* T);
//�������ݿ�
int DT_Database_Create(TCHAR* db_name);
//ɾ�����ݿ�
int DT_Database_Delete(TCHAR* db_name);
//�������ݿ�
int DT_Database_Open(TCHAR* db_name);
//�������������ļ�
int DT_Table_Create(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node);
//ɾ�����������ļ�
int DT_Table_Delete(TCHAR* db_name, TCHAR* rid, TCHAR* file_name, BTree* T, BTree_node* node);
//��ѯ���������ļ�
int DT_Data_Select(char* db_url, BTree* T, BTree_node* node); 
//����ת��
int DT_AutoCode(char RID[]); 


int DT_Login(char* username, char* password)
{
	int i = 0, flag = 0;
	char ch = 0;
	TCHAR true_password[] = TEXT("111"), true_username[] = TEXT("111");

	if (!_tcscmp(username, true_username))  //CASE_1���û���������ȷ
	{
		if (!_tcscmp(password, true_password))  //CASE_1_1������������ȷ
			return 1;
		else  //CASE_1_2�������������
			return -1;
	}
	else  //CASE_2���û����������
		return -2;
}


int DT_DatabaseFile_Read(TCHAR* db_name, BTree* T)
{
	char db_url[18], SID[10], haven[16];

	errno_t err;
	FILE* fp;

	_tcscpy_s(db_url, _tcslen(db_name) + 1, db_name);
	strcat_s(db_url, strlen(db_url) + strlen("�����.txt") + 1, "�����.txt");

	BTree_CreateTree(&T, 3);

	if ((err = fopen_s(&fp, db_url, "r")) == 0)
	{
		fseek(fp, 0L, SEEK_SET);
		while (fgets(haven, 16, fp) != NULL)  //��ȡ���ݿ��ļ��е��ļ��������ڴ�������B��
		{
			_tcscpy_s(SID, _tcslen(haven) + 1, haven);  //����β׺
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
	strcat_s(setting, strlen(setting) + strlen("��������ע���.txt") + 1, "��������ע���.txt");

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
	strcat_s(db_url, strlen(db_url) + strlen("�����.txt") + 1, "�����.txt");

	err = fopen_s(&fp3, "db_temp.txt", "w");  //�������ݿ��ļ�����
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
	strcat_s(db_url, strlen(db_url) + strlen("�����") + 1, "�����");

	strcpy_s(setting, strlen(db_url) + 1, db_url);
	strcat_s(setting, strlen(setting) + strlen("����ע���.txt") + 1, "����ע���.txt");

	if ((err = fopen_s(&fp, setting, "r")) != 0)						//	���ע����ļ��Ƿ���ڣ���ֹ��ʧ����
	{
		err1 = fopen_s(&fp5, setting, "w");
		fclose(fp5);
	}
	else
		return -1;
	
	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	if ((err = fopen_s(&fp, db_url, "r")) != 0)							//	�����ļ��Ƿ����
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
	strcat_s(db_url, strlen(db_url) + strlen("�����") + 1, "�����");

	strcpy_s(setting, strlen(db_url) + 1, db_url);
	strcat_s(setting, strlen(setting) + strlen("����ע���.txt") + 1, "����ע���.txt");
	remove(setting);

	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	err = fopen_s(&fp, db_url, "r");  //����صĿ���б���ɾ��
	while (fscanf_s(fp, "%s", haven, 18) != EOF) remove(haven);  //ʹ����fscanf�������ڷ�ֹ�������\n��ɴ���
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
	strcat_s(db_url, strlen(db_url) + strlen("�����") + 1, "�����");
	strcat_s(db_url, strlen(db_url) + strlen(".txt") + 1, ".txt");

	if ((err = fopen_s(&fp, db_url, "r")) == 0)  //�ж����ݿ��Ƿ����
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
	strcat_s(setting, strlen(setting) + strlen("��������ע���.txt") + 1, "��������ע���.txt");

	strcpy_s(file_url, strlen(db_url) + 1, db_url);
	strcat_s(file_url, strlen(file_url) + strlen("-") + 1, "-");
	strcat_s(file_url, strlen(file_url) + strlen(Rid) + 1, Rid);
	strcat_s(file_url, strlen(file_url) + strlen(file_name) + 1, file_name);
	strcat_s(file_url, strlen(file_url) + strlen(".txt") + 1, ".txt");

	//���ļ�ȫ��д��ע����ļ�
	err = fopen_s(&fp5, setting, "a+");
	fprintf(fp5, "%s\n", file_url);
	fclose(fp5);

	//������������д����������ļ�
	if (BTree_Search(&T, node, key, 0) == -1)
	{
		BTree_Insert(T, key);
		err1 = fopen_s(&fp2, file_url, "w");  //��ֻд��ʽ�򿪣����ļ������������ļ�
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
	strcat_s(setting, strlen(setting) + strlen("��������ע���.txt") + 1, "��������ע���.txt");

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
		fclose(fp6);  //�ȹر��ļ������޸�����
		rename("_temp.txt", setting);

		if (!(remove(file_url)))  //��ɾ���ɹ��򷵻�0���˴���û�н��в���
			return 1;
		else  //ɾ��ʧ��
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
	printf("���Ҵ�������\n");
	printf("-----------------------------------\n");
	printf("��׼��ʽ��202001-01\n");
	printf("�밴�ո�ʽ������������ļ��ı�ţ�");

	scanf_s("%s", research_id, 10);
	strcpy_s(Rid, strlen("TEST") + 1, "TEST");
	strcat_s(Rid, strlen(Rid) + strlen(research_id) + 1, research_id);
	int key = DT_AutoCode(research_id);

	printf("��������������ļ��ı��⣨10�����ڣ���");
	scanf_s("%s", file_name, 30);
	strtok_s(db_url, "�����", &p);
	strcpy_s(file_url, strlen(db_url) + 1, db_url);
	strcat_s(file_url, strlen(file_url) + strlen("-") + 1, "-");
	strcat_s(file_url, strlen(file_url) + strlen(Rid) + 1, Rid);
	strcat_s(file_url, strlen(file_url) + strlen(file_name) + 1, file_name);
	strcat_s(file_url, strlen(file_url) + strlen(".txt") + 1, ".txt");

	if (BTree_Search(&T, node, key, 0) != -1)
	{
		printf("-----------------------------------\n");
		printf("���������ļ����ڣ�\n");
		printf("B����ѯ��� SID ��%d\n", key);
		printf("���������ļ��� RID ��%s\n", file_url);
		printf("-----------------------------------\n");
		if ((err = fopen_s(&fp2, file_url, "r") == 0))
		{
			while (fgets(code, 1000, fp2) != NULL)
			{
				printf("%s", code);
			}
			printf("��ѯ����ɹ���\n");
			//fclose(fp2);
			printf("-----------------------------------\n");

			printf("�Ƿ���Ҫ���ƴ�������?\n1.��Ҫ����\n2.����Ҫ\n");
			printf("-----------------------------------\n");
			printf("����������ѡ��");
			scanf_s("%d", &flag1);
			if (flag1 == 1) StringAutoCopy(fp2);

			fclose(fp2);
			printf("�������������......");
			_getch();
			return 0;
		}
		else
		{
			system("cls");
			printf("-----------------------------------\n");
			printf("��ѯ���������ļ�����\n");
			printf("-----------------------------------\n");
			printf("��������ѡ��\n");
			printf("1.���²�ѯ\n2.������ѯ\n3.�˳�����\n");
			printf("-----------------------------------\n");
			printf("����������ѡ��");
			scanf_s("%d", &flag);
			if (flag == 1) return 1;
			if (flag == 2) return 0;
			if (flag == 3) exit(0);
			else
			{
				printf("û�и�ѡ��\n������ѯ");
				return 0;
			}
		}
	}
	system("cls");
	printf("-----------------------------------\n");
	printf("�ô��������ļ��������ڸ����ݿ⣡\n");
	printf("-----------------------------------\n");
	printf("��������ѡ��\n");
	printf("1.��ѯ����\n2.ȡ����ѯ\n3.�˳�����\n");
	printf("-----------------------------------\n");
	printf("����������ѡ��");
	scanf_s("%d", &flag);
	if (flag == 1) return 1;
	if (flag == 2) return 0;
	if (flag == 3) exit(0);
	else
	{
		printf("-----------------------------------\n");
		printf("û�и�ѡ��\n������ѯ");
		Sleep(1000);
		return 0;
	}
}


int DT_AutoCode(char RID[])  //----�Զ�����----//  �û������ѯ��ţ��Զ�����ѯ��� RID ת��Ϊ������� SID
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
