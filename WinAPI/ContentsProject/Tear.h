#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>

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

	void Fire(class APlayer* _Player, class AItem* _Item, FVector2D _StartPos, FVector2D _Dir, int _Att, float _TearSpeed, float _ItemTearSpeed, float _Duration, FVector2D _Scale);


	void UpdateTearPosion(float _DeltaTime);
	void ApplyGravity(FVector2D _Gravity);
	void ApplyForce(float );

	void Explode(AActor* _Other);
	void Explosion();

	void TimeBasedExplosion();
	void BoundaryExplosion(AActor* _Other);
	void MapObjectCollision(AActor* _Other);
	void ItemImpackCollision(AActor* _Other);
	virtual void HandleMonsterCollision(AActor* _Other);

	void SetTearRenderer(USpriteRenderer* _TearEffectRenderer)
	{
		TearEffectRenderer = _TearEffectRenderer;
		TearEffectRenderer->SetActive(true);
	}

protected:
	int ActorAtt = 0;

	float TearSpeed = 0.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.8f;

	float TearMaxSpeed = 1000.0f;

	// 아이템 관련 스텟
	FVector2D RendererScale = FVector2D::ZERO;
	FVector2D CollisionScale = FVector2D::ZERO;

	FVector2D Velocity = FVector2D::ZERO;
	FVector2D Force = FVector2D::ZERO;
	FVector2D Gravity = FVector2D(0.0f, 3500.0f);
	float GravityActivationTime = Duration - 0.15f;

	FVector2D Dir = FVector2D::ZERO;
	bool IsUpDownDir = false;

	class APlayer* Player = nullptr;

	FVector2D KnockbackDir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
	class U2DCollision* TearCollision = nullptr;

	AActor* CollisionActor = nullptr;
private:
	
};

