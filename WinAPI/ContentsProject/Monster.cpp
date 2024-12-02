#include "PreCompile.h"
#include "Monster.h"

#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"
#include "DecalObject.h"

AMonster::AMonster()
{
	DamagedEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DamagedEffectRenderer->CreateAnimation("DamagedEffect", "effect_bloodpoof.png", 0, 11, 0.07f, false);
	DamagedEffectRenderer->CreateAnimation("DamagedEffect_End", "effect_bloodpoof.png", 11, 11, 0.1f, false);
	DamagedEffectRenderer->SetComponentScale({ 64, 64 });
	DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
	DamagedEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	DamagedEffectRenderer->SetActive(false);

	SpawnEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpawnEffectRenderer->CreateAnimation("SpawnEffect", "SpawnEffect_Large.png", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, { 0.2f, 0.1f, 0.12f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.04f, 0.03f, 0.02f, 0.02f }, false);

	SpawnEffectRenderer->SetComponentLocation({ 0, -40 });
	SpawnEffectScale = { 128, 128 };
	SpawnEffectRenderer->SetComponentScale({ 128, 128 });
	SpawnEffectRenderer->ChangeAnimation("SpawnEffect");
	SpawnEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	SpawnEffectRenderer->SetActive(false);

	BloodEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BloodEffectRenderer->CreateAnimation("DeathEffect", "LargeBloodExplosion.png", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.05f }, false);
	BloodEffectRenderer->SetComponentLocation({0, -30});
	BloodEffectRenderer->SetComponentScale({256, 256});
	BloodEffectRenderer->SetOrder(ERenderOrder::Decal);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");
	BloodEffectRenderer->SetActive(false);


	RandomSeed = static_cast<__int64>(time(nullptr)) ^ reinterpret_cast<__int64>(this);
	(MonsterRandom);
	MonsterRandom.SetSeed(RandomSeed);

	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	CollisionFuctionSetting();

	AActor* MainPawn = GetWorld()->GetPawn();
	Player = dynamic_cast<APlayer*>(MainPawn);


	BodyRenderer->SetActive(false);
	BodyCollision->SetActive(false);

	if (nullptr == DetectCollision)
	{
		return;
	}
	DetectCollision->SetActive(false);
}

void AMonster::Tick(float _DeltaTime)
{
	ARoom* PlayerCurRoom = ARoom::GetCurRoom();
	if (PlayerCurRoom != ParentRoom) // �÷��̾�� ���� ������ ���ٸ� ����
	{
		return;
	}

	if (true == APlayGameMode::IsGamePaused()) // ������ �Ͻ�������� ��� ����
	{
		return;
	}

	Super::Tick(_DeltaTime);
	MonsterInputDebug();

	BodyCollisionCooldownElapsed += _DeltaTime;

	ReverseForce(_DeltaTime);

	if (FVector2D::ZERO != Force)
	{
		AddActorLocation(Force * _DeltaTime);

	}

	SpawnAnimation(); // ���� 1ȸ�� ���

	if (ParentRoom != ARoom::GetCurRoom()) // �ʰ� ������ �� �ٱ����� ���� �� ����.
	{
		return;
	}
	ClampPositionToRoom();
	
	DeathCheck(_DeltaTime);

	KnockbackTick();
	Move(_DeltaTime);
	ChasePlayer(_DeltaTime);
	HandleCollisionDamage();

	CurStateAnimation(_DeltaTime);

}

void AMonster::ReverseForce(float _DeltaTime)
{
	if (FVector2D::ZERO == Force)
	{
		return;
	}

	FVector2D Reverse = -Force;
	Reverse.Normalize();

	Force += Reverse * _DeltaTime * 100.0f;

	if (50.0f >= Force.Length())
	{
		Force = FVector2D::ZERO;
	}
}

// �÷��̾� �Ѿư�
void AMonster::ChaseMove(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}

	Force = FVector2D::ZERO;
	Direction = GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMonster::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected)
	{
		return;
	}

	ChangeAggressiveAnimation();

	Attack(_DeltaTime);
	ChaseMove(_DeltaTime);
}

