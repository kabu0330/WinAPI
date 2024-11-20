#pragma once
#include "Item.h"

// ���� :
class AKey : public AItem
{
public:
	// constrcuter destructer
	AKey();
	~AKey();

	// delete Function
	AKey(const AKey& _Other) = delete;
	AKey(AKey&& _Other) noexcept = delete;
	AKey& operator=(const AKey& _Other) = delete;
	AKey& operator=(AKey&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool EatFunction(APlayer* _Player) override; // ������ ���� ���
	void UseItem(APlayer* _Player) override; // ������ ���

	void DropSucessAnimation();

protected:

private:

};

