#include "./Checkbox.hpp"

Checkbox::Checkbox(string label, int x, int y){
	font_.loadFromFile("./Data/times.ttf");

	label_.setString(label);
	label_.setPosition(x, y);
	label_.setColor(sf::Color::Black);
	label_.setFont(font_);
	label_.setCharacterSize(18);
	
	optionDescription_.setString("");
	optionDescription_.setPosition(x, y + 35);
	optionDescription_.setFont(font_);
	optionDescription_.setCharacterSize(16);
	optionDescription_.setColor(sf::Color::Black);
	
	xPos_ = x;
	yPos_ = y;
}

Checkbox::~Checkbox(){
	for(int o = options_.size() - 1; o >= 0; o--){
		delete options_[o];
	}
}

string Checkbox::GetValue(){return value_->GetLabel();}
int Checkbox::GetX(){return xPos_;}
int Checkbox::GetY(){return yPos_;}

void Checkbox::CreateOption(string label, sf::Color color, string description, int x, int y){
	CheckboxOption* option = new CheckboxOption(label, color, description, &font_);
	option->SetPosition(x, y);
	options_.push_back(option);
	
	if(options_.size() == 1){
		value_ = options_[0];
		optionDescription_.setString(value_->GetDescription());
		RefreshOptionImages();
	}
}

void Checkbox::RefreshOptionImages(){
	for(int o = 0; o < options_.size(); o++){
		options_[o]->SetState("unchecked");
	}
	
	value_->SetState("checked");
}

CheckboxOption* Checkbox::GetClickedOption(int clickX, int clickY){
	CheckboxOption* result = NULL;

	for(int o = 0; o < options_.size(); o++){
		if(clickX >= options_[o]->GetRect()->getPosition().x && clickX < options_[o]->GetRect()->getPosition().x + 20
		&& clickY >= options_[o]->GetRect()->getPosition().y && clickY < options_[o]->GetRect()->getPosition().y + 20){
			result = options_[o];
		}
	}
	
	return result;
}

void Checkbox::HandleInput(sf::Event* event){
	if(event->type == sf::Event::MouseButtonPressed){
		if(event->mouseButton.button == sf::Mouse::Left){
			CheckboxOption* clickedOption = GetClickedOption(event->mouseButton.x, event->mouseButton.y);
			if(clickedOption != NULL){
				value_ = clickedOption;
				optionDescription_.setString(value_->GetDescription());
				RefreshOptionImages();
			}
			
		}
		
	}
}

void Checkbox::Draw(sf::RenderWindow* window){
	window->draw(label_);
	
	for(int o = 0; o < options_.size(); o++){
		options_[o]->Draw(window);
	}
	
	window->draw(optionDescription_);
}