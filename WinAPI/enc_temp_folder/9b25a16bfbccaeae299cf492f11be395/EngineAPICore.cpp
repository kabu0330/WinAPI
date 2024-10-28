#include "PreCompile.h"
#include "EngineAPICore.h"

#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineDelegate.h>

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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;

	// â Ŭ���� ���
	UEngineWindow::EngineWindowInit(_Inst);

	//UEngineWindow NewWindow;
	//NewWindow.Open();

	// ������ â ����
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

// 

void UEngineAPICore::EngineBeginPlay()
{
	UserCore->BeginPlay();
}

void UEngineAPICore::EngineTick()
{
	UserCore->Tick();
	MainCore->Tick();
}

void UEngineAPICore::Tick()
{
}

void UEngineAPICore::Render()
{
}

