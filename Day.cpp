#include "./Calendar.hpp"
#include "./Day.hpp"

Day::Day(int x, int y, sf::Font* font){
	xPos_ = x;
	yPos_ = y;
	
	dayRect_.setPosition(x, y);
	dayRect_.setSize(sf::Vector2f(106, 56));
	dayRect_.setOutlineThickness(2);
	dayRect_.setOutlineColor(sf::Color(000, 000, 000));
	
	numText_.setString("");
	numText_.setFont(*font);
	numText_.setPosition(xPos_ + 3, yPos_ + 1);
	numText_.setColor(sf::Color::Black);
	numText_.setCharacterSize(12);
	numText_.setStyle(sf::Text::Bold);
	
	for(int i = 0; i < 5; i++){
		dayContent_.push_back(new sf::Text());
		dayContent_[i]->setString("");
		dayContent_[i]->setFont(*font);
		dayContent_[i]->setColor(sf::Color::Black);
		dayContent_[i]->setCharacterSize(10);
		dayContent_[i]->setPosition(xPos_ + 20, yPos_ + 1 + (11 * i));
	}
	
	type_ = "default";
}

Day::~Day(){

}

int Day::GetNumber(){return number_;}
string Day::GetType(){return type_;}
int Day::GetX(){return xPos_;}
int Day::GetY(){return yPos_;}
vector<Event*>* Day::GetEvents(){return &events_;}

Event* Day::GetClickedEvent(int clickX, int clickY){
	Event* result = NULL;
	
	for(int e = 0; e < events_.size(); e++){
		if(clickX >= events_[e]->GetEventText()->getPosition().x 
		&& clickX < events_[e]->GetEventText()->getPosition().x + events_[e]->GetEventText()->getGlobalBounds().width
		&& clickY >= events_[e]->GetEventText()->getPosition().y 
		&& clickY < events_[e]->GetEventText()->getPosition().y + events_[e]->GetEventText()->getGlobalBounds().height + 5){
			result = events_[e];
		}
	}
	
	return result;
}

void Day::SetNumber(int num){
	number_ = num;
	
	string numString;
	stringstream ss;
	ss << number_;
	numString = ss.str();
	
	numText_.setString(numString);
}

void Day::SetType(string type){
	type_ = type;
	
	if(type_ == "default"){
		dayRect_.setFillColor(sf::Color(255, 255, 255));
	} else
	if(type_ == "current"){
		dayRect_.setFillColor(sf::Color(98, 220, 123));
	} else
	if(type_ == "inactive"){
		dayRect_.setFillColor(sf::Color(174, 174, 174));
	}
}

void Day::SetSelected(bool selected){
	selected_ = selected;
	
	if(selected_ == true){
		dayRect_.setOutlineColor(sf::Color(38, 188, 00));
	} else{
		dayRect_.setOutlineColor(sf::Color(000, 000, 000));
	}
}

void Day::AddEvent(Event* event){
	events_.push_back(event);
	RefreshContent();
}

bool Day::DeleteEvent(Event* event){
	bool result = false;

	for(int e = 0; e < events_.size(); e++){
		if(events_[e] == event){
			events_.erase(events_.begin() + e);
			RefreshContent();
			result = true;
		}
	}
	
	return result;
}

void Day::RefreshContent(){
	for(int i = 0; i < dayContent_.size(); i++){
		dayContent_[i]->setString("");
	}

	if(events_.size() <= 5){
		for(int e = 0; e < events_.size(); e++){
			string content = "- ";
			string title = events_[e]->GetTitle();
			
			for(int c = 0; c < title.length(); c++){
				if(dayContent_[e]->getGlobalBounds().width <= 80){
					content += title[c];
					dayContent_[e]->setString(content);
				}
			}
			
			if(events_[e]->GetType() == "unique"){
				dayContent_[e]->setColor(sf::Color::Black);
			} else
			if(events_[e]->GetType() == "persistent"){
				dayContent_[e]->setColor(sf::Color::Blue);
			}
		}
	} else{
		for(int e = 0; e < 5; e++){
			string content = "- ";
			string title = events_[e]->GetTitle();
			
			for(int c = 0; c < title.length(); c++){
				if(dayContent_[e]->getGlobalBounds().width <= 80){
					content += title[c];
					dayContent_[e]->setString(content);
				}
			}
			
			if(events_[e]->GetType() == "unique"){
				dayContent_[e]->setColor(sf::Color::Black);
			} else
			if(events_[e]->GetType() == "persistent"){
				dayContent_[e]->setColor(sf::Color::Blue);
			}
		}
	}
}

void Day::ClearContent(){
	events_.clear();
	
	for(int i = 0; i < dayContent_.size(); i++){
		dayContent_[i]->setString("");
	}
}

void Day::UpdateEventTextColor(Event* targetEvent){
	//Set the text of all of this day's events to the default color.
	for(int e = 0; e < events_.size(); e++){
		if(events_[e]->GetType() == "unique"){
			events_[e]->GetEventText()->setColor(sf::Color::Black);
		} else
		if(events_[e]->GetType() == "persistent"){
			events_[e]->GetEventText()->setColor(sf::Color::Blue);
		}
	}
	
	//Change the target event's text color.
	if(targetEvent != NULL){
		targetEvent->GetEventText()->setColor(sf::Color(56, 188, 0, 255));
	}
}

void Day::Draw(sf::RenderWindow* window){
	window->draw(dayRect_);
	window->draw(numText_);
	
	for(int i = 0; i < dayContent_.size(); i++){
		window->draw(*dayContent_[i]);
	}
}

void Day::DrawEvents(sf::RenderWindow* window){
	for(int i = 0; i < events_.size(); i++){
		events_[i]->Draw(window, i);
	}
}
