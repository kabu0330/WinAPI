#include "PreCompile.h"
#include "MetalBlock.h"

AMetalBlock::AMetalBlock()
{
	SetName("MetalBlock");
	BodyRendererScale = { 64, 64 };
	FVector2D BodyCollisionScale = { 50, 50 };
	FVector2D BlockingPathCollisionScale = { 50 , 50 };
	FVector2D BlockingPathCollisionPivot = { 0, 0 };

	CanExplode = false; // Bomb과 상호 작용 가능 조건
	IsTearDamageable = false;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("METALBLOCKS", "rocks.png", 4, 4);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_PlayerBack);
	BodyRenderer->ChangeAnimation("METALBLOCKS");
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	BlockingPathCollision->SetComponentScale(BlockingPathCollisionScale);
	BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	BlockingPathCollision->SetCollisionType(ECollisionType::Rect);
}

AMetalBlock::~AMetalBlock()
{
}

void AMetalBlock::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void AMetalBlock::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
}

