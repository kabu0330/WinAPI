#include "PreCompile.h"
#include "EngineAPICore.h"

UEngineAPICore* UEngineAPICore::MainCore = nullptr;

UEngineAPICore::UEngineAPICore()
{
    MainCore = this;
}

UEngineAPICore::~UEngineAPICore()
{
}

int UEngineAPICore::EngineStart(HINSTANCE _Inst)
{
	// 창 클래스 등록
	UEngineWindow::EngineWindowInit(_Inst);

	//UEngineWindow NewWindow;
	//NewWindow.Open();

	UEngineAPICore Core;

	// 윈도우 창 생성
	Core.EngineMainWindow.Open(); 
	
	// 왜 함수 포인터로 엔진 루프를 돌리냐?
	// 메시지 처리 함수가 EngineWindow::WindowMessageLoop 함수이다.
	// 상위 클래스는 하위 클래스의 존재를 알아서는 안된다는 규칙에 따라 (헤더 순환 참조 문제)
	// EngineWindow에 UEngineAPICore 객체를 만들지 않고 UEngineAPICore::EngineLoop 함수를 윈도우 메시지 루프에서 돌리기 위해
	// 객체의 생성 없이 함수만 넘겨서 실행시키기 위해 함수 포인터를 이용한 방법을 선택했다.
	// 또 다른 방법으론 다형성(상속)을 이용하는 방법도 있다.
	EngineDelegate NewDel;
	NewDel = EngineLoop;

	// 메시지 무한 루프 -> 게임 프레임 돌릴거임
	return UEngineWindow::WindowMessageLoop(NewDel);
}

void UEngineAPICore::EngineLoop()
{
	MainCore->Tick();
	MainCore->Render();
}

void UEngineAPICore::Tick()
{
}

void UEngineAPICore::Render()
{
}

