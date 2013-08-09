#include <windows.h>
#include <stdbool.h>

WNDCLASS wc;
typedef int (*fnPointer)(WPARAM,LPARAM);
struct bridgeStruct{
    int *iCmdShow;
    LPCTSTR* imgArray;
    WPARAM* keyListener;
    unsigned int keyListenerCount = 0;
    fnPointer *keyListenerFn;
    int* clickListenerX1;
    int* clickListenerX2;
    int* clickListenerY1;
    int* clickListenerY2;
    unsigned int clickListenerCount = 0;
    fnPointer *clickListenerFn;
}bridge;

bool __winH_loadImg(HDC hdc,LPCTSTR* bmpfile,short width,short height)
{
	if((NULL == hdc) || (NULL == bmpfile))
		return false;
	HANDLE hBmp = LoadImage(NULL,bmpfile,IMAGE_BITMAP,width,height,LR_LOADFROMFILE);
	if (NULL == hBmp)
		return false;
	HDC dcmem = CreateCompatibleDC(NULL);
	if(NULL == SelectObject(dcmem,hBmp))
	{
		DeleteDC(dcmem);
		return false;
	}
	BITMAP bm;
	GetObject(hBmp,sizeof(bm),&bm);
	if(BitBlt(hdc,0,0,bm.bmWidth,bm.bmHeight,dcmem,0,0,SRCCOPY) == 0)
	{
		DeleteDC(dcmem);
		return false;
	}keyListenerCount
	DeleteDC(dcmem);
	return true;
}

int _ct_checkListener(WPARAM wparam,LPARAM lparam)
{
    unsigned int x = 0;
    while(x < bridge.keyListenerCount)
    {
        if(wparam == bridge.keyListener[x])
            return keyListenerFn[x](wparam,lparam);
        x++;
    }
    return 0;
}

int _ct_checkCollition(LPARAM lparam)
{
    WPARAM deleteThis;
    unsigned int z = 0;
    x = GET_X_LPARAM(lparam);
    y = GET_Y_LPARAM(lparam);
    while(z < bridge.clickListenerCount)
    {
        if(x > bridge.clickListenerX1[z] && y > bridge.clickListenerY1[z] && x < bridge.clickListenerX2[z] && y < bridge.clickListenerY2[z])
            return bridge.clickListenerFn[z](deleteThis,lparam);
        z++;
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch( message )
    {
        case WM_CREATE:
            return 0;
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc;
                short buff = 0,len = 0;
                hdc = BeginPaint(hwnd,&ps);
                while(buff < len)
                {
                    __winH_loadImg(hdc,bridge.imgArray[buff],0,0);
                    buff++;
                }

                // draw a circle and a 2 squares
                Ellipse(hdc, 20, 20, 160, 160 );
                Rectangle(hdc, 50, 50, 90, 90 );
                Rectangle(hdc, 100, 50, 140, 90 );

                EndPaint( hwnd,&ps);
            }
            return 0;
            break;
        case WM_KEYDOWN:
            return _ct_checkListener(wparam,lparam);
            break;
        case WM_RBUTTONUP:
            return _ct_checkCollition(lparam);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;
    }
    return DefWindowProc(hwnd,message,wparam,lparam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL,IDI_APPLICATION);
    wc.hCursor = LoadCursor( NULL,IDC_HAND);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = TEXT("pokeMain");
    bridge.iCmdShow = iCmdShow;
    RegisterClass(&wc);
    tonchy();
}

int mkWin(short width,short height,const char* title)
{
    HWND hwnd = CreateWindow(TEXT("pokeMain"),TEXT(title),WS_OVERLAPPEDWINDOW,0,0,width,height,NULL, NULL,wc.hInstance,NULL);
    ShowWindow(hwnd,bridge.iCmdShow);
    return UpdateWindow(hwnd);
}

int go()
{
    MSG msg;
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

char loadImg(const char* fileName)
{
    return 0;
}
