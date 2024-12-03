#include "PreCompile.h"
#include "Player.h"
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
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

#include "Item.h"
#include "Heart.h"
#include "Bomb.h"
#include "Key.h"
#include "InnerEye.h"

int APlayer::Heart = 6;
int APlayer::HeartMax = 8;	
int APlayer::TotalHeartMax = 24;

APlayer::APlayer()
{
	SetName("Isaac");

	SetActorLocation({ Global::WindowSize.Half().iX(), Global::WindowSize.Half().iY() + 100});

	InitPos = GetActorLocation();
	Global::PlayerHeadOffset = { 0, -32 };

	SpriteSetting(); 
	CollisionSetting(); 

	DebugOn();
	//SwitchInvincibility(); // 디버깅용 무적
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	UISetting();
	CollisionFuctionSetting();

	PennyPickupNumber->SetValue(0);
	BombPickupNumber->SetValue(0);
	KeyPickupNumber->SetValue(0);
	
	BombCooldown = 2.0f;
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// 디버그 기능 집합
	PlayerDebugSetting(_DeltaTime);

	Death(_DeltaTime);
	PlayerLimit();
	UITick(_DeltaTime);

	if (true == APlayGameMode::IsGamePaused()) // 메뉴를 열면 정지
	{
		return;
	}
	if (true == IsDeath()) // 플레이어가 죽으면 정지
	{
		return;
	}

	ReverseForce(_DeltaTime);

	IsCameraMove(); // 워프

	CurStateAnimation(_DeltaTime);

	ARoom* CurRoom = ARoom::GetCurRoom();
	if (true == CurRoom->IsCameraMove()) 
	{
		return; // 워프 후 몬스터가 스폰되는 연출시간 동안 움직이지 않도록 
	}

	Move(_DeltaTime);
	InputAttack(_DeltaTime);
	InputItem();
	UpdateItemPos();
	ChangeDetectCollisionDirection();

	BombCooldown += _DeltaTime;
}

void APlayer::CollisionFuctionSetting()
{
	BodyCollision->SetCollisionEnter(std::bind(&APlayer::ShowHitAnimation, this, std::placeholders::_1));
	WarpCollision->SetCollisionStay(std::bind(&APlayer::ClampPositionToRoom, this));
	DetectCollision->SetCollisionStay(std::bind(&APlayer::ChaseDirection, this, std::placeholders::_1));
}

// 맵 바깥으로 나가지 못하게 막는 함수
void APlayer::ClampPositionToRoom()
{
	FVector2D Pos = GetActorLocation();
	FVector2D HalfScale = WarpCollision->GetComponentScale();
	FVector2D FootPos = Pos + WarpCollision->GetComponentLocation();

	float FootLeft  = FootPos.X - HalfScale.X;
	float FootRight = FootPos.X + HalfScale.X;
	float FootUp    = FootPos.Y - HalfScale.Y;
	float FootDown  = FootPos.Y + HalfScale.Y;

	ARoom* CurRoom = ARoom::GetCurRoom();
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2.0f;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 1.9f;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	FVector2D ClampedPos = FVector2D(FVector2D::Clamp(FootPos.X, LeftEdge, RightEdge), FVector2D::Clamp(FootPos.Y, TopEdge, BotEdge));

	if (LeftEdge > FootPos.X)
	{
		SetActorLocation(ClampedPos - (FootPos - Pos));
	}
	if (RightEdge < FootPos.X)
	{
		SetActorLocation(ClampedPos - (FootPos - Pos));
	}
	if (TopEdge > FootPos.Y)
	{
		SetActorLocation(ClampedPos - (FootPos - Pos));
	}
	if (BotEdge < FootPos.Y)
	{
		SetActorLocation(ClampedPos - (FootPos - Pos));
	}
}

