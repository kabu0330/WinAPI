#pragma once
#include "SceneComponent.h"
#include <set>

// ���� :
class U2DCollision : public USceneComponent
{
public:
	friend class ULevel;

	U2DCollision();
	~U2DCollision();

	void BeginPlay() override;

	void ComponentTick(float _DeltaTime) override;

	template<typename EnumType>
	EnumType GetGroup()
	{
		return static_cast<EnumType>(CollisionGroup);
	}

	int GetGroup()
	{
		return CollisionGroup;
	}


	template<typename EnumType>
	void SetCollisionGroup(EnumType _CollisionGroup)
	{
		SetCollisionGroup(static_cast<int>(_CollisionGroup));
	}

	void SetCollisionGroup(int _CollisionGroup)
	{
		CollisionGroup = _CollisionGroup;
	}

	template<typename EnumType>
	AActor* CollisionOnce(EnumType _OtherCollisionGroup, FVector2D _NextPos = FVector2D::ZERO)
	{
		// �Ѹ����� �浹üũ
		std::vector<AActor*> Result;
		Collision(static_cast<int>(_OtherCollisionGroup), Result, _NextPos, 1);

		if (true == Result.empty())
		{
			return nullptr;
		}

		return Result[0];
	}

	template<typename EnumType>
	std::vector<AActor*> CollisionAll(EnumType _OtherCollisionGroup, FVector2D _NextDir)
	{
		// �ִ� 42�ﰳ�� �浹üũ
		std::vector<AActor*> Result;
		Collision(static_cast<int>(_OtherCollisionGroup), Result, _NextDir, -1);

		return Result;
	}

	// �ִ� ����� �浹�Ұų�
	bool Collision(int _OtherCollisionGroup, std::vector<AActor*>& _Result, FVector2D _NextDir, unsigned int  _Limite);

	void SetCollisionType(ECollisionType _CollisionType)
	{
		CollisionType = _CollisionType;
	}

	ECollisionType GetCollisionType()
	{
		return CollisionType;
	}

	void SetCameraEffect(bool _IsCameraEffect)
	{
		IsCameraEffect = _IsCameraEffect;
	}

	//                                        �浹�� ���
	void SetCollisionEnter(std::function<void(AActor*)> _Function);
	void SetCollisionStay(std::function<void(AActor*)> _Function);
	void SetCollisionEnd(std::function<void(AActor*)> _Function);

protected:

private:
	void CollisionSetRelease();

	ECollisionType CollisionType = ECollisionType::Circle;
	int CollisionGroup = -1;


	void CollisionEventCheck(class U2DCollision* _Other);
	std::set<U2DCollision*> CollisionCheckSet;

	bool IsCameraEffect = true;

	std::function<void(AActor*)> Enter;
	std::function<void(AActor*)> Stay;
	std::function<void(AActor*)> End;

	// delete Function
	U2DCollision(const U2DCollision& _Other) = delete;
	U2DCollision(U2DCollision&& _Other) noexcept = delete;
	U2DCollision& operator=(const U2DCollision& _Other) = delete;
	U2DCollision& operator=(U2DCollision&& _Other) noexcept = delete;
};