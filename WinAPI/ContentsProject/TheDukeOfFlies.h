#pragma once
#include "Monster.h"

// 설명 : Boss - 파리 공작
class ATheDukeOfFlies : public AMonster
{
public:
	// constrcuter destructer
	ATheDukeOfFlies();
	~ATheDukeOfFlies();

	// delete Function
	ATheDukeOfFlies(const ATheDukeOfFlies& _Other) = delete;
	ATheDukeOfFlies(ATheDukeOfFlies&& _Other) noexcept = delete;
	ATheDukeOfFlies& operator=(const ATheDukeOfFlies& _Other) = delete;
	ATheDukeOfFlies& operator=(ATheDukeOfFlies&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Attack(float _DeltaTime) override
	{

	}

	// 패턴 1
	void SummonFlies();
	void BeginSummonFliesLogic();
	void BeginSummonFliesAnimaition();
	void EndSummonFliesAnimaition();
	bool HasMaxFlies();

protected:

private:
	USpriteRenderer* DustEffectRenderer = nullptr;
	USpriteRenderer* DustCloudEffectRenderer = nullptr;
	int MaxFlyCount = 0;
};

