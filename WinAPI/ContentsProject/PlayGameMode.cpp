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

bool APlayGameMode::GamePaused = false;

void APlayGameMode::BeginPlay()
{
	// ������ ��������� ���� 1ȸ ������ ���⼭ �Ѵ�.
	
	// Player�� BeginPlay �Լ������� ���� ������ ȿ���� �� �� ������
	// Player�μ��� ������ �������� �����ֱ� ���ؼ� Player�� ����� �ƴϸ� ��� ���⼭ �����.
	GetWorld()->SetCameraPos({0, 0});
	GetWorld()->SetCameraToMainPawn(false);
	//GetWorld()->SetCameraPos(GetWorld()->GetPawn()->GetActorLocation());
	//GetWorld()->SetCameraToMainPawn(true); // ī�޶� �÷��̾� ����

	ARoom* BaseRoom    = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom0 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom1 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom2 = GetWorld()->SpawnActor<ARoom>();
	ARoom* MinionRoom3 = GetWorld()->SpawnActor<ARoom>();
	ARoom* BossRoom    = GetWorld()->SpawnActor<ARoom>();

	   BaseRoom->InterLinkRoom(MinionRoom0, RoomDir::LEFT );
	   BaseRoom->InterLinkRoom(MinionRoom1, RoomDir::RIGHT);
	   BaseRoom->InterLinkRoom(MinionRoom2, RoomDir::UP   );
	   BaseRoom->InterLinkRoom(MinionRoom3, RoomDir::DOWN );
	MinionRoom3->InterLinkRoom(BossRoom   , RoomDir::DOWN );

	   BaseRoom->SetName("BaseRoom"   );
	MinionRoom0->SetName("MinionRoom0");
	MinionRoom1->SetName("MinionRoom1");
	MinionRoom2->SetName("MinionRoom2");
	MinionRoom3->SetName("MinionRoom3");
	   BossRoom->SetName("BossRoom"   );
	
	   ARoom::SetCurRoom(BaseRoom);

	// Monster
	//AMonster* TestMonster = GetWorld()->SpawnActor<AMonster>();
	AMonster* AttackMonster = GetWorld()->SpawnActor<AAttackFly>();


	CollisionGroupLinkSetting();
	UISetting();
}

void APlayGameMode::CollisionGroupLinkSetting()
{
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Warp, ECollisionGroup::Object_Wall);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Warp, ECollisionGroup::Warp);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Body, ECollisionGroup::Monster_Body);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Player_Attack, ECollisionGroup::Object_Wall);

	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Body, ECollisionGroup::Object_Wall);
	GetWorld()->CollisionGroupLink(ECollisionGroup::Monster_Attack, ECollisionGroup::Object_Wall);
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

	// �÷��̾ ������ �޴� â ������.
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
	TextUI->SetTextSpriteName("banner.png");
	TextUI->SetOrder(ERenderOrder::UI);
	TextUI->SetTextScale({ 16, 10 });
	TextUI->SetActorLocation({ 400, 100 });
	//TextUI->SetTextOut("TheBindingOfIsaac"); // ���� ���� �ذ� ���?

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
	GamePaused = false; // ���⼭ �÷��̾ �̵� ����
#endif // _DEBUG

}

void APlayGameMode::FadeActiveFalse()
{
	// Fade�� ȭ�鿡 ������ �� ������ ���� ���ϸ� �����Ѽ� ����� �� �ϰ� ���� �ݵ�� Active�� ��Ȱ��ȭ �ؾ��Ѵ�.
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
