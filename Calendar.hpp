#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "./Day.hpp"
#include "./Event.hpp"
#include "./TextField.hpp"
#include "./Checkbox.hpp"

using namespace std;

class Calendar{
	private:
	static Calendar* instance_;
	sf::RenderWindow window_;
	sf::Event event_;
	sf::Font font_;
	
	time_t currentTime_;
	struct tm* currentDate_;
	
	vector<Day*> month_;
	Day* target_;
	Event* targetEvent_;
	int activeMonth_;
	int activeYear_;
	
	vector<Event*> persistentEvents_;
	vector<Event*> uniqueEvents_;
	
	sf::Text monthLabel_;
	sf::Text daysLabel_;
	
	sf::Texture leftSelectorImage_;
	sf::Texture rightSelectorImage_;
	sf::Sprite leftSelector_;
	sf::Sprite rightSelector_;
	
	sf::Texture dividerImage_;
	sf::Sprite divider_;
	sf::Text selectedDay_;
	
	sf::Texture deleteButtonImage_;
	sf::Sprite deleteButton_;
	sf::Texture addNoteButtonImage_;
	sf::Sprite addNoteButton_;
	
	TextField* targetTextField_;
	TextField* titleField_;
	sf::Text addANote_;
	Checkbox* typeBox_;
	
	Calendar();
	
	string ConvertIntToString(int);
	void CenterText(sf::Text*, int, int, int);
	void UpdateDayColors();
	bool SpriteIncludesPoint(sf::Sprite*, int, int);
	bool RectIncludesPoint(sf::RectangleShape*, int, int);
	
	string AddSpaces(string);
	string RemoveSpaces(string);
	
	void LoadPersistentEvents();
	void LoadUniqueEvents();
	void DeleteEvent(Event*);
	void ConstructMonth();
	void HandleInput(sf::Event*);
	
	void AdvanceMonth();
	void RegressMonth();
	
	void Save();
	
	public:
	~Calendar();
	static Calendar* GetInstance();
	
	sf::Font* GetFont();
	string GetMonthString(int);
	int GetDaysInMonth(int, int);
	Day* GetClickedDay(int, int);
	
	
	void Run();
	
	
};
