#include "./Calendar.hpp"

Calendar* Calendar::instance_;

//PRIVATE:
Calendar::Calendar(){
	window_.create(sf::VideoMode(800, 600, 32), "Calendar by Jeremy Gertridge", sf::Style::Close); //add sf::Style::Resize to make the window resizable.
	window_.setFramerateLimit(60);
	window_.setVerticalSyncEnabled(true);
	
	font_.loadFromFile("./Data/arial.ttf");
	
	currentTime_ = time(0);
	currentDate_ = localtime(&currentTime_);
	
	target_ = NULL;
	//targetEvent_ = NULL;
	
	activeMonth_ = currentDate_->tm_mon;
	activeYear_ = currentDate_->tm_year + 1900;
	
	monthLabel_.setString(GetMonthString(currentDate_->tm_mon) + " " + ConvertIntToString(currentDate_->tm_year + 1900));
	monthLabel_.setFont(font_);
	monthLabel_.setColor(sf::Color::Black);
	monthLabel_.setCharacterSize(20);
	monthLabel_.setStyle(sf::Text::Bold);
	CenterText(&monthLabel_, 0, 800, 10);
	
	daysLabel_.setString("       Sunday          Monday          Tuesday      Wednesday       Thursday         Friday          Saturday    ");
	daysLabel_.setPosition(0, 40);
	daysLabel_.setFont(font_);
	daysLabel_.setColor(sf::Color::Black);
	daysLabel_.setCharacterSize(16);
	daysLabel_.setStyle(sf::Text::Bold);
	
	leftSelectorImage_.loadFromFile("./Images/SelectorLeft.png");
	rightSelectorImage_.loadFromFile("./Images/SelectorRight.png");
	leftSelector_.setTexture(leftSelectorImage_);
	leftSelector_.setPosition(200, 10);
	rightSelector_.setTexture(rightSelectorImage_);
	rightSelector_.setPosition(579, 10);
	
	dividerImage_.loadFromFile("./Images/Divider.png");
	divider_.setTexture(dividerImage_);
	divider_.setPosition(320, 448);
	selectedDay_.setString("");
	selectedDay_.setFont(font_);
	selectedDay_.setColor(sf::Color::Black);
	selectedDay_.setCharacterSize(16);
	selectedDay_.setStyle(sf::Text::Bold);
	
	deleteButtonImage_.loadFromFile("./Images/DeleteButton2.png");
	deleteButton_.setTexture(deleteButtonImage_);
	
	addNoteButtonImage_.loadFromFile("./Images/AddNote.png");
	addNoteButton_.setTexture(addNoteButtonImage_);
	addNoteButton_.setPosition(300, 510);
	
	targetTextField_ = NULL;
	addANote_.setString("Add a note: ");
	addANote_.setFont(font_);
	addANote_.setPosition(11, 442);
	addANote_.setColor(sf::Color::Black);
	addANote_.setCharacterSize(18);
	addANote_.setStyle(sf::Text::Bold);
	titleField_ = new TextField("text", "Note: ", 10, 475);
	typeBox_ = new Checkbox("Type: ", 10, 515);
	typeBox_->CreateOption("Unique ", sf::Color::Black, "Unique notes are for a specific day of a specific year. \nThey do not carry over to future years.", 80, 515);
	typeBox_->CreateOption("Persistent ", sf::Color::Blue, "Persistent notes will appear on the same day of every \nyear. Useful for birthdays and such.", 180, 515);
	
	LoadPersistentEvents();
	LoadUniqueEvents();
	ConstructMonth();
}

string Calendar::ConvertIntToString(int n){
	stringstream ss;
	ss << n;
	string returnString = ss.str();
	return returnString;
}

void Calendar::CenterText(sf::Text* text, int x1, int x2, int y){
	int width = text->getGlobalBounds().width;
	int x = x1 + ((x2 - x1) - width) / 2;
	text->setPosition(x, y);
}

void Calendar::UpdateDayColors(){
	//First, make sure all days are unselected.
	for(int d = 0; d < month_.size(); d++){
		month_[d]->SetSelected(false);
	}
	
	//Then, select the current target.
	if(target_ != NULL){
		target_->SetSelected(true);
	}
	
}

