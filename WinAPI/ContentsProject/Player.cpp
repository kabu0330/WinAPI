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

#include "Item.h"

int APlayer::Heart = 6;
int APlayer::HeartMax = 8;

APlayer::APlayer()
{
	SetName("Isaac");

	SetActorLocation(Global::WindowSize.Half()); // 1. Actor�� ��ġ�� �ǹ̰� �־ ũ��� �ǹ̰� ����.
	InitPos = GetActorLocation();

	SpriteSetting(); // 2. ���¿� ���� �ִϸ��̼� ������ �����Ѵ�.
	CollisionSetting(); // 3. ĳ������ �̵������� ������ �浹ü�� �����Ѵ�. 

	DebugOn(); // ����� ��� ����
	//SwitchInvincibility(); // ������ ����
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	UISetting();
	CollisionFuctionSetting();

	//BGM = UEngineSound::Play("doorOpen.wav");
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// ����� ��� ����
	PlayerDebugSetting(_DeltaTime);

	Death(_DeltaTime);
	UITick(_DeltaTime);

	if (true == APlayGameMode::IsGamePaused()) // �޴��� ���� ����
	{
		return;
	}
	if (true == IsDeath()) // �÷��̾ ������ ����
	{
		return;
	}

	ReverseForce(_DeltaTime);

	IsCameraMove(); // ����

	// ����
	CurStateAnimation(_DeltaTime);

	ARoom* CurRoom = ARoom::GetCurRoom();
	if (true == CurRoom->IsCameraMove()) 
	{
		return; // ���� �� ���Ͱ� �����Ǵ� ����ð� ���� �������� �ʵ��� 
	}

	// ����
	KnockbackTick(_DeltaTime);

	Move(_DeltaTime);
	InputAttack(_DeltaTime);

}

void APlayer::CollisionFuctionSetting()
{
	BodyCollision->SetCollisionEnter(std::bind(&APlayer::ShowHitAnimation, this, std::placeholders::_1));
	WarpCollision->SetCollisionStay(std::bind(&APlayer::ClampPositionToRoom, this));
}

// �� �ٱ����� ������ ���ϰ� ���� �Լ�
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
		SetActorLocation(Pos + FVector2D{1, 0});
	}
	if (RightEdge < FootPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -1, 0 });
	}
	if (TopEdge > FootPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 1 });
	}
	if (BotEdge < FootPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -1 });
	}
}

// ������ ó��, ���� �� �ִϸ��̼� ��� �Լ�
int APlayer::ApplyDamaged(AActor* _Player, int _Att, FVector2D _Dir)
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
	else if (true == Player->IsInvincible()) // �����̸� ����
	{
		return 0;
	}

	if (true == Player->IsDeath())
	{
		return 0;
	}

	Player->ShowHitAnimation(_Player);
	Player->BeginBlinkEffect();

	IsHit = true;
	Invincibility = true;

	KnockbackStartPos = GetActorLocation();
	UpPos = KnockbackStartPos + FVector2D(0.0f, -50.0f);
	FVector2D Offset = _Dir;
	Offset.Normalize();
	KnockbackDistance = Offset * 20.0f; // �˹�Ÿ� Lerp ����

	TimeEventer.PushEvent(1.0f, std::bind(&APlayer::SwitchIsHit, this)); // �������� �ٽ� �Ա���� �ɸ��� �ð�

	Heart -= _Att;
	if (Heart < 0)
	{
		Heart = 0;
	}
	return Heart;
}

void APlayer::KnockbackTick(float _DeltaTime)
{
	if (false == IsHit)
	{
		return;
	}

	KnockbackDuration -= _DeltaTime;
	if (KnockbackDuration <= 0.0f)
	{
		KnockbackDuration = 0.0f;
		return; // �˹� ���� �ð� ����
	}


	float KnockbackLerpAlpha = 1.0f - (KnockbackDuration / 1.0f);
	FVector2D StartPos = FVector2D::ZERO;
	FVector2D CurPos = FVector2D::Lerp(KnockbackStartPos, KnockbackStartPos + KnockbackDistance, KnockbackLerpAlpha);

	float UpOffset = -50.0f * std::sin(KnockbackLerpAlpha * static_cast<float>(std::numbers::pi));
	FVector2D FinalPos = CurPos + FVector2D(0.0f, UpOffset);
	SetActorLocation(FinalPos);
}

