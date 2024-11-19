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

#include "Item.h"
#include "Heart.h"
#include "Bomb.h"

bool APlayGameMode::GamePaused = false;

void APlayGameMode::BeginPlay()
{
	// 레벨이 만들어지면 최초 1회 세팅은 여기서 한다.
	
	GetWorld()->SetCameraPos({0, 0});
	GetWorld()->SetCameraToMainPawn(false);
	//GetWorld()->SetCameraPos(GetWorld()->GetPawn()->GetActorLocation());
	//GetWorld()->SetCameraToMainPawn(true); // 카메라가 플레이어 추적

	Spawn();

	CollisionGroupLinkSetting();
	UISetting();
}

void APlayGameMode::Spawn()
{
	// Room
	ARoom* BaseRoom = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom = GetWorld()->SpawnActor<ARoom>();

	BaseRoom->InterLinkRoom(MinionRoom0, RoomDir::LEFT);
	BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::RIGHT);
	BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::UP);
	BaseRoom->InterLinkRoom(MinionRoom3, RoomDir::DOWN);
	MinionRoom3->InterLinkRoom(BossRoom, RoomDir::DOWN);

	BaseRoom->SetName("BaseRoom");
	MinionRoom0->SetName("MinionRoom0");
	MinionRoom1->SetName("MinionRoom1");
	MinionRoom2->SetName("MinionRoom2");
	MinionRoom3->SetName("MinionRoom3");
	BossRoom->SetName("BossRoom");

	ARoom::SetCurRoom(BaseRoom);

	// Monster
	//MinionRoom0->CreateMonster<AAttackFly>({ 150, 0 });
	//MinionRoom0->CreateMonster<AAttackFly>({ 150, 100 });
	//MinionRoom0->CreateMonster<AAttackFly>({ 50, 50 });
	//MinionRoom0->CreateMonster<AFly>({ 100, 30 });
	//MinionRoom0->CreateMonster<AFly>({ 100, -50 });
	//MinionRoom1->CreateMonster<AHopper>({ 150, 0 });
	//MinionRoom1->CreateMonster<AHopper>({ 100, 100 });
	//MinionRoom2->CreateMonster<AHost>({ 150, 0 });
	//MinionRoom2->CreateMonster<AHost>({ 150, -50 });
	//MinionRoom3->CreateMonster<ADip>({ 150, 50 });
	//MinionRoom3->CreateMonster<ADip>({ 150, 0 });
	//MinionRoom3->CreateMonster<APooter>({ 100, 100 });
	//MinionRoom3->CreateMonster<APooter>({ 250, 50 });
	//BossRoom->CreateMonster<ATheDukeOfFlies>({ 150, 0 });
	
	//BaseRoom->CreateMonster<ATheDukeOfFlies>({ 150, 0 });

	// Object
	//ARoomObject* MetalBlock = BaseRoom->CreateObject<ARock>({100, 0});
	//MetalBlock->SetSprite("METALBLOCKS");

	//ARoomObject* Poop = BaseRoom->CreateObject <APoop>({ 100, 0 });
	////Poop->SetSprite("CORNY_POOP");
	//Poop->SetSprite("GOLDEN_POOP");

	//ARoomObject* Poop = BaseRoom->CreateObject<AFire>({ 100, 0 });
	//AItem* Heart = BaseRoom->CreateItem<AHeart>(nullptr, { 100, 0 });
	//AItem* HalfHeart = BaseRoom->CreateItem<AHeart>(nullptr, { -100, 0 });
	//HalfHeart->ChangeAnimation("HalfHeart");

	AItem* Heart = BaseRoom->CreateItem<ABomb>(nullptr, { 100, 0 });
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
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_FlyingBody); // 플레이어는 공중의 몬스터도 데미지를 입힌다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_FlyingBodyNonCollision);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Monster_Barrier); // 플레이어의 공격을 막으며 즉시 터진다.
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Object);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Room_Wall); // 플레이어의 공격이 벽에 닿으면 즉시 터진다.

	// Monster
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Body, ECollisionGroup::Room_Wall); // 몬스터는 맵 밖을 나갈 수 없다.

	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Player_Body); 
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Room_Wall); 

	// Object
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Player_Warp);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Object, ECollisionGroup::Monster_BodyNonCollision);

	// Item
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item     , ECollisionGroup::Room_Wall);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_Bomb, ECollisionGroup::Player_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Item_Bomb, ECollisionGroup::Monster_Body);
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
	UEngineDebug::CoreOutPutString("Player Pos : " + GetWorld()->GetPawn()->GetActorLocation().ToString(), { 0, Global::WindowSize.iY() - 60 });
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
