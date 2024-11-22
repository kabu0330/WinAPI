#include "PreCompile.h"
#include "BossIntroScene.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Fade.h"

ABossIntroScene::ABossIntroScene()
{
	float PosOffset = 100.0f;

	PlayertitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PlayertitleRenderer->CreateAnimation("Player", "Isaac", { 0, 1, 2, 3 }, 0.3f, false);
	PlayertitleRenderer->SetOrder(ERenderOrder::Loading);
	PlayertitleRenderer->SetComponentScale({ 300, 150 });
	PlayertitleRenderer->SetComponentLocation({ -PosOffset , Global::WindowSize.Y * 0.17f });
	PlayertitleRenderer->ChangeAnimation("Player");
	PlayertitleRenderer->SetActive(false);
	PlayertitleRenderer->SetCameraEffect(false);

	BosstitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BosstitleRenderer->CreateAnimation("Boss", "Duke", { 0, 1, 2, 3 }, 0.3f, false);
	BosstitleRenderer->SetOrder(ERenderOrder::Loading);
	BosstitleRenderer->SetComponentScale({ 330, 165});
	BosstitleRenderer->SetComponentLocation({Global::WindowSize.X + PosOffset , Global::WindowSize.Y * 0.15f});
	BosstitleRenderer->ChangeAnimation("Boss");
	BosstitleRenderer->SetActive(false);
	BosstitleRenderer->SetCameraEffect(false);


	PlayerRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PlayerRenderer->SetSprite("issacportrait.png");
	PlayerRenderer->SetOrder(ERenderOrder::Loading);
	PlayerRenderer->SetComponentScale({ 224, 156 });
	PlayerRenderer->SetComponentLocation({ -PosOffset , Global::WindowSize.Y * 0.65f });
	PlayerRenderer->SetActive(false);
	PlayerRenderer->SetCameraEffect(false);

	BossRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BossRenderer->SetSprite("dukeofflies.png");
	BossRenderer->SetOrder(ERenderOrder::Loading);
	BossRenderer->SetComponentScale({ 384, 384 });
	BossRenderer->SetComponentLocation({ Global::WindowSize.X + PosOffset + 20 , Global::WindowSize.Y * 0.55f });
	BossRenderer->SetActive(false);
	BossRenderer->SetCameraEffect(false);


	PlayerSpotRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PlayerSpotRenderer->CreateAnimation("Player", "Spot", {1, 0, 1}, 0.5f, false);
	PlayerSpotRenderer->SetOrder(ERenderOrder::Loading_Back);
	PlayerSpotRenderer->SetComponentScale({ 378, 101 });
	PlayerSpotRenderer->SetComponentLocation({ -PosOffset , Global::WindowSize.Y * 0.76f });
	PlayerSpotRenderer->ChangeAnimation("Player");
	PlayerSpotRenderer->SetActive(false);
	PlayerSpotRenderer->SetCameraEffect(false);

	BossSpotRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BossSpotRenderer->CreateAnimation("Boss", "Spot", { 1, 0, 1 }, 1.0f, false);
	BossSpotRenderer->SetOrder(ERenderOrder::Loading_Back);
	BossSpotRenderer->SetComponentScale({ 430, 115 });
	BossSpotRenderer->SetComponentLocation({ Global::WindowSize.X + PosOffset + 20 , Global::WindowSize.Y * 0.8f });
	BossSpotRenderer->ChangeAnimation("Boss");
	BossSpotRenderer->SetActive(false);
	BossSpotRenderer->SetCameraEffect(false);

	VsScale = { 405, 203 }; // 270, 135
	VsRenderer = CreateDefaultSubObject<USpriteRenderer>();
	VsRenderer->CreateAnimation("Vs", "Vs", 0, 17, 0.03f, false);
	VsRenderer->SetOrder(ERenderOrder::Loading);
	VsRenderer->SetComponentScale(VsScale);
	VsRenderer->SetComponentLocation({ Global::WindowHalfScale.X , Global::WindowSize.Y * 0.20f });
	VsRenderer->ChangeAnimation("Vs");
	VsRenderer->SetActive(false);
	VsRenderer->SetCameraEffect(false);

	VsScaleOffset = VsScale;

}

void ABossIntroScene::BeginPlay()
{
	Super::BeginPlay();

	IntroTime = 3.0f;
	FadeTime = 2.0f;

	PlayerDir = FVector2D::RIGHT;
	BossDir = FVector2D::RIGHT;

}

void ABossIntroScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Move(_DeltaTime);
}

void ABossIntroScene::Move(float _DeltaTime)
{
	FVector2D ReversePlayerDir = PlayerDir;
	FVector2D ReverseBossDir = -BossDir;

	ReversePlayerDir.Normalize();
	ReverseBossDir.Normalize();

	float Force = 500.0f;
	int OffestX = 100;
	FVector2D CurPlayerPos = PlayertitleRenderer->GetComponentLocation() + PlayertitleRenderer->GetComponentScale().Half();
	FVector2D TargetPlayerPos = { Global::WindowHalfScale.iX() - OffestX, 0 };

	FVector2D CurBossPos = BosstitleRenderer->GetComponentLocation() - BosstitleRenderer->GetComponentScale().Half();
	FVector2D TargetBossPos = { Global::WindowHalfScale.iX() + OffestX, 0};


	if (TargetBossPos.X >= CurBossPos.X)
	{
		BossDir = FVector2D::ZERO;
		PlayerDir = FVector2D::ZERO;
	}
	if (10.0f > ReverseBossDir.Length())
	{
		float ScaleForce = -200.0f;
		VsScaleOffset.Normalize();

		if (270 > VsScale.X)
		{
			int a = 0;
		}
		VsScaleOffset += VsScaleOffset * _DeltaTime * ScaleForce;
		VsRenderer->AddComponentScale(VsScaleOffset);

		//TimeEventer.PushEvent(IntroTime, [this]() {
		//	float FullbackForce = 10.0f;
		//	BossDir = FVector2D::LEFT * FullbackForce;
		//	PlayerDir = FVector2D::RIGHT * FullbackForce; });
	}

	ReversePlayerDir += ReversePlayerDir * _DeltaTime * Force;
	ReverseBossDir +=   ReverseBossDir * _DeltaTime * Force;

	PlayerRenderer->AddComponentLocation(ReversePlayerDir);
	PlayertitleRenderer->AddComponentLocation(ReversePlayerDir);
	PlayerSpotRenderer->AddComponentLocation(ReversePlayerDir);

	BossRenderer->AddComponentLocation(ReverseBossDir);
	BosstitleRenderer->AddComponentLocation(ReverseBossDir);
	BossSpotRenderer->AddComponentLocation(ReverseBossDir);


}

void ABossIntroScene::ShowScene()
{
	    PlayertitleRenderer->SetActive(true);
	      BosstitleRenderer->SetActive(true);
		  PlayerRenderer->SetActive(true);
		  BossRenderer->SetActive(true);
	PlayerSpotRenderer->SetActive(true);
	  BossSpotRenderer->SetActive(true);
	        VsRenderer->SetActive(true);

	AFade::MainFade->GetRenderer()->SetAlphaFloat(1.0f);
	AFade::MainFade->GetRenderer()->SetActive(true);
}

void ABossIntroScene::CloseScene()
{
	    PlayertitleRenderer->SetActive(false);
	      BosstitleRenderer->SetActive(false);
		  PlayerRenderer->SetActive(false);
		  BossRenderer->SetActive(false);
	PlayerSpotRenderer->SetActive(false);
	  BossSpotRenderer->SetActive(false);
	//        VsRenderer->SetActive(false);

	AFade::MainFade->GetRenderer()->SetActive(false);
}

void ABossIntroScene::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(FadeTime, std::bind(&ABossIntroScene::FadeChange, this), true, false);
}

void ABossIntroScene::FadeIn()
{
	FadeValue = 1.0f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(FadeTime, std::bind(&ABossIntroScene::VsRendererFadeChange, this), true, false);
}

void ABossIntroScene::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5f * FadeDir;
	    PlayertitleRenderer->SetAlphaFloat(FadeValue);
	      BosstitleRenderer->SetAlphaFloat(FadeValue);
	//PlayerSpotRenderer->SetAlphaFloat(FadeValue);
	//  BossSpotRenderer->SetAlphaFloat(FadeValue);
	//        VsRenderer->SetAlphaFloat(FadeValue);
}

void ABossIntroScene::VsRendererFadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.5f * FadeDir;
	VsRenderer->SetAlphaFloat(FadeValue);
}

ABossIntroScene::~ABossIntroScene()
{
}
