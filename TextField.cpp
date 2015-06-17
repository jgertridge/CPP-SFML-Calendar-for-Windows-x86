#include "./TextField.hpp"
#include "./Calendar.hpp"

TextField::TextField(string type, string label, int x, int y){
	font_.loadFromFile("./Data/calibri.ttf");

	type_ = type;
	
	xPos_ = x;
	yPos_ = y;
	
	label_.setString(label);
	label_.setPosition(xPos_, yPos_);
	label_.setFont(font_);
	label_.setCharacterSize(18);
	label_.setColor(sf::Color::Black);
	
	borderRect_.setPosition(xPos_ + label_.getGlobalBounds().width, yPos_);
	borderRect_.setSize(sf::Vector2f(330, 20));
	borderRect_.setOutlineThickness(2);
	borderRect_.setOutlineColor(sf::Color(000, 000, 000));
	
	value_ = "";
	valueText_.setString("");
	valueText_.setPosition(xPos_ + label_.getGlobalBounds().width + 4, yPos_ + 1);
	valueText_.setFont(font_);
	valueText_.setCharacterSize(16);
	valueText_.setColor(sf::Color::Black);
}

TextField::~TextField(){

}

sf::RectangleShape* TextField::GetRect(){return &borderRect_;}
string TextField::GetValue(){return value_;}

void TextField::HandleInput(sf::Event* event){
	if(type_ == "text"){
		if(event->type == sf::Event::TextEntered && (event->text.unicode >= 32 && event->text.unicode <= 126)){
			if(valueText_.getGlobalBounds().width <= 334 - 16){
				value_ += static_cast<char>(event->text.unicode);
				valueText_.setString(value_);
			}
		}
	} else
	if(type_ == "number"){
		if(event->type == sf::Event::TextEntered && (event->text.unicode >= 48 && event->text.unicode <= 57)){
			if(valueText_.getGlobalBounds().width <= 334 - 16){
				value_ += static_cast<char>(event->text.unicode);
				valueText_.setString(value_);
			}
		}
	}
	
	if(event->type == sf::Event::KeyPressed){
		if(event->key.code == sf::Keyboard::BackSpace){
			value_ = value_.substr(0, value_.length() - 1);
			valueText_.setString(value_);
		}
	}
}

void TextField::SetValue(string value){
	value_ = value;
	valueText_.setString(value_);
}

void TextField::SetBorder(string type){
	if(type == "normal"){borderRect_.setOutlineColor(sf::Color(000, 000, 000));} else
	if(type == "targeted"){borderRect_.setOutlineColor(sf::Color(38, 188, 00));}
}

void TextField::Draw(sf::RenderWindow* window){
	window->draw(label_);
	window->draw(borderRect_);
	window->draw(valueText_);
}