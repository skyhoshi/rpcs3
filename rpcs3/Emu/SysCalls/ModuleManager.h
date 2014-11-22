#pragma once
#include "Modules.h"

class ModuleManager
{
	Module* m_modules[3][0xff];//keep pointer to modules split in 3 categories according to their id
	uint m_max_module_id; //max index in m_modules[2][], m_modules[1][] and m_modules[0][]
	uint m_module_2_count; //max index in m_modules[2][]
	std::mutex m_funcs_lock;
	std::vector<ModuleFunc *> m_modules_funcs_list;
	std::vector<Module> m_mod_init; //owner of Module
	bool initialized;
	std::unordered_map<u32, u32> m_registered_functions;

public:
	ModuleManager();
	~ModuleManager();

	void init();
	void AddFunc(ModuleFunc *func);
	void SetModule(u16 id, Module* module);//insert into m_modules
	bool IsLoadedFunc(u32 id) const;
	bool CallFunc(PPUThread& CPU, u32 num);
	bool UnloadFunc(u32 id);
	void UnloadModules();
	u32 GetFuncNumById(u32 id);
	Module* GetModuleByName(const std::string& name);
	Module* GetModuleById(u16 id);

	void register_function(u32 fnid, u32 fstub)
	{
		m_registered_functions[fnid] = fstub;
	}

	bool get_function_stub(u32 fnid, be_t<u32>& res)
	{
		auto f = m_registered_functions.find(fnid);

		if (f == m_registered_functions.end())
			return false;

		res = f->second;
		return true;
	}
};