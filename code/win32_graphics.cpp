// Chandler Carruthi - Compiler Author 

#include <windows.h>
#include <stdint.h>

#include "sd_sphere.h"

// Differentiate between static variable types
#define internal static 
#define local_persist static 
#define global_variable static 

typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int32_t int32;
typedef int64_t int64;

struct win32_offscreen_buffer
{
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};

// TODO(nial): These are global only for now
global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackbuffer;

struct win32_window_dimension
{
	int Width;
	int Height;
};

global_variable float GlobalWidth;
global_variable float GlobalHeight;

win32_window_dimension 
Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Width = ClientRect.right - ClientRect.left;
	Result.Height = ClientRect.bottom - ClientRect.top;

	return(Result);
}

internal void
RenderGradient(win32_offscreen_buffer Buffer, float angle)
{
	// Define primitive shape 
	Sphere sphere = {0.0, 0.0, 12.0, 5.0};

	vec3 camera_position = {0.0, 0.0, 0.0};
	vec3 light_position = {50.0, 50.0, -5.0};
	vec3 point_in_space = {};

	float field_of_view = 3.14 / 4.0;

	// TODO(nial): Let's see what the optimizer does
	uint8 *Row = (uint8 *)Buffer.Memory; 
	for (int Y = 0; Y < Buffer.Height; ++Y)
	{
		uint32 *Pixel = (uint32 *)Row;
		for (int X = 0; X < Buffer.Width; ++X)
		{
			float X_norm = ((X/GlobalWidth) * 2.0) - 1.0;
			float Y_norm = ((Y/GlobalHeight) * -2.0) + 1.0;
			X_norm = X_norm * (GlobalWidth/GlobalHeight);

			vec2 uv = {X_norm, Y_norm};	// place_holder for the time being 
			Ray ray = {camera_position, ray_direction(field_of_view, uv)};

			float dist = shortest_distance_to_surface(&sphere, ray.origin, ray.direction, 
													  &point_in_space, MIN_DIST, MAX_DIST, angle);
			if (dist > MAX_DIST - EPSILON)
			{
				//printf("%s\n", "Background");
				uint8 Red = 192;
				uint8 Green = 192;
				uint8 Blue = 192;
				
				*Pixel++ = (Red << 16) | (Green << 8) | Blue;
			}

			else
			{
				//printf("%s\n", "ItsAHit");

				/*
				point_in_space.x = ray.direction.x * dist;
				point_in_space.y = ray.direction.y * dist;
				point_in_space.z = ray.direction.z * dist;

				float x = point_in_space.x + ray.origin.x;
				float y = point_in_space.y + ray.origin.y;
				float z = point_in_space.z + ray.origin.z;

				point_in_space = {x, y, z};
				*/

				vec3 normal = estimate_normal(&sphere, point_in_space);
				float intensity = diffuse(point_in_space, light_position, normal);

				int value = (int)(255 * intensity);
				uint8 Red = value;
				uint8 Green = value;
				uint8 Blue = value;

				*Pixel++ = (Red << 16) | (Green << 8) | Blue;
			}

			/*
			Pixel in memory: 00 00 00 00 -> RR GG BB xx
			in Little Endian 
			uint8 Red;
			uint8 Blue = (X + XOffset);
			uint8 Green = (Y + YOffset);
			*Pixel++ = ((Green << 8) | Blue);	
			*/
		}

		Row += Buffer.Pitch;
	}
}

// DIB = Device Independent Bitmap
internal void 
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	// TODO(nial): Bulletproof this.
	// Maybe don't free first, free after, then free first if that fails.

	if (Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->BytesPerPixel = 4;

	// NOTE(nial): When the biHeight field is negative, this is the clue to
	// Windows to treat this bitmap as top-down, not bottom-up, meaning that
	// the first three bytes of the image are the color for the top-left pixel
	// in the bitmap, not the bottom left.
	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // top down DIB
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32; // 8 bits of padding 
	Buffer->Info.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (Buffer->Width * Buffer->Height)*Buffer->BytesPerPixel;
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	// TODO(nial): Probably want to clear this to black

	Buffer->Pitch = Buffer->Width*Buffer->BytesPerPixel;

}

internal void
Win32DisplayBufferInWindow(HDC DeviceContext, 
						   int WindowWidth, int WindowHeight, 
						   win32_offscreen_buffer Buffer,
						   int X, int Y, int Width,  int Height)
{
	// TODO(nial): Aspect ratio correction
	// TODO(nial): Play with stretch modes 
	StretchDIBits(DeviceContext, 
				 /*
				 X, Y, Width, Height,
				 X, Y, Width, Height,
				 */
				 0, 0, WindowWidth, WindowHeight,
				 0, 0, Buffer.Width, Buffer.Height,
				 Buffer.Memory,
				 &Buffer.Info, 
				 DIB_RGB_COLORS, SRCCOPY);
}


LRESULT CALLBACK 
Win32MainWindowCallback(HWND Window, 
						UINT Message, 
						WPARAM WParam, 
						LPARAM LParam) 
{
	LRESULT Result = 0;

	switch(Message)
	{
		case WM_SIZE:
		{
			OutputDebugString("WM_SIZE\n");
			break;
		} 

		case WM_DESTROY:
		{
			// TODO(nial): Handle this with a messsage to the user?
			GlobalRunning = false;
			break;
		} 
		
		case WM_CLOSE:
		{
			// TODO(nial): Handle this as an error - recreate window?Î
			GlobalRunning = false;
			break;
		} 

		case WM_ACTIVATEAPP:
		{
			OutputDebugString("WM_ACTIVATEAPP\n");
			break;
		} 

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, 
									   GlobalBackbuffer, X, Y, Dimension.Width, Dimension.Height);
			break;
		}
		
		default:
		{
			//OutputDebugString("default\n");
			Result = DefWindowProc(Window, Message, WParam, LParam);
			break;
		} 
	}

	return(Result);
}	

int CALLBACK
WinMain(HINSTANCE Instance,
		HINSTANCE PrevInstance,
		LPSTR CommandLine,
		int  ShowCode)
{
	//MessageBoxA(0, "Come check me.", "Nail", MB_OK | MB_ICONINFORMATION);

	WNDCLASS WindowClass = {};

	GlobalWidth = 800.0;
	GlobalHeight = 450.0;
	Win32ResizeDIBSection(&GlobalBackbuffer, 800, 450);

	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
//	WindowClass.hIcon;
	WindowClass.lpszClassName = "WindowClass";

	if (RegisterClass(&WindowClass))
	{
		HWND Window = 
			CreateWindowEx(
				0,
				WindowClass.lpszClassName,
				"Graphics",
				WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				0,
				0,
				Instance,
				0);

		if (Window)			
		{
			// NOTE(nial): Since we specified CS_OWNDC, we can just
			// get one device context and use it forever because we 
			// are not sharing it with anyone. 
			HDC DeviceContext = GetDC(Window);

			float angle = 0;

			GlobalRunning = true;
			while(GlobalRunning)
			{
				MSG Message;
				while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if (Message.message == WM_QUIT)
					{
						GlobalRunning = false;
					}

					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}
				
				RenderGradient(GlobalBackbuffer, angle);
				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, 
									  	   GlobalBackbuffer, 0, 0, Dimension.Width, Dimension.Height);
				angle += 0.01;
			}
		}

		else
		{
			// TODO(nial): Logging
		}
			
	}

	else
	{
		// TODO(nial): Logging
	}
		

	return(0);
}
