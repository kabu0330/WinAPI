#include "PreCompile.h"
#include "Monster.h"

#include <EngineBase/EngineMath.h>
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

	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	GetDirectionToPlayer();
	Attack(_DeltaTime);

	DeathCheck(_DeltaTime);
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
	Tear->Fire(TearPos, TearDir, ProjectileSpeed, Att);
	CoolDownElapsed = 0.0f;

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
