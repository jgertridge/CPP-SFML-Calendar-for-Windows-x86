#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class Event{
	private:
	string type_;
	string title_;
	int month_;
	int day_;
	int year_;
	sf::Text eventText_;
	
	
	public:
	Event(string, int, int, int); //unique event
	Event(string, int, int); //persisting event
	~Event();
	
	string GetType();
	string GetTitle();
	int GetMonth();
	int GetDay();
	int GetYear();
	sf::Text* GetEventText();
	
	void Draw(sf::RenderWindow*, int);


};