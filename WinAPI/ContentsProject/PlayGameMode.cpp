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

bool APlayGameMode::GamePaused = false;

void APlayGameMode::BeginPlay()
{
	GetWorld()->SetCameraPos({0, 0});
	GetWorld()->SetCameraToMainPawn(false);

	Spawn();

	CollisionGroupLinkSetting();
	UISetting();
}

void APlayGameMode::Spawn()
{
	// Room
	ARoom* BaseRoom = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* TreasureRoom2 = GetWorld()->SpawnActor<ARoom>();
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
	BossRoom->SetRoomType(ERoomType::BOSS);

	BaseRoom->SetName("BaseRoom");
	TreasureRoom0->SetName("TreasureRoom0");
	TreasureRoom1->SetName("TreasureRoom1");
	TreasureRoom2->SetName("TreasureRoom2");
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

	BaseRoom->InterLinkRoom(TreasureRoom0, RoomDir::UP);
	BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::LEFT);
	BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::RIGHT);
	//BaseRoom->InterLinkRoom(BossRoom, RoomDir::DOWN);

	// Left Root
	MinionRoom1->InterLinkRoom(MinionRoom0, RoomDir::UP);
	MinionRoom1->InterLinkRoom(MinionRoom3, RoomDir::DOWN);

	MinionRoom3->InterLinkRoom(TreasureRoom1, RoomDir::LEFT);
	MinionRoom3->InterLinkRoom(MinionRoom6, RoomDir::DOWN);

	MinionRoom6->InterLinkRoom(MinionRoom7, RoomDir::RIGHT);
	MinionRoom6->InterLinkRoom(MinionRoom9, RoomDir::DOWN);

	MinionRoom9->InterLinkRoom(BossRoom, RoomDir::LEFT);

	MinionRoom7->InterLinkRoom(MinionRoom8, RoomDir::RIGHT);

	// Right Root
	MinionRoom2->InterLinkRoom(MinionRoom4, RoomDir::DOWN);

	MinionRoom4->InterLinkRoom(MinionRoom5, RoomDir::RIGHT);
	MinionRoom4->InterLinkRoom(MinionRoom8, RoomDir::DOWN); // Left Root Link
	MinionRoom8->InterLinkRoom(TreasureRoom2, RoomDir::RIGHT);



	// TreasureRoom0
	{
	 	ARoomObject* Fire0 = TreasureRoom0->CreateObject<AFire>(nullptr, {  150,    0 });
		ARoomObject* Fire1 = TreasureRoom0->CreateObject<AFire>(nullptr, { -150,    0 });
		ARoomObject* Fire2 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310, -190 });
		ARoomObject* Fire3 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310,  180 });
		ARoomObject* Fire4 = TreasureRoom0->CreateObject<AFire>(nullptr, {  310, -190 });
		ARoomObject* Fire5 = TreasureRoom0->CreateObject<AFire>(nullptr, {  310,  180 });
		SpawnRandomItem(TreasureRoom0);
	}

	// TreasureRoom1
	{
		ARoomObject* Fire2 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310, -190 });
		ARoomObject* Fire3 = TreasureRoom0->CreateObject<AFire>(nullptr, { -310,  180 });
		ARoomObject* Fire4 = TreasureRoom0->CreateObject<AFire>(nullptr, { 310, -190 });
		ARoomObject* Fire5 = TreasureRoom0->CreateObject<AFire>(nullptr, { 310,  180 });
		SpawnRandomItem(TreasureRoom1);
	}

	// TreasureRoom2
	{
		ARoomObject* Fire0 = TreasureRoom2->CreateObject<AFire>(nullptr, {  150, 0 });
		ARoomObject* Fire1 = TreasureRoom2->CreateObject<AFire>(nullptr, { -150, 0 });
		SpawnRandomItem(TreasureRoom2);
	}


	// BaseRoom : 테스트용
	//BaseRoom->CreateMonster<ATheDukeOfFlies>({ -150, 0 });
	
	// MinionRoom0 
	// MinionRoom1 : 플레이어 오른쪽 : 파리맵
	{
		//ARoomObject* Poop0 = MinionRoom1->CreateObject<APoop>(nullptr, { -310, 180 });
		//Poop0->SetSprite("CORNY_POOP");
		//ARoomObject* Poop1 = MinionRoom1->CreateObject<APoop>(nullptr, { -310, -190 });
		//Poop1->SetSprite("CORNY_POOP");
		//ARoomObject* Poop2 = MinionRoom1->CreateObject<APoop>(nullptr, { 310, 180 });
		//Poop2->SetSprite("CORNY_POOP");
		//ARoomObject* Poop3 = MinionRoom1->CreateObject<APoop>(nullptr, { 310, -190 });
		//Poop3->SetSprite("CORNY_POOP");
		//ARoomObject* Poop4 = MinionRoom1->CreateObject<APoop>(nullptr, { 0, 0 });
		//Poop4->SetSprite("CORNY_POOP");
		//ARoomObject* Poop5 = MinionRoom1->CreateObject<APoop>(nullptr, { -50, 50 });
		//Poop5->SetSprite("GOLDEN_POOP");
		//ARoomObject* Poop6 = MinionRoom1->CreateObject<APoop>(nullptr, { -50, -50 });
		//Poop6->SetSprite("CORNY_POOP");
		//ARoomObject* Poop7 = MinionRoom1->CreateObject<APoop>(nullptr, { -100, 0 });
		//Poop7->SetSprite("CORNY_POOP");
		//ARoomObject* Poop8 = MinionRoom1->CreateObject<APoop>(nullptr, { 50, -50 });
		//Poop8->SetSprite("CORNY_POOP");
		//ARoomObject* Poop9 = MinionRoom1->CreateObject<APoop>(nullptr, { 50, 50 });
		//Poop9->SetSprite("CORNY_POOP");
		//ARoomObject* Poop10 = MinionRoom1->CreateObject<APoop>(nullptr, { 100, 0 });
		//Poop10->SetSprite("CORNY_POOP");

		MinionRoom1->CreateMonster<AAttackFly>({ 0, -80 });
		MinionRoom1->CreateMonster<AAttackFly>({ 50, -10 });
		MinionRoom1->CreateMonster<AAttackFly>({ 0, 70 });
		MinionRoom1->CreateMonster<AAttackFly>({ 0, 0 });
		MinionRoom1->CreateMonster<AFly>({ -50, -50 });
		MinionRoom1->CreateMonster<AFly>({ -100, -10 });
		MinionRoom1->CreateMonster<AFly>({ -50, 40 });

	}

	// MinionRoom3 : 플레이어 위쪽
	{
		//ARoomObject* Object0 = MinionRoom3->CreateObject<ARock>(nullptr, { -180, 100 });
		//Object0->SetSprite("TINTEDROCKS1");
		//ARoomObject* Object1 = MinionRoom3->CreateObject<ARock>(nullptr, { 180, 100 });
		//Object1->SetSprite("TINTEDROCKS2");
		//ARoomObject* Object2 = MinionRoom3->CreateObject<ARock>(nullptr, { -180, -100 });
		//ARoomObject* Object3 = MinionRoom3->CreateObject<ARock>(nullptr, { 180, -100 });
		//MinionRoom3->CreateMonster<AHost>({ 220, 50 });
		//MinionRoom3->CreateMonster<AHost>({ 0, 150 });
		//MinionRoom3->CreateMonster<AHost>({ -220, 50 });
		//MinionRoom3->CreateMonster<AAttackFly>({ -150, 80 });
		//MinionRoom3->CreateMonster<AAttackFly>({ 150, 80 });		
	}

	// MinionRoom6
	{
		BaseRoom->CreateItem<ABomb>(nullptr, {-310, -180});
		BaseRoom->CreateItem<ABomb>(nullptr, {-260, -180});
		BaseRoom->CreateItem<ABomb>(nullptr, {-310, -135});
		BaseRoom->CreateItem<ABomb>(nullptr, { 310, 180 });
		BaseRoom->CreateItem<ABomb>(nullptr, { 260, 180 });
		BaseRoom->CreateItem<ABomb>(nullptr, { 310, 135 });
		BaseRoom->CreateItem<ABomb>(nullptr, { 0, -135 });

		// 중앙
		ARoomObject* Object0 = BaseRoom->CreateObject<AMetalBlock>(nullptr, { 0, 0 });
		ARoomObject* Object1 = BaseRoom->CreateObject<ARock>(nullptr, { -50, 0 });
		ARoomObject* Object2 = BaseRoom->CreateObject<ARock>(nullptr, { 50, 0 });
		ARoomObject* Object3 = BaseRoom->CreateObject<ARock>(nullptr, { 0, 50 });
		ARoomObject* Object4 = BaseRoom->CreateObject<ARock>(nullptr, { 0, -50 });

		// 좌상단
		ARoomObject* Object11 = BaseRoom->CreateObject<ARock>(nullptr, { -310, -80 });
		ARoomObject* Object12 = BaseRoom->CreateObject<ARock>(nullptr, { -260, -80 });
		ARoomObject* Object13 = BaseRoom->CreateObject<AMetalBlock>(nullptr, { -210, -80 });
		ARoomObject* Object14 = BaseRoom->CreateObject<ARock>(nullptr, { -210, -130 });
		ARoomObject* Object15 = BaseRoom->CreateObject<ARock>(nullptr, { -260, -130 });
		ARoomObject* Object16 = BaseRoom->CreateObject<ARock>(nullptr, { -210, -180 });

		// 우하단
		ARoomObject* Object21 = BaseRoom->CreateObject<ARock>(nullptr, { 310, 80 });
		ARoomObject* Object22 = BaseRoom->CreateObject<ARock>(nullptr, { 260, 80 });
		ARoomObject* Object23 = BaseRoom->CreateObject<AMetalBlock>(nullptr, { 210, 80 });
		ARoomObject* Object24 = BaseRoom->CreateObject<ARock>(nullptr, { 210, 130 });
		ARoomObject* Object25 = BaseRoom->CreateObject<ARock>(nullptr, { 260, 130 });
		ARoomObject* Object26 = BaseRoom->CreateObject<ARock>(nullptr, { 210, 180 });
		Object1->SetSprite("TINTEDROCKS2");
		Object11->SetSprite("TINTEDROCKS2");
		Object12->SetSprite("TINTEDROCKS1");
		Object14->SetSprite("TINTEDROCKS1");
		Object21->SetSprite("TINTEDROCKS2");
		Object22->SetSprite("TINTEDROCKS1");
		Object24->SetSprite("TINTEDROCKS1");
	}


	// MinionRoom2 : 플레이어 왼쪽
	{
		ARoomObject* Poop0 = MinionRoom2->CreateObject<APoop>(nullptr, { 0, 0 });
		ARoomObject* Poop1 = MinionRoom2->CreateObject<APoop>(nullptr, { -50, -50 });
		ARoomObject* Poop2 = MinionRoom2->CreateObject<APoop>(nullptr, { 50, 50 });
		ARoomObject* Poop3 = MinionRoom2->CreateObject<APoop>(nullptr, { -50, 50 });
		ARoomObject* Poop4 = MinionRoom2->CreateObject<APoop>(nullptr, { 50, -50 });
		Poop0->SetSprite("CORNY_POOP");
		Poop1->SetSprite("CORNY_POOP");
		Poop2->SetSprite("CORNY_POOP");
		Poop3->SetSprite("CORNY_POOP");
		Poop4->SetSprite("CORNY_POOP");

		MinionRoom2->CreateMonster<ADip>({ 50, 0 });
		MinionRoom2->CreateMonster<ADip>({ -50, 0 });
		MinionRoom2->CreateMonster<ADip>({ 0, -50 });
		MinionRoom2->CreateMonster<ADip>({ 0, 50 });
	}

	//BaseRoom->CreateItem<ABomb>(); // Test
