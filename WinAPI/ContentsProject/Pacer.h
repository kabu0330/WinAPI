#pragma once
#include "Monster.h"

// Ό³Έν :
class APacer : public AMonster
{
public:
	// constrcuter destructer
	APacer();
	~APacer();

	// delete Function
	APacer(const APacer& _Other) = delete;
	APacer(APacer&& _Other) noexcept = delete;
	APacer& operator=(const APacer& _Other) = delete;
	APacer& operator=(APacer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}

	void ChaseMove(float _DeltaTime) override
	{

	}
	void CurStateAnimation(float _DeltaTime) override;



protected:

private:

};

