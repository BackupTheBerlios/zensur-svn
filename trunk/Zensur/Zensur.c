#include "Zensur.h"
#define MAX_LOADSTRING		100
#define MAX_NUM_OF_MARKS	50
#define NUM_EXT				4

// Global Variables:
static BOOL bErrorMsgDisplayed = FALSE;
static HINSTANCE hInst;										// current instance
static HWND hWndDlg;
static SIZE DlgSizeBase;
static SIZE DlgSizeExtOffSet;
static HWND hBtnExt[NUM_EXT - 1];
static HWND hEditMarks[NUM_EXT];
static HWND hEditPercentage[NUM_EXT];
static HWND hLbPercentage[NUM_EXT];
static HWND hBtnCalc;
static HWND hLbAverage, hLbErrorMsg;
static LONG_PTR OldEditProc[NUM_EXT];
static TCHAR szTitle[MAX_LOADSTRING];						// the title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];					// the main window class name

static unsigned char percentages = 1;


// forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	EditProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void                UpdateUI(unsigned short);
void                Calc();
void				NotifyInvalidCharacter();
void				NotifyLimitedNumberOfMarks();
void				ClearNotification();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
 	MSG msg;
	HACCEL hAccelTable;

	InitCommonControls();

	// initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ZENSURWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ACC_ZENSURWIN32);
	
	// main message loop:	
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(hWndDlg, hAccelTable, &msg)) 
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );			
		}
	}	
	return (int) msg.wParam;
}

// This function and its usage are only necessary if you want this code
// to be compatible with Win32 systems prior to the 'RegisterClassEx'
// function that was added to Windows 95. It is important to call this function
// so that the application will get 'well formed' small icons associated
// with it.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndDlgProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= DLGWINDOWEXTRA;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadImage(hInstance,
									MAKEINTRESOURCE(IDI_ZENSUR),
									IMAGE_ICON, 
									GetSystemMetrics(SM_CXICON), 
									GetSystemMetrics(SM_CYICON), 
									LR_DEFAULTCOLOR);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;
	wcex.hIconSm		= LoadImage(hInstance,
									MAKEINTRESOURCE(IDI_ZENSUR),
									IMAGE_ICON, 
									GetSystemMetrics(SM_CXSMICON), 
									GetSystemMetrics(SM_CYSMICON), 
									LR_DEFAULTCOLOR);

	return RegisterClassEx(&wcex);
}

