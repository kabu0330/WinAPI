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
#include "BossHpBar.h"
#include "BossIntroScene.h"

ATheDukeOfFlies::ATheDukeOfFlies()
{
	/* 이름     : */ SetName("TheDukeOfFlies");
	/* 체력     : */ SetHp(110);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(40);
	/* 이동시간 : */ SetMoveDuration(1.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 300 , 300 });
	/* 발사속도 : */ SetShootingSpeed(300.0f);
	/* 쿨타임   : */ SetCooldown(5.0f);
	CanKnockback = false;
	float SkillprepareTime0 = 1.0f;
	float SkillprepareTime1 = 1.0f;
	SkillCastDelay = SkillprepareTime0 + SkillprepareTime1;

	SkillPostActionTime = 1.5f;

	BodyScale = { 100 , 100 };

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "duke.png", 2, 2, 0.1f, false);
	BodyRenderer->CreateAnimation("Attack", "duke.png", {1, 3, 0}, { SkillprepareTime0, SkillprepareTime1, SkillPostActionTime}, false);
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

	// 스킬을 쓰지 않아도 쿨타임을 먼저 적용시켜버림
	CooldownElapsed = 3.0f;
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
	DisplayBossIntro();
	DisplayBossHpBar();
	
	SkillCooldown(_DeltaTime);

	SummonFlies();
	BlowAway();
	SummonBigFlies();
}

void ATheDukeOfFlies::DisplayBossIntro()
{
	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}
	// 플레이어가 보스 방에 들어오면
	APlayGameMode::SetGamePaused(true);
	ABossIntroScene* BossIntro = GetWorld()->SpawnActor<ABossIntroScene>();
	BossIntro->ShowScene();
	
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


void ATheDukeOfFlies::SummonFlies()
{
	if (true == IsAttacking)
	{
		return;
	}
	if (CooldownElapsed < SummonCooldownDuration)
	{
		return;
	}
	if (true == HasMaxFlies()) // 최대 소환 마릿수 넘어갔으면 리턴
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Attack");
	ModifySkillCooldownElapsed();

	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonFliesLogic, this));
	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonFliesAnimaition, this));

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

	TimeEventer.PushEvent(SkillPostActionTime, [this]() {
		IsAttacking = false; 
		CooldownElapsed = 0.0f; });
}

void ATheDukeOfFlies::BeginSummonFliesAnimaition()
{
	BlackDustEffectRenderer->SetActive(true);	
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust");
	TimeEventer.PushEvent(SkillPostActionTime, std::bind(&ATheDukeOfFlies::EndSummonFliesAnimaition, this));
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
	if (BlowAwayCooldownElapesd < BlowAwayCooldownDuration)
	{
		return false;
	}

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
		if (true == Fly->GetIsFollowBoss())
		{
			++Followers;
		}	
	}

	// 스킬 발동조건
	BlowAwayTriggerValue = 3; // 4마리
	if (BlowAwayTriggerValue <= Followers)
	{
		// 나를 따라 주변을 멤도는 파리가 4마리 이상이라면 BlowAway 스킬을 써!!!
		return true; 
	}
	return false;
}

void ATheDukeOfFlies::BlowAway()
{
	if (true == IsAttacking)
	{
		return;
	}

	// 나를 따르는 파리가 4마리 미만이면 스킬 쓰지마.
	if (false == CanBlowAway())
	{
		return;
	}
	
	BodyRenderer->ChangeAnimation("Attack");
	ModifySkillCooldownElapsed();


	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginBlowAwayLogic, this));
	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginBlowAwayAnimaition, this));

}

void ATheDukeOfFlies::BeginBlowAwayLogic()
{
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

		Fly->GetForce() = Dir * 250.0f;
		
	}
	
	TimeEventer.PushEvent(SkillPostActionTime, [this]() {
		IsAttacking = false;
		BlowAwayCooldownElapesd = 0.0f; });
}

