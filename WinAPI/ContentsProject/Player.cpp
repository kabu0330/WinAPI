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
#include "Room.h"
#include "PlayGameMode.h"

void APlayer::RunSoundPlay()
{
	UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	// 1. Actor�� ��ġ�� �ǹ̰� �־ ũ��� �ǹ̰� ����.
	SetActorLocation(Global::WindowSize.Half());

	// 2. ���¿� ���� �ִϸ��̼� ������ �����Ѵ�.
	SpriteSetting();

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

	Move(_DeltaTime);

	CameraMoveTime = 1.0f;

	if (UEngineInput::GetInst().IsDown('U'))
	{
		CameraMoveDir = FVector2D::UP;
		CameraMove = true;
		FVector2D RoomScale = Global::WindowScale;
		StartCameraPos = GetWorld()->GetCameraPos();
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('H'))
	{
		CameraMoveDir = FVector2D::LEFT;
		CameraMove = true;
		FVector2D RoomScale = Global::WindowScale;
		StartCameraPos = GetWorld()->GetCameraPos();
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('K'))
	{
		CameraMoveDir = FVector2D::RIGHT;
		CameraMove = true;
		FVector2D RoomScale = Global::WindowScale;
		StartCameraPos = GetWorld()->GetCameraPos();
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('J'))
	{
		CameraMoveDir = FVector2D::DOWN;
		CameraMove = true;
		FVector2D RoomScale = Global::WindowScale;
		StartCameraPos = GetWorld()->GetCameraPos();
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}

	if (true == CameraMove)
	{
		FVector2D CamPos = FVector2D::Lerp(StartCameraPos, EndCameraPos, CameraMoveTime);

		GetWorld()->SetCameraPos(CamPos);

	}


	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// �÷��̾ ���� ������ Bullet�� �����Ѵ�.
		ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}
}

void APlayer::Move(float _DeltaTime)
{
	// �Է� ��� 1
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		State = State::LEFT;
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);

	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		State = State::RIGHT;
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		State = State::UP;
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);

	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		State = State::DOWN;
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		State = State::IDLE;
	}

	FVector2D Dir = GetTransform().Location;
	Dir = FVector2D::ZERO;

	switch (State)
	{
	case APlayer::State::IDLE:
	{
		Dir = FVector2D::ZERO;
		BodyRenderer->ChangeAnimation("Body_Idle");
		HeadRenderer->ChangeAnimation("Head_Down");
		break;
	}
	case APlayer::State::LEFT:
	{
		Dir = FVector2D::LEFT;
		BodyRenderer->ChangeAnimation("Body_Left");
		HeadRenderer->ChangeAnimation("Head_Left");
		break;
	}
	case APlayer::State::RIGHT:
	{
		Dir = FVector2D::RIGHT;
		BodyRenderer->ChangeAnimation("Body_Right");
		HeadRenderer->ChangeAnimation("Head_RIght");
		break;
	}
	case APlayer::State::UP:
	{
		Dir = FVector2D::UP;
		BodyRenderer->ChangeAnimation("Body_Up");
		HeadRenderer->ChangeAnimation("Head_Up");
		break;
	}
	case APlayer::State::DOWN:
	{
		Dir = FVector2D::DOWN;
		BodyRenderer->ChangeAnimation("Body_Down");
		HeadRenderer->ChangeAnimation("Head_Down");
		break;
	}
	case APlayer::State::MAX:
		break;
	default:
		break;
	}
}

void APlayer::CameraRoomMove(float _DeltaTime)
{

}

void APlayer::SpriteSetting()
{
	// 1. ����� �������� �ϳ� �����.
	// 2. CreateDefualtSubObject �Լ��� ����Ͽ� ������ ������Ʈ�� �����.
	// 3. CreateAnimation �Լ��� ����Ͽ� ("���� �̸�", "�̹���.png", ���ۼ���, ������)�� �����Ѵ�.
	// 4. SetComponentScale �Լ��� ����Ͽ� ������ ������Ʈ�� ũ�⸦ �����Ѵ�.
	// 5. ChanageAnimation �Լ��� ����Ͽ� �⺻ ������ �����Ѵ�.
	// 6. SetOrder�� ���� ������ ���� �� �ִ�.

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Body_Left", "Body.png", 1, 9, 0.05f);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, 0.05f);
	BodyRenderer->CreateAnimation("Body_Down", "Body.png", 20, 29, 0.05f);
	BodyRenderer->CreateAnimation("Body_Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, 0.05f);
	BodyRenderer->CreateAnimation("Body_Idle", "Body.png", 29, 29, 0.05f);

	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Body_Idle");

	/////////////////////////////////////////////////////////////////////////////// 

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", "Head.png", 1, 1, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right", "Head.png", 3, 3, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down", "Head.png", 7, 7, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up", "Head.png", 5, 5, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left", "Head.png", 0, 1, 0.5f);
	HeadRenderer->CreateAnimation("Head_Attack_Right", "Head.png", 2, 3, 0.5f);
	HeadRenderer->CreateAnimation("Head_Attack_Down", "Head.png", 6, 7, 0.5f);
	HeadRenderer->CreateAnimation("Head_Attack_Up", "Head.png", 4, 5, 0.5f);

	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 4 });
	HeadRenderer->SetComponentScale({ 64, 64 }); 
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::PLAYER);
	HeadRenderer->SetOrder(ERenderOrder::PLAYER);
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

