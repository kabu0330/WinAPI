#include "PreCompile.h"
#include "ContentsCore.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineSound.h>
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
	// Monster
	UImageManager::GetInst().CuttingSprite("Fly.png", 5, 3);
	UImageManager::GetInst().CuttingSprite("Fly002.png", 6, 2);
	UImageManager::GetInst().CuttingSprite("Hopper.png", 12, 1);
	UImageManager::GetInst().CuttingSprite("Host.png", 3, 1);
	UImageManager::GetInst().CuttingSprite("Dip.png", 3, 2);
	UImageManager::GetInst().CuttingSprite("Pooter_Left.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("Pooter_Right.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("duke.png", 2, 2);

	// MonsterEffect
	UImageManager::GetInst().CuttingSprite("effect_bloodpoof.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("LargeBloodExplosion.png", 5, 2);
	UImageManager::GetInst().CuttingSprite("SpawnEffect_Large.png", 5, 3);
	UImageManager::GetInst().CuttingSprite("effect_black_dust.png", 3, 4);
	UImageManager::GetInst().CuttingSprite("effect_dust.png", 3, 4);

	// MapObject
	UImageManager::GetInst().CuttingSprite("rocks.png", 4, 8);
	UImageManager::GetInst().CuttingSprite("poops.png", 5, 5);
	UImageManager::GetInst().CuttingSprite("fire_top.png", 3, 2);
	UImageManager::GetInst().CuttingSprite("fire_bottom.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("effect_bloodpool.png", 4, 6);

	// Item
	UImageManager::GetInst().CuttingSprite("bomb.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("bomb_spark.png", 4, 2);
	UImageManager::GetInst().CuttingSprite("hot_flames.png", 5, 2);
	UImageManager::GetInst().CuttingSprite("Explosion.png", 5, 3);


	// Player
	UImageManager::GetInst().CuttingSprite("Body.png", 5, 6);
	UImageManager::GetInst().CuttingSprite("Head.png", 5, 2);
	UImageManager::GetInst().CuttingSprite("Isaac.png", 4, 3);
	UImageManager::GetInst().CuttingSprite("Death_Head.png", 2, 1);
	UImageManager::GetInst().CuttingSprite("Death_Body.png", 5, 1);

	UImageManager::GetInst().CuttingSprite("effect_tearpoofa.png", 4, 4);
	UImageManager::GetInst().CuttingSprite("effect_bloodtear.png", 4, 4);


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
	// TitleGameMode
	UEngineDirectory TitleDir;
	TitleDir.MoveParentToDirectory("Resources//Image");
	TitleDir.Append("Title");
	UImageManager::GetInst().LoadFolder(TitleDir.GetPathToString());

	// PlayGameMode
	// 1. Loading
	UEngineDirectory PlayModeLoading;
	PlayModeLoading.MoveParentToDirectory("Resources//Image//Play");
	PlayModeLoading.Append("PlayModeLoading");
	UImageManager::GetInst().LoadFolder(PlayModeLoading.GetPathToString());

	// 2. Playing
	UEngineDirectory DoorLockSprite;
	DoorLockSprite.MoveParentToDirectory("Resources//Image//Play");
	DoorLockSprite.Append("NormalDoor");
	UImageManager::GetInst().LoadFolder(DoorLockSprite.GetPathToString());

	UEngineDirectory DoorOpenSprite;
	DoorOpenSprite.MoveParentToDirectory("Resources//Image//Play");
	DoorOpenSprite.Append("OpenDoor");
	UImageManager::GetInst().LoadFolder(DoorOpenSprite.GetPathToString());

	UEngineDirectory ItemHeart;
	ItemHeart.MoveParentToDirectory("Resources//Image//Play//Item");
	ItemHeart.Append("Heart");
	UImageManager::GetInst().LoadFolder(ItemHeart.GetPathToString());

	UEngineDirectory ItemHalfHeart;
	ItemHalfHeart.MoveParentToDirectory("Resources//Image//Play//Item");
	ItemHalfHeart.Append("HalfHeart");
	UImageManager::GetInst().LoadFolder(ItemHalfHeart.GetPathToString());

	UEngineDirectory Bomb;
	Bomb.MoveParentToDirectory("Resources//Image//Play//Item");
	Bomb.Append("Bomb");
	UImageManager::GetInst().LoadFolder(Bomb.GetPathToString());
}

void ContentsCore::ResourceLoad()
{
	// �̹��� ���ҽ�
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

		Dir.Append("Image");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(/*true*/);
		// ����Ʈ �Ű������� true�̸�, ���� ���丮�� ���ҽ��� �����´�.

		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();

			// Load �Լ� : �̹����� ���ϸ��� Key / HDC�� ��ġ, ũ�⸦ ������ �������ִ� �Լ�
			UImageManager::GetInst().Load(FilePath);
		}
	}

	// ���� ���ҽ�
	{
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}

		Dir.Append("Sounds");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineSound::Load(FilePath);
		}
	}
}

void ContentsCore::Tick()
{
}

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}