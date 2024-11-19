#pragma once
#include "RoomObject.h"

// Ό³Έν :
class ABloodPool : public ARoomObject
{
public:
	// constrcuter destructer
	ABloodPool();
	~ABloodPool();

	// delete Function
	ABloodPool(const ABloodPool& _Other) = delete;
	ABloodPool(ABloodPool&& _Other) noexcept = delete;
	ABloodPool& operator=(const ABloodPool& _Other) = delete;
	ABloodPool& operator=(ABloodPool&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:

};

