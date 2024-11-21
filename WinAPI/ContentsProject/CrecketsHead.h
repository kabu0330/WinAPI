#pragma once
#include "Item.h"

// 설명 :
class ACrecketsHead : public AItem
{
public:
	// constrcuter destructer
	ACrecketsHead();
	~ACrecketsHead();

	// delete Function
	ACrecketsHead(const ACrecketsHead& _Other) = delete;
	ACrecketsHead(ACrecketsHead&& _Other) noexcept = delete;
	ACrecketsHead& operator=(const ACrecketsHead& _Other) = delete;
	ACrecketsHead& operator=(ACrecketsHead&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SpriteSetting();

	bool EatFunction(APlayer* _Player) override;

	// 플레이어와 상호작용하는 아이템 드랍
	void DropSucessAnimation(APlayer* _Player);

protected:

private:

};

