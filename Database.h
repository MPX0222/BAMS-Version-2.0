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
FILE* fp5; //ע����ļ�
FILE* fp6; //ע����޸���ʱ�ļ�

//��¼
int DT_Login(char* username, char* password);
//�������ݿ����
int DT_Database_Func();
//�������ݱ����
int DT_Table_Func(char* db_name);
//�������ݿ�
int DT_Database_Create(TCHAR* db_name);
//ɾ�����ݿ�
int DT_Database_Delete(TCHAR* db_name);
//�������ݿ�
int DT_Database_Open(TCHAR* db_name);
//�������������ļ�
int DT_Table_Create(char* db_url, BTree* T, BTree_node* node, char* setting);
//ɾ�����������ļ�
int DT_Table_Delete(char* db_url, BTree* T, BTree_node* node, char* seeting);
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


int DT_Table_Func(char* db_url)
{
	int flag = 0, func = 0, count = 0;
	char db_name[13] = { 0 }, SID[10], haven[16], file_haven[45]; char* p;
	char setting[45];
	errno_t err, err1;
	BTree T;

	BTree_CreateTree(&T, 3);
	err = fopen_s(&fp, db_url, "r");  //����Ҫ�ظ�д��仰����Ϊerr������ʱ�Ѿ�����ֵ�����д����仰������ʾ�ļ���ռ�ôӶ�ʹerr��ֵ�����ı䡾���޸���
	fseek(fp, 0L, SEEK_SET);

	while (fgets(haven, 16, fp) != NULL)  //��ȡ���ݿ��ļ��е��ļ��������ڴ�������B��
	{
		strcpy_s(SID, strlen(haven) + 1, haven);  //����β׺
		int key = atoi(SID);
		BTree_Insert(&T, key);
	}
	fclose(fp);

	printf("-----------------------------------\n");
	strcpy_s(db_name, strlen(strtok_s(db_url, ".txt", &p)) + 1, strtok_s(db_url, ".txt", &p));
	printf("%s\n", db_name);
	
	//����ע�������
	strcpy_s(setting, strlen(db_name) + 1, db_name);
	strcat_s(setting, strlen(setting) + strlen("����ע���.txt") + 1, "����ע���.txt");

	if ((err = fopen_s(&fp5, setting, "r")) == 0) {
		printf("-----------------------------------\n");
		printf("----< ���������ļ�ע��� >----\n");
		printf("-----------------------------------\n");
		while (fgets(file_haven, 45, fp5) != NULL) {
			printf("%s", file_haven);
			count++;
		}
		fclose(fp5);
	}
	printf("-----------------------------------\n");
	printf("�ÿ����ܴ��������ļ�����%d\n", count);

	printf("-----------------------------------\n");
	printf("1.�������������ļ�\n2.ɾ�����������ļ�\n3.���Ҵ��������ļ�\n4.��Ӵ��롾�ݲ�֧�֡�\n5.ɾ�����롾�ݲ�֧�֡�\n6.�޸Ĵ��롾�ݲ�֧�֡�\n7.�˳�����\n");
	printf("��������Ҫʹ�õĹ��ܣ�");
	scanf_s("%d", &func);

	switch (func)
	{
	case 1: while (DT_Table_Create(db_url, &T, T.root, setting)); break;  //CASE_1���������������ļ�
	case 2: while (DT_Table_Delete(db_url, &T, T.root, setting)); break;  //CASE_2��ɾ�����������ļ�
	case 3: while (DT_Data_Select(db_url, &T, T.root)); break;  //CASE_3����ѯ���������ļ�
	case 4: break;  //CASE_4��������ݡ��ݲ�֧�֡�
	case 5: break;  //CASE_5��ɾ�����ݡ��ݲ�֧�֡�
	case 6: //CASE_6���޸����ݡ��ݲ�֧�֡�
	case 7: exit(0); break;  //CASE_7���˳�����
	}

	system("cls");
	strcat_s(db_name, strlen(db_name) + strlen(".txt") + 1, ".txt");
	err1 = fopen_s(&fp4, "db_temp.txt", "w");  //�������ݿ��ļ�����
	BTree_Traverse(&T, T.root, 0);
	remove(db_name);
	fclose(fp4);
	rename("db_temp.txt", db_name);

	printf("-----------------------------------\n");
	printf("1.����ʹ��\n2.�˳�����\n");
	printf("-----------------------------------\n");
	printf("����������ѡ��");
	scanf_s("%d", &flag);
	if (flag == 1) return 0;  //CASE_1������ʹ��
	if (flag == 2) exit(0);  //CASE_3���˳�����
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

int DT_Table_Create(char* db_url, BTree* T, BTree_node* node, char* setting)  //----���ݱ���ģ�飨�ļ����棩----//  �û�������ʾ�������ݱ�
{
	system("cls");
	int flag = 0, n = 0, i = 0, line;
	char research_id[10] = { 0 };
	char Rid[14], file_name[30], file_url[45], code[200];
	char* p;
	errno_t err;

	
	printf("-----------------------------------\n");
	printf("�½����������ļ�\n");
	printf("-----------------------------------\n");
	printf("��׼��ʽ��202001-01\n");
	printf("�밴�ո�ʽ������������ļ��ı�ţ�");
	rewind(stdin);
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

	err = fopen_s(&fp5, setting, "a+");
	fprintf(fp5, "%s\n", file_url);
	fclose(fp5);

	if (BTree_Search(&T, node, key, 0) == -1)
	{
		BTree_Insert(T, key);
		err = fopen_s(&fp2, file_url, "w");  //��ֻд��ʽ�򿪣����ļ������������ļ�
		printf("-----------------------------------\n");
		printf("���������ݣ��ɸ��ƣ�:\n");
		printf("----------------------------------\n");
		GetPlateString(fp2);
		fclose(fp2);
		fflush(stdin);
		printf("�����ɹ�\n");
		printf("-----------------------------------\n");
		printf("�������������......");
		_getch();
		return 0;
	}
	else
	{
		system("cls");
		printf("-----------------------------------\n");
		printf("�����ݱ������Ѿ����ڣ�����������\n");
		printf("-----------------------------------\n");
		printf("��������ѡ��:\n");
		printf("1.���´������ݱ�\n2.ȡ���������ݱ�\n3.�˳�����\n");
		printf("-----------------------------------\n");
		printf("����������ѡ��");
		scanf_s("%d", &flag);
		if (flag == 1) return 1;  //CASE_1�����´���
		if (flag == 2) return 0;  //CASE_2��ȡ������
		if (flag == 3) exit(0);  //CASE_3���˳�����
		else
		{
			printf("-----------------------------------\n");
			printf("ѡ����ڣ�\nȡ���������ݱ�");
			Sleep(2000);
			return 0;
		}
	}
}

int DT_Table_Delete(char* db_url, BTree* T, BTree_node* node, char* setting)  //----���ݱ�ɾ��ģ��----//
{
	system("cls");
	int flag;
	errno_t err, err1;
	char research_id[10] = { 0 };
	char Rid[14], file_name[30], file_url[45], haven[45], code[200];
	char* p;

	printf("-----------------------------------\n");
	printf("ɾ�����������ļ�\n");
	printf("-----------------------------------\n");
	printf("��׼��ʽ��202001-01\n");
	printf("�밴�ո�ʽ������������ļ��ı�ţ�");
	rewind(stdin);
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

	BTree_Print(&T, T->root, 0);
	if (BTree_Search(&T, node, key, 0) != -1)
	{
		printf("-----------------------------------\n");
		printf("�ô��������ļ����ڣ�\n");
		printf("B����ѯ��� SID ��%d\n", key);
		printf("���������ļ��� RID ��%s\n", file_url);
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
		fclose(fp6);  //�ȹر��ļ������޸�����
		rename("_temp.txt", setting);

		if (!(remove(file_url)))  //��ɾ���ɹ��򷵻�0���˴���û�н��в���
		{
			BTree_DeleteTree(T, key);
			printf("ɾ���ɹ���\n");
			printf("-----------------------------------\n");
			printf("�������������......");
			_getch();
			return 0;
		}
		else  //ɾ��ʧ��
		{
			system("cls");
			printf("-----------------------------------\n");
			printf("ɾ�����������ļ�����\n");
			printf("-----------------------------------\n");
			printf("��������ѡ��\n");
			printf("1.����ɾ��\n2.����ɾ��\n3.�˳�����\n");
			printf("-----------------------------------\n");
			printf("����������ѡ��");
			scanf_s("%d", &flag);
			if (flag == 1) return 1;
			if (flag == 2) return 0;
			if (flag == 3) exit(0);
			else
			{
				printf("-----------------------------------\n");
				printf("û�и�ѡ��\n����ɾ��");
				//Sleep(1000);
				return 0;
			}
		}
	}
	  //CASE_2:�����ݱ������ݿ���
	system("cls");
	printf("-----------------------------------\n");
	printf("�ô��������ļ��������ڿ⣡\n");
	printf("-----------------------------------\n");
	printf("��������ѡ��\n");
	printf("1.ɾ������\n2.ȡ��ɾ��\n3.�˳�����\n");
	printf("-----------------------------------\n");
	printf("����������ѡ��");
	scanf_s("%d", &flag);
	if (flag == 1) return 1;
	if (flag == 2) return 0;
	if (flag == 3) exit(0);
	else
	{
		printf("-----------------------------------\n");
		printf("û�и�ѡ��\nȡ��ɾ��");
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
