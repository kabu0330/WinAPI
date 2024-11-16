#include "PreCompile.h"
#include "Monster.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"
#include "DeathDebris.h"

AMonster::AMonster()
{
	DamagedEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DamagedEffectRenderer->CreateAnimation("DamagedEffect", "effect_bloodpoof.png", 0, 10, 0.1f, false);
	DamagedEffectRenderer->SetComponentScale({ 64, 64 });
	DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
	DamagedEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	DamagedEffectRenderer->SetActive(false);

	SpawnEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpawnEffectRenderer->CreateAnimation("DeathEffect", "LargeBloodExplosion.png", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {0.1f, 0.1f, 0.1f, 0.1f,0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 5.0f}, false);
	SpawnEffectRenderer->SetComponentLocation({ 0, -40 });
	SpawnEffectScale = { 256, 256 };
	SpawnEffectRenderer->SetComponentScale(SpawnEffectScale);
	SpawnEffectRenderer->ChangeAnimation("DeathEffect");
	SpawnEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	SpawnEffectRenderer->SetActive(false);

	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	AActor* MainPawn = GetWorld()->GetPawn();
	Player = dynamic_cast<APlayer*>(MainPawn);
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	MonsterInputDebug();

	if (ParentRoom != ARoom::GetCurRoom()) // �ʰ� ������ �� �ٱ����� ���� �� ����.
	{
		return;
	}

	if (true == APlayGameMode::IsGamePaused()) // ������ �Ͻ�������� ��� ����
	{
		return;
	}

	ClampPositionToRoom();

	CurStateAnimation(_DeltaTime);

	Move(_DeltaTime);

	ChasePlayer(_DeltaTime);
	HandleCollisionDamage(_DeltaTime);

	DeathCheck(_DeltaTime);
}

// ������ ġƮŰ
void AMonster::MonsterInputDebug()
{
	if (UEngineInput::GetInst().IsDown('F'))
	{
		Hp = 0;
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

void AMonster::Death(float _DeltaTime)
{
	if (nullptr != BodyCollision)
	{
		BodyRenderer->SetActive(false);
		CollisionDestroy();
		RendererDestroy();
	}

	// �ִϸ��̼� �ٲܰŸ� �Լ� ������
	DeathDebris* BloodEffect = GetWorld()->SpawnActor<DeathDebris>();

	if (false == BloodEffect->IsCurAnimationEnd())
	{
		return;
	}
	Destroy();
}

// �÷��̾� �Ѿư�
void AMonster::ChaseMove(float _DeltaTime)
{
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

	Attack(_DeltaTime);
	ChaseMove(_DeltaTime);
}

void AMonster::Move(float _DeltaTime)
{
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
	CoolDownElapsed += _DeltaTime;
	if (CoolDownElapsed < Cooldown)
	{
		return;
	}

	IsAttack = true;
	TimeEventer.PushEvent(0.5f, std::bind(&AMonster::SwitchIsAttacking, this));

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CoolDownElapsed = 0.0f;
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

	static UEngineRandom Random;
	Random.SetSeed(time(nullptr));
	int Result = Random.RandomInt(0, 7);

	if (PrevDir == Result) // ������ �̵��� ����� ������ �ٽ� �̵��� ���� ���� ����
	{
		UEngineRandom Reroll;
		Reroll.SetSeed(time(nullptr) + Result + 1); // Result�� 0�ϼ��� �����ϱ� 1 ���ؼ� �ݵ�� �õ尪�� �ٲ۴�.
		int RerollResult = Reroll.RandomInt(0, 7);
		Result = RerollResult;
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
	FVector2D OffsetPos = Pos + BodyCollision->GetComponentLocation();

	ARoom* CurRoom = ParentRoom;
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	if (LeftEdge > OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ 2, 0 });
	}
	if (RightEdge < OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -2, 0 });
	}
	if (TopEdge > OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 2 });
	}
	if (BotEdge < OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -2 });
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
void AMonster::HandleCollisionDamage(float _DeltaTime)
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

	BodyCollisionCooldownElapsed += _DeltaTime;
	if (BodyCollisionCooldownElapsed < BodyCollisionCooldown)
	{
		return;
	}

	APlayer* CollisionPlayer = dynamic_cast<APlayer*>(CollisionActor);
	CollisionPlayer->ApplyDamaged(CollisionActor, CollisionAtt);
	CollisionPlayer->ShowHitAnimation(CollisionPlayer);

	BodyCollisionCooldownElapsed = 0.0f;

	// Debug ��� �޽���
	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "���� " + std::to_string(CollisionAtt) + " �� �浹 �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionPlayer->GetHp()));
}

// ������ �ִ� ���͸� �������ؼ� ���, Tick�� ���Ϳ��� ������ ���ؼ� �ϴ� �����д�.
void AMonster::CurStateAnimation(float _DeltaTime)
{
}

AMonster::~AMonster()
{
}
