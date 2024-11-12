#include "PreCompile.h"
#include "Monster.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Global.h"
#include "ContentsEnum.h"


AMonster::AMonster()
{
	SetName("Fly");
	SetActorLocation({ Global::WindowHalfScale.X + 250, Global::WindowHalfScale.Y });

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 35, 35 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::MONSTER_BODY);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->CreateAnimation("Fly.IDle", "Fly.png", 1, 2, 0.1f);
	Renderer->CreateAnimation("Fly_Death", "Fly.png", 4, 14, 0.03f, false);
	Renderer->SetComponentScale({256, 256});
	Renderer->ChangeAnimation("Fly.IDle");

	SetHp(10);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale({ 450, 450 });
	DetectCollision->SetCollisionGroup(ECollisionGroup::MONSETR_DETECTINRANGE);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);


	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	Move(_DeltaTime);
	ChasePlayer(_DeltaTime);


	DeathCheck(_DeltaTime);
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

void AMonster::ChaseMove(float _DeltaTime)
{
	Direction = GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMonster::Move(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected) // 플레이어를 추적하지 않는 중이면
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveTime)
	{
		if (MoveElapsedTime > MoveCooldown)
		{
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	Direction = FVector2D::LEFT;
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);

}

FVector2D AMonster::GetMoveDir()
{
	UEngineRandom Random;
	Random.SetSeed(time(nullptr));
	Random.RandomInt(0, 7);

	return FVector2D();
}

void AMonster::DeathCheck(float _DeltaTime)
{
	if (false == IsDeath())
	{
		return;
	}

	Death(_DeltaTime);
}

void AMonster::Death(float _DeltaTime)
{
	if (nullptr != BodyCollision)
	{
		BodyCollision->Destroy();
		BodyCollision = nullptr;
		Renderer->ChangeAnimation("Fly_Death");
	}

	Renderer->Destroy(0.3f);
	this->Destroy(0.4f);
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

bool AMonster::IsPlayerNearby()
{
	AActor* Player = GetWorld()->GetPawn();
	FVector2D PlayerPos = Player->GetActorLocation();
	FVector2D MonsterPos = this->GetActorLocation();
	FVector2D Distance = PlayerPos - MonsterPos;

	float Length = Distance.Length();
	float DetectLength = DetectCollision->GetComponentScale().Half().iX();

	if (DetectLength >= Length)
	{
		return true;
	}
	return false;
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

AMonster::~AMonster()
{
}
