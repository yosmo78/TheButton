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

#define STRINGIFY(x) #x

uint32_t screen_width = DEFAULT_SCREEN_WIDTH;
uint32_t screen_height = DEFAULT_SCREEN_HEIGHT;

//state
uint8_t Running;

//Window handles
HDC hDC;
HWND WindowHandle;

#define GL_COMPILE_STATUS   	0x8B81
#define GL_LINK_STATUS      	0x8B82
#define GL_VERTEX_SHADER    	0x8B31
#define GL_FRAGMENT_SHADER  	0x8B30
#define GL_ARRAY_BUFFER     	0x8892
#define GL_ELEMENT_ARRAY_BUFFER	0x8893
#define GL_STATIC_DRAW      	0x88E4

#ifdef _WIN64
typedef int64_t khronos_ssize_t;
#else
typedef int32_t khronos_ssize_t;
#endif

typedef khronos_ssize_t GLsizeiptr;
typedef char GLchar;

GLuint (__stdcall *glCreateShader)(GLenum shaderType) = NULL;
void   (__stdcall *glShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) = NULL;
void   (__stdcall *glCompileShader)(GLuint shader) = NULL;
void   (__stdcall *glGetShaderiv)(GLuint shader, GLenum pname, GLint* params) = NULL;
void   (__stdcall *glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = NULL;
GLuint (__stdcall *glCreateProgram)(void) = NULL;
void   (__stdcall *glAttachShader)(GLuint program, GLuint shader) = NULL;
void   (__stdcall *glLinkProgram)(GLuint program) = NULL;
void   (__stdcall *glGetProgramiv)(GLuint program, GLenum pname,GLint* params) = NULL;
void   (__stdcall *glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = NULL;
void   (__stdcall *glDeleteShader)(GLuint shader) = NULL;
void   (__stdcall *glUseProgram)(GLuint program) = NULL;
void   (__stdcall *glGenVertexArrays)(GLsizei n, GLuint* arrays) = NULL; 
void   (__stdcall *glGenBuffers)(GLsizei n, GLuint* buffers) = NULL;
void   (__stdcall *glBindVertexArray)(GLuint array) = NULL; 
void   (__stdcall *glBindBuffer)(GLenum target, GLuint buffer) = NULL;
void   (__stdcall *glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage) = NULL;
void   (__stdcall *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) = NULL;
void   (__stdcall *glEnableVertexAttribArray)(GLuint index) = NULL;
GLint  (__stdcall *glGetUniformLocation)(GLuint program, const GLchar *name) = NULL;
void   (__stdcall *glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
void   (__stdcall *glUniform3fv)( GLint location, GLsizei count, const GLfloat *value) = NULL;
void   (__stdcall *glUniform4fv)( GLint location, GLsizei count, const GLfloat *value) = NULL;

void CloseProgram()
{
	Running = 0;
}

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
        CloseProgram();
    } break;

    case WM_PAINT: //to allow us to update the window
    {
        //drawScene( shaderProgram, 0.0f );
        PAINTSTRUCT Paint;
        HDC DeviceContext = BeginPaint(Window, &Paint); //will fill out struct with information on where to paint
        EndPaint(Window, &Paint); //closes paint area
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
int CreateOpenGLWindow()
#else
int CreateOpenGLWindow( HINSTANCE Instance )
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
    WindowHandle = CreateWindowEx(0, WindowClass.lpszClassName, "The Button",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, screen_width, screen_height, //if fullscreen get monitor width and height
        0, 0, WindowClass.hInstance, NULL);
    if( !WindowHandle )
    {
    	return -1;
    }
    
    //Associate OpenGL with out window
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
        PFD_TYPE_RGBA,												// The kind of framebuffer. RGBA or palette.
        32,															// Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
        0, 0, 0, 0,
        24,															// Number of bits for the depthbuffer
        8,															// Number of bits for the stencilbuffer
        0,															// Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    hDC = GetDC(WindowHandle);				// Get a DC for our window
    int letWindowsChooseThisPixelFormat = ChoosePixelFormat(hDC, &pfd); // Let windows select an appropriate pixel format
    if( !letWindowsChooseThisPixelFormat )
    {
        return -1;
    }
    if( !SetPixelFormat(hDC, letWindowsChooseThisPixelFormat, &pfd) )
    {
        return -1;
    }
    HGLRC ourOpenGLRC = wglCreateContext(hDC);
    if (!ourOpenGLRC)
    {
        return -1;
    }
    wglMakeCurrent(hDC, ourOpenGLRC); // Make our render context current
    return 0;
}

/* make sure you are in wgl Context!*/
inline void loadGLFuncPtrs()
{
    //load needed version specific opengl funcs
    int ver = glGetString(GL_VERSION)[0] - '0'; //is opengl 2.0 compatible
    if (ver > 4) ver = 4; //future proofing
    switch (ver)
    {
        case 4: 
        {
            //opengl version 4.0 required
        }
        case 3: 
        {
            //opengl version 3.0 required
            glGenVertexArrays = (void(__stdcall*)(GLsizei n, GLuint * arrays))((void*)wglGetProcAddress("glGenVertexArrays"));
            glBindVertexArray = (void(__stdcall*)(GLuint array))((void*)wglGetProcAddress("glBindVertexArray"));
        }
        case 2:
        {
            //opengl version 2.0 required
            glCreateShader = (GLuint(__stdcall *)(GLenum shaderType))((void*)wglGetProcAddress("glCreateShader"));
            glShaderSource = (void (__stdcall*)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length))((void*)wglGetProcAddress("glShaderSource"));
            glCompileShader = (void (__stdcall*)(GLuint shader))((void*)wglGetProcAddress("glCompileShader"));
            glGetShaderiv = (void (__stdcall*)(GLuint shader, GLenum pname, GLint* params))((void*)wglGetProcAddress("glGetShaderiv"));
            glGetShaderInfoLog = (void (__stdcall*)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog))((void*)wglGetProcAddress("glGetShaderInfoLog"));
            glCreateProgram = (GLuint(__stdcall*)(void))((void*)wglGetProcAddress("glCreateProgram"));
            glAttachShader = (void (__stdcall*)(GLuint program, GLuint shader))((void*)wglGetProcAddress("glAttachShader"));
            glLinkProgram = (void (__stdcall*)(GLuint program))((void*)wglGetProcAddress("glLinkProgram"));
            glGetProgramiv = (void (__stdcall*)(GLuint program, GLenum pname, GLint* params))((void*)wglGetProcAddress("glGetProgramiv"));
            glGetProgramInfoLog = (void (__stdcall*)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog))((void*)wglGetProcAddress("glGetProgramInfoLog"));
            glDeleteShader = (void (__stdcall*)(GLuint shader))((void*)wglGetProcAddress("glDeleteShader"));
            glUseProgram = (void(__stdcall*)(GLuint program))((void*)wglGetProcAddress("glUseProgram"));
            glGenBuffers = (void(__stdcall*)(GLsizei n, GLuint * buffers))((void*)wglGetProcAddress("glGenBuffers"));
            glBindBuffer = (void(__stdcall*)(GLenum target, GLuint buffer))((void*)wglGetProcAddress("glBindBuffer"));
            glBufferData = (void(__stdcall*)(GLenum target, GLsizeiptr size, const void* data, GLenum usage))((void*)wglGetProcAddress("glBufferData"));
            glVertexAttribPointer = (void(__stdcall*)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer))((void*)wglGetProcAddress("glVertexAttribPointer"));
            glEnableVertexAttribArray = (void(__stdcall*)(GLuint index))((void*)wglGetProcAddress("glEnableVertexAttribArray"));
        	glGetUniformLocation = (GLint(__stdcall *)(GLuint program, const GLchar *name))((void*)wglGetProcAddress("glGetUniformLocation"));
        	glUniformMatrix4fv = (void(__stdcall *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))((void*)wglGetProcAddress("glUniformMatrix4fv"));
        	glUniform3fv = (void(__stdcall *)( GLint location, GLsizei count, const GLfloat *value))((void*)wglGetProcAddress("glUniform3fv"));
        	glUniform4fv = (void(__stdcall *)( GLint location, GLsizei count, const GLfloat *value))((void*)wglGetProcAddress("glUniform4fv"));
        } break;
        case 1:
            printf("Compiled Shaders NOT SUPPORTED!\n");
            break;
        default:
            printf("UNKNOWN OPENGL MAJOR VERSION");
    }
    return;
}

