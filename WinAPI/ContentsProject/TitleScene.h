#pragma once
#include <EngineCore/Actor.h>
#include <map>
#include <list>

enum class TitleSceneDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

enum class ArrowPos
{
	NONE,
	TOP,
	MID,
	BOT,
	MAX
};

// Ό³Έν :
class ATitleScene : public AActor
{
public:
	// constrcuter destructer
	ATitleScene();
	~ATitleScene();

	// delete Function
	ATitleScene(const ATitleScene& _Other) = delete;
	ATitleScene(ATitleScene&& _Other) noexcept = delete;
	ATitleScene& operator=(const ATitleScene& _Other) = delete;
	ATitleScene& operator=(ATitleScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);

	bool IsLinking(ATitleScene* _Scene);
	bool InterLinkScene(ATitleScene* _Scene, TitleSceneDir _Dir);
	ATitleScene* LinkScene(ATitleScene* _Scene, TitleSceneDir _Dir);

	TitleSceneDir SwitchTitleSceneDir(TitleSceneDir _Dir);
	void SetSceneLocation(ATitleScene* _Scene, TitleSceneDir _Dir);

	void CheckInput();
	void CheckInputArrow();
	void SwitchArrowPos(bool _UpDown);

	void SetFirstScene();
	bool IsLastScene();

	bool MoveToScene(TitleSceneDir _Dir);
	void MoveToCameraInitSetting(ATitleScene*  _Scene, TitleSceneDir _Dir);
	void MoveToCamera(float _DeltaTime);
	bool IsAtDestination();
	void ChangeCurrentScene();

	void ChangeAnimationScene();

protected:

private:
	class USpriteRenderer* TitleRenderer = nullptr;

	std::map<TitleSceneDir, ATitleScene*> Scenes;
	
	static ATitleScene* CurrentScene;
	static ATitleScene* FirstScene;

	FVector2D Force = FVector2D::ZERO;
	FVector2D CameraMoveForce = FVector2D::ZERO;
	FVector2D TargetScenePos = FVector2D::ZERO;
	bool IsCameraMove = false;
	bool IsNextScene = true;

	static class USpriteRenderer* ArrowRenderer;
	ArrowPos CurArrowPos = ArrowPos::TOP;

};

