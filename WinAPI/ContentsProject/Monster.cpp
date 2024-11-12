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

	BodyCollisionCheck(_DeltaTime);

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

void AMonster::BodyCollisionCheck(float _DeltaTime)
{
	AActor* CollisionActor = BodyCollision->CollisionOnce(ECollisionGroup::PLAYER_BODY);

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
	CollisionPlayer->ApplyDamaged(1);
	CollisionPlayer->CollisionEnter(CollisionPlayer);

	BodyCollisionCooldownElapsed = 0.0f;

	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "���� " + std::to_string(1) + " �� �浹 �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionPlayer->GetHp()));

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
	if (true == PlayerDetected) // �÷��̾ �������� �ʴ� ���̸�
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveTime) // ���� �ð� �̵��ϸ� ����
	{
		if (MoveElapsedTime > MoveCooldown) // ���� �� ���� �ð��� �帣�� �ٽ� �̵�
		{
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	Direction = GetRandomDir();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
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

	UEngineRandom Random;
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
	float DetectLength = DetectCollision->GetComponentScale().Half().X;

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
