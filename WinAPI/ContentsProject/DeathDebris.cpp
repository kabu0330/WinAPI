#include "PreCompile.h"
#include "DeathDebris.h"

#include "ContentsEnum.h"

DeathDebris::DeathDebris()
{
	BloodEffect = CreateDefaultSubObject<USpriteRenderer>();
	BloodEffect->CreateAnimation("DeathEffect", "LargeBloodExplosion.png", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 0.1f, false);

	BloodEffect->SetComponentLocation(BloodEffectLocation);
	BloodEffect->SetComponentScale(BloodEffectScale);
	BloodEffect->SetOrder(ERenderOrder::MonsterDeathDebris);
	BloodEffect->ChangeAnimation("DeathEffect");
	BloodEffect->SetActive(true);
}

DeathDebris::~DeathDebris()
{
}

void DeathDebris::BeginPlay()
{
	Super::BeginPlay();
}

void DeathDebris::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

