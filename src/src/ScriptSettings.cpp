
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
		log("Loading settings file");
		while (!file.eof())
		{
			std::getline(file, input);
			handleLine(&input);
			count++;
		}

		file.close();
		log("settings file loaded");

		for (pair<string, int> i : *settings)
		{
			string msg("");
			msg.append(i.first);
			msg.append(": ");
			msg.append(to_string(i.second));
			log(msg.c_str());
		}
	}
	else
	{
		log("unable to load from data file");
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
		string msg("invalid line: ");
		msg.append(line->c_str());
		log(msg.c_str());
		return;
	}

	string key = parts.at(0);
	if (!is_number(parts.at(1)))
	{
		string msg("invalid line: ");
		msg.append(line->c_str());
		log(msg.c_str());
		return;
	}

	int value = stoi(parts.at(1));
	(*settings)[key] = value;
}