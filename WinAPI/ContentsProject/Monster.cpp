#include "PreCompile.h"
#include "Monster.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"

AMonster::AMonster()
{
	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

		AActor* MainPawn = GetWorld()->GetPawn();
	Player = dynamic_cast<APlayer*>(MainPawn);
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}

	if (true == APlayGameMode::IsGamePaused())
	{
		return;
	}

	ClampPositionToRoom();
	Move(_DeltaTime);
	ChasePlayer(_DeltaTime);

	BodyCollisionCheck(_DeltaTime);

	DeathCheck(_DeltaTime);
}

void AMonster::Death(float _DeltaTime)
{
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
		RendererDestroy();
	}

	// 모든 몬스터의 사망 애니메이션 이름은 "Death"로 고정한다. 아니면 자식클래스에서 재정의
	BodyRenderer->ChangeAnimation("Death");

	if (false == BodyRenderer->IsCurAnimationEnd())
	{
		return;
	}
	Destroy();
}

void AMonster::ChaseMove(float _DeltaTime)
{
	Direction = GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMonster::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected)
	{
		return;
	}

	Attack(_DeltaTime);
	ChaseMove(_DeltaTime);
}

void AMonster::Move(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected) // 플레이어를 추적하지 않는 중이면
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // 일정 시간 이동하면 리턴
	{
		if (MoveElapsedTime > MoveCooldown) // 멈춘 뒤 일정 시간이 흐르면 다시 이동
		{
			Direction = GetRandomDir();
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}


	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMonster::Attack(float _DeltaTime)
{
	CoolDownElapsed += _DeltaTime;
	if (CoolDownElapsed < Cooldown)
	{
		return;
	}

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CoolDownElapsed = 0.0f;
}

FVector2D AMonster::GetRandomDir()
{
	FVector2D LeftTop = FVector2D::LEFT + FVector2D::UP;
	LeftTop.Normalize();

	FVector2D LeftBot = FVector2D::LEFT + FVector2D::DOWN;
	LeftBot.Normalize();

	FVector2D RightTop = FVector2D::RIGHT + FVector2D::UP;
	RightTop.Normalize();

	FVector2D RightBot = FVector2D::RIGHT + FVector2D::DOWN;
	RightBot.Normalize();

	static UEngineRandom Random;
	Random.SetSeed(time(nullptr));
	int Result = Random.RandomInt(0, 7);

	FVector2D Dir = FVector2D::ZERO;
	switch (Result)
	{
	case 0:
		Dir = FVector2D::LEFT;
		break;
	case 1:
		Dir = FVector2D::RIGHT;
		break;
	case 2:
		Dir = FVector2D::UP;
		break;
	case 3:
		Dir = FVector2D::DOWN;
		break;
	case 4:
		Dir = LeftTop;
		break;
	case 5:
		Dir = LeftBot;
		break;
	case 6:
		Dir = RightTop;
		break;
	case 7:
		Dir = RightBot;
		break;
	default:
		break;
	}

	return FVector2D(Dir);
}

FVector2D AMonster::GetDirectionToPlayer()
{
	AActor* Player = GetWorld()->GetPawn();
	FVector2D PlayerPos = Player->GetActorLocation();
	FVector2D MonsterPos = this->GetActorLocation();
	FVector2D Distance = PlayerPos - MonsterPos;

	Distance.Normalize();

	return Distance;
}

void AMonster::ClampPositionToRoom()
{
	if (nullptr == BodyCollision)
	{
		return;
	}

	FVector2D Pos = GetActorLocation();
	FVector2D OffsetPos = Pos + BodyCollision->GetComponentLocation();

	ARoom* CurRoom = ParentRoom;
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	if (LeftEdge > OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ 1, 0 });
	}
	if (RightEdge < OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -1, 0 });
	}
	if (TopEdge > OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 1 });
	}
	if (BotEdge < OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -1 });
	}
}

bool AMonster::IsPlayerNearby()
{
	if (nullptr == DetectCollision)
	{
		return false;
	}
	AActor* Player = GetWorld()->GetPawn();
	FVector2D PlayerPos = Player->GetActorLocation();
	FVector2D MonsterPos = this->GetActorLocation();
	FVector2D Distance = PlayerPos - MonsterPos;

	float Length = Distance.Length();
	float DetectLength = DetectCollision->GetComponentScale().Half().X;

	if (DetectLength >= Length)
	{
		return true;
	}
	return false;
}

void AMonster::BodyCollisionCheck(float _DeltaTime)
{
	if (nullptr == BodyCollision)
	{
		return;
	}

	AActor* CollisionActor = BodyCollision->CollisionOnce(ECollisionGroup::Player_Body);

	if (nullptr == CollisionActor)
	{
		return;
	}

	BodyCollisionCooldownElapsed += _DeltaTime;
	if (BodyCollisionCooldownElapsed < BodyCollisionCooldown)
	{
		return;
	}

	APlayer* CollisionPlayer = dynamic_cast<APlayer*>(CollisionActor);
	CollisionPlayer->ApplyDamaged(CollisionAtt);
	CollisionPlayer->ShowHitAnimation(CollisionPlayer);

	BodyCollisionCooldownElapsed = 0.0f;

	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "에게 " + std::to_string(CollisionAtt) + " 의 충돌 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionPlayer->GetHp()));

}

AMonster::~AMonster()
{
}
