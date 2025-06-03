#pragma once
#include "Item.h"

// 설명 :
class APenny : public AItem
{
public:
	// constrcuter destructer
	APenny();
	~APenny();

	// delete Function
	APenny(const APenny& _Other) = delete;
	APenny(APenny&& _Other) noexcept = delete;
	APenny& operator=(const APenny& _Other) = delete;
	APenny& operator=(APenny&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool EatFunction(APlayer* _Player) override; // 아이템 습득 즉시
	void UseItem(APlayer* _Player) override; // 아이템 사용

	void DropSucessAnimation();
	void EndPickup();

protected:

private:
	bool IsPickup = false;
};

