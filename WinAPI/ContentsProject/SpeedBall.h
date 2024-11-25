#pragma once
#include "Item.h"

// ���� :
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

	// �÷��̾�� ��ȣ�ۿ��ϴ� ������ ���
	void DropSucessAnimation(APlayer* _Player);

protected:

private:

};

