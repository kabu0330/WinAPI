#pragma once
// user header
#include <EngineBase/EngineDelegate.h>

// 설명 : 윈도우 창 클래스 생성 및 윈도우 출력
class UEngineWindow
{
public:
	static void EngineWindowInit(HINSTANCE _Instance);
	static void CreateWindowClass(const WNDCLASSEXA& _Class);

	static int WindowMessageLoop(EngineDelegate _FrameFuction, EngineDelegate _FrameFunction);

	// constrcuter destructer
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;
	//

	void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");

	void Open(std::string_view _TitleName = "Window");

	inline HDC GetBackBuffer()
	{
		return BackBuffer;
	}

protected:

private:
	// 내 프로그램의 주소 
	static HINSTANCE hInstance; 

	// 창을 여러 개 띄울거라면 창 클래스를 map에 저장
	static std::map<std::string, WNDCLASSEXA> WindowClasss;

	HDC BackBuffer = nullptr;
	HWND WindowHandle = nullptr; // 윈도우 창 주소(몇 번째 창)
};