bool Calendar::SpriteIncludesPoint(sf::Sprite* sprite, int x, int y){
	if(x >= sprite->getPosition().x && x < sprite->getPosition().x + sprite->getGlobalBounds().width 
	&& y >= sprite->getPosition().y && y < sprite->getPosition().y + sprite->getGlobalBounds().height){
		return true;
	}
	
	return false;
}

bool Calendar::RectIncludesPoint(sf::RectangleShape* rect, int x, int y){
	if(x >= rect->getPosition().x && x < rect->getPosition().x + rect->getGlobalBounds().width 
	&& y >= rect->getPosition().y && y < rect->getPosition().y + rect->getGlobalBounds().height){
		return true;
	}
	
	return false;
}

string Calendar::AddSpaces(string str){
	for(int c = 0; c < str.length(); c++){
		if(str[c] == '_'){
			str[c] = ' ';
		}
	}
	
	return str;
}

//Important note: '' = char, "" = string/const char
string Calendar::RemoveSpaces(string str){
	for(int c = 0; c < str.length(); c++){
		if(str[c] == ' '){
			str[c] = '_';
		}
	}
	
	return str;
}

void Calendar::LoadPersistentEvents(){
	ifstream file;
	file.open("./Data/PersistingEvents.cll");
	
	string title = "";
	int month = 0, day = 0;
	
	while(file >> title){
		file >> month;
		file >> day;
		
		persistentEvents_.push_back(new Event(AddSpaces(title), month, day));
	}
	
	file.close();
}

void Calendar::LoadUniqueEvents(){
	ifstream file;
	file.open("./Data/UniqueEvents.cll");
	
	string title = "";
	int month = 0, day = 0, year = 0;
	
	while(file >> title){
		file >> month;
		file >> day;
		file >> year;
		
		uniqueEvents_.push_back(new Event(AddSpaces(title), month, day, year));
	}
	
	file.close();
}

void Calendar::DeleteEvent(Event* event){
	string type = event->GetType();
	
	if(type == "persistent"){
		for(int p = 0; p < persistentEvents_.size(); p++){
			if(persistentEvents_[p] == event){
				persistentEvents_.erase(persistentEvents_.begin() + p);
			}
		}
	} else
	if(type == "unique"){
		for(int p = 0; p < uniqueEvents_.size(); p++){
			if(uniqueEvents_[p] == event){
				uniqueEvents_.erase(uniqueEvents_.begin() + p);
			}
		}
	}
}

void Calendar::ConstructMonth(){
	//create day objects.
	for(int h = 0; h < 6; h++){
		for(int w = 0; w < 7; w++){
			month_.push_back(new Day(11 + (w * 111), 70 + (h * 61), &font_));
		}
	}
	
	int dayOfTheWeek = currentDate_->tm_wday;
	
	//find which day of the week the first of the month falls under.
	for(int d = currentDate_->tm_mday; d > 0; d--){
		if(dayOfTheWeek == 0){
			dayOfTheWeek = 6;
		} else{
			dayOfTheWeek--;
		}
	}
	
	int daysInLastMonth = 0;
	if(currentDate_->tm_mon != 0){
		daysInLastMonth = GetDaysInMonth(currentDate_->tm_mon - 1, currentDate_->tm_year + 1900);
	} else{
		daysInLastMonth = GetDaysInMonth(11, (currentDate_->tm_year - 1) + 1900);
	}
	int daysInThisMonth = GetDaysInMonth(currentDate_->tm_mon, currentDate_->tm_year + 1900);
	
	//even without this, the calendar is still accurate. It just looks nicer to have some days from the previous month displayed.
	if(dayOfTheWeek == 0){dayOfTheWeek += 7;} 
	
	int iterator = 0;
	
	//set last month's days.
	for(int n = daysInLastMonth - (dayOfTheWeek); n <= daysInLastMonth; n++){
		month_[iterator]->SetNumber(n);	
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//set this month's days.
	for(int i = 1; i <= daysInThisMonth; i++){
		month_[iterator]->SetNumber(i);
	
		if(i == currentDate_->tm_mday){
			month_[iterator]->SetType("current");
		} else{
			month_[iterator]->SetType("default");
		}
		
		iterator++;
	}
	
	//set next month's days.
	for(int r = 1; iterator < month_.size(); r++){
		month_[iterator]->SetNumber(r);
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//add any persistent events.
	for(int p = 0; p < persistentEvents_.size(); p++){
		if(persistentEvents_[p]->GetMonth() == activeMonth_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == persistentEvents_[p]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						//month_[day]->AddContent(persistentEvents_[p]->GetTitle());
						month_[day]->AddEvent(persistentEvents_[p]);
					}
				}
			}
		}
	}
	
	//add any unique events.
	for(int ue = 0; ue < uniqueEvents_.size(); ue++){
		if(uniqueEvents_[ue]->GetMonth() == activeMonth_ && uniqueEvents_[ue]->GetYear() == activeYear_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == uniqueEvents_[ue]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						//month_[day]->AddContent(uniqueEvents_[ue]->GetTitle());
						month_[day]->AddEvent(uniqueEvents_[ue]);
					}
				}
			}
		}
	}
}

