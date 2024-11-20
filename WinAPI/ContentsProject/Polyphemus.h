#pragma once
#include "Item.h"

// ���� :
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

	// �÷��̾�� ��ȣ�ۿ��ϴ� ������ ���
	void DropSucessAnimation(APlayer* _Player);


protected:

private:
	
};

