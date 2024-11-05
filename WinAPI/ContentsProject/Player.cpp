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

void APlayer::RunSoundPlay()
{
	UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	// 1. Actor의 위치는 의미가 있어도 크기는 의미가 없다.
	SetActorLocation(Global::WindowSize.Half());

	// 2. 상태에 따른 애니메이션 동작을 정의한다.
	SpriteSetting();

	// 3. 캐릭터의 이동영역을 지정할 충돌체를 생성한다.
 
	// 4. 캐릭터의 히트박스를 설정할 충돌체를 생성한다.
	// CreateDefaultSubObject<U2DCollision>();
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	// 직접 카메라 피봇을 설정해줘야 한다.
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
	Super::Tick(_DeltaTime);

	Move(_DeltaTime);
	InputAttack(_DeltaTime);

	CurStateAnimation(_DeltaTime);

	CameraPosMove(_DeltaTime);
}

void APlayer::Move(float _DeltaTime)
{
	// 방 이동을 중에 캐릭터는 움직일 수 없다.
	if (true == CameraMove)
	{
		return;
	}

	// 자연스럽게 이동하게 보이는 법 : 이동(로직)과 렌더를 분리할 것

	// 입력 방법 1 : 단순 키 입력에 로직을 추가하여 처리
	
	// 이동
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		BodyState = LowerState::LEFT;
		HeadState = UpperState::LEFT;
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);

	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		BodyState = LowerState::RIGHT;
		HeadState = UpperState::RIGHT;
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		BodyState = LowerState::UP;
		HeadState = UpperState::UP;
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);

	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		BodyState = LowerState::DOWN;
		HeadState = UpperState::DOWN;
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		BodyState = LowerState::IDLE;
		// HeadState = UpperState::IDLE; // 이걸 고치면 바로 얼굴이 원래대로 돌아옴
		
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

			// 플레이어를 어떻게 다음 방의 문 앞에 배치시킬지 생각해보기
			//SetActorLocation(다음 방 문 앞);
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

	// false니까 공격. true로 변환.
	if (true == TearFire)
	{
		CoolDownElapsed += _DeltaTime;

		//    0.0   > 0.5 = false
		if (CoolDownElapsed > Cooldown)
		{
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
		HeadRenderer->ChangeAnimation("Head_Attack_Left");
		break;
	case APlayer::UpperState::ATTACK_RIGHT:
		HeadRenderer->ChangeAnimation("Head_Attack_Right");
		break;
	case APlayer::UpperState::ATTACK_UP:
		HeadRenderer->ChangeAnimation("Head_Attack_Up");
		break;
	case APlayer::UpperState::ATTACK_DOWN:
		HeadRenderer->ChangeAnimation("Head_Attack_Down");
		break;
	default:
		break;
	}

	// HeadState가 공격 중 모션이라면
	if (static_cast<int>(UpperState::ATTACK_LEFT) <= static_cast<int>(HeadState))
	{
		StateElapesd += _DeltaTime;
		
		// 0.5초 이상이 경과한다면 BodyState 따라가.
		if (StateElapesd >= StateTime)
		{
			StateElapesd = 0.0f;
			HeadState = static_cast<UpperState>(BodyState);

			
		}		
	}
}

void APlayer::SpriteSetting()
{
	// 1. 헤더에 랜더러를 하나 만든다.
	// 2. CreateDefualtSubObject 함수를 사용하여 렌더러 컴포넌트를 만든다.
	// 3. CreateAnimation 함수를 사용하여 ("동작 이름", "이미지.png", 동작순서, 프레임)을 설정한다.
	// 4. SetComponentScale 함수를 사용하여 렌더러 컴포넌트의 크기를 정의한다.
	// 5. ChanageAnimation 함수를 사용하여 기본 동작을 정의한다.
	// 6. SetOrder로 정렬 순서를 정할 수 있다.

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
	HeadRenderer->CreateAnimation("Head_Attack_Left" , "Head.png", 0, 1, 0.2f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Right", "Head.png", 2, 3, 0.2f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Down" , "Head.png", 6, 7, 0.2f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Up"   , "Head.png", 4, 5, 0.2f, false);

	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 4 });
	HeadRenderer->SetComponentScale({ 64, 64 }); 
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::PLAYER);
	HeadRenderer->SetOrder(ERenderOrder::PLAYER);
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

