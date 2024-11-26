#pragma once
#include "RoomObject.h"

// Ό³Έν :
class ARock : public ARoomObject
{
public:
	// constrcuter destructer
	ARock();
	~ARock();

	// delete Function
	ARock(const ARock& _Other) = delete;
	ARock(ARock&& _Other) noexcept = delete;
	ARock& operator=(const ARock& _Other) = delete;
	ARock& operator=(ARock&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	void DestroyRenderer() override;
	void DestroyCollision() override;
protected:

private:

};

