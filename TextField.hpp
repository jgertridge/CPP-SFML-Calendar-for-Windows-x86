#pragma once

#include <iostream>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class TextField{
	private:
	sf::Font font_;
	string type_;
	int xPos_, yPos_;
	sf::Text label_; 
	sf::RectangleShape borderRect_;
	string value_;
	sf::Text valueText_;
	
	
	public:
	TextField(string, string, int, int);
	~TextField();
	
	sf::RectangleShape* GetRect();
	string GetValue();
	
	void HandleInput(sf::Event*);
	void SetValue(string);
	void SetBorder(string);
	void Draw(sf::RenderWindow*);
	
	
};
