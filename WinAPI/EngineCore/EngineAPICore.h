#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/UEngineTimer.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

#include "Level.h"

// ���� : ���� ������Ʈ���� �����Լ��� ������ ��û
class UContentsCore
{
public:
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
};

// ���� : ���� ����
// ���� : ������ ����, ���� ����, ������ Tick/Render ����
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

		// ���� ��尡 ������ Ư���� �����ϴ� �߿��� ��ü
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// ������ string���� �����ϰ� string���� ȣ���Ѵ�.
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
	UEngineWindow EngineMainWindow = UEngineWindow(); // ���� ���� ������

	// ������� ��� ����
	std::map<std::string, class ULevel*> Levels; 

	// ���� ���� ������ ���� �־�� �ϴ� ����
	// ȭ�� ��ȯ�� ������ ü���� ������� ���� ��ȯ
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	void Tick();
};

// ������ ����� ���������� ���� �ʹٸ�
// 
// 1. �Լ�������
// 2. �����Լ�
