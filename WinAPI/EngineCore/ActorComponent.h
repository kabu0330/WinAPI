#pragma once
#include <map>
#include <EngineBase/Object.h>
#include <EngineBase/EngineDelegate.h>
#include "EngineCoreDebug.h"
#include "EngineSprite.h"

// "�÷��̾��� ��ġ�� �������� ��� �ִ�."�� ǥ���ϴ� ��ɵ��� �𸮾󿡼��� SceneComponent��� �Ѵ�. (HP Bar, ���� ��)
// ��ġ�� ������ �ʴ� ActorComponent�� �ִ�. (��ã�� ���, ���� ���, ����Ʈ, ��ȭ ��)
// Component								: Actor�� ���� ���
// ActorComponent : public Object			: ��ġ�� �ʿ� ����		
// SceneComponent : public ActorComponent	: ��ġ�� �ʿ���
// Renderer		  : public SceneComponent	: �̹��� ����
// 2DCollision	  : public SceneComponent	: �̹��� ����
// �̷ν� Actor�� ���� ���� �̹����� ���� �� �ְ� �ȴ�.

// ���� : Actor�� ���� ���(�̹���, �浹ü)
class UActorComponent : public UObject
{
public:
	// ���͸��� ParentActor �����Ҽ� �ְ� �����ϱ� ���ؼ� �Ʒ��� ���� §��.
	friend class AActor;
	typedef UActorComponent Super;

	UActorComponent();
	virtual ~UActorComponent() = 0; // ��üȭ ����Ű�� �ϴ¹� �� �ϳ�

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	virtual void BeginPlay() {}

	virtual void ComponentTick(float _DeltaTime) {}

	class AActor* GetActor()
	{
		return ParentActor;
	}

	bool IsActive() override
	{
		// �������� �ڽ��� ���� ���Ϳ��� ���ӵȴ�.
		// �θ�        true            true
		return UObject::IsActive() && GetActor()->IsActive();
	}

	bool IsDestroy() override
	{
		// �θ�        true            true
		return UObject::IsDestroy() || GetActor()->IsDestroy();
	}

protected:

private:
	class AActor* ParentActor;
};

