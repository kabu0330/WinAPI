#pragma once
#include "Monster.h"

// Ό³Έν :
class ADip : public AMonster
{
public:
	// constrcuter destructer
	ADip();
	~ADip();

	// delete Function
	ADip(const ADip& _Other) = delete;
	ADip(ADip&& _Other) noexcept = delete;
	ADip& operator=(const ADip& _Other) = delete;
	ADip& operator=(ADip&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Move(float _DeltaTime) override;
	void ChangeIdle();
	void CreateGib() override;

	void Attack(float _DeltaTime) override
	{

	}

	void ChaseMove(float _DeltaTime) override
	{

	}

protected:

private:

};

