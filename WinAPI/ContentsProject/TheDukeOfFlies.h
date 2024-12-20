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
	void Death(float _DeltaTime) override;
	void CreateGib() override;
	void Oscillation(float _DeltaTime);

	FVector2D GetRandomDir() override;
	void ClampPositionToRoom() override;
	void BeginBlinkEffect() override;

	int ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir) override;

	// 인트로
	void DisplayBossIntro();
	void SpawnAnimation() override;

	// UI
	void DisplayBossHpBar();

	// 스킬 공통
	void SkillCooldown(float _DeltaTime);
	void ModifySkillCooldownElapsed();
	void RemoveFlies();

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

	USpriteRenderer* DamagedEffectRenderers[4] = { nullptr, };


	bool IsAttacking = false;
	float SkillCastDelay = 0.0f;
	float SkillPostActionTime = 0.0f;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyRendererScale = FVector2D::ZERO;

	// 패턴 1 : SummonFlies
	std::list<AMonster*> Flies; // 내가 소환한 파리들
	int MaxFlyCount = 0;
	float SummonCooldownDuration = 5.0f;

	// 패턴 2 : BlowAway
	int BlowAwayTriggerValue = 0; // 스킬 발동 조건을 지정할 변수
	float BlowAwayCooldownElapesd = 0.0f;
	float BlowAwayCooldownDuration = 5.5f;
	
	// 패턴 3 : SummonBigFly
	float SummonBigFlyCooldownElapsed = 0.0f;
	float SummonBigFlyCooldownDuration = 8.0f;
	
	
	// UI
	class ABossHpBar* BossHpBor = nullptr;
	bool ReadyToBossIntro = false;

	USoundPlayer DeathSound;
	bool IsPlayingDeathSound = false;
};

