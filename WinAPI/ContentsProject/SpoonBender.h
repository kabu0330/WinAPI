#pragma once
#include "Item.h"

// ���� :
class ASpoonBender : public AItem
{
public:
	// constrcuter destructer
	ASpoonBender();
	~ASpoonBender();

	// delete Function
	ASpoonBender(const ASpoonBender& _Other) = delete;
	ASpoonBender(ASpoonBender&& _Other) noexcept = delete;
	ASpoonBender& operator=(const ASpoonBender& _Other) = delete;
	ASpoonBender& operator=(ASpoonBender&& _Other) noexcept = delete;

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

