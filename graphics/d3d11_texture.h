#pragma once

#include <d3dx10.h>
#include <d3dx11.h>
#include <string>
#include <unordered_map>

struct D3D11Texture
{
	static std::unordered_map<std::string, D3D11Texture> & DB(void)
	{
		static std::unordered_map<std::string, D3D11Texture> db_ = {};
		return db_;
	}

	std::string file_name_;
	D3DXVECTOR2 split_;
	D3DXVECTOR2 size_;
	ID3D11ShaderResourceView * resource_;
};

inline std::vector<std::string> GetAllFileNamesInPath(const std::string & dir_name, const std::string & extension)
{
	HANDLE hFind;
	WIN32_FIND_DATAA win32fd;
	std::vector<std::string> file_names;

	char c[255];
	GetCurrentDirectoryA(255, c);
	std::string search_name = std::string(c) + dir_name + "\\*." + extension;

	hFind = FindFirstFileA(search_name.c_str(), &win32fd);

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			std::string str = win32fd.cFileName;
			std::string sub = str.substr(0, strlen(str.c_str()) - strlen(extension.c_str()) - 1);
			file_names.push_back(sub);
		}
	} while (FindNextFileA(hFind, &win32fd));

	FindClose(hFind);

	return file_names;
};