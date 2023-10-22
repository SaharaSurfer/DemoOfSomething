#include "Interface.h"

int main()
{
	Interface interface;
	
	std::string entry = interface.LoadText("Text\\Entry.txt");
	interface.RenderText(entry);
	system("pause");

	system("cls");
	std::string character_creation = "Locations\\banner.txt";
	interface.LoadLocation(character_creation);
	interface.RenderLocation();

	//Выводятся рассы и ожидается ввод игрока
	interface.RenderText("Choose your race: \n");

	return 0;
}