// 데미지 처리, 무적 및 애니메이션 출력 함수
int APlayer::ApplyDamaged(AActor* _Player, const int& _Att, const FVector2D& _Dir)
{
	if (true == IsHit)
	{
		return 0;
	}
	APlayer* Player = dynamic_cast<APlayer*>(_Player);
	if (nullptr == Player)
	{
		return 0;
	}
	else if (true == Player->IsInvincible()) // 무적이면 리턴
	{
		return 0;
	}

	if (true == Player->IsDeath())
	{
		return 0;
	}	
	
	Sound = UEngineSound::Play("hurt_grunt_2.wav");

	Player->ShowHitAnimation(_Player);
	Player->BeginBlinkEffect();

	IsHit = true;
	Invincibility = true;

	KnockbackStartPos = GetActorLocation();
	UpPos = KnockbackStartPos + FVector2D(0.0f, -50.0f);
	FVector2D Offset = _Dir;
	Offset.Normalize();
	KnockbackDistance = Offset * 20.0f; // 넉백거리 Lerp 적용

	TimeEventer.PushEvent(1.0f, std::bind(&APlayer::SwitchIsHit, this)); // 데미지를 다시 입기까지 걸리는 시간

	Heart -= _Att;
	if (Heart < 0)
	{
		Heart = 0;
	}
	return Heart;
}

void APlayer::ReverseForce(const float& _DeltaTime)
{
	FVector2D Reverse = -Force;
	Reverse.Normalize();

	Force += Reverse * _DeltaTime * 100.0f;

	if (10.0f > Force.Length())
	{
		Force = FVector2D::ZERO;
	}

	AddActorLocation(Force * _DeltaTime);
}

void APlayer::ShowHitAnimation(AActor* _Other)
{
	if (true == Invincibility) // 무적상태면 리턴
	{
		return;
	}
	if (true == IsDead) // 죽었으면 리턴
	{
		return;
	}

	FullRenderer->ChangeAnimation("Damaged");
	FullRenderer->SetActive(true);
	BodyRenderer->SetActive(false);
	HeadRenderer->SetActive(false);

	Invincibility = true; // 무적

	// 원래 무적 해제 및 애니메이션으로 복귀
	TimeEventer.PushEvent(InvincibilityDuration, std::bind(&APlayer::RestoreDefaultMotion, this));
}

void APlayer::RestoreDefaultMotion()
{
	if (true == IsDead) // 죽었으면 리턴
	{
		return;
	}

	Invincibility = false; // 무적해제
	FullRenderer->SetActive(false);
	BodyRenderer->SetActive(true);
	HeadRenderer->SetActive(true);
}

void APlayer::BeginBlinkEffect()
{
	if (true == IsDead) // 죽었으면 리턴
	{
		return;
	}

	FullRenderer->SetAlphaFloat(0.0f);
	BodyRenderer->SetAlphaFloat(0.0f);
	HeadRenderer->SetAlphaFloat(0.0f);
	TimeEventer.PushEvent(0.1f, std::bind(&APlayer::StayBlinkEffect, this)); 
}

void APlayer::StayBlinkEffect()
{
	if (true == IsDead) // 죽었으면 리턴
	{
		FullRenderer->SetAlphaFloat(1.0f);
		BodyRenderer->SetAlphaFloat(0.0f);
		HeadRenderer->SetAlphaFloat(0.0f);
		return;
	}

	FullRenderer->SetAlphaFloat(1.0f);
	BodyRenderer->SetAlphaFloat(1.0f);
	HeadRenderer->SetAlphaFloat(1.0f);

	++FadeCount;
	int BlinkCount = 4;
	if (BlinkCount < FadeCount)
	{
		FadeCount = 0;
		return;
	}
	TimeEventer.PushEvent(0.1f, std::bind(&APlayer::BeginBlinkEffect, this));
}

