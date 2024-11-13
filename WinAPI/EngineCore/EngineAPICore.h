#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/UEngineTimer.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

#include "Level.h"

// 설명 : 하위 프로젝트에서 가상함수로 엔진에 요청
class UContentsCore
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
};

// 설명 : 게임 엔진
// 설정 : 윈도우 생성, 레벨 관리, 컨텐츠 Tick/Render 루프
class UEngineAPICore
{
public:
	// constructer destructer
	UEngineAPICore();
	~UEngineAPICore();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;
	//

	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	float GetDeltaTime()
	{
		return DeltaTimer.GetDeltaTime();
	}

	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		ULevel* NewLevel = new ULevel();

		// 게임 모드가 레벨의 특성을 설정하는 중요한 객체
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// 레벨을 string으로 저장하고 string으로 호출한다.
		Levels.insert({ _LevelName.data(), NewLevel });

		return NewLevel;
	}

	void OpenLevel(std::string_view _LevelName);

protected:

private:
	static void EngineBeginPlay();
	static void EngineTick();
	static UEngineAPICore* MainCore;
	static UContentsCore* UserCore;
	
	UEngineTimer  DeltaTimer       = UEngineTimer();
	UEngineWindow EngineMainWindow = UEngineWindow(); // 엔진 메인 윈도우

	// 만들어진 모든 레벨
	std::map<std::string, class ULevel*> Levels; 

	// 현재 내가 눈으로 보고 있어야 하는 레벨
	// 화면 전환시 포인터 체인지 방식으로 레벨 전환
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	void Tick();
};

// 엔진의 기능을 컨텐츠에서 쓰고 싶다면
// 
// 1. 함수포인터
// 2. 가상함수
