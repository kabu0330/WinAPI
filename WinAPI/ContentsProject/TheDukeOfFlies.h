#pragma once
#include "Monster.h"

// ���� : Boss - �ĸ� ����
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

	// ���� 1 : SummonFlies : �ĸ� ������ ��ȯ
	bool HasMaxFlies(); // ��ų �ߵ� ���� �˻�
	void SummonFlies();
	void BeginSummonFliesLogic();
	void BeginSummonFliesAnimaition();
	void EndSummonFliesAnimaition();
	std::list<AMonster*>& GetFliesList()
	{
		return Flies;
	}
	
	// ���� 2 : BlowAway : �ĸ� �о��
	bool CanBlowAway(); // ��ų �ߵ� ���� �˻�
	void BlowAway();
	void BeginBlowAwayLogic();
	void BeginBlowAwayAnimaition();
	void EndBlowAwayAnimaition();

protected:

private:
	USpriteRenderer* BlackDustEffectRenderer = nullptr;
	USpriteRenderer* DustEffectRenderer = nullptr;

	// ���� 1 : SummonFlies
	std::list<AMonster*> Flies; // ���� ��ȯ�� �ĸ���
	int MaxFlyCount = 0;

	// ���� 2 : BlowAway
	int BlowAwayTriggerValue = 0; // ��ų �ߵ� ������ ������ ����
	float BlowAwayCooldownElapesd = 0.0f;
	float BlowAwayCooldownDuration = 6.0f;
	

};

