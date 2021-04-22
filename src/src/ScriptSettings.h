#pragma once

typedef std::map<std::string, int> SettingsMap;

class ScriptSettings
{
private:
	static SettingsMap* settings;

public:
	static void load(const char* filename, SettingsMap* defaults);
	static int get(const char* key);
	static bool getBool(const char* key);

private:
	static void handleLine(string* line);
};