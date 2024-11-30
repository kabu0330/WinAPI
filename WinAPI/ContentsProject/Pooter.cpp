#include "PreCompile.h"
#include "Pooter.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

APooter::APooter()
{
	/* 이름     : */ SetName("Pooter");
	/* 체력     : */ SetHp(14);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(10);
	/* 이동시간 : */ SetMoveDuration(1.0f);
	/* 정지시간 : */ SetMoveCooldown(1.0f);
	/* 탐색범위 : */ SetDetectRange({ 600 , 600 });
	/* 발사속도 : */ SetShootingSpeed(350.0f);
	/* 쿨타임   : */ SetCooldown(3.0f);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Pooter_Left.png", { 0, 1, 14, 15 }, 0.15f);
	BodyRenderer->CreateAnimation("Idle_Right", "Pooter_Right.png", { 0, 1 }, 0.15f); // 이미지가 조금 안맞는듯...?
	BodyRenderer->CreateAnimation("Attack", "Pooter_Left.png", 2, 15, 0.05f, false);
	BodyRenderer->CreateAnimation("Attack_Right", "Pooter_Right.png", 2, 15, 0.05f, false);
	BodyRenderer->SetComponentLocation({ 0, 13 });
	BodyRenderer->SetComponentScale({ 128, 128 });
	BodyRenderer->ChangeAnimation("Idle_Right");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Circle);
	DetectCollision->SetActive(true);
}

APooter::~APooter()
{
}

void APooter::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

}

void APooter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

}

void APooter::ChangeAnimIdle()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (State == MonsterState::LEFT)
	{
		BodyRenderer->ChangeAnimation("Idle");
	}
	else
	{
		BodyRenderer->ChangeAnimation("Idle_Right");
	}
	
}

void APooter::Attack(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	// 공격패턴

	std::string CurAnimationName = BodyRenderer->GetCurAnimationName();

	if (false == IsAttacking())
	{
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			TimeEventer.PushEvent(0.01f, std::bind(&AMonster::ChangeAnimIdle, this));
		}
	}

	CooldownElapsed += _DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}

	if ("Idle" == CurAnimationName)
	{
		BodyRenderer->ChangeAnimation("Attack");
	}
	else if ("Idle_Right" == CurAnimationName)
	{
		BodyRenderer->ChangeAnimation("Attack_Right");
	}

	PreparationElapesd += _DeltaTime;
	if (PreparationElapesd < PreparationDuration)
	{
		return;
	}

	IsAttack = true;
	TimeEventer.PushEvent(BodyRenderer->IsCurAnimationEnd(), std::bind(&AMonster::SwitchIsAttacking, this));

	Sound = UEngineSound::Play("blood_fire.wav");

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() + 12 };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CooldownElapsed = 0.0f;
	PreparationElapesd = 0.0f;
}

FVector2D APooter::GetRandomDir()
{
	FVector2D LeftTop = FVector2D::LEFT + FVector2D::UP;
	LeftTop.Normalize();

	FVector2D LeftBot = FVector2D::LEFT + FVector2D::DOWN;
	LeftBot.Normalize();

	FVector2D RightTop = FVector2D::RIGHT + FVector2D::UP;
	RightTop.Normalize();

	FVector2D RightBot = FVector2D::RIGHT + FVector2D::DOWN;
	RightBot.Normalize();

	static UEngineRandom RandomPooter;
	RandomPooter.SetSeed(time(nullptr));
	int Result = RandomPooter.RandomInt(0, 7);

	if (PrevDir == Result) // 이전에 이동한 방향과 같으면 다시 이동할 방향 랜덤 돌려
	{
		UEngineRandom Reroll;
		Reroll.SetSeed(time(nullptr) + Result + 1); // Result가 0일수도 있으니까 1 더해서 반드시 시드값을 바꾼다.
		int RerollResult = Reroll.RandomInt(0, 7);
		Result = RerollResult;
	}

	FVector2D Dir = FVector2D::ZERO;
	switch (Result)
	{
	case 0:
		Dir = FVector2D::LEFT;
		State = MonsterState::LEFT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle");
		}
		PrevDir = 0;
		break;
	case 1:
		Dir = FVector2D::RIGHT;
		State = MonsterState::RIGHT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle_Right");
		}
		PrevDir = 1;
		break;
	case 2:
		Dir = FVector2D::UP;
		State = MonsterState::LEFT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle");
		}
		PrevDir = 2;
		break;
	case 3:
		Dir = FVector2D::DOWN;
		State = MonsterState::RIGHT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle_Right");
		}
		PrevDir = 3;
		break;
	case 4:
		State = MonsterState::LEFT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle");
		}
		Dir = LeftTop;
		PrevDir = 4;
		break;
	case 5:
		Dir = LeftBot;
		State = MonsterState::LEFT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle");
		}
		PrevDir = 5;
		break;
	case 6:
		Dir = RightTop;
		State = MonsterState::RIGHT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle_Right");
		}
		PrevDir = 6;
		break;
	case 7:
		Dir = RightBot;
		State = MonsterState::RIGHT;
		if (false == IsAttack)
		{
			BodyRenderer->ChangeAnimation("Idle_Right");
		}
		PrevDir = 7;
		break;
	default:
		break;
	}

	MonsterState CurState = State;
	return FVector2D(Dir);
}





