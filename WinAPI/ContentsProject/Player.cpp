#include "PreCompile.h"
#include "Player.h"
#include <string>
#include <vector>

#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"
#include "PlayGameMode.h"
#include "Global.h"
#include "Room.h"
#include "Tear.h"

#include "DeathReportScene.h"
#include "HeartUI.h"
#include "PickupItemUI.h"
#include "PickupNumberUI.h"

int APlayer::Heart = 6;
int APlayer::HeartMax = 8;

APlayer::APlayer()
{
	SetName("Isaac");

	SetActorLocation(Global::WindowSize.Half()); // 1. Actor의 위치는 의미가 있어도 크기는 의미가 없다.
	InitPos = GetActorLocation();

	SpriteSetting(); // 2. 상태에 따른 애니메이션 동작을 정의한다.
	CollisionSetting(); // 3. 캐릭터의 이동영역을 지정할 충돌체를 생성한다. 

	DebugOn(); // 디버그 대상에 포함
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	UISetting();
	CollisionFuctionSetting();
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// 디버그 기능 집합
	PlayerDebugSetting(_DeltaTime);

	Death(_DeltaTime);
	UITick(_DeltaTime);

	if (true == APlayGameMode::IsGamePaused()) // 메뉴를 열면 정지
	{
		return;
	}
	if (true == IsDeath()) // 플레이어가 죽으면 정지
	{
		return;
	}

	RestoreInitialRenderState(_DeltaTime);
	IsCameraMove(); // 워프
	ARoom* CurRoom = ARoom::GetCurRoom();
	if (true == CurRoom->IsCameraMove()) 
	{
		return; // 워프 후 몬스터가 스폰되는 연출시간 동안 움직이지 않도록 
	}

	// 로직
	Move(_DeltaTime);
	InputAttack(_DeltaTime);

	// 렌더
	CurStateAnimation(_DeltaTime);

}

void APlayer::CollisionFuctionSetting()
{
	BodyCollision->SetCollisionEnter(std::bind(&APlayer::ShowHitAnimation, this, std::placeholders::_1));
	WarpCollision->SetCollisionStay(std::bind(&APlayer::ClampPositionToRoom, this));
}

// 맵 바깥으로 나가지 못하게 막는 함수
void APlayer::ClampPositionToRoom()
{
	FVector2D Pos = GetActorLocation();
	FVector2D FootPos = Pos + WarpCollision->GetComponentLocation();

	ARoom* CurRoom = ARoom::GetCurRoom();
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	if (LeftEdge > FootPos.X)
	{
		SetActorLocation(Pos + FVector2D{2, 0});
	}
	if (RightEdge < FootPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -2, 0 });
	}
	if (TopEdge > FootPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 2 });
	}
	if (BotEdge < FootPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -2 });
	}
}

void APlayer::ShowHitAnimation(AActor* _Other)
{
	if (true == FullRenderer->IsActive())
	{
		return;
	}
	if (true == IsDead)
	{
		return;
	}

	FullRenderer->ChangeAnimation("Damaged");
	FullRenderer->SetActive(true);
	BodyRenderer->SetActive(false);
	HeadRenderer->SetActive(false);
}

void APlayer::UITick(float _DeltaTime)
{
	  PlayerHpToHeart->SetPlayerHp(Heart);
	PennyPickupNumber->SetValue(PennyCount);
 	 BombPickupNumber->SetValue(BombCount);
	  KeyPickupNumber->SetValue(KeyCount);
}

// 플레이어가 공격받는 애니메이션이 출력되는 함수
void APlayer::RestoreInitialRenderState(float _DeltaTime)
{	// 피격 중이라면 리턴
	if (false == FullRenderer->IsActive())
	{
		return;
	}

	StateElapsed += _DeltaTime;
	float ActionDuration = 0.5f;
	if (StateElapsed > ActionDuration)
	{
		FullRenderer->SetActive(false);
		BodyRenderer->SetActive(true);
		HeadRenderer->SetActive(true);
		StateElapsed = 0.0f;
		return;
	}
}

