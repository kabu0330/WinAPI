#pragma once
#include <EngineBase/EngineString.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>

#include "Player.h"
#include "Monster.h"
#include "RoomObject.h"

// 설명 : 모든 아이템의 속성을 정의하는 클래스
class AItem : public AActor
{
public:
	// constrcuter destructer
	AItem();
	~AItem();

	// delete Function
	AItem(const AItem& _Other) = delete;
	AItem(AItem&& _Other) noexcept = delete;
	AItem& operator=(const AItem& _Other) = delete;
	AItem& operator=(AItem&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);
	void CollisionSetting();
	
	void RemoveRoomData();
	
	virtual void EatFunction(APlayer* _Player) {}; // 아이템 습득 즉시
	virtual void UseItem() {}; // 아이템 사용
	
	// 드랍, 드랍실패
	void Drop(AActor* _Player);
	void DropSuccess()
	{
		IsDrop = true;
	}
	void FailToPickup(class APlayer* _Player);
	void ReverseForce(float _DeltaTime);

	// 폭탄
	void AreaWideAttack(AActor* _Actor);
	void Knockback(AActor* _Actor);

	void ClampPositionToRoom(); // 방 안으로 이동범위 고정
	FVector2D Reflect(FVector2D _Dir);
	void ChangeAnimation(std::string_view _Name)
	{
		std::string Name = _Name.data();
		BodyRenderer->ChangeAnimation(Name);
	}

	virtual void IdleAnimation(); // 기본 애니메이션
	void SpawnAnimation(); 
	void HoverAnimation(); // 플레이어가 아이템을 습득하고 공중에 들고 있는 경우
	void GlowAnimation(); // 플레이어가 아이템을 습득하고 공중에 들고 있을 때 출력할 이펙트 효과

	void SetParentRoom(class ARoom* _Parent)
	{
		ParentRoom = _Parent;
	}

	FVector2D GetForce() const
	{
		return Force;
	}

	void SetForce(FVector2D _Force)
	{
		Force = _Force;
	}

	// 다이나믹 캐스트로 액터가 플레이어, 몬스터, 오브젝트 중 무엇인지 판별
	template<typename ActorType>
	ActorType* CastActorToType(AActor* _Actor)
	{
		ActorType* Actor = dynamic_cast<ActorType*>(_Actor);
		if (nullptr != Actor)
		{
			return Actor;
		}				
		return nullptr;
	}


protected:
	int ItemCount = 0;
	int Att = 0;

	float TimeElapsed = 0.0f;
	class ARoom* ParentRoom = nullptr;

	USpriteRenderer* BodyRenderer = nullptr;

	U2DCollision* PlayerCollision = nullptr;
	U2DCollision* MonsterCollision = nullptr;
	U2DCollision* UniversalCollision = nullptr;

	U2DCollision* ImpactCollision = nullptr; // 플레이어 공격에 밀려날 수 있는 충돌체

	FVector2D BodyRendererScale = FVector2D::ZERO;
	FVector2D BodyRendererLocation = FVector2D::ZERO;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;

	FVector2D Force = FVector2D::ZERO;

	bool IsAtBoundary = false;  // 맵 경계면에 위치했냐 -> 반사

	bool IsUseEnd = false;
	bool IsDrop = false; // 맵에서 자신의 정보를 삭제

private:

};

