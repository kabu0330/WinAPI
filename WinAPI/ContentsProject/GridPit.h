#pragma once
#include "RoomObject.h"

// Ό³Έν :
class AGridPit : public ARoomObject
{
public:
	// constrcuter destructer
	AGridPit();
	~AGridPit();

	// delete Function
	AGridPit(const AGridPit& _Other) = delete;
	AGridPit(AGridPit&& _Other) noexcept = delete;
	AGridPit& operator=(const AGridPit& _Other) = delete;
	AGridPit& operator=(AGridPit&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:

};

