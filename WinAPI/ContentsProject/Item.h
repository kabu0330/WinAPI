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
	
	// 드랍, 드랍실패
	void CollisionSetting();
	void Drop(AActor* _Player);
	void FailToPickup(class APlayer* _Player);
	void ReverseForce(float _DeltaTime);

	// 폭탄
	void AreaWideAttack(AActor* _Actor);

	void ClampPositionToRoom(); // 방 안으로 이동범위 고정
	FVector2D Reflect(FVector2D _Dir);
	void ChangeAnimation(std::string_view _Name)
	{
		std::string Name = _Name.data();
		BodyRenderer->ChangeAnimation(Name);
	}

	virtual void IdleAnimation(); // 기본 애니메이션
	void HoverAnimation(); // 플레이어가 아이템을 습득하고 공중에 들고 있는 경우
	void GlowAnimation(); // 플레이어가 아이템을 습득하고 공중에 들고 있을 때 출력할 이펙트 효과

	void SetParentRoom(class ARoom* _Parent)
	{
		ParentRoom = _Parent;
	}

	FVector2D& GetForce()
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
	float TimeElapsed = 0.0f;
	class ARoom* ParentRoom = nullptr;

	USpriteRenderer* BodyRenderer = nullptr;
	USpriteRenderer* EffectRenderer = nullptr;

	U2DCollision* PlayerCollision = nullptr;
	U2DCollision* MonsterCollision = nullptr;
	U2DCollision* UniversalCollision = nullptr;

	FVector2D BodyRendererScale = FVector2D::ZERO;
	FVector2D BodyRendererLocation = FVector2D::ZERO;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;

	FVector2D Force = FVector2D::ZERO;

	bool IsAtBoundary = false;
private:

};