void APlayer::UITick(const float& _DeltaTime)
{
	static int LastPennyCount = -1;
	static int LastBombCount = -1;
	static int LastKeyCount = -1;

	  PlayerHpToHeart->SetPlayerHp(Heart);
	  int CurrentPennyCount = CheckPickupItemCount("Penny");
	  int CurrentBombCount = CheckPickupItemCount("Bomb");
	  int CurrentKeyCount = CheckPickupItemCount("Key");

	  if (LastPennyCount != CurrentPennyCount)
	  {
		  PennyPickupNumber->SetValue(CurrentPennyCount);
		  LastPennyCount = CurrentPennyCount;
	  }

	  if (LastBombCount != CurrentBombCount)
	  {
		  BombPickupNumber->SetValue(CurrentBombCount);
		  LastBombCount = CurrentBombCount;
	  }

	  if (LastKeyCount != CurrentKeyCount)
	  {
		  KeyPickupNumber->SetValue(CurrentKeyCount);
		  LastKeyCount = CurrentKeyCount;
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
	WarpCollision->SetComponentScale({ 30, 15 });
	WarpCollision->SetCollisionGroup(ECollisionGroup::Player_Warp);
	WarpCollision->SetCollisionType(ECollisionType::Rect);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale({0, 0});
	DetectCollision->SetCollisionGroup(ECollisionGroup::Player_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);

	SetActorScale(WarpCollision->GetComponentScale());
}

bool APlayer::IsDeath()
{
	if (Heart <= 0)
	{
		Heart = 0;
		IsDead = true;
		IsHit = false;
		return true;
	}

	return false;
}

void APlayer::Death(const float& _DeltaTime)
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
		UEngineSound::AllSoundStop();
		Sound = UEngineSound::Play("isaac died_02.ogg");
		Sound = UEngineSound::Play("isaac_dies_1.wav");

		FullRenderer->SetComponentScale({ 120, 120 });
		FullRenderer->ChangeAnimation("Death");
		BodyRenderer->SetActive(false);
		HeadRenderer->SetActive(false);
		FullRenderer->SetActive(true);
		BodyRenderer->SetComponentLocation({ -5, -BodyRenderer->GetComponentScale().Half().iY() -10});
		HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() - 40});
		SpiritFadeOut();

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

	float SpiritSpeed = 100.0f;
	Direction = FVector2D::UP;
	BodyRenderer->AddComponentLocation(Direction * SpiritSpeed * DeltaTime);
	HeadRenderer->AddComponentLocation(Direction * SpiritSpeed * DeltaTime);
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
		Sound = UEngineSound::Play("button_press.wav");
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
	Direction = FVector2D::ZERO;
}

bool APlayer::TryPickupItem(AItem* _Item, const int& _Count)
{
	if (false == _Item->EatFunction(this))
	{
		return false;
	}

	_Item->PickupSuccess();

	if (false == _Item->IsStored()) 
	{
		return true;
	}

	AddItem(_Item, _Count);

	return true;
}

void APlayer::AddItem(AItem* _Item, const int& _Count)
{
	EItemType ItemType = _Item->GetItemType();
	if (EItemType::PASSIVE == ItemType)
	{
		PassiveItem = nullptr; // 기존 자료를 지우고
		PassiveItem = _Item; // 새로운 아이템 정보를 넣는다.
	}

	std::string ItemName = _Item->GetName();
	for (int i = 0; i < _Count; i++)
	{
		Items.push_back(_Item);
		ItemCounts[ItemName]++;
	}
}

void APlayer::InputItem()
{
	AItem* Item = nullptr;
	if (UEngineInput::GetInst().IsDown('E'))
	{
		if (BombCooldown < BombDuration)
		{
			return;
		}

		Item = ReturnItem("Bomb");
		if (nullptr != Item)
		{
			Item->UseItem(this);

			RemveItem(Item);
		}

		BombCooldown = 0.0f;
	}
}

void APlayer::RemveItem(AItem* _Item)
{
	if (true == Items.empty())
	{
		return;
	}
	auto Iterator = std::find(Items.begin(), Items.end(), _Item);
	if (Iterator != Items.end())
	{
		Items.remove(_Item); // 리스트에서 아이템 제거
	}

	std::string ItemName = _Item->GetName();
	if (0 < ItemCounts[ItemName])
	{
		ItemCounts[ItemName]--;
	}
	else if (0 == ItemCounts[ItemName])
	{
		ItemCounts.erase(ItemName);
	}
}

void APlayer::UpdateItemPos()
{
	std::list<AItem*>::iterator StartIter = Items.begin();
	std::list<AItem*>::iterator EndIter = Items.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		AItem* Item = *StartIter;
		// 아이템의 소유권이 완전히 플레이어한테 귀속되면 위치도 플레이어한테 고정
		if (true == Item->IsOwned())
		{
			FVector2D PlayerPos = GetActorLocation();
			Item->SetActorLocation(PlayerPos);
		}
	}
}

