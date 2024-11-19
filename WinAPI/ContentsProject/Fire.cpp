#include "PreCompile.h"
#include "Fire.h"

AFire::AFire()
{
	SetName("Fire");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 40, 40 };
	BodyCollisionLocation = { 0, -20 };
	FVector2D BlockingPathCollisionScale = { 30 , 30 };
	FVector2D BlockingPathCollisionPivot = { 0, 5 };

	Hp = 4;
	CanExplode = false;
	IsTearDamageable = true;
	IsBlockingPath = false;
	IsAttackable = true;

	float BurningFrame = 0.3f;
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("FireBottom_Flame", "fire_bottom.png", 1, 1, BurningFrame);
	BodyRenderer->CreateAnimation("FireBottom_Burned", "fire_bottom.png", {0, 4, 5}, 1.0f);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->ChangeAnimation("FireBottom_Flame");
	BodyRenderer->SetActive(true);

	FlameBaseRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FlameBaseRenderer->CreateAnimation("FireBottom_Flame", "fire_top.png", 0, 5, BurningFrame);
	FlameBaseRenderer->SetComponentLocation({ 0, 0 });
	FlameBaseRenderer->SetComponentScale(BodyRendererScale * 3.5f);
	FlameBaseRenderer->SetOrder(ERenderOrder::Object_Back);
	FlameBaseRenderer->ChangeAnimation("FireBottom_Flame");
	FlameBaseRenderer->SetActive(true);
	FlameBaseRenderer->SetAlphaFloat(0.8f);


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation(BodyCollisionLocation);
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	//BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	//BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	//BlockingPathCollision->SetComponentScale(BlockingPathCollisionScale);
	//BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	//BlockingPathCollision->SetCollisionType(ECollisionType::Rect);
}

AFire::~AFire()
{
}

void AFire::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void AFire::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);

}

void AFire::SwitchAnimation()
{
	if (false == IsTearDamageable)
	{
		return;
	}

	switch (Hp)
	{
	case 4:
		break;
	case 3:
		FlameBaseRenderer->SetComponentLocation({ 0, 0 });
		FlameBaseRenderer->SetComponentScale(BodyRendererScale * 2.5f);

		BodyCollisionLocation = { 0, -15 };
		BodyCollisionScale = { 30, 30 };
		BodyCollision->SetComponentLocation(BodyCollisionLocation);
		BodyCollision->SetComponentScale(BodyCollisionScale);
		break;
	case 2:
		FlameBaseRenderer->SetComponentLocation({ 0, 0 });
		FlameBaseRenderer->SetComponentScale(BodyRendererScale * 1.8f);

		BodyCollisionLocation = { 0, -10 };
		BodyCollisionScale = { 20, 20 };
		BodyCollision->SetComponentLocation(BodyCollisionLocation);
		BodyCollision->SetComponentScale(BodyCollisionScale);

		break;
	case 1:
	{
		FlameBaseRenderer->SetComponentLocation({ 0, 0 });
		FlameBaseRenderer->SetComponentScale(BodyRendererScale * 1.2f);

		BodyCollisionLocation = { 0, -5 };
		BodyCollisionScale = { 15, 15 };
		BodyCollision->SetComponentLocation(BodyCollisionLocation);
		BodyCollision->SetComponentScale(BodyCollisionScale);

	}
		break;
	case 0: // 충돌체가 파괴되고 남은 잔해물
	{
		if (nullptr != FlameBaseRenderer)
		{
			FlameBaseRenderer->SetActive(false);
			FlameBaseRenderer->Destroy();
		}
		if (nullptr != BodyCollision)
		{
			BodyCollision->Destroy();
		}
		BodyRenderer->ChangeAnimation("FireBottom_Burned");
	}
		break;
	default:
		break;
	}
}

