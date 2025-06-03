#pragma once
#include "Item.h"

// 설명 :
class AInnerEye : public AItem
{
public:
	AInnerEye();
	~AInnerEye();

	// delete Function
	AInnerEye(const AInnerEye& _Other) = delete;
	AInnerEye(AInnerEye&& _Other) noexcept = delete;
	AInnerEye& operator=(const AInnerEye& _Other) = delete;
	AInnerEye& operator=(AInnerEye&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SpriteSetting();

	bool EatFunction(APlayer* _Player) override;

	// 플레이어와 상호작용하는 아이템 드랍
	void DropSucessAnimation(APlayer* _Player);
	void TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed) override;
protected:

private:

};

