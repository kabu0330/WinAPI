#pragma once
#include "Item.h"

// 설명 :
class APolyphemus : public AItem
{
public:
	// constrcuter destructer
	APolyphemus();
	~APolyphemus();

	// delete Function
	APolyphemus(const APolyphemus& _Other) = delete;
	APolyphemus(APolyphemus&& _Other) noexcept = delete;
	APolyphemus& operator=(const APolyphemus& _Other) = delete;
	APolyphemus& operator=(APolyphemus&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SpriteSetting();

	bool EatFunction(APlayer* _Player) override;

	// 플레이어와 상호작용하는 아이템 드랍
	void DropSucessAnimation(APlayer* _Player);


protected:

private:
	
};

