#pragma once
// user header
#include <EngineBase/EngineDelegate.h>

// ���� : ������ â Ŭ���� ���� �� ������ ���
class UEngineWindow
{
public:
	static void EngineWindowInit(HINSTANCE _Instance);
	static void CreateWindowClass(const WNDCLASSEXA& _Class);

	static int WindowMessageLoop(EngineDelegate _FrameFuction);

	// constrcuter destructer
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");

	void Open(std::string_view _TitleName = "Window");

protected:

private:
	// �� ���α׷��� �ּ� 
	static HINSTANCE hInstance; 

	// â�� ���� �� ���Ŷ�� â Ŭ������ map�� ����
	static std::map<std::string, WNDCLASSEXA> WindowClasss;

	HWND WindowHandle = nullptr; // ������ â �ּ�(�� ��° â)
};

