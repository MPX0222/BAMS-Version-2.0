/*
 * Program Name:    BAMS Version 2.0
 * Language:        C
 * Author:          MPX
 * Date:            2020.8.13 - 2020.8.19
 */

#include "Database.h"
#include "Btree.h"
#include "GetPlate.h"
#include<windows.h>

#define LoginTime 3
#define MaxLength 100

static int INPUTWINDOW_WIDTH = 300;        //  标准输入窗口宽度
static int INPUTWINDOW_LENTH = 500;        //  标准输入窗口长度
static int MENUWINDOW_WIDTH = 480;         //  标准菜单窗口宽度
static int MENUWINDOW_LENTH = 800;        //  标准菜单窗口长度
static int SHOWWINDOW_WIDTH = 1200;        //  标准显示窗口宽度
static int SHOWWINDOW_LENTH = 1500;        //  标准显示窗口长度

int FLAG = 0;

LRESULT CALLBACK Login_UI(HWND, UINT, WPARAM, LPARAM);				/*1.登录窗口*/
LRESULT CALLBACK Database_Menu_UI(HWND, UINT, WPARAM, LPARAM);		/*2.数据库操作界面窗口*/
LRESULT CALLBACK Database_InputIDC_UI(HWND, UINT, WPARAM, LPARAM);	/*3.数据库信息输入窗口1*/
LRESULT CALLBACK Database_InputIDD_UI(HWND, UINT, WPARAM, LPARAM);	/*3.数据库信息输入窗口2*/
LRESULT CALLBACK Database_InputIDO_UI(HWND, UINT, WPARAM, LPARAM);	/*3.数据库信息输入窗口3*/
LRESULT CALLBACK Table_Menu_UI(HWND, UINT, WPARAM, LPARAM);			/*4.数据表操作界面窗口*/
LRESULT CALLBACK Table_InputID_UI(HWND, UINT, WPARAM, LPARAM);		/*5.数据表信息输入窗口*/
LRESULT CALLBACK CodeData_Show_UI(HWND, UINT, WPARAM, LPARAM);		/*6.数据查询显示窗口*/

static TCHAR LOGIN_CLASS[] = TEXT("LOGIN");                             /*窗口类：登录*/
static TCHAR DATABASE_MENU_CLASS[] = TEXT("DATABASE_MENU");             /*窗口类：数据库操作界面*/
static TCHAR DATABASE_INPUTID_CLASS1[] = TEXT("DATABASE_INPUTIDC");       /*窗口类：数据库信息输入*/
static TCHAR DATABASE_INPUTID_CLASS2[] = TEXT("DATABASE_INPUTIDD");       /*窗口类：数据库信息输入*/
static TCHAR DATABASE_INPUTID_CLASS3[] = TEXT("DATABASE_INPUTIDO");       /*窗口类：数据库信息输入*/
static TCHAR TABLE_MENU_CLASS[] = TEXT("TABLE_MENU");                   /*窗口类：数据表操作界面*/
static TCHAR TABLE_INPUTID_CLASS[] = TEXT("TABLE_INPUTID");             /*窗口类：数据表信息输入*/
static TCHAR CODEDATA_SHOW_CLASS[] = TEXT("CODEDATA_SHOW");             /*窗口类：数据查询显示*/

static TCHAR LOGIN_TITLE[] = TEXT("BAMS Version 2.0 登录");
static TCHAR DATABASE_MENU_TITLE[] = TEXT("BAMS Version 2.0 数据库菜单");
static TCHAR DATABASE_INPUTID_TITLE[] = TEXT("BAMS Version 2.0 数据库信息输入");
static TCHAR TABLE_MENU_TITLE[] = TEXT("BAMS Version 2.0 数据表菜单");
static TCHAR TABLE_INPUTID_TITLE[] = TEXT("BAMS Version 2.0 数据表信息输入");
static TCHAR CODEDATA_SHOW_TITLE[] = TEXT("BAMS Version 2.0 数据显示 ");

