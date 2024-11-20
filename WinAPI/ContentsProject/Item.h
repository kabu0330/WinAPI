#pragma once
#include <EngineBase/EngineString.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>

#include "Player.h"
#include "Monster.h"
#include "RoomObject.h"
#include "Global.h"

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
	void Tick(float _DeltaTime) override;
	void CollisionSetting();
	
	void ItemDestroy();
	
	virtual void RemoveRoomData(); // 아이템 습득 시 룸에서 렌더러 삭제
	virtual bool EatFunction(APlayer* _Player) = 0; // 아이템 습득 즉시
	virtual void UseItem(APlayer* _Player) {}; // 아이템 사용
	
	// 드랍, 드랍실패
	void Drop(AActor* _Player);
	void DropSuccess();
	void FailToPickup(class APlayer* _Player);
	void ReverseForce(float _DeltaTime);

	void DropEffect()
	{
		if (false == IsDropEffect)
		{
			return;
		}
		DropEffectRenderer->SetActive(true);
		DropEffectRenderer->ChangeAnimation("StarFlash");
	}

	// 폭탄
	void AreaWideAttack(AActor* _Actor);
	void Knockback(AActor* _Actor);

	void ClampPositionToRoom(); // 방 안으로 이동범위 고정
	FVector2D Reflect(FVector2D _Dir);
	void ChangeAnimation(std::string_view _Name)
	{
		std::string Name = _Name.data();
		DropRenderer->ChangeAnimation(Name);
	}

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

	USpriteRenderer* DropRenderer = nullptr;
	USpriteRenderer* DropEffectRenderer = nullptr;
	USpriteRenderer* BodyRenderer = nullptr;

	USpriteRenderer* HeadRenderer = nullptr; // 얼굴 바뀌는 아이템

	U2DCollision* PlayerCollision = nullptr; // 플레이어와 습득 상호작용을 하려면 반드시 만들어야 함
	U2DCollision* MonsterCollision = nullptr;
	U2DCollision* UniversalCollision = nullptr;

	U2DCollision* ImpactCollision = nullptr; // 플레이어 공격에 밀려날 수 있는 충돌체

	FVector2D BodyRendererScale = FVector2D::ZERO;
	FVector2D BodyRendererLocation = FVector2D::ZERO;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;

	FVector2D Force = FVector2D::ZERO;
	FVector2D DownForce = FVector2D::ZERO;

	bool IsAtBoundary = false;  // 맵 경계면에 위치했냐 -> 반사

	bool IsUseEnd = false;
	bool IsDrop = false; // 맵에서 자신의 정보를 삭제
	bool IsDropEffect = false;

private:

};

