#include "PreCompile.h"
#include "EnginePath.h"
#include <EngineBase/EngineDebug.h>

UEnginePath::UEnginePath()
	: Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path)
	: Path(_Path)
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path)
	: Path(_Path)
{
}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
	return Path.string();
}

std::string UEnginePath::GetFileName()
{
	if (true == IsDirectory())
	{
		MSGASSERT("파일 경로일때만 GetFileName을 호출할수 있습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("디렉토리 경로일때만 GetDirectoryName을 호출할수 있습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetExtension()
{
	return Path.extension().string();
}

void UEnginePath::Append(std::string_view _AppendName)
{
	Path.append(_AppendName);
}

bool UEnginePath::IsExists()
{
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로일때만 'Move Parent To Directory' 함수를 호출할 수 있습니다");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;

	// root_path : 드라이브 위치(C:\\ 이나 D:\\과 같이 '내 컴퓨터'에서 보이는) 
	std::filesystem::path Root = CurPath.root_path();
	while (true)
	{
		CurPath = DummyPath.Path;

		// 무한루프 탈출 조건 append에 도달하기 전에 탈출
		if (CurPath == Root)
		{
			break;
		}

		CurPath.append(_Path); // 현재 경로에서 _Path(Resource) 폴더명을 추가하여 검사
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath; // 찾은 경로를 멤버에 저장
			break;
		}
		DummyPath.MoveParent(); // 찾는 폴더가 없으면 상위 폴더로 이동
	}

	return Result; // 찾고자 하는 폴더를 찾으면 true 반환
}
