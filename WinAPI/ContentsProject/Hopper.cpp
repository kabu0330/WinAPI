#include "PreCompile.h"
#include "Hopper.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

AHopper::AHopper()
{
	/* �̸�     : */ SetName("Hopper");
	/* ü��     : */ SetHp(10);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(250);
	/* �̵��ð� : */ SetMoveDuration(0.8f);
	/* �����ð� : */ SetMoveCooldown(1.5f);
	/* Ž������ : */ SetDetectRange({ 0 , 0 });

	
	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Hopper_Idle", "Hopper2.png", { 0, 0 }, 0.1f);
	BodyRenderer->CreateAnimation("Hopper_Move", "Hopper2.png", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 1, 0}, 0.15f, false);
	BodyRenderer->CreateAnimation("Death", "effect_bloodpoof.png", 0, 9, 0.1f, false);
	BodyRenderer->SetComponentLocation({ -7, -15 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Hopper_Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monsetr);
}

AHopper::~AHopper()
{
}

void AHopper::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AHopper::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void AHopper::Move(float _DeltaTime)
{
	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // ���� �ð� �̵��ϸ� ����
	{
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			BodyRenderer->ChangeAnimation("Hopper_Idle");
		}
		if (MoveElapsedTime > MoveCooldown) // ���� �� ���� �ð��� �帣�� �ٽ� �̵�
		{
			BodyRenderer->ChangeAnimation("Hopper_Move");
			Direction = GetRandomDir();
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}
