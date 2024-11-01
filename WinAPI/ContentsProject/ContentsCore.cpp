#include "PreCompile.h"
#include "Player.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>

#include "ContentsCore.h"
#include "PlayGameMode.h"
#include "Global.h"
#include "Room.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

// ������ ����ǰ� �� �� ���� ����ȴ�.
void ContentsCore::BeginPlay()
{
	// ���� ������ ����
	// 1. ���� ���� Core ������ �޸� �ּ� ����(GetCore)
	// 2. â Ŭ���� ���� �� ������ â ����, ���� HDC ����
	
	// 1. ������ Ÿ��Ʋ�� ���⼭ ���Ѵ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");


	// 2. ������ ����� �ݵ�� ���ؾ� �Ѵ�.
	Global::WindowSize = { 1280, 720 };

	// 3. ������ ����� �缳���ϴ� �������� ���� HDC�� �ƴ� �ڿ��� �׸��� �׸� BackBuffer�� �����Ѵ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, Global::WindowSize);


	// 4. ���� ���ҽ� �����´�.
	// Directory Ŭ������ �����ڸ� ���� ���� ���� ���� ��θ� EnginePath�� Path ����� �����Ѵ�.
	UEngineDirectory Dir;

	// ����� : ������� �� ���� �������Ͽ� ������ �� �� ���� ������ ��ΰ� �ٸ���.
	// �� ��, ���� � ��Ȳ������ �� ���ҽ� ������ ã�ư� �� �ֵ��� ����� ������ �Ѵ�.
	// ���� ���ҽ� ������ �����ϰ� �ִ� �������� ���´�.
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(/*true*/); 
	// ����Ʈ �Ű������� true�̸�, ���� ���丮�� ���ҽ��� �����´�.

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();

		// Load �Լ� : �̹����� ���ϸ��� Key / HDC�� ��ġ, ũ�⸦ ������ �������ִ� �Լ�
		UImageManager::GetInst().Load(FilePath);
	}
	
	// �̹��� �� �忡�� ��������Ʈ�� ���� ũ�Ⱑ ������
	//UImageManager::GetInst().CuttingSprite("icon.png", { 1024, 1024 });
	//UImageManager::GetInst().CuttingSprite("SampleMap(848,536).png", { 848, 536 });
	UImageManager::GetInst().CuttingSprite("Body.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("Head.png", 5, 2);


	//{

	//	UEngineDirectory BombDir;
	//	BombDir.MoveParentToDirectory("Resources");
	//	BombDir.Append("bomb");

	//	UImageManager::GetInst().LoadFolder(BombDir.GetPathToString());

	//}



	// ������ �����Ѵ�.
	//UEngineAPICore::GetCore()->CreateLevel("Title");
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");
	//UEngineAPICore::GetCore()->CreateLevel("End");

	// ���� ����� ������ �����Ѵ�.
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

void ContentsCore::Tick()
{
	// ���� ��� �뵵�� ����
}

