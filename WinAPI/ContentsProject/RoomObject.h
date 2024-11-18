#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineString.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Player.h"
#include "Monster.h"

// ���� : �ʿ� �����ϴ� ��� �ʿ�����Ʈ�� �θ� Ŭ����
class ARoomObject : public AActor
{
public:
	// constrcuter destructer
	ARoomObject();
	~ARoomObject();

	// delete Function
	ARoomObject(const ARoomObject& _Other) = delete;
	ARoomObject(ARoomObject&& _Other) noexcept = delete;
	ARoomObject& operator=(const ARoomObject& _Other) = delete;
	ARoomObject& operator=(ARoomObject&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);
	void ChangeRenderOrder();

	void SetSprite(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);
		ObjectName = UpperName;
	}

	U2DCollision* GetBodyCollision()
	{
		return BodyCollision;
	}

	void CollisionSetting();
	void Blocker(AActor* _Actor);
	void PlayerCollision(class APlayer* _Player, FVector2D _Pos);
	void MonsterCollision(class AMonster* _Monster, FVector2D _Pos);

	// �ı��� ������Ʈ
	virtual void SwitchAnimation();
	int ApplyDamaged(AActor* _Actor);
	void DestroyCollision();
	bool IsDeath();

	// Fire
	void DealDamageToPlayer(AActor* _Actor);
	
	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}
	
	int GetHp()
	{
		return Hp;
	}

protected:
	// ��ֹ��� ����� ������Ʈ
	USpriteRenderer* BodyRenderer = nullptr;
	U2DCollision* BodyCollision = nullptr;
	U2DCollision* BlockingPathCollision = nullptr;
	FVector2D BodyRendererScale = FVector2D::ZERO;
	std::string ObjectName = "";

	bool CanExplode       = false; // �ı��� �� �ִ� ������Ʈ����
	bool IsTearDamageable = false; // �÷��̾� ���� �������� �ı��� �� �ִ� ������Ʈ����
	bool IsBlockingPath   = false;
	bool IsAttackable     = false;
	bool IsDead = false;
	int Hp = 9999; // �÷��̾� ���� �������� �ı��� �� �ִٸ� Hp�� ������ �Ѵ�.

	class ARoom* ParentRoom = nullptr;
private:

};

