#include "global_settings.h"
#include <iostream>
#include <fstream>
#include <string>


void GlobalSettings::UpdateValuesFromFile()
{

	std::string key; //The key
	std::string value = "";
	std::string dummy = "";

	settings_.clear();

	std::ifstream myfile("../Resources/Global_Settings.txt");
	if (myfile.is_open())
	{
		while (myfile)
		{
		
			std::getline(myfile, key, '=');		//Read up to the : delimiter into key
			std::getline(myfile, value, '\n');	//Read up to the newline into value
			//std::getline(myfile, dummy, '\n');

			settings_[key] = std::stof(value);

		}
		myfile.close();
	}
	else std::cout << "Unable to open file";


	std::cout << "Updated settings..." << std::endl;
	for (const auto &p : settings_)
	{
		std::cout << p.first << '\t' << p.second << std::endl;
	}
	std::cout << "SETTINGS SIZE: " <<settings_.size() << std::endl;
}

float GlobalSettings::ValueOf(std::string setting_name)
{
	// Not sure if it's costly to check this each time.
	if (settings_.find(setting_name) == settings_.end())
	{
		std::cout << "ERROR: " << setting_name << " was not found. Check spelling." << std::endl;
		return 0.0f;
	}
	float returnValue = settings_[setting_name];
	return returnValue;
}

void GlobalSettings::SeedRandomSeed() {
	srand(static_cast<unsigned>(time(NULL)));
}

int GlobalSettings::GetRandomInt() {
	return rand();
}

GlobalSettings::~GlobalSettings()
{
}