void APlayer::ChaseDirection(AActor* _Monster)
{
	if (nullptr == DetectCollision)
	{
		return;
	}

	if (false == DetectCollision->IsActive())
	{
		return;
	}

	if (nullptr == _Monster)
	{
		ChaseTearDir = FVector2D::ZERO;
		return;
	}

	ChaseTearDir = FVector2D::ZERO;

	FVector2D NewDir =_Monster->GetActorLocation() - GetActorLocation();
	NewDir.Normalize();

	ChaseTearDir = NewDir;

}

void APlayer::ChangeDetectCollisionDirection()
{
	if (FVector2D::ZERO == DetectCollision->GetComponentScale())
	{
		return;
	}

	ChaseTearDir = FVector2D::ZERO;

	float OffsetX = DetectCollision->GetComponentScale().Half().X;
	float OffsetY = DetectCollision->GetComponentScale().Half().Y;
	if (HeadState == UpperState::ATTACK_LEFT)
	{
		DetectCollision->SetComponentLocation({ -OffsetX, 0.0f });
	}
	else if (HeadState == UpperState::ATTACK_RIGHT)
	{
		DetectCollision->SetComponentLocation({ OffsetX, 0.0f });
	}
	else if (HeadState == UpperState::ATTACK_UP)
	{
		DetectCollision->SetComponentLocation({ 0.0f, -OffsetY });
	}
	else if (HeadState == UpperState::ATTACK_DOWN)
	{
		DetectCollision->SetComponentLocation({ 0.0f, OffsetY });
	}


	FVector2D Pos = DetectCollision->GetComponentLocation();
	int a = 0;
}

void APlayer::RemovePassiveItem()
{
	std::list<AItem*>::iterator StartIter = Items.begin();
	std::list<AItem*>::iterator EndIter = Items.end();

	for (; StartIter != EndIter; )
	{
		AItem* Item = *StartIter;
		EItemType ItemType = Item->GetItemType();

		if (ItemType != EItemType::PASSIVE)
		{
			++StartIter;
			continue;
		}
		//Items.remove(Item);
		StartIter = Items.erase(StartIter);
		Item = nullptr;
		PassiveItem = nullptr;
		return;
	}
}

AItem* APlayer::ReturnItem(std::string_view _ItemName)
{
	std::string FindItemName = _ItemName.data();
	std::list<AItem*>::iterator StartIter = Items.begin();
	std::list<AItem*>::iterator EndIter = Items.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		AItem* Item = *StartIter;
		std::string ItemName = Item->GetName();
		if (FindItemName == ItemName)
		{
			return Item;
		}
	}
	return nullptr;
}

