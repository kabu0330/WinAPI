#pragma once
#include "SceneComponent.h"
#include <set>

// 설명 :
class U2DCollision : public USceneComponent
{
public:
	friend class ULevel;

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
	AActor* CollisionOnce(EnumType _OtherCollisionGroup, FVector2D _NextPos = FVector2D::ZERO)
	{

		// 한마리랑 충돌체크
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
		// 최대 42억개와 충돌체크
		std::vector<AActor*> Result;
		Collision(static_cast<int>(_OtherCollisionGroup), Result, _NextDir, -1);

		return Result;
	}

	// 최대 몇마리랑 충돌할거냐
	bool Collision(int _OtherCollisionGroup, std::vector<AActor*>& _Result, FVector2D _NextDir, unsigned int  _Limite);

	void SetCollisionType(ECollisionType _CollisionType)
	{
		CollisionType = _CollisionType;
	}

	ECollisionType GetCollisionType()
	{
		return CollisionType;
	}

	//                                        충돌한 상대
	void SetCollisionEnter(std::function<void(AActor*)> _Function);
	void SetCollisionStay(std::function<void(AActor*)> _Function);
	void SetCollisionEnd(std::function<void(AActor*)> _Function);

protected:

private:
	// 충돌체의 오더는 약간 의미가 다르다.
	// -1 충돌 그룹을 지정해주지 않았다
	// -1 은 사용하면 안된다.
	// 양수만 된다.
	ECollisionType CollisionType = ECollisionType::CirCle;
	int CollisionGroup = -1;


	void CollisionEventCheck(class U2DCollision* _Other);
	std::set<U2DCollision*> CollisionCheckSet;

	std::function<void(AActor*)> Enter;
	std::function<void(AActor*)> Stay;
	std::function<void(AActor*)> End;
};