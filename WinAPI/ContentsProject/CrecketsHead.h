#pragma once
#include "Item.h"

// ���� :
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

	// �÷��̾�� ��ȣ�ۿ��ϴ� ������ ���
	void DropSucessAnimation(APlayer* _Player);

protected:

private:

};

