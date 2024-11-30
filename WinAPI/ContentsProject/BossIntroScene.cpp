#include "PreCompile.h"
#include "BossIntroScene.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Fade.h"
#include "PlayGameMode.h"

ABossIntroScene::ABossIntroScene()
{
	float PosOffset = 100.0f;

	PlayertitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PlayertitleRenderer->CreateAnimation("Player", "Isaac", { 0, 1, 2, 3 }, { 0.6f, 0.3f, 0.3f, 0.3f }, false);
	PlayertitleRenderer->SetOrder(ERenderOrder::Loading);
	PlayertitleRenderer->SetComponentScale({ 300, 150 });
	PlayertitleRenderer->SetComponentLocation({ -PosOffset , Global::WindowSize.Y * 0.17f });
	PlayertitleRenderer->ChangeAnimation("Player");
	PlayertitleRenderer->SetActive(false);
	PlayertitleRenderer->SetCameraEffect(false);

	BosstitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BosstitleRenderer->CreateAnimation("Boss", "Duke", { 0, 1, 2, 3 }, { 0.6f, 0.3f, 0.3f, 0.3f }, false);
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
	PlayerSpotRenderer->CreateAnimation("Player", "Spot", {1, 0, 1}, {0.9f, 0.3f, 0.3f}, false);
	PlayerSpotRenderer->SetOrder(ERenderOrder::Loading_Back);
	PlayerSpotRenderer->SetComponentScale({ 378, 101 });
	PlayerSpotRenderer->SetComponentLocation({ -PosOffset , Global::WindowSize.Y * 0.76f });
	PlayerSpotRenderer->ChangeAnimation("Player");
	PlayerSpotRenderer->SetActive(false);
	PlayerSpotRenderer->SetCameraEffect(false);

	BossSpotRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BossSpotRenderer->CreateAnimation("Boss", "Spot", { 1, 0, 1 }, { 0.9f, 0.3f, 0.3f }, false);
	BossSpotRenderer->SetOrder(ERenderOrder::Loading_Back);
	BossSpotRenderer->SetComponentScale({ 430, 115 });
	BossSpotRenderer->SetComponentLocation({ Global::WindowSize.X + PosOffset + 20 , Global::WindowSize.Y * 0.8f });
	BossSpotRenderer->ChangeAnimation("Boss");
	BossSpotRenderer->SetActive(false);
	BossSpotRenderer->SetCameraEffect(false);

	VsScale = { 810, 405 }; // 270, 135
	float AnimationTime = 0.023f;
	float SlowAnimTime = 0.3f;
	VsRenderer = CreateDefaultSubObject<USpriteRenderer>();
	VsRenderer->CreateAnimation("Vs", "Vs", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }, { SlowAnimTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, AnimationTime, SlowAnimTime, SlowAnimTime, SlowAnimTime, SlowAnimTime, SlowAnimTime }, false);
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

	PlayerDir = FVector2D::RIGHT;
	BossDir = FVector2D::RIGHT;
	UEngineSound::AllSoundStop();  
	Sound = UEngineSound::Play("boss_fight_intro_jingle_01.ogg");
	Sound = UEngineSound::Play("bossIntro.wav");
}

void ABossIntroScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Move(_DeltaTime);
	AnimateScale(_DeltaTime);
}

void ABossIntroScene::Move(float _DeltaTime)
{
	FVector2D ReversePlayerDir = PlayerDir;
	FVector2D ReverseBossDir = -BossDir;

	float Force = 500.0f;
	int OffestX = 100;

	FVector2D CurBossPos = BosstitleRenderer->GetComponentLocation() - BosstitleRenderer->GetComponentScale().Half();
	FVector2D TargetBossPos = { Global::WindowHalfScale.iX() + OffestX, 0};


	if (false == ReverseMove) // 인트로 이미지 입장
	{
		if (TargetBossPos.X >= CurBossPos.X) 
		{
			// 목표 위치에 도달하면 정지
			BossDir = FVector2D::ZERO;
			PlayerDir = FVector2D::ZERO;

			// 아이작 이미지의 떨림
			Oscillation(_DeltaTime);

			// 이미지가 화면 위치에 도착 후 일정 시간이 지나면 퇴장을 위한 방향 부여
			TimeEventer.PushEvent(IntroTime, [this]() {
				ReverseMove = true;
				PlayerDir = FVector2D::RIGHT;
				BossDir = FVector2D::RIGHT;
				FadeOut();});
		}
	}
	else if (true == ReverseMove) // 퇴장
	{
		ReversePlayerDir = -PlayerDir;
		ReverseBossDir = BossDir;
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

void ABossIntroScene::AnimateScale(float _DeltaTime)
{
	FVector2D VsScaleValue = VsScaleOffset;
	VsScaleValue.Normalize();

	float ScaleForce = (true == IsShrinking ? -1000.0f : 0.0f);

	FVector2D ScaleDelta = VsScaleValue * _DeltaTime * ScaleForce;

	VsRenderer->AddComponentScale(ScaleDelta);

	FVector2D VsRendererScale = VsRenderer->GetComponentScale();
	if (true == IsShrinking) // 축소 중일 때
	{
		// 최소 크기에 도달하면
		if (VsRendererScale.X <= MinScale.X && VsRendererScale.Y < MinScale.Y)
		{
			VsRenderer->SetComponentScale(MinScale);
		}
	}
}

void ABossIntroScene::Oscillation(float _DeltaTime)
{
	// 진동 속도와 강도 설정
	const float OscillationSpeed = 1000.0f; // 진동 속도
	const float OscillationMagnitude = 5.0f; // 진동 크기

	// 사인 함수를 이용해 진동 계산
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 위치 업데이트
	FVector2D NewScale = { 224.0f + Oscillation, 156.0f + Oscillation };
	PlayerRenderer->SetComponentScale(NewScale);
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
	        VsRenderer->SetActive(false);

	AFade::MainFade->GetRenderer()->SetActive(false);
	APlayGameMode::SetGamePaused(false);
}

void ABossIntroScene::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(FadeTime, std::bind(&ABossIntroScene::FadeChange, this), true, false);
	TimeEventer.PushEvent(FadeTime, std::bind(&ABossIntroScene::CloseScene, this));
}

void ABossIntroScene::FadeIn()
{
	FadeValue = 0.3f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(FadeTime, std::bind(&ABossIntroScene::VsRendererFadeChange, this), true, false);
}

void ABossIntroScene::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.9f * FadeDir;
	PlayertitleRenderer->SetAlphaFloat(FadeValue);
	  BosstitleRenderer->SetAlphaFloat(FadeValue);
	     PlayerRenderer->SetAlphaFloat(FadeValue);
	       BossRenderer->SetAlphaFloat(FadeValue);
	 PlayerSpotRenderer->SetAlphaFloat(FadeValue);
	   BossSpotRenderer->SetAlphaFloat(FadeValue);
	         VsRenderer->SetAlphaFloat(FadeValue);
			 int a = 0;
}

void ABossIntroScene::VsRendererFadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.9f * FadeDir;
	VsRenderer->SetAlphaFloat(FadeValue);
}

ABossIntroScene::~ABossIntroScene()
{
	UEngineSound::AllSoundStop();
	BossFightBGM = UEngineSound::Play("basic_boss_fight.ogg");
	BossFightBGM.Loop(999);
}
