#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABloodTear : public AActor
{
public:
	// constrcuter destructer
	ABloodTear();
	~ABloodTear();

	// delete Function
	ABloodTear(const ABloodTear& _Other) = delete;
	ABloodTear(ABloodTear&& _Other) noexcept = delete;
	ABloodTear& operator=(const ABloodTear& _Other) = delete;
	ABloodTear& operator=(ABloodTear&& _Other) noexcept = delete;

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, float _Att);
	void TriggerExplosion(float _DeltaTime);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	int ActorAtt = 0;

	float Speed = 350.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.85f;
	float ExplosionDelay = 1.0f;

	FVector2D Dir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;
};

