#pragma once

#include <utils\memory.h>

#include <string>

template<class _Window, class _Graphics>
class Engine final
{
public:
	void Begin(std::string title, unsigned int width, unsigned int height)
	{
		{// �E�B���h�E�̕\���A�O���t�B�b�N�X�̏�����
			utils::SafeNew(this->window_, title, width, height);

			utils::SafeNew(this->graphics_, this->window_->Hwnd(), this->window_->Width(), this->window_->Height());
		}
	}

	void Run(void)
	{
		{// ���C�����[�v
			while (this->window_->MessageLoop())
			{
				{
					this->graphics_->Render();
				}
			}
		}
	}

	void End(void)
	{
		{// �O���t�B�b�N�X�A�E�B���h�E�̃������J��
			utils::SafeDelete(this->graphics_);

			utils::SafeDelete(this->window_);
		}
	}

private:
	_Window * window_ = nullptr;
	_Graphics * graphics_ = nullptr;

public:
	_Graphics * const Graphics(void)
	{
		return this->graphics_;
	}
	_Window * const Window(void)
	{
		return this->window_;
	}
};