#include "PreCompile.h"
#include "NumberOne.h"

ANumberOne::ANumberOne()
{
	SetName("NumberOne");
	BodyRendererScale = { 32, 32 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	IsMove = true;
	ItemType = EItemType::PASSIVE; // 눈물을 변화시키냐?

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->SetSprite("NumberOne.png");
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

ANumberOne::~ANumberOne()
{
}

void ANumberOne::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();

	SpriteSetting();
}

void ANumberOne::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

void ANumberOne::SpriteSetting()
{
	std::string SpriteName = "NumberOne_Head.png";
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
	HeadRenderer->SetComponentScale({ 63, 63 });
	HeadRenderer->ChangeAnimation("Head_Down");


	HeadRenderer->SetOrder(ERenderOrder::PlayerHead);
	HeadRenderer->SetActive(false);
}

bool ANumberOne::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 1)
	{
		return false; // 이미 동일 아이템이 있다.
	}

	SetActorLocation({ _Player->GetActorLocation().iX(), _Player->GetActorLocation().iY() - 70 });

	IsMove = false;
	IsPushback = true;
	IsPickedUp = true;
	DropSucessAnimation(_Player);

	// 아이템 효과 설정
	Player->ChangeHeadRenderer(HeadRenderer); // 얼굴 변경

	Player->InitTear();
	Player->AddTearCooldown(0.15f);
	Player->AddTearDuration(-0.3f);

	return true;
}

void ANumberOne::DropSucessAnimation(APlayer* _Player)
{
	if (false == IsPickedUp)
	{
		return;
	}
	if (nullptr == DropRenderer)
	{
		return;
	}

	Sound = UEngineSound::Play("power_up1.wav");

	IsDropEffect = true;
	Player->ChangePlayerAnimation(DropAnimationDuration, "Drop");

	DropEffectRenderer->SetComponentScale({ 160, 160 });

	TimeEventer.PushEvent(DropAnimationDuration, [this]() {
		DropRenderer->SetActive(false);
		DropEffectRenderer->SetActive(false);
		IsOwnedByPlayer = true;
		});
}

void ANumberOne::TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed)
{
	APlayer* Player = _Player;
	int Att = Player->GetAtt();
	float TearSpeed = Player->GetTearSpeed();
	float TearDuration = Player->GetTearDuration();
	FVector2D TearPos = _TearPos + FVector2D(0.0f, 25.0f);
	FVector2D TearScale = Player->GetTearScale();

	ATear* ItemTear = GetWorld()->SpawnActor<ATear>();
	USpriteRenderer* TearEffectRenderer = ItemTear->GetTearRenderer();
	TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoof_Yellow.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoof_Yellow.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 92, 92 });
	TearEffectRenderer->SetOrder(ERenderOrder::Tear);
	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");

	ItemTear->Fire(Player, this, TearPos, _TearDir, Att, _PlayerSpeed, TearSpeed, TearDuration, TearScale);
}
