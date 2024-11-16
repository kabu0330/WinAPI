#pragma once
#include "Monster.h"

// Ό³Έν :
class AHopper : public AMonster
{
public:
	// constrcuter destructer
	AHopper();
	~AHopper();

	// delete Function
	AHopper(const AHopper& _Other) = delete;
	AHopper(AHopper&& _Other) noexcept = delete;
	AHopper& operator=(const AHopper& _Other) = delete;
	AHopper& operator=(AHopper&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void Move(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}

	void ChaseMove(float _DeltaTime) override
	{

	}

	void RandomSpeed();

protected:

private:

};

