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
#include "DecalObject.h"

ATheDukeOfFlies::ATheDukeOfFlies()
{
	/* �̸�     : */ SetName("TheDukeOfFlies");
	/* ü��     : */ SetHp(200);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(50);
	/* �̵��ð� : */ SetMoveDuration(1.0f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 200 , 200 });
	/* �߻�ӵ� : */ SetShootingSpeed(300.0f);
	CanKnockback = false;
	float SkillprepareTime0 = 1.0f;
	float SkillprepareTime1 = 1.0f;
	SkillCastDelay = SkillprepareTime0 + SkillprepareTime1;

	SkillPostActionTime = 1.5f;

	BodyCollisionScale = { 100 , 100 };
	BodyRendererScale = { 160, 128 };

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "duke.png", 2, 2, 0.1f, false);
	BodyRenderer->CreateAnimation("Attack", "duke.png", {1, 3, 0}, { SkillprepareTime0, SkillprepareTime1, SkillPostActionTime}, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale); // 80, 64
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

	for (int i = 0; i < 4; i++)
	{
		DamagedEffectRenderers[i] = CreateDefaultSubObject<USpriteRenderer>();
		DamagedEffectRenderers[i]->CreateAnimation("DamagedEffect", "effect_bloodpoof.png", 0, 11, 0.07f, false);
		DamagedEffectRenderers[i]->SetComponentScale({ 64, 64 });
		DamagedEffectRenderers[i]->ChangeAnimation("DamagedEffect");
		DamagedEffectRenderers[i]->SetOrder(ERenderOrder::MonsterEffect);
		DamagedEffectRenderers[i]->SetActive(false);
	}
	DamagedEffectRenderers[0]->SetComponentLocation({-20, -30});
	DamagedEffectRenderers[1]->SetComponentLocation({ 20, -30});
	DamagedEffectRenderers[2]->SetComponentLocation({-20,  30});
	DamagedEffectRenderers[3]->SetComponentLocation({ 20,  30});

	// �ʿ� ������ �� �ִ� �ĸ� ���ڴ� �ִ� 12������, 11���������� �ĸ� ��ȯ ������ �� �� ����
	MaxFlyCount = 15; // ������ 12����
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

	// ��ų�� ���� �ʾƵ� ��Ÿ���� ���� ������ѹ���
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

	if (true == APlayGameMode::IsGamePaused()) // ������ �Ͻ�������� ��� ����
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
	if (true == ReadyToBossIntro)
	{
		return;
	}
	// �÷��̾ ���� �濡 ������
	ReadyToBossIntro = true;

	APlayGameMode::SetGamePaused(true);

	ABossIntroScene* BossIntro = GetWorld()->SpawnActor<ABossIntroScene>();
	BossIntro->ShowScene();

	BossIntro->Destroy(BossIntro->GetPlayTime() + 1.5f);
}

void ATheDukeOfFlies::SpawnAnimation()
{
	if (ParentRoom != ARoom::GetCurRoom())
	{
		return;
	}
	if (false == ReadyToBossIntro)
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
	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();

		UEngineSound::AllSoundStop();
		DeathSound = UEngineSound::Play("boss_fight jingle_outro_v2_12.ogg");
		TimeEventer.PushEvent(15.0f, []() {
			APlayGameMode::GetPlayGameModeBGM() = UEngineSound::Play("diptera_sonata_basement.ogg"); });
		
		for (int i = 0; i < 4; i++)
		{
			TimeEventer.PushEvent(1.0f + ( - 0.2f * i), [this, i]() {
				DamagedEffectRenderers[i]->SetActive(true); });
		}

		TimeEventer.PushEvent(4.0f, [this]() {
			BloodEffectRenderer->SetActive(true);
			BloodEffectRenderer->ChangeAnimation("DeathEffect"); });
	}

	// 3. ���� �����
	if (nullptr == BodyRenderer)
	{
		ParentRoom->RemoveMonster(this);
		Destroy(7.5f);
		return;
	}

	RemoveFlies();
	SetMoveSpeed(0);
	SummonCooldownDuration = 0.0f;
	BlowAwayCooldownDuration = 0.0f;
	SummonBigFlyCooldownDuration = 0.0f;

	BodyRenderer->ChangeAnimation("Idle");
	Oscillation(_DeltaTime);

	if (nullptr == BloodEffectRenderer)
	{
		return;
	}

	if (false == BloodEffectRenderer->IsActive())
	{
		return;
	}
	if (true == BloodEffectRenderer->IsActive())
	{
		BodyRenderer->SetActive(false);
	}

	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		Sound = UEngineSound::Play("death_burst_large_0.wav");
		CreateGib();
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;
		BloodEffectRenderer->Destroy(0.1f);
		BloodEffectRenderer = nullptr;
		return;
	}

}