void ATheDukeOfFlies::BeginBlowAwayAnimaition()
{
	DustEffectRenderer->SetActive(true);
	DustEffectRenderer->ChangeAnimation("Dust");

	TimeEventer.PushEvent(SkillPostActionTime, std::bind(&ATheDukeOfFlies::EndBlowAwayAnimaition, this));
}

void ATheDukeOfFlies::EndBlowAwayAnimaition()
{
	BodyRenderer->ChangeAnimation("Idle");

	DustEffectRenderer->SetActive(false);
	DustEffectRenderer->ChangeAnimation("Dust_End");

}

bool ATheDukeOfFlies::CanSummonBigFlies()
{
	int Count = 0;

	std::list<AMonster*>::iterator StartIter = Flies.begin();
	std::list<AMonster*>::iterator EndIter = Flies.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		AAttackFly* Fly = dynamic_cast<AAttackFly*>(*StartIter);
		if (nullptr == Fly)
		{
			continue;
		}
		if (false == Fly->GetIsFollowBoss())
		{
			continue;		
		}

		++Count; // 나를 따르는 부하들이 몇마리나 있느냐
	}

	// 스킬 발동 조건
	int TriggerNumber = 1; 
	if (TriggerNumber >= Count)
	{
		return true;
	}
	return false;
}

void ATheDukeOfFlies::SummonBigFlies()
{
	if (true == IsAttacking)
	{
		return;
	}
	if (SummonBigFlyCooldownElapsed < SummonBigFlyCooldownDuration)
	{
		return;
	}
	if (false == CanSummonBigFlies()) // 나를 따르는 파리가 몇마리야?
	{
		return;
	}
	// 나를 따르는 파리가 없어? 그러면 빅파리 소환해.

	BodyRenderer->ChangeAnimation("Attack");
	ModifySkillCooldownElapsed();

	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonBigFliesLogic, this));
	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonFliesAnimaition, this)); // 애니메이션은 재탕
}

void ATheDukeOfFlies::BeginSummonBigFliesLogic()
{
	ARoom* ParentRoom = ARoom::GetCurRoom();
	if (nullptr == ParentRoom)
	{
		return;
	}

	// 스폰 위치
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);

	float OrbitRadius = 150.0f;
	float Angles[2] = { // 스폰 위치를 각도로 지정
	static_cast<float>(40.0f * std::numbers::pi / 180.0f),
	static_cast<float>(140.0f * std::numbers::pi / 180.0f)
	};

	AAttackFly* ChildFly[2] = { nullptr, };
	for (int i = 0; i < 2; i++)
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
		ChildFly[i]->SetHp(10);
		ChildFly[i]->SetScale(1.2f);

		Flies.push_back(ChildFly[i]);
	}

	TimeEventer.PushEvent(SkillPostActionTime, [this]() {
		IsAttacking = false; 
		SummonBigFlyCooldownElapsed = 0.0f; });
}


void ATheDukeOfFlies::SkillCooldown(float _DeltaTime)
{
	if (true == IsAttacking)
	{
		return;
	}
	CooldownElapsed += _DeltaTime;
	BlowAwayCooldownElapesd += _DeltaTime;
	SummonBigFlyCooldownElapsed += _DeltaTime;
}

void ATheDukeOfFlies::ModifySkillCooldownElapsed()
{
	IsAttacking = true;
	/*if (SkillCastDelay < CooldownElapsed)
	{
		CooldownElapsed -= SkillCastDelay;
	}
	if (SkillCastDelay < BlowAwayCooldownElapesd)
	{
		BlowAwayCooldownElapesd -= SkillCastDelay;
	}
	if (SkillCastDelay < SummonBigFlyCooldownElapsed)
	{
		SummonBigFlyCooldownElapsed -= SkillCastDelay;
	}
	*/
}

ATheDukeOfFlies::~ATheDukeOfFlies()
{
}


