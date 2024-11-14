#pragma once
#include <EngineCore/Actor.h>
#include "BloodTear.h"
#include "Player.h"
#include "Room.h"

// ���� :
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
	virtual void ClampPositionToRoom() {} // �̵� ����

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
		if (true != Renderer->IsCurAnimationEnd())
		{
			return;
		}
		Renderer->SetActive(false);
		Renderer->Destroy();
		Renderer = nullptr;
	}

	bool IsDeath()
	{
		return Hp <= 0;
	}

	int GetHp() const
	{
		return Hp;
	}
	void SetHp(int _Hp) // Hp ����
	{
		Hp = _Hp;
	}
	int ApplyDamaged(int _PlayerAtt) // �ǰ�
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

	class USpriteRenderer* Renderer = nullptr;

	// Stat
	int   Hp    = 1;
	int   Att   = 1;
	int CollisionAtt = 1;
	float Speed = 100;
	FVector2D Direction = FVector2D::ZERO;

	// �̵� ���� ��Ÿ��
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

	ARoom* ParentRoom = nullptr;
	APlayer* Player = nullptr;

private:

};

