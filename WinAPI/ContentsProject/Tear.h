#pragma once
#include <EngineCore/Actor.h>

// 설명 : 플레이어의 공격 발사체 
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

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, int _Att);
	void Explosion();
	void UpdateTearPosion(float _DeltaTime);
	void CheckForExplosion(float _DeltaTime);
	void CollisionSetting();
	void Explode(AActor* _Other);

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	int ActorAtt = 0;

	float Speed = 350.0f;
	float SpeedMax = 500.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.85f;

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

