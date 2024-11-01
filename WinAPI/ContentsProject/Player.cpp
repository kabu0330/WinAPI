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
	// Actor�� ��ġ�� �ǹ̰� �־ ũ��� �ǹ̰� ����.
	SetActorLocation(Global::WindowSize.Half());
	//SetSprite("icon.png");

	// �������� �ϳ� �����.
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

	// ���� ī�޶� �Ǻ��� ��������� �Ѵ�.
	GetWorld()->SetCameraToMainPawn(false);
	//FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	//GetWorld()->SetCameraPivot(Size.Half() * -1.0f);

	// ���ǻ��� : �ڱⰡ �ڱ� �ڽ��� SpawnActor�ϸ� ���� ������ ���� ���� �����÷ο�
	// �Է� ��� 2 : �̺�Ʈ ������� ó��
	//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	EngineDebug();
	//this;
	// �Է� ��� 1
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
		// �÷��̾ ���� ������ Bullet�� �����Ѵ�.
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

// �Է� ��� 2 : �̺�Ʈ ������� ó��
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

