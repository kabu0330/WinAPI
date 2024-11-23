#include "PreCompile.h"
#include "SpoonBender.h"

ASpoonBender::ASpoonBender()
{
	SetName("SpoonBender");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	IsMove = true;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->SetSprite("collectibles_003_spoonbender.png");
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

ASpoonBender::~ASpoonBender()
{
}

void ASpoonBender::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();

	SpriteSetting();
}

void ASpoonBender::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

void ASpoonBender::SpriteSetting()
{
	std::string SpriteName = "SpoonBender.png";
	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", SpriteName, 6, 6, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Right", SpriteName, 2, 2, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Down", SpriteName, 0, 0, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Up", SpriteName, 4, 4, 0.5f, false);
	HeadRenderer->CreateAnimation("Head_Attack_Left", SpriteName, { 6, 7, 6 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Right", SpriteName, { 2, 3, 2 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Down", SpriteName, { 0, 1, 0 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Attack_Up", SpriteName, { 4, 5, 4 }, 0.2f);
	HeadRenderer->CreateAnimation("Head_Death", "Death_Head.png", 0, 0, 0.5f);

	HeadRenderer->SetComponentLocation({ 0, Global::PlayerHeadOffset.iY() + 7 });
	HeadRenderer->SetComponentScale({ 64, 64 });
	HeadRenderer->ChangeAnimation("Head_Down");


	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);
	HeadRenderer->SetActive(false);
}

bool ASpoonBender::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 1)
	{
		return false; // 이미 동일 아이템이 있다.
	}

	SetActorLocation({ _Player->GetActorLocation().iX(), _Player->GetActorLocation().iY() - 70 });

	IsMove = false;
	IsPushback = true;
	IsDrop = true;
	DropSucessAnimation(_Player);

	// 아이템 효과 설정
	Player->ChangeHeadRenderer(HeadRenderer); // 얼굴 변경

	Player->InitTear();
	Player->SetDetectCollision();


	return true;
}

void ASpoonBender::DropSucessAnimation(APlayer* _Player)
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

	TimeEventer.PushEvent(DropAnimationDuration, [this]() {
		DropRenderer->SetActive(false);
		DropEffectRenderer->SetActive(false);
		IsOwnedByPlayer = true;
		});
}
