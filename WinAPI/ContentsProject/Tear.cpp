#include "PreCompile.h"
#include "Tear.h"
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"

ATear::ATear()
{
	SetActorLocation(FVector2D::ZERO);
	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoofa.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoofa.png", 1, 15, 0.025f, false);
	TearEffectRenderer->SetComponentScale({ 64, 64 });
	TearEffectRenderer->SetOrder(ERenderOrder::TEAR);
	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");
	TearEffectRenderer->SetActive(true);
}

// 값만 받아서 멤버에 저장한다.
void ATear::Fire(FVector2D _StartPos, FVector2D _Dir)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir;

	// Debug
	//FVector2D Offset = GetActorLocation();
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

	AddActorLocation(Dir * _DeltaTime * Speed);

	// Tear 비활성화 조건
	// 1. 일정 시간이 지나면
	TimeElapesd += _DeltaTime;
	if (1.0f < TimeElapesd)
	{
		TearEffectRenderer->ChangeAnimation("Player_Tear_Attack");
		Dir = FVector2D::ZERO; // 그 자리에서 더 이상 이동않고 터뜨린다.

		if (1.5f < TimeElapesd)
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