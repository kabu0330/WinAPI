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
	// 컴포넌트의 생명주기는 액터의 생명주기와 같다.
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
		static float DebugInterval = 0.1f; // 디버그 렌더링 주기 (0.1초)
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

			AccumulatedTime = 0.0f; // 시간 초기화
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
	

		// Component Tick 시간 측정
		//auto ComponentStartTime = std::chrono::high_resolution_clock::now();

		(*StartIter)->ComponentTick(_DeltaTime);

		//auto ComponentEndTime = std::chrono::high_resolution_clock::now();

		//auto ComponentDuration = std::chrono::duration_cast<std::chrono::microseconds>(ComponentEndTime - ComponentStartTime).count();
		//UEngineDebug::OutPutString((*StartIter)->GetName() + " Tick took :" + std::to_string(ComponentDuration) + " microseconds");
	}

	//auto EndTime = std::chrono::high_resolution_clock::now(); // 종료 시간 측정
	//auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count(); // 실행 시간 계산
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

	// 컴포넌트의 생성주기는 액터의 생명주기와 같다고 한다.
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

		// 액터는 죽을 컴포넌트가 있으면 진짜 죽인다.
		delete Component;
		StartIter = Components.erase(StartIter);
	}
}