void AMonster::Move(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected) // �÷��̾ �������� �ʴ� ���̸�
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // ���� �ð� �̵��ϸ� ����
	{
		if (MoveElapsedTime > MoveCooldown) // ���� �� ���� �ð��� �帣�� �ٽ� �̵�
		{
			Direction = GetRandomDir(); // �÷��̾ Ž�� ������ ���ٸ� ���� �̵�
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMonster::Attack(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}

	if (nullptr == BodyRenderer)
	{
		return;
	}

	CooldownElapsed += _DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}

	IsAttack = true;
	TimeEventer.PushEvent(0.5f, std::bind(&AMonster::SwitchIsAttacking, this));

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CooldownElapsed = 0.0f;
}

// ������ �̵� ������ 8����
FVector2D AMonster::GetRandomDir()
{
	FVector2D LeftTop = FVector2D::LEFT + FVector2D::UP;
	LeftTop.Normalize();

	FVector2D LeftBot = FVector2D::LEFT + FVector2D::DOWN;
	LeftBot.Normalize();

	FVector2D RightTop = FVector2D::RIGHT + FVector2D::UP;
	RightTop.Normalize();

	FVector2D RightBot = FVector2D::RIGHT + FVector2D::DOWN;
	RightBot.Normalize();

	//static UEngineRandom MonsterRandomDir;
	//MonsterRandomDir.SetSeed(time(nullptr));
	int Result = MonsterRandom.RandomInt(0, 7);

	if (-1 != PrevDir)
	{
		if (PrevDir == Result) // ������ �̵��� ����� ������ �ٽ� �̵��� ���� ���� ����
		{
			static UEngineRandom Reroll;
			Reroll.SetSeed(time(nullptr) + Result + 1); // Result�� 0�ϼ��� �����ϱ� 1 ���ؼ� �ݵ�� �õ尪�� �ٲ۴�.
			int RerollResult = Reroll.RandomInt(0, 7);
			Result = RerollResult;
		}
	}

	FVector2D Dir = FVector2D::ZERO;
	switch (Result)
	{
	case 0:
		Dir = FVector2D::LEFT;
		State = MonsterState::LEFT;
		PrevDir = 0;
		break;
	case 1:
		Dir = FVector2D::RIGHT;
		State = MonsterState::RIGHT;
		PrevDir = 1;
		break;
	case 2:
		Dir = FVector2D::UP;
		State = MonsterState::UP;
		PrevDir = 2;
		break;
	case 3:
		Dir = FVector2D::DOWN;
		State = MonsterState::DOWN;
		PrevDir = 3;
		break;
	case 4:
		State = MonsterState::LEFT;
		Dir = LeftTop;
		PrevDir = 4;
		break;
	case 5:
		Dir = LeftBot;
		State = MonsterState::LEFT;
		PrevDir = 5;
		break;
	case 6:
		Dir = RightTop;
		State = MonsterState::RIGHT;
		PrevDir = 6;
		break;
	case 7:
		Dir = RightBot;
		State = MonsterState::RIGHT;
		PrevDir = 7;
		break;
	default:
		break;
	}

	return FVector2D(Dir);
}

// �÷��̾ �������� ������ ��� ���⿡ �ִ��ĸ� ����ȭ
FVector2D AMonster::GetDirectionToPlayer()
{
	AActor* Player = GetWorld()->GetPawn();
	FVector2D PlayerPos = Player->GetActorLocation();
	FVector2D MonsterPos = this->GetActorLocation();
	FVector2D Distance = PlayerPos - MonsterPos;

	Distance.Normalize();

	if (Distance.X <= 0)
	{
		State = MonsterState::LEFT;
	}
	else if (Distance.X > 0)
	{
		State = MonsterState::RIGHT;
	}

	return Distance;
}

