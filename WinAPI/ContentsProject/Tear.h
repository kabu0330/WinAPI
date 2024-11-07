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

	void Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed);
	void Reset();
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	float Speed = 380.0f;
	float SpeedMax = 600.0f;
	float TimeElapesd = 0.0f;
	float Duration = 0.85f;

	float ResistanceActivationTime = 0.4f;
	float Resistance = 0.8f;

	float GravityActivationTime = 0.65f;
	FVector2D Gravity = FVector2D(Resistance * 0.75f, 0.18f);
	FVector2D GravityDir = FVector2D::ZERO;

	FVector2D Dir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
};