void ATheDukeOfFlies::CreateGib()
{
	int Index = MonsterRandom.RandomInt(0, 13);

	const int Count = 8;
	ARoomObject* Gib[Count];
	ADecalObject* Decal[Count];
	USpriteRenderer* GibRenderer[Count];

	for (int i = 0; i < Count; i++)
	{
		Gib[i] = ParentRoom->CreateObject<ADecalObject>(this);
		Decal[i] = dynamic_cast<ADecalObject*>(Gib[i]);

		GibRenderer[i] = Gib[i]->GetBodyRenderer();
		//GibRenderer[i]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 0, 0, 0.1f, false);
		GibRenderer[i]->SetComponentScale({ 64, 64 });
		GibRenderer[i]->SetActive(true);
		GibRenderer[i]->SetOrder(ERenderOrder::MonsterDeathDebris);
	}

	Decal[0]->SetMove(this, FVector2D(0, 0));
	Decal[1]->SetMove(this, FVector2D(-20, -30));
	Decal[2]->SetMove(this, FVector2D(-60, -60));
	Decal[3]->SetMove(this, FVector2D(30, 30));
	Decal[4]->SetMove(this, FVector2D(80, 60));
	Decal[5]->SetMove(this, FVector2D(0, -50));
	Decal[6]->SetMove(this, FVector2D(-20, 0));
	Decal[7]->SetMove(this, FVector2D(0, -20));

	GibRenderer[0]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 0, 0, 0.1f, false);
	GibRenderer[1]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 13, 13, 0.1f, false);
	GibRenderer[2]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 2, 2, 0.1f, false);
	GibRenderer[3]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 6, 6, 0.1f, false);
	GibRenderer[4]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 10, 10, 0.1f, false);
	GibRenderer[5]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 12, 12, 0.1f, false);
	GibRenderer[6]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 7, 7, 0.1f, false);
	GibRenderer[7]->CreateAnimation("Gib", "effect_030_bloodgibs.png", 9, 9, 0.1f, false);

	for (int i = 0; i < Count; i++)
	{
		GibRenderer[i]->ChangeAnimation("Gib");
	}


	ARoomObject* BloodPool = ParentRoom->CreateObject<ADecalObject>(this);
	ADecalObject* BloodPoolDecal = dynamic_cast<ADecalObject*>(BloodPool);
	if (nullptr == BloodPoolDecal)
	{
		return;
	}

	USpriteRenderer* BloodPoolRenderer = BloodPool->GetBodyRenderer();
	BloodPoolRenderer->CreateAnimation("BloodPool", "effect_bloodpool.png", 21, 21, 0.1f, false);
	BloodPoolRenderer->SetComponentScale({ 384, 384 });
	BloodPoolRenderer->SetActive(true);
	BloodPoolRenderer->SetAlphaFloat(0.5f);
	BloodPoolRenderer->SetOrder(ERenderOrder::Decal);
	BloodPoolRenderer->ChangeAnimation("BloodPool");
}

void ATheDukeOfFlies::Oscillation(float _DeltaTime)
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	// ���� �ӵ��� ���� ����
	const float OscillationSpeed = 1000.0f; // ���� �ӵ�
	const float OscillationMagnitude = 10.0f; // ���� ũ��

	// ���� �Լ��� �̿��� ���� ���
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// ��ġ ������Ʈ
	FVector2D NewScale = { BodyRendererScale.X + Oscillation, BodyRendererScale.Y + Oscillation };
	BodyRenderer->SetComponentScale(NewScale);
}

