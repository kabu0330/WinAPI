#include "PreCompile.h"
#include "Polyphemus.h"

APolyphemus::APolyphemus()
{
	SetName("Polyphemus");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	IsDropEffect = true;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->SetSprite("Polyphemus.png");
	DropRenderer->SetComponentLocation({ 0, 0 });
	DropRenderer->SetComponentScale(BodyRendererScale);
	DropRenderer->SetOrder(ERenderOrder::ItemEffect);
	DropRenderer->SetActive(true);

	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Circle);
}

APolyphemus::~APolyphemus()
{
}

void APolyphemus::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();

	SpriteSetting();
}

void APolyphemus::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

void APolyphemus::SpriteSetting()
{
	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left"        , "costume_106_polyphemus.png", 6, 6, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right"       , "costume_106_polyphemus.png", 2, 2, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down"        , "costume_106_polyphemus.png", 0, 0, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up"          , "costume_106_polyphemus.png", 4, 4, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left" , "costume_106_polyphemus.png", { 6, 7, 6 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Right", "costume_106_polyphemus.png", { 2, 3, 2 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Down" , "costume_106_polyphemus.png", { 0, 1, 0 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Attack_Up"   , "costume_106_polyphemus.png", { 4, 5, 4 }, 0.12f);
	HeadRenderer->CreateAnimation("Head_Death"       , "Death_Head.png", 0, 0, 0.5f);

	HeadRenderer->SetComponentLocation({ 0, Global::PlayerHeadOffset.iY() + 4});
	HeadRenderer->SetComponentScale({ 64, 64 });
	HeadRenderer->ChangeAnimation("Head_Down");


	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);
	HeadRenderer->SetActive(false);
}

bool APolyphemus::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 1)
	{
		return false; // 이미 동일 아이템이 있다.
	}
	IsDrop = true;
	DropSucessAnimation(_Player);

	return true;
}

void APolyphemus::DropSucessAnimation(APlayer* _Player)
{
	if (false == IsDrop)
	{
		return;
	}
	if (nullptr == DropRenderer)
	{
		return;
	}
	
	SetActorLocation(_Player->GetActorLocation());

	FVector2D Dir = { _Player->GetActorLocation().X, _Player->GetActorLocation().Y};
	Dir.Normalize(); // 방향벡터

	Force = Dir * FVector2D(0.0f, -350.0f);
	
	_Player->ChangePlayerAnimation(2.0f, "Drop");
	DropEffectRenderer->SetComponentScale({ 160, 160 });
	TimeEventer.PushEvent(2.0f, [this]() { Force = FVector2D(0, 1) * 120.0f; });
	TimeEventer.PushEvent(3.0f, [this]() { 	
		DropRenderer->SetActive(false); 
		DropEffectRenderer->SetActive(false); 
		IsOwnedByPlayer = true;
		});
}


