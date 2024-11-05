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

int APlayer::Hp = 6;

//void APlayer::RunSoundPlay()
//{
//	UEngineDebug::OutPutString("SoundPlay");
//}

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
	InputAttack(_DeltaTime);

	CurStateAnimation(_DeltaTime);

	CameraPosMove(_DeltaTime);
}

void APlayer::Move(float _DeltaTime)
{
	// �� �̵��� �߿� ĳ���ʹ� ������ �� ����.
	if (true == CameraMove)
	{
		return;
	}

	// �Է� ��� 1 : �ܼ� Ű �Է¿� ������ �߰��Ͽ� ó��
	
	// �ڿ������� �̵��ϰ� ���̴� �� : �̵�(����)�� ������ �и��� ��
	// �̵�
	FVector2D Dir = FVector2D::ZERO;
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Dir = FVector2D::LEFT;
		BodyState = LowerState::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Dir = FVector2D::RIGHT;
		BodyState = LowerState::RIGHT;
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Dir = FVector2D::UP;
		BodyState = LowerState::UP;
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Dir = FVector2D::DOWN;
		BodyState = LowerState::DOWN;
	}

	AddActorLocation(Dir * _DeltaTime * Speed);

	if (true == IsAttack())
	{
		int BodyDir = GetAttackDir(); 
		  HeadState = static_cast<UpperState>(BodyDir);
	}
	else
	{
		int BodyDir = static_cast<int>(BodyState); // �̵� ������ ���� ��ȯ
		  HeadState = static_cast<UpperState>(BodyDir);
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		BodyState = LowerState::IDLE;

		// ���� ���°� �ƴϸ� Ű�Է� ������ IDLE�� ��ȯ
		if (false == IsAttack())
		{
			HeadState = UpperState::IDLE; 
		}	
	}
}

void APlayer::CameraPosMove(float _DeltaTime)
{
	FVector2D RoomScale = Global::WindowScale;
	FVector2D PlayerMovePos = GetActorLocation();
	StartCameraPos = GetWorld()->GetCameraPos();

	if (UEngineInput::GetInst().IsDown('H'))
	{
		CameraMove    = true;
		CameraMoveDir = FVector2D::LEFT;
		EndCameraPos  = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('K'))
	{
		CameraMove    = true;
		CameraMoveDir = FVector2D::RIGHT;
		EndCameraPos  = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('U'))
	{
		CameraMove = true;
		CameraMoveDir = FVector2D::UP;
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('J'))
	{
		CameraMove    = true;
		CameraMoveDir = FVector2D::DOWN;
		EndCameraPos  = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}

	if (true == CameraMove)
	{
		LerpAlpha = CameraMoveTime / 1.0f;
		FVector2D CamPos = FVector2D::Lerp(StartCameraPos, EndCameraPos, LerpAlpha);

		GetWorld()->SetCameraPos(CamPos);
		SetActorLocation(PlayerMovePos);

		CameraMoveTime += _DeltaTime;
		if (1.0f <= CameraMoveTime)
		{
			CameraMove     = false;
			CameraMoveTime = 0.0f;

			// �÷��̾ ��� ���� ���� �� �տ� ��ġ��ų�� �����غ���
			//SetActorLocation(���� �� �� ��);
		}
	}
}

void APlayer::InputAttack(float _DeltaTime)
{
	if (false == TearFire && (UEngineInput::GetInst().IsDown(VK_LEFT) ||
		UEngineInput::GetInst().IsDown(VK_RIGHT) ||
		UEngineInput::GetInst().IsDown(VK_UP) ||
		UEngineInput::GetInst().IsDown(VK_DOWN)))
	{
		Attack(_DeltaTime);
	}

	// false�ϱ� ����. true�� ��ȯ.
	if (true == TearFire)
	{
		// ���������� ��Ÿ�� ��� ����
		CoolDownElapsed += _DeltaTime;

		//  ��Ÿ���� ����Ǹ�, 
		if (CoolDownElapsed > Cooldown)
		{
			//TearFire�� false�� �ǵ��� ���� ���� ���·� �ٲٰ� ��Ÿ�� �ʱ�ȭ
			TearFire = false;
			CoolDownElapsed = 0.0f;
		}
	}
}

void APlayer::Attack(float _DeltaTime)
{
	TearFire = true;

	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() - HeadRenderer->GetComponentScale().Half().iY() + 10 };
	if (UEngineInput::GetInst().IsDown(VK_RIGHT))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		Tear->Fire(TearPos, FVector2D::RIGHT);
		HeadState = UpperState::ATTACK_RIGHT;
	}
	if (UEngineInput::GetInst().IsDown(VK_LEFT))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		Tear->Fire(TearPos, FVector2D::LEFT);
		HeadState = UpperState::ATTACK_LEFT;
	}
	if (UEngineInput::GetInst().IsDown(VK_UP))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		Tear->Fire(TearPos, FVector2D::UP);
		HeadState = UpperState::ATTACK_UP;
	}
	if (UEngineInput::GetInst().IsDown(VK_DOWN))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		Tear->Fire(TearPos, FVector2D::DOWN);
		HeadState = UpperState::ATTACK_DOWN;
	}
	SetAttackDir(HeadState);
}

void APlayer::SetAttackDir(UpperState _HeadState)
{
	// ���� ���� �ƴ϶�� Head�� State�� ������ ������ ����.
	if (false == TearFire)
	{
		return;
	}

	CurAttackHeadDir = static_cast<int>(_HeadState);
}

void APlayer::CurStateAnimation(float _DeltaTime)
{
	switch (BodyState)
	{
	case APlayer::LowerState::IDLE:
		BodyRenderer->ChangeAnimation("Body_Idle");
		break;
	case APlayer::LowerState::LEFT:
		BodyRenderer->ChangeAnimation("Body_Left");
		break;
	case APlayer::LowerState::RIGHT:
		BodyRenderer->ChangeAnimation("Body_Right");
		break;
	case APlayer::LowerState::UP:
		BodyRenderer->ChangeAnimation("Body_Up");
		break;
	case APlayer::LowerState::DOWN:
		BodyRenderer->ChangeAnimation("Body_Down");
		break;
	default:
		break;
	}

	switch (HeadState)
	{
	case APlayer::UpperState::IDLE:
		HeadRenderer->ChangeAnimation("Head_Down");
		break;
	case APlayer::UpperState::LEFT:
		HeadRenderer->ChangeAnimation("Head_Left");
		break;
	case APlayer::UpperState::RIGHT:
		HeadRenderer->ChangeAnimation("Head_RIght");
		break;
	case APlayer::UpperState::UP:
		HeadRenderer->ChangeAnimation("Head_Up");
		break;
	case APlayer::UpperState::DOWN:
		HeadRenderer->ChangeAnimation("Head_Down");
		break;
	case APlayer::UpperState::ATTACK_LEFT:
		HeadRenderer->ChangeAnimation("Head_Attack_Left", true);
		break;
	case APlayer::UpperState::ATTACK_RIGHT:
		HeadRenderer->ChangeAnimation("Head_Attack_Right", true);
		break;
	case APlayer::UpperState::ATTACK_UP:
		HeadRenderer->ChangeAnimation("Head_Attack_Up", true);
		break;
	case APlayer::UpperState::ATTACK_DOWN:
		HeadRenderer->ChangeAnimation("Head_Attack_Down", true);
		break;
	default:
		break;
	}

	// HeadState�� ���� �� ����̶��
	if (static_cast<int>(UpperState::ATTACK_LEFT) <= static_cast<int>(HeadState))
	{
		StateElapesd += _DeltaTime;
		
		// 0.2�� �̻��� ����Ѵٸ� BodyState ����.
		if (StateElapesd >= StateTime)
		{
			StateElapesd = 0.0f;
			HeadState = static_cast<UpperState>(BodyState);

			int a = 0;
		}		
	}
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
	BodyRenderer->CreateAnimation("Body_Left" , "Body.png",  1,  9, 0.05f);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, 0.05f);
	BodyRenderer->CreateAnimation("Body_Down" , "Body.png", 20, 29, 0.05f);
	BodyRenderer->CreateAnimation("Body_Up"   , "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, 0.05f);
	BodyRenderer->CreateAnimation("Body_Idle" , "Body.png", 29, 29, 0.05f);

	BodyRenderer->SetComponentScale({ 64, 64 });
	//BodyRenderer->SetComponentLocation(GetActorLocation());
	BodyRenderer->ChangeAnimation("Body_Idle");

	// Body
	/////////////////////////////////////////////////////////////////////////////// 
	// Head

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left"        , "Head.png", 1, 1, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right"       , "Head.png", 3, 3, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down"        , "Head.png", 7, 7, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up"          , "Head.png", 5, 5, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left" , "Head.png", 0, 1, 0.05f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Right", "Head.png", 2, 3, 0.05f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Down" , "Head.png", 6, 7, 0.05f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Up"   , "Head.png", 4, 5, 0.05f, false);

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

