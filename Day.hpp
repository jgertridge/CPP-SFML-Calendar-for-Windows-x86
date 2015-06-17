#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "./Event.hpp"

using namespace std;

class Day{
	private:
	sf::RectangleShape dayRect_;
	sf::Text numText_;
	int xPos_, yPos_;
	int number_;
	vector<sf::Text*> dayContent_;
	string type_;
	bool selected_;
	
	vector<Event*> events_;
	
	public:
	Day(int, int, sf::Font*);
	~Day();
	
	int GetX();
	int GetY();
	int GetNumber();
	string GetType();
	
	vector<Event*>* GetEvents();
	Event* GetClickedEvent(int, int);
	
	void SetNumber(int);
	void SetType(string);
	void SetSelected(bool);
	void AddEvent(Event*);
	bool DeleteEvent(Event*);
	void RefreshContent();
	void ClearContent();
	void UpdateEventTextColor(Event*);
	void Draw(sf::RenderWindow*);
	void DrawEvents(sf::RenderWindow*);


};