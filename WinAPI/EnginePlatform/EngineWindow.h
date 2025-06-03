#pragma once
// user header
#include <EngineBase/EngineDelegate.h>
#include "EngineWinImage.h"
#include <EngineBase/EngineMath.h>

// ���� : ������ â Ŭ���� ���� �� ������ ���
class UEngineWindow
{
public:
	static void EngineWindowInit(HINSTANCE _Instance);
	static void CreateWindowClass(const WNDCLASSEXA& _Class);

	static int WindowMessageLoop(EngineDelegate _FrameFuction, EngineDelegate _FrameFunction);

	UEngineWindow();
	~UEngineWindow();

	void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");

	void Open(std::string_view _TitleName = "Window");

	inline FVector2D GetWindowSize() const
	{
		return WindowSize;
	}

	inline UEngineWinImage* GetWindowImage() const
	{
		return WindowImage;
	}

	inline UEngineWinImage* GetBackBuffer() const
	{
		return BackBufferImage;
	}

	// window â �̸� ����
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	void SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale);

	FVector2D GetMousePos();

protected:

private:
	// �� ���α׷��� �ּ� 
	static HINSTANCE hInstance; 

	// â�� ���� �� ���Ŷ�� â Ŭ������ map�� ����
	static std::map<std::string, WNDCLASSEXA> WindowClasss;

	FVector2D WindowSize;

	
	UEngineWinImage* BackBufferImage  = nullptr; // HDC : �ڿ��� â�� �׸� �׸��� �׸��� ȭ��
	UEngineWinImage* WindowImage = nullptr; // HDC : ��¥ â�� �׷����� ȭ��

	HWND WindowHandle = nullptr; // ������ â �ּ�(�� ��° â)

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;
};

