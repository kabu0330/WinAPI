#pragma once
// user header
#include <EngineBase/EngineDelegate.h>
#include "EngineWinImage.h"
#include <EngineBase/EngineMath.h>

// 설명 : 윈도우 창 클래스 생성 및 윈도우 출력
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

	// window 창 이름 설정
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	void SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale);

	FVector2D GetMousePos();

protected:

private:
	// 내 프로그램의 주소 
	static HINSTANCE hInstance; 

	// 창을 여러 개 띄울거라면 창 클래스를 map에 저장
	static std::map<std::string, WNDCLASSEXA> WindowClasss;

	FVector2D WindowSize;

	
	UEngineWinImage* BackBufferImage  = nullptr; // HDC : 뒤에서 창에 그릴 그림을 그리는 화면
	UEngineWinImage* WindowImage = nullptr; // HDC : 진짜 창에 그려지는 화면

	HWND WindowHandle = nullptr; // 윈도우 창 주소(몇 번째 창)

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;
};