//  In this function, we save the instance handle in a global variable and
//  create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int i;
	HFONT EditFont, StaticFont;
	RECT rc0, rc1;
	
	// store instance handle in our global variable 
	hInst = hInstance;
   
	hWndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAIN), 0, NULL);
	
	hBtnCalc = GetDlgItem(hWndDlg, IDC_BUTTON_CALC);
	
	hBtnExt[0] = GetDlgItem(hWndDlg, IDC_BUTTON_EXT1);
	hEditMarks[0] = GetDlgItem(hWndDlg, IDC_EDIT_MARKS1);
	hEditPercentage[0] = GetDlgItem(hWndDlg, IDC_EDIT_PERCENTAGE1);
	hLbPercentage[0] = GetDlgItem(hWndDlg, IDC_STATIC_PERCENTAGE1);

	hBtnExt[1] = GetDlgItem(hWndDlg, IDC_BUTTON_EXT2);
	hEditMarks[1] = GetDlgItem(hWndDlg, IDC_EDIT_MARKS2);
	hEditPercentage[1] = GetDlgItem(hWndDlg, IDC_EDIT_PERCENTAGE2);
	hLbPercentage[1] = GetDlgItem(hWndDlg, IDC_STATIC_PERCENTAGE2);

	hBtnExt[2] = GetDlgItem(hWndDlg, IDC_BUTTON_EXT3);
	hEditMarks[2] = GetDlgItem(hWndDlg, IDC_EDIT_MARKS3);
	hEditPercentage[2] = GetDlgItem(hWndDlg, IDC_EDIT_PERCENTAGE3);
	hLbPercentage[2] = GetDlgItem(hWndDlg, IDC_STATIC_PERCENTAGE3);

	hEditMarks[3] = GetDlgItem(hWndDlg, IDC_EDIT_MARKS4);
	hEditPercentage[3] = GetDlgItem(hWndDlg, IDC_EDIT_PERCENTAGE4);
	hLbPercentage[3] = GetDlgItem(hWndDlg, IDC_STATIC_PERCENTAGE4);
	
	// create font for edit controls
	EditFont = CreateFont(-MulDiv(14, GetDeviceCaps(GetDC(hEditMarks[0]), LOGPIXELSY), 72), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Microsoft Sans Serif");

	for (i = 0; i < NUM_EXT; ++i)
	{
		// change font
		SendMessage(hEditMarks[i], WM_SETFONT, (WPARAM)EditFont, 0);
		SendMessage(hEditPercentage[i], WM_SETFONT, (WPARAM)EditFont, 0);
		SendMessage(hLbPercentage[i], WM_SETFONT, (WPARAM)EditFont, 0);
		// set new window procedure
		OldEditProc[i] = SetWindowLongPtr(hEditMarks[i], GWLP_WNDPROC, (LONG_PTR)EditProc);
	}

	GetWindowRect(hWndDlg, &rc0);
	DlgSizeBase.cx = rc0.right - rc0.left;
	DlgSizeBase.cy = rc0.bottom - rc0.top;
	
	GetWindowRect(hEditMarks[0], &rc0);
	GetWindowRect(hLbPercentage[0], &rc1);
	DlgSizeExtOffSet.cx = rc1.right - rc0.right;
	GetWindowRect(hEditMarks[1], &rc1);
	DlgSizeExtOffSet.cy = rc1.top - rc0.top;
	

	hLbAverage = GetDlgItem(hWndDlg, IDC_STATIC_AVERAGE);
	StaticFont = CreateFont(-MulDiv(15, GetDeviceCaps(GetDC(hLbAverage), LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Microsoft Sans Serif");
	SendMessage(hLbAverage, WM_SETFONT, (WPARAM)StaticFont, TRUE);
	
	hLbErrorMsg = GetDlgItem(hWndDlg, IDC_STATIC_ERROR_MSG);
	
	ShowWindow(hWndDlg, SW_SHOWNORMAL);
	SetFocus(hEditMarks[0]);

	return TRUE;
}

// processes messages for the main window
LRESULT CALLBACK WndDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	
	switch (message) 
	{	
	case WM_CREATE:		
		return TRUE;

	case WM_CTLCOLORSTATIC:
		if (GetWindowLong((HWND)lParam, GWL_ID) == IDC_STATIC_AVERAGE)
		{
			SetTextColor((HDC)wParam, RGB(158, 0, 57));			
		}
		if (GetWindowLong((HWND)lParam, GWL_ID) == IDC_STATIC_ERROR_MSG)
		{
			SetTextColor((HDC)wParam, RGB(255, 0, 0));			
		}
		SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNFACE));
		return (LRESULT)GetStockObject(NULL_BRUSH);
	
	// process the application menu
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam);		
		switch (wmId)
		{
		case IDC_BUTTON_EXT1:
			UpdateUI(1);
			break;
		case IDC_BUTTON_EXT2:
			UpdateUI(2);
			break;
		case IDC_BUTTON_EXT3:
			UpdateUI(3);
			break;		
		case IDC_BUTTON_CALC:
			Calc();
			SetFocus(hEditMarks[0]);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hDlg, (DLGPROC)About);
			break;
		case IDM_EXIT:			
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;		
		}
		break;

	// post a quit message and return
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hDlg, message, wParam, lParam);
}