// �ʰ� ���� �� ������ ��������.
void AMonster::ClampPositionToRoom()
{
	if (nullptr == BodyCollision)
	{
		return;
	}

	FVector2D Pos = GetActorLocation();
	FVector2D HalfScale = BodyCollision->GetComponentScale().Half();
	FVector2D OffsetPos = { Pos + BodyCollision->GetComponentLocation() + HalfScale } ;

	FVector2D FootLeftPos = { OffsetPos.X - HalfScale.X, OffsetPos.Y};
	FVector2D FootRightPos = { OffsetPos.X + HalfScale.X, OffsetPos.Y};
	FVector2D FootTopPos = { OffsetPos.X , OffsetPos.Y - HalfScale.Y};
	FVector2D FootBotPos = { OffsetPos.X , OffsetPos.Y + HalfScale.Y};

	ARoom* CurRoom = ParentRoom;
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 1.95f;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 1.8f;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	FVector2D ClampedPos = FVector2D(FVector2D::Clamp(OffsetPos.X, LeftEdge, RightEdge), FVector2D::Clamp(OffsetPos.Y, TopEdge, BotEdge));

	if (LeftEdge > FootLeftPos.X)
	{
		SetActorLocation(ClampedPos - (OffsetPos - Pos));
	}
	if (RightEdge < FootRightPos.X)
	{
		SetActorLocation(ClampedPos - (OffsetPos - Pos));
	}
	if (TopEdge > OffsetPos.Y)
	{
		SetActorLocation(ClampedPos - (OffsetPos - Pos));
	}
	if (BotEdge < FootBotPos.Y)
	{
		SetActorLocation(ClampedPos - (OffsetPos - Pos));
	}
}

// �÷��̾ Ž�� ���� ���� ���Գ�?
bool AMonster::IsPlayerNearby()
{
	if (nullptr == DetectCollision)
	{
		return false;
	}
	AActor* Player = GetWorld()->GetPawn();
	FVector2D PlayerPos = Player->GetActorLocation();
	FVector2D MonsterPos = this->GetActorLocation();
	FVector2D Distance = PlayerPos - MonsterPos;

	float Length = Distance.Length();
	float DetectLength = DetectCollision->GetComponentScale().Half().X;

	if (DetectLength >= Length)
	{
		return true;
	}
	return false;
}

// �÷��̾�� �浹 �� �÷��̾�� �������� �ְ� �ǰ� �ִϸ��̼��� ���۽�Ų��.
void AMonster::HandleCollisionDamage()
{
	if (nullptr == BodyCollision)
	{
		return;
	}

	AActor* CollisionActor = BodyCollision->CollisionOnce(ECollisionGroup::Player_Body);

	if (nullptr == CollisionActor)
	{
		return;
	}

	if (BodyCollisionCooldownElapsed < BodyCollisionCooldown)
	{
		return;
	}

	APlayer* CollisionPlayer = dynamic_cast<APlayer*>(CollisionActor);
	CollisionPlayer->ApplyDamaged(CollisionActor, CollisionAtt, Direction);

	BodyCollisionCooldownElapsed = 0.0f;

	// Debug ��� �޽���
	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "���� " + std::to_string(CollisionAtt) + " �� �浹 �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionPlayer->GetHp()));
}

void AMonster::SpawnAnimation()
{
	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}
	if (true == SpawEvent)
	{
		return;
	}

	// �÷��̾�� ���� �濡 ������
	if (nullptr != SpawnEffectRenderer)
	{
		SpawEvent = true; // FadeOut Trigger
		SpawnEffectRenderer->SetActive(true);
		SpawnFadeOut();
		TimeEventer.PushEvent(0.4f, std::bind(&AMonster::BodyRender, this));		
	}
	else
	{
		TimeEventer.PushEvent(0.4f, std::bind(&AMonster::BodyRender, this));
	}

}

