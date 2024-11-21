#include "PreCompile.h"
#include "Polyphemus.h"

APolyphemus::APolyphemus()
{
	SetName("Polyphemus");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	IsMove = true;

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
	std::string SpriteName = "costume_106_polyphemus.png";
	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left"        , SpriteName, 6, 6, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right"       , SpriteName, 2, 2, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down"        , SpriteName, 0, 0, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up"          , SpriteName, 4, 4, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left" , SpriteName, { 6, 7, 6 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Right", SpriteName, { 2, 3, 2 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Down" , SpriteName, { 0, 1, 0 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Up"   , SpriteName, { 4, 5, 4 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Death"       , "Death_Head.png", 0, 0, 0.5f);

	HeadRenderer->SetComponentLocation({ 0, Global::PlayerHeadOffset.iY() + 29});
	HeadRenderer->SetComponentScale({ 120, 120});
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

	SetActorLocation(_Player->GetActorLocation());	

	FVector2D Dir = { _Player->GetActorLocation().X, abs(_Player->GetActorLocation().Y) };
	Dir.Normalize(); // 방향벡터

	Force = Dir * HoverForce;

	IsMove = false;
	IsPushback = true;
	IsDrop = true;
	DropSucessAnimation(_Player);

	// 아이템 효과 설정
	Player->ChangeHeadRenderer(HeadRenderer); // 얼굴 변경



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

	IsDropEffect = true;
	Player->ChangePlayerAnimation(DropAnimationDuration, "Drop");

	DropEffectRenderer->SetComponentScale({ 160, 160 });

	TimeEventer.PushEvent(DropAnimationDuration / 2.0f, [this]() { Force = Gravity; });
	TimeEventer.PushEvent(DropAnimationDuration, [this]() {
		DropRenderer->SetActive(false); 
		DropEffectRenderer->SetActive(false); 
		IsOwnedByPlayer = true;
		});
}




