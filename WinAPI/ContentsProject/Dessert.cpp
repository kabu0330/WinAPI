#include "PreCompile.h"
#include "Dessert.h"

#include "Player.h"

ADessert::ADessert()
{
	SetName("Dessert");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 2;
	IsMove = true;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->CreateAnimation("Drop", "collectibles_024_dessert.png", 0, 0, 0.3f);
	DropRenderer->SetComponentLocation({ 0, 0 });
	DropRenderer->SetComponentScale(BodyRendererScale);
	DropRenderer->SetOrder(ERenderOrder::Item);
	DropRenderer->ChangeAnimation("Drop");
	DropRenderer->SetActive(true);


	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Rect);
}

ADessert::~ADessert()
{
}

void ADessert::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
}

void ADessert::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

bool ADessert::EatFunction(APlayer* _Player)
{
	int MaxHp = APlayer::GetTotalHpMax();
	int CurMaxHp = _Player->GetHptMax();
	int CurHp = _Player->GetHp();
	if (CurMaxHp == MaxHp)
	{
		if (CurHp != MaxHp)
		{
			Player->AddHp(ItemCount); 
		}
		// 더이상 증가할 필요가 없어도 먹고 끝낸다
	}

	SetActorLocation(_Player->GetActorLocation());

	FVector2D Dir = { _Player->GetActorLocation().X, abs(_Player->GetActorLocation().Y) };
	Dir.Normalize(); // 방향벡터

	Force = Dir * HoverForce;

	IsMove = false;
	IsPushback = false;
	IsDrop = true;
	DropSucessAnimation(_Player);

	// 아이템 효과 설정
	Player->AddHpMax(ItemCount);
	Player->AddHp(ItemCount);

	return true;	
}

void ADessert::DropSucessAnimation(APlayer* _Player)
{
	if (false == IsDrop)
	{
		return;
	}
	if (nullptr == DropRenderer)
	{
		return;
	}

	Player->ChangePlayerAnimation(DropAnimationDuration, "Drop");

	DropEffectRenderer->SetComponentScale({ 160, 160 });

	TimeEventer.PushEvent(DropAnimationDuration / 2.0f, [this]() { Force = Gravity; });
	TimeEventer.PushEvent(DropAnimationDuration, [this]() {
		DropEffectRenderer->SetActive(false);
		IsOwnedByPlayer = false;
		IsUseEnd = true;
		});
}


