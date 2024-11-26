#include "PreCompile.h"
#include "DecalObject.h"
#include <EngineBase/EngineRandom.h>

ADecalObject::ADecalObject()
{
	SetName("DecalObject");
	BodyRendererScale = { 64, 64 };

	CanExplode = false;
	IsTearDamageable = false;
	IsBlockingPath = false;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("bloodpool00", "effect_bloodpool.png",  0,  0, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool01", "effect_bloodpool.png",  1,  1, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool02", "effect_bloodpool.png",  2,  2, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool03", "effect_bloodpool.png",  3,  3, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool04", "effect_bloodpool.png",  4,  4, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool05", "effect_bloodpool.png",  5,  5, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool06", "effect_bloodpool.png",  6,  6, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool07", "effect_bloodpool.png",  7,  7, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool08", "effect_bloodpool.png",  8,  8, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool09", "effect_bloodpool.png",  9,  9, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool10", "effect_bloodpool.png", 10, 10, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool11", "effect_bloodpool.png", 11, 11, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool12", "effect_bloodpool.png", 12, 12, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool13", "effect_bloodpool.png", 13, 13, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool14", "effect_bloodpool.png", 14, 14, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool15", "effect_bloodpool.png", 15, 15, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool16", "effect_bloodpool.png", 16, 16, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool17", "effect_bloodpool.png", 17, 17, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool18", "effect_bloodpool.png", 18, 18, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool19", "effect_bloodpool.png", 19, 19, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool20", "effect_bloodpool.png", 20, 20, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool21", "effect_bloodpool.png", 21, 21, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool22", "effect_bloodpool.png", 22, 22, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool23", "effect_bloodpool.png", 23, 23, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool24", "effect_bloodpool.png", 24, 24, 0.1f, false);

	BodyRenderer->CreateAnimation("bombradius" , "effect_017_bombradius.png", 0, 8, 0.1f, false);


	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Decal);
	BodyRenderer->ChangeAnimation("bloodpool00");
	BodyRenderer->SetAlphaFloat(1.0f);
	BodyRenderer->SetActive(true);

}

void ADecalObject::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();

}

void ADecalObject::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);

	Move(_DeltaTime);
}

void ADecalObject::Move(float _DeltaTime)
{
	if (false == IsMove)
	{
		return;
	}

	FVector2D Gravity = { 0.0f, 1.8f };
	//Gravity *= _DeltaTime;
	Force += Gravity;

	FVector2D ThisPos = GetActorLocation();

	float Duration = 0.5f;
	TimeElapsed += _DeltaTime;
	if (Duration < TimeElapsed)
	{
		IsMove = false;
		Force = FVector2D::ZERO;
		TimeElapsed = 0.0f;
		return;
	}

	AddActorLocation(Force * _DeltaTime);
}

void ADecalObject::SetMove(AActor* _Actor)
{
	IsMove = true;
	
	UEngineRandom Random;
	Random.SetSeed(time(NULL));
	int ResultX = Random.RandomInt(-50, 50);
	int ResultY = Random.RandomInt(50, 100);

	FVector2D StartPos = _Actor->GetActorLocation();
	TargetPos = FVector2D(StartPos.X + ResultX, StartPos.Y - ResultY);
	FVector2D Dir = TargetPos - StartPos;
	Dir.Normalize();

	Force = Dir * 200.0f;
}

ADecalObject::~ADecalObject()
{
}
