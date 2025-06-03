#pragma once
#include "Item.h"

// Ό³Έν :
class ADessert  : public AItem
{
public:
	ADessert();
	~ADessert();

	// delete Function
	ADessert(const ADessert& _Other) = delete;
	ADessert(ADessert&& _Other) noexcept = delete;
	ADessert& operator=(const ADessert& _Other) = delete;
	ADessert& operator=(ADessert&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool EatFunction(APlayer* _Player) override;

	void DropSucessAnimation(APlayer* _Player);

protected:

private:

};

