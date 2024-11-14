#pragma once
#include <EngineCore/Actor.h>
#include "BloodTear.h"
#include "Player.h"
#include <EngineCore/SpriteRenderer.h>

// 설명 :
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

	virtual void Move(float _DeltaTime) {}
	virtual void ChaseMove(float _DeltaTime) {}
	virtual void Attack(float _DeltaTime) {}
	virtual void Death(float _DeltaTime) {}
	virtual void ClampPositionToRoom() {} // 이동 제한

	virtual void SetParentRoom(class ARoom* _Parent)
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
		BodyCollision->Destroy();
		BodyCollision = nullptr;
		DetectCollision->Destroy();
		DetectCollision = nullptr;
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
	int ApplyDamaged(int _PlayerAtt) // 피격
	{
		Hp -= _PlayerAtt;
		if (Hp < 0)
		{
			Hp = 0;
		}
		return Hp;
	}

	int GetAtt() const
	{
		return Att;
	}


protected:
	class U2DCollision* BodyCollision = nullptr;

	class USpriteRenderer* BodyRenderer = nullptr;

	// Stat
	int   Hp    = 1;
	int   Att   = 1;
	int CollisionAtt = 1;
	float Speed = 100;
	FVector2D Direction = FVector2D::ZERO;

	// 이동 관련 쿨타임
	float MoveElapsedTime = 0.0f;
	float MoveCooldown = 5.0f;
	float MoveTime = 1.0f;

	// Detect And Chase
	bool PlayerDetected = false;
	class U2DCollision* DetectCollision = nullptr;

	// BodyCollision Attack
	float BodyCollisionCooldownElapsed = 0.0f;
	float BodyCollisionCooldown = 1.0f;

	// Attack
	ABloodTear* Tear = nullptr;
	float Cooldown = 2.0f;
	float CoolDownElapsed = 0.0f;
	float ShootingSpeed = 300.0f;
	FVector2D TearDir = FVector2D::ZERO;

	class ARoom* ParentRoom = nullptr;
	APlayer* Player = nullptr;

private:

};

