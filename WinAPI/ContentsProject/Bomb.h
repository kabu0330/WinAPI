#pragma once
#include "Item.h"

// 설명 :
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

	bool EatFunction(APlayer* _Player) override; // 아이템 습득 즉시
	void UseItem(APlayer* _Player) override; // 아이템 사용

	void Explosion();

protected:

private:
	USpriteRenderer* BobmSparkEffectRenderer = nullptr;
	USpriteRenderer* ExplosionEffectRenderer = nullptr;
	USpriteRenderer* BodyRenderer = nullptr;

};

