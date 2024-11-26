#pragma once
#include "RoomObject.h"

// Ό³Έν :
class AMetalBlock : public ARoomObject
{
public:
	// constrcuter destructer
	AMetalBlock();
	~AMetalBlock();

	// delete Function
	AMetalBlock(const AMetalBlock& _Other) = delete;
	AMetalBlock(AMetalBlock&& _Other) noexcept = delete;
	AMetalBlock& operator=(const AMetalBlock& _Other) = delete;
	AMetalBlock& operator=(AMetalBlock&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:

};

