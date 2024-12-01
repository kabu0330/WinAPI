#include "PreCompile.h"
#include "Gaper.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Room.h"
#include "Pacer.h"

AGaper::AGaper()
{
	/* �̸�     : */ SetName("Gaper");
	/* ü��     : */ SetHp(10);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(120);
	/* �̵��ð� : */ SetMoveDuration(1.5f);
	/* �����ð� : */ SetMoveCooldown(1.0f);
	/* Ž������ : */ SetDetectRange({ 600 , 300 });
	/* �߻�ӵ� : */ SetShootingSpeed(0.0f);
	/* ��Ÿ��   : */ SetCooldown(0.0f);

	float FaceFrameSpeed = 1.0f;
	float FrameSpeed = 0.13f;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Left", "Body.png", 1, 9, FrameSpeed);
	BodyRenderer->CreateAnimation("Right", "Body.png", 10, 19, FrameSpeed);
	BodyRenderer->CreateAnimation("Down", "Body.png", 20, 29, FrameSpeed);
	BodyRenderer->CreateAnimation("Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Idle", "Body.png", 29, 29, FrameSpeed);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);


	std::string HeadName = "monster_017_gaper.png";

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Idle", HeadName, 0, 1, FaceFrameSpeed, false);
	//HeadRenderer->CreateAnimation("Chase", HeadName, 2, 3, FaceFrameSpeed, false);
	HeadRenderer->SetComponentLocation({ 0 , Global::PlayerHeadOffset.iY() + 30 });
	HeadRenderer->SetComponentScale({ 128, 128 });
	HeadRenderer->ChangeAnimation("Idle");
	HeadRenderer->SetOrder(ERenderOrder::Monster);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, -16 });
	BodyCollision->SetComponentScale({ 40, 55 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);
	DetectCollision->SetActive(true);
}

AGaper::~AGaper()
{
}

void AGaper::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

	State = MonsterState::NONE;
}

void AGaper::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	CheckDirection();

	SoundTimeElapsed += _DeltaTime;

}

void AGaper::Move(float _DeltaTime)
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
		IsCahsing = false;
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // ���� �ð� �̵��ϸ� ����
	{
		if (MoveElapsedTime > MoveCooldown) // ���� �� ���� �ð��� �帣�� �ٽ� �̵�
		{
			Direction = GetRandomDir(); // �÷��̾ Ž�� ������ ���ٸ� ���� �̵�
			//HeadRenderer->ChangeAnimation("Idle");
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}
	SetMoveSpeed(100);

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AGaper::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected)
	{
		return;
	}

	ChaseMove(_DeltaTime);
}

void AGaper::ChaseMove(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}

	SetMoveSpeed(180);

	if (false == IsCahsing)
	{
		//HeadRenderer->ChangeAnimation("Chase"); 
		IsCahsing = true;
	}
	Force = FVector2D::ZERO;
	Direction = GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);

	ChaseSound(_DeltaTime);
}

void AGaper::ChaseSound(float _DeltaTime)
{
	if (SoundTimeElapsed > SoundDuration)
	{
		SoundTimeElapsed = 0.0f;
		IsPlaySound = true;
	}

	if (false == IsPlaySound)
	{
		return;
	}

	Sound = UEngineSound::Play("monster_roar.wav");
	IsPlaySound = false;
}

void AGaper::Death(float _DeltaTime)
{
	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		Sound = UEngineSound::Play("death_burst_small.wav");
		Sound = UEngineSound::Play("meat_impact.wav");

		SummonPacerLogic();
		BloodEffectRenderer->SetActive(true);
		CreateGib();
		CollisionDestroy();
	}

	// 3. ���� �����
	if (nullptr == BodyRenderer)
	{
		ParentRoom->RemoveMonster(this);
		Destroy();
		return;
	}

	BloodEffectRenderer->ChangeAnimation("DeathEffect");


	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;
		BloodEffectRenderer->Destroy(0.1f);
		return;
	}

	BodyRenderer->SetActive(false);
	HeadRenderer->SetActive(false);
}

void AGaper::SummonPacerLogic()
{
	FVector2D Pos = GetActorLocation() - ParentRoom->GetActorLocation();
	AMonster* Pacer = ParentRoom->CreateMonster<APacer>({ Pos.X , Pos.Y });
	SetDestroySpawnEffect();
}

void AGaper::CurStateAnimation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (true == IsAttack)
	{
		return;
	}

	std::string Left = "Left";
	std::string Right = "Right";
	std::string Up = "Up";
	std::string Down = "Down";
	std::string Idle = "Idle";
	std::string Chase = "Chase";

	switch (State)
	{
	case MonsterState::LEFT:
		//HeadRenderer->ChangeAnimation(Idle);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RIGHT:
		//HeadRenderer->ChangeAnimation(Idle);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::UP:
		//HeadRenderer->ChangeAnimation(Idle);
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::DOWN:
		//HeadRenderer->ChangeAnimation(Idle);
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::ATTCK_LEFT:
	case MonsterState::ATTCK_RIGHT:
	case MonsterState::ATTCK_UP:
	case MonsterState::ATTCK_DOWN:
	case MonsterState::RUNAWAY_LEFT:
	case MonsterState::RUNAWAY_RIGHT:
	case MonsterState::RUNAWAY_UP:
	case MonsterState::RUNAWAY_DOWN:
	case MonsterState::RUNAWAY_NONE:
	case MonsterState::ATTCK_NONE:
	case MonsterState::NONE:
	case MonsterState::MAX:
	default:
		HeadRenderer->ChangeAnimation(Idle);
		BodyRenderer->ChangeAnimation(Idle);
		break;
	}
}

void AGaper::CheckDirection()
{
}