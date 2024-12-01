#include "PreCompile.h"
#include "Fire.h"
#include "Room.h"

AFire::AFire()
{
	SetName("Fire");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 40, 60 };
	BodyCollisionLocation = { 0, -20 };

	Hp = 3;
	CanExplode = false;
	IsTearDamageable = true;
	IsBlockingPath = false;
	IsAttackable = true;

	float BurningFrame = 0.2f;
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("FireBottom_Flame", "fire_bottom.png", 1, 1, BurningFrame);
	BodyRenderer->CreateAnimation("FireBottom_Burned", "fire_bottom.png", 1, 1, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_PlayerBack);
	BodyRenderer->ChangeAnimation("FireBottom_Flame");
	BodyRenderer->SetActive(true);

	FlameBaseRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FlameBaseRenderer->CreateAnimation("FireBottom_Flame", "fire_top.png", 0, 5, BurningFrame);
	FlameBaseRenderer->SetComponentLocation({ 0, 0 });
	FlameBaseRenderer->SetComponentScale(BodyRendererScale * 4.0f);
	FlameBaseRenderer->SetOrder(ERenderOrder::Object_PlayerBack);
	FlameBaseRenderer->ChangeAnimation("FireBottom_Flame");
	FlameBaseRenderer->SetActive(true);
	FlameBaseRenderer->SetAlphaFloat(0.7f);


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation(BodyCollisionLocation);
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);
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
	case 3:
		break;
	case 2:
	
		FlameBaseRenderer->SetComponentLocation({ 0, 0 });
		FlameBaseRenderer->SetComponentScale(BodyRendererScale * 3.0f);

		BodyCollisionLocation = { 0, -10 };
		BodyCollisionScale = { 30, 30 };
		BodyCollision->SetComponentLocation(BodyCollisionLocation);
		BodyCollision->SetComponentScale(BodyCollisionScale);

		break;
	case 1:
	{

		FlameBaseRenderer->SetComponentLocation({ 0, 0 });
		FlameBaseRenderer->SetComponentScale(BodyRendererScale * 1.5f);

		BodyCollisionLocation = { 0, -5 };
		BodyCollisionScale = { 25, 25 };
		BodyCollision->SetComponentLocation(BodyCollisionLocation);
		BodyCollision->SetComponentScale(BodyCollisionScale);

	}
		break;
	case 0: // 충돌체가 파괴되고 남은 잔해물
	{
		if (false == IsFireDeath)
		{
			Sound = UEngineSound::Play("firedeath hiss.wav");
			Sound.SetVolume(0.5f);
			IsFireDeath = true;
		}
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

