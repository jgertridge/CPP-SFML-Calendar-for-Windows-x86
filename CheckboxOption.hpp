#pragma once

#include <iostream>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class CheckboxOption{
	private:
	sf::RectangleShape box_;
	int xPos_, yPos_;
	sf::Text label_;
	string description_;
	
	
	public:
	CheckboxOption(string, sf::Color, string, sf::Font*);
	~CheckboxOption();

	string GetLabel();
	string GetDescription();
	sf::RectangleShape* GetRect();
	int GetWidth();
	
	void SetPosition(int, int);
	void SetState(string);
	void Draw(sf::RenderWindow*);

};