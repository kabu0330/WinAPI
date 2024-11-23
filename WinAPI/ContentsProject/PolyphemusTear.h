#pragma once
#include "Tear.h"

// Ό³Έν :
class APolyphemusTear : public ATear
{
public:
	// constrcuter destructer
	APolyphemusTear();
	~APolyphemusTear();

	// delete Function
	APolyphemusTear(const APolyphemusTear& _Other) = delete;
	APolyphemusTear(APolyphemusTear&& _Other) noexcept = delete;
	APolyphemusTear& operator=(const APolyphemusTear& _Other) = delete;
	APolyphemusTear& operator=(APolyphemusTear&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void HandleMonsterCollision(AActor* _Other);

protected:

private:

};