FVector2D ATheDukeOfFlies::GetRandomDir()
{
	FVector2D LeftTop = FVector2D::LEFT + FVector2D::UP;
	LeftTop.Normalize();

	FVector2D LeftBot = FVector2D::LEFT + FVector2D::DOWN;
	LeftBot.Normalize();

	FVector2D RightTop = FVector2D::RIGHT + FVector2D::UP;
	RightTop.Normalize();

	FVector2D RightBot = FVector2D::RIGHT + FVector2D::DOWN;
	RightBot.Normalize();

	int Result = MonsterRandom.RandomInt(4, 7);

	if (-1 != PrevDir)
	{
		if (PrevDir == Result) // ������ �̵��� ����� ������ �ٽ� �̵��� ���� ���� ����
		{
			static UEngineRandom Reroll;
			Reroll.SetSeed(time(nullptr) + Result + 1); // Result�� 0�ϼ��� �����ϱ� 1 ���ؼ� �ݵ�� �õ尪�� �ٲ۴�.
			int RerollResult = Reroll.RandomInt(4, 7);
			Result = RerollResult;
		}
	}

	FVector2D Dir = FVector2D::ZERO;
	switch (Result)
	{
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

void ATheDukeOfFlies::ClampPositionToRoom()
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
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 1.8f;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 1.7f;

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

void ATheDukeOfFlies::BeginBlinkEffect()
{
	if (true == IsDeath()) // �׾����� ����
	{
		return;
	}

	//BodyRenderer->SetAlphaFloat(0.0f);
	//DamagedEffectRenderer->SetAlphaFloat(1.0f);
	//TimeEventer.PushEvent(0.1f, std::bind(&AMonster::StayBlinkEffect, this));
	TimeEventer.PushEvent(0.5f, std::bind_front(&AMonster::OffDamagedEffect, this));
}

int ATheDukeOfFlies::ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir)
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

	TimeEventer.PushEvent(KnockbackDuration, std::bind(&AMonster::SwitchIsHit, this));

	Hp -= _PlayerAtt;
	if (Hp < 0)
	{
		Hp = 0;
	}
	return Hp;
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
	if (true == HasMaxFlies()) // �ִ� ��ȯ ������ �Ѿ���� ����
	{
		return;
	}
	if (true == IsDeath())
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
	if (true == IsDeath())
	{
		return;
	}

	// ���� ��ġ
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);

	float OrbitRadius = 150.0f;
	const int Count = 3;
	float Angles[Count] = { // ���� ��ġ�� ������ ����
	static_cast<float>(50.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(90.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(130.0f * std::numbers::pi / 180.0f), 
	//static_cast<float>(170.0f * std::numbers::pi / 180.0f) 
	};

	AAttackFly* ChildFly[Count] = { nullptr, };
	for (int i = 0; i < Count; i++)
	{
		// ������ ��� ��ġ ���
		FVector2D Offset;
		Offset.X = OrbitRadius * cos(Angles[i]);
		Offset.Y = OrbitRadius * sin(Angles[i]);

		// ������ ���� ��ġ: SetFliesPos�� �������� Offset �߰�
		FVector2D SpawnPosition = SetFliesPos + Offset;

		// ���� ����
		ChildFly[i] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SpawnPosition));
		if (ChildFly[i] == nullptr)
		{
			MSGASSERT("AttackFly ���� ����");
			return;
		}

		// ���ϰ� ������ ����ٴϵ��� ����
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
	if (true == IsDeath())
	{
		return;
	}

	Sound = UEngineSound::Play("fly_cough_2.wav");

	BlackDustEffectRenderer->SetActive(true);	
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust");
	TimeEventer.PushEvent(SkillPostActionTime, std::bind(&ATheDukeOfFlies::EndSummonFliesAnimaition, this));
}

