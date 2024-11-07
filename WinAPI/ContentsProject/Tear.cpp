#include "PreCompile.h"
#include "Tear.h"
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"


ATear::ATear()
{
	SetActorLocation(FVector2D::ZERO);
	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoofa.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoofa.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 96, 96 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::TEAR);
	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");
	TearEffectRenderer->SetActive(true);
}

// 값만 받아서 멤버에 저장한다.
void ATear::Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir;

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	float FinalSpeed = 0.0f;
	if (true == Player->PlayerIsMove())
	{
		FinalSpeed = Speed + _Speed;
		Duration = 0.8f;
	}
	else
	{
		FinalSpeed = Speed;
	}

	Speed = FinalSpeed / Duration;
	if (Speed > SpeedMax)
	{
		Speed = SpeedMax;
		ResistanceActivationTime = 0.2f;
		Resistance = 0.7f;
		GravityActivationTime = 0.5f;
	}
}

void ATear::Reset()
{
	TearEffectRenderer->SetActive(false);
}

void ATear::BeginPlay()
{
	Super::BeginPlay();
}

void ATear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (false == TearEffectRenderer->IsActive())
	{
		return;
	}

	TimeElapesd += _DeltaTime;
	if (Dir == FVector2D::LEFT || Dir == FVector2D::RIGHT)
	{
		if (GravityActivationTime + 0.1 < TimeElapesd) // 마지막에 중력으로 떨어뜨린다.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}

		else if (GravityActivationTime < TimeElapesd) // 속도를 늦춘다.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}
		else if (ResistanceActivationTime < TimeElapesd)
		{
			AddActorLocation(Dir * _DeltaTime * Speed * Resistance);
		}
		else
		{
			AddActorLocation(Dir * _DeltaTime * Speed);
		}
	}
	else
	{
		if (ResistanceActivationTime < TimeElapesd)
		{
			AddActorLocation(Dir * _DeltaTime * Speed * Resistance);
		}
		else
		{
			AddActorLocation(Dir * _DeltaTime * Speed);
		}
	}


	// Tear 비활성화 조건
	// 1. 일정 시간이 지나면
	if (Duration < TimeElapesd)
	{
		Dir = FVector2D::ZERO;        // 그 자리에서 더 이상 이동않고 터뜨린다.
		TearEffectRenderer->ChangeAnimation("Player_Tear_Attack");
		SetActorLocation(GetActorLocation());

		if (Duration + 0.5f < TimeElapesd) // 마지막 애니메이션까지 렌더될 시간을 번다.
		{
			Reset();
		}
	}

	// 2. 맵 밖으로 벗어나면
	// 3. 액터와 충돌하면

}

ATear::~ATear()
{
}