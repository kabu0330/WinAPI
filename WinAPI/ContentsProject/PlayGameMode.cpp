#include "PreCompile.h"
#include "PlayGameMode.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EnginePlatform/EngineInput.h>

#include "ContentsEnum.h"
#include "Global.h"
#include "ContentsCore.h"
#include "Player.h"
#include "PickupNumberUI.h"
#include "HeartUI.h"
#include "PickupItemUI.h"
#include "BannerTextUI.h"
#include "Monster.h"
#include "Fade.h"
#include "LoadingScene.h"
#include "DeathReportScene.h"
#include "MenuScene.h"

#include "AttackFly.h"
#include "Fly.h"
#include "Hopper.h"
#include "Host.h"
#include "Dip.h"
#include "Pooter.h"
#include "Mulligan.h"
#include "Mulligoon.h"
#include "Horf.h"
#include "BlindCreep.h"
#include "Pacer.h"
#include "Gaper.h"
#include "TheDukeOfFlies.h"

#include "RoomObject.h"
#include "Rock.h"
#include "Poop.h"
#include "Fire.h"
#include "MetalBlock.h"

#include "Item.h"
#include "Heart.h"
#include "Bomb.h"
#include "Penny.h"
#include "Key.h"
#include "Polyphemus.h"
#include "CrecketsHead.h"
#include "Dessert.h"
#include "InnerEye.h"
#include "SpoonBender.h"
#include "SpeedBall.h"
#include "NumberOne.h"
#include "ItemAlter.h"
#include "GridPit.h"

bool APlayGameMode::GamePaused = false;
USoundPlayer APlayGameMode::PlayGameModeBGM;

void APlayGameMode::BeginPlay()
{
	GetWorld()->SetCameraPos({0, 0});
	GetWorld()->SetCameraToMainPawn(false);

	Spawn();

	CollisionGroupLinkSetting();
	UISetting();

}

void APlayGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	EngineDebug(_DeltaTime);
	CheckInput();


	if (false == Sound.IsPlaying() && false == IsPlayingBGM)
	{
		PlayGameModeBGM = UEngineSound::Play("diptera_sonata_basement.ogg");
		PlayGameModeBGM.Loop(999);
		IsPlayingBGM = true;
	}
}

