#pragma once
#include "Item.h"

// Ό³Έν :
class AHeart : public AItem
{
public:
	// constrcuter destructer
	AHeart();
	~AHeart();

	// delete Function
	AHeart(const AHeart& _Other) = delete;
	AHeart(AHeart&& _Other) noexcept = delete;
	AHeart& operator=(const AHeart& _Other) = delete;
	AHeart& operator=(AHeart&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);

protected:

private:

};

