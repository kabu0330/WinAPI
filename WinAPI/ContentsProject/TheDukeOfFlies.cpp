#include "PreCompile.h"
#include "TheDukeOfFlies.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

#include "Room.h"
#include "AttackFly.h"

ATheDukeOfFlies::ATheDukeOfFlies()
{
	/* �̸�     : */ SetName("TheDukeOfFlies");
	/* ü��     : */ SetHp(110);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(0);
	/* �̵��ð� : */ SetMoveDuration(1.0f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 100 , 100 });
	/* �߻�ӵ� : */ SetShootingSpeed(300.0f);
	/* ��Ÿ��   : */ SetCooldown(5.0f);


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 110, 110 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "duke.png", 2, 2, 0.1f, false);
	BodyRenderer->CreateAnimation("Attack", "duke.png", {1, 3, 0}, {1.0f, 1.5f, 2.0f}, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 160, 128 }); // 80, 64
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DustEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DustEffectRenderer->CreateAnimation("Dust", "effect_black_dust.png", 0, 10, 0.15f, false);
	DustEffectRenderer->CreateAnimation("Dust_End", "effect_black_dust.png", 10, 10, 0.1f, false);
	DustEffectRenderer->SetComponentLocation({ 0,20 });
	DustEffectRenderer->SetComponentScale({ 290, 160 }); // 145, 80
	DustEffectRenderer->ChangeAnimation("Dust");
	DustEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	DustEffectRenderer->SetAlphaFloat(0.9f);

	//DustCloudEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//DustCloudEffectRenderer->CreateAnimation("Dust_Cloud", "effect_black_dust_cloud.bmp", 0, 7, 0.5f, false);
	//DustCloudEffectRenderer->SetComponentLocation({ 0, 0 });
	//DustCloudEffectRenderer->SetComponentScale({ 52, 52 });
	//DustCloudEffectRenderer->ChangeAnimation("Dust_Cloud");
	//DustCloudEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);
}

ATheDukeOfFlies::~ATheDukeOfFlies()
{
}

void ATheDukeOfFlies::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

	DamagedEffectRenderer->SetComponentLocation({ 0, -30 });
	DamagedEffectRenderer->SetComponentScale({ 128, 128 });

	SpawnEffectRenderer->SetComponentLocation({ 0, -100 });
	SpawnEffectRenderer->SetComponentScale({ 384, 384});

	BloodEffectRenderer->SetComponentLocation({ 0, -60 });
	BloodEffectRenderer->SetComponentScale({ 512, 512 });

	DustEffectRenderer->SetActive(false);
}

void ATheDukeOfFlies::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	if (true == IsDeath())
	{
		return;
	}
	SummonFlies();
}

void ATheDukeOfFlies::SummonFlies()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();

	CooldownElapsed += DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Attack");
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginSummonFliesAnimaition, this));
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::BeginSummonFliesLogic, this));

	CooldownElapsed = 0.0f;
}

void ATheDukeOfFlies::BeginSummonFliesLogic()
{
	ARoom* ParentRoom = ARoom::GetCurRoom();
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);
	FVector2D OffsetX = FVector2D(30, 0);
	FVector2D OffsetY = FVector2D(0, -20);

	AAttackFly* ChildFly[3] = { 0, };
	ChildFly[0] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SetFliesPos));
	ChildFly[1] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SetFliesPos + OffsetX + OffsetY));
	ChildFly[2] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SetFliesPos - OffsetX + OffsetY));

	for (int i = 0; i < 3; i++)
	{
		if (nullptr == ChildFly[i])
		{
			MSGASSERT("������ AttackFly �����͸� �޴µ� �����߽��ϴ�.");
			return;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ChildFly[i]->SetIsFollowBoss(this); // ���� ������ ����
	}

}

void ATheDukeOfFlies::BeginSummonFliesAnimaition()
{
	DustEffectRenderer->SetActive(true);	
	DustEffectRenderer->ChangeAnimation("Dust");
	TimeEventer.PushEvent(2.5f, std::bind(&ATheDukeOfFlies::EndSummonFliesAnimaition, this));
}

void ATheDukeOfFlies::EndSummonFliesAnimaition()
{
	BodyRenderer->ChangeAnimation("Idle");
	DustEffectRenderer->ChangeAnimation("Dust_End");
	DustEffectRenderer->SetActive(false);
}


