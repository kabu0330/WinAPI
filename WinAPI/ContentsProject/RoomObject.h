#pragma once
#include <EngineBase/EngineMath.h>
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

	void SetSprite(std::string_view _Name)
	{
		if (nullptr == BodyRenderer)
		{
			return;
		}
		BodyRenderer->ChangeAnimation(_Name);
	}

	U2DCollision* GetBodyCollision()
	{
		return BodyCollision;
	}

	void CollisionSetting();
	void Blocker(AActor* _Actor);
	void PlayerCollision(class APlayer* _Player, FVector2D _Pos);
	void MonsterCollision(class AMonster* _Monster, FVector2D _Pos);
	void DestroyCollision();
	bool CheckHp();
	
	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}

protected:
	// ��ֹ��� ����� ������Ʈ
	USpriteRenderer* BodyRenderer = nullptr;
	U2DCollision* BodyCollision = nullptr;
	FVector2D Scale = FVector2D::ZERO;

	// ��ȣ�ۿ��� ������ ������Ʈ
	USpriteRenderer* ExplodingRenderer = nullptr;
	U2DCollision* ExplodingCollision = nullptr;
	bool CanExplode = false; // �ı��� �� �ִ� ������Ʈ����
	bool IsTearDamageable = false; // �÷��̾� ���� �������� �ı��� �� �ִ� ������Ʈ����
	bool IsDead = false;
	int Hp = 9999; // �÷��̾� ���� �������� �ı��� �� �ִٸ� Hp�� ������ �Ѵ�.

	class ARoom* ParentRoom = nullptr;
private:

};

