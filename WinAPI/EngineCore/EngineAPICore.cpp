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
	// �޸� ���� üũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;

	// â Ŭ���� ���
	UEngineWindow::EngineWindowInit(_Inst);

	//UEngineWindow NewWindow;
	//NewWindow.Open();

	// ������ â ����, ������ ������ �����ھ� ����
	UEngineAPICore Core = UEngineAPICore();
	Core.EngineMainWindow.Open(); 
	
	// �� �Լ� �����ͷ� ���� ������ ������?
	// �޽��� ó�� �Լ��� EngineWindow::WindowMessageLoop �Լ��̴�.
	// ���� Ŭ������ ���� Ŭ������ ���縦 �˾Ƽ��� �ȵȴٴ� ��Ģ�� ���� (��� ��ȯ ���� ����)
	// EngineWindow�� UEngineAPICore ��ü�� ������ �ʰ� UEngineAPICore::EngineLoop �Լ��� ������ �޽��� �������� ������ ����
	// ��ü�� ���� ���� �Լ��� �Ѱܼ� �����Ű�� ���� �Լ� �����͸� �̿��� ����� �����ߴ�.
	// �� �ٸ� ������� �����Լ��� �̿��ϴ� ����� �ִ�.
	EngineDelegate Start = EngineDelegate(std::bind(EngineBeginPlay));
	EngineDelegate FrameLoop = EngineDelegate(std::bind(EngineTick));;

	return UEngineWindow::WindowMessageLoop(Start, FrameLoop);
}

// ���� ���� �� �� �ѹ��� ����� �͵�
void UEngineAPICore::EngineBeginPlay()
{
	// 1. ������ �߽� ������ ���� �缳��
	// 2. ���ҽ� ����
	// 3. ���� ����
	UserCore->BeginPlay();
}

// ������ : EngineTick �Լ��� 1�ʿ� �� �� ����ǳ�
void UEngineAPICore::EngineTick()
{
	UserCore->Tick(); // ������ �ھ�� ���� ���ó�� ����
	MainCore->Tick(); // DeltaTime, ������ Tick�� Render 
}

void UEngineAPICore::Tick()
{
	// Delta Time
	DeltaTimer.TimeCheck();
	float DeltaTime = DeltaTimer.GetDeltaTime();
	//

	// �Է� üũ
	UEngineInput::GetInst().KeyCheck(DeltaTime);

	// �Է� �̺�Ʈ üũ
	UEngineInput::GetInst().EventCheck(DeltaTime);

	if (nullptr == CurLevel)
	{
		MSGASSERT("���� �ھ ���� ������ �������� �ʾҽ��ϴ�.");
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
		MSGASSERT(ChangeName + " ��� �̸��� ������ �������� �ʽ��ϴ�.");
		return;
	}

	CurLevel = FindIter->second;
}



