#pragma once
#include <Windows.h>
#include <EnginePlatform/EngineWindow.h>

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
// ���� : â ũ�� ����
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

	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	void CreateLevel(std::string_view _LevelName)
	{
		ULevel* NewLevel = new ULevel();

	}

protected:

private:
	static void EngineBeginPlay();
	static void EngineTick();
	static UEngineAPICore* MainCore;
	static UContentsCore* UserCore;
	

	UEngineWindow EngineMainWindow;
	std::map<std::string, class ULevel*> Levels;


	void Tick();
	void Render();
};

// ������ ����� ���������� ���� �ʹٸ�
// 
// 1. �Լ�������
// 2. �����Լ�
