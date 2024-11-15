#pragma once
#include "Monster.h"

// Ό³Έν :
class AHost : public AMonster
{
public:
	// constrcuter destructer
	AHost();
	~AHost();

	// delete Function
	AHost(const AHost& _Other) = delete;
	AHost(AHost&& _Other) noexcept = delete;
	AHost& operator=(const AHost& _Other) = delete;
	AHost& operator=(AHost&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override;
	void FireTripleShotForward();
	void ChangeIdle();


	void Move(float _DeltaTime) override
	{

	}

	void ChaseMove(float _DeltaTime) override
	{

	}

protected:

private:
	class U2DCollision* HeadCollision = nullptr;
	FVector2D HeadCollisionScale = FVector2D::ZERO;
};

