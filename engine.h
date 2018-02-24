#pragma once

#include <scene.h>

#include <utils\memory.h>

#include <string>

template<class _Window, class _Graphics>
class Engine final
{
public:
	void Begin(std::string title, unsigned int width, unsigned int height)
	{
		{// ウィンドウの表示、グラフィックスの初期化
			utils::SafeNew(this->window_, title, width, height);

			utils::SafeNew(this->graphics_, this->window_->Hwnd(), this->window_->Width(), this->window_->Height());
		}
	}

	void Run(void)
	{
		{// メインループ
			while (this->window_->MessageLoop() && this->scene_ && this->scene_->Run())
			{
				{//描画
					this->graphics_->Render(this->scene_);
				}
			}
		}
	}

	void End(void)
	{
		{// シーン、グラフィックス、ウィンドウのメモリ開放
			utils::SafeDelete(this->scene_);

			utils::SafeDelete(this->graphics_);

			utils::SafeDelete(this->window_);
		}
	}

private:
	_Window * window_ = nullptr;
	_Graphics * graphics_ = nullptr;
	IScene<typename _Graphics::model_type> * scene_ = nullptr;

public:
	template<class _Scene, class ... Args>
	void SetCurrentScene(Args ... args)
	{
		utils::SafeDelete(this->scene_);
		this->scene_ = new _Scene(args ...);
	}

public:
	_Window * const Window(void)
	{
		return this->window_;
	}
	_Graphics * const Graphics(void)
	{
		return this->graphics_;
	}
	IScene<typename _Graphics::model_type> * const Scene(void)
	{
		return this->scene_;
	}
};