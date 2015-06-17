#include "./CheckboxOption.hpp"

CheckboxOption::CheckboxOption(string label, sf::Color color, string description, sf::Font* font){
	box_.setPosition(xPos_ + label_.getGlobalBounds().width, yPos_);
	box_.setSize(sf::Vector2f(16, 16));
	box_.setOutlineThickness(2);
	box_.setOutlineColor(sf::Color(000, 000, 000));
	
	label_.setString(label);
	label_.setFont(*font);
	label_.setCharacterSize(16);
	label_.setColor(color);
	
	description_ = description;
}

CheckboxOption::~CheckboxOption(){

}

string CheckboxOption::GetLabel(){return label_.getString();}
string CheckboxOption::GetDescription(){return description_;}
sf::RectangleShape* CheckboxOption::GetRect(){return &box_;}

int CheckboxOption::GetWidth(){
	return label_.getGlobalBounds().width + 22;
}

void CheckboxOption::SetPosition(int x, int y){
	xPos_ = x;
	yPos_ = y;
	
	label_.setPosition(xPos_, yPos_ + 2);
	box_.setPosition(xPos_ + label_.getGlobalBounds().width + 2, yPos_ + 2);
}

void CheckboxOption::SetState(string state){
	if(state == "unchecked"){
		box_.setFillColor(sf::Color(255, 255, 255));
	} else
	if(state == "checked"){
		box_.setFillColor(sf::Color(98, 220, 123));
	}
}

void CheckboxOption::Draw(sf::RenderWindow* window){
	window->draw(label_);
	window->draw(box_);
}