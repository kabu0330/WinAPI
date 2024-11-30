#pragma once
#include "Monster.h"

// Ό³Έν :
class AHorf : public AMonster
{
public:
	// constrcuter destructer
	AHorf();
	~AHorf();

	// delete Function
	AHorf(const AHorf& _Other) = delete;
	AHorf(AHorf&& _Other) noexcept = delete;
	AHorf& operator=(const AHorf& _Other) = delete;
	AHorf& operator=(AHorf&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override;

	void ChangeAggressiveAnimation() override;

	void Oscillation(float _DeltaTime);



protected:

private:
	FVector2D BodyScale = FVector2D::ZERO;
	FVector2D CurPos = FVector2D::ZERO;

	bool IsOscillate = false;
};

