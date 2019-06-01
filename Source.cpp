#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include "Game.h"
using namespace std;
#define PI 3.14159265
float moveball(float x) {
	x -= x / 15;
	return x;
}
class object {
private:
	float x, y, radius;
	std::string file;
public:
	sf::Image iobject;
	sf::Texture tobject;
	sf::Sprite sobject;


	object(float X, float Y, float Radius, std::string f)
	{
		x = X;
		y = Y;
		radius = Radius;

		iobject.loadFromFile("" + f);
		iobject.createMaskFromColor(sf::Color::White);
		tobject.loadFromImage(iobject);
		sobject.setTexture(tobject);
		sobject.setOrigin(radius, radius);
		sobject.setPosition(x, y);
	}
	~object() {}
};

class Player : public object {
private: float timer, a, b, c, k;
		 int bump;
public:
	Player(float X, float Y, float Radius, std::string f) : object(X, Y, Radius, f) {
		timer = 0;
		a = 0;
		b = 0;
		c = 0;
		bump = 40;
	}
	void update() {
		sobject.rotate(6);
		timer = 0;
		a += 6;
		c = sin((PI*a) / 180);
		b = cos((PI*a) / 180);
	}
	void go(float time, float k) {
		sobject.move(b*0.4*time*k, c*0.4*time*k);
	}
	bool border() {
		if (sobject.getPosition().y >= 660) {
			sobject.move(0, -bump);
			sobject.rotate(4);
			timer = 0;
			a += 4;
			return true;
		}
		if (sobject.getPosition().y <= 70) {
			sobject.move(0, bump);
			sobject.rotate(4);
			timer = 0;
			a += 4;
			return true;
		}
		if (sobject.getPosition().x >= 1210) {
			sobject.move(-bump, 0);
			sobject.rotate(4);
			timer = 0;
			a += 4;
			return true;
		}
		if (sobject.getPosition().x <= 70) {
			sobject.move(bump, 0);
			sobject.rotate(4);
			timer = 0;
			a += 4;
			return true;
		}
		return false;
	}
	~Player() {}
};

class Ball : public object {
public:
	Ball(float X, float Y, float Radius, std::string f) : object(X, Y, Radius, f) {
	}
	void go(float bx, float by) {
		sobject.move(-bx*0.1, -by*0.1);
		if (sobject.getPosition().y <= 45 || sobject.getPosition().y >= 675) {
			by = -by;
		}
		if (sobject.getPosition().y >= 690) {
			sobject.move(0, -20);
		}
		if (sobject.getPosition().y <= 25) {
			sobject.move(0, 20);
		}
		if ((sobject.getPosition().x <= 45 || sobject.getPosition().x >= 1235) && (sobject.getPosition().y < 280 || sobject.getPosition().y>445)) {
			bx = -bx;
		}
		if (sobject.getPosition().x <= 25 && (sobject.getPosition().y < 280 || sobject.getPosition().y>445)) {
			sobject.move(20, 0);
		}
		if (sobject.getPosition().x >= 1255 && (sobject.getPosition().y < 280 || sobject.getPosition().y>445)) {
			sobject.move(-20, 0);
		}
		if (sobject.getPosition().x <= 45 && sobject.getPosition().y <= 45)
			sobject.move(100, 100);
		if (sobject.getPosition().x <= 45 && sobject.getPosition().y >= 675)
			sobject.move(100, -100);
		if (sobject.getPosition().x >= 1235 && sobject.getPosition().y <= 45)
			sobject.move(-100, 100);
		if (sobject.getPosition().x >= 1235 && sobject.getPosition().y >= 675)
			sobject.move(-100, -100);
	}
	~Ball() {}
};
float length(float a, float b, float c, float d) {
	float r = pow((pow((a - c), 2) + pow((b - d), 2)), 0.5);
	return r;
}