void ATheDukeOfFlies::EndSummonFliesAnimaition()
{
	if (true == IsDeath())
	{
		return;
	}

	BodyRenderer->ChangeAnimation("Idle");
	BlackDustEffectRenderer->ChangeAnimation("Black_Dust_End");
	BlackDustEffectRenderer->SetActive(false);
}

bool ATheDukeOfFlies::HasMaxFlies()
{
	int CurFlies = ParentRoom->CountFly();
	if (MaxFlyCount <= CurFlies)
	{
		return true; // ���� ����
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

	// ��ų �ߵ�����
	BlowAwayTriggerValue = 3; // 4����
	if (BlowAwayTriggerValue <= Followers)
	{
		// ���� ���� �ֺ��� �⵵�� �ĸ��� 4���� �̻��̶�� BlowAway ��ų�� ��!!!
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

	// ���� ������ �ĸ��� 4���� �̸��̸� ��ų ������.
	if (false == CanBlowAway())
	{
		return;
	}
	if (true == IsDeath())
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
	if (true == IsDeath())
	{
		return;
	}

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

		Fly->GetForce() = Dir * 230.0f;
		
	}
	
	TimeEventer.PushEvent(SkillPostActionTime, [this]() {
		IsAttacking = false;
		BlowAwayCooldownElapesd = 0.0f; });
}

void ATheDukeOfFlies::BeginBlowAwayAnimaition()
{
	if (true == IsDeath())
	{
		return;
	}

	Sound = UEngineSound::Play("fly_cough.wav");

	DustEffectRenderer->SetActive(true);
	DustEffectRenderer->ChangeAnimation("Dust");

	TimeEventer.PushEvent(SkillPostActionTime, std::bind(&ATheDukeOfFlies::EndBlowAwayAnimaition, this));
}

void ATheDukeOfFlies::EndBlowAwayAnimaition()
{
	if (true == IsDeath())
	{
		return;
	}

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

		++Count; // ���� ������ ���ϵ��� ����� �ִ���
	}

	// ��ų �ߵ� ����
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
	if (false == CanSummonBigFlies()) // ���� ������ �ĸ��� �����?
	{
		return;
	}
	if (true == IsDeath())
	{
		return;
	}
	// ���� ������ �ĸ��� ����? �׷��� ���ĸ� ��ȯ��.

	BodyRenderer->ChangeAnimation("Attack");
	ModifySkillCooldownElapsed();

	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonBigFliesLogic, this));
	TimeEventer.PushEvent(SkillCastDelay, std::bind(&ATheDukeOfFlies::BeginSummonFliesAnimaition, this)); // �ִϸ��̼��� ����
}

void ATheDukeOfFlies::BeginSummonBigFliesLogic()
{
	ARoom* ParentRoom = ARoom::GetCurRoom();
	if (nullptr == ParentRoom)
	{
		return;
	}
	if (true == IsDeath())
	{
		return;
	}
	// ���� ��ġ
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);

	float OrbitRadius = 150.0f;
	float Angles[2] = { // ���� ��ġ�� ������ ����
	static_cast<float>(40.0f * std::numbers::pi / 180.0f),
	static_cast<float>(140.0f * std::numbers::pi / 180.0f)
	};

	AAttackFly* ChildFly[2] = { nullptr, };
	for (int i = 0; i < 2; i++)
	{
		// ������ ��� ��ġ ���
		FVector2D Offset;
		Offset.X = OrbitRadius * cos(Angles[i]);
		Offset.Y = OrbitRadius * sin(Angles[i]);

		// ������ ���� ��ġ: SetFliesPos�� �������� Offset �߰�
		FVector2D SpawnPosition = SetFliesPos + Offset;

		// ���� ����
		ChildFly[i] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SpawnPosition));
		if (ChildFly[i] == nullptr)
		{
			MSGASSERT("AttackFly ���� ����");
			return;
		}

		// ���ϰ� ������ ����ٴϵ��� ����
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
}

ATheDukeOfFlies::~ATheDukeOfFlies()
{
}