void APlayer::Move(const float& _DeltaTime)
{
	if (true == IsMovementStopped) // 방 이동을 중에 캐릭터는 움직일 수 없다.
	{
		return;
	}

	// 기존에 프레임에 따라 가속도의 결과값 영향이 너무 커 60프레임 기준으로 계산
	const float FixedTimeStep = 0.016f;
	float RemainingTime = _DeltaTime;
	while (RemainingTime > 0.0f)
	{
		float DeltaTime = UEngineMath::Min(RemainingTime, FixedTimeStep); // 현재 틱 계산
		RemainingTime -= DeltaTime;

		if (true == ProcessMovementInput())
		{
			if (FVector2D::ZERO != Direction)
			{
				Direction.Normalize();
			}

			TargetSpeed = Direction * MaxSpeed;

			const float FinalSpeedLength = FinalSpeed.Length(); 
			FVector2D CurDir = FVector2D::ZERO;
			if (0.0f < FinalSpeedLength)
			{
				CurDir = FinalSpeed / FinalSpeedLength;
			}
			else
			{
				CurDir = FVector2D::ZERO;
			}

			// 기본 출발속도를 주어 최저 속도를 보장
			if (30.0f > FinalSpeed.Length())
			{
				FinalSpeed += Direction * (MaxSpeed * 0.2f); 
			}

			// 방향전환
			if (50.0f > FinalSpeed.Length())
			{
				// 속도가 낮을 때 새로운 방향으로 이동할 경우 부드럽게 이동
				FinalSpeed = FVector2D::Lerp(FinalSpeed, TargetSpeed, MoveAcc * DeltaTime);
			}
			else if (FVector2D::DotProduct(CurDir, Direction) < 0.0f)
			{
				// 반대 방향이라면 기존속도의 영향을 빠르게 줄이고 최저 속도를 보장하여 답답하지 않게 함
				FinalSpeed = FVector2D::Lerp(FinalSpeed, FVector2D::ZERO, MoveAcc * DeltaTime);
				FinalSpeed += Direction * (MaxSpeed * 0.3f);
			}
			else // 같은 방향이라면 그대로
			{
				FinalSpeed = FinalSpeed + (TargetSpeed - FinalSpeed) * MoveAcc * DeltaTime;
			}
		}

		else // 키입력이 없는 경우 감속 처리
		{
			TargetSpeed = FVector2D::ZERO;

			static const float GlideSpeed = 120.0f;       // 미끄러질 속도
			static const float GlideDecayFactor = 0.98f; // 감속 비율
			static const float GlideDuration = 0.2f;     // 미끄러짐 지속 시간 (초)
			static float CurrentGlideTime = 0.0f;        // 현재 미끄러짐 시간
			static bool IsGliding = false;               // 미끄러짐 상태 플래그

			// 미끄러짐 초기화
			if (!IsGliding)
			{
				IsGliding = true;
				CurrentGlideTime = 0.0f;
			}

			// 미끄러짐 유지
			if (CurrentGlideTime < GlideDuration)
			{
				CurrentGlideTime += DeltaTime;
				FinalSpeed = FinalSpeed.GetNormal() * GlideSpeed; // 일정 속도 유지
			}
			else // 감속 처리
			{
				FinalSpeed *= GlideDecayFactor;

				if (FinalSpeed.Length() < 10.0f)
				{
					FinalSpeed = FVector2D::ZERO;
					IsGliding = false; // 감속 종료
				}
			}
		}

		// 최대속도 제한 : 항상 절댓값으로
		if (abs(FinalSpeed.X) > MaxSpeed || abs(FinalSpeed.Y) > MaxSpeed)
		{
			FinalSpeed.Normalize();
			FinalSpeed *= MaxSpeed;
		}

		AddActorLocation(FinalSpeed * DeltaTime); // 실제 이동값
	}

	UpdateHeadState();

	// 키 입력 상태가 아니면
	if (true == HasMovementInput())
	{
		return;
	}

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

bool APlayer::ProcessMovementInput()
{
	Direction = FVector2D::ZERO;
	IsMove = false;

	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		Direction += FVector2D::LEFT;
		BodyState = LowerState::LEFT;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		Direction += FVector2D::RIGHT;
		BodyState = LowerState::RIGHT;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		Direction += FVector2D::UP;
		BodyState = LowerState::UP;
		IsMove = true;
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		Direction += FVector2D::DOWN;
		BodyState = LowerState::DOWN;
		IsMove = true;
	}

	return IsMove;
}

void APlayer::UpdateHeadState()
{
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
}

bool APlayer::HasMovementInput()
{
	return UEngineInput::GetInst().IsPress('A') ||
		   UEngineInput::GetInst().IsPress('D') ||
		   UEngineInput::GetInst().IsPress('W') ||
		   UEngineInput::GetInst().IsPress('S');
}

