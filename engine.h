#pragma once

#include <window.h>
#include <graphics.h>
#include <utils\memory.h>

template<class _Graphics>
class Engine final
{
public:
	Engine(std::string title, unsigned int width, unsigned int height)
	{
		{// �E�B���h�E�̕\���A�O���t�B�b�N�X�̏�����
			utils::SafeNew(this->window_, title, width, height);
			
			utils::SafeNew<_Graphics>(this->graphics_, this->window_->Hwnd(), this->window_->Width(), this->window_->Height());
		}

		{// ���C�����[�v
			while (this->window_->MessageLoop())
			{
				this->graphics_->Render();
			}
		}
	}
	~Engine(void)
	{
		{// �O���t�B�b�N�X�A�E�B���h�E�̃������J��
			utils::SafeDelete(this->graphics_);

			utils::SafeDelete(this->window_);
		}
	}

private:
	Window * window_ = nullptr;
	Graphics * graphics_ = nullptr;
};