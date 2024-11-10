#pragma once
#include <EngineCore/Actor.h>

// 설명 : 눈물공격 
class ATear : public AActor
{
public:
	// constrcuter destructer
	ATear();
	~ATear();

	// delete Function
	ATear(const ATear& _Other) = delete;
	ATear(ATear&& _Other) noexcept = delete;
	ATear& operator=(const ATear& _Other) = delete;
	ATear& operator=(ATear&& _Other) noexcept = delete;

	void Attack();

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, float _Att);
	void Reset();
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void TriggerExplosion(float _DeltaTime);

protected:

private:
	float ActorAtt = 0.0f;

	float Speed = 350.0f;
	float SpeedMax = 500.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.85f;
	float ExplosionDelay = 1.0f;

	float ResistanceActivationTime = 0.6f;
	float Resistance = 0.8f;

	float GravityActivationTime = 0.7f;
	FVector2D Gravity = FVector2D(Resistance * 0.85f, 0.35f);
	FVector2D GravityDir = FVector2D::ZERO;

	FVector2D Dir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;
};

