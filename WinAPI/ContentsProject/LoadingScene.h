#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ALoadingScene : public AActor
{
public:
	// constrcuter destructer
	ALoadingScene();
	~ALoadingScene();

	// delete Function
	ALoadingScene(const ALoadingScene& _Other) = delete;
	ALoadingScene(ALoadingScene&& _Other) noexcept = delete;
	ALoadingScene& operator=(const ALoadingScene& _Other) = delete;
	ALoadingScene& operator=(ALoadingScene&& _Other) noexcept = delete;

	void RandomImage();
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void FadeOut();
	class USpriteRenderer* GetRenderer()
	{
		return Renderer;
	}

protected:

private:
	class USpriteRenderer* Renderer;
	float FadeValue = 0.0f;
	float FadeDir = 1.0f;
	void FadeChange();
};