void Calendar::HandleInput(sf::Event* event){
	if(event->type == sf::Event::MouseButtonPressed){
		if(event->mouseButton.button == sf::Mouse::Left){
			//Handle directional arrows.
			if(SpriteIncludesPoint(&leftSelector_, event->mouseButton.x, event->mouseButton.y) == true){
				RegressMonth();
			} else
			if(SpriteIncludesPoint(&rightSelector_, event->mouseButton.x, event->mouseButton.y) == true){
				AdvanceMonth();
			}
			
			//Handle the text input field.
			if(RectIncludesPoint(titleField_->GetRect(), event->mouseButton.x, event->mouseButton.y) == true){
				targetTextField_ = titleField_;
				targetTextField_->SetBorder("targeted");
			}
			
			//Handle days and their events.
			Day* day = GetClickedDay(event->mouseButton.x, event->mouseButton.y);
			if(day != NULL){
				target_ = day;
				
				targetEvent_ = NULL;
				targetTextField_ = NULL;
				titleField_->SetBorder("normal");
				target_->UpdateEventTextColor(targetEvent_);
				
				string targetsMonth = "";
				string targetsYear = "";
				
				if(target_->GetType() == "inactive"){
					if(target_->GetNumber() > 15){
						if(activeMonth_ == 0){
							targetsMonth = GetMonthString(11);
							targetsYear = ConvertIntToString(activeYear_ - 1);
						} else{
							targetsMonth = GetMonthString(activeMonth_ - 1);
							targetsYear = ConvertIntToString(activeYear_);
						}
					} else if(target_->GetNumber() < 15){
						if(activeMonth_ == 11){
							targetsMonth = GetMonthString(0);
							targetsYear = ConvertIntToString(activeYear_ + 1);
						} else{
							targetsMonth = GetMonthString(activeMonth_ + 1);
							targetsYear = ConvertIntToString(activeYear_);
						}
					}
				} else{
					targetsMonth = GetMonthString(activeMonth_);
					targetsYear = ConvertIntToString(activeYear_);
				}
				
				string proxy = targetsMonth;
				proxy += " ";
				proxy += ConvertIntToString(target_->GetNumber());
				proxy += " ";
				proxy += targetsYear;
				
				UpdateDayColors();
				
				selectedDay_.setString(proxy);
				CenterText(&selectedDay_, 0, 800, 440);
			} 
			
			if(target_ != NULL){
				Event* clickedEvent = target_->GetClickedEvent(event->mouseButton.x, event->mouseButton.y);
				if(clickedEvent != NULL){
					targetTextField_ = NULL;
					titleField_->SetBorder("normal");
					targetEvent_ = clickedEvent;
					target_->UpdateEventTextColor(clickedEvent);
					deleteButton_.setPosition(420 + clickedEvent->GetEventText()->getGlobalBounds().width + 4, clickedEvent->GetEventText()->getPosition().y);
				}
			}
			
			//Handle add note button.
			if(target_ != NULL){
				if(SpriteIncludesPoint(&addNoteButton_, event->mouseButton.x, event->mouseButton.y) == true){
					if(titleField_->GetValue() != ""){
						if(typeBox_->GetValue() == "Unique "){
							Event* newEvent = new Event(titleField_->GetValue(), activeMonth_, target_->GetNumber(), activeYear_);
							uniqueEvents_.push_back(newEvent);
							target_->AddEvent(newEvent);
							titleField_->SetValue("");
						} else
						if(typeBox_->GetValue() == "Persistent "){
							Event* newEvent = new Event(titleField_->GetValue(), activeMonth_, target_->GetNumber());
							persistentEvents_.push_back(newEvent);
							target_->AddEvent(newEvent);
							titleField_->SetValue("");
						}
					}
				}
			}
			
			//Handle event delete button.
			if(targetEvent_ != NULL){
				if(SpriteIncludesPoint(&deleteButton_, event->mouseButton.x, event->mouseButton.y) == true){
					if(target_->DeleteEvent(targetEvent_) == true){ //if deleting the event from the day was successful (for visual reasons)
						DeleteEvent(targetEvent_); //delete the event completely.
						targetEvent_ = NULL;
					}
				}
			}
			
		}
	}
	if(event->type == sf::Event::KeyPressed){
		if(event->key.code == sf::Keyboard::Delete){
			if(targetEvent_ != NULL){
				if(target_->DeleteEvent(targetEvent_) == true){ //if deleting the event from the day was successful
					DeleteEvent(targetEvent_); //delete the event completely.
					targetEvent_ = NULL;
				}
			}
		}
		
		if(event->key.code == sf::Keyboard::Return){
			if(target_ != NULL){
				if(titleField_->GetValue() != ""){
					if(typeBox_->GetValue() == "Unique "){
						Event* newEvent = new Event(titleField_->GetValue(), activeMonth_, target_->GetNumber(), activeYear_);
						uniqueEvents_.push_back(newEvent);
						target_->AddEvent(newEvent);
						titleField_->SetValue("");
					} else
					if(typeBox_->GetValue() == "Persistent "){
						Event* newEvent = new Event(titleField_->GetValue(), activeMonth_, target_->GetNumber());
						persistentEvents_.push_back(newEvent);
						target_->AddEvent(newEvent);
						titleField_->SetValue("");
					}
				}
			}
		}
		
		if(event->key.code == sf::Keyboard::Left){
			RegressMonth();
			UpdateDayColors();
		}
		if(event->key.code == sf::Keyboard::Right){
			AdvanceMonth();
			UpdateDayColors();
		}
		if(event->key.code == sf::Keyboard::Escape){
			target_ = NULL;
			targetEvent_ = NULL;
			targetTextField_ = NULL;
			UpdateDayColors();
		}
		
	}
}

