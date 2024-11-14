#pragma once
#include "Monster.h"

// Ό³Έν :
class AttackFly : public AMonster
{
public:
	// constrcuter destructer
	AttackFly();
	~AttackFly();

	// delete Function
	AttackFly(const AttackFly& _Other) = delete;
	AttackFly(AttackFly&& _Other) noexcept = delete;
	AttackFly& operator=(const AttackFly& _Other) = delete;
	AttackFly& operator=(AttackFly&& _Other) noexcept = delete;

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

	void DeathCheck(float _DeltaTime);
	void Death(float _DeltaTime);

protected:

private:

};

