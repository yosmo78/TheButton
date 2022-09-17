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
	
	return 0;
}