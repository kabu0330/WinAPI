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
	DamagedEffectRenderer->CreateAnimation("DamagedEffect", "effect_bloodpoof.png", 0, 10);
	DamagedEffectRenderer->SetComponentScale({ 64, 64 });
	DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
	DamagedEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	DamagedEffectRenderer->SetActive(false);

	SpawnEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpawnEffectRenderer->CreateAnimation("SpawnEffect", "SpawnEffect_Large.png", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f }, false);
	SpawnEffectRenderer->SetComponentLocation({ 0, -40 });
	SpawnEffectScale = { 128, 128 };
	SpawnEffectRenderer->SetComponentScale(SpawnEffectScale);
	SpawnEffectRenderer->ChangeAnimation("SpawnEffect");
	SpawnEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);

	BloodEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BloodEffectRenderer->CreateAnimation("DeathEffect", "LargeBloodExplosion.png", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 30.0f }, false);
	BloodEffectRenderer->SetComponentLocation(BloodEffectLocation);
	BloodEffectRenderer->SetComponentScale(BloodEffectScale);
	BloodEffectRenderer->SetOrder(ERenderOrder::MonsterDeathDebris);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");
	BloodEffectRenderer->SetActive(false);

	DebugOn();
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	CollisionFuctionSetting();

	AActor* MainPawn = GetWorld()->GetPawn();
	Player = dynamic_cast<APlayer*>(MainPawn);

	SpawnEffectRenderer->SetActive(false);
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
	Super::Tick(_DeltaTime);
	MonsterInputDebug();
	BodyCollisionCooldownElapsed += _DeltaTime;

	if (true == APlayGameMode::IsGamePaused()) // 게임이 일시정지라면 모두 정지
	{
		return;
	}

	SpawnAnimation(); // 최초 1회만 재생

	if (ParentRoom != ARoom::GetCurRoom()) // 너가 생성된 맵 바깥으로 나갈 수 없어.
	{
		return;
	}
	ClampPositionToRoom();
	
	KnockbackTick();
	Move(_DeltaTime);
	ChasePlayer(_DeltaTime);
	HandleCollisionDamage();

	CurStateAnimation(_DeltaTime);

	DeathCheck(_DeltaTime);
}

// 플레이어 쫓아가
void AMonster::ChaseMove(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}

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
	if (true == IsHit)
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected) // 플레이어를 추적하지 않는 중이면
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // 일정 시간 이동하면 리턴
	{
		if (MoveElapsedTime > MoveCooldown) // 멈춘 뒤 일정 시간이 흐르면 다시 이동
		{
			Direction = GetRandomDir(); // 플레이어가 탐색 범위에 없다면 랜덤 이동
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
	if (nullptr == BodyRenderer)
	{
		return;
	}

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

// 몬스터의 이동 방향은 8방향
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

	static UEngineRandom MonsterRandomDir;
	MonsterRandomDir.SetSeed(time(nullptr));
	int Result = MonsterRandomDir.RandomInt(0, 7);

	if (PrevDir == Result) // 이전에 이동한 방향과 같으면 다시 이동할 방향 랜덤 돌려
	{
		UEngineRandom Reroll;
		Reroll.SetSeed(time(nullptr) + Result + 1); // Result가 0일수도 있으니까 1 더해서 반드시 시드값을 바꾼다.
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

// 플레이어를 기준으로 몬스터의 어느 방향에 있느냐를 정규화
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

// 너가 속한 맵 밖으로 나가지마.
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
		SetActorLocation(Pos + FVector2D{ 1, 0 });
	}
	if (RightEdge < OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -1, 0 });
	}
	if (TopEdge > OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 1 });
	}
	if (BotEdge < OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -1 });
	}
}

// 플레이어가 탐색 범위 내로 들어왔냐?
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

// 플레이어와 충돌 시 플레이어에게 데미지를 주고 피격 애니메이션을 동작시킨다.
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

	// Debug 출력 메시지
	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "에게 " + std::to_string(CollisionAtt) + " 의 충돌 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionPlayer->GetHp()));
}

