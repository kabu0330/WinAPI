#pragma once
#include <EngineCore/Actor.h>

// 설명 : 몬스터의 공격 발사체
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

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed = 300.0f, int _Att = 1);
	void Explosion();

	void UpdateTearPosion(float _DeltaTime);

	void CollisionSetting();
	void Explode(AActor* _Other);

	void TimeBasedExplosion();
	void BoundaryExplosion();
	void MapObjectCollision(AActor* _Other);
	void HandleMonsterCollision(AActor* _Other);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	int ActorAtt = 0;

	float Speed = 300.0f;
	float Duration = 0.85f;

	float TimeElapesd = 0.0f;
	float ExplosionDelay = 1.0f;

	FVector2D Dir = FVector2D::ZERO;
	FVector2D KnockbackDir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;

	USoundPlayer Sound;
};

