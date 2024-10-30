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

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

// ������ ����ǰ� �� �� ���� ����ȴ�.
void ContentsCore::BeginPlay()
{
	// ������ Ÿ��Ʋ�� ���⼭ ���Ѵ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");


	// ������ ����� �ݵ�� ���ؾ� �Ѵ�.
	Global::WindowSize = { 1280, 720 };
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, Global::WindowSize);


	// ���� ���ҽ� �����´�.
	UEngineDirectory Dir;

	// ����� : ������� �� ���� �������Ͽ� ������ �� �� ���� ������ ��ΰ� �ٸ���.
	// �� ��, ���� � ��Ȳ������ �� ���ҽ� ������ ã�ư� �� �ֵ��� ����� ������ �Ѵ�.
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}

	UImageManager::GetInst().CuttingSprite("icon.png", { 1024, 1024 });
	UImageManager::GetInst().CuttingSprite("SampleMap(848,536).png", { 848, 536 });

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

