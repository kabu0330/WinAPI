#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>

// �� �̻� ������ ������� �ʰ� pragma comment�� ���̺귯���� �������Ͽ� ����
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