void AMonster::BodyRender()
{
	if (nullptr != BodyRenderer)
	{
		BodyRenderer->SetActive(true);
	}

	if (nullptr != BodyCollision)
	{
		BodyCollision->SetActive(true);
	}

	if (nullptr != DetectCollision)
	{
		DetectCollision->SetActive(true);
	}

	if (nullptr == SpawnEffectRenderer)
	{
		return;
	}

	Sound = UEngineSound::Play("summon.wav");
	Sound.SetVolume(0.5f);

	TimeEventer.PushEvent(1.0f, [this]() {
		SpawEvent = false;
		if (nullptr == SpawnEffectRenderer)
		{
			return;
		}
		SpawnEffectRenderer->Destroy();
		SpawnEffectRenderer = nullptr; 
		 });

}

void AMonster::Death(float _DeltaTime)
{
	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		Sound = UEngineSound::Play("death_burst_small.wav");
		Sound = UEngineSound::Play("meat_impact.wav");

		BloodEffectRenderer->SetActive(true);
		CreateGib();
		CollisionDestroy();
		DeathSound();
	}

	// 3. ���� �����
	if (nullptr == BodyRenderer)
	{
		ParentRoom->RemoveMonster(this);
		Destroy();
		return;
	}

	// Death �ִϸ��̼��� ���� ������ ������
	// RendererDestroy();

	SetMoveSpeed(0);

	BloodEffectRenderer->ChangeAnimation("DeathEffect");


	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;
		BloodEffectRenderer->Destroy(0.1f);
		return;
	}

	BodyRenderer->SetActive(false);
}

void AMonster::CreateGib()
{
	int Index = MonsterRandom.RandomInt(0, 13);

	ARoomObject* Gib = ParentRoom->CreateObject<ADecalObject>(this);
	ADecalObject* Decal = dynamic_cast<ADecalObject*>(Gib);
	if (nullptr != Decal)
	{
		Decal->SetMove(this);
	}

	USpriteRenderer* GibRenderer = Gib->GetBodyRenderer();
	GibRenderer->CreateAnimation("Gib", "effect_030_bloodgibs.png", Index, Index, 0.1f, false);
	GibRenderer->SetComponentScale({ 64, 64 });
	GibRenderer->SetActive(true);
	GibRenderer->SetOrder(ERenderOrder::MonsterDeathDebris);
	GibRenderer->ChangeAnimation("Gib");

	int BloodPoolIndex = MonsterRandom.RandomInt(0, 23);
	ARoomObject* BloodPool = ParentRoom->CreateObject<ADecalObject>(this);
	ADecalObject* BloodPoolDecal = dynamic_cast<ADecalObject*>(BloodPool);
	if (nullptr == BloodPoolDecal)
	{
		return;
	}

	USpriteRenderer* BloodPoolRenderer = BloodPool->GetBodyRenderer();
	BloodPoolRenderer->CreateAnimation("BloodPool", "effect_bloodpool.png", BloodPoolIndex, BloodPoolIndex, 0.1f, false);
	BloodPoolRenderer->SetComponentScale({ 256, 256 });
	BloodPoolRenderer->SetActive(true);
	BloodPoolRenderer->SetAlphaFloat(0.5f);
	BloodPoolRenderer->SetOrder(ERenderOrder::Decal);
	BloodPoolRenderer->ChangeAnimation("BloodPool");
}

int AMonster::ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir)
{
	AMonster* Monster = dynamic_cast<AMonster*>(_Monster);
	if (nullptr == Monster)
	{
		return 0;
	}
	else if (true == Monster->IsInvincible()) // �����̸� ����
	{
		return Hp;
	}

	if (true == Monster->IsDeath())
	{
		return 0;
	}

	DamagedEffectRenderer->SetActive(true);
	DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
	BeginBlinkEffect();

	IsHit = true;

	KnockbackPower = 0.5f;
	KnockbackDistance = _Dir * KnockbackPower;
	
	TimeEventer.PushEvent(KnockbackDuration, std::bind(&AMonster::SwitchIsHit, this));
	
	Hp -= _PlayerAtt;
	if (Hp < 0)
	{
		Hp = 0;
	}
	return Hp;
	
}

