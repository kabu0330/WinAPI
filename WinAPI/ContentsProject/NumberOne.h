#pragma once
#include "Item.h"

// ���� :
class ANumberOne : public AItem
{
public:
	ANumberOne();
	~ANumberOne();

	// delete Function
	ANumberOne(const ANumberOne& _Other) = delete;
	ANumberOne(ANumberOne&& _Other) noexcept = delete;
	ANumberOne& operator=(const ANumberOne& _Other) = delete;
	ANumberOne& operator=(ANumberOne&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SpriteSetting();

	bool EatFunction(APlayer* _Player) override;

	// �÷��̾�� ��ȣ�ۿ��ϴ� ������ ���
	void DropSucessAnimation(APlayer* _Player);
	void TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed) override;

protected:

private:

};

