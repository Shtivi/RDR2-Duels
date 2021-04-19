#pragma once

enum PromptMode
{
	Standard,
	Hold,
	SemiHold
};

class Prompt
{
	public:
		int handle;

	private: 
		bool isEnabled;
		bool isVisible;
		const char* text;
		Hash control;
		Entity targetEntity;
		PromptMode mode;
		bool semiHoldShouldReturn;

	public: 
		Prompt(const char* text, Hash control);
		Prompt(const char* text, Hash control, PromptMode mode);

		Entity getTargetEntity();
		bool getIsEnabled();

		void setText(const char* text);
		void setControl(Hash control);
		void setTargetEntity(Entity entity);
		void setPriority(int priority);
		void setMode(PromptMode mode);
		void setIsEnabled(bool enabled);
		bool isControlPressed();
		void setGroup(int group);
		void attachToCampfire();
		void attachToRestingScenario();

		bool isActivatedByPlayer();
		void show();
		void hide();
		void remove();

	private:
		void update();
};