void Calendar::AdvanceMonth(){
	target_ = NULL;
	targetEvent_ = NULL;
	targetTextField_ = NULL;
	
	int dayOfTheWeek = 0;
	bool found = false;
	
	//clear all content
	for(int yad = 0; yad < month_.size(); yad++){
		month_[yad]->ClearContent();
	}
	
	for(int d = 0; d < month_.size(); d++){
		if(month_[d]->GetType() == "inactive" && month_[d]->GetNumber() == 1){
			found = true;
		} else if(found == false){
			if(dayOfTheWeek == 6){
				dayOfTheWeek = 0;
			} else{
				dayOfTheWeek++;
			}
		}
	}
	
	if(activeMonth_ == 11){
		activeMonth_ = 0;
		activeYear_++;
	} else{
		activeMonth_++;
	}
	monthLabel_.setString(GetMonthString(activeMonth_) + " " + ConvertIntToString(activeYear_));
	
	int daysInLastMonth = 0;
	if(activeMonth_ != 0){
		daysInLastMonth = GetDaysInMonth(activeMonth_ - 1, activeYear_);
	} else{
		daysInLastMonth = GetDaysInMonth(11, activeYear_ - 1);
	}
	int daysInThisMonth = GetDaysInMonth(activeMonth_, activeYear_);
	
	int iterator = 0;

	//even without this, the calendar is still accurate. It just looks nicer to have some days from the previous month displayed.
	if(dayOfTheWeek == 0){dayOfTheWeek += 7;} 
	
	//set last month's days.
	for(int n = daysInLastMonth - (dayOfTheWeek - 1); n <= daysInLastMonth; n++){
		month_[iterator]->SetNumber(n);	
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//set this month's days.
	for(int i = 1; i <= daysInThisMonth; i++){
		month_[iterator]->SetNumber(i);
		
		if(i == currentDate_->tm_mday && activeMonth_ == currentDate_->tm_mon && activeYear_ == currentDate_->tm_year + 1900){
			month_[iterator]->SetType("current");
		} else{
			month_[iterator]->SetType("default");
		}

		iterator++;
	}
	
	//set next month's days.
	for(int r = 1; iterator < month_.size(); r++){
		month_[iterator]->SetNumber(r);
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//add any persistent events
	for(int p = 0; p < persistentEvents_.size(); p++){
		if(persistentEvents_[p]->GetMonth() == activeMonth_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == persistentEvents_[p]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						month_[day]->AddEvent(persistentEvents_[p]);
					}
				}
			}
		}
	}
	
	//add any unique events
	for(int ue = 0; ue < uniqueEvents_.size(); ue++){
		if(uniqueEvents_[ue]->GetMonth() == activeMonth_ && uniqueEvents_[ue]->GetYear() == activeYear_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == uniqueEvents_[ue]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						month_[day]->AddEvent(uniqueEvents_[ue]);
					}
				}
			}
		}
	}
	
}