//ARoomObject* Object00 = BaseRoom->CreateObject<ARock>(nullptr, { -310, -120 });
//ARoomObject* Object01 = BaseRoom->CreateObject<ARock>(nullptr, { -260, -120 });

//ARoomObject* Object02 = BaseRoom->CreateObject<ARock>(nullptr, { -310, -20 });
//ARoomObject* Object03 = BaseRoom->CreateObject<ARock>(nullptr, { -260, -20 });
//ARoomObject* Object04 = BaseRoom->CreateObject<ARock>(nullptr, { -210, -20 });
//ARoomObject* Object05 = BaseRoom->CreateObject<ARock>(nullptr, { -160, -20 });

//ARoomObject* Object06 = BaseRoom->CreateObject<ARock>(nullptr, { -310, 80 });
//ARoomObject* Object07 = BaseRoom->CreateObject<ARock>(nullptr, { -260, 80 });
//ARoomObject* Object08 = BaseRoom->CreateObject<ARock>(nullptr, { -210, 80 });
//ARoomObject* Object09 = BaseRoom->CreateObject<ARock>(nullptr, { -210, 80 });
//ARoomObject* Object10 = BaseRoom->CreateObject<ARock>(nullptr, { -160, 80 });
//ARoomObject* Object11 = BaseRoom->CreateObject<ARock>(nullptr, { -110, 80 });
//ARoomObject* Object12 = BaseRoom->CreateObject<ARock>(nullptr, { -60, 80 });

