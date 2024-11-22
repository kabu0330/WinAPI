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

	void DisplayBossHpBar();

	void Attack(float _DeltaTime) override
	{

	}
	void Death(float _DeltaTime) override;
	void RemoveFlies();

	void SkillCooldown(float _DeltaTime);
	void ModifySkillCooldownElapsed();

	// 패턴 1 : SummonFlies : 파리 세마리 소환
	bool HasMaxFlies(); // 스킬 발동 조건 검사
	void SummonFlies();
	void BeginSummonFliesLogic();
	void BeginSummonFliesAnimaition();
	void EndSummonFliesAnimaition();
	std::list<AMonster*>& GetFliesList()
	{
		return Flies;
	}
	
	// 패턴 2 : BlowAway : 파리 밀어내기
	bool CanBlowAway(); // 스킬 발동 조건 검사
	void BlowAway();
	void BeginBlowAwayLogic();
	void BeginBlowAwayAnimaition();
	void EndBlowAwayAnimaition();

	// 패턴 3 : 왕파리 소환
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

	// 패턴 1 : SummonFlies
	std::list<AMonster*> Flies; // 내가 소환한 파리들
	int MaxFlyCount = 0;
	float SummonCooldownDuration = 4.0f;

	// 패턴 2 : BlowAway
	int BlowAwayTriggerValue = 0; // 스킬 발동 조건을 지정할 변수
	float BlowAwayCooldownElapesd = 0.0f;
	float BlowAwayCooldownDuration = 3.0f;
	
	// 패턴 3 : SummonBigFly
	float SummonBigFlyCooldownElapsed = 0.0f;
	float SummonBigFlyCooldownDuration = 6.0f;
	
	

	// UI
	class ABossHpBar* BossHpBor = nullptr;
};

