#include "PreCompile.h"
#include "TheDukeOfFlies.h"
#include <cmath>

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

#include "Room.h"
#include "AttackFly.h"
#include "PlayGameMode.h"
#include"BossHpBar.h"

ATheDukeOfFlies::ATheDukeOfFlies()
{
	/* 이름     : */ SetName("TheDukeOfFlies");
	/* 체력     : */ SetHp(3);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(0);
	/* 이동시간 : */ SetMoveDuration(1.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 300 , 300 });
	/* 발사속도 : */ SetShootingSpeed(300.0f);
	/* 쿨타임   : */ SetCooldown(5.0f);
	CanKnockback = false;

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 110, 110 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "duke.png", 2, 2, 0.1f, false);
	BodyRenderer->CreateAnimation("Attack", "duke.png", {1, 3, 0}, {1.0f, 1.5f, 2.0f}, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 160, 128 }); // 80, 64
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	BlackDustEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BlackDustEffectRenderer->CreateAnimation("Black_Dust", "effect_black_dust.png", 0, 10, 0.1f, false);
	BlackDustEffectRenderer->CreateAnimation("Black_Dust_End", "effect_black_dust.png", 10, 10, 0.1f, false);
	BlackDustEffectRenderer->SetComponentLocation({ 0,20 });
	BlackDustEffectRenderer->SetComponentScale({ 290, 160 }); // 145, 80
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust");
	BlackDustEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	BlackDustEffectRenderer->SetAlphaFloat(0.7f);
	BlackDustEffectRenderer->SetActive(false);

	DustEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DustEffectRenderer->CreateAnimation("Dust", "effect_dust.png", 0, 10, 0.1f, false);
	DustEffectRenderer->CreateAnimation("Dust_End", "effect_dust.png", 10, 10, 0.1f, false);
	DustEffectRenderer->SetComponentLocation({ 0, 20 });
	DustEffectRenderer->SetComponentScale({ 319, 176 });
	DustEffectRenderer->ChangeAnimation("Dust");
	DustEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	DustEffectRenderer->SetAlphaFloat(0.7f);
	DustEffectRenderer->SetActive(false);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Circle);
	DetectCollision->SetActive(true);

	// 맵에 존재할 수 있는 파리 숫자는 최대 12마리임, 11마리까지는 파리 소환 패턴을 쓸 수 있음
	MaxFlyCount = 12; // 원작은 12마리
}

void ATheDukeOfFlies::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

	BossHpBor = GetWorld()->SpawnActor<ABossHpBar>();
	BossHpBor->SetHpBar(this);

	DamagedEffectRenderer->SetComponentLocation({ 0, -30 });
	DamagedEffectRenderer->SetComponentScale({ 128, 128 });

	SpawnEffectRenderer->SetComponentLocation({ 0, -100 });
	SpawnEffectRenderer->SetComponentScale({ 384, 384});

	BloodEffectRenderer->SetComponentLocation({ 0, -60 });
	BloodEffectRenderer->SetComponentScale({ 512, 512 });

	BlowAwayCooldownElapesd = 1.0f; // 스킬을 쓰지 않아도 쿨타임을 먼저 적용시켜버림
}

void ATheDukeOfFlies::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);


	ARoom* PlayerCurRoom = ARoom::GetCurRoom();
	if (PlayerCurRoom != ParentRoom)
	{
		return;
	}

	if (true == APlayGameMode::IsGamePaused()) // 게임이 일시정지라면 모두 정지
	{
		return;
	}

	if (true == IsDeath())
	{
		BossHpBor->SetDisplay(false);
		return;
	}
	
	SummonFlies();
	BlowAway();
	DisplayBossHpBar();
}

void ATheDukeOfFlies::Death(float _DeltaTime)
{
	// 1. 충돌체(바디 + 탐색) 끄고
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
	}

	// 3. 액터 지우고
	if (nullptr == BodyRenderer)
	{
		ParentRoom->RemoveMonster(this);
		Destroy();
		return;
	}

	RemoveFlies();
	SetMoveSpeed(0);
	BloodEffectRenderer->SetActive(true);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");

	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;

		return;
	}

	BodyRenderer->SetActive(false);
}

void ATheDukeOfFlies::RemoveFlies()
{
	std::list<AMonster*>::iterator StartIter = Flies.begin();
	std::list<AMonster*>::iterator EndIter = Flies.end();

	for (; StartIter != EndIter; )
	{
		AAttackFly* Fly = dynamic_cast<AAttackFly*>(*StartIter);
		if (nullptr == Fly)
		{
			++StartIter;
			continue;
		}
		Fly->SetIsFollowBoss(nullptr);
		StartIter = Flies.erase(StartIter);
	}
	Flies.clear();
}

void ATheDukeOfFlies::DisplayBossHpBar()
{
	if (true == IsDeath())
	{
		return;
	}
	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}
	BossHpBor->SetDisplay(true);
}