//ARoomObject* Object = BaseRoom->CreateObject<ARock>(nullptr, { -310, 180 });
//aObject1->SetSprite("TINTEDROCKS1");
	
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
		for (int i = 0; i < 3; i++)
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
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_Body); // 몬스터는 오브젝트를 통과할 수 없다. 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_BodyNonCollision); // 그러나 Fly타입 몬스터는 통과한다.

	// Item
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Room_Wall);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_FlyingBody);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Monster_FlyingBodyNonCollision);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_UniversalHit, ECollisionGroup::Object);
}

void APlayGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	EngineDebug(_DeltaTime);
	CheckInput();

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
	ABannerTextUI* TextUI = GetWorld()->SpawnActor<ABannerTextUI>();
	TextUI->SetName("TextUI");
	TextUI->SetTextSpriteName("banner.png");
	TextUI->SetOrder(ERenderOrder::UI);
	TextUI->SetTextScale({ 16, 10 });
	TextUI->SetActorLocation({ 400, 100 });
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

#else
	GamePaused = true;
	USpriteRenderer* FadeRenderer = AFade::MainFade->GetRenderer();

	LoadingImage = GetWorld()->SpawnActor<ALoadingScene>();
	USpriteRenderer* LoadingRenderer = LoadingImage->GetRenderer();

	FadeRenderer->SetActive(true);
	LoadingRenderer->SetActive(true);

	TimeEventer.PushEvent(3.0f, std::bind(&APlayGameMode::FadeOut, this));
#endif // DEBUG
}

void APlayGameMode::FadeOut()
{
#ifdef _DEBUG

#else
	AFade::MainFade->FadeOut();
	LoadingImage->FadeOut();

	TimeEventer.PushEvent(5.0f, std::bind(&APlayGameMode::FadeActiveFalse, this));
	LoadingImage->Destroy(5.0f);
	GamePaused = false; // 여기서 플레이어가 이동 가능
#endif // _DEBUG

}

void APlayGameMode::FadeActiveFalse()
{
	// Fade가 화면에 렌더될 때 굉장히 많은 부하를 일으켜서 사용을 다 하고 나면 반드시 Active를 비활성화 해야한다.
#ifdef _DEBUG

#else
	AFade::MainFade->SetActive(false);
#endif // _DEBUG
}

APlayGameMode::APlayGameMode()
{
}

APlayGameMode::~APlayGameMode()
{
}
