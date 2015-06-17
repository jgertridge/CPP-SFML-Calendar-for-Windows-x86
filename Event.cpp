#include "./Event.hpp"
#include "./Calendar.hpp"

Event::Event(string title, int month, int day, int year){
	type_ = "unique";
	title_ = title;
	month_ = month;
	day_ = day;
	year_ = year;
	
	eventText_.setString("- " + title_);
	eventText_.setColor(sf::Color::Black);
	eventText_.setCharacterSize(16);
}

Event::Event(string title, int month, int day){
	type_ = "persistent";
	title_ = title;
	month_ = month;
	day_ = day;
	year_ = -1;
	
	eventText_.setString("- " + title_);
	eventText_.setColor(sf::Color::Blue);
	eventText_.setCharacterSize(16);
}

Event::~Event(){

}

string Event::GetType(){return type_;}
string Event::GetTitle(){return title_;}
int Event::GetMonth(){return month_;}
int Event::GetDay(){return day_;}
int Event::GetYear(){return year_;}
sf::Text* Event::GetEventText(){return &eventText_;}

void Event::Draw(sf::RenderWindow* window, int iterator){
	eventText_.setPosition(420, 470 + (20 * iterator));
	eventText_.setFont(*Calendar::GetInstance()->GetFont());
	window->draw(eventText_);
}
