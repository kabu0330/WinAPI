#pragma once
#include "Monster.h"

// ���� :
class AAttackFly : public AMonster
{
public:
	// constrcuter destructer
	AAttackFly();
	~AAttackFly();

	// delete Function
	AAttackFly(const AAttackFly& _Other) = delete;
	AAttackFly(AAttackFly&& _Other) noexcept = delete;
	AAttackFly& operator=(const AAttackFly& _Other) = delete;
	AAttackFly& operator=(AAttackFly&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}

	void Move(float _DeltaTime) override;

	void SetIsFollowBoss(AMonster* _Boss)
	{
		ParentBoss = _Boss;
		IsFollowBoss = true;

		if (nullptr == SpawnEffectRenderer)
		{
			return;
		}
		SpawnEffectRenderer->Destroy();
		SpawnEffectRenderer = nullptr;
	}
	void SetInitialAngle(float _Angle);

	void ChasePlayer(float _DeltaTime) override;
	void ChaseMove(float _DeltaTime) override;

	void RemoveFly();
	bool GetIsFollowBoss()
	{
		return IsFollowBoss;
	}
	void SetHp(int _Value)
	{
		Hp = _Value;
	}
	void SetScale(float _Value)
	{
		RendererScale *= _Value;
		CollisionScale *= _Value;
	}

	void CreateGib();

protected:

private:
	
	void FollowBoss(float _DeltaTime);

	AMonster* ParentBoss = nullptr;
	bool IsFollowBoss = false;
	float OrbitRadius = 120.0f;
	float OrbitSpeed = 2.0f;
	float Angle = 0.0f;

	FVector2D RendererScale = FVector2D::ZERO;
	FVector2D CollisionScale = FVector2D::ZERO;
	float AdjustScale = 0.0f;


	USoundPlayer FlyingSound;

};