inline GLuint LoadShaderFromString(const char* shaderString, GLenum shaderType, const char* shaderName)
{
	GLuint shaderDescriptor = glCreateShader(shaderType);
    glShaderSource(shaderDescriptor, 1, (const GLchar**) &shaderString, NULL); 
    glCompileShader(shaderDescriptor);
    GLint success;
    glGetShaderiv(shaderDescriptor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderDescriptor, 1024, NULL, infoLog);
        FILE * file = fopen("GLERROR.txt","w");
        fprintf(file,"ERROR SHADER_COMPILATION_ERROR of type: %s\n%s\n", shaderName, infoLog);
        fclose(file);
        exit(-1);
    }
    return shaderDescriptor;
}

/* don't forget to use after compiling*/
inline GLuint compileShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader); 
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf("ERROR PROGRAM_LINKING_ERROR of type: PROGRAM\n%s\n",infoLog);
        exit(-1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

struct Vertex
{
    Vec3 vPos;
    Vec3 vNormal;
};

//https://github.com/yosmo78/Win32OpenGL-FPSCamera
void LoadMeshes()
{
    const uint32_t NUM_MODELS = 5;


    




    GLuint VBOs[NUM_MODELS];
    GLuint EBOs[NUM_MODELS];
    glGenVertexArrays(2, modelVAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(2, EBOs);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(modelVAOs[0]); //can only bind one VAO at a time

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(??), ??, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // norm attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //FOR EAO's
    // The index buffer binding is stored within the VAO. If no VAO is bound, then you cannot bind a buffer object to GL_ELEMENT_ARRAY_BUFFER (this might not be true, so verify). Meaning GL_ELEMENT_ARRAY_BUFFER is not global like GL_ARRAY_BUFFER 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndicies), cubeIndicies, GL_STATIC_DRAW);

    glBindVertexArray(modelVAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

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
#if MAIN_DEBUG
	if( CreateOpenGLWindow() )
#else
    if( CreateOpenGLWindow( Instance ) )
#endif
    {
        return -1;
    }
    loadGLFuncPtrs();

    LoadMeshes();


    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    int64_t PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
    LARGE_INTEGER LastCounter;
    QueryPerformanceCounter(&LastCounter);

    Running = 1;

    const char vertexShader[] =
    #include "vertShader.glsl"
    ;

    const char fragmentShader[] = 
    #include "fragShader.glsl"
    ;

    GLuint vertShaderID = LoadShaderFromString(vertexShader,GL_VERTEX_SHADER,"Vertex Shader");
    GLuint fragShaderID = LoadShaderFromString(fragmentShader,GL_FRAGMENT_SHADER,"Fragment Shader");
    GLuint shaderProgramID = compileShaderProgram(vertShaderID,fragShaderID);

    while( Running )
    {
        uint64_t EndCycleCount = __rdtsc();
    
        LARGE_INTEGER EndCounter;
        QueryPerformanceCounter(&EndCounter);
    
        //Display the value here
        int64_t CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
        float deltaTime = (CounterElapsed / (float)PerfCountFrequency);
        double MSPerFrame = (double) ((1000.0f*CounterElapsed)/ (double)PerfCountFrequency);
        double FPS = PerfCountFrequency/(double)CounterElapsed;
        LastCounter = EndCounter;
    
#if MAIN_DEBUG
        char buf[55];
        sprintf(&buf[0],"The Button: fps %f",FPS);
        SetWindowTextA( WindowHandle, &buf[0]);
#endif
        MSG Message;eveexsh
        
        while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
        	switch(Message.message)
        	{
        		case WM_QUIT:
        		{
        			CloseProgram();
        			break;
        		}
        		case WM_SYSKEYDOWN:
          		case WM_SYSKEYUP:
          		case WM_KEYDOWN:
          		case WM_KEYUP:
          		{
          			uint32_t VKCode = (uint32_t) Message.wParam;
              		bool WasDown = (Message.lParam & (1<<30)) != 0;
              		bool IsDown = (Message.lParam & (1<<31)) == 0;
              		bool AltKeyWasDown = (Message.lParam & (1 << 29));
              		switch(VKCode)
              		{
              			case VK_F4:
              			{
              				if(AltKeyWasDown)
              				{
              					CloseProgram();
              				}
              				break;
              			}
              			default:
              			{
              				break;
              			}
              		}
          			break;
          		}
        		default:
        		{
        			TranslateMessage(&Message);
            		DispatchMessage(&Message);
            		break;
        		}
        	}
        }


        //bind default shaders
        glUseProgram(shaderProgramID);



        SwapBuffers(hDC);
    }


	return 0;
}