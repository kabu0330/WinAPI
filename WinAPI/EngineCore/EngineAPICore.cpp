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
	// â Ŭ���� ���
	UEngineWindow::EngineWindowInit(_Inst);

	//UEngineWindow NewWindow;
	//NewWindow.Open();

	UEngineAPICore Core;

	// ������ â ����
	Core.EngineMainWindow.Open(); 
	
	// �� �Լ� �����ͷ� ���� ������ ������?
	// �޽��� ó�� �Լ��� EngineWindow::WindowMessageLoop �Լ��̴�.
	// ���� Ŭ������ ���� Ŭ������ ���縦 �˾Ƽ��� �ȵȴٴ� ��Ģ�� ���� (��� ��ȯ ���� ����)
	// EngineWindow�� UEngineAPICore ��ü�� ������ �ʰ� UEngineAPICore::EngineLoop �Լ��� ������ �޽��� �������� ������ ����
	// ��ü�� ���� ���� �Լ��� �Ѱܼ� �����Ű�� ���� �Լ� �����͸� �̿��� ����� �����ߴ�.
	// �� �ٸ� ������� ������(���)�� �̿��ϴ� ����� �ִ�.
	EngineDelegate NewDel;
	NewDel = EngineLoop;

	// �޽��� ���� ���� -> ���� ������ ��������
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