HWND login, db_menu, db_input_C, db_input_D, db_input_O, tb_menu, tb_input, code_show;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPervInstance, PSTR szCmdLine, int iCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;               //  窗口样式：
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //  窗口最小化图标：缺省图标
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);         //  窗口光标：箭头光标
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);    //  窗口背景颜色：标准窗口颜色
    wndclass.hInstance = hInstance;                         //  窗口实例句柄：
    wndclass.lpszMenuName = NULL;                           //  窗口菜单：无
    wndclass.cbClsExtra = 0;                                //  窗口类拓展：无
    wndclass.cbWndExtra = 0;                                //  窗口实例拓展：无



    wndclass.lpszClassName = LOGIN_CLASS;       //  创建 登录 界面窗口对象
    wndclass.lpfnWndProc = Login_UI;            //  设置 登录 界面窗口回调函数

    if (!RegisterClass(&wndclass)) 
    {
        MessageBox(NULL, TEXT("登录界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_MENU_CLASS;       //  创建 数据库菜单 界面窗口对象
    wndclass.lpfnWndProc = Database_Menu_UI;            //  设置 数据库菜单 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据库菜单界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS1;       //  创建 数据库信息输入1 界面窗口对象
    wndclass.lpfnWndProc = Database_InputIDC_UI;            //  设置 数据库信息输入1 界面窗口回调函数

    if (!RegisterClass(&wndclass)) 
    {
        MessageBox(NULL, TEXT("数据库信息输入界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS2;       //  创建 数据库信息输入2 界面窗口对象
    wndclass.lpfnWndProc = Database_InputIDD_UI;            //  设置 数据库信息输入2 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据库信息输入界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = DATABASE_INPUTID_CLASS3;       //  创建 数据库信息输入3 界面窗口对象
    wndclass.lpfnWndProc = Database_InputIDO_UI;            //  设置 数据库信息输入3 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据库信息输入界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }

    wndclass.lpszClassName = TABLE_MENU_CLASS;       //  创建 数据表菜单 界面窗口对象
    wndclass.lpfnWndProc = Table_Menu_UI;            //  设置 数据表菜单 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据表菜单界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }
/*
    wndclass.lpszClassName = TABLE_INPUTID_CLASS;       //  创建 数据表信息输入 界面窗口对象
    wndclass.lpfnWndProc = Table_InputID_UI;            //  设置 数据表信息输入 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据表信息输入界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }
/*
    wndclass.lpszClassName = CODEDATA_SHOW_CLASS;       //  创建 数据显示 界面窗口对象
    wndclass.lpfnWndProc = CodeData_Show_UI;            //  设置 数据显示 界面窗口回调函数

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("数据显示界面注册失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
        return 0;
    }
*/


    login = CreateWindow(
        LOGIN_CLASS,
        LOGIN_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  窗口初始显示坐标：（500 , 260）
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  窗口初始显示大小： 500 × 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(login, iCmdShow);
    UpdateWindow(login);

    db_menu = CreateWindow(
        DATABASE_MENU_CLASS,
        DATABASE_MENU_TITLE,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        350, 190,                                       //  窗口初始显示坐标：（500 , 500）
        MENUWINDOW_LENTH, MENUWINDOW_WIDTH,               //  窗口初始显示大小： 1200 × 1200
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_menu, SW_HIDE);
    UpdateWindow(db_menu);

    db_input_C = CreateWindow(
        DATABASE_INPUTID_CLASS1,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  窗口初始显示坐标：（500 , 500）
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  窗口初始显示大小： 500 × 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_C, SW_HIDE);
    UpdateWindow(db_input_C);

    db_input_D = CreateWindow(
        DATABASE_INPUTID_CLASS2,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  窗口初始显示坐标：（500 , 500）
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  窗口初始显示大小： 500 × 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_D, SW_HIDE);
    UpdateWindow(db_input_D);

    db_input_O = CreateWindow(
        DATABASE_INPUTID_CLASS3,
        DATABASE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        520, 260,                                       //  窗口初始显示坐标：（500 , 500）
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //  窗口初始显示大小： 500 × 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(db_input_O, SW_HIDE);
    UpdateWindow(db_input_O);

    tb_menu = CreateWindow(
        TABLE_MENU_CLASS,
        TABLE_MENU_TITLE,
        WS_OVERLAPPEDWINDOW,
        500, 500,                                       //  窗口初始显示坐标：（500 , 500）
        MENUWINDOW_LENTH, MENUWINDOW_WIDTH,               //  窗口初始显示大小： 1200 × 1200
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_menu, SW_HIDE);
    UpdateWindow(tb_menu);
/*
    tb_input = CreateWindow(
        TABLE_INPUTID_CLASS,
        TABLE_INPUTID_TITLE,
        WS_OVERLAPPEDWINDOW,
        500, 500,                                       //窗口初始显示坐标：（500 , 500）
        INPUTWINDOW_LENTH, INPUTWINDOW_WIDTH,             //窗口初始显示大小： 500 × 500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(tb_input, SW_HIDE);
    UpdateWindow(tb_input);
/*
    code_show = CreateWindow(
        CODEDATA_SHOW_CLASS,
        CODEDATA_SHOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        500, 500,                                       //窗口初始显示坐标：（500 , 500）
        SHOWWINDOW_LENTH, SHOWWINDOW_WIDTH,               //窗口初始显示大小： 1500 × 1500
        NULL, NULL,
        hInstance,
        NULL);
    ShowWindow(code_show, SW_HIDE);
    UpdateWindow(code_show);
*/
    // 主消息循环

    while (GetMessage(&msg, NULL, 0, 0))        //从消息队列中获取信息
    {
        TranslateMessage(&msg);                 //将虚拟键信息转换为字符信息
        DispatchMessage(&msg);                  //分发到（第一个）窗口的回调函数
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
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建静态文本框控件--用户名
        TEXT1 = CreateWindow(TEXT("static"), TEXT("用户名："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/| SS_RIGHT /*水平居右*/,
            10 /*x坐标*/, 80 /*y坐标*/, 70 /*宽度*/, 26 /*高度*/,
            login /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );
        //创建静态文本框控件--密码
        TEXT2 = CreateWindow(TEXT("static"), TEXT("密码："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/| SS_RIGHT /*水平居右*/,
            10, 140, 70, 26,
            login, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建单行文本框控件
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            100, 80, 200, 26,
            login, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建密码输入框
        INPUT2 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD /*密码*/ | ES_AUTOHSCROLL /*水平滚动*/,
            100, 140, 200, 26,
            login, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件
        LOGIN = CreateWindow(TEXT("button"), TEXT("登录"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 110, 100, 30,
            login, (HMENU)5, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(LOGIN, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(login, &ps);
        SelectObject(hdc, hFont);               //  设置显示字体
        GetClientRect(login, &rect);

        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0"), -1, &rect, DT_LEFT | DT_CENTER);        //  绘制登录界面标题

        if (flag == 3)      //  尝试次数尚未用完
        {
            TextOut(hdc, 100, 180, TEXT("您有三次输入机会"), 8);
        }
        if (flag == 2)      //  尝试次数尚未用完
        {
            TextOut(hdc, 100, 180, TEXT("账号与密码不匹配，请重试！\n还剩余两次输入机会"), 23);
        }
        if (flag == 1)      //  尝试次数尚未用完
        {
            TextOut(hdc, 100, 180, TEXT("账号与密码不匹配，请重试！\n还剩余一次输入机会"), 23);
        }
        if (flag == 0)      //  尝试次数已用完
        {
            TextOut(hdc, 100, 180, TEXT("您的输入机会已用完！"), 10);
        }

        EndPaint(login, &ps);
        return 0;

    case WM_COMMAND:

        wmid = LOWORD(wParam);
        wmevent = HIWORD(wParam);

        if ((HWND)lParam == LOGIN) {

            GetWindowText(INPUT1, USERNAME, MaxLength);
            GetWindowText(INPUT2, PASSWORD, MaxLength);
            wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n您的用户账号：%s\r\n您的用户密码：%s"), USERNAME, PASSWORD);
            MessageBox(login, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);

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
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建按钮控件--创建数据库
        CREATE_DB = CreateWindow(TEXT("BUTTON"), TEXT("创建代码数据库 Create Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430 /*x坐标*/, 80 /*y坐标*/, 300 /*宽度*/, 45 /*高度*/,
            db_menu /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );
        //创建按钮控件--删除数据库
        DELETE_DB = CreateWindow(TEXT("BUTTON"), TEXT("删除代码数据库 Delete Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 165, 300, 45,
            db_menu, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建按钮控件--调用数据库
        OPEN_DB = CreateWindow(TEXT("BUTTON"), TEXT("调用代码数据库 Open Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 250, 300, 45,
            db_menu, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建按钮控件--退出程序
        EXIT = CreateWindow(TEXT("BUTTON"), TEXT("退出程序 Exit"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 335, 300, 45,
            db_menu, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(CREATE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(DELETE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(OPEN_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(EXIT, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_menu, &ps);
        SelectObject(hdc, hFont);           //  设置显示字体
        GetClientRect(db_menu, &rect1);
        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0  数据库"), -1, &rect1, DT_LEFT | DT_CENTER);        //  绘制界面标题
        DrawText(hdc,
            TEXT("\n\n\nC代码库\n\nJava代码库\n\nPython代码库（未创建）\n\nC++代码库（未创建）\n\nMATLAB代码库（未创建）\n\nSQL代码库（未创建）"),
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
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建静态文本框控件--库名
        TEXT1 = CreateWindow(TEXT("static"), TEXT("库 名："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ | SS_RIGHT /*水平居右*/,
            10 /*x坐标*/, 80 /*y坐标*/, 70 /*宽度*/, 26 /*高度*/,
            db_input_C /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("名称字数限制：10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ /*水平居右*/,
            100 /*x坐标*/, 140 /*y坐标*/, 120 /*宽度*/, 26 /*高度*/,
            db_input_C /*父窗口句柄*/, (HMENU)5 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        //创建单行文本框控件
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            100, 80, 200, 26,
            db_input_C, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--确定
        CONF = CreateWindow(TEXT("button"), TEXT("确定"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 80, 100, 30,
            db_input_C, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--返回上一级
        BACK = CreateWindow(TEXT("button"), TEXT("返回上一级"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 140, 100, 30,
            db_input_C, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_C, &ps);
        SelectObject(hdc, hFont);               //  设置显示字体
        GetClientRect(db_input_C, &rect);
        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0  数据库创建\n\n[请输入数据库名称]"), -1, &rect, DT_LEFT | DT_CENTER);        //  绘制登录界面标题
        
        EndPaint(db_input_C, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DBNAME, 10);
            if (DT_Database_Create(DBNAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 创建成功！"), DBNAME);
                MessageBox(db_input_C, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);
                ShowWindow(db_input_C, SW_HIDE);
                UpdateWindow(db_input_C);
            }
            else if (DT_Database_Create(DBNAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 已经存在！"), DBNAME);
                MessageBox(db_input_C, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);
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
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建静态文本框控件--库名
        TEXT1 = CreateWindow(TEXT("static"), TEXT("库 名："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ | SS_RIGHT /*水平居右*/,
            10 /*x坐标*/, 80 /*y坐标*/, 70 /*宽度*/, 26 /*高度*/,
            db_input_D /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("名称字数限制：10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ /*水平居右*/,
            100 /*x坐标*/, 140 /*y坐标*/, 120 /*宽度*/, 26 /*高度*/,
            db_input_D /*父窗口句柄*/, (HMENU)5 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        //创建单行文本框控件
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            100, 80, 200, 26,
            db_input_D, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--确定
        CONF = CreateWindow(TEXT("button"), TEXT("确定"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 80, 100, 30,
            db_input_D, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--返回上一级
        BACK = CreateWindow(TEXT("button"), TEXT("返回上一级"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 140, 100, 30,
            db_input_D, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_D, &ps);
        SelectObject(hdc, hFont);               //  设置显示字体
        GetClientRect(db_input_D, &rect);
        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0  数据库删除\n\n[请输入数据库名称]"), -1, &rect, DT_LEFT | DT_CENTER);        //  绘制登录界面标题

        EndPaint(db_input_D, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DBNAME, 10);
            if (DT_Database_Delete(DBNAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 删除成功！"), DBNAME);
                MessageBox(db_input_D, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);
                ShowWindow(db_input_D, SW_HIDE);
                UpdateWindow(db_input_D);
            }
            else if (DT_Database_Delete(DBNAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 不存在！"), DBNAME);
                MessageBox(db_input_D, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);
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
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建静态文本框控件--库名
        TEXT1 = CreateWindow(TEXT("static"), TEXT("库 名："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ | SS_RIGHT /*水平居右*/,
            10 /*x坐标*/, 80 /*y坐标*/, 70 /*宽度*/, 26 /*高度*/,
            db_input_O /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        TEXT2 = CreateWindow(TEXT("static"), TEXT("名称字数限制：10"),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ /*水平居右*/,
            100 /*x坐标*/, 140 /*y坐标*/, 120 /*宽度*/, 26 /*高度*/,
            db_input_O /*父窗口句柄*/, (HMENU)5 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );

        //创建单行文本框控件
        INPUT1 = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            100, 80, 200, 26,
            db_input_O, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--确定
        CONF = CreateWindow(TEXT("button"), TEXT("确定"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 80, 100, 30,
            db_input_O, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //创建按钮控件--返回上一级
        BACK = CreateWindow(TEXT("button"), TEXT("返回上一级"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            350, 140, 100, 30,
            db_input_O, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT2, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(INPUT1, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(BACK, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(CONF, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(db_input_O, &ps);
        SelectObject(hdc, hFont);               //  设置显示字体
        GetClientRect(db_input_O, &rect);
        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0  数据库调用\n\n[请输入数据库名称]"), -1, &rect, DT_LEFT | DT_CENTER);        //  绘制登录界面标题

        EndPaint(db_input_O, &ps);
        return 0;

    case WM_COMMAND:

        if ((HWND)lParam == CONF)
        {
            GetWindowText(INPUT1, DBNAME, 10);
            if (DT_Database_Open(DBNAME) == 1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 调用成功！"), DBNAME);
                MessageBox(db_input_O, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);

                //  关闭此前的界面
                ShowWindow(db_menu, SW_HIDE);
                UpdateWindow(db_menu);
                ShowWindow(db_input_O, SW_HIDE);
                UpdateWindow(db_input_O);

                ShowWindow(tb_menu, SW_SHOW);                   //  打开数据表菜单界面
                UpdateWindow(tb_menu);
            }
            if (DT_Database_Open(DBNAME) == -1)
            {
                wsprintf(PROMPT, TEXT("BAMS Version 2.0 系统提示：\r\n数据库：%s 错误或不存在！"), DBNAME);
                MessageBox(db_input_O, PROMPT, TEXT("信息提示"), MB_ICONINFORMATION);

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
    RECT rect = { 30, 30, 300, 390 }, rect1;

    static HWND CREATE_DB, DELETE_DB, OPEN_DB, EXIT, TEXT1;
    static HFONT hFont;                                             //  定义：逻辑字体


    switch (message)
    {

    case WM_CREATE:

        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑"));

        //创建按钮控件--创建数据库
        CREATE_DB = CreateWindow(TEXT("BUTTON"), TEXT("创建代码数据库 Create Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430 /*x坐标*/, 80 /*y坐标*/, 300 /*宽度*/, 45 /*高度*/,
            tb_menu /*父窗口句柄*/, (HMENU)1 /*控件ID*/, ((LPCREATESTRUCT)lParam)->hInstance /*当前程序实例句柄*/, NULL
        );
        //创建按钮控件--删除数据库
        DELETE_DB = CreateWindow(TEXT("BUTTON"), TEXT("删除代码数据库 Delete Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 165, 300, 45,
            tb_menu, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建按钮控件--调用数据库
        OPEN_DB = CreateWindow(TEXT("BUTTON"), TEXT("调用代码数据库 Open Database"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 250, 300, 45,
            tb_menu, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );
        //创建按钮控件--退出程序
        EXIT = CreateWindow(TEXT("BUTTON"), TEXT("退出程序 Exit"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            430, 335, 300, 45,
            tb_menu, (HMENU)4, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        //依次设置控件的字体
        SendMessage(CREATE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(DELETE_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(OPEN_DB, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(EXIT, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(TEXT1, WM_SETFONT, (WPARAM)hFont, NULL);
        //break;

    case WM_PAINT:

        hdc = BeginPaint(tb_menu, &ps);
        SelectObject(hdc, hFont);           //  设置显示字体
        GetClientRect(tb_menu, &rect1);
        DrawText(hdc, TEXT("BAMS 代码管理系统 Version 2.0  数据库"), -1, &rect1, DT_LEFT | DT_CENTER);        //  绘制界面标题

        EndPaint(tb_menu, &ps);
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