void APlayer::CameraPosMove(const float& _DeltaTime)
{
	if (UEngineInput::GetInst().IsPress('H') ||
		UEngineInput::GetInst().IsPress('K') ||
		UEngineInput::GetInst().IsPress('U') ||
		UEngineInput::GetInst().IsPress('J'))
	{
		FVector2D RoomScale = Global::WindowSize;
		FVector2D PlayerMovePos = GetActorLocation();
		StartCameraPos = GetWorld()->GetCameraPos();

		if (UEngineInput::GetInst().IsDown('H'))
		{
			IsMovementStopped = true;
			CameraMoveDir = FVector2D::LEFT;
			EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
		}
		if (UEngineInput::GetInst().IsDown('K'))
		{
			IsMovementStopped = true;
			CameraMoveDir = FVector2D::RIGHT;
			EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
		}
		if (UEngineInput::GetInst().IsDown('U'))
		{
			IsMovementStopped = true;
			CameraMoveDir = FVector2D::UP;
			EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
		}
		if (UEngineInput::GetInst().IsDown('J'))
		{
			IsMovementStopped = true;
			CameraMoveDir = FVector2D::DOWN;
			EndCameraPos = GetWorld()->GetCameraPos() + RoomScale * CameraMoveDir;
		}

		if (true == IsMovementStopped)
		{
			LerpAlpha = CameraMoveTime / 1.0f;
			FVector2D CamPos = FVector2D::Lerp(StartCameraPos, EndCameraPos, LerpAlpha);

			GetWorld()->SetCameraPos(CamPos);
			SetActorLocation(PlayerMovePos);

			CameraMoveTime += _DeltaTime;
			if (1.0f <= CameraMoveTime)
			{
				IsMovementStopped = false;
				CameraMoveTime = 0.0f;
			}
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

void APlayer::InputAttack(const float& _DeltaTime)
{
	if (false == CanFire)
	{
		return;
	}
	// 공격 입력이 처음 들어왔을 때 동작
	if (false == TearFire &&
		(UEngineInput::GetInst().IsDown(VK_LEFT) ||
		UEngineInput::GetInst().IsDown(VK_RIGHT) ||
		UEngineInput::GetInst().IsDown(VK_UP)    ||
		UEngineInput::GetInst().IsDown(VK_DOWN)))
	{
		Attack(_DeltaTime);
		return;
	}

	if (true == TearFire)				// false니까 공격. true로 변환.
	{
		TearCoolDownElapsed += _DeltaTime; 	// 공격했으면 쿨타임 계산 시작
		if (TearCoolDownElapsed > TearCooldown) //  쿨타임이 경과되면, 
		{
			//TearFire를 false로 되돌려 공격 가능 상태로 바꾸고 쿨타임 초기화
			TearFire = false;
			TearCoolDownElapsed = 0.0f;

			if (UEngineInput::GetInst().IsPress(VK_LEFT) ||
				UEngineInput::GetInst().IsPress(VK_RIGHT) ||
				UEngineInput::GetInst().IsPress(VK_UP) ||
				UEngineInput::GetInst().IsPress(VK_DOWN))
			{
				Attack(_DeltaTime);
				TearCoolDownElapsed = 0.0f;
				return;
			}
		}
	}
}

void APlayer::Attack(const float& _DeltaTime)
{
	TearFire = true; // true일 때, Cooldown시간 동안 Attack 함수가 호출될 수 없다.

	Sound = UEngineSound::Play("tear_fire_5.wav");

	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() + Global::PlayerHeadOffset.iY() + 10};
	TearDir = FVector2D::ZERO;

	// 눈물이 좌/우로 번갈아 발사되는 디테일을 위해 Pos를 한번 더 조정한다.
	int LRPos = 20;
	if (UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		TearDir = FVector2D::LEFT;
		HeadState = UpperState::ATTACK_LEFT;

		if (true == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_4.wav");
			TearPos = TearPos + FVector2D{ -LRPos, -3 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_5.wav");
			TearPos = TearPos + FVector2D{ -LRPos, 3 };
			LeftFire = true;
		}
	}
	if (UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		TearDir = FVector2D::RIGHT;
		HeadState = UpperState::ATTACK_RIGHT;

		if (true == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_4.wav");
			TearPos = TearPos + FVector2D{ +LRPos, -3 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_5.wav");
			TearPos = TearPos + FVector2D{ +LRPos, 3 };
			LeftFire = true;
		}
	}
	if (UEngineInput::GetInst().IsPress(VK_UP))
	{
		TearDir = FVector2D::UP;
		HeadState = UpperState::ATTACK_UP;

		if (true == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_4.wav");
			TearPos = TearPos + FVector2D{ -7, -LRPos };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_5.wav");
			TearPos = TearPos + FVector2D{ +7, -LRPos };
			LeftFire = true;
		}
	}
	if (UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		TearDir = FVector2D::DOWN;
		HeadState = UpperState::ATTACK_DOWN;

		if (true == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_4.wav");
			TearPos = TearPos + FVector2D{ -7, 0 };
			LeftFire = false;
		}
		else if (false == LeftFire)
		{
			Sound = UEngineSound::Play("tear_fire_5.wav");
			TearPos = TearPos + FVector2D{ +7, 0 };
			LeftFire = true;
		}
	}

	// 아이템 능력
	if (nullptr != DetectCollision)
	{
		if (FVector2D::ZERO != DetectCollision->GetComponentScale())
		{
			if (FVector2D::ZERO != ChaseTearDir)
			{
				TearDir = ChaseTearDir;
			}
		}
	}

	float PlayerSpeed = FinalSpeed.Length();

	if (nullptr != PassiveItem)
	{
		std::list<AItem*>::iterator StartIter = Items.begin();
		std::list<AItem*>::iterator EndIter = Items.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AItem* Item = *StartIter;

			Item->TearFire(this, TearPos, TearDir, PlayerSpeed);
		}
	}
	else
	{
		ATear* Tear = GetWorld()->SpawnActor<ATear>();
		Tear->Fire(this, nullptr, TearPos, TearDir, Att, PlayerSpeed, TearSpeed, TearDuration, TearScale);
	}



	SetAttackDir(HeadState);
}

void APlayer::SetAttackDir(const UpperState& _HeadState)
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
	float FrameSpeed = 0.08f;
	BodyRenderer->CreateAnimation("Body_Left", "Body.png", 1, 9, FrameSpeed);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, FrameSpeed);
	BodyRenderer->CreateAnimation("Body_Down", "Body.png", 20, 29, FrameSpeed);
	BodyRenderer->CreateAnimation("Body_Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Body_Idle", "Body.png", 29, 29, FrameSpeed);
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

	HeadRenderer->SetComponentLocation({ 0, Global::PlayerHeadOffset.iY() + 4});
	HeadRenderer->SetComponentScale({ 64, 64 });
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::Player);
	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);

	////////////////////////////////////////////////////////////////////////////////
	// Event
	FullRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FullRenderer->CreateAnimation("Death", "Isaac.png", { 0, 6, 3 }, {0.2f, 0.12f, 0.1f}, false);
	FullRenderer->CreateAnimation("Damaged", "Isaac.png", 6, 6, 0.1f);
	FullRenderer->CreateAnimation("Drop", "Isaac.png", 5, 5, 0.1f);
	FullRenderer->CreateAnimation("Consume", "Isaac.png", 2, 2, 0.1f);
	FullRenderer->SetComponentScale({ 128, 128 });
	FullRenderer->SetOrder(ERenderOrder::Player);
	FullRenderer->SetComponentLocation({ 5, -20 });
	FullRenderer->ChangeAnimation("Damaged");
	FullRenderer->SetActive(false);

}

