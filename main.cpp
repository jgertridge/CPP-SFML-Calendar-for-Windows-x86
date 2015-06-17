#include <iostream>

#include "./Calendar.hpp"

using namespace std;
int main(){
	Calendar::GetInstance()->Run();
	return 0;
}
