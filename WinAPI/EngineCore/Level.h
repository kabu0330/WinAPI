#pragma once
#include "GameMode.h"

// ���� : ȭ�鿡 ��µ� ��� ����(+������Ʈ)��� ī�޶� �����ϴ� Ŭ����
// Ÿ��Ʋ ����, �÷��� ����, ���� ���� �� ȭ�鿡 ��µ� ���͵��� �����Ѵ�.
class ULevel
{
public:
	friend class USpriteRenderer; // �������� ��� ����� �� �Ѱ��ش�.
	friend class UEngineAPICore;

	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	// ���� CurLevel ������
	void LevelChangeStart();

	// �� ���� ���ο� ������ �ٲ�ž�.
	void LevelChangeEnd();

	void Tick(float _DeltaTime);
	void Render(float _DeltaTime);

	template<typename ActorType>
	ActorType* SpawnActor()
	{
		ActorType* NewActor = new ActorType();
		AActor* ActorPtr = dynamic_cast<AActor*>(NewActor);
		ActorPtr->World = this;

		BeginPlayList.push_back(ActorPtr);
		// NewActor->BeginPlay();
		// AllActors.push_back(NewActor);
		return NewActor;
	}

	void SetCameraToMainPawn(bool _IsCameraToMainPawn)
	{
		IsCameraToMainPawn = _IsCameraToMainPawn;
	}

	void SetCameraPivot(FVector2D _Pivot)
	{
		CameraPivot = _Pivot;
	}

	void SetCameraPos(FVector2D _Pos)
	{
		CameraPos = _Pos;
	}

	FVector2D GetCameraPivot()
	{
		return CameraPivot;
	}

	FVector2D GetCameraPos()
	{
		return CameraPos;
	}

	void AddCameraPos(FVector2D _Pos)
	{
		CameraPos += _Pos;
	}

	AActor* GetPlayer()
	{
		return MainPawn;
	}

protected:

private:
	void ScreenClear();
	void DoubleBuffering();

	template<typename GameModeType, typename MainPawnType>
	void CreateGameMode()
	{
		// ������ ������ ��Ģ�� ������ ���� ���
		GameMode = new GameModeType();

		// �÷��̾�
		MainPawn = new MainPawnType();

		// ULevel�� ������ ���� '����'��
		MainPawn->World = this;
		GameMode->World = this;

		// �ʱ� ����
		BeginPlayList.push_back(GameMode);
		BeginPlayList.push_back(MainPawn);

		//GameMode->BeginPlay();
		//MainPawn->BeginPlay();
		//AllActors.push_back(GameMode);
		//AllActors.push_back(MainPawn);
	}

	// �������� ���������� �Ժη� ȣ������ ���ϰ� �ϱ� ���ؼ� private�� �־�� �Ѵ�.
	void PushRenderer(class USpriteRenderer* _Renderer);
	void ChangeRenderOrder(class USpriteRenderer* _Renderer, int _PrevOrder);

	AGameMode* GameMode = nullptr;

	AActor* MainPawn = nullptr;
	std::list<AActor*> AllActors;

	std::list<AActor*> BeginPlayList;

	// �������Ѵ�. : ���ʿ��� ������ ���� �ʵ��� Key ���� int�� map, ������� �������
	std::map<int, std::list<class USpriteRenderer*>> Renderers;

	// ī�޶�
	bool IsCameraToMainPawn = true;
	FVector2D CameraPos;
	FVector2D CameraPivot;
};