void APlayer::UISetting()
{
	// Heart
	PlayerHpToHeart = GetWorld()->SpawnActor<AHeartUI>();
	PlayerHpToHeart->SetName("PlayerHp_UI");
	PlayerHpToHeart->SetTextSpriteName("hearts.png");
	PlayerHpToHeart->SetOrder(ERenderOrder::UI);
	PlayerHpToHeart->SetTextScale({ 32, 32 });
	PlayerHpToHeart->SetActorLocation({ 140, 50 });

	// Penny
	PennyUI = GetWorld()->SpawnActor<APickupItemUI>();
	PlayerHpToHeart->SetName("Penny_UI");
	PennyUI->SetTextSpriteName("ui_crafting.png");
	PennyUI->SetOrder(ERenderOrder::UI);
	PennyUI->SetTextScale({ 34, 38 });
	PennyUI->SetActorLocation({ 57, 95 });
	PennyUI->SetValue(8); // SetSpriteIndex

	PennyPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	PennyPickupNumber->SetName("PennyPickupNumber");
	PennyPickupNumber->SetTextSpriteName("pickup.png");
	PennyPickupNumber->SetOrder(ERenderOrder::UI);
	PennyPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	PennyPickupNumber->SetActorLocation({ 77, 95 });

	// Bomb
	FVector2D Offset = FVector2D(0, +25);
	BombUI = GetWorld()->SpawnActor<APickupItemUI>();
	BombUI->SetName("BombUI");
	BombUI->SetTextSpriteName("ui_crafting.png");
	BombUI->SetOrder(ERenderOrder::UI);
	BombUI->SetTextScale({ 32, 32 });
	BombUI->SetActorLocation(FVector2D(55, 95) + Offset);
	BombUI->SetValue(15); //SetSpriteIndex

	BombPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	BombPickupNumber->SetName("BombPickupNumber");
	BombPickupNumber->SetTextSpriteName("pickup.png");
	BombPickupNumber->SetOrder(ERenderOrder::UI);
	BombPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	BombPickupNumber->SetActorLocation(PennyPickupNumber->GetActorLocation() + Offset);

	// Key
	KeyUI = GetWorld()->SpawnActor<APickupItemUI>();
	KeyUI->SetName("KeyUI");
	KeyUI->SetTextSpriteName("ui_crafting.png");
	KeyUI->SetOrder(ERenderOrder::UI);
	KeyUI->SetTextScale({ 32, 32 });
	KeyUI->SetActorLocation(BombUI->GetActorLocation() + Offset);
	KeyUI->SetValue(12); //SetSpriteIndex

	KeyPickupNumber = GetWorld()->SpawnActor<APickupNumberUI>();
	KeyPickupNumber->SetName("KeyPickupNumber");
	KeyPickupNumber->SetTextSpriteName("pickup.png");
	KeyPickupNumber->SetOrder(ERenderOrder::UI);
	KeyPickupNumber->SetTextScale({ 20, 24 }); // 10, 12
	KeyPickupNumber->SetActorLocation(BombPickupNumber->GetActorLocation() + Offset);
}

