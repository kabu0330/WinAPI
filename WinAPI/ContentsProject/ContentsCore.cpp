#include "PreCompile.h"
#include "ContentsCore.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>

#include "PlayGameMode.h"
#include "TitleGameMode.h"

#include "Global.h"
#include "Player.h"
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
	Global::WindowSize = { 960, 538 };
	Global::WindowScale = Global::WindowSize;
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
	
	SpriteSetting(); // �̹��� �� �忡 ���ִ� ��������Ʈ �ڸ���(.png / .bmp)
	FolderSetting(); // ���� ���� �ȿ� ���ִ� �̹������� ��������Ʈ�� ���� (����)

	// ������ �����ϰ� ���� ȭ�鿡 ��µ� ������ �����Ѵ�.
	LevelSetting();
}

void ContentsCore::LevelSetting()
{
	// ������ �����Ѵ�.
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");
	//UEngineAPICore::GetCore()->CreateLevel("End");

	// ���� ����� ������ �����Ѵ�.
	//UEngineAPICore::GetCore()->OpenLevel("Title");
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

void ContentsCore::SpriteSetting()
{
	// �̹��� �� �忡�� ��������Ʈ�� �� ���� ���� ������ �ڸ���.
	UImageManager::GetInst().CuttingSprite("Body.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("Head.png", 5, 2);
	UImageManager::GetInst().CuttingSprite("NormalRoomDoor.png", 4, 2);

}

void ContentsCore::FolderSetting()
{
	UEngineDirectory TitleDir;
	TitleDir.MoveParentToDirectory("Resources");
	TitleDir.Append("Title");

	UImageManager::GetInst().LoadFolder(TitleDir.GetPathToString());
}

void ContentsCore::Tick()
{
	// ���� ��� �뵵�� ����
}
