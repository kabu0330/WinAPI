#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineString.h>
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
	void ChangeRenderOrder();

	void SetSprite(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);
		ObjectName = UpperName;
	}
	virtual void SetSprite(std::string_view _StrNum, FVector2D _Scale) {};

	U2DCollision* GetBodyCollision()
	{
		return BodyCollision;
	}

	void CollisionSetting();
	void Blocker(AActor* _Actor);
	void PlayerCollision(class APlayer* _Player, FVector2D _Pos);
	void MonsterCollision(class AMonster* _Monster, FVector2D _Pos);

	// 파괴형 오브젝트
	virtual void SwitchAnimation();
	int ApplyDamaged(AActor* _Actor);
	void DestroyCollision();
	bool IsDeath();

	// Fire
	void DealDamageToPlayer(AActor* _Actor);
	

	
	int GetHp()
	{
		return Hp;
	}

	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}

	FVector2D& GetForce()
	{
		return Force;
	}

	void SetForce(FVector2D _Force)
	{
		Force = _Force;
	}

protected:
	FVector2D Force = FVector2D::ZERO;

	// 장애물로 기능할 오브젝트
	USpriteRenderer* BodyRenderer = nullptr;
	U2DCollision* BodyCollision = nullptr;
	U2DCollision* BlockingPathCollision = nullptr;
	FVector2D BodyRendererScale = FVector2D::ZERO;
	std::string ObjectName = "";

	bool CanExplode       = false; // 파괴될 수 있는 오브젝트인지
	bool IsTearDamageable = false; // 플레이어 눈물 공격으로 파괴될 수 있는 오브젝트인지
	bool IsBlockingPath   = false;
	bool IsAttackable     = false;
	bool IsDead = false;
	int Hp = 9999; // 플레이어 눈물 공격으로 파괴될 수 있다면 Hp를 가져야 한다.

	class ARoom* ParentRoom = nullptr;
private:

};

