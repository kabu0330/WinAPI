#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include "Player.h"
#include "Monster.h"

// 설명 : 맵에 존재하는 모든 맵오브젝트의 부모 클래스
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
	// 장애물로 기능할 오브젝트
	USpriteRenderer* BodyRenderer = nullptr;
	U2DCollision* BodyCollision = nullptr;
	FVector2D Scale = FVector2D::ZERO;

	// 상호작용이 가능한 오브젝트
	USpriteRenderer* ExplodingRenderer = nullptr;
	U2DCollision* ExplodingCollision = nullptr;
	bool CanExplode = false; // 파괴될 수 있는 오브젝트인지
	bool IsTearDamageable = false; // 플레이어 눈물 공격으로 파괴될 수 있는 오브젝트인지
	bool IsDead = false;
	int Hp = 9999; // 플레이어 눈물 공격으로 파괴될 수 있다면 Hp를 가져야 한다.

	class ARoom* ParentRoom = nullptr;
private:

};

