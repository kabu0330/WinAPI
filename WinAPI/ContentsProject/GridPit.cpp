#include "PreCompile.h"
#include "GridPit.h"

AGridPit::AGridPit()
{
	SetName("GridPit");
	BodyRendererScale = { 64, 64 };
	FVector2D BlockingPathCollisionPivot = { 0, 0 };

	CanExplode = false; // Bomb과 상호 작용 가능 조건
	IsTearDamageable = false;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("GRIDPIT_LEFT" , "grid_pit.png", 17, 17);
	BodyRenderer->CreateAnimation("GRIDPIT_MID"  , "grid_pit.png", 16, 16);
	BodyRenderer->CreateAnimation("GRIDPIT_RIGHT", "grid_pit.png", 18, 18);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Front);
	BodyRenderer->ChangeAnimation("GRIDPIT_MID");
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyRendererScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	BlockingPathCollision->SetComponentScale(BodyRendererScale);
	BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	BlockingPathCollision->SetCollisionType(ECollisionType::Rect);
}

AGridPit::~AGridPit()
{
}

void AGridPit::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void AGridPit::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
}

