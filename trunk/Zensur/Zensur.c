#include "Zensur.h"
#define MAX_LOADSTRING		100
#define MAX_NUM_OF_MARKS	50

// Global Variables:
BOOL bErrorMsgDisplayed = FALSE;
WNDPROC OldEditProc;
HINSTANCE hInst;								// current instance
HWND hWndDlg, hEditMarks, hBtnCalc, hLbAverage, hLbErrorMsg;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	EditProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
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

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ZENSURWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ACC_ZENSURWIN32);
	
	// Main message loop:	
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
	HFONT EditFont, StaticFont;
	
	// Store instance handle in our global variable 
	hInst = hInstance;
   
	hWndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAIN), 0, NULL);
	
	hBtnCalc = GetDlgItem(hWndDlg, IDC_BUTTON_BERECHNEN);
	
	
	hEditMarks = GetDlgItem(hWndDlg, IDC_EDIT_MARKS);
	// Change font of edit control
	EditFont = CreateFont(-MulDiv(14, GetDeviceCaps(GetDC(hEditMarks), LOGPIXELSY), 72), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Microsoft Sans Serif");
	SendMessage(hEditMarks, WM_SETFONT, (WPARAM)EditFont, 0);	
	// Set new window procedure for edit control
	OldEditProc = (WNDPROC) SetWindowLongPtr(hEditMarks, GWL_WNDPROC, (LONG_PTR)EditProc);	


	hLbAverage = GetDlgItem(hWndDlg, IDC_STATIC_AVERAGE);
	StaticFont = CreateFont(-MulDiv(15, GetDeviceCaps(GetDC(hLbAverage), LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Microsoft Sans Serif");
	SendMessage(hLbAverage, WM_SETFONT, (WPARAM)StaticFont, TRUE);
	
	hLbErrorMsg = GetDlgItem(hWndDlg, IDC_STATIC_ERROR_MSG);
	
	ShowWindow(hWndDlg, SW_SHOWNORMAL);
	SetFocus(hEditMarks);

	return TRUE;
}

// Processes messages for the main window
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
		case IDC_BUTTON_BERECHNEN:
			Calc();
			SetFocus(hEditMarks);
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
	LRESULT Selection;

	switch (message)
	{
	case WM_SETFOCUS:
		SendMessage(hEditMarksControl, EM_SETSEL, 0, GetWindowTextLength(hEditMarks));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			SendMessage(hBtnCalc, BM_SETSTATE, TRUE, 0);
			UpdateWindow(hBtnCalc);			
			SleepEx(100, TRUE);	
			SendMessage(hBtnCalc, BM_SETSTATE, FALSE, 0);
			SendMessage(hWndDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_BERECHNEN, BN_CLICKED), (LPARAM)hBtnCalc);			
			SendMessage(hEditMarksControl, EM_SETSEL, 0, GetWindowTextLength(hEditMarks));
			break;
		}

	case WM_CHAR:
		switch (wParam)
		{			
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
			Selection = SendMessage(hEditMarksControl, EM_GETSEL, 0, 0);
			Selection = HIWORD(Selection) - LOWORD(Selection);
			if (GetWindowTextLength(hEditMarks) >= MAX_NUM_OF_MARKS
			    &&  Selection == 0) {
				NotifyLimitedNumberOfMarks();
				return 0;
			}
			break;

		case VK_BACK:			
		case VK_DELETE:
		case VK_LEFT:
		case VK_RIGHT:
			break;
		
		default:
			NotifyInvalidCharacter();
			return 0;
		}
		ClearNotification();
	}
	
	return CallWindowProc(OldEditProc, hEditMarksControl, message, wParam, lParam);
}

// Message handler for about box.

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
			SetFocus(hEditMarks);			
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Calc()
{	
	int NumOfMarks;
	char buf[MAX_NUM_OF_MARKS + 1];

	NumOfMarks = GetWindowTextLength(hEditMarks);
	if (NumOfMarks)
	{		
		int i, Sum;
		float Average;
		Sum = 0; Average = 0;
		GetWindowText(hEditMarks, buf, NumOfMarks + 1);
		for (i=0; i<NumOfMarks; i++)
		{
			Sum += buf[i]-48;	
		}
		Average = (float)Sum /(float)NumOfMarks;
		_snprintf_s(buf, MAX_NUM_OF_MARKS + 1, 6, "%.4f", Average);
		SetWindowText(hLbAverage, buf);
	}
}

void NotifyInvalidCharacter() {
	SetWindowText(hLbErrorMsg, TEXT("Sie können nur die Zahlen 1 bis 6 eingeben."));
	bErrorMsgDisplayed = TRUE;
}

void NotifyLimitedNumberOfMarks() {
	SetWindowText(hLbErrorMsg, TEXT("Sie können maximal 50 Zensuren eingeben."));
	bErrorMsgDisplayed = TRUE;
}

void ClearNotification() {
	if (bErrorMsgDisplayed == TRUE) {
		SetWindowText(hLbErrorMsg, TEXT(""));
		InvalidateRect(hLbErrorMsg, NULL, TRUE);
		RedrawWindow(hLbErrorMsg, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
		bErrorMsgDisplayed = FALSE;
	}
}

