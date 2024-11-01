#pragma once
#include <filesystem>

// 설명 : 파일 경로, 파일 경로상 이동
class UEnginePath
{
public:
	// constrcuter destructer
	UEnginePath();
	UEnginePath(std::string_view _Path);
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	//// delete Function
	//UEnginePath(const UEnginePath& _Other) = delete;
	//UEnginePath(UEnginePath&& _Other) noexcept = delete;
	//UEnginePath& operator=(const UEnginePath& _Other) = delete;
	//UEnginePath& operator=(UEnginePath&& _Other) noexcept = delete;

	bool IsExists();
	void MoveParent();

	std::string GetPathToString();

	// 파일명 + 확장자 포함
	std::string GetFileName();
	std::string GetDirectoryName();

	std::string GetExtension();

	void Append(std::string_view _AppendName);

	bool MoveParentToDirectory(std::string_view _Path);
	bool IsDirectory();

	bool IsFile();

protected:
	std::filesystem::path Path;

private:

};

