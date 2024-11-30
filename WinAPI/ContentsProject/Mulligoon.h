#pragma once
#include "Monster.h"

// 설명 :
class AMulligoon : public AMonster
{
public:
	// constrcuter destructer
	AMulligoon();
	~AMulligoon();

	// delete Function
	AMulligoon(const AMulligoon& _Other) = delete;
	AMulligoon(AMulligoon&& _Other) noexcept = delete;
	AMulligoon& operator=(const AMulligoon& _Other) = delete;
	AMulligoon& operator=(AMulligoon&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Move(float _DeltaTime) override;
	void ChasePlayer(float _DeltaTime) override;
	void ChaseMove(float _DeltaTime) override;
	void RunAwaySound(float _DeltaTime);

	void CurStateAnimation(float _DeltaTime) override;
	void CheckDirection();

	void Attack(float _DeltaTime) override;
	void SummonFliesLogic();
	void SummonFliesRender();

	void Death(float _DeltaTime) override;

	int ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir); // 피격

	void Oscillation(float _DeltaTime);

protected:

private:
	USpriteRenderer* HeadRenderer = nullptr;
	float RunAwayTimeElapsed = 0.0f;
	float RunAwayDuration = 6.0f;

	float SoundTimeElapsed = 0.0f;
	float SoundDuration = 4.0f;
	bool IsPlaySound = true;

	FVector2D BodyScale = FVector2D::ZERO;


};

