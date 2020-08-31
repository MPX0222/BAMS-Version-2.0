/*
 * Program Name:    BAMS Version 2.0
 * Language:        C
 * Author:          MPX
 * Date:            2020.8.13 - 2020.8.28
 */

#include "Database.h"
#include "Btree.h"
#include "GetPlate.h"
#include<windows.h>

#define LoginTime 3
#define MaxLength 100

static int INPUTWINDOW_WIDTH = 300;        //  ��׼���봰�ڿ��
static int INPUTWINDOW_LENTH = 500;        //  ��׼���봰�ڳ���
static int MENUWINDOW_WIDTH = 480;         //  ��׼�˵����ڿ��
static int MENUWINDOW_LENTH = 800;        //  ��׼�˵����ڳ���
static int SHOWWINDOW_WIDTH = 1200;        //  ��׼��ʾ���ڿ��
static int SHOWWINDOW_LENTH = 1500;        //  ��׼��ʾ���ڳ���

int FLAG = 0;

TCHAR DB_NAME[10];                              //  ������һ�������Ĳ���
BTree T;

FILE* fp1;      //���ļ�
FILE* fp2;      //����ע����ļ�
FILE* fp3;      //����ע����滻�ļ�

LRESULT CALLBACK Login_UI(HWND, UINT, WPARAM, LPARAM);				/*1.��¼����*/
LRESULT CALLBACK Database_Menu_UI(HWND, UINT, WPARAM, LPARAM);		/*2.���ݿ�������洰��*/
LRESULT CALLBACK Database_InputIDC_UI(HWND, UINT, WPARAM, LPARAM);	/*3.1���ݿⴴ����Ϣ���봰��1*/
LRESULT CALLBACK Database_InputIDD_UI(HWND, UINT, WPARAM, LPARAM);	/*3.2���ݿ�ɾ����Ϣ���봰��2*/
LRESULT CALLBACK Database_InputIDO_UI(HWND, UINT, WPARAM, LPARAM);	/*3.3���ݿ������Ϣ���봰��3*/
LRESULT CALLBACK Table_Menu_UI(HWND, UINT, WPARAM, LPARAM);			/*4.���ݱ�������洰��*/
LRESULT CALLBACK Table_InputIDC_UI(HWND, UINT, WPARAM, LPARAM);		/*5.1���ݱ�����Ϣ���봰��*/
LRESULT CALLBACK Table_InputIDD_UI(HWND, UINT, WPARAM, LPARAM);		/*5.2���ݱ�ɾ����Ϣ���봰��*/
LRESULT CALLBACK Table_InputIDO_UI(HWND, UINT, WPARAM, LPARAM);		/*5.3���ݱ������Ϣ���봰��*/
LRESULT CALLBACK CodeData_Show_UI(HWND, UINT, WPARAM, LPARAM);		/*6.���ݲ�ѯ��ʾ����*/

static TCHAR LOGIN_CLASS[] = TEXT("LOGIN");                             /*�����ࣺ��¼*/
static TCHAR DATABASE_MENU_CLASS[] = TEXT("DATABASE_MENU");             /*�����ࣺ���ݿ��������*/
static TCHAR DATABASE_INPUTID_CLASS1[] = TEXT("DATABASE_INPUTIDC");       /*�����ࣺ���ݿⴴ����Ϣ����*/
static TCHAR DATABASE_INPUTID_CLASS2[] = TEXT("DATABASE_INPUTIDD");       /*�����ࣺ���ݿ�ɾ����Ϣ����*/
static TCHAR DATABASE_INPUTID_CLASS3[] = TEXT("DATABASE_INPUTIDO");       /*�����ࣺ���ݿ������Ϣ����*/
static TCHAR TABLE_MENU_CLASS[] = TEXT("TABLE_MENU");                   /*�����ࣺ���ݱ��������*/
static TCHAR TABLE_INPUTID_CLASS1[] = TEXT("TABLE_INPUTIDC");             /*�����ࣺ���ݱ�����Ϣ����*/
static TCHAR TABLE_INPUTID_CLASS2[] = TEXT("TABLE_INPUTIDD");             /*�����ࣺ���ݱ�ɾ����Ϣ����*/
static TCHAR TABLE_INPUTID_CLASS3[] = TEXT("TABLE_INPUTIDO");             /*�����ࣺ���ݱ������Ϣ����*/
static TCHAR CODEDATA_SHOW_CLASS[] = TEXT("CODEDATA_SHOW");             /*�����ࣺ���ݲ�ѯ��ʾ*/

