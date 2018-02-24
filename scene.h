#pragma once

#include <component.h>

#include <string>
#include <typeinfo>
#include <vector>
#include <unordered_map>

template<class _Model>
class IScene
{
public:
	virtual ~IScene(void)
	{
		for (auto itr = this->model_list_.begin(); itr != this->model_list_.end();)
		{
			auto model = *itr;
			delete model;
			itr = this->model_list_.erase(itr);
		}
	}

private:
	std::vector<_Model*> model_list_;

public:
	std::vector<_Model*> & ModelList(void)
	{
		return this->model_list_;
	}

public:
	template<class _SuperModel, class ... Args>
	_SuperModel * const Create(Args ... args)
	{
		auto model = new _SuperModel(this, args ...);
		this->model_list_.emplace_back(model);
		return model;
	}

private:
	std::unordered_map<std::string, IComponent<IScene<_Model>>*> component_list_;

public:
	template<class _Component, class ... Args>
	_Component * const Add(Args ... args)
	{
		auto component = new _Component(this, args ...);
		auto name = typeid(_Component).name();
		this->component_list_[name] = component;
		return component;
	}
	template<class _Component, class ... Args>
	_Component * const Get(Args ... args)
	{
		auto name = typeid(_Component).name();
		return static_cast<_Component*>(this->component_list_[name]);
	}

private:
	bool pause_ = false;
	bool running_ = true;

public:
	bool Run(void)
	{
		this->pause_ ? this->Pause() : this->Update();

		this->Always();

		return this->running_;
	}

private:
	void Update(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Update();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}

		for (auto itr = this->model_list_.begin(); itr != this->model_list_.end();)
		{
			auto model = (*itr);
			if (model)
			{
				model->Update();
				++itr;
			}
			else
			{
				delete model;
				itr = this->model_list_.erase(itr);
			}
		}
	}
	void Pause(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Pause();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}

		for (auto itr = this->model_list_.begin(); itr != this->model_list_.end();)
		{
			auto model = (*itr);
			if (model)
			{
				model->Pause();
				++itr;
			}
			else
			{
				delete model;
				itr = this->model_list_.erase(itr);
			}
		}
	}
	void Always(void)
	{
		for (auto itr = this->component_list_.begin(); itr != this->component_list_.end();)
		{
			auto component = (*itr).second;
			if (component)
			{
				component->Always();
				++itr;
			}
			else
			{
				delete component;
				itr = this->component_list_.erase(itr);
			}
		}

		for (auto itr = this->model_list_.begin(); itr != this->model_list_.end();)
		{
			auto model = (*itr);
			if (model)
			{
				model->Always();
				++itr;
			}
			else
			{
				delete model;
				itr = this->model_list_.erase(itr);
			}
		}

		for (auto itr = this->model_list_.begin(); itr != this->model_list_.end();)
		{
			auto model = (*itr);
			if (model)
			{
				model->Adjust();
				++itr;
			}
			else
			{
				delete model;
				itr = this->model_list_.erase(itr);
			}
		}
	}
};