void APlayer::CollisionSetting()
{
	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, -15 });
	BodyCollision->SetComponentScale({ 30, 50 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Player_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	WarpCollision = CreateDefaultSubObject<U2DCollision>();
	WarpCollision->SetComponentLocation({ 0, 5 });
	WarpCollision->SetComponentScale({ 30, 20 });
	WarpCollision->SetCollisionGroup(ECollisionGroup::Player_Warp);
	WarpCollision->SetCollisionType(ECollisionType::CirCle);

	SetActorScale(WarpCollision->GetComponentScale());
}

bool APlayer::IsDeath()
{
	if (Heart <= 0)
	{
		Heart = 0;
		IsDead = true;
		return true;
	}

	return false;
}

void APlayer::Death(float _DeltaTime)
{
	if (false == IsDeath())
	{
		return;
	}

	APlayGameMode::SetGamePaused(true); // 조작 멈춰, 몬스터들도 멈춰

	WarpCollision->SetActive(false);
	BodyCollision->SetActive(false);

	DeathAnimation();
}

void APlayer::DeathAnimation()
{
	if (false == IsResetReady)
	{
		FullRenderer->SetComponentScale({ 120, 120 });
		FullRenderer->ChangeAnimation("Death");
		
		FullRenderer->SetActive(true);
		BodyRenderer->SetActive(false);
		HeadRenderer->SetActive(false);
		BodyRenderer->SetComponentLocation({ -5, -BodyRenderer->GetComponentScale().Half().iY() -10});
		HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() - 40});
		FadeOut();

		FVector2D Pos = GetActorLocation();
		DeathPos = GetActorLocation() ;

		SetActorLocation(DeathPos);
	}

	IsResetReady = true; 

	TimeEventer.PushEvent(1.0f, std::bind(&APlayer::SpiritAnimation, this));
}

void APlayer::SpiritAnimation()
{
	FullRenderer->SetActive(true);
	BodyRenderer->SetActive(true);
	HeadRenderer->SetActive(true);

	BodyRenderer->ChangeAnimation("Body_Death");
	HeadRenderer->ChangeAnimation("Head_Death");

	// 데스리포트 호출
	TimeEventer.PushEvent(2.0f, std::bind(&APlayer::ShowDeathReport, this));

	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();

	float SpiritSpeed = 50.0f;
	Dir = FVector2D::UP;
	BodyRenderer->AddComponentLocation(Dir * SpiritSpeed * DeltaTime);
	HeadRenderer->AddComponentLocation(Dir * SpiritSpeed * DeltaTime);
}

void APlayer::ShowDeathReport()
{
	if (false == IsDeath())
	{
		return;
	}
	
	ADeathReportScene::DeathReport->ShowDeathReport();
	
	if (UEngineInput::GetInst().IsDown(VK_SPACE))
	{

		ADeathReportScene::DeathReport->CloseDeathReport();

		// Static이면 리셋 설정 해줘야 한다.
		Reset();

		UEngineAPICore::GetCore()->ResetLevel<APlayGameMode, APlayer>("Play");
		UEngineAPICore::GetCore()->OpenLevel("Title");
	}

}

void APlayer::Reset()
{
	Heart = 6;
	HeartMax = 8;

	BodyCollision->SetActive(true);
	WarpCollision->SetActive(true);

	HeadState = UpperState::IDLE;
	BodyState = LowerState::IDLE;

	APlayGameMode::SetGamePaused(false);
	IsDead = false;
	IsResetReady = false;
	Dir = FVector2D::ZERO;

	//BodyRenderer->SetAlphaFloat(1.0f);
	//HeadRenderer->SetAlphaFloat(1.0f);

	//SetActorLocation(InitPos);
	//BodyRenderer->SetComponentLocation(GetActorLocation() - Global::WindowHalfScale);
	//HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 4 });

	//SpiritMoveElapsed = 0.0f;
}