void Calendar::RegressMonth(){
	target_ = NULL;
	targetEvent_ = NULL;
	targetTextField_ = NULL;

	int numToFind = 0;
	if(activeMonth_ != 0){
		numToFind = GetDaysInMonth(activeMonth_ - 1, activeYear_);
	} else{
		numToFind = GetDaysInMonth(11, activeYear_ - 1);
	}
	
	int dayOfTheWeek = 0;
	bool found = false;
	
	//clear all content
	for(int yad = 0; yad < month_.size(); yad++){
		month_[yad]->ClearContent();
	}
	
	//find reference point (the final day) from the current month.
	for(int d = 0; d < month_.size(); d++){
		if(month_[d]->GetType() == "inactive" && month_[d]->GetNumber() == numToFind){
			found = true;
		} else if(found == false){
			if(dayOfTheWeek == 6){
				dayOfTheWeek = 0;
			} else{
				dayOfTheWeek++;
			}
		}
	}
	
	//find the day of the week that the first of last month falls under.
	for(int a = numToFind - 1; a >= 1; a--){
		if(dayOfTheWeek == 0){
			dayOfTheWeek = 6;
		} else{
			dayOfTheWeek--;
		}
	}
	
	//change current month.
	if(activeMonth_ == 0){
		activeMonth_ = 11;
		activeYear_--;
	} else{
		activeMonth_--;
	}
	monthLabel_.setString(GetMonthString(activeMonth_) + " " + ConvertIntToString(activeYear_));
	
	int daysInLastMonth = 0;
	if(activeMonth_ != 0){
		daysInLastMonth = GetDaysInMonth(activeMonth_ - 1, activeYear_);
	} else{
		daysInLastMonth = GetDaysInMonth(11, activeYear_ - 1);
	}
	int daysInThisMonth = GetDaysInMonth(activeMonth_, activeYear_);
	
	int iterator = 0;
	
	//even without this, the calendar is still accurate. It just looks nicer to have some days from the previous month displayed.
	if(dayOfTheWeek == 0){dayOfTheWeek += 7;} 
	
	//set last month's days.
	for(int n = daysInLastMonth - (dayOfTheWeek - 1); n <= daysInLastMonth; n++){
		month_[iterator]->SetNumber(n);	
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//set this month's days.
	for(int i = 1; i <= daysInThisMonth; i++){
		month_[iterator]->SetNumber(i);
		
		if(i == currentDate_->tm_mday && activeMonth_ == currentDate_->tm_mon && activeYear_ == currentDate_->tm_year + 1900){
			month_[iterator]->SetType("current");
		} else{
			month_[iterator]->SetType("default");
		}
		
		iterator++;
	}
	
	//set next month's days.
	for(int r = 1; iterator < month_.size(); r++){
		month_[iterator]->SetNumber(r);
		month_[iterator]->SetType("inactive");
		iterator++;
	}
	
	//add any persistent events.
	for(int p = 0; p < persistentEvents_.size(); p++){
		if(persistentEvents_[p]->GetMonth() == activeMonth_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == persistentEvents_[p]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						month_[day]->AddEvent(persistentEvents_[p]);
					}
				}
			}
		}
	}
	
	//add any unique events.
	for(int ue = 0; ue < uniqueEvents_.size(); ue++){
		if(uniqueEvents_[ue]->GetMonth() == activeMonth_ && uniqueEvents_[ue]->GetYear() == activeYear_){
			for(int day = 0; day < month_.size(); day++){
				if(month_[day]->GetNumber() == uniqueEvents_[ue]->GetDay()){
					if(month_[day]->GetType() == "default" || month_[day]->GetType() == "current"){
						month_[day]->AddEvent(uniqueEvents_[ue]);
					}
				}
			}
		}
	}
}

