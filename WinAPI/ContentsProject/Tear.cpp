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

}

ATear::~ATear()
{
}

void ATear::BeginPlay()
{
}

void ATear::Tick(float _DeltaTime)
{
	if (true)
	{

	}
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);

	TimeElapesd += _DeltaTime;

	if (1.0f < TimeElapesd)
	{
		TearEffectRenderer->ChangeAnimation("Player_Tear_Attack");
		//this->Destroy(2.0f);
		
	}
}

