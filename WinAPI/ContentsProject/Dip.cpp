#include "PreCompile.h"
#include "Dip.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"


ADip::ADip()
{
	/* �̸�     : */ SetName("Dip");
	/* ü��     : */ SetHp(4);
	/* ���ݷ�   : */ SetAtt(0);
	/* �̵��ӵ� : */ SetMoveSpeed(0);
	/* �̵��ð� : */ SetMoveDuration(0.5f);
	/* �����ð� : */ SetMoveCooldown(1.5f);
	/* Ž������ : */ SetDetectRange({ 0 , 0 });


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Dip.png", 0, 2, 0.4f);
	BodyRenderer->CreateAnimation("Move", "Dip.png", 3, 4, 0.15f, true);
	BodyRenderer->CreateAnimation("Death", "effect_bloodpoof.png", 0, 9, 0.1f, false);
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);
}

ADip::~ADip()
{
}

void ADip::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void ADip::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void ADip::Move(float _DeltaTime)
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
			Direction = GetRandomDir();
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	if (true == IsDeath())
	{
		return;
	}

	BodyRenderer->ChangeAnimation("Move");
	TimeEventer.PushEvent(GetMoveDuration() + 0.3f, std::bind(&AMonster::ChangeAnimIdle, this));

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