void APlayer::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * FadeWeight * FadeDir;
	BodyRenderer->SetAlphaFloat(FadeValue);
	HeadRenderer->SetAlphaFloat(FadeValue);
}

void APlayer::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	FadeWeight = 0.5f;
	TimeEventer.PushEvent(0.5f, std::bind(&APlayer::FadeChange, this), true, false);
}

void APlayer::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	FadeWeight = 0.5f;
	TimeEventer.PushEvent(0.5f, std::bind(&APlayer::FadeChange, this), true, false);
}

void APlayer::SpiritFadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	FadeWeight = 0.4f;
	TimeEventer.PushEvent(3.0f, std::bind(&APlayer::FadeChange, this), true, false);
}

void APlayer::CurStateAnimation(const float& _DeltaTime)
{
	if (true == IsDead) // 죽었으면 리턴
	{
		return;
	}

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

void APlayer::PlayerDebugSetting(const float& _DeltaTime)
{
	//CameraPosMove(_DeltaTime);
	CheatKey(_DeltaTime);
	ResetDebug();

	UEngineDebug::CoreOutPutString("FinalSpeed : " + FinalSpeed.ToString());
	//UEngineDebug::CoreOutPutString("Body : " + BodyRenderer->GetComponentLocation().ToString());
	//UEngineDebug::CoreOutPutString("Head : " + HeadRenderer->GetComponentLocation().ToString());
}

void APlayer::ResetDebug()
{
	if (UEngineInput::GetInst().IsDown(VK_F1))
	{
		BodyCollision->SetActiveSwitch();
		SwitchInvincibility();
	}

	if (UEngineInput::GetInst().IsDown(VK_F4))
	{
		Reset();
		UEngineSound::AllSoundStop();
		Sound = UEngineSound::Play("title_screen_jingle_v1_01.ogg");
		UEngineAPICore::GetCore()->ResetLevel<APlayGameMode, APlayer>("Play");
	}

	if (UEngineInput::GetInst().IsDown('Z'))
	{
		APlayGameMode::SwitchGamePause();
	}
}

void APlayer::PlayerLimit()
{
	if (Heart >= HeartMax)
	{
		Heart = HeartMax;
	}
}

void APlayer::CheatKey(const float& _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (UEngineInput::GetInst().IsDown('I'))
	{
		Heart = 0;
	}
	if (UEngineInput::GetInst().IsDown(VK_F2))
	{
		Heart = HeartMax;
	}
	if (UEngineInput::GetInst().IsDown(VK_F3))
	{
		Heart -= 1;
	}
	if (UEngineInput::GetInst().IsDown('J'))
	{
		if (false == IsBombCheat) // 딱 1번만 적용
		{
			for (int i = 0; i < 5; i++)
			{
				AItem* InitBomb = ARoom::GetCurRoom()->CreateItem<ABomb>(this);
				TryPickupItem(InitBomb, 1);
			}
			IsBombCheat = true;
		}
	}
	if (UEngineInput::GetInst().IsDown(VK_F7))
	{
		ARoom::GetCurRoom()->CreateItem<AInnerEye>(this, {50, -20});
	}
}