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

	void Death(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}


protected:

private:

};