static TCHAR LOGIN_TITLE[] = TEXT("BAMS Version 2.0 ��¼");
static TCHAR DATABASE_MENU_TITLE[] = TEXT("BAMS Version 2.0 ���ݿ�˵�");
static TCHAR DATABASE_INPUTID_TITLE[] = TEXT("BAMS Version 2.0 ���ݿ���Ϣ����");
static TCHAR TABLE_MENU_TITLE[] = TEXT("BAMS Version 2.0 ���ݱ�˵�");
static TCHAR TABLE_INPUTID_TITLE[] = TEXT("BAMS Version 2.0 ���ݱ���Ϣ����");
static TCHAR CODEDATA_SHOW_TITLE[] = TEXT("BAMS Version 2.0 ������ʾ ");

HWND login, db_menu, db_input_C, db_input_D, db_input_O, tb_menu, tb_input_C, tb_input_D, tb_input_O, code_show;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, PSTR szCmdLine, int iCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;               //  ������ʽ��
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //  ������С��ͼ�꣺ȱʡͼ��
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);         //  ���ڹ�꣺��ͷ���
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);    //  ���ڱ�����ɫ����׼������ɫ
    wndclass.hInstance = hInstance;                         //  ����ʵ�������
    wndclass.lpszMenuName = NULL;                           //  ���ڲ˵�����
    wndclass.cbClsExtra = 0;                                //  ��������չ����
    wndclass.cbWndExtra = 0;                                //  ����ʵ����չ����



    wndclass.lpszClassName = LOGIN_CLASS;       //  ���� ��¼ ���洰�ڶ���
    wndclass.lpfnWndProc = Login_UI;            //  ���� ��¼ ���洰�ڻص�����

    if (!RegisterClass(&wndclass)) 
    {
        MessageBox(NULL, TEXT("��¼����ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_MENU_CLASS;       //  ���� ���ݿ�˵� ���洰�ڶ���
    wndclass.lpfnWndProc = Database_Menu_UI;            //  ���� ���ݿ�˵� ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݿ�˵�����ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS1;       //  ���� ���ݿ���Ϣ����1 ���洰�ڶ���
    wndclass.lpfnWndProc = Database_InputIDC_UI;            //  ���� ���ݿ���Ϣ����1 ���洰�ڻص�����

    if (!RegisterClass(&wndclass)) 
    {
        MessageBox(NULL, TEXT("���ݿ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS2;       //  ���� ���ݿ���Ϣ����2 ���洰�ڶ���
    wndclass.lpfnWndProc = Database_InputIDD_UI;            //  ���� ���ݿ���Ϣ����2 ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݿ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS3;       //  ���� ���ݿ���Ϣ����3 ���洰�ڶ���
    wndclass.lpfnWndProc = Database_InputIDO_UI;            //  ���� ���ݿ���Ϣ����3 ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݿ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = TABLE_MENU_CLASS;       //  ���� ���ݱ�˵� ���洰�ڶ���
    wndclass.lpfnWndProc = Table_Menu_UI;            //  ���� ���ݱ�˵� ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݱ�˵�����ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = TABLE_INPUTID_CLASS1;       //  ���� ���ݱ���Ϣ���� ���洰�ڶ���
    wndclass.lpfnWndProc = Table_InputIDC_UI;            //  ���� ���ݱ���Ϣ���� ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݱ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = TABLE_INPUTID_CLASS2;       //  ���� ���ݱ���Ϣ���� ���洰�ڶ���
    wndclass.lpfnWndProc = Table_InputIDD_UI;            //  ���� ���ݱ���Ϣ���� ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݱ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = TABLE_INPUTID_CLASS3;       //  ���� ���ݱ���Ϣ���� ���洰�ڶ���
    wndclass.lpfnWndProc = Table_InputIDO_UI;            //  ���� ���ݱ���Ϣ���� ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���ݱ���Ϣ�������ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }
/*
    wndclass.lpszClassName = CODEDATA_SHOW_CLASS;       //  ���� ������ʾ ���洰�ڶ���
    wndclass.lpfnWndProc = CodeData_Show_UI;            //  ���� ������ʾ ���洰�ڻص�����

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("������ʾ����ע��ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
        return 0;
    }
*/


    login = CreateWindow(
        LOGIN_CLASS,
        LOGIN_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 260��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(login, iCmdShow);
    UpdateWindow(login);

    db_menu = CreateWindow(
        DATABASE_MENU_CLASS,
        DATABASE_MENU_TITLE,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        350, 190,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        MENUWINDOW_LENTH, MENUWINDOW_WIDTH,               //  ���ڳ�ʼ��ʾ��С�� 1200 �� 1200
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_menu, SW_HIDE);
    UpdateWindow(db_menu);

    db_input_C = CreateWindow(
        DATABASE_INPUTID_CLASS1,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_C, SW_HIDE);
    UpdateWindow(db_input_C);

    db_input_D = CreateWindow(
        DATABASE_INPUTID_CLASS2,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_D, SW_HIDE);
    UpdateWindow(db_input_D);

    db_input_O = CreateWindow(
        DATABASE_INPUTID_CLASS3,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_O, SW_HIDE);
    UpdateWindow(db_input_O);

    tb_menu = CreateWindow(
        TABLE_MENU_CLASS,
        TABLE_MENU_TITLE,
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        350, 190,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        MENUWINDOW_LENTH, MENUWINDOW_WIDTH,               //  ���ڳ�ʼ��ʾ��С�� 1200 �� 1200
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_menu, SW_HIDE);
    UpdateWindow(tb_menu);

    tb_input_C = CreateWindow(
        TABLE_INPUTID_CLASS1,
        TABLE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_input_C, SW_HIDE);
    UpdateWindow(tb_input_C);

    tb_input_D = CreateWindow(
        TABLE_INPUTID_CLASS2,
        TABLE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_input_D, SW_HIDE);
    UpdateWindow(tb_input_D);

    tb_input_O = CreateWindow(
        TABLE_INPUTID_CLASS3,
        TABLE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  ���ڳ�ʼ��ʾ���꣺��500 , 500��
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  ���ڳ�ʼ��ʾ��С�� 500 �� 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_input_O, SW_HIDE);
    UpdateWindow(tb_input_O);
/*
    code_show = CreateWindow(
        CODEDATA_SHOW_CLASS,
        CODEDATA_SHOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        500, 500,                                       //���ڳ�ʼ��ʾ���꣺��500 , 500��
        SHOWWINDOW_LENTH, SHOWWINDOW_WIDTH,               //���ڳ�ʼ��ʾ��С�� 1500 �� 1500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(code_show, SW_HIDE);
    UpdateWindow(code_show);
*/
    // ����Ϣѭ��

    while (GetMessage(&msg, NULL, 0, 0))        //����Ϣ�����л�ȡ��Ϣ
    {
        TranslateMessage(&msg);                 //���������Ϣת��Ϊ�ַ���Ϣ
        DispatchMessage(&msg);                  //�ַ�������һ�������ڵĻص�����
    }

    return msg.wParam;
}

LRESULT CALLBACK Login_UI(HWND login, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 20, 80, 40, 60 }, rect2 = { 20, 170, 40, 60 }, rect3 = { 100, 250, 70, 20 }, rect4 = { 100, 180, 80, 40 };
    TCHAR USERNAME[MaxLength], PASSWORD[MaxLength], PROMPT[MaxLength];

    static int flag = LoginTime;
    int wmid, wmevent;
    static HWND LOGIN, INPUT1, INPUT2, TEXT1, TEXT2;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--�û���
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�û�����"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/| SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            login /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );
        //������̬�ı���ؼ�--����
        TEXT2 = CreateWindow(TEXT("static"), TEXT("���룺"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/| SS_RIGHT /*ˮƽ����*/,
            10, 140, 70, 26,
            login, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            login, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //�������������
        INPUT2 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD /*����*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 140, 200, 26,
            login, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�
        LOGIN = CreateWindow(TEXT("button"), TEXT("��¼"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 110, 100, 30,
            login, (HMENU)5, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(LOGIN, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(login, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(login, &rect);

        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������

        if (flag == 3)      //  ���Դ�����δ����
        {
            TextOut(hdc, 100, 180, TEXT("���������������"), 8);
        }
        if (flag == 2)      //  ���Դ�����δ����
        {
            TextOut(hdc, 100, 180, TEXT("�˺������벻ƥ�䣬�����ԣ�\n��ʣ�������������"), 23);
        }
        if (flag == 1)      //  ���Դ�����δ����
        {
            TextOut(hdc, 100, 180, TEXT("�˺������벻ƥ�䣬�����ԣ�\n��ʣ��һ���������"), 23);
        }
        if (flag == 0)      //  ���Դ���������
        {
            TextOut(hdc, 100, 180, TEXT("����������������꣡"), 10);
        }

        EndPaint(login, &ps);
        return 0;

    case WM_COMMAND:

        wmid = LOWORD(wParam);
        wmevent = HIWORD(wParam);

        if ((HWND)lParam == LOGIN) {

            GetWindowText(INPUT1, USERNAME, MaxLength);
            GetWindowText(INPUT2, PASSWORD, MaxLength);
            wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n�����û��˺ţ�%s\r\n�����û����룺%s"), USERNAME, PASSWORD);
            MessageBox(login, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);

            if (DT_Login(USERNAME, PASSWORD) > 0)
            {
                ShowWindow(login, SW_HIDE);
                ShowWindow(db_menu, SW_SHOW);
                UpdateWindow(db_menu);
            }
            if (DT_Login(USERNAME, PASSWORD) < 0)
            {
                flag = flag - 1;
                InvalidateRect(login, NULL, TRUE);
                UpdateWindow(login);
            }
            //break;

        }
        //break;
        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(login, message, wParam, lParam);
}

LRESULT CALLBACK Database_Menu_UI(HWND db_menu, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect = {30, 30, 300, 390}, rect1;

    static HWND CREATE_DB, DELETE_DB, OPEN_DB, EXIT, TEXT1;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������ť�ؼ�--�������ݿ�
        CREATE_DB = CreateWindow(TEXT("BUTTON"), TEXT("�����������ݿ� Create Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430 /*x����*/, 80 /*y����*/, 300 /*���*/, 45 /*�߶�*/,
            db_menu /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );
        //������ť�ؼ�--ɾ�����ݿ�
        DELETE_DB = CreateWindow(TEXT("BUTTON"), TEXT("ɾ���������ݿ� Delete Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 165, 300, 45,
            db_menu, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //������ť�ؼ�--�������ݿ�
        OPEN_DB = CreateWindow(TEXT("BUTTON"), TEXT("���ô������ݿ� Open Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 250, 300, 45,
            db_menu, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //������ť�ؼ�--�˳�����
        EXIT = CreateWindow(TEXT("BUTTON"), TEXT("�˳����� Exit"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 335, 300, 45,
            db_menu, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(CREATE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(DELETE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(OPEN_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(EXIT, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_menu, &ps);
        SelectObject(hdc, hFont);           //  ������ʾ����
        GetClientRect(db_menu, &rect1);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  ���ݿ�"), -1, &rect1, DT_LEFT | DT_CENTER);        //  ���ƽ������
        DrawText(hdc,
            TEXT("\n\n\nC�����\n\nJava�����\n\nPython����⣨δ������\n\nC++����⣨δ������\n\nMATLAB����⣨δ������\n\nSQL����⣨δ������"),
            -1, &rect, DT_LEFT);
        
        EndPaint(db_menu, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CREATE_DB)
        {
            ShowWindow(db_input_C, SW_SHOW);
            UpdateWindow(db_input_C);
        }
        if ((HWND)lParam == DELETE_DB)
        {
            ShowWindow(db_input_D, SW_SHOW);
            UpdateWindow(db_input_D);
        }
        if ((HWND)lParam == OPEN_DB)
        {
            ShowWindow(db_input_O, SW_SHOW);
            UpdateWindow(db_input_O);
        }
        if ((HWND)lParam == EXIT)
        {
            DestroyWindow(db_menu);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(db_menu, message, wParam, lParam);
}

LRESULT CALLBACK Database_InputIDC_UI(HWND db_input_C, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 100, 140, 100, 26 };
    TCHAR DBNAME[10], PROMPT[100];

    int flag = 0;
    static HWND CONF, INPUT1, TEXT1, TEXT2, BACK;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--����
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�� ����"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ | SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            db_input_C /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("�����������ƣ�10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            100 /*x����*/, 140 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            db_input_C /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            db_input_C, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--ȷ��
        CONF = CreateWindow(TEXT("button"), TEXT("ȷ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 80, 100, 30,
            db_input_C, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--������һ��
        BACK = CreateWindow(TEXT("button"), TEXT("������һ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 140, 100, 30,
            db_input_C, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_C, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(db_input_C, &rect);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  ���ݿⴴ��\n\n[���������ݿ�����]"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������
        
        EndPaint(db_input_C, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DBNAME, 10);
            if (DT_Database_Create(DBNAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s �����ɹ���"), DBNAME);
                MessageBox(db_input_C, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(db_input_C, SW_HIDE);
                UpdateWindow(db_input_C);
            }
            else if (DT_Database_Create(DBNAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s �Ѿ����ڣ�"), DBNAME);
                MessageBox(db_input_C, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(db_input_C, SW_HIDE);
                UpdateWindow(db_input_C);
            }
        }

        if ((HWND)lParam == BACK)
        {
            ShowWindow(db_input_C, SW_HIDE);
            UpdateWindow(db_input_C);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(db_input_C, message, wParam, lParam);
}

LRESULT CALLBACK Database_InputIDD_UI(HWND db_input_D, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 100, 140, 100, 26 };
    TCHAR DBNAME[10], PROMPT[100];

    int flag = 0;
    static HWND CONF, INPUT1, TEXT1, TEXT2, BACK;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--����
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�� ����"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ | SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            db_input_D /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("�����������ƣ�10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            100 /*x����*/, 140 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            db_input_D /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            db_input_D, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--ȷ��
        CONF = CreateWindow(TEXT("button"), TEXT("ȷ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 80, 100, 30,
            db_input_D, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--������һ��
        BACK = CreateWindow(TEXT("button"), TEXT("������һ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 140, 100, 30,
            db_input_D, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_D, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(db_input_D, &rect);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  ���ݿ�ɾ��\n\n[���������ݿ�����]"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������

        EndPaint(db_input_D, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DBNAME, 10);
            if (DT_Database_Delete(DBNAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s ɾ���ɹ���"), DBNAME);
                MessageBox(db_input_D, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(db_input_D, SW_HIDE);
                UpdateWindow(db_input_D);
            }
            else if (DT_Database_Delete(DBNAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s �����ڣ�"), DBNAME);
                MessageBox(db_input_D, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(db_input_D, SW_HIDE);
                UpdateWindow(db_input_D);
            }
        }

        if ((HWND)lParam == BACK)
        {
            ShowWindow(db_input_D, SW_HIDE);
            UpdateWindow(db_input_D);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(db_input_D, message, wParam, lParam);
}

LRESULT CALLBACK Database_InputIDO_UI(HWND db_input_O, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 100, 140, 100, 26 };
    TCHAR DBNAME[10], PROMPT[100];

    int flag = 0;
    static HWND CONF, INPUT1, TEXT1, TEXT2, BACK;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--����
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�� ����"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ | SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            db_input_O /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("�����������ƣ�10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            100 /*x����*/, 140 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            db_input_O /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            db_input_O, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--ȷ��
        CONF = CreateWindow(TEXT("button"), TEXT("ȷ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 80, 100, 30,
            db_input_O, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--������һ��
        BACK = CreateWindow(TEXT("button"), TEXT("������һ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 140, 100, 30,
            db_input_O, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_O, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(db_input_O, &rect);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  ���ݿ����\n\n[���������ݿ�����]"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������

        EndPaint(db_input_O, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DB_NAME, 10);
            if (DT_Database_Open(DB_NAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s ���óɹ���"), DB_NAME);
                MessageBox(db_input_O, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);

                //  �رմ�ǰ�Ľ���
                ShowWindow(db_menu, SW_HIDE);
                UpdateWindow(db_menu);
                ShowWindow(db_input_O, SW_HIDE);
                UpdateWindow(db_input_O);

                ShowWindow(tb_menu, SW_SHOW);                   //  �����ݱ�˵�����
                UpdateWindow(tb_menu);
            }
            if (DT_Database_Open(DB_NAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n���ݿ⣺%s ����򲻴��ڣ�"), DB_NAME);
                MessageBox(db_input_O, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);

                ShowWindow(db_input_O, SW_HIDE);
                UpdateWindow(db_input_O);
            }
        }

        if ((HWND)lParam == BACK)
        {
            ShowWindow(db_input_O, SW_HIDE);
            UpdateWindow(db_input_O);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(db_input_O, message, wParam, lParam);
    
}

LRESULT CALLBACK Table_Menu_UI(HWND tb_menu, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect = { 30, 30, 100, 190 }, rect1;
    char file_name[10000];

    static HWND CREATE_TB, DELETE_TB, OPEN_TB, EXIT, SHOW, TEXT1;
    static HFONT hFont;                                             //  ���壺�߼�����
    int count = 0, flag = 0;

    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������ť�ؼ�--�������ݱ�
        CREATE_TB = CreateWindow(TEXT("BUTTON"), TEXT("�������������ļ� Create CodeFile"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430 /*x����*/, 80 /*y����*/, 300 /*���*/, 45 /*�߶�*/,
            tb_menu /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );
        //������ť�ؼ�--ɾ�����ݱ�
        DELETE_TB = CreateWindow(TEXT("BUTTON"), TEXT("ɾ�����������ļ� Delete CodeFile"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 165, 300, 45,
            tb_menu, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //������ť�ؼ�--�������ݱ�
        OPEN_TB = CreateWindow(TEXT("BUTTON"), TEXT("�鿴���������ļ� Select CodeFile"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 250, 300, 45,
            tb_menu, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //������ť�ؼ�--�˳�����
        EXIT = CreateWindow(TEXT("BUTTON"), TEXT("�˳����� Exit"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 335, 300, 45,
            tb_menu, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--�˳�����
        SHOW = CreateWindow(TEXT("BUTTON"), TEXT("��ʾ Show"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            430, 395, 100, 30,
            tb_menu, (HMENU)5, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        /*
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�˳����� Exit\nextit"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | WS_VSCROLL,
            30, 60, 350, 350,
            tb_menu, (HMENU)5, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        */


        //�������ÿؼ�������
        SendMessage(CREATE_TB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(DELETE_TB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(OPEN_TB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(EXIT, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(SHOW, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(tb_menu, &ps);
        SelectObject(hdc, hFont);           //  ������ʾ����
        GetClientRect(tb_menu, &rect1);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0"), -1, &rect1, DT_LEFT | DT_CENTER);        //  ���ƽ������

        if (flag == 1)
        {
            DrawText(hdc, file_name, -1, &rect, DT_LEFT);
        }

        EndPaint(tb_menu, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == SHOW)
        {
            //��ȡ���ļ�����B��
            DT_DatabaseFile_Read(DB_NAME, &T);
            //��ȡ���ݿ�ע����ļ���д�뵽������
            DT_SettingFile_Read(DB_NAME, file_name);

            flag = 1;
            InvalidateRect(tb_menu, &rect, TRUE);
            SendMessage(tb_menu, WM_PAINT, wParam, lParam);
            //UpdateWindow(tb_menu);

        }
       
        if ((HWND)lParam == CREATE_TB)
        {
            ShowWindow(tb_input_C, SW_SHOW);
            UpdateWindow(tb_input_C);
        }
        if ((HWND)lParam == DELETE_TB)
        {
            ShowWindow(tb_input_D, SW_SHOW);
            UpdateWindow(tb_input_D);
        }		

        if ((HWND)lParam == OPEN_TB)
        {
            ShowWindow(tb_input_O, SW_SHOW);
            UpdateWindow(tb_input_O);
        }

        if ((HWND)lParam == EXIT)
        {
            DestroyWindow(tb_menu);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(tb_menu, message, wParam, lParam);
}

LRESULT CALLBACK Table_InputIDC_UI(HWND tb_input_C, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 100, 140, 100, 26 };
    TCHAR RID[10], TITLE[10], TB_URL[24], PROMPT[100];

    int flag = 0;
    static HWND CONF, INPUT1, INPUT2, TEXT1, TEXT2, TEXT3, BACK;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--����
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�ļ���ţ�"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ | SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            tb_input_C /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("�ļ����⣺"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            10 /*x����*/, 140 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            tb_input_C /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT3 = CreateWindow(TEXT("static"), TEXT("�����������ƣ�10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            100 /*x����*/, 200 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            tb_input_C /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            tb_input_C, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        INPUT2 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 140, 200, 26,
            tb_input_C, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--ȷ��
        CONF = CreateWindow(TEXT("button"), TEXT("ȷ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 80, 100, 30,
            tb_input_C, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--������һ��
        BACK = CreateWindow(TEXT("button"), TEXT("������һ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 140, 100, 30,
            tb_input_C, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(tb_input_C, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(tb_input_C, &rect);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  �����ļ�����\n\n[�����������ļ���Ϣ]"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������

        EndPaint(tb_input_C, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, RID, 10);
            GetWindowText(INPUT2, TITLE, 10);

            if (DT_Table_Create(DB_NAME, RID, TITLE, &T, T.root) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n�����ɹ���"));
                MessageBox(tb_input_C, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(tb_input_C, SW_HIDE);
                UpdateWindow(tb_input_C);
            }
            else if (DT_Table_Create(DB_NAME, RID, TITLE, &T, T.root) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n�ļ��Ѿ����ڣ�"));
                MessageBox(tb_input_C, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(tb_input_C, SW_HIDE);
                UpdateWindow(tb_input_C);
            }
        }

        if ((HWND)lParam == BACK)
        {
            ShowWindow(tb_input_C, SW_HIDE);
            UpdateWindow(tb_input_C);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(tb_input_C, message, wParam, lParam);
}

LRESULT CALLBACK Table_InputIDD_UI(HWND tb_input_D, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect, rect1 = { 100, 140, 100, 26 };
    TCHAR RID[10], TITLE[10], TB_URL[24], PROMPT[100];

    int flag = 0;
    static HWND CONF, INPUT1, INPUT2, TEXT1, TEXT2, TEXT3, BACK;
    static HFONT hFont;                                             //  ���壺�߼�����


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        //������̬�ı���ؼ�--����
        TEXT1 = CreateWindow(TEXT("static"), TEXT("�ļ���ţ�"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ | SS_RIGHT /*ˮƽ����*/,
            10 /*x����*/, 80 /*y����*/, 70 /*���*/, 26 /*�߶�*/,
            tb_input_D /*�����ھ��*/, (HMENU)1 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("�ļ����⣺"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            10 /*x����*/, 140 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            tb_input_D /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        TEXT3 = CreateWindow(TEXT("static"), TEXT("�����������ƣ�10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*��ֱ����*/ /*ˮƽ����*/,
            100 /*x����*/, 200 /*y����*/, 120 /*���*/, 26 /*�߶�*/,
            tb_input_D /*�����ھ��*/, (HMENU)5 /*�ؼ�ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*��ǰ����ʵ�����*/, NULL
        );

        //���������ı���ؼ�
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 80, 200, 26,
            tb_input_D, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        INPUT2 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*�߿�*/ | ES_AUTOHSCROLL /*ˮƽ����*/,
            100, 140, 200, 26,
            tb_input_D, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--ȷ��
        CONF = CreateWindow(TEXT("button"), TEXT("ȷ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 80, 100, 30,
            tb_input_D, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //������ť�ؼ�--������һ��
        BACK = CreateWindow(TEXT("button"), TEXT("������һ��"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
            350, 140, 100, 30,
            tb_input_D, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //�������ÿؼ�������
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(tb_input_D, &ps);
        SelectObject(hdc, hFont);               //  ������ʾ����
        GetClientRect(tb_input_D, &rect);
        DrawText(hdc, TEXT("BAMS �������ϵͳ Version 2.0  �����ļ�����\n\n[�����������ļ���Ϣ]"), -1, &rect, DT_LEFT | DT_CENTER);        //  ���Ƶ�¼�������

        EndPaint(tb_input_D, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, RID, 10);
            GetWindowText(INPUT2, TITLE, 10);

            if (DT_Table_Delete(DB_NAME, RID, TITLE, &T, T.root) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\nɾ���ɹ���"));
                MessageBox(tb_input_D, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(tb_input_D, SW_HIDE);
                UpdateWindow(tb_input_D);
            }
            else if (DT_Table_Delete(DB_NAME, RID, TITLE, &T, T.root) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 ϵͳ��ʾ��\r\n�ļ������ڣ�"));
                MessageBox(tb_input_D, PROMPT, TEXT("��Ϣ��ʾ"), MB_ICONINFORMATION);
                ShowWindow(tb_input_D, SW_HIDE);
                UpdateWindow(tb_input_D);
            }
        }

        if ((HWND)lParam == BACK)
        {
            ShowWindow(tb_input_D, SW_HIDE);
            UpdateWindow(tb_input_D);
        }

        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(tb_input_D, message, wParam, lParam);
}

LRESULT CALLBACK Table_InputIDO_UI(HWND tb_input_O, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR DBNAME[10], PROMPT[100];

    static HWND SELECT[5 * 6];
    static HFONT hFont;                                             //  ���壺�߼�����

    errno_t err;


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*��*/, -7/*��*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
            FALSE/*б��?*/, FALSE/*�»���?*/, FALSE/*ɾ����?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("΢���ź�"));

        

    case WM_PAINT:

        hdc = BeginPaint(tb_input_O, &ps);

        EndPaint(tb_input_O, &ps);
        return 0;

    case WM_COMMAND:


        return 0;

    case WM_DESTROY:

        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(tb_input_O, message, wParam, lParam);

}

LRESULT CALLBACK CodeData_Show_UI(HWND code_show, UINT message, WPARAM wParam, LPARAM lParam)
{

}