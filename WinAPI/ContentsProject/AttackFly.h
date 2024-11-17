#pragma once
#include "Monster.h"

// Ό³Έν :
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
	}
	void SetInitialAngle(float _Angle);

	void ChasePlayer(float _DeltaTime) override;
	void ChaseMove(float _DeltaTime) override;
protected:

private:
	
	void FollowBoss(float _DeltaTime);
	AMonster* ParentBoss = nullptr;
	bool IsFollowBoss = false;
	float OrbitRadius = 100.0f;
	float OrbitSpeed = 2.0f;
	float Angle = 0.0f;


};

