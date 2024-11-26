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
		BodyRenderer->ChangeAnimation(UpperName);
		ObjectName = UpperName; // 눈물 피격에 따라 단계별로 애니메이션이 바뀌어야 한다면 이름이 있어야 함..
	}
	virtual void SetSprite(std::string_view _AnimName, FVector2D _Scale) {};

	void CollisionSetting();
	void Blocker(AActor* _Actor);
	void BlockPlayerCollision(class APlayer* _Player, FVector2D _Pos);
	void BlockMonsterCollision(class AMonster* _Monster, FVector2D _Pos);
	
	// 파괴형 오브젝트
	int ApplyDamaged(AActor* _Actor);

	bool IsDeath();
	virtual void DestroyCollision();
	virtual void DestroyRenderer() {}; // Rock에서 재정의
	virtual void SwitchAnimation();
	

	// Fire
	void DealDamageToPlayer(AActor* _Actor);
	

	// 아이템 드랍
	void SpawnItem();

	
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

	// 맵에 렌더러를 남기고 싶을 때
	USpriteRenderer* GetBodyRenderer()
	{
		return BodyRenderer;
	}

	U2DCollision* GetBodyCollision()
	{
		return BodyCollision;
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