void AMonster::SpawnAnimation()
{
	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}

	// 플레이어와 같은 방에 있으면
	if (nullptr != SpawnEffectRenderer)
	{
		SpawnEffectRenderer->SetActive(true);
		SpawnFadeOut();
		TimeEventer.PushEvent(0.3f, std::bind(&AMonster::BodyRender, this));
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
	if (false == SpawnEffectRenderer->IsCurAnimationEnd())
	{
		return;
	}
	SpawnEffectRenderer->Destroy();
	SpawnEffectRenderer = nullptr;
}

void AMonster::Death(float _DeltaTime)
{
	// 1. 충돌체(바디 + 탐색) 끄고
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
		TimeEventer.PushEvent(15.0f, std::bind(&AMonster::FadeOut, this));
	}

	// 3. 액터 지우고
	if (nullptr == BodyRenderer)
	{
		Destroy();
		return;
	}

	// Death 애니메이션이 따로 있으면 재정의
	// 2. Death 애니메이션 재생하고
	// BodyRenderer->ChangeAnimation("Death")
	// 
	// 3. Body렌더 끄고
	// RendererDestroy();

	BloodEffectRenderer->SetActive(true);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");

	//FadeOut();

	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;

		return;
	}

	BodyRenderer->SetActive(false);
}

int AMonster::ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir)
{
	{
		AMonster* Monster = dynamic_cast<AMonster*>(_Monster);
		if (nullptr == Monster)
		{
			return 0;
		}
		else if (true == Monster->IsInvincible()) // 무적이면 리턴
		{
			return Hp;
		}

		if (true == Monster->IsDeath())
		{
			return 0;
		}

		Monster->DamagedEffectRenderer->SetActive(true);
		Monster->DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
		TimeEventer.PushEvent(1.0f, std::bind(&AMonster::SwitchDamagedEffectRenderer, this));
		BeginBlinkEffect();

		IsHit = true;
		KnockbackDistance = _Dir * 0.5;
		
		TimeEventer.PushEvent(KnockbackDuration, std::bind(&AMonster::SwitchIsHit, this));
		
		Hp -= _PlayerAtt;
		if (Hp < 0)
		{
			Hp = 0;
		}
		return Hp;
	}
}

void AMonster::KnockbackTick()
{
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
	if (true == IsDeath()) // 죽었으면 리턴
	{
		return;
	}

	BodyRenderer->SetAlphaFloat(0.0f);
	DamagedEffectRenderer->SetAlphaFloat(1.0f);
	TimeEventer.PushEvent(0.1f, std::bind(&AMonster::StayBlinkEffect, this));
}

void AMonster::StayBlinkEffect()
{
	if (true == IsDeath()) // 죽었으면 리턴
	{
		BodyRenderer->SetAlphaFloat(0.0f);
		return;
	}

	BodyRenderer->SetAlphaFloat(1.0f);

	++FadeCount;
	if (3 < FadeCount)
	{
		FadeCount = 0;
		return;
	}
	TimeEventer.PushEvent(0.1f, std::bind(&AMonster::BeginBlinkEffect, this));
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
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	FadeValue += DeltaTime * 0.4f * FadeDir;
	SpawnEffectRenderer->SetAlphaFloat(FadeValue);
}

void AMonster::SpawnFadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventer.PushEvent(0.1f, std::bind(&AMonster::FadeChange, this), true, false);
}

// 디버깅용 치트키
void AMonster::MonsterInputDebug()
{
	if (UEngineInput::GetInst().IsDown('F'))
	{
		Hp = 0;
	}
}

void AMonster::CollisionFuctionSetting()
{
	BodyCollision->SetCollisionEnter(std::bind(&AMonster::HandleCollisionDamage, this));
}

void AMonster::ChangeAnimIdle()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Idle");
}

// 방향이 있는 몬스터만 재정의해서 사용, Tick을 몬스터에서 돌리기 위해서 일단 만들어둔다.
void AMonster::CurStateAnimation(float _DeltaTime)
{
}

AMonster::~AMonster()
{
}
