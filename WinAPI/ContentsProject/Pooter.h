#pragma once
#include "Monster.h"

// Ό³Έν :
class APooter : public AMonster
{
public:
	// constrcuter destructer
	APooter();
	~APooter();

	// delete Function
	APooter(const APooter& _Other) = delete;
	APooter(APooter&& _Other) noexcept = delete;
	APooter& operator=(const APooter& _Other) = delete;
	APooter& operator=(APooter&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override;
	FVector2D GetRandomDir() override;
	void ChangeAnimIdle() override;
protected:

private:
	float PreparationDuration = 0.4f;
	float PreparationElapesd = 0.0f;
	bool IsAttackAnimation = false;
};

