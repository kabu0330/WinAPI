#include "PreCompile.h"
#include "Player.h"
#include <string>
#include <vector>

#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Tear.h"
#include "Global.h"
#include "ContentsEnum.h"

void APlayer::RunSoundPlay()
{
	UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	// 1. Actor�� ��ġ�� �ǹ̰� �־ ũ��� �ǹ̰� ����.
	SetActorLocation(Global::WindowSize.Half());

	// 2. ���¿� ���� �ִϸ��̼� ������ �����Ѵ�.
	AnimationSetting();

	// 3. ĳ������ �̵������� ������ �浹ü�� �����Ѵ�.
 
	// 4. ĳ������ ��Ʈ�ڽ��� ������ �浹ü�� �����Ѵ�.
	// CreateDefaultSubObject<U2DCollision>();
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	// ���� ī�޶� �Ǻ��� ��������� �Ѵ�.
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
	Super::Tick(_DeltaTime);

	EngineDebug();

	// �Է� ��� 1
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		BodyRenderer->ChangeAnimation("Body_Left");
		HeadRenderer->ChangeAnimation("Head_Left");
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		BodyRenderer->ChangeAnimation("Body_Right");
		HeadRenderer->ChangeAnimation("Head_RIght");
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		BodyRenderer->ChangeAnimation("Body_Up");
		HeadRenderer->ChangeAnimation("Head_Up");
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		BodyRenderer->ChangeAnimation("Body_Down");
		HeadRenderer->ChangeAnimation("Head_Down");
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		BodyRenderer->ChangeAnimation("Body_Idle");
		HeadRenderer->ChangeAnimation("Head_Down");
	}

	// �밢�� �̵��� ��� �� ���ΰ�?

	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// �÷��̾ ���� ������ Bullet�� �����Ѵ�.
		ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}

	//if (true == UEngineInput::GetInst().IsDown('R'))
	//{
	//	SetSprite("icon.png", MySpriteIndex);
	//	++MySpriteIndex;
	//}
}

void APlayer::AnimationSetting()
{
	// 1. ����� �������� �ϳ� �����.
	// 2. CreateDefualtSubObject �Լ��� ����Ͽ� ������ ������Ʈ�� �����.
	// 3. CreateAnimation �Լ��� ����Ͽ� ("���� �̸�", "�̹���.png", ���ۼ���, ������)�� �����Ѵ�.
	// 4. SetComponentScale �Լ��� ����Ͽ� ������ ������Ʈ�� ũ�⸦ �����Ѵ�.
	// 5. ChanageAnimation �Լ��� ����Ͽ� �⺻ ������ �����Ѵ�.
	// 6. SetOrder�� ���� ������ ���� �� �ִ�.

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Body_Left", "Body.png", 1, 9, 0.1f);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, 0.1f);
	BodyRenderer->CreateAnimation("Body_Down", "Body.png", 20, 29, 0.1f);
	BodyRenderer->CreateAnimation("Body_Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f });
	BodyRenderer->CreateAnimation("Body_Idle", "Body.png", 29, 29, 0.1f);
	BodyRenderer->SetComponentScale({ 45, 45 });
	BodyRenderer->ChangeAnimation("Body_Idle");

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", "Head.png", 0, 1, 0.5f);
	HeadRenderer->CreateAnimation("Head_Right", "Head.png", 2, 3, 0.5f);
	HeadRenderer->CreateAnimation("Head_Down", "Head.png", 6, 7, 0.5f);
	HeadRenderer->CreateAnimation("Head_Up", "Head.png", 4, 5, 0.5f);
	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 3 });
	HeadRenderer->SetComponentScale({ 45, 45 });
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::PLAYER);
	HeadRenderer->SetOrder(ERenderOrder::PLAYER);
	//SpriteRenderer->SetComponentLocation({ -100, 0 });


	//SpriteRenderer->CreateAnimation("Run_Right", "Player_Right.png", { 2, 3, 4 }, { 0.1f, 0.1f, 0.1f });
	//SpriteRenderer->ChangeAnimation("Idle_Right");
	//SpriteRenderer->ChangeAnimation("Idle_Right");
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

