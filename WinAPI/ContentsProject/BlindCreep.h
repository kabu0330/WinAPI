#pragma once
#include "Monster.h"

// Ό³Έν :
class ABlindCreep : public AMonster
{
public:
	// constrcuter destructer
	ABlindCreep();
	~ABlindCreep();

	// delete Function
	ABlindCreep(const ABlindCreep& _Other) = delete;
	ABlindCreep(ABlindCreep&& _Other) noexcept = delete;
	ABlindCreep& operator=(const ABlindCreep& _Other) = delete;
	ABlindCreep& operator=(ABlindCreep&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override;
	void FireTripleShotForward();
	void ChangeIdle();
	void ChaseMove(float _DeltaTime) override;
	void RandomSpeed();
protected:

private:

};

