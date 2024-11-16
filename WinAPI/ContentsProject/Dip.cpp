#include "PreCompile.h"
#include "Dip.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"


ADip::ADip()
{
	/* 이름     : */ SetName("Dip");
	/* 체력     : */ SetHp(4);
	/* 공격력   : */ SetAtt(0);
	/* 이동속도 : */ SetMoveSpeed(0);
	/* 이동시간 : */ SetMoveDuration(0.5f);
	/* 정지시간 : */ SetMoveCooldown(1.5f);
	/* 탐색범위 : */ SetDetectRange({ 0 , 0 });


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
	if (true == PlayerDetected) // 플레이어를 추적하지 않는 중이면
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // 일정 시간 이동하면 리턴
	{
		if (MoveElapsedTime > MoveCooldown) // 멈춘 뒤 일정 시간이 흐르면 다시 이동
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

