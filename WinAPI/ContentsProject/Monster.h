#pragma once
#include <EngineCore/Actor.h>
#include "BloodTear.h"

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

	void DeathCheck(float _DeltaTime);
	void Death(float _DeltaTime);

	void Attack(float _DeltaTime);

	FVector2D GetDirectionToPlayer();

	bool IsDeath()
	{
		return Hp <= 0;
	}

	int GetHp() const
	{
		return Hp;
	}
	void SetHp(int _Hp) // Hp 세팅
	{
		Hp = _Hp;
	}
	int ApplyDamaged(int _PlayerAtt) // 피격
	{
		Hp -= _PlayerAtt;
		if (Hp < 0)
		{
			Hp = 0;
		}
		return Hp;
	}

	int GetAtt() const
	{
		return Att;
	}


protected:
	class U2DCollision* BodyCollision = nullptr;

	class USpriteRenderer* Renderer = nullptr;

	int   Hp    = 1;
	int   Att   = 1;
	float Speed = 50;


	ABloodTear* Tear = nullptr;
	float Cooldown = 2.0f;
	float CoolDownElapsed = 0.0f;
	float ProjectileSpeed = 300.0f;
	FVector2D TearDir = FVector2D::ZERO;
	//bool TearFire = false;
	
private:

};

