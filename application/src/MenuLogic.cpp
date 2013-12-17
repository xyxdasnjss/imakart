#include "MenuLogic.hpp"
#include "ButtonActionEnum.hpp"
#include <iostream>

MenuLogic::MenuLogic():
	nbButtonInMenu(0), positionButtonSelected(0){
}

const InterfaceElement* MenuLogic::getTabInterfaceElement(const unsigned int positionOfElement){
	if(positionOfElement < nbButtonInMenu)
		return tabButtonLogicMenu[positionOfElement];
	else
		return NULL;
}

MenuLogic* MenuLogic::initialiseMainMenu(){
	MenuLogic* mainMenu = new MenuLogic;
	
	ButtonLogic* jouer = new ButtonLogic("jouer");
	ButtonLogic* options = new ButtonLogic("options");
	ButtonLogic* credits = new ButtonLogic("credits");
	ButtonLogic* quitter = new ButtonLogic("quitter");

	jouer->setAction(PLAY);
	options->setAction(OPTIONS);
	credits->setAction(CREDITS);
	quitter->setAction(EXIT);

	mainMenu->addButton(jouer);
	mainMenu->addButton(options);
	mainMenu->addButton(credits);
	mainMenu->addButton(quitter);
	
	return mainMenu;
}

MenuLogic* MenuLogic::initialiseKartMenu(std::vector <std::string> kartsName){
	MenuLogic* kartMenu = new MenuLogic;

	for(unsigned int i = 0; i < kartsName.size(); ++i){
		ButtonLogic* kart = new ButtonLogic(kartsName[i]);
		kart->setAction(SELECT);
		kartMenu->addButton(kart);
	}

	return kartMenu;
}

MenuLogic* MenuLogic::initialiseMapMenu(){
	MenuLogic* mapMenu = new MenuLogic;
	return mapMenu;
}

void MenuLogic::addButton(ButtonLogic* buttonToAdd){
	tabButtonLogicMenu.push_back(buttonToAdd);
	if(nbButtonInMenu == 0){
		tabButtonLogicMenu[0]->changeState();
	}
	++nbButtonInMenu;
}

void MenuLogic::nextButton(){
	tabButtonLogicMenu[positionButtonSelected]->changeState();
	++positionButtonSelected;
	if(positionButtonSelected >= nbButtonInMenu) {
		positionButtonSelected = 0;
	}
	tabButtonLogicMenu[positionButtonSelected]->changeState();
}

void MenuLogic::previousButton(){
	tabButtonLogicMenu[positionButtonSelected]->changeState();
	if(positionButtonSelected == 0) {
		positionButtonSelected = nbButtonInMenu-1;
	} else {
		--positionButtonSelected;
	}
	tabButtonLogicMenu[positionButtonSelected]->changeState();
}

ButtonAction MenuLogic::submitButton(){
	return tabButtonLogicMenu[positionButtonSelected]->getButtonAction();
}

MenuLogic::~MenuLogic(){
	for (size_t i = 0; i < tabButtonLogicMenu.size(); ++i)
	{
	  delete tabButtonLogicMenu[i];
	}
}
