#pragma once
#include "Monster.h"

// Ό³Έν :
class AGaper : public AMonster
{
public:
	// constrcuter destructer
	AGaper();
	~AGaper();

	// delete Function
	AGaper(const AGaper& _Other) = delete;
	AGaper(AGaper&& _Other) noexcept = delete;
	AGaper& operator=(const AGaper& _Other) = delete;
	AGaper& operator=(AGaper&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Move(float _DeltaTime) override;
	void ChasePlayer(float _DeltaTime) override;
	void ChaseMove(float _DeltaTime) override;
	void ChaseSound(float _DeltaTime);

	void CurStateAnimation(float _DeltaTime) override;
	void CheckDirection();

	void Death(float _DeltaTime) override;
	void SummonPacerLogic();

protected:

private:
	USpriteRenderer* HeadRenderer = nullptr;

	float SoundTimeElapsed = 0.0f;
	float SoundDuration = 10.0f;
	bool IsPlaySound = true;
	bool IsCahsing = false;
};

