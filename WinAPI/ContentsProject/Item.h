#pragma once
#include <EngineBase/EngineString.h>
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>

#include "Player.h"
#include "Monster.h"
#include "RoomObject.h"
#include "Global.h"

enum class EItemType
{
	NONE,
	PASSIVE, // ���� ȿ���� �ٲ� ������
	USE,     // ���� �Բ� ��� �Ҹ�
	MAX
};

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
	void Tick(float _DeltaTime) override;
	void CollisionSetting();
	void Move(float _DeltaTIme);
	
	void ItemDestroy();
	
	virtual void RemoveRoomData(); // ������ ���� �� �뿡�� ������ ����
	virtual bool EatFunction(APlayer* _Player) = 0; // ������ ���� ���
	virtual void UseItem(APlayer* _Player) {}; // ������ ���
	
	// ���, �������
	void Drop(AActor* _Player);
	void DropSuccess();
	void FailToPickup(class APlayer* _Player);
	void ReverseForce(float _DeltaTime);
	void SetLocation();


	virtual void DropEffect()
	{
		if (false == IsDropEffect)
		{
			return;
		}
		DropEffectRenderer->SetActive(true);
		DropEffectRenderer->ChangeAnimation("StarFlash");
	}

	// Heart
	virtual void SetHalfHeart() {};

	// ��ź
	void AreaWideAttack(AActor* _Actor);
	void Knockback(AActor* _Actor);

	// Tear�� �ٲٴ� �нú� ������
	virtual void TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed) {}; // �ڽ�Ŭ�������� ������

	void ClampPositionToRoom(); // �� ������ �̵����� ����
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
	bool IsPushBackItems() const
	{
		return IsPushback;
	}

	bool IsOwned()
	{
		return IsOwnedByPlayer;
	}

	EItemType GetItemType() const
	{
		return ItemType;
	}

protected:
	EItemType ItemType = EItemType::NONE;
	int ItemCount = 0;
	int Att = 0;

	float DropAnimationDuration = 3.0f;

	float TimeElapsed = 0.0f;
	class ARoom* ParentRoom = nullptr;

	USpriteRenderer* DropRenderer = nullptr;
	USpriteRenderer* DropEffectRenderer = nullptr;
	USpriteRenderer* BodyRenderer = nullptr;

	USpriteRenderer* HeadRenderer = nullptr; // �� �ٲ�� ������


	U2DCollision* PlayerCollision = nullptr; // �÷��̾�� ���� ��ȣ�ۿ��� �Ϸ��� �ݵ�� ������ ��
	U2DCollision* MonsterCollision = nullptr;
	U2DCollision* UniversalCollision = nullptr;

	U2DCollision* ImpactCollision = nullptr; // �÷��̾� ���ݿ� �з��� �� �ִ� �浹ü

	FVector2D BodyRendererScale = FVector2D::ZERO;
	FVector2D BodyRendererLocation = FVector2D::ZERO;
	FVector2D BodyCollisionScale = FVector2D::ZERO;
	FVector2D BodyCollisionLocation = FVector2D::ZERO;

	FVector2D Force = FVector2D::ZERO;
	FVector2D DropLocation = FVector2D::ZERO;
	FVector2D Direction = FVector2D::UP;

	// EatMotionSpeed
	FVector2D HoverForce = FVector2D(0.0f, -280.0f);
	FVector2D Gravity = FVector2D(0.0f, 120.0f);

	APlayer* Player = nullptr;
	bool IsAtBoundary = false;  // �� ���鿡 ��ġ�߳� -> �ݻ�

	bool IsUseEnd = false;
	bool IsDrop = false; // �ʿ��� �ڽ��� ������ ����
	bool IsDropEffect = false; // �÷��̾� ���� ��½ ���ø��� �����۸� �ش�
	bool IsPushback = true; // �÷��̾� ������ ����Ʈ�� �� ������.
	bool IsMove = false; // �ʿ� �������� �� �� �Ʒ��� �����ϰų�
	bool IsOwnedByPlayer = false; // ��ġ �÷��̾����� ����

private:

};

