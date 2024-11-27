#include "PreCompile.h"
#include "Penny.h"
#include "Room.h"

APenny::APenny()
{
	SetName("Penny");
	BodyRendererScale = { 128, 128 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	ItemType = EItemType::USE;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->CreateAnimation("Appearance", "penny.png", 7, 13, 0.2f, false);
	DropRenderer->CreateAnimation("Drop", "penny.png", 0, 5, 0.3f);
	DropRenderer->CreateAnimation("DropEffect", "penny.png", 14, 23, 0.1f, false);
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

APenny::~APenny()
{
}

void APenny::BeginPlay()
{
    Super::BeginPlay();
    AItem::BeginPlay();
	TimeEventer.PushEvent(1.0f, [this]() {DropRenderer->ChangeAnimation("Drop"); });
}

void APenny::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
    AItem::Tick(_DeltaTime);
}

bool APenny::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 99)
	{
		return false; // ¸ø¸ÔÀ¸¸é Æ¨°Ü³½´Ù.
	}

	DropSucessAnimation();

	return true;
}

void APenny::UseItem(APlayer* _Player)
{
}

void APenny::DropSucessAnimation()
{
	if (nullptr == DropRenderer)
	{
		return;
	}

	Sound = UEngineSound::Play("dime_pick_up.wav");

	DropRenderer->ChangeAnimation("DropEffect");
}