void APlayer::Move(float _DeltaTime)
{
	if (true == CameraMove) // 방 이동을 중에 캐릭터는 움직일 수 없다.
	{
		return;
	}

	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Dir += FVector2D::LEFT;
		BodyState = LowerState::LEFT;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Dir += FVector2D::RIGHT;
		BodyState = LowerState::RIGHT;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Dir += FVector2D::UP;
		BodyState = LowerState::UP;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Dir += FVector2D::DOWN;
		BodyState = LowerState::DOWN;
		IsMove = true;
	}

	if (true == IsMove)
	{
		Dir.Normalize();
		FVector2D TargetSpeed = Dir * SpeedMax * 1.0f;

		//FinalSpeed += Dir * MoveAcc * _DeltaTime; 	// 가속도
		FinalSpeed = FVector2D::Lerp(FinalSpeed, TargetSpeed, MoveAcc * _DeltaTime);
		FVector2D Result = FinalSpeed;
	}
	else
	{
		//FinalSpeed *= 700.0f * _DeltaTime;
		FinalSpeed = FVector2D::ZERO;
	}

	// 최대속도 제한 : 항상 절댓값으로
	if (abs(FinalSpeed.X) > SpeedMax || abs(FinalSpeed.Y) > SpeedMax)
	{
		FinalSpeed.Normalize();
		FinalSpeed *= SpeedMax;
	}

	AddActorLocation(FinalSpeed * _DeltaTime);


	// HeadState 설정 : 공격 중인지, 아닌지
	if (true == IsAttack())
	{
		int BodyDir = GetAttackDir(); // 공격방향을 Attack 함수에서 int로 받아와서 전달
		HeadState = static_cast<UpperState>(BodyDir);
	}
	else
	{
		int BodyDir = static_cast<int>(BodyState); // 이동방향 그대로 얼굴이 전환
		HeadState = static_cast<UpperState>(BodyDir);
	}

	// 키 입력 상태가 아니면
	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		IsMove = false;

		TimeElapsed += _DeltaTime;
		if (TimeElapsed > StateTime)
		{
			BodyState = LowerState::IDLE;
			TimeElapsed = 0.0f;
		}

		// 공격 상태가 아니고, 키입력 없으면 IDLE로 전환
		if (false == IsAttack())
		{
			HeadState = UpperState::IDLE;
		}
	}
}

void APlayer::CameraPosMove(float _DeltaTime)
{
	FVector2D RoomScale = Global::WindowSize;
	FVector2D PlayerMovePos = GetActorLocation();
	StartCameraPos = GetWorld()->GetCameraPos();

	if (UEngineInput::GetInst().IsDown('H'))
	{
		CameraMove = true;
		CameraMoveDir = FVector2D::LEFT;
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('K'))
	{
		CameraMove = true;
		CameraMoveDir = FVector2D::RIGHT;
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('U'))
	{
		CameraMove = true;
		CameraMoveDir = FVector2D::UP;
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
	}
	if (UEngineInput::GetInst().IsDown('J'))
	{
		CameraMove = true;
		CameraMoveDir = FVector2D::DOWN;
		EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
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
			CameraMove = false;
			CameraMoveTime = 0.0f;
		}
	}
}

void APlayer::IsCameraMove()
{
	ARoom* CurRoom = ARoom::GetCurRoom();
	if (true == CurRoom->IsCameraMove())
	{
		HeadState = UpperState::IDLE;
		BodyState = LowerState::IDLE;
	}
}

void APlayer::InputAttack(float _DeltaTime)
{
	// 공격 입력이 처음 들어왔을 때 동작
	if (false == TearFire &&
		(UEngineInput::GetInst().IsDown(VK_LEFT) ||
		UEngineInput::GetInst().IsDown(VK_RIGHT) ||
		UEngineInput::GetInst().IsDown(VK_UP)    ||
		UEngineInput::GetInst().IsDown(VK_DOWN)))
	{
		Attack(_DeltaTime);
	}

	if (true == TearFire)				// false니까 공격. true로 변환.
	{
		CoolDownElapsed += _DeltaTime; 	// 공격했으면 쿨타임 계산 시작
		if (CoolDownElapsed > Cooldown) //  쿨타임이 경과되면, 
		{
			//TearFire를 false로 되돌려 공격 가능 상태로 바꾸고 쿨타임 초기화
			TearFire = false;
			CoolDownElapsed = 0.0f;

			if (UEngineInput::GetInst().IsPress(VK_LEFT) ||
				UEngineInput::GetInst().IsPress(VK_RIGHT) ||
				UEngineInput::GetInst().IsPress(VK_UP) ||
				UEngineInput::GetInst().IsPress(VK_DOWN))
			{
				Attack(_DeltaTime);
			}
		}
	}
}

