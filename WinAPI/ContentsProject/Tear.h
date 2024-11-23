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

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CollisionSetting();

	void Fire(class APlayer* _Player, FVector2D _StartPos, FVector2D _Dir, float _Speed, int _Att);


	void UpdateTearPosion(float _DeltaTime);
	void ApplyGravity(FVector2D _Gravity);
	void ApplyForce(float );

	void Explode(AActor* _Other);
	void Explosion();

	void TimeBasedExplosion();
	void BoundaryExplosion(AActor* _Other);
	void MapObjectCollision(AActor* _Other);
	void ItemImpackCollision(AActor* _Other);
	void HandleMonsterCollision(AActor* _Other);


protected:

private:
	int ActorAtt = 0;

	float Speed = 720.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.7f;

	float ResistanceActivationTime = Duration - 0.2f;
	float Resistance = 0.8f;

	float GravityActivationTime = Duration - 0.1f;
	//FVector2D Gravity = FVector2D(Resistance * 0.85f, 0.35f);
	FVector2D GravityDir = FVector2D::ZERO;
	FTransform TearTrans;


	FVector2D Velocity = FVector2D::ZERO;
	FVector2D Force = FVector2D::ZERO;
	FVector2D Gravity = FVector2D(0.0f, 6000.0f);

	FVector2D Dir = FVector2D::ZERO;
	bool IsUpDownDir = false;
	
	class APlayer* Player = nullptr;
	
	FVector2D KnockbackDir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;
};

