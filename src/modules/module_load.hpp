#pragma once
#include "abstract_module.hpp"
#include <main_memory.hpp>
#include <thread>

using namespace lbs;

class ModuleLoad : public AbstractModule, public Observer<std::pair<uint32_t, uint32_t>>
{
	enum class STATE
	{
		MAIN,
		LOAD,
		PURGE,
		DECIDE,
		
		YES,
		NO,
	};

public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	void draw_main();
	void draw_load();
	void draw_purge();
	void emit( const std::pair<uint32_t, uint32_t>& src ) override;
	void draw_decision();
	void draw_purge_progress( std::pair<uint32_t, uint32_t> progress );
	void draw_load_progress( std::pair<uint32_t, uint32_t> progress );

private:
	TransitionTable<STATE, Events::DIGITAL, void()> m_transitionTable;
	std::vector<String> m_samplePacks;
	std::vector<String> m_samples;
	Graphics* m_graphics;
	unsigned int m_samplePackSelection { 0 };
	bool m_inPurge { false };
	bool m_inLoading { false };
	bool m_return { false };
	
	Window m_top;
	Window m_bottom;
};