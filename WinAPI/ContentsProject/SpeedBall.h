#pragma once
#include "Item.h"

// 설명 :
class ASpeedBall : public AItem
{
public:
	// constrcuter destructer
	ASpeedBall();
	~ASpeedBall();

	// delete Function
	ASpeedBall(const ASpeedBall& _Other) = delete;
	ASpeedBall(ASpeedBall&& _Other) noexcept = delete;
	ASpeedBall& operator=(const ASpeedBall& _Other) = delete;
	ASpeedBall& operator=(ASpeedBall&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SpriteSetting();

	bool EatFunction(APlayer* _Player) override;

	// 플레이어와 상호작용하는 아이템 드랍
	void DropSucessAnimation(APlayer* _Player);

protected:

private:

};

