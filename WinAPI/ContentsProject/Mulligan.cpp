#include "PreCompile.h"
#include "Mulligan.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Room.h"
#include "AttackFly.h"
#include "Fly.h"


AMulligan::AMulligan()
{
	/* �̸�     : */ SetName("Mulligan");
	/* ü��     : */ SetHp(15);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(90);
	/* �̵��ð� : */ SetMoveDuration(2.0f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 450 , 350 });
	/* �߻�ӵ� : */ SetShootingSpeed(0.0f);
	/* ��Ÿ��   : */ SetCooldown(0.0f);

	State = MonsterState::NONE;

	float FaceFrameSpeed = 0.2f;
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


	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Left", "monster_054_mulligan_left.png", 0, 2, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("Right", "monster_054_mulligan_right.png", 0, 2, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("RunAway_Left", "monster_054_mulligan_left.png", 3, 5, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("RunAway_Right", "monster_054_mulligan_right.png", 3, 5, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("Attack_Left", "monster_054_mulligan_left.png", { 6, 7, 8 }, { 1.0f, 0.1f, 0.1f }, false);
	HeadRenderer->CreateAnimation("Attack_Right", "monster_054_mulligan_right.png", { 6, 7, 8 }, { 1.0f, 0.1f, 0.1f }, false);
	HeadRenderer->SetComponentLocation({ 1, Global::PlayerHeadOffset.iY() + 36 });
	HeadRenderer->SetComponentScale({ 256, 256 });
	HeadRenderer->ChangeAnimation("Left");
	HeadRenderer->SetOrder(ERenderOrder::Monster);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, -15 });
	BodyCollision->SetComponentScale({ 30, 50 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);
	DetectCollision->SetActive(true);
}

AMulligan::~AMulligan()
{
}

void AMulligan::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AMulligan::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	CheckDirection();
	Oscillation(_DeltaTime);
	Attack(_DeltaTime);

	SoundTimeElapsed += _DeltaTime;
}

void AMulligan::Move(float _DeltaTime)
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
	SetMoveSpeed(100);

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMulligan::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected)
	{
		return;
	}

	ChaseMove(_DeltaTime);
	
}

// �÷��̾ �Ѵ°� �ƴ϶� ��������.
void AMulligan::ChaseMove(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}

	SetMoveSpeed(150);

	Force = FVector2D::ZERO;
	Direction = -1 * GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
	
	RunAwayTimeElapsed += _DeltaTime;	

	RunAwaySound(_DeltaTime);
}

void AMulligan::RunAwaySound(float _DeltaTime)
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

	Sound = UEngineSound::Play("scared_whimper_2.wav");
	IsPlaySound = false;
}

void AMulligan::Attack(float _DeltaTime)
{
	if (RunAwayTimeElapsed < RunAwayDuration)
	{
		return;
	}

	Invincibility = true; // ����
	IsAttack = true;

	SetMoveSpeed(0);
	HeadRenderer->ChangeAnimation("Attack_Right");
	HeadRenderer->SetComponentScale({224, 224});
	BodyRenderer->ChangeAnimation("Idle");

	RunAwayTimeElapsed = 0.0f;
	TimeEventer.PushEvent(1.2f, [this, _DeltaTime]() {
		SummonFliesLogic();
		SummonFliesRender();
		Hp = 0; });

}

void AMulligan::SummonFliesLogic()
{
	FVector2D Pos = GetActorLocation() - ParentRoom->GetActorLocation();
	FVector2D Offset = { 30.0f, 30.0f };
	ParentRoom->CreateMonster<AAttackFly>({ Pos.X , Pos.Y });
	ParentRoom->CreateMonster<AFly>({Pos.X - Offset.X, Pos.Y });
	ParentRoom->CreateMonster<AFly>({Pos.X + Offset.X, Pos.Y });
	ParentRoom->CreateMonster<AFly>({ Pos.X , Pos.Y - Offset.Y });
}

void AMulligan::SummonFliesRender()
{
	Sound = UEngineSound::Play("explosion_weak1.wav");

	USpriteRenderer* ExplosionEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExplosionEffectRenderer->CreateAnimation("Explosion", "Explosion.png", 0, 11, 0.10f, false);
	//ExplosionEffectRenderer->SetComponentLocation({ 0, 50 });
	ExplosionEffectRenderer->SetComponentScale({ 300, 300 });
	ExplosionEffectRenderer->SetOrder(ERenderOrder::ItemEffect);
	ExplosionEffectRenderer->ChangeAnimation("Explosion");
}

void AMulligan::Death(float _DeltaTime)
{
	if (false == IsAttack)
	{
		SummonFliesLogic();
		SummonFliesRender();
		IsAttack = true;
	}

	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		Sound = UEngineSound::Play("death_burst_small.wav");
		Sound = UEngineSound::Play("meat_impact.wav");

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

int AMulligan::ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir)
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

// �÷��̾ ���� �ݰ濡 ���� ���� State�� Ȯ��
void AMulligan::CheckDirection()
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

	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected)
	{
		return;
	}

	if (0.0f > Direction.X)
	{
		State = MonsterState::LEFT;
	}
	else if (0.0f < Direction.X)
	{
		State = MonsterState::RIGHT;
	}
	else if (0.0f > Direction.Y)
	{
		State = MonsterState::UP;
	}
	else if (0.0f < Direction.Y)
	{
		State = MonsterState::DOWN;
	}
}

void AMulligan::Oscillation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	// ���� �ӵ��� ���� ����
	const float OscillationSpeed = 1000.0f; // ���� �ӵ�
	const float OscillationMagnitude = 10.0f; // ���� ũ��

	// ���� �Լ��� �̿��� ���� ���
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// ������Ʈ
	FVector2D BodyRendererScale = { 64, 64 };
	FVector2D HeadRendererScale = { 256, 256 };
	FVector2D NewBodyScale = { BodyRendererScale.X + Oscillation,BodyRendererScale.Y + Oscillation};
	FVector2D NewHeadScale = { HeadRendererScale.X + Oscillation,HeadRendererScale.Y + Oscillation};
	BodyRenderer->SetComponentScale(NewBodyScale);
	HeadRenderer->SetComponentScale(NewHeadScale);
}

void AMulligan::CurStateAnimation(float _DeltaTime)
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
	std::string RunAway = "RunAway_";
	std::string Attack = "Attack_";

	switch (State)
	{
	case MonsterState::LEFT:
		HeadRenderer->ChangeAnimation(Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RIGHT:
		HeadRenderer->ChangeAnimation(Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::UP:
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::DOWN:
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::ATTCK_LEFT:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::ATTCK_RIGHT:
		HeadRenderer->ChangeAnimation(Attack + Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::ATTCK_UP:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::ATTCK_DOWN:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::RUNAWAY_LEFT:
		HeadRenderer->ChangeAnimation(RunAway + Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RUNAWAY_RIGHT:
		HeadRenderer->ChangeAnimation(RunAway + Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::RUNAWAY_UP:
		HeadRenderer->ChangeAnimation(RunAway + Up);
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::RUNAWAY_DOWN:
		HeadRenderer->ChangeAnimation(RunAway + Down);
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::RUNAWAY_NONE:
	case MonsterState::ATTCK_NONE:
	case MonsterState::NONE:
	case MonsterState::MAX:
	default:
		HeadRenderer->ChangeAnimation(Left);
		BodyRenderer->ChangeAnimation("Idle");
		break;
	}

}