#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABullet : public AActor
{
public:
	// constrcuter destructer
	ABullet();
	~ABullet();

	// delete Function
	ABullet(const ABullet& _Other) = delete;
	ABullet(ABullet&& _Other) noexcept = delete;
	ABullet& operator=(const ABullet& _Other) = delete;
	ABullet& operator=(ABullet&& _Other) noexcept = delete;

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, float _Att);
	void TriggerExplosion(float _DeltaTime);

	void PlayerFireLogic(float _DeltaTime);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

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

	FVector2D Dir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;
};

