#pragma once
#include <EngineBase/EngineString.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>

#include "Player.h"
#include "Monster.h"
#include "RoomObject.h"

// ���� : ��� �������� �Ӽ��� �����ϴ� Ŭ����
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
	
	virtual void EatFunction(APlayer* _Player) {}; // ������ ���� ���
	virtual void UseItem() {}; // ������ ���
	
	// ���, �������
	void Drop(AActor* _Player);
	void DropSuccess()
	{
		IsDrop = true;
	}
	void FailToPickup(class APlayer* _Player);
	void ReverseForce(float _DeltaTime);

	// ��ź
	void AreaWideAttack(AActor* _Actor);
	void Knockback(AActor* _Actor);

	void ClampPositionToRoom(); // �� ������ �̵����� ����
	FVector2D Reflect(FVector2D _Dir);
	void ChangeAnimation(std::string_view _Name)
	{
		std::string Name = _Name.data();
		BodyRenderer->ChangeAnimation(Name);
	}

	virtual void IdleAnimation(); // �⺻ �ִϸ��̼�
	void SpawnAnimation(); 
	void HoverAnimation(); // �÷��̾ �������� �����ϰ� ���߿� ��� �ִ� ���
	void GlowAnimation(); // �÷��̾ �������� �����ϰ� ���߿� ��� ���� �� ����� ����Ʈ ȿ��

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

	// ���̳��� ĳ��Ʈ�� ���Ͱ� �÷��̾�, ����, ������Ʈ �� �������� �Ǻ�
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

	U2DCollision* ImpactCollision = nullptr; // �÷��̾� ���ݿ� �з��� �� �ִ� �浹ü

	FVector2D BodyRendererScale = FVector2D::ZERO;
	FVector2D BodyRendererLocation = FVector2D::ZERO;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;

	FVector2D Force = FVector2D::ZERO;

	bool IsAtBoundary = false;  // �� ���鿡 ��ġ�߳� -> �ݻ�

	bool IsUseEnd = false;
	bool IsDrop = false; // �ʿ��� �ڽ��� ������ ����

private:

};

