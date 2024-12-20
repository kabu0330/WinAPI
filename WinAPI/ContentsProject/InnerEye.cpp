#include "PreCompile.h"
#include "InnerEye.h"

AInnerEye::AInnerEye()
{
	SetName("InnerEye");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	IsMove = true;
	ItemType = EItemType::PASSIVE;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->SetSprite("collectibles_002_theinnereye.png");
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

AInnerEye::~AInnerEye()
{
}

void AInnerEye::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();

	SpriteSetting();
}

void AInnerEye::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

void AInnerEye::SpriteSetting()
{
	std::string SpriteName = "TheInnerEye.png";
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

bool AInnerEye::EatFunction(APlayer* _Player)
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
	//Player->AddTearSpeed(200.0f);


	return true;
}

void AInnerEye::DropSucessAnimation(APlayer* _Player)
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

void AInnerEye::TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed)
{
	APlayer* Player = _Player;
	int Att = Player->GetAtt();
	float TearSpeed = Player->GetTearSpeed();
	float TearDuration = Player->GetTearDuration();
	FVector2D TearScale = Player->GetTearScale() + FVector2D(-12, -12);
	FVector2D TearPos = _Player->GetActorLocation() + FVector2D(0, -25);

	ATear* Tear = GetWorld()->SpawnActor<ATear>();
	Tear->Fire(Player, nullptr, TearPos, _TearDir, Att, _PlayerSpeed, TearSpeed, TearDuration, TearScale);

	// 두번째 발사
	FVector2D SecondTearPos = FVector2D::ZERO;
	if (FVector2D::LEFT == _TearDir || FVector2D::RIGHT == _TearDir)
	{
		SecondTearPos = TearPos + FVector2D(0, -15);
	}
	else
	{
		SecondTearPos = TearPos + FVector2D(15, 0);
	}

	ATear* SecondTear = GetWorld()->SpawnActor<ATear>();
	SecondTear->Fire(Player, nullptr, SecondTearPos, _TearDir, Att, _PlayerSpeed, TearSpeed, TearDuration, TearScale);

	// 세번째 발사
	FVector2D ThirdTearPos = FVector2D::ZERO;
	if (FVector2D::LEFT == _TearDir || FVector2D::RIGHT == _TearDir)
	{
		ThirdTearPos = TearPos + FVector2D(0, 15);
	}
	else
	{
		ThirdTearPos = TearPos + FVector2D(-15, 0);
	}

	ATear* ThirdTear = GetWorld()->SpawnActor<ATear>();
	ThirdTear->Fire(Player, nullptr, ThirdTearPos, _TearDir, Att, _PlayerSpeed, TearSpeed, TearDuration, TearScale);
}