void APlayGameMode::Spawn()
{
	// Room
	ARoom* BaseRoom = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom4 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom5 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom4 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom5 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom6 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom7 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom8 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom9 = GetWorld()->SpawnActor<ARoom>();

	// 현재 맵 초기 세팅
	ARoom::SetCurRoom(BaseRoom);

	     BaseRoom->SetRoomType(ERoomType::BASE);
	TreasureRoom0->SetRoomType(ERoomType::TREASURE);
	TreasureRoom1->SetRoomType(ERoomType::TREASURE);
	TreasureRoom2->SetRoomType(ERoomType::TREASURE);
	TreasureRoom3->SetRoomType(ERoomType::TREASURE);
	TreasureRoom4->SetRoomType(ERoomType::TREASURE);
	TreasureRoom5->SetRoomType(ERoomType::TREASURE);
	     BossRoom->SetRoomType(ERoomType::BOSS);

	BaseRoom->SetName("BaseRoom");
	TreasureRoom0->SetName("TreasureRoom0");
	TreasureRoom1->SetName("TreasureRoom1");
	TreasureRoom2->SetName("TreasureRoom2");
	TreasureRoom3->SetName("TreasureRoom3");
	TreasureRoom4->SetName("TreasureRoom4");
	TreasureRoom5->SetName("TreasureRoom5");
	MinionRoom0->SetName("MinionRoom0");
	MinionRoom1->SetName("MinionRoom1");
	MinionRoom2->SetName("MinionRoom2");
	MinionRoom3->SetName("MinionRoom3");
	MinionRoom4->SetName("MinionRoom4");
	MinionRoom5->SetName("MinionRoom5");
	MinionRoom6->SetName("MinionRoom6");
	MinionRoom7->SetName("MinionRoom7");
	MinionRoom8->SetName("MinionRoom8");
	MinionRoom9->SetName("MinionRoom9");
	BossRoom->SetName("BossRoom");

	BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::LEFT);
	BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::RIGHT);
	BaseRoom->InterLinkRoom(BossRoom, RoomDir::DOWN);

	// Left Root
	MinionRoom1->InterLinkRoom(MinionRoom0, RoomDir::UP);
	MinionRoom1->InterLinkRoom(TreasureRoom0, RoomDir::LEFT);
	MinionRoom1->InterLinkRoom(MinionRoom3, RoomDir::DOWN);

	MinionRoom3->InterLinkRoom(MinionRoom6, RoomDir::DOWN);

	MinionRoom6->InterLinkRoom(MinionRoom9, RoomDir::DOWN);
	MinionRoom6->InterLinkRoom(TreasureRoom1, RoomDir::LEFT);
	MinionRoom6->InterLinkRoom(MinionRoom7, RoomDir::RIGHT);


	MinionRoom7->InterLinkRoom(MinionRoom8, RoomDir::RIGHT);
	MinionRoom7->InterLinkRoom(TreasureRoom3, RoomDir::DOWN);
	MinionRoom7->InterLinkRoom(BossRoom, RoomDir::UP);

	MinionRoom9->InterLinkRoom(TreasureRoom4, RoomDir::DOWN);

	// Right Root
	MinionRoom2->InterLinkRoom(MinionRoom4, RoomDir::DOWN);
	MinionRoom2->InterLinkRoom(MinionRoom5, RoomDir::RIGHT);

	MinionRoom4->InterLinkRoom(MinionRoom8, RoomDir::DOWN); // Left Root Link

	MinionRoom5->InterLinkRoom(TreasureRoom5, RoomDir::UP);

	MinionRoom8->InterLinkRoom(TreasureRoom2, RoomDir::RIGHT);


	// BaseRoom
	for (int i = 0; i < 5; i++)
	{
		AItem* InitBomb = BaseRoom->CreateItem<ABomb>(nullptr, { 108, -22 });
	}


	// TreasureRoom0
	{
	 	ARoomObject* Fire0 = TreasureRoom0->CreateObject<AFire>(nullptr, {  150,    0 });
		ARoomObject* Fire1 = TreasureRoom0->CreateObject<AFire>(nullptr, { -150,    0 });
		ARoomObject* Fire2 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310, -180 });
		ARoomObject* Fire3 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310,  180 });
		ARoomObject* Fire4 = TreasureRoom0->CreateObject<AFire>(nullptr, {  310, -180 });
		ARoomObject* Fire5 = TreasureRoom0->CreateObject<AFire>(nullptr, {  310,  180 });
		TreasureRoom0->CreateObject<AItemAlter>(nullptr, {0, 0});
		SpawnRandomItem(TreasureRoom0);
	}

	// TreasureRoom1
	{
		ARoomObject* Fire0 = TreasureRoom1->CreateObject<AFire>(nullptr, { 150, 0 });
		ARoomObject* Fire1 = TreasureRoom1->CreateObject<AFire>(nullptr, { -150, 0 });
		TreasureRoom1->CreateObject<AItemAlter>(nullptr, { 0, 0 });
		SpawnRandomItem(TreasureRoom1);
	}

	// TreasureRoom2
	{
		ARoomObject* Fire0 = TreasureRoom2->CreateObject<AFire>(nullptr, { 150, 0 });
		ARoomObject* Fire1 = TreasureRoom2->CreateObject<AFire>(nullptr, { -150, 0 });
		ARoomObject* Fire2 = TreasureRoom2->CreateObject<AFire>(nullptr, { -310, -180 });
		ARoomObject* Fire3 = TreasureRoom2->CreateObject<AFire>(nullptr, { -310,  180 });
		ARoomObject* Fire4 = TreasureRoom2->CreateObject<AFire>(nullptr, { 310, -180 });
		ARoomObject* Fire5 = TreasureRoom2->CreateObject<AFire>(nullptr, { 310,  180 });
		TreasureRoom2->CreateObject<AItemAlter>(nullptr, { 0, 0 });
		SpawnRandomItem(TreasureRoom2);
	}

	// TreasureRoom3
	{
		ARoomObject* Fire0 = TreasureRoom3->CreateObject<AFire>(nullptr, { 150, 0 });
		ARoomObject* Fire1 = TreasureRoom3->CreateObject<AFire>(nullptr, { -150, 0 });
		ARoomObject* Fire2 = TreasureRoom3->CreateObject<AFire>(nullptr, { -310, -180 });
		ARoomObject* Fire3 = TreasureRoom3->CreateObject<AFire>(nullptr, { -310,  180 });
		ARoomObject* Fire4 = TreasureRoom3->CreateObject<AFire>(nullptr, { 310, -180 });
		ARoomObject* Fire5 = TreasureRoom3->CreateObject<AFire>(nullptr, { 310,  180 });
		TreasureRoom3->CreateObject<AItemAlter>(nullptr, { 0, 0 });
		SpawnRandomItem(TreasureRoom3);
	}

	// TreasureRoom4
	{
		ARoomObject* Fire0 = TreasureRoom4->CreateObject<AFire>(nullptr, { 150, 0 });
		ARoomObject* Fire1 = TreasureRoom4->CreateObject<AFire>(nullptr, { -150, 0 });
		TreasureRoom4->CreateObject<AItemAlter>(nullptr, { 0, 0 });
		SpawnRandomItem(TreasureRoom4);
	}

	// TreasureRoom5
	{
		ARoomObject* Fire0 = TreasureRoom5->CreateObject<AFire>(nullptr, { 150, 0 });
		ARoomObject* Fire1 = TreasureRoom5->CreateObject<AFire>(nullptr, { -150, 0 });
		TreasureRoom5->CreateObject<AItemAlter>(nullptr, { 0, 0 });
		SpawnRandomItem(TreasureRoom5);
	}


	// Left Root
	
	// MinionRoom0 : 플레이어 아래쪽 // 만든 모든 몬스터들
	{
		AMonster* Monster0  = MinionRoom0->CreateMonster<APacer>({-260, -150});
		AMonster* Monster1  = MinionRoom0->CreateMonster<AHopper>({-230, -150});
		AMonster* Monster2  = MinionRoom0->CreateMonster<AAttackFly>({-180, -150});
		AMonster* Monster3  = MinionRoom0->CreateMonster<AFly>({-130, -150});
		AMonster* Monster4  = MinionRoom0->CreateMonster<ABlindCreep>({-80, -150});
		AMonster* Monster5  = MinionRoom0->CreateMonster<ADip>({-30, -150});
		AMonster* Monster6  = MinionRoom0->CreateMonster<AGaper>({20, -150});
		AMonster* Monster7  = MinionRoom0->CreateMonster<AHorf>({120, -150});
		AMonster* Monster8  = MinionRoom0->CreateMonster<AHost>({170, -150});
		AMonster* Monster9  = MinionRoom0->CreateMonster<APooter>({220, -150});
		AMonster* Monster10 = MinionRoom0->CreateMonster<AMulligan>({-260, -100});
		AMonster* Monster11 = MinionRoom0->CreateMonster<AMulligoon>({260, -100});

		for (int i = 0; i < 13; i++)
		{
			MinionRoom0->CreateObject<AGridPit>(nullptr, { -310 + (51 * i), -60});
		}
	}
	
	//MinionRoom1 : 플레이어 오른쪽 : 파리맵
	{
		MinionRoom1->CreateMonster<AAttackFly>({ 0, -80 });
		MinionRoom1->CreateMonster<AAttackFly>({ 50, -10 });
		MinionRoom1->CreateMonster<AAttackFly>({ 0, 70 });
		MinionRoom1->CreateMonster<AAttackFly>({ 0, 0 });
		MinionRoom1->CreateMonster<AFly>({ -50, -50 });
		MinionRoom1->CreateMonster<AFly>({ -100, -10 });
		MinionRoom1->CreateMonster<AFly>({ -50, 40 });

		ARoomObject* Poop0 = MinionRoom1->CreateObject<APoop>(nullptr, { -310, 180 });
		ARoomObject* Poop1 = MinionRoom1->CreateObject<APoop>(nullptr, { -310, -190 });
		ARoomObject* Poop2 = MinionRoom1->CreateObject<APoop>(nullptr, { 310, 180 });
		ARoomObject* Poop3 = MinionRoom1->CreateObject<APoop>(nullptr, { 310, -190 });
		ARoomObject* Poop4 = MinionRoom1->CreateObject<APoop>(nullptr, { 0, 0 });
		ARoomObject* Poop5 = MinionRoom1->CreateObject<APoop>(nullptr, { -50, 50 });
		ARoomObject* Poop6 = MinionRoom1->CreateObject<APoop>(nullptr, { -50, -50 });
		ARoomObject* Poop7 = MinionRoom1->CreateObject<APoop>(nullptr, { -100, 0 });
		ARoomObject* Poop8 = MinionRoom1->CreateObject<APoop>(nullptr, { 50, -50 });
		ARoomObject* Poop9 = MinionRoom1->CreateObject<APoop>(nullptr, { 50, 50 });
		ARoomObject* Poop10 = MinionRoom1->CreateObject<APoop>(nullptr, { 100, 0 });

		Poop0->SetSprite("CORNY_POOP");
		Poop1->SetSprite("CORNY_POOP");
		Poop2->SetSprite("CORNY_POOP");
		Poop3->SetSprite("CORNY_POOP");
		Poop4->SetSprite("CORNY_POOP");
		Poop5->SetSprite("GOLDEN_POOP");
		Poop6->SetSprite("CORNY_POOP");
		Poop7->SetSprite("CORNY_POOP");
		Poop8->SetSprite("CORNY_POOP");
		Poop9->SetSprite("CORNY_POOP");
		Poop10->SetSprite("CORNY_POOP");
	}

	// MinionRoom3 : 플레이어 위쪽 // 눈 먼 크리프
	{
		MinionRoom3->CreateMonster<ABlindCreep>({ -280,100 });
		MinionRoom3->CreateMonster<ABlindCreep>({ -280,0 });
		MinionRoom3->CreateMonster<ABlindCreep>({ -280,-100 });
		ARoomObject* GridPit00 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, -170 });
		ARoomObject* GridPit01 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, -120 });
		ARoomObject* GridPit02 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, -70 });
		ARoomObject* GridPit03 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, -20 });
		ARoomObject* GridPit04 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, 30 });
		ARoomObject* GridPit05 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, 80 });
		ARoomObject* GridPit06 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, 130 });
		ARoomObject* GridPit07 = MinionRoom3->CreateObject<AGridPit>(nullptr, { -220, 180 });

		ARoomObject* GridPit10 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, -170 });
		ARoomObject* GridPit11 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, -120 });
		ARoomObject* GridPit12 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, -70 });
		ARoomObject* GridPit13 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, -20 });
		ARoomObject* GridPit14 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, 30 });
		ARoomObject* GridPit15 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, 80 });
		ARoomObject* GridPit16 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, 130 });
		ARoomObject* GridPit17 = MinionRoom3->CreateObject<AGridPit>(nullptr, { 220, 180 });

		MinionRoom3->CreateObject<AFire>(nullptr, { 0, 0 });
		MinionRoom3->CreateObject<AFire>(nullptr, { -160,  -170 });
		MinionRoom3->CreateObject<AFire>(nullptr, { 160, -170 });
		MinionRoom3->CreateObject<AFire>(nullptr, { -160, 180 });
		MinionRoom3->CreateObject<AFire>(nullptr, { 160, 180 });
	}

	// MinionRoom6 : 플레이어 위 // 게이퍼, 페이서, 멀리건, 멀리군
	{
		MinionRoom6->CreateMonster<AMulligan>({ -150, -100 });
		MinionRoom6->CreateMonster<AMulligan>({ -150, 100 });
		MinionRoom6->CreateMonster<AMulligoon>({ 150, -100 });
		MinionRoom6->CreateMonster<AMulligoon>({ -150, 100 });
		MinionRoom6->CreateMonster<AGaper>({ -50, 0 });
		MinionRoom6->CreateMonster<AGaper>({ -100, 150 });

		// 중앙
		ARoomObject* Object0 = MinionRoom6->CreateObject<ARock>(nullptr, { -100,   0 });
		ARoomObject* Object1 = MinionRoom6->CreateObject<ARock>(nullptr, { -50, -50 });
		ARoomObject* Object2 = MinionRoom6->CreateObject<ARock>(nullptr, { -50,  50 });
		ARoomObject* Object3 = MinionRoom6->CreateObject<ARock>(nullptr, { -100,  50 });
		ARoomObject* Object4 = MinionRoom6->CreateObject<ARock>(nullptr, { 0,  50 });
		ARoomObject* Object5 = MinionRoom6->CreateObject<ARock>(nullptr, { -100,  -50 });
		ARoomObject* Object6 = MinionRoom6->CreateObject<ARock>(nullptr, { 0,  -50 });
		ARoomObject* Poop0 = MinionRoom6->CreateObject<APoop>(nullptr, { 0,   0 });
		Poop0->SetSprite("CORNY_POOP");


		MinionRoom6->CreateObject<ARock>(nullptr, { -220, -100 });
		MinionRoom6->CreateObject<ARock>(nullptr, { -220, 100 });
		MinionRoom6->CreateObject<ARock>(nullptr, { 160, -100 });
		MinionRoom6->CreateObject<ARock>(nullptr, { 160, 100 });
	}

	// MinionRoom7 : 플레이어 좌, 우 // 푸터, 호퍼
	{
		MinionRoom7->CreateMonster<APooter>({ 80, -10 });
		MinionRoom7->CreateMonster<APooter>({ -80, -10 });
		MinionRoom7->CreateMonster<AHopper>({ 0, -80 });
		MinionRoom7->CreateMonster<AHopper>({ 0, 80 });

		ARoomObject* Object0 = MinionRoom7->CreateObject<ARock>(nullptr, { -150, -120 });
		ARoomObject* Object1 = MinionRoom7->CreateObject<ARock>(nullptr, { 150, 120 });
		ARoomObject* Object2 = MinionRoom7->CreateObject<ARock>(nullptr, { -150, 120 });
		ARoomObject* Object3 = MinionRoom7->CreateObject<ARock>(nullptr, { 150, -120 });
		Object1->SetSprite("TINTEDROCKS1");
	}

	// MinionRoom9 : 플레이어 위쪽 // 호스트 
	{
		MinionRoom9->CreateMonster<AHost>({ 220, 50 });
		MinionRoom9->CreateMonster<AHost>({ -220, 50 });

		ARoomObject* Object0 = MinionRoom9->CreateObject<ARock>(nullptr, { -180, 100 });
		ARoomObject* Object1 = MinionRoom9->CreateObject<ARock>(nullptr, { 180, 100 });
		ARoomObject* Object2 = MinionRoom9->CreateObject<ARock>(nullptr, { -180, -100 });
		ARoomObject* Object3 = MinionRoom9->CreateObject<ARock>(nullptr, { 180, -100 });
		Object0->SetSprite("TINTEDROCKS1");
		Object1->SetSprite("TINTEDROCKS2");
	}


	// Right Root
	//
	//MinionRoom2 : 플레이어 왼쪽 // 딥
	{
		MinionRoom2->CreateMonster<ADip>({ 50, 0 });
		MinionRoom2->CreateMonster<ADip>({ -50, 0 });
		MinionRoom2->CreateMonster<ADip>({ 0, -50 });
		MinionRoom2->CreateMonster<ADip>({ 0, 50 });

		ARoomObject* Poop0 = MinionRoom2->CreateObject<APoop>(nullptr, { 0, 0 });
		ARoomObject* Poop1 = MinionRoom2->CreateObject<APoop>(nullptr, { -50, -50 });
		ARoomObject* Poop2 = MinionRoom2->CreateObject<APoop>(nullptr, { 50, 50 });
		ARoomObject* Poop3 = MinionRoom2->CreateObject<APoop>(nullptr, { -50, 50 });
		ARoomObject* Poop4 = MinionRoom2->CreateObject<APoop>(nullptr, { 50, -50 });
		Poop0->SetSprite("GOLDEN_POOP");
		Poop1->SetSprite("CORNY_POOP");
		Poop2->SetSprite("CORNY_POOP");
		Poop3->SetSprite("CORNY_POOP");
		Poop4->SetSprite("CORNY_POOP");
	}

	// MinionRoom5 : 플레이어 왼쪽 // 폭탄
	{
		MinionRoom5->CreateItem<ABomb>(nullptr, { -310, -180 });
		MinionRoom5->CreateItem<ABomb>(nullptr, { -260, -180 });
		MinionRoom5->CreateItem<ABomb>(nullptr, { -310, -135 });
		MinionRoom5->CreateItem<ABomb>(nullptr, { 310, 180 });
		MinionRoom5->CreateItem<ABomb>(nullptr, { 260, 180 });
		MinionRoom5->CreateItem<ABomb>(nullptr, { 310, 135 });

		// 중앙
		ARoomObject* Object0 = MinionRoom5->CreateObject<AMetalBlock>(nullptr, { 0, 0 });
		ARoomObject* Object1 = MinionRoom5->CreateObject<ARock>(nullptr, { -50, 0 });
		ARoomObject* Object2 = MinionRoom5->CreateObject<ARock>(nullptr, { 50, 0 });
		ARoomObject* Object3 = MinionRoom5->CreateObject<ARock>(nullptr, { 0, 50 });
		ARoomObject* Object4 = MinionRoom5->CreateObject<ARock>(nullptr, { 0, -50 });

		// 좌상단
		ARoomObject* Object11 = MinionRoom5->CreateObject<ARock>(nullptr, { -310, -80 });
		ARoomObject* Object12 = MinionRoom5->CreateObject<ARock>(nullptr, { -260, -80 });
		ARoomObject* Object13 = MinionRoom5->CreateObject<AMetalBlock>(nullptr, { -210, -80 });
		ARoomObject* Object14 = MinionRoom5->CreateObject<ARock>(nullptr, { -210, -130 });
		ARoomObject* Object15 = MinionRoom5->CreateObject<ARock>(nullptr, { -260, -130 });
		ARoomObject* Object16 = MinionRoom5->CreateObject<ARock>(nullptr, { -210, -180 });

		// 우하단
		ARoomObject* Object21 = MinionRoom5->CreateObject<ARock>(nullptr, { 310, 80 });
		ARoomObject* Object22 = MinionRoom5->CreateObject<ARock>(nullptr, { 260, 80 });
		ARoomObject* Object23 = MinionRoom5->CreateObject<AMetalBlock>(nullptr, { 210, 80 });
		ARoomObject* Object24 = MinionRoom5->CreateObject<ARock>(nullptr, { 210, 130 });
		ARoomObject* Object25 = MinionRoom5->CreateObject<ARock>(nullptr, { 260, 130 });
		ARoomObject* Object26 = MinionRoom5->CreateObject<ARock>(nullptr, { 210, 180 });
		Object1->SetSprite("TINTEDROCKS2");
		Object11->SetSprite("TINTEDROCKS2");
		Object12->SetSprite("TINTEDROCKS1");
		Object14->SetSprite("TINTEDROCKS1");
		Object21->SetSprite("TINTEDROCKS2");
		Object22->SetSprite("TINTEDROCKS1");
		Object24->SetSprite("TINTEDROCKS1");
	}

	// MinionRoom4 : 플레이어 위쪽 // 호프
	{
		AMonster* Horf0 = MinionRoom4->CreateMonster<AHorf>({ 50, -54 });
		AMonster* Horf2 = MinionRoom4->CreateMonster<AHorf>({ -50, -54 });
		AMonster* Horf1 = MinionRoom4->CreateMonster<AHorf>({ 50,  46 });
		AMonster* Horf3 = MinionRoom4->CreateMonster<AHorf>({ -50,  46 });

		// 중앙
		MinionRoom4->CreateObject<AMetalBlock>(nullptr, { 0, -100 });
		MinionRoom4->CreateObject<AMetalBlock>(nullptr, { 0, -50 });
		MinionRoom4->CreateObject<AMetalBlock>(nullptr, { 0, 0 });
		MinionRoom4->CreateObject<AMetalBlock>(nullptr, { 0, 50 });
		MinionRoom4->CreateObject<AMetalBlock>(nullptr, { 0, 100 });

		MinionRoom4->CreateObject<ARock>(nullptr, { -50, -100 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -50, 0 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -50, 100 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 50, -100 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 50, 0 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 50, 100 });

		// 좌측
		MinionRoom4->CreateObject<ARock>(nullptr, { -160, -100 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -160, -50 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -160, 0 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -160, 50 });
		MinionRoom4->CreateObject<ARock>(nullptr, { -160, 100 });

		// 우측
		MinionRoom4->CreateObject<ARock>(nullptr, { 160, -100 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 160, -50 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 160, 0 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 160, 50 });
		MinionRoom4->CreateObject<ARock>(nullptr, { 160, 100 });
	}

	// MinionRoom8 : 플레이어 위쪽 // 멀리건, 멀리군
	{
		MinionRoom8->CreateMonster<AMulligan>({ 150, 80 });
		MinionRoom8->CreateMonster<AMulligan>({ -200, 120 });
		MinionRoom8->CreateMonster<AMulligoon>({ 0, 180 });
		MinionRoom8->CreateMonster<AFly>({ 0, 0 });
		MinionRoom8->CreateMonster<AAttackFly>({ 150, 120 });
		MinionRoom8->CreateMonster<AAttackFly>({ -150, 120 });

		// 중앙
		ARoomObject* Object0 = MinionRoom8->CreateObject<ARock>(nullptr, { -50, -50 });
		ARoomObject* Object1 = MinionRoom8->CreateObject<ARock>(nullptr, { -50, 0 });
		ARoomObject* Object2 = MinionRoom8->CreateObject<ARock>(nullptr, { -50, 50 });
		ARoomObject* Object3 = MinionRoom8->CreateObject<ARock>(nullptr, { 0, -50 });
		ARoomObject* Object4 = MinionRoom8->CreateObject<ARock>(nullptr, { 0, 50 });
		ARoomObject* Object5 = MinionRoom8->CreateObject<ARock>(nullptr, { 50, -50 });
		ARoomObject* Object6 = MinionRoom8->CreateObject<ARock>(nullptr, { 50, 0 });
		ARoomObject* Object7 = MinionRoom8->CreateObject<ARock>(nullptr, { 50, 50 });
		ARoomObject* Object8 = MinionRoom8->CreateObject<ARock>(nullptr, { -150, -120 });
		ARoomObject* Object9 = MinionRoom8->CreateObject<ARock>(nullptr, { 150, 120 });
		ARoomObject* Object10 = MinionRoom8->CreateObject<ARock>(nullptr, { 150, -120 });
		ARoomObject* Object11 = MinionRoom8->CreateObject<ARock>(nullptr, { -150, 120 });
		ARoomObject* Object12 = MinionRoom8->CreateObject<ARock>(nullptr, { -200, 10 });
		ARoomObject* Object13 = MinionRoom8->CreateObject<ARock>(nullptr, { 200, 10 });
		ARoomObject* Object14 = MinionRoom8->CreateObject<ARock>(nullptr, { -310, -180 });
		ARoomObject* Object15 = MinionRoom8->CreateObject<ARock>(nullptr, { -310, 180 });
		ARoomObject* Object16 = MinionRoom8->CreateObject<ARock>(nullptr, { 310, -180 });
		ARoomObject* Object17 = MinionRoom8->CreateObject<ARock>(nullptr, { 310, 180 });
		Object1->SetSprite("TINTEDROCKS1");
		Object7->SetSprite("TINTEDROCKS1");
		Object8->SetSprite("TINTEDROCKS2");
		Object9->SetSprite("TINTEDROCKS5");
		Object14->SetSprite("TINTEDROCKS1");
		Object15->SetSprite("TINTEDROCKS2");
		Object16->SetSprite("TINTEDROCKS4");
	}
	
	// BossRoom
	{
		// 좌상단
		ARoomObject* Object01 = BossRoom->CreateObject<ARock>(nullptr, { -310, -180 });
		ARoomObject* Object02 = BossRoom->CreateObject<ARock>(nullptr, { -260, -180 });
		ARoomObject* Object03 = BossRoom->CreateObject<ARock>(nullptr, { -310, -130 });
		ARoomObject* Object04 = BossRoom->CreateObject<ARock>(nullptr, { -210, -180 });
		ARoomObject* Object05 = BossRoom->CreateObject<ARock>(nullptr, { -310, -80 });
		ARoomObject* Object06 = BossRoom->CreateObject<ARock>(nullptr, { -210, -80 });

		// 좌하단
		ARoomObject* Object11 = BossRoom->CreateObject<ARock>(nullptr, { -310, 180 });
		ARoomObject* Object12 = BossRoom->CreateObject<ARock>(nullptr, { -260, 180 });
		ARoomObject* Object13 = BossRoom->CreateObject<ARock>(nullptr, { -310, 130 });
		ARoomObject* Object14 = BossRoom->CreateObject<ARock>(nullptr, { -210, 180 });
		ARoomObject* Object15 = BossRoom->CreateObject<ARock>(nullptr, { -310, 80 });
		ARoomObject* Object16 = BossRoom->CreateObject<ARock>(nullptr, { -210, 80 });

		// 우하단
		ARoomObject* Object21 = BossRoom->CreateObject<ARock>(nullptr, { 310, 180 });
		ARoomObject* Object22 = BossRoom->CreateObject<ARock>(nullptr, { 260, 180 });
		ARoomObject* Object23 = BossRoom->CreateObject<ARock>(nullptr, { 310, 130 });
		ARoomObject* Object24 = BossRoom->CreateObject<ARock>(nullptr, { 210, 180 });
		ARoomObject* Object25 = BossRoom->CreateObject<ARock>(nullptr, { 310, 80 });
		ARoomObject* Object26 = BossRoom->CreateObject<ARock>(nullptr, { 210, 80 });

		// 우상단
		ARoomObject* Object31 = BossRoom->CreateObject<ARock>(nullptr, { 310, -180 });
		ARoomObject* Object32 = BossRoom->CreateObject<ARock>(nullptr, { 260, -180 });
		ARoomObject* Object33 = BossRoom->CreateObject<ARock>(nullptr, { 310, -130 });
		ARoomObject* Object34 = BossRoom->CreateObject<ARock>(nullptr, { 210, -180 });
		ARoomObject* Object35 = BossRoom->CreateObject<ARock>(nullptr, { 310, -80 });
		ARoomObject* Object36 = BossRoom->CreateObject<ARock>(nullptr, { 210, -80 });

		Object06->SetSprite("TINTEDROCKS5");
		Object16->SetSprite("TINTEDROCKS0");
		Object26->SetSprite("TINTEDROCKS1");
		Object36->SetSprite("TINTEDROCKS0");

		Object02->SetSprite("TINTEDROCKS2");
		Object12->SetSprite("TINTEDROCKS2");
		Object22->SetSprite("TINTEDROCKS2");
		Object32->SetSprite("TINTEDROCKS2");

		Object03->SetSprite("TINTEDROCKS1");
		Object13->SetSprite("TINTEDROCKS1");
		Object23->SetSprite("TINTEDROCKS1");
		Object33->SetSprite("TINTEDROCKS1");

		BossRoom->CreateMonster<ATheDukeOfFlies>({ -100, 0 });
	}
	
}