void AMonster::KnockbackTick()
{
	if (false == CanKnockback)
	{
		return;
	}
	if (false == IsHit)
	{
		return;
	}
	LerpAlpha = KnockbackDuration / 1.0f;
	FVector2D StartPos = FVector2D::ZERO;
	FVector2D Result = FVector2D::Lerp(StartPos, KnockbackDistance, LerpAlpha);
	SetActorLocation(GetActorLocation() + Result);
}

void AMonster::BeginBlinkEffect()
{
	if (true == IsDeath()) // �׾����� ����
	{
		return;
	}

	BodyRenderer->SetAlphaFloat(0.0f);
	DamagedEffectRenderer->SetAlphaFloat(1.0f);
	TimeEventer.PushEvent(0.1f, std::bind(&AMonster::StayBlinkEffect, this));
	TimeEventer.PushEvent(0.5f,std::bind_front(&AMonster::OffDamagedEffect, this));
}

void AMonster::StayBlinkEffect()
{
	if (true == IsDeath()) // �׾����� ����
	{
		BodyRenderer->SetAlphaFloat(0.0f);
		return;
	}

	BodyRenderer->SetAlphaFloat(1.0f);

	++FadeCount;
	if (1 < FadeCount)
	{
		FadeCount = 0;
		return;
	}
	TimeEventer.PushEvent(0.1f, std::bind(&AMonster::BeginBlinkEffect, this));
}

void AMonster::AreaWideAttack(AActor* _Actor)
{
	ARoom* Room = CastActorToType<ARoom>(_Actor);
	if (nullptr != Room)
	{
		return;
	}

	FVector2D Dir = _Actor->GetActorLocation() - GetActorLocation();
	Dir.Normalize(); // ���⺤��

	APlayer* Player = CastActorToType<APlayer>(_Actor);
	if (nullptr != Player)
	{
		Player->ApplyDamaged(Player, 2, Dir); // ��ź�� �÷��̾�� ��Ʈ ��ĭ�� ���ظ� �ش�.
	}

	AMonster* Monster = CastActorToType<AMonster>(_Actor);
	if (nullptr != Monster)
	{
		Monster->ApplyDamaged(Monster, Att, Dir); // ��ź�� ���Ϳ��� ���ظ� ������.
	}

	ARoomObject* Object = CastActorToType<ARoomObject>(_Actor);
	if (nullptr != Object)
	{
		Object->ApplyDamaged(this);
	}
}

void AMonster::FadeChange()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.2f * FadeDir;
	BloodEffectRenderer->SetAlphaFloat(FadeValue);
}

void AMonster::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	TimeEventer.PushEvent(0.5f, std::bind(&AMonster::FadeChange, this), true, false);
}

void AMonster::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(5.0f, std::bind(&AMonster::FadeChange, this), true, false);
}

void AMonster::SpawnFadeChange()
{
	if (nullptr == SpawnEffectRenderer)
	{
		return;
	}
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.4f * FadeDir;
	SpawnEffectRenderer->SetAlphaFloat(FadeValue);
}

void AMonster::SpawnFadeOut()
{
	if (nullptr == SpawnEffectRenderer)
	{
		return;
	}
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(2.0f, std::bind(&AMonster::SpawnFadeChange, this), true, false);
}

// ������ ġƮŰ
void AMonster::MonsterInputDebug()
{
	if (UEngineInput::GetInst().IsDown('G'))
	{
		if (ParentRoom == ARoom::GetCurRoom())
		{
			Hp = 0;
		}
		
	}
}

void AMonster::CollisionFuctionSetting()
{
	BodyCollision->SetCollisionEnter(std::bind(&AMonster::HandleCollisionDamage, this));

	if (nullptr != UniversalCollision)
	{
		UniversalCollision->SetCollisionEnter(std::bind(&AMonster::AreaWideAttack, this, std::placeholders::_1));
	}
}

void AMonster::ChangeAnimIdle()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Idle");
}

// ������ �ִ� ���͸� �������ؼ� ���, Tick�� ���Ϳ��� ������ ���ؼ� �ϴ� �����д�.
void AMonster::CurStateAnimation(float _DeltaTime)
{
}

AMonster::~AMonster()
{
}
