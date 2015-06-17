#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "./CheckboxOption.hpp"

using namespace std;

class Checkbox{
	private:
	sf::Font font_;
	sf::Text label_;
	sf::Text optionDescription_;
	CheckboxOption* value_;
	vector<CheckboxOption*> options_;
	int xPos_, yPos_;
	
	public:
	Checkbox(string, int, int);
	~Checkbox();
	
	string GetValue();
	int GetX();
	int GetY();
	
	void CreateOption(string, sf::Color, string, int, int);
	void RefreshOptionImages();
	CheckboxOption* GetClickedOption(int, int);
	void HandleInput(sf::Event*);
	void Draw(sf::RenderWindow*);


};