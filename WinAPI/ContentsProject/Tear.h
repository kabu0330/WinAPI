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

	void Fire(float _DeltaTime);
	void Reset();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Speed = 200.0f;
	float TimeElapesd = 0;


	class USpriteRenderer* TearEffectRenderer = nullptr;
};

