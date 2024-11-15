#include "PreCompile.h"
#include "EngineAPICore.h"

#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineSound.h>

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
	UEngineSound::Release();
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
	// 현재 레벨을 리셋할거야?
	if (true == IsCurLevelReset)
	{
		delete CurLevel; // 그럼 지워
		CurLevel = nullptr;
		IsCurLevelReset = false; 
		// 이제 리셋한 정보를 담은 NextLevel이 아래 과정을 거쳐 CurLevel이 된다.
	}

	if (nullptr != NextLevel)
	{
		// 레벨들을 왔다갔다 할 때가 있기 때문에
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel;

		NextLevel->LevelChangeStart();

		NextLevel = nullptr;

		// 델타타임이 지연될수 있으므로 델타타임을 초기화시켜주는것이 좋다.
		DeltaTimer.TimeStart();
	}

	// Delta Time
	DeltaTimer.TimeCheck();
	float DeltaTime = DeltaTimer.GetDeltaTime();
	//

	// 사운드 
	UEngineSound::Update();

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
	CurLevel->Render(DeltaTime);
	CurLevel->Collision(DeltaTime);

	// 틱돌고 렌더돌고 충돌돌고 릴리즈
	CurLevel->Release(DeltaTime);
}

void UEngineAPICore::OpenLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);
	
	std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(UpperName);
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	if (EndIter == FindIter)
	{
		MSGASSERT(UpperName + " 라는 이름의 레벨은 존재하지 않습니다.");
		return;
	}

	NextLevel = FindIter->second;
}