bool game() {
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	sf::Packet packet;	
	char type;
	char mode = ' ';
	char buffer[2000];
	size_t received;
	string text = "connect to: ";

	std::cout << ip << std::endl;
	std::cout << "Type:  c -client, s -server" << std::endl;
	std::cin >> type;
	if (type == 's') {
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket); 
		text += "Serwer";
		mode = 's';

	}
	else if (type == 'c') {
		std::cout << "new ip:";
		std::cin >> ip;

		socket.connect(ip, 2000);
		text += "client";
		mode = 'r';
	}
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	sf::RenderWindow window;
	sf::Image imap, ired, iblue;
	sf::Texture  tmap, tred, tblue;
	sf::Sprite  smap, sred, sblue;
	Player player1(960, 360, 65, "Player1.png");
	Player player2(320, 360, 65, "Player2.png");
	Ball ball(640, 360, 40, "ball.png");
	window.create(sf::VideoMode(1280, 720), "Test");
	int rrr = menu(window);
	if (rrr == 1) {
		return false;
	}
	imap.loadFromFile("Untitled.png");
	ired.loadFromFile("red.png");
	iblue.loadFromFile("blue.png");
	tmap.loadFromImage(imap);
	tred.loadFromImage(ired);
	tblue.loadFromImage(iblue);
	smap.setTexture(tmap);
	sred.setTexture(tred);
	sblue.setTexture(tblue);
	sred.setPosition(320, 180);
	sblue.setPosition(320, 180);
	sf::Music musicg;
	sf::Music musicw;
	musicg.openFromFile("goal.ogg");
	musicw.openFromFile("Winner.ogg");
	float timer1 = 0;
	float timer2 = 0;
	sf::Font font;
	int score1 = 0;
	int score2 = 0;
	float k = 1;
	if (font.loadFromFile("arial.ttf"))
		std::cout << "lool" << std::endl;
	sf::Text goal1;
	sf::Text goal2;
	goal1.setFont(font);
	goal2.setFont(font);
	std::string s1 = "0";
	std::string s2 = "0";
	goal1.setCharacterSize(200);
	goal2.setCharacterSize(200);
	goal1.setPosition(480, 0);
	goal2.setPosition(680, 0);
	sf::Clock clock;
	float r = 0;
	float r1 = 0;
	float r2 = 0;
	float buzzx = 0;
	float buzzy = 0;
	float buzzxb = 0;
	float buzzyb = 0;
	float balltimer = 0;
	float closetimer = 0;
	int newscore1, newscore2;
	sf::Vector2f newPos1, newPos2, newBal;
	sf::Vector2f prev1, prev2, prevb;
	socket.setBlocking(false);
	bool update = false;
	float newTime, newTime1, newTime2;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return false;
			else if (event.type == sf::Event::GainedFocus)
				update = true;
			else if (event.type == sf::Event::LostFocus)
				update = false;
		}

		prev1 = player1.sobject.getPosition();
		prev2 = player2.sobject.getPosition();
		prevb = ball.sobject.getPosition();

		timer1 += time;
		if (timer1 > 50) {
			player1.update(); timer1 = 0;
		}
		if (update) {
			if (mode = 's') {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					player2.go(time, -k);
					timer2 = 0;
				}
			}
			if (mode = 'r') {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				{
					player1.go(time, k);
					timer1 = 0;
				}
			}
		}

		if (player1.border()) {
			player1.update(); timer1 = 0;
		}
		timer2 += time;
		if (timer2 > 50) {
			player2.update(); timer2 = 0;
		}


		if (player2.border()) {
			player2.update(); timer2 = 0;
		}
		r = length(player1.sobject.getPosition().x, player1.sobject.getPosition().y, player2.sobject.getPosition().x, player2.sobject.getPosition().y);
		r1 = length(player1.sobject.getPosition().x, player1.sobject.getPosition().y, ball.sobject.getPosition().x, ball.sobject.getPosition().y);
		r2 = length(player2.sobject.getPosition().x, player2.sobject.getPosition().y, ball.sobject.getPosition().x, ball.sobject.getPosition().y);
		if (r <= 130) {
			buzzx = player1.sobject.getPosition().x - player2.sobject.getPosition().x;
			buzzy = player1.sobject.getPosition().y - player2.sobject.getPosition().y;
			player1.sobject.move(buzzx*0.01, buzzy*0.01); timer1 = 0;
			player2.sobject.move(-buzzx*0.01, -buzzy*0.01); timer2 = 0;
		}
		if (r <= 110) {
			buzzx = player1.sobject.getPosition().x - player2.sobject.getPosition().x;
			buzzy = player1.sobject.getPosition().y - player2.sobject.getPosition().y;
			player1.sobject.move(buzzx*0.1, buzzy*0.1); timer1 = 0;
			player2.sobject.move(-buzzx*0.1, -buzzy*0.1); timer2 = 0;
		}
		if (r1 <= 105) {
			buzzxb = player1.sobject.getPosition().x - ball.sobject.getPosition().x;
			buzzyb = player1.sobject.getPosition().y - ball.sobject.getPosition().y;
			timer1 = 0;
		}
		if (r2 <= 105) {
			buzzxb = player2.sobject.getPosition().x - ball.sobject.getPosition().x;
			buzzyb = player2.sobject.getPosition().y - ball.sobject.getPosition().y;
			timer2 = 0;
		}

		ball.go(buzzxb, buzzyb);
		buzzxb = moveball(buzzxb);
		buzzyb = moveball(buzzyb);
		if (-1<buzzxb<1 && -1<buzzyb<1)
		{
			buzzxb = 0;
			buzzyb = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			ball.sobject.move(1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			ball.sobject.move(-1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			ball.sobject.move(0, -1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			ball.sobject.move(0, 1);
		}

		if (mode == 'r') {
			if (prev1 != player1.sobject.getPosition() || prev2 != player2.sobject.getPosition() || prevb != ball.sobject.getPosition()) {
				packet << player2.sobject.getPosition().x << player2.sobject.getPosition().y << player1.sobject.getPosition().x << player1.sobject.getPosition().y << ball.sobject.getPosition().x << ball.sobject.getPosition().y;
			}
			socket.send(packet);
		}
		else {
			if (prev1 != player1.sobject.getPosition() || prev2 != player2.sobject.getPosition() || prevb != ball.sobject.getPosition()) {
				packet << player2.sobject.getPosition().x << player2.sobject.getPosition().y << player1.sobject.getPosition().x << player1.sobject.getPosition().y << ball.sobject.getPosition().x << ball.sobject.getPosition().y<<time<<timer1<<timer2;
			}
			socket.send(packet);
		}
		socket.receive(packet);
		if (mode == 'r') {
			if (packet >> newPos2.x >> newPos2.y >> newPos1.x >> newPos1.y >> newBal.x >> newBal.y>>newTime>>newTime1>>newTime2) {
				player2.sobject.setPosition(newPos2);
				player1.sobject.setPosition(newPos1);
				ball.sobject.setPosition(newBal);
				time = newTime;
				timer1 = newTime1;
				timer2 = newTime2;
			}
		}
		else {
			if (packet >> newPos2.x >> newPos2.y >> newPos1.x >> newPos1.y >> newBal.x >> newBal.y) {
				player2.sobject.setPosition(newPos2);
				player1.sobject.setPosition(newPos1);
				ball.sobject.setPosition(newBal);
			}
		}

		if (ball.sobject.getPosition().x < -35) {
			balltimer += time;
			if (balltimer > 790) {
				score1 += 1;
				player1.sobject.setPosition(960, 360);
				player2.sobject.setPosition(320, 360);
				ball.sobject.setPosition(640, 360);
				balltimer = 0;
			}
			musicg.play();
			switch (score1) {
			case 1:
			{   s1 = "1";
			break;
			}
			case 2:
			{   s1 = "2";
			break;
			}
			case 3:
			{   s1 = "3";
			musicg.pause();
			musicw.play();
			break;
			}
			default: {s1 = "0"; }
			}
		}
		goal2.setString(s1);
		if (ball.sobject.getPosition().x > 1315) {
			balltimer += time;
			if (balltimer > 790) {
				score2 += 1;
				player1.sobject.setPosition(960, 360);
				player2.sobject.setPosition(320, 360);
				ball.sobject.setPosition(640, 360);
				balltimer = 0;
			}
			musicg.play();
			switch (score2) {
			case 1:
			{   s2 = "1";
			break;
			}
			case 2:
			{   s2 = "2";
			break;
			}
			case 3:
			{   s2 = "3";
			musicg.pause();
			musicw.play();
			break;
			}
			default: {s2 = "0"; }
			}
		}
		goal1.setString(s2);
		window.clear(sf::Color::Cyan);
		window.draw(smap);
		window.draw(goal1);
		window.draw(goal2);
		window.draw(player1.sobject);
		window.draw(player2.sobject);
		window.draw(ball.sobject);
		if (score1 == 3 || score2 == 3) {
			closetimer += time;
			if (closetimer > 1000) {
				if (score1 == 3)
					window.draw(sred);
				else window.draw(sblue);
				k = 0;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) { return true; }
			}
		}
		window.display();
		if (mode == 'r')
			sleep(sf::milliseconds(10));
	}
}
void gameRunning() {
	if (game()) { gameRunning(); }
}
int main() {
	gameRunning();
	return 0;
}
