#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AMenuScene : public AActor
{
public:
	static AMenuScene* Menu;

	// constrcuter destructer
	AMenuScene();
	~AMenuScene();

	// delete Function
	AMenuScene(const AMenuScene& _Other) = delete;
	AMenuScene(AMenuScene&& _Other) noexcept = delete;
	AMenuScene& operator=(const AMenuScene& _Other) = delete;
	AMenuScene& operator=(AMenuScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SlideInUI();

	void ShowMenu();
	void CloseMenu();

protected:

private:
	class USpriteRenderer* Renderer;
	class USpriteRenderer* PaperRenderer;
	class USpriteRenderer* MyStuffRenderer;
};

