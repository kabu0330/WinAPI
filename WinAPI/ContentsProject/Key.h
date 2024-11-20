#pragma once
#include "Item.h"

// 설명 :
class AKey : public AItem
{
public:
	// constrcuter destructer
	AKey();
	~AKey();

	// delete Function
	AKey(const AKey& _Other) = delete;
	AKey(AKey&& _Other) noexcept = delete;
	AKey& operator=(const AKey& _Other) = delete;
	AKey& operator=(AKey&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool EatFunction(APlayer* _Player) override; // 아이템 습득 즉시
	void UseItem(APlayer* _Player) override; // 아이템 사용

	void DropSucessAnimation();

protected:

private:

};