void Calendar::Save(){
	//Save persistent events.
	ofstream persistentEventFile;
	persistentEventFile.open("./Data/PersistingEvents.cll");

	for(int p = 0; p < persistentEvents_.size(); p++){
		persistentEventFile << RemoveSpaces(persistentEvents_[p]->GetTitle()) << " ";
		persistentEventFile << persistentEvents_[p]->GetMonth() << " ";
		persistentEventFile << persistentEvents_[p]->GetDay() << " ";
		persistentEventFile << "\n";
	}
	
	persistentEventFile.close();
	
	//Save unique events.
	ofstream uniqueEventFile;
	uniqueEventFile.open("./Data/uniqueEvents.cll");

	for(int u = 0; u < uniqueEvents_.size(); u++){
		uniqueEventFile << RemoveSpaces(uniqueEvents_[u]->GetTitle()) << " ";
		uniqueEventFile << uniqueEvents_[u]->GetMonth() << " ";
		uniqueEventFile << uniqueEvents_[u]->GetDay() << " ";
		uniqueEventFile << uniqueEvents_[u]->GetYear() << " ";
		uniqueEventFile << "\n";
	}
	
	uniqueEventFile.close();
}

//PUBLIC:
Calendar::~Calendar(){
	
	
}

Calendar* Calendar::GetInstance(){
	if(!instance_){
		instance_ = new Calendar();
	}

	return instance_;
}

sf::Font* Calendar::GetFont(){return &font_;}

string Calendar::GetMonthString(int monthNum){
	string returnString = "";

	switch(monthNum){
		case 0: returnString = "January"; break;
		case 1: returnString = "February"; break;
		case 2: returnString = "March"; break;
		case 3: returnString = "April"; break;
		case 4: returnString = "May"; break;
		case 5: returnString = "June"; break;
		case 6: returnString = "July"; break;
		case 7: returnString = "August"; break;
		case 8: returnString = "September"; break;
		case 9: returnString = "October"; break;
		case 10: returnString = "November"; break;
		case 11: returnString = "December"; break;
	}
	
	return returnString;
}

int Calendar::GetDaysInMonth(int month, int year){
	int result = 0;

	switch(month){
		case 0: result = 31; break;
		case 1: 
			if(year % 4 == false && year % 100 != false || year % 400 == false){
				result = 29;
			} else{
				result = 28;
			}
		break;
		case 2: result = 31; break;
		case 3: result = 30; break;
		case 4: result = 31; break;
		case 5: result = 30; break;
		case 6: result = 31; break;
		case 7: result = 31; break;
		case 8: result = 30; break;
		case 9: result = 31; break;
		case 10: result = 30; break;
		case 11: result = 31; break;
	}
	
	return result;
}

Day* Calendar::GetClickedDay(int clickX, int clickY){
	Day* result = NULL;

	for(int d = 0; d < month_.size(); d++){
		if(clickX >= month_[d]->GetX() && clickX < month_[d]->GetX() + 110
		&& clickY >= month_[d]->GetY() && clickY < month_[d]->GetY() + 60){
			result = month_[d];
		}
	}
	
	return result;
}

void Calendar::Run(){
	while(window_.isOpen()){
		while(window_.pollEvent(event_)){
			if(event_.type == sf::Event::Closed){
				Save();
				window_.close();
			}
			
			HandleInput(&event_);
			
			if(target_ != NULL && targetTextField_ != NULL){
				targetTextField_->HandleInput(&event_);
			}
			
			if(target_ != NULL){typeBox_->HandleInput(&event_);}
			
		}
		
		window_.clear(sf::Color::White);
		
		window_.draw(leftSelector_);
		window_.draw(monthLabel_);
		window_.draw(rightSelector_);
		window_.draw(daysLabel_);
		
		for(int d = 0; d < month_.size(); d++){
			month_[d]->Draw(&window_);
		}
		
		window_.draw(divider_);
		
		if(target_ != NULL){
			window_.draw(selectedDay_);
			
			if(target_ ->GetType() != "inactive"){
				window_.draw(addANote_);
				titleField_->Draw(&window_);
				typeBox_->Draw(&window_);
				window_.draw(addNoteButton_);
			}
		}
		
		if(target_ != NULL){target_->DrawEvents(&window_);}
		if(target_ != NULL && targetEvent_ != NULL){window_.draw(deleteButton_);}
		
		window_.display();
	}
}










