#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
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
	void Death(float _DeltaTime);

	int GetMonsterHp() const
	{
		return Hp;
	}
	int GetMonsterAtt() const
	{
		return Att;
	}
	int ApplyDamaged(float _PlayerAtt)
	{
		Hp -= _PlayerAtt;
		if (Hp < 0)
		{
			Hp = 0;
		}
		return Hp;
	}

protected:
	class U2DCollision* BodyCollision = nullptr;

	class USpriteRenderer* Renderer = nullptr;


	class ATear* Tear = nullptr;

	int   Hp    = 1;
	int   Att   = 1;
	float Speed = 0;
	
private:

};

