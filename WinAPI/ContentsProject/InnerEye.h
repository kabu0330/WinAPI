#pragma once
#include "Item.h"

// ���� :
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

	// �÷��̾�� ��ȣ�ۿ��ϴ� ������ ���
	void DropSucessAnimation(APlayer* _Player);
	void TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed) override;
protected:

private:

};

