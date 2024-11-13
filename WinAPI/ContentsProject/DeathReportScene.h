#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ADeathReportScene : public AActor
{
public:
	static ADeathReportScene* DeathReport;
	// constrcuter destructer
	ADeathReportScene();
	~ADeathReportScene();

	// delete Function
	ADeathReportScene(const ADeathReportScene& _Other) = delete;
	ADeathReportScene(ADeathReportScene&& _Other) noexcept = delete;
	ADeathReportScene& operator=(const ADeathReportScene& _Other) = delete;
	ADeathReportScene& operator=(ADeathReportScene&& _Other) noexcept = delete;


	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void ShowDeathReport();
	void CloseDeathReport();

	class USpriteRenderer* GetRenderer()
	{
		return Renderer;
	}

protected:

private:
	class USpriteRenderer* Renderer;
	class USpriteRenderer* RestartRenderer;
	class USpriteRenderer* ExitRenderer;
};

