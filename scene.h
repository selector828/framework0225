#pragma once

#include <vector>

template<class _Model>
class Scene
{
private:
	std::vector<_Model*> model_list_;

public:
	std::vector<_Model*> & ModelList(void)
	{
		return this->model_list_;
	}

public:
	template<class _SuperModel, class ... Args>
	_SuperModel * const AddModel(Args ... args)
	{
		auto model = new _SuperModel(args ...);
		this->model_list_.emplace_back(model);
		return model;
	}
};