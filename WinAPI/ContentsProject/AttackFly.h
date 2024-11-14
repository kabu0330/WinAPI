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

	void Attack(float _DeltaTime);
	void ChaseMove(float _DeltaTime);
	void Move(float _DeltaTime);
	FVector2D GetRandomDir();
	void ClampPositionToRoom();

	bool IsPlayerNearby();
	FVector2D GetDirectionToPlayer();
	void ChasePlayer(float _DeltaTime);

	void BodyCollisionCheck(float _DeltaTime);

	void Death(float _DeltaTime);

protected:

private:

};