void APlayer::ReverseForce(float _DeltaTime)
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
	if (true == Invincibility) // �������¸� ����
	{
		return;
	}
	if (true == FullRenderer->IsActive()) // �̹� �ǰ� ���¸� ����
	{
		return;
	}
	if (true == IsDead) // �׾����� ����
	{
		return;
	}

	FullRenderer->ChangeAnimation("Damaged");
	FullRenderer->SetActive(true);
	BodyRenderer->SetActive(false);
	HeadRenderer->SetActive(false);

	Invincibility = true; // ����

	// ���� �ִϸ��̼����� ����
	TimeEventer.PushEvent(0.3f, std::bind(&APlayer::RestoreDefaultMotion, this));
}

void APlayer::RestoreDefaultMotion()
{
	if (true == IsDead) // �׾����� ����
	{
		return;
	}

	SwitchInvincibility();
	FullRenderer->SetActive(false);
	BodyRenderer->SetActive(true);
	HeadRenderer->SetActive(true);
}

void APlayer::BeginBlinkEffect()
{
	if (true == IsDead) // �׾����� ����
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
	if (true == IsDead) // �׾����� ����
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
	if (5 < FadeCount)
	{
		FadeCount = 0;
		return;
	}
	TimeEventer.PushEvent(0.1f, std::bind(&APlayer::BeginBlinkEffect, this));
}

