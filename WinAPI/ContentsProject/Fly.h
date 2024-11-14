#pragma once
#include "Monster.h"

// Ό³Έν :
class AFly : public AMonster
{
public:
	// constrcuter destructer
	AFly();
	~AFly();

	// delete Function
	AFly(const AFly& _Other) = delete;
	AFly(AFly&& _Other) noexcept = delete;
	AFly& operator=(const AFly& _Other) = delete;
	AFly& operator=(AFly&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}

	void ChaseMove(float _DeltaTime) override
	{

	}

	void HandleCollisionDamage(float _DeltaTime)
	{

	}

protected:

private:

};

