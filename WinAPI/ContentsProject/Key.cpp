#include "PreCompile.h"
#include "Key.h"

AKey::AKey()
{
	SetName("Key");
	BodyRendererScale = { 128, 128 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	ItemType = EItemType::USE;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->CreateAnimation("Appearance", "pickup_003_key.png", 0, 0, 0.2f, false);	
	DropRenderer->CreateAnimation("DropEffect", "Drop.png", 0, 3, 0.1f, false);
	DropRenderer->SetComponentLocation({ 0, 0 });
	DropRenderer->SetComponentScale(BodyRendererScale);
	DropRenderer->SetOrder(ERenderOrder::ItemEffect);
	DropRenderer->ChangeAnimation("Appearance");
	DropRenderer->SetActive(true);

	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Rect);
}

AKey::~AKey()
{
}

void AKey::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
}

void AKey::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

bool AKey::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 99)
	{
		return false; // ¸ø¸ÔÀ¸¸é Æ¨°Ü³½´Ù.
	}

	DropSucessAnimation();
	IsDrop = true;

	return true;
}

void AKey::UseItem(APlayer* _Player)
{
}

void AKey::DropSucessAnimation()
{
	if (nullptr == DropRenderer)
	{
		return;
	}
	DropRenderer->ChangeAnimation("DropEffect");
	TimeEventer.PushEvent(0.4f, [this]() {DropRenderer->SetActive(false); });
}