void ATheDukeOfFlies::SummonFlies()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();

	CooldownElapsed += DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}
	if (true == HasMaxFlies()) // 최대 소환 마릿수 넘어갔으면 리턴
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Attack");
	CooldownElapsed = 0.0f;

	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginSummonFliesLogic, this));
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginSummonFliesAnimaition, this));

}

void ATheDukeOfFlies::BeginSummonFliesLogic()
{
	ARoom* ParentRoom = ARoom::GetCurRoom();
	if (nullptr == ParentRoom)
	{
		return;
	}

	// 스폰 위치
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);

	float OrbitRadius = 150.0f;
	float Angles[3] = { // 스폰 위치를 각도로 지정
	static_cast<float>(50.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(90.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(130.0f * std::numbers::pi / 180.0f) 
	};

	AAttackFly* ChildFly[3] = { nullptr, };
	for (int i = 0; i < 3; i++)
	{
		// 부하의 상대 위치 계산
		FVector2D Offset;
		Offset.X = OrbitRadius * cos(Angles[i]);
		Offset.Y = OrbitRadius * sin(Angles[i]);

		// 부하의 최종 위치: SetFliesPos를 기준으로 Offset 추가
		FVector2D SpawnPosition = SetFliesPos + Offset;

		// 부하 생성
		ChildFly[i] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SpawnPosition));
		if (ChildFly[i] == nullptr)
		{
			MSGASSERT("AttackFly 생성 실패");
			return;
		}

		// 부하가 보스를 따라다니도록 설정
		ChildFly[i]->SetIsFollowBoss(this);
		ChildFly[i]->SetInitialAngle(Angles[i]);

		Flies.push_back(ChildFly[i]);
	}

}

void ATheDukeOfFlies::BeginSummonFliesAnimaition()
{
	BlackDustEffectRenderer->SetActive(true);	
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust");
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::EndSummonFliesAnimaition, this));
}

void ATheDukeOfFlies::EndSummonFliesAnimaition()
{
	BodyRenderer->ChangeAnimation("Idle");
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust_End");
	BlackDustEffectRenderer->SetActive(false);
}

bool ATheDukeOfFlies::HasMaxFlies()
{
	int CurFlies = ParentRoom->CountFly();
	if (MaxFlyCount <= CurFlies)
	{
		return true; // 스폰 금지
	}

	return false;
}

bool ATheDukeOfFlies::CanBlowAway()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();

	BlowAwayCooldownElapesd += DeltaTime;
	if (BlowAwayCooldownElapesd < BlowAwayCooldownDuration)
	{
		return false;
	}

	return true;

	std::list<AMonster*>::iterator StartIter = Flies.begin();
	std::list<AMonster*>::iterator EndIter = Flies.end();
	int Followers = 0;

	for (; StartIter != EndIter; ++StartIter)
	{
		AMonster* Monster = *StartIter;
		AAttackFly* Fly = dynamic_cast<AAttackFly*>(Monster);
		if (nullptr == Fly)
		{
			continue;
		}
		++Followers;
		int a = 0;
	}
	BlowAwayTriggerValue = 1;
	if (BlowAwayTriggerValue < Followers)
	{
		int a = 0;
		// 나를 따라 주변을 멤도는 파리가 4마리 이상이라면 BlowAway 스킬을 써!!!
		return true; 
	}
	return false;
}

void ATheDukeOfFlies::BlowAway()
{
	// 나를 따르는 파리가 4마리 미만이면 스킬 쓰지마.
	if (false == CanBlowAway())
	{
		return;
	}
	
	BodyRenderer->ChangeAnimation("Attack");
	BlowAwayCooldownElapesd = 0.0f;

	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginBlowAwayLogic, this));
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginBlowAwayAnimaition, this));

}

// 미완
void ATheDukeOfFlies::BeginBlowAwayLogic()
{
	FVector2D CenterPos = this->GetActorLocation() - ParentRoom->GetActorLocation();
	float InitialSpeed = 500.0f;
	float Duration = 0.3f;
	float DecelerationFactor = 0.95f;  // 감속 계수


	std::list<AMonster*>::iterator StartIter = Flies.begin();
	std::list<AMonster*>::iterator EndIter = Flies.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		AMonster* Monster = *StartIter;
		AAttackFly* Fly = dynamic_cast<AAttackFly*>(Monster);
		if (nullptr == Fly)
		{
			continue;
		}

		FVector2D Dir = Fly->GetActorLocation() - GetActorLocation();
		Dir.Normalize();

		Fly->GetForce() = Dir * 200.0f;
		
	}
}

void ATheDukeOfFlies::BeginBlowAwayAnimaition()
{
	DustEffectRenderer->SetActive(true);
	DustEffectRenderer->ChangeAnimation("Dust");

	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::EndBlowAwayAnimaition, this));
}

void ATheDukeOfFlies::EndBlowAwayAnimaition()
{
	BodyRenderer->ChangeAnimation("Idle");

	DustEffectRenderer->SetActive(false);
	DustEffectRenderer->ChangeAnimation("Dust_End");
}

ATheDukeOfFlies::~ATheDukeOfFlies()
{
}


