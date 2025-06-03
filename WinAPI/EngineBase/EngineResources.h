#pragma once
#include "Object.h"
#include "EnginePath.h"

// Ό³Έν : 
class UEngineResources : public UObject
{
public:
	UEngineResources();
	~UEngineResources();

protected:

private:
	UEnginePath Path;

	// delete Function
	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;
};

