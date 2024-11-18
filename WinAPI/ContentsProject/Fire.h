#pragma once
#include "RoomObject.h"

// Ό³Έν :
class AFire : public ARoomObject
{
public:
	// constrcuter destructer
	AFire();
	~AFire();

	// delete Function
	AFire(const AFire& _Other) = delete;
	AFire(AFire&& _Other) noexcept = delete;
	AFire& operator=(const AFire& _Other) = delete;
	AFire& operator=(AFire&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:
	USpriteRenderer* FlameHeaderRenderer = nullptr;
	USpriteRenderer* FlameBaseRenderer = nullptr;
};

