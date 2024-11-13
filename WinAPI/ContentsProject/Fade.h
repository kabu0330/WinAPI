#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AFade : public AActor
{
public:
	static AFade* MainFade;
	// constrcuter destructer
	AFade();
	~AFade();

	// delete Function
	AFade(const AFade& _Other) = delete;
	AFade(AFade&& _Other) noexcept = delete;
	AFade& operator=(const AFade& _Other) = delete;
	AFade& operator=(AFade&& _Other) noexcept = delete;

	void FadeIn();
	void FadeOut();
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	class USpriteRenderer* GetRenderer()
	{
		return Renderer;
	}

protected:

private:
	void LevelChangeStart() override;
	float FadeValue = 0.0f;
	float FadeDir = 1.0f;
	class USpriteRenderer* Renderer;
	void FadeChange();
};

