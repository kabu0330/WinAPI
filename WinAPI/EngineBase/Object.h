#pragma once
#include <string>

// 설명 : 이름이 필요한 모든 클래스는 UObject를 상속받아야 한다.
class UObject
{
public:
	// constrcuter destructer
	UObject();

	// 많은 곳에서 쓰일 것이기 때문에 혹여나 자식 클래스의 소멸자가 호출되지 않는 위험성을 막기 위해
	// 가상함수로 소멸자를 선언한다.
	virtual ~UObject();

	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;

	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

protected:

private:
	std::string Name;
};

