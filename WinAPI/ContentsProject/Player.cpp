#include "PreCompile.h"
#include "Player.h"
#include <string>

#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Tear.h"
#include "Global.h"

void APlayer::RunSoundPlay()
{
	UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	// Actor의 위치는 의미가 있어도 크기는 의미가 없다.
	SetActorLocation(Global::WindowSize.Half());
	//SetSprite("icon.png");

	// 랜더러를 하나 만든다.
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//SpriteRenderer->SetSprite("icon.png");
	SpriteRenderer->CreateAnimation("Idle_Left", "Body.png", 1, 9, 0.1f);
	SpriteRenderer->CreateAnimation("Idle_Right", "Body.png", 1, 9, 0.1f);
	SpriteRenderer->SetComponentScale({ 45, 45 });
	SpriteRenderer->ChangeAnimation("Idle_Left");

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", "Head.png", 0, 1, 0.5f);
	HeadRenderer->SetComponentLocation({ 0, - SpriteRenderer->GetComponentScale().Half().iY() + 2});
	HeadRenderer->SetComponentScale({ 45, 45 });
	HeadRenderer->ChangeAnimation("Head_Left");
	//SpriteRenderer->SetComponentLocation({ -100, 0 });


	//SpriteRenderer->CreateAnimation("Run_Right", "Player_Right.png", { 2, 3, 4 }, { 0.1f, 0.1f, 0.1f });
	//SpriteRenderer->ChangeAnimation("Idle_Right");
	//SpriteRenderer->ChangeAnimation("Idle_Right");

	// CreateDefaultSubObject<U2DCollision>();
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	// 직접 카메라 피봇을 설정해줘야 한다.
	GetWorld()->SetCameraToMainPawn(false);
	//FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	//GetWorld()->SetCameraPivot(Size.Half() * -1.0f);

	// 주의사항 : 자기가 자기 자신을 SpawnActor하면 무한 스폰에 빠져 스택 오버플로우
	// 입력 방법 2 : 이벤트 방식으로 처리
	//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	EngineDebug();
	//this;
	// 입력 방법 1
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		//SpriteRenderer->ChangeAnimation("Run_Right");
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}

	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// 플레이어가 속한 레벨에 Bullet을 생성한다.
        ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		//SpriteRenderer->ChangeAnimation("Idle_Right");
	}

	//if (true == UEngineInput::GetInst().IsDown('R'))
	//{
	//	SetSprite("icon.png", MySpriteIndex);
	//	++MySpriteIndex;
	//}
}

void APlayer::EngineDebug()
{
	UEngineDebug::CoreOutPutString("Player : " + GetActorLocation().ToString());

	if (true == UEngineInput::GetInst().IsDown('B'))
	{
		UEngineDebug::SwitchIsDebug();
	}
}

// 입력 방법 2 : 이벤트 방식으로 처리
//void APlayer::LeftMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
//}
//
//void APlayer::RightMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
//}
//
//void APlayer::UpMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
//}
//
//void APlayer::DownMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
//}

