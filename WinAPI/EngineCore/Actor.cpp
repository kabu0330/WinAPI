#include "PreCompile.h"
#include "Actor.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/EngineAPICore.h>

#include "EngineCoreDebug.h"
#include "EngineSprite.h"
#include "ImageManager.h"
#include "ActorComponent.h"

std::list<UActorComponent*> AActor::ComponentList;

void AActor::ComponentBeginPlay()
{
	{
		std::list<UActorComponent*>::iterator StartIter = ComponentList.begin();
		std::list<UActorComponent*>::iterator EndIter = ComponentList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			UActorComponent* CurActor = *StartIter;
			CurActor->BeginPlay();
		}

		ComponentList.clear();
	}
}

AActor::AActor()
{

}

AActor::~AActor()
{
	// ������Ʈ�� �����ֱ�� ������ �����ֱ�� ����.
	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		UActorComponent* Component = *StartIter;

		if (nullptr != Component)
		{
			delete Component;
		}
	}

	Components.clear();
}

void AActor::Tick(float _DeltaTime)
{
	//auto StartTime = std::chrono::high_resolution_clock::now();

	if (true == IsDebug())
	{
		static float DebugInterval = 0.1f; // ����� ������ �ֱ� (0.1��)
		static float AccumulatedTime = 0.0f;

		AccumulatedTime += _DeltaTime;
		if (AccumulatedTime >= DebugInterval)
		{
			FVector2D Pos = GetActorLocation();
			FVector2D CameraPos = GetWorld()->GetCameraPos();

			FTransform Trans;
			Trans.Location = Pos - CameraPos;
			Trans.Scale = { 6, 6 };

			UEngineDebug::CoreDebugRender(Trans, UEngineDebug::EDebugPosType::Circle);

			AccumulatedTime = 0.0f; // �ð� �ʱ�ȭ
		}
	}

	TimeEventer.Update(_DeltaTime);

	std::list<class UActorComponent*>::iterator StartIter = Components.begin();
	std::list<class UActorComponent*>::iterator EndIter = Components.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (false == (*StartIter)->IsActive())
		{
			continue;
		}
	

		// Component Tick �ð� ����
		//auto ComponentStartTime = std::chrono::high_resolution_clock::now();

		(*StartIter)->ComponentTick(_DeltaTime);

		//auto ComponentEndTime = std::chrono::high_resolution_clock::now();

		//auto ComponentDuration = std::chrono::duration_cast<std::chrono::microseconds>(ComponentEndTime - ComponentStartTime).count();
		//UEngineDebug::OutPutString((*StartIter)->GetName() + " Tick took :" + std::to_string(ComponentDuration) + " microseconds");
	}

	//auto EndTime = std::chrono::high_resolution_clock::now(); // ���� �ð� ����
	//auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count(); // ���� �ð� ���
}

void AActor::ReleaseTimeCheck(float _DeltaTime)
{
	UObject::ReleaseTimeCheck(_DeltaTime);

	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		UActorComponent* Component = *StartIter;
		Component->ReleaseTimeCheck(_DeltaTime);
	}
}

void AActor::ReleaseCheck(float _DeltaTime)
{
	UObject::ReleaseCheck(_DeltaTime);

	// ������Ʈ�� �����ֱ�� ������ �����ֱ�� ���ٰ� �Ѵ�.
	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; )
	{
		UActorComponent* Component = *StartIter;

		if (false == Component->IsDestroy())
		{
			Component->ReleaseCheck(_DeltaTime);
			++StartIter;
			continue;
		}

		// ���ʹ� ���� ������Ʈ�� ������ ��¥ ���δ�.
		delete Component;
		StartIter = Components.erase(StartIter);
	}
}

