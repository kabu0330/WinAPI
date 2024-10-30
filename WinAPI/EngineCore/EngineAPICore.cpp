#include "PreCompile.h"
#include "EngineAPICore.h"

#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>

UEngineAPICore* UEngineAPICore::MainCore = nullptr;
UContentsCore* UEngineAPICore::UserCore = nullptr;

UEngineAPICore::UEngineAPICore()
{
    MainCore = this;
}

UEngineAPICore::~UEngineAPICore()
{
	std::map<std::string, class ULevel*>::iterator StartIter = Levels.begin();
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Levels.clear();
}

int UEngineAPICore::EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore)
{
	// 메모리 누수 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;

	// 창 클래스 등록
	UEngineWindow::EngineWindowInit(_Inst);

	//UEngineWindow NewWindow;
	//NewWindow.Open();

	// 윈도우 창 생성, 엔진을 관리할 엔진코어 생성
	UEngineAPICore Core = UEngineAPICore();
	Core.EngineMainWindow.Open(); 
	
	// 왜 함수 포인터로 엔진 루프를 돌리냐?
	// 메시지 처리 함수가 EngineWindow::WindowMessageLoop 함수이다.
	// 상위 클래스는 하위 클래스의 존재를 알아서는 안된다는 규칙에 따라 (헤더 순환 참조 문제)
	// EngineWindow에 UEngineAPICore 객체를 만들지 않고 UEngineAPICore::EngineLoop 함수를 윈도우 메시지 루프에서 돌리기 위해
	// 객체의 생성 없이 함수만 넘겨서 실행시키기 위해 함수 포인터를 이용한 방법을 선택했다.
	// 또 다른 방법으론 가상함수를 이용하는 방법도 있다.
	EngineDelegate Start = EngineDelegate(std::bind(EngineBeginPlay));
	EngineDelegate FrameLoop = EngineDelegate(std::bind(EngineTick));;

	return UEngineWindow::WindowMessageLoop(Start, FrameLoop);
}

// 게임 시작 시 딱 한번만 실행될 것들
void UEngineAPICore::EngineBeginPlay()
{
	// 1. 컨텐츠 중심 윈도우 세팅 재설정
	// 2. 리소스 세팅
	// 3. 레벨 세팅
	UserCore->BeginPlay();
}

// 프레임 : EngineTick 함수가 1초에 몇 번 실행되냐
void UEngineAPICore::EngineTick()
{
	UserCore->Tick(); // 컨텐츠 코어는 현재 사용처가 없음
	MainCore->Tick(); // DeltaTime, 레벨의 Tick과 Render 
}

void UEngineAPICore::Tick()
{
	// Delta Time
	DeltaTimer.TimeCheck();
	float DeltaTime = DeltaTimer.GetDeltaTime();
	//

	// 입력 체크
	UEngineInput::GetInst().KeyCheck(DeltaTime);

	// 입력 이벤트 체크
	UEngineInput::GetInst().EventCheck(DeltaTime);

	if (nullptr == CurLevel)
	{
		MSGASSERT("엔진 코어에 현재 레벨이 지정되지 않았습니다.");
		return;
	}

	CurLevel->Tick(DeltaTime);
	CurLevel->Render();
}

void UEngineAPICore::OpenLevel(std::string_view _LevelName)
{
	std::string ChangeName = _LevelName.data();
	
	std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(ChangeName);
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	if (EndIter == FindIter)
	{
		MSGASSERT(ChangeName + " 라는 이름의 레벨은 존재하지 않습니다.");
		return;
	}

	CurLevel = FindIter->second;
}