LRESULT CALLBACK EditProc(HWND hEditMarksControl, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	LRESULT Selection;

	switch (message)
	{
	case WM_SETFOCUS:
		SendMessage(hEditMarksControl, EM_SETSEL, 0, GetWindowTextLength(hEditMarksControl));
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
			ClearNotification();
			Selection = SendMessage(hEditMarksControl, EM_GETSEL, 0, 0);
			Selection = HIWORD(Selection) - LOWORD(Selection);
			if (GetWindowTextLength(hEditMarksControl) >= MAX_NUM_OF_MARKS
			    &&  Selection == 0)
			{
				NotifyLimitedNumberOfMarks();
				return 0;
			}			
			break;

		case VK_BACK:			
		case VK_DELETE:
		case VK_LEFT:
		case VK_RIGHT:
			ClearNotification();
			break;
		
		case VK_RETURN:
			ClearNotification();
			SendMessage(hBtnCalc, BM_SETSTATE, TRUE, 0);
			UpdateWindow(hBtnCalc);			
			SleepEx(100, TRUE);	
			SendMessage(hBtnCalc, BM_SETSTATE, FALSE, 0);
			SendMessage(hWndDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_CALC, BN_CLICKED), (LPARAM)hBtnCalc);			
			SendMessage(hEditMarksControl, EM_SETSEL, 0, GetWindowTextLength(hEditMarksControl));
			break;
		
		default:
			NotifyInvalidCharacter();
			return 0;
		}
	}

	// lookup index for old window procedure of current edit control
	for (i = 0; i < NUM_EXT; ++i)
	{
		if (hEditMarks[i] == hEditMarksControl)
		{
			break;
		}
	}
	
	return CallWindowProc(OldEditProc[i], hEditMarksControl, message, wParam, lParam);
}

// message handler for about box.

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			SetFocus(hEditMarks[0]);			
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void UpdateUI(unsigned short extButtonNr)
{
	if (extButtonNr == percentages)
	{
		SetWindowText(hBtnExt[extButtonNr-1], "-");
		ShowWindow(hEditPercentage[extButtonNr-1], SW_SHOW);
		ShowWindow(hLbPercentage[extButtonNr-1], SW_SHOW);
		ShowWindow(hBtnExt[extButtonNr], SW_SHOW);
		ShowWindow(hEditMarks[extButtonNr], SW_SHOW);
		ShowWindow(hEditPercentage[extButtonNr], SW_SHOW);
		ShowWindow(hLbPercentage[extButtonNr], SW_SHOW);
		SetWindowPos(hWndDlg,
			NULL, 0, 0,
			DlgSizeBase.cx + DlgSizeExtOffSet.cx,
			DlgSizeBase.cy + DlgSizeExtOffSet.cy * extButtonNr,
			SWP_NOMOVE | SWP_NOZORDER);
		SetWindowPos(hWndDlg,
			NULL, 0, 0,
			DlgSizeBase.cx + DlgSizeExtOffSet.cx,
			DlgSizeBase.cy + DlgSizeExtOffSet.cy * extButtonNr,
			SWP_NOMOVE | SWP_NOZORDER);

		++percentages;
	}
}

void Calc()
{	
	int NumOfMarks;
	char buf[MAX_NUM_OF_MARKS + 1];

	NumOfMarks = GetWindowTextLength(hEditMarks[0]);
	if (NumOfMarks)
	{		
		int i, Sum;
		float Average;
		Sum = 0; Average = 0;
		GetWindowText(hEditMarks[0], buf, NumOfMarks + 1);
		for (i=0; i<NumOfMarks; i++)
		{
			Sum += buf[i]-48;	
		}
		Average = (float)Sum /(float)NumOfMarks;
		_snprintf_s(buf, MAX_NUM_OF_MARKS + 1, 6, "%.4f", Average);
		SetWindowText(hLbAverage, buf);
	}
}

void NotifyInvalidCharacter()
{
	SetWindowText(hLbErrorMsg, TEXT("Sie können nur die Zahlen 1 bis 6 eingeben."));
	bErrorMsgDisplayed = TRUE;
}

void NotifyLimitedNumberOfMarks()
{
	SetWindowText(hLbErrorMsg, TEXT("Sie können maximal 50 Zensuren eingeben."));
	bErrorMsgDisplayed = TRUE;
}

void ClearNotification()
{
	if (bErrorMsgDisplayed == TRUE)
	{
		SetWindowText(hLbErrorMsg, TEXT(""));		
		RedrawWindow(hWndDlg, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
		bErrorMsgDisplayed = FALSE;
	}
}

