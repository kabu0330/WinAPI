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
	// 1. 윈도우 타이틀을 여기서 정한다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("The Binding of Isaac");


	// 2. 윈도우 사이즈를 반드시 정의해야 한다.
	Global::WindowSize = { 960, 540 };
	Global::WindowHalfScale = Global::WindowSize.Half();
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, Global::WindowSize);
}

// 엔진이 실행되고 단 한 번만 실행된다.
void ContentsCore::BeginPlay()
{
	// 진입 전까지 과정
	// 1. 게임 엔진 Core 전역에 메모리 주소 전달(GetCore)
	// 2. 창 클래스 설정 및 윈도우 창 생성, 메인 HDC 생성
	
	WindowSetting();

	// 3. 파일 리소스 가져온다.
	ResourceLoad();
	
	SpriteSetting(); // 이미지 한 장에 모여있는 스프라이트 자르기(.png / .bmp)
	FolderSetting(); // 하위 폴더 안에 모여있는 이미지들을 스프라이트로 묶기 (폴더)

	// 레벨을 생성하고 최초 화면에 출력될 레벨을 결정한다.
	LevelSetting();
}

void ContentsCore::LevelSetting()
{
	// 레벨을 생성한다.
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");
	//UEngineAPICore::GetCore()->CreateLevel("End");

	// 최초 실행될 레벨을 결정한다.
	UEngineAPICore::GetCore()->OpenLevel("Title");
	UEngineAPICore::GetCore()->OpenLevel("Play");
}

void ContentsCore::SpriteSetting()
{
	// 이미지 한 장에서 스프라이트를 몇 개로 만들 것인지 자른다. (가로, 세로)
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
	// 이미지 리소스
	{
		// Directory 클래스를 생성자를 통해 현재 실행 파일 경로를 EnginePath의 Path 멤버에 저장한다.
		UEngineDirectory Dir;

		// 상대경로 : 디버깅을 할 때와 릴리즈하여 배포할 때 내 실행 파일의 경로가 다르다.
		// 이 때, 언제 어떤 상황에서도 내 리소스 파일을 찾아갈 수 있도록 방법을 만들어야 한다.
		// 현재 리소스 파일을 보관하고 있는 폴더명을 적는다.
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}

		Dir.Append("Image");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(/*true*/);
		// 디폴트 매개변수는 true이며, 하위 디렉토리의 리소스도 가져온다.

		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();

			// Load 함수 : 이미지의 파일명을 Key / HDC와 위치, 크기를 값으로 저장해주는 함수
			UImageManager::GetInst().Load(FilePath);
		}
	}

	// 사운드 리소스
	{
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
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