void APlayer::Attack(float _DeltaTime)
{
	TearFire = true; // true일 때, Cooldown시간 동안 Attack 함수가 호출될 수 없다.

	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() - HeadRenderer->GetComponentScale().Half().iY() + 10 };

	// 눈물이 좌/우로 번갈아 발사되는 디테일을 위해 Pos를 한번 더 조정한다.
	if (UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		if (true == LeftFire)
		{
			TearPos = TearPos + FVector2D{ -15, -3 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			TearPos = TearPos + FVector2D{ -15, 3 };
			LeftFire = true;
		}
		TearDir = FVector2D::LEFT;
		HeadState = UpperState::ATTACK_LEFT;
	}
	if (UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		if (true == LeftFire)
		{
			TearPos = TearPos + FVector2D{ +15, -3 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			TearPos = TearPos + FVector2D{ +15, 3 };
			LeftFire = true;
		}
		TearDir = FVector2D::RIGHT;
		HeadState = UpperState::ATTACK_RIGHT;
	}
	if (UEngineInput::GetInst().IsPress(VK_UP))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		if (true == LeftFire)
		{
			TearPos = TearPos + FVector2D{ -7, -15 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			TearPos = TearPos + FVector2D{ +7, -15 };
			LeftFire = true;
		}
		TearDir = FVector2D::UP;
		HeadState = UpperState::ATTACK_UP;
	}
	if (UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		Tear = GetWorld()->SpawnActor<ATear>();
		if (true == LeftFire)
		{
			TearPos = TearPos + FVector2D{ -7, 0 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			TearPos = TearPos + FVector2D{ +7, 0 };
			LeftFire = true;
		}
		TearDir = FVector2D::DOWN;
		HeadState = UpperState::ATTACK_DOWN;
	}

	Tear->Fire(TearPos, TearDir, SpeedMax, Att);

	SetAttackDir(HeadState);
}

void APlayer::SetAttackDir(UpperState _HeadState)
{
	// 공격 중이 아니라면 Head의 State를 수정할 이유가 없다.
	if (false == TearFire)
	{
		return;
	}

	CurAttackHeadDir = static_cast<int>(_HeadState);
}

void APlayer::SpriteSetting()
{
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Body_Left", "Body.png", 1, 9, 0.05f);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, 0.05f);
	BodyRenderer->CreateAnimation("Body_Down", "Body.png", 20, 29, 0.05f);
	BodyRenderer->CreateAnimation("Body_Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, 0.05f);
	BodyRenderer->CreateAnimation("Body_Idle", "Body.png", 29, 29, 0.05f);
	BodyRenderer->CreateAnimation("Body_Death", "Death_Body.png", 0, 4, 0.2f);

	BodyRenderer->SetComponentScale({ 64, 64 });
	//BodyRenderer->ChangeAnimation("Body_Idle");
	BodyRenderer->ChangeAnimation("Body_Death");

	// Body
	/////////////////////////////////////////////////////////////////////////////// 
	// Head

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", "Head.png", 1, 1, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right", "Head.png", 3, 3, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down", "Head.png", 7, 7, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up", "Head.png", 5, 5, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left", "Head.png", { 1, 0, 1 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Right", "Head.png", { 3, 2, 3 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Down", "Head.png", { 7, 6, 7 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Up", "Head.png", { 5, 4, 5 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Death", "Death_Head.png", 0, 0, 0.5f);

	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 4 });
	HeadRenderer->SetComponentScale({ 64, 64 });
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::Player);
	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);

	////////////////////////////////////////////////////////////////////////////////
	// Event
	FullRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FullRenderer->CreateAnimation("Death", "Isaac.png", { 0, 6, 3 }, 0.1f, false);
	FullRenderer->CreateAnimation("Damaged", "Isaac.png", 6, 6, 0.1f);
	FullRenderer->SetComponentScale({ 128, 128 });
	FullRenderer->SetOrder(ERenderOrder::Player);
	FullRenderer->SetPivot({ 0, -20 });
	FullRenderer->SetActive(false);

}

void APlayer::UISetting()
{
	// Heart
	PlayerHpToHeart = GetWorld()->SpawnActor<AHeartUI>();
	PlayerHpToHeart->SetTextSpriteName("hearts.png");
	PlayerHpToHeart->SetOrder(ERenderOrder::UI);
	PlayerHpToHeart->SetTextScale({ 32, 32 });
	PlayerHpToHeart->SetActorLocation({ 135, 45 });

	// Penny
	PennyUI = GetWorld()->SpawnActor<APickupItemUI>();
	PennyUI->SetTextSpriteName("ui_crafting.png");
	PennyUI->SetOrder(ERenderOrder::UI);
	PennyUI->SetTextScale({ 34, 38 });
	PennyUI->SetActorLocation({ 57, 95 });
	PennyUI->SetValue(8); // SetSpriteIndex

	PennyPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	PennyPickupNumber->SetTextSpriteName("pickup.png");
	PennyPickupNumber->SetOrder(ERenderOrder::UI);
	PennyPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	PennyPickupNumber->SetActorLocation({ 77, 95 });

	// Bomb
	FVector2D Offset = FVector2D(0, +25);
	BombUI = GetWorld()->SpawnActor<APickupItemUI>();
	BombUI->SetTextSpriteName("ui_crafting.png");
	BombUI->SetOrder(ERenderOrder::UI);
	BombUI->SetTextScale({ 32, 32 });
	BombUI->SetActorLocation(FVector2D(55, 95) + Offset);
	BombUI->SetValue(15); //SetSpriteIndex

	BombPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	BombPickupNumber->SetTextSpriteName("pickup.png");
	BombPickupNumber->SetOrder(ERenderOrder::UI);
	BombPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	BombPickupNumber->SetActorLocation(PennyPickupNumber->GetActorLocation() + Offset);
	BombPickupNumber->SetValue(BombCount);

	// Key
	KeyUI = GetWorld()->SpawnActor<APickupItemUI>();
	KeyUI->SetTextSpriteName("ui_crafting.png");
	KeyUI->SetOrder(ERenderOrder::UI);
	KeyUI->SetTextScale({ 32, 32 });
	KeyUI->SetActorLocation(BombUI->GetActorLocation() + Offset);
	KeyUI->SetValue(12); //SetSpriteIndex

	KeyPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	KeyPickupNumber->SetTextSpriteName("pickup.png");
	KeyPickupNumber->SetOrder(ERenderOrder::UI);
	KeyPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	KeyPickupNumber->SetActorLocation(BombPickupNumber->GetActorLocation() + Offset);
}

void APlayer::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.3f * FadeDir;
	BodyRenderer->SetAlphaFloat(FadeValue);
	HeadRenderer->SetAlphaFloat(FadeValue);
}

