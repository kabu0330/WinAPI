#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
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

	void Fire(FVector2D _StartPos, FVector2D _Dir);
	void Reset();
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	float Speed = 400.0f;
	float TimeElapesd = 0.0f;
	float Duration = 1.0f;

	FVector2D Dir = FVector2D::ZERO;

	class USpriteRenderer* TearEffectRenderer = nullptr;
};

