#pragma once
#include "SceneComponent.h"

// 설명 :
class U2DCollision : public USceneComponent
{
public:
	// constrcuter destructer
	U2DCollision();
	~U2DCollision();

	// delete Function
	U2DCollision(const U2DCollision& _Other) = delete;
	U2DCollision(U2DCollision&& _Other) noexcept = delete;
	U2DCollision& operator=(const U2DCollision& _Other) = delete;
	U2DCollision& operator=(U2DCollision&& _Other) noexcept = delete;

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
	AActor* CollisionOnce(EnumType _OtherCollisionGroup)
	{

		// 한마리랑 충돌체크
		std::vector<AActor*> Result;
		Collision(static_cast<int>(_OtherCollisionGroup), Result, 1);

		if (true == Result.empty())
		{
			return nullptr;
		}

		return Result[0];
	}

	template<typename EnumType>
	std::vector<AActor*> CollisionAll(EnumType _OtherCollisionGroup)
	{
		// 최대 42억개와 충돌체크
		std::vector<AActor*> Result;
		Collision(static_cast<int>(_OtherCollisionGroup), Result, -1);

		return Result;
	}

	// 최대 몇마리랑 충돌할거냐
	bool Collision(int _OtherCollisionGroup, std::vector<AActor*>& _Result, unsigned int  _Limite);

	void SetCollisionType(ECollisionType _CollisionType)
	{
		CollisionType = _CollisionType;
	}

protected:

private:
	// 충돌체의 오더는 약간 의미가 다르다.
	// -1 충돌 그룹을 지정해주지 않았다
	// -1 은 사용하면 안된다.
	// 양수만 된다.
	ECollisionType CollisionType = ECollisionType::CirCle;
	int CollisionGroup = -1;
};