#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN      
#endif

#include <windows.h> 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL\gl.h>
#include <math.h>
#include <time.h>

#define DEFAULT_SCREEN_WIDTH 680
#define DEFAULT_SCREEN_HEIGHT 420

uint32_t screen_width = DEFAULT_SCREEN_WIDTH;
uint32_t screen_height = DEFAULT_SCREEN_HEIGHT;

LRESULT CALLBACK
Win32MainWindowCallback(
    HWND Window,   //HWND a handle to a window
    UINT Message,  //System defined message, Window will call us and ask us to respond it
    WPARAM WParam, //
    LPARAM LParam) //
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_SIZE:
    {
    	screen_width = LOWORD(LParam);
    	screen_height = HIWORD(LParam);
        //glViewport(0, 0, screen_width, screen_height); //needed for window resizing
        //CenterCursor( Window );
    }break;

    case WM_MOVE:
    {
    	//CenterCursor( Window );
    }break;

    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)LParam;
        lpMMI->ptMinTrackSize.x = 300;
        lpMMI->ptMinTrackSize.y = 300;
    }break;

    case WM_CLOSE: //when user clicks on the X button on the window
    {
        //CloseProgram();
    } break;

    case WM_PAINT: //to allow us to update the window
    {
        //drawScene( shaderProgram, 0.0f );
        //PAINTSTRUCT Paint;
        //HDC DeviceContext = BeginPaint(Window, &Paint); //will fill out struct with information on where to paint
        //EndPaint(Window, &Paint); //closes paint area
    } break;


    case WM_ACTIVATE:
    {
    	switch(WParam)
    	{
    		case WA_ACTIVE:
    		case WA_CLICKACTIVE:
    		{

    		} break;
    		case WA_INACTIVE:
    		{
    			//Pause();
    		} break;
    		default:
    		{
    			//error
    		}
    	}
    } break;

    default:
        Result = DefWindowProc(Window, Message, WParam, LParam); //call windows to handle default behavior of things we don't handle
    }

    return Result;
}

#if MAIN_DEBUG
int main()
#else
int APIENTRY WinMain(
    _In_ HINSTANCE Instance,
    _In_opt_ HINSTANCE PrevInstance,
    _In_ LPSTR CommandLine,
    _In_ int ShowCode)
#endif
{
	WNDCLASSEX WindowClass;
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; //https://devblogs.microsoft.com/oldnewthing/20060601-06/?p=31003
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
#if MAIN_DEBUG
    WindowClass.hInstance = GetModuleHandle(NULL);
#else
    WindowClass.hInstance = Instance;
#endif
    WindowClass.hIcon = LoadIcon(0, IDI_APPLICATION); //IDI_APPLICATION: Default application icon, 0 means use a default Icon
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW); //IDC_ARROW: Standard arrow, 0 means used a predefined Cursor
    WindowClass.hbrBackground = NULL; 
    WindowClass.lpszMenuName = NULL;	// No menu 
    WindowClass.lpszClassName = "TheButtonWindowClass"; //name our class
    WindowClass.hIconSm = NULL; //can also do default Icon here? will NULL be default automatically?
    if ( !RegisterClassEx(&WindowClass) )
    {
    	return -1;
    }
    HWND WindowHandle = CreateWindowEx(0, WindowClass.lpszClassName, "The Button",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, screen_width, screen_height, //if fullscreen get monitor width and height
        0, 0, WindowClass.hInstance, NULL);
    if( !WindowHandle )
    {
    	return -1;
    }



	return 0;
}