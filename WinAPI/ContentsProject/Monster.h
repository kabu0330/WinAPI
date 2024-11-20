#pragma once
#include <EngineCore/Actor.h>
#include "BloodTear.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include <EngineBase/EngineRandom.h>
#include <numbers>

enum class MonsterState
{
	NONE, 
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ATTCK_NONE, // +-5 하면 방향이 맞다.
	ATTCK_LEFT,
	ATTCK_RIGHT,
	ATTCK_UP,
	ATTCK_DOWN,
	MAX
};


// 설명 : 몬스터의 속성을 정의한 부모 클래스
class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster();

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void MonsterInputDebug();
	void CollisionFuctionSetting();
	void ReverseForce(float _DeltaTime);

	virtual void Move(float _DeltaTime);
	virtual void ChaseMove(float _DeltaTime);
	virtual void ChasePlayer(float _DeltaTime);
	virtual void Attack(float _DeltaTime);
	virtual void CurStateAnimation(float _DeltaTime);
	virtual FVector2D GetRandomDir();
	virtual void ChangeAnimIdle();
	virtual void HandleCollisionDamage();
	virtual void Death(float _DeltaTime);

	int ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir); // 피격
	void KnockbackTick();

	virtual void SpawnAnimation();
	void BodyRender();

	FVector2D GetDirectionToPlayer();
	void ClampPositionToRoom(); // 방 안으로 이동범위 고정
	bool IsPlayerNearby();

	void BeginBlinkEffect();
	void StayBlinkEffect();

	void SetParentRoom(class ARoom* _Parent)
	{
		ParentRoom = _Parent;
	}

	void DeathCheck(float _DeltaTime)
	{
		if (false == IsDeath())
		{
			return;
		}

		Death(_DeltaTime);
	}

	void CollisionDestroy()
	{
		if (nullptr != BodyCollision)
		{
			BodyCollision->Destroy();
			BodyCollision->SetActive(false);
			BodyCollision = nullptr;
		}

		if (nullptr != DetectCollision)
		{
			DetectCollision->Destroy();
			DetectCollision->SetActive(false);
			DetectCollision = nullptr;
		}
	}

	void RendererDestroy()
	{
		if (true != BodyRenderer->IsCurAnimationEnd())
		{
			return;
		}
		BodyRenderer->SetActive(false);
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;
	}

	class U2DCollision* GetBodyCollision()
	{
		return BodyCollision;
	}

	bool IsDeath()
	{
		return Hp <= 0;
	}

	int GetHp() const
	{
		return Hp;
	}
	void SetHp(int _Hp) // Hp 세팅
	{
		Hp = _Hp;
	}

	void SwitchDamagedEffectRenderer()
	{
		DamagedEffectRenderer->SetActive(false);
	}

	int GetAtt() const
	{
		return Att;
	}
	void SetAtt(int _Att)
	{
		Att = _Att;
	}

	float GetMoveSpeed() const
	{
		return Speed;
	}
	void SetMoveSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	float GetMoveCooldown() const
	{
		return MoveCooldown;
	}
	void SetMoveCooldown(float _MoveCooldown)
	{
		MoveCooldown = _MoveCooldown;
	}

	float GetMoveDuration() const
	{
		return MoveDuration;
	}
	void SetMoveDuration(float _MoveDuration)
	{
		MoveDuration = _MoveDuration;
	}

	FVector2D GetDetectRange() const 
	{
		return DetectRange;
	}
	void SetDetectRange(FVector2D _DetectRange)
	{
		DetectRange = _DetectRange;
	}

	float GetMoveElapsedTime() const
	{
		return MoveElapsedTime;
	}
	
	float GetCooldown() const
	{
		return Cooldown;
	}
	void SetCooldown(float _Cooldown)
	{
		Cooldown = _Cooldown;
	}

	float GetShootingSpeed() const
	{
		return ShootingSpeed;
	}
	void SetShootingSpeed(float _ShootingSpeed)
	{
		ShootingSpeed = _ShootingSpeed;
	}
	FVector2D GetTearDir() const
	{
		return TearDir;
	}
	void SetTearDir(FVector2D _TearDir)
	{
		TearDir = _TearDir;
	}

	void SetInvincible(bool _OnOff)
	{
		Invincibility = _OnOff;
	}
	void SwitchInvincibility()
	{
		Invincibility = !Invincibility;
	}
	bool IsInvincible()
	{
		return Invincibility;
	}
	void EnterInvincibility()
	{
		BodyCollision->SetActive(false);
	}

	bool IsAttacking()
	{
		return IsAttack;
	}

	void SwitchIsAttacking()
	{
		IsAttack = !IsAttack;
	}
	void SwitchIsHit()
	{
		IsHit = !IsHit;
	}
	
	U2DCollision* GetHeadCollision()
	{
		return HeadCollision;
	}

	FVector2D& GetForce()
	{
		return Force;
	}

	void SetForce(FVector2D _Force)
	{
		Force = _Force;
	}

	void OffDamagedEffect()
	{
		DamagedEffectRenderer->ChangeAnimation("DamagedEffect_End");
	}

protected:
	U2DCollision* BodyCollision = nullptr;
	class USpriteRenderer* BodyRenderer = nullptr;

	U2DCollision* HeadCollision = nullptr;
	FVector2D HeadCollisionScale = FVector2D::ZERO;

	MonsterState State = MonsterState::LEFT;
	class USpriteRenderer* DamagedEffectRenderer = nullptr;
	class USpriteRenderer* SpawnEffectRenderer = nullptr;
	class USpriteRenderer* BloodEffectRenderer = nullptr;

	// Boss 패턴 
	FVector2D Force = FVector2D::ZERO;

	FVector2D SpawnEffectScale = FVector2D::ZERO;
	bool SpawEvent = false;

	void FadeChange();
	void FadeIn();
	void FadeOut();
	void SpawnFadeChange();
	void SpawnFadeOut();

	float FadeValue = 0.0f;
	float FadeDir = 1.0f;
	int FadeCount = 0;

	// Stat
	int   Hp    = 1;
	int   Att   = 1;
	int CollisionAtt = 1;
	float Speed = 50;
	FVector2D Direction = FVector2D::ZERO;
	int PrevDir = -1;

	bool IsDeathAnimationPlay = false;

	bool IsAttack = false;
	bool IsHit = false; // 맞았냐
	bool CanKnockback = true; // 넉백 대상이냐
	float KnockbackPower = 0.0f;
	float KnockbackDuration = 0.3f;
	float LerpAlpha = 0.0f;
	FVector2D KnockbackDistance = FVector2D::ZERO;
	// 무적
	bool Invincibility = false;

	// 이동 관련 쿨타임
	float MoveElapsedTime = 0.0f;
	float MoveCooldown = 5.0f;
	float MoveDuration = 1.0f;

	// Detect And Chase
	FVector2D DetectRange = FVector2D(100, 100);
	bool PlayerDetected = false;
	class U2DCollision* DetectCollision = nullptr;

	// BodyCollision Attack
	float BodyCollisionCooldownElapsed = 0.0f;
	float BodyCollisionCooldown = 1.0f;

	// Attack
	ABloodTear* Tear = nullptr;
	float Cooldown = 2.0f;
	float CooldownElapsed = 0.0f;
	float ShootingSpeed = 300.0f;
	FVector2D TearDir = FVector2D::ZERO;

	class ARoom* ParentRoom = nullptr;
	class APlayer* Player = nullptr;


	// Random
	__int64 RandomSeed = 0;
	UEngineRandom MonsterRandom;
private:

};

