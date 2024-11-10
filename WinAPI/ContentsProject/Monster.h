#pragma once
#include <EngineCore/Actor.h>

// 설명 :
class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster();

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CreateMonster();

	void DeathCheck(float _DeltaTime);
	void Death(float _DeltaTime);

	bool IsDeath()
	{
		return Hp <= 0;
	}

	float GetMonsterHp() const
	{
		return Hp;
	}
	void SetMonsterHp(float _Hp) // Hp 세팅
	{
		Hp = _Hp;
	}
	float ApplyDamaged(float _PlayerAtt) // 피격
	{
		Hp -= _PlayerAtt;
		if (Hp < 0)
		{
			Hp = 0;
		}
		return Hp;
	}

	int GetMonsterAtt() const
	{
		return Att;
	}


protected:
	class U2DCollision* BodyCollision = nullptr;

	class USpriteRenderer* Renderer = nullptr;

	class ATear* Tear = nullptr;

	float Hp    = 1;
	int   Att   = 1;
	float Speed = 0;
	
private:

};

