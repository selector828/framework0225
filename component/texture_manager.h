#pragma once

#include <d3dx11.h>
#include <d3dx10.h>
#include <string>
#include <unordered_map>

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

class TextureManager
{
public:
	TextureManager(void)
	{
		std::vector<std::string> fileName = GetAllFileNamesInPath("/resource/texture", "png");
		for (auto & name : fileName)
		{
			char fname[255];
			int w, h;
			sscanf(name.c_str(), "%s %d %d", fname, &w, &h);
			D3DX11_IMAGE_INFO dii;
			D3DX11CreateShaderResourceViewFromFileA(this->device_, std::string("resource/texture/" + std::string(name) + ".png").c_str(), NULL, NULL, &this->db_[fname].resource_, NULL);
			D3DX11GetImageInfoFromFileA(std::string("resource/texture/" + std::string(name) + ".png").c_str(), NULL, &dii, NULL);
			if (this->db_[fname].resource_ != nullptr)
			{
				this->db_[fname].file_name_ = fname;
				this->db_[fname].split_.x = (float)w;
				this->db_[fname].split_.y = (float)h;
				this->db_[fname].size_.x = (float)dii.Width / w;
				this->db_[fname].size_.y = (float)dii.Height / h;
			}
		}
	}
	virtual ~TextureManager(void)
	{

	}

private:
};