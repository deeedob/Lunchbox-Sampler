#pragma once
#include "abstract_module.hpp"
#include <list>


using namespace lbs;

class ModuleLoad : public AbstractModule
{
	enum class STATE
	{
		
		//MAINSTATES: SEITEN
		LOAD_MODULE_MAIN,     //SAMPLE OR SAMPLEPACK?
		LOAD_MODULE_SUB_1,    //ADD FILE TO FLASH OR REMOVE FILE FROM FLASH
		LOAD_MODULE_SUB_2,      //CHOOSE SAMPLEPACK TO ADD/REMOVE
		LOAD_MODULE_SUB_2b,     //CHOOSE SAMPLE OUT OF SAMPLEPACK TO ADD/REMOVE
		LOAD_MODULE_SUB_3,     //CHOOSE NOTE FOR SAMPLE/ STARTNOTE FOR SAMPLEPACK
		
		//SUBMENU STATES
		YES,
		NO,
	};
public:
	ModuleLoad();
	void enter( Graphics* g ) override;
	void update( Graphics* g, Events::DIGITAL e ) override;
	void exit() override;
private:
	void draw_load_module(Graphics* g);
	void draw_load_module_sub0(Graphics* g);
	void draw_load_module_loadbar(Graphics* g);
	void draw_load_module_sub1(Graphics* g);
	void draw_load_module_sub2(Graphics* g);
	void draw_load_module_sub2b(Graphics* g);
	void draw_load_module_sub3(Graphics* g);
private:
	std::vector <STATE> m_mainStates;
	std::vector <STATE> m_addPack;
	std::vector<STATE> m_loadMenu;
	std::vector<STATE> m_chooseFile;
	std::vector<STATE> m_AddOrRemove;
	std::vector<STATE>::iterator m_current_mainStates;
	std::vector<STATE>::iterator m_current_subStates;
	bool m_isSample = true;
	bool m_isAdd =true;
	std::string bsp_data_Flash[12]={"Sample1","Sample2", "Sample3", "Sample4", "Sample5", "Sample6", "Sample7", "Sample8", "Sample9", "Sample10", "Sample11", "Sample12"};
	std::string bsp_data_Flash2[6][12]={{"SamplePack1","Sample1","Sample2", "Sample3", "Sample4", "Sample5", "Sample6", "Sample7", "Sample8", "Sample9", "Sample10", "Sample11"},{"SamplePack2"}, {"SamplePack3"}, {"SamplePack4"}, {"SamplePack5"},{"SamplePack6"}};
	Window m_topNav;
	Window m_bottom;
	
	int m_key =1;
	int m_selected_sample =0;
	int m_selected_pack =0;
	int m_first_visible_sample =0;
};