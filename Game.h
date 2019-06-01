#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int menu(sf::RenderWindow & window) {
	sf::Image iStart, iTutorial, iExit, iInfo, iBackground;
	sf::Texture tStart, tTutorial, tExit, tInfo, tBackground;
	sf::Music music;
	music.openFromFile("start.ogg");
	music.play();
	iStart.loadFromFile("Start.png");
	iTutorial.loadFromFile("T.png");
	iExit.loadFromFile("Exit.png");
	iInfo.loadFromFile("Tutorial.png");
	iBackground.loadFromFile("Menu.png");
	iStart.createMaskFromColor(sf::Color::White);
	iTutorial.createMaskFromColor(sf::Color::White);
	iExit.createMaskFromColor(sf::Color::White);
	tStart.loadFromImage(iStart);
	tTutorial.loadFromImage(iTutorial);
	tExit.loadFromImage(iExit);
	tInfo.loadFromImage(iInfo);
	tBackground.loadFromImage(iBackground);
	sf::Sprite sStart(tStart), sTutorial(tTutorial), sExit(tExit), sInfo(tInfo), sBackground(tBackground);
	bool isMenu = 1;
	int menuNum = 0;
	sStart.setPosition(545, 230);
	sTutorial.setPosition(540, 320);
	sExit.setPosition(585, 410);
	while (isMenu)
	{
		sStart.setColor(sf::Color::Black);
		sTutorial.setColor(sf::Color::Black);
		sExit.setColor(sf::Color::Black);
		menuNum = 0;
		window.clear(sf::Color());
		if (sf::IntRect(545, 230, 182, 60).contains(sf::Mouse::getPosition(window))) { sStart.setColor(sf::Color::Red); menuNum = 1; }
		if (sf::IntRect(540, 320, 192, 60).contains(sf::Mouse::getPosition(window))) { sTutorial.setColor(sf::Color::Red); menuNum = 2; }
		if (sf::IntRect(585, 410, 110, 60).contains(sf::Mouse::getPosition(window))) { sExit.setColor(sf::Color::Red); menuNum = 3; }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (menuNum == 1) {
				isMenu = false; return 0;
			}
			if (menuNum == 2) { window.draw(sInfo); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)); }
			if (menuNum == 3) {  isMenu = false; return 1; }

		}
		window.draw(sBackground);
		window.draw(sStart);
		window.draw(sTutorial);
		window.draw(sExit);
		window.display();
	}
}