void APlayer::UITick(float _DeltaTime)
{
	  PlayerHpToHeart->SetPlayerHp(Heart);
	PennyPickupNumber->SetValue(PennyCount);
 	 BombPickupNumber->SetValue(BombCount);
	  KeyPickupNumber->SetValue(KeyCount);
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

void APlayer::Death(float _DeltaTime)
{
	if (false == IsDeath())
	{
		return;
	}

	APlayGameMode::SetGamePaused(true); // ���� ����, ���͵鵵 ����

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

	// ��������Ʈ ȣ��
	TimeEventer.PushEvent(2.0f, std::bind(&APlayer::ShowDeathReport, this));

	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();

	float SpiritSpeed = 100.0f;
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

		// Static�̸� ���� ���� ����� �Ѵ�.
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
}

bool APlayer::Drop(AItem* _Item, int _Count)
{
	return false;
}

void APlayer::Move(float _DeltaTime)
{
	if (true == CameraMove) // �� �̵��� �߿� ĳ���ʹ� ������ �� ����.
	{
		return;
	}

	// ������ �����ӿ� ���� ���ӵ��� ����� ������ �ʹ� Ŀ 60������ �������� ���
	const float FixedTimeStep = 0.016f;
	float RemainingTime = _DeltaTime;
	while (RemainingTime > 0.0f)
	{
		float DeltaTime = UEngineMath::Min(RemainingTime, FixedTimeStep); // ���� ƽ ���
		RemainingTime -= DeltaTime;

		if (true == ProcessMovementInput())
		{
			if (FVector2D::ZERO != Dir)
			{
				Dir.Normalize();
			}

			TargetSpeed = Dir * SpeedMax;

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

			// �⺻ ��߼ӵ��� �־� ���� �ӵ��� ����
			if (30.0f > FinalSpeed.Length())
			{
				FinalSpeed += Dir * (SpeedMax * 0.2f); 
			}

			// ������ȯ
			if (50.0f > FinalSpeed.Length())
			{
				// �ӵ��� ���� �� ���ο� �������� �̵��� ��� �ε巴�� �̵�
				FinalSpeed = FVector2D::Lerp(FinalSpeed, TargetSpeed, MoveAcc * DeltaTime);
			}
			else if (FVector2D::DotProduct(CurDir, Dir) < 0.0f)
			{
				// �ݴ� �����̶�� �����ӵ��� ������ ������ ���̰� ���� �ӵ��� �����Ͽ� ������� �ʰ� ��
				FinalSpeed = FVector2D::Lerp(FinalSpeed, FVector2D::ZERO, MoveAcc * DeltaTime);
				FinalSpeed += Dir * (SpeedMax * 0.3f);
			}
			else // ���� �����̶�� �״��
			{
				FinalSpeed = FinalSpeed + (TargetSpeed - FinalSpeed) * MoveAcc * DeltaTime;
			}
		}

		else // Ű�Է��� ���� ��� ���� ó��
		{
			TargetSpeed = FVector2D::ZERO;

			static const float GlideSpeed = 120.0f;       // �̲����� �ӵ�
			static const float GlideDecayFactor = 0.98f; // ���� ����
			static const float GlideDuration = 0.2f;     // �̲����� ���� �ð� (��)
			static float CurrentGlideTime = 0.0f;        // ���� �̲����� �ð�
			static bool IsGliding = false;               // �̲����� ���� �÷���

			// �̲����� �ʱ�ȭ
			if (!IsGliding)
			{
				IsGliding = true;
				CurrentGlideTime = 0.0f;
			}

			// �̲����� ����
			if (CurrentGlideTime < GlideDuration)
			{
				CurrentGlideTime += DeltaTime;
				FinalSpeed = FinalSpeed.GetNormal() * GlideSpeed; // ���� �ӵ� ����
			}
			else // ���� ó��
			{
				FinalSpeed *= GlideDecayFactor;

				if (FinalSpeed.Length() < 10.0f)
				{
					FinalSpeed = FVector2D::ZERO;
					IsGliding = false; // ���� ����
				}
			}
		}

		// �ִ�ӵ� ���� : �׻� ��������
		if (abs(FinalSpeed.X) > SpeedMax || abs(FinalSpeed.Y) > SpeedMax)
		{
			FinalSpeed.Normalize();
			FinalSpeed *= SpeedMax;
		}

		AddActorLocation(FinalSpeed * DeltaTime); // ���� �̵���
	}

	UpdateHeadState();

	// Ű �Է� ���°� �ƴϸ�
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

	// ���� ���°� �ƴϰ�, Ű�Է� ������ IDLE�� ��ȯ
	if (false == IsAttack())
	{
		HeadState = UpperState::IDLE;
	}
}

bool APlayer::ProcessMovementInput()
{
	Dir = FVector2D::ZERO;
	IsMove = false;

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

	return IsMove;
}

void APlayer::UpdateHeadState()
{
	// HeadState ���� : ���� ������, �ƴ���
	if (true == IsAttack())
	{
		int BodyDir = GetAttackDir(); // ���ݹ����� Attack �Լ����� int�� �޾ƿͼ� ����
		HeadState = static_cast<UpperState>(BodyDir);
	}
	else
	{
		int BodyDir = static_cast<int>(BodyState); // �̵����� �״�� ���� ��ȯ
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
	// ���� �Է��� ó�� ������ �� ����
	if (false == TearFire &&
		(UEngineInput::GetInst().IsDown(VK_LEFT) ||
		UEngineInput::GetInst().IsDown(VK_RIGHT) ||
		UEngineInput::GetInst().IsDown(VK_UP)    ||
		UEngineInput::GetInst().IsDown(VK_DOWN)))
	{
		Attack(_DeltaTime);
	}

	if (true == TearFire)				// false�ϱ� ����. true�� ��ȯ.
	{
		CoolDownElapsed += _DeltaTime; 	// ���������� ��Ÿ�� ��� ����
		if (CoolDownElapsed > Cooldown) //  ��Ÿ���� ����Ǹ�, 
		{
			//TearFire�� false�� �ǵ��� ���� ���� ���·� �ٲٰ� ��Ÿ�� �ʱ�ȭ
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
	TearFire = true; // true�� ��, Cooldown�ð� ���� Attack �Լ��� ȣ��� �� ����.

	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() - HeadRenderer->GetComponentScale().Half().iY() + 10 };

	// ������ ��/��� ������ �߻�Ǵ� �������� ���� Pos�� �ѹ� �� �����Ѵ�.
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
	// ���� ���� �ƴ϶�� Head�� State�� ������ ������ ����.
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

	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 4 });
	HeadRenderer->SetComponentScale({ 64, 64 });
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::Player);
	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);

	////////////////////////////////////////////////////////////////////////////////
	// Event
	FullRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FullRenderer->CreateAnimation("Death", "Isaac.png", { 0, 6, 3 }, {0.2f, 0.12f, 0.1f}, false);
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
	BombPickupNumber->SetValue(BombCount);

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

void APlayer::CurStateAnimation(float _DeltaTime)
{
	if (true == IsDead) // �׾����� ����
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

	// HeadState�� ���� �� ����̶��
	if (static_cast<int>(UpperState::ATTACK_LEFT) <= static_cast<int>(HeadState))
	{
		StateElapesd += _DeltaTime;

		// 0.2�� �̻��� ����Ѵٸ� BodyState ����.
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
	if (UEngineInput::GetInst().IsDown(VK_F1))
	{
		BodyCollision->SetActiveSwitch();
		SwitchInvincibility();
	}

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