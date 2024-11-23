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
	void Death(float _DeltaTime) override;

	FVector2D GetRandomDir() override;
	void ClampPositionToRoom() override;
	void BeginBlinkEffect() override;

	// ��Ʈ��
	void DisplayBossIntro();
	void SpawnAnimation() override;

	// UI
	void DisplayBossHpBar();

	// ��ų ����
	void SkillCooldown(float _DeltaTime);
	void ModifySkillCooldownElapsed();
	void RemoveFlies();

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

	// ���� 3 : ���ĸ� ��ȯ
	bool CanSummonBigFlies();
	void SummonBigFlies();
	void BeginSummonBigFliesLogic();

protected:

private:
	USpriteRenderer* BlackDustEffectRenderer = nullptr;
	USpriteRenderer* DustEffectRenderer = nullptr;

	bool IsAttacking = false;
	float SkillCastDelay = 0.0f;
	float SkillPostActionTime = 0.0f;
	FVector2D BodyScale = FVector2D::ZERO;

	// ���� 1 : SummonFlies
	std::list<AMonster*> Flies; // ���� ��ȯ�� �ĸ���
	int MaxFlyCount = 0;
	float SummonCooldownDuration = 4.0f;

	// ���� 2 : BlowAway
	int BlowAwayTriggerValue = 0; // ��ų �ߵ� ������ ������ ����
	float BlowAwayCooldownElapesd = 0.0f;
	float BlowAwayCooldownDuration = 5.5f;
	
	// ���� 3 : SummonBigFly
	float SummonBigFlyCooldownElapsed = 0.0f;
	float SummonBigFlyCooldownDuration = 7.0f;
	
	

	// UI
	class ABossHpBar* BossHpBor = nullptr;
	bool ReadyToBossIntro = false;
};

