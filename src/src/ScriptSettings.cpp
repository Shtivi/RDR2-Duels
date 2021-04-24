
#include "Main.h"

SettingsMap* ScriptSettings::settings = NULL;

void ScriptSettings::load(const char* filename, SettingsMap* defaults)
{
	ifstream file;
	string input;
	int count = 0;

	settings = new SettingsMap(*defaults);

	file.open(filename);;
	if (file.is_open())
	{
		log(string("loading settings file: ").append(filename));
		while (!file.eof())
		{
			std::getline(file, input);
			handleLine(&input);
			count++;
		}

		file.close();
		log("settings file loaded");
	}
	else
	{
		log("Unable to load from data file, please make sure it exists.");
	}
}

int ScriptSettings::get(const char* key)
{
	if (settings->find(string(key)) == settings->end())
	{
		return -1;
	}

	return (*settings)[key];
}

bool ScriptSettings::getBool(const char* key)
{
	const int value = get(key);
	return value != -1 && value != 0;
}
	

void ScriptSettings::handleLine(string* line)
{
	if (line->empty())
	{
		return;
	}

	const char firstChar = line->at(0);
	if (firstChar == '/' || firstChar == '\\' || firstChar == '[' || firstChar == ';')
	{
		return;
	}

	vector<string> parts = split(*line, '=');
	if (parts.size() != 2)
	{
		log(string("cannot parse line: ").append(line->c_str()).append(", please make sure it is assembled of the correct format (<setting_name>=<settings_value>)."));
		return;
	}

	string key = parts.at(0);
	if (!is_number(parts.at(1)))
	{
		log(string("cannot parse line: ").append(line->c_str()).append(", value is not a number."));
		return;
	}

	int value = stoi(parts.at(1));
	(*settings)[key] = value;
}