#pragma once
#include <EngineBase/Object.h>

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

	// constrcuter destructer
	UActorComponent();
	// ��üȭ ����Ű�� �ϴ¹��� �ϳ�
	virtual ~UActorComponent() = 0;

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

protected:

private:
	class AActor* ParentActor;
};

