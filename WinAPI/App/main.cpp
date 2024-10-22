#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>

// 더 이상 참조를 사용하지 않고 pragma comment로 라이브러리를 실행파일에 포함
#pragma comment(lib, "EnginePlatform.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UEngineWindow::EngineWindowInit(hInstance);
	
	UEngineWindow NewWindow;
	NewWindow.Open();

	return UEngineWindow::WindowMessageLoop();
}