void APlayGameMode::SpawnRandomItem(ARoom* _ParentRoom)
{
	RandomIndex();

	FVector2D Offset = { 0, -40 };
	switch (RandomValues[Index])
	{
	case 0:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<APolyphemus>(nullptr, Offset);
		break;
	}
	case 1:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<ACrecketsHead>(nullptr, Offset);
		break;
	}
	case 2:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<ADessert>(nullptr, Offset);
		break;
	}
	case 3:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<AInnerEye>(nullptr, Offset);
		break;
	}
	case 4:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<ASpoonBender>(nullptr, Offset);
		break;
	}
	case 5:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<ASpeedBall>(nullptr, Offset);
		break;
	}
	case 6:
	{
		AItem* PassiveItem = _ParentRoom->CreateItem<ANumberOne>(nullptr, Offset);
		break;
	}
	default:
		break;
	}

	++Index;
}

void APlayGameMode::RandomIndex()
{
	const int PassiveItemCount = 7;

	if (false == IsSwap)
	{
		int ItemCreate[PassiveItemCount] = { 0, 1, 2, 3, 4, 5, 6 };
		for (int i = 0; i < 1000; i++)
		{
			int Left = Random.RandomInt(0, PassiveItemCount - 1);
			int Right = Random.RandomInt(0, PassiveItemCount - 1);
			int Swap = ItemCreate[Left];
			ItemCreate[Left] = ItemCreate[Right];
			ItemCreate[Right] = Swap;
		}
		for (int i = 0; i < 5; i++)
		{
			RandomValues[i] = ItemCreate[i];
		}
	}

	IsSwap = true;
}

void APlayGameMode::CollisionGroupLinkSetting()
{
	// Player
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Warp, ECollisionGroup::Room_Wall); // 플레이어는 맵 밖을 나갈 수 없다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Warp, ECollisionGroup::Warp); // 플레이어는 문과 충돌하여 다음 맵으로 넘어간다.

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Body, ECollisionGroup::Monster_Body); // 플레이어는 몬스터와 충돌하면 데미지를 입는다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Body, ECollisionGroup::Monster_FlyingBody); 

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_Body); // 플레이어는 몬스터를 공격할 수 있다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_BodyNonCollision);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_FlyingBody); 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_FlyingBodyNonCollision);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_Barrier); // 플레이어의 공격을 막으며 즉시 터진다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Object);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Item_Impact);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Room_TearBoundary); // 플레이어의 공격이 벽에 닿으면 즉시 터진다.

	// 특정 아이템을 먹을 경우 몬스터 추적 기능 추가
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_DetectInRange, ECollisionGroup::Monster_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_DetectInRange, ECollisionGroup::Monster_BodyNonCollision);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_DetectInRange, ECollisionGroup::Monster_FlyingBody);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_DetectInRange, ECollisionGroup::Monster_FlyingBodyNonCollision);

	// Monster
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Body, ECollisionGroup::Room_Wall); // 몬스터는 맵 밖을 나갈 수 없다.

	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Player_Body); 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Room_TearBoundary);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Object);

	// Object
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Player_Warp);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object_Bottom, ECollisionGroup::Player_Warp);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_Body); // 몬스터는 오브젝트를 통과할 수 없다. 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object_Bottom, ECollisionGroup::Monster_Body); 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_BodyNonCollision); 

	// Item
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Room_Wall);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_Impact, ECollisionGroup::Object);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_FlyingBody);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_FlyingBodyNonCollision);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Object);
}

void APlayGameMode::CheckInput()
{
	AActor* Pawn = GetWorld()->GetPawn();
	APlayer* Player = dynamic_cast<APlayer*>(Pawn);	

	// 플레이어가 죽으면 메뉴 창 열지마.
	if (true == Player->IsDeath())
	{
		return;
	}

	if (UEngineInput::GetInst().IsDown(VK_ESCAPE))
	{
		Sound = UEngineSound::Play("button_press.wav");

		if (false == IsShowMenu)
		{
			AMenuScene::Menu->ShowMenu();
			SwitchIsShowDeathReport();
			SetGamePaused(true);
		}
		else
		{
			AMenuScene::Menu->CloseMenu();
			SwitchIsShowDeathReport();
			SetGamePaused(false);
		}
	}
}

void APlayGameMode::UISetting()
{
	// Text
	//ABannerTextUI* TextUI = GetWorld()->SpawnActor<ABannerTextUI>();
	//TextUI->SetName("TextUI");
	//TextUI->SetTextSpriteName("banner.png");
	//TextUI->SetOrder(ERenderOrder::UI);
	//TextUI->SetTextScale({ 16, 10 });
	//TextUI->SetActorLocation({ 400, 100 });
	//TextUI->SetTextOut("TheBindingOfIsaac"); // 띄어쓰기 문제 해결 어떻게?

	// Death Report
	ADeathReportScene::DeathReport = GetWorld()->SpawnActor<ADeathReportScene>();
	AMenuScene::Menu = GetWorld()->SpawnActor<AMenuScene>();
}

void APlayGameMode::EngineDebug(float _DeltaTime)
{
	FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(static_cast<int>(1.0f / _DeltaTime)));
	UEngineDebug::CoreOutPutString("Player Pos : " + GetWorld()->GetPawn()->GetActorLocation().ToString(), { 0, Global::WindowSize.iY() - 20 });
	UEngineDebug::CoreOutPutString("Camera Pos : " + GetWorld()->GetCameraPos().ToString(), { 0, Global::WindowSize.iY() - 40 });
	//UEngineDebug::CoreOutPutString("Mouse Pos : " + MousePos.ToString(), { 0, Global::WindowSize.iY() - 20 });
	UEngineDebug::CoreOutPutString("Room : " + ARoom::GetCurRoom()->GetName(), { Global::WindowSize.iX() - 200, 0});

	if (true == UEngineInput::GetInst().IsDown('B'))
	{
		UEngineDebug::SwitchIsDebug();
	}
}

void APlayGameMode::LevelChangeStart()
{
	AFade::MainFade = GetWorld()->SpawnActor<AFade>();



#ifdef _DEBUG
	TimeEventer.PushEvent(7.0f, [this]() {	UEngineSound::AllSoundStop();
	Sound = UEngineSound::Play("basementIntro.ogg"); });

#else
	GamePaused = true;
	USpriteRenderer* FadeRenderer = AFade::MainFade->GetRenderer();

	LoadingImage = GetWorld()->SpawnActor<ALoadingScene>();
	USpriteRenderer* LoadingRenderer = LoadingImage->GetRenderer();

	FadeRenderer->SetActive(true);
	LoadingRenderer->SetActive(true);

	TimeEventer.PushEvent(7.0f, [this]() {	
		UEngineSound::AllSoundStop();
		Sound = UEngineSound::Play("basementIntro.ogg"); 
	}); // 여기서 플레이어가 이동 가능

	TimeEventer.PushEvent(5.0f, std::bind(&APlayGameMode::FadeOut, this));
#endif // DEBUG
}

void APlayGameMode::FadeOut()
{
#ifdef _DEBUG

#else
	AFade::MainFade->FadeOut();
	LoadingImage->FadeOut();

	LoadingImage->Destroy(5.0f);
	GamePaused = false;
#endif // _DEBUG

}

APlayGameMode::APlayGameMode()
{
}

APlayGameMode::~APlayGameMode()
{
}
