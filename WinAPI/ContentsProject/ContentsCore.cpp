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

void ContentsCore::WindowSetting()
{
	// 1. ������ Ÿ��Ʋ�� ���⼭ ���Ѵ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");


	// 2. ������ ����� �ݵ�� �����ؾ� �Ѵ�.
	Global::WindowSize = { 960, 540 };
	Global::WindowHalfScale = Global::WindowSize.Half();
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, Global::WindowSize);
}

// ������ ����ǰ� �� �� ���� ����ȴ�.
void ContentsCore::BeginPlay()
{
	// ���� ������ ����
	// 1. ���� ���� Core ������ �޸� �ּ� ����(GetCore)
	// 2. â Ŭ���� ���� �� ������ â ����, ���� HDC ����
	
	WindowSetting();

	// 3. ���� ���ҽ� �����´�.
	ResourceLoad();
	
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
	UEngineAPICore::GetCore()->OpenLevel("Title");
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

void ContentsCore::SpriteSetting()
{
	// �̹��� �� �忡�� ��������Ʈ�� �� ���� ���� ������ �ڸ���. (����, ����)
	// Player
	UImageManager::GetInst().CuttingSprite("Body.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("Head.png", 5, 2);
	UImageManager::GetInst().CuttingSprite("Isaac.png", 4, 3);

	UImageManager::GetInst().CuttingSprite("effect_tearpoofa.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("effect_bloodtear.png", 4, 4);

	// Monster
	UImageManager::GetInst().CuttingSprite("Fly.png", 5, 3);
	UImageManager::GetInst().CuttingSprite("Fly002.png", 6, 2);

	// Background
	UImageManager::GetInst().CuttingSprite("NormalRoomDoor.png", 4, 2);

	// UI
	UImageManager::GetInst().CuttingSprite("pickup.png", 10, 1); // (10, 12)
	UImageManager::GetInst().CuttingSprite("banner.png", 26, 1); // (12, 10)
	UImageManager::GetInst().CuttingSprite("hearts.png", 5, 2); // (80, 32)
	UImageManager::GetInst().CuttingSprite("ui_crafting.png", 8, 4); // (128, 64)

}

void ContentsCore::FolderSetting()
{
	UEngineDirectory TitleDir;
	TitleDir.MoveParentToDirectory("Resources");
	TitleDir.Append("Title");
	UImageManager::GetInst().LoadFolder(TitleDir.GetPathToString());

	UEngineDirectory DoorLockSprite;
	DoorLockSprite.MoveParentToDirectory("Resources//Play");
	DoorLockSprite.Append("NormalDoor");
	UImageManager::GetInst().LoadFolder(DoorLockSprite.GetPathToString());

	UEngineDirectory DoorOpenSprite;
	DoorOpenSprite.MoveParentToDirectory("Resources//Play");
	DoorOpenSprite.Append("OpenDoor");
	UImageManager::GetInst().LoadFolder(DoorOpenSprite.GetPathToString());
}

void ContentsCore::ResourceLoad()
{
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
}

void ContentsCore::Tick()
{
	// ���� ��� �뵵�� ����
}

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}