void APlayer::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(3.0f, std::bind_front(&APlayer::FadeChange, this), true, false);
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
	case APlayer::LowerState::DEATH:
		BodyRenderer->ChangeAnimation("Body_Death");
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
	case APlayer::UpperState::DEATH:
		HeadRenderer->ChangeAnimation("Head_Death");
		break;
	default:
		break;
	}

	// HeadState가 공격 중 모션이라면
	if (static_cast<int>(UpperState::ATTACK_LEFT) <= static_cast<int>(HeadState))
	{
		StateElapesd += _DeltaTime;

		// 0.2초 이상이 경과한다면 BodyState 따라가.
		if (StateElapesd >= StateTime)
		{
			StateElapesd = 0.0f;
			HeadState = static_cast<UpperState>(BodyState);
		}
	}
}

void APlayer::PlayerDebugSetting(float _DeltaTime)
{
	CameraPosMove(_DeltaTime);
	UIDebug(_DeltaTime);
	ResetDebug();

	UEngineDebug::CoreOutPutString("FinalSpeed : " + FinalSpeed.ToString());
	//UEngineDebug::CoreOutPutString("Body : " + BodyRenderer->GetComponentLocation().ToString());
	//UEngineDebug::CoreOutPutString("Head : " + HeadRenderer->GetComponentLocation().ToString());
}

void APlayer::ResetDebug()
{
	if (UEngineInput::GetInst().IsDown(VK_F3))
	{
		Reset();
		UEngineAPICore::GetCore()->ResetLevel<APlayGameMode, APlayer>("Play");
	}
}

void APlayer::UIDebug(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (UEngineInput::GetInst().IsDown('I'))
	{
		Heart = 0;
	}
	if (UEngineInput::GetInst().IsDown('M'))
	{
		Heart = HeartMax;
	}
	if (UEngineInput::GetInst().IsDown('O'))
	{
		PennyCount += 1;
	}
	if (UEngineInput::GetInst().IsDown('P'))
	{
		BombCount += 1;
	}
	if (UEngineInput::GetInst().IsDown('L'))
	{
		KeyCount += 1;
	}
}