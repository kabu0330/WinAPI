#pragma once
#include "Item.h"

// ���� :
class ABomb : public AItem
{
public:
	// constrcuter destructer
	ABomb();
	~ABomb();

	// delete Function
	ABomb(const ABomb& _Other) = delete;
	ABomb(ABomb&& _Other) noexcept = delete;
	ABomb& operator=(const ABomb& _Other) = delete;
	ABomb& operator=(ABomb&& _Other) noexcept = delete;
	
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool EatFunction(APlayer* _Player) override; // ������ ���� ���
	void UseItem(APlayer* _Player) override; // ������ ���

	void Explosion();

protected:

private:
	USpriteRenderer* BobmSparkEffectRenderer = nullptr;
	USpriteRenderer* ExplosionEffectRenderer = nullptr;
	USpriteRenderer* BodyRenderer = nullptr;

};

