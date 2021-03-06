#include "../GameEngine.h"
#include "../Missile.h"
#include "../Collision.h"
#include "SFML/Audio.hpp"
#include "Explosion.h"
#include "HUD.h"
#include"../GameObject.h"
#include"../FuelCheck.h"
#include"../HighScore.h"

void Engine::start() {

	//inital parameters set
	float r = 0.0f, a = 0.0f;
	float planeX = 40.0f, planeY = 100.0f;
	float truckX = 700.0f, truckY = 680.0f; //673 would be better for proper placement
	float tankX = 30.0f, tankY = 680.0f;	//665 would be better for proper placement
	float planeOffsetX = 5.0f;
	float truckOffsetX = 3.0f;
	float tankOffsetX = 4.0f;
	float planeOffsetY = 0.0f;
	float hudX = 102.0f, hudY = -15.0f;
	float planelifeX = 52.0f, planelifeY = 40.0f;
	float fueliconX = 1265.0f, fueliconY = 40.0f;
	float missileX = planeX, missileY = planeY;
	float missileOffsetX = planeOffsetX, missileOffsetY = planeOffsetY;
	bool missileLaunched = false;
	bool truckCollision = false;
	bool tankCollision = false;
	FuelCheck fuelcheck;

	std::string planePosition = "right";
	sf::Clock clk;
	sf::Clock animationTime;
	sf::Clock explosionTime;

	//Making Objects for Music class
	sf::Music launching, ingamemusic, planesound, explosion;


	//Loading Sounds actually its not loading it is streaming good for memory management
	if (!launching.openFromFile("resources/Missilelunched.wav"))
		std::cout << "can't find sound" << std::endl;
	if (!planesound.openFromFile("resources/planesound.wav"))
		std::cout << "can't find sound" << std::endl;
	if (!ingamemusic.openFromFile("resources/ingame.wav"))
		std::cout << "not working" << std::endl;
	if (!explosion.openFromFile("resources/Explosion.wav"))
		std::cout << "sound not streamed" << std::endl;



	//window event
	sf::RenderWindow window(sf::VideoMode(1366, 768), "SKY BOMBER");
	window.setFramerateLimit(60);

	//Playing Music on Background
	ingamemusic.play();
	planesound.play();
	planesound.setLoop(true); //Looping For Continuous Sound

	//Textures 
	sf::Texture bgtexture, hud, planeTexture, fuel, planelife, tank1Texture, tank2Texture, missileTexture;



	//Loading Textures
	if (!planelife.loadFromFile("resources/planelifeicon.png"))						//Life Texture
		std::cout << "Cannot load from file" << std::endl;
	if (!hud.loadFromFile("resources/HUD.png"))										//HUD Texture
		std::cout << "Cannot load from file" << std::endl;
	if (!fuel.loadFromFile("resources/fuel.png"))									//Fuel Icon
		std::cout << "Cannot load form file" << std::endl;
	if (!bgtexture.loadFromFile("resources/background.png"))						//Main Background
		std::cout << "Cannot load form file" << std::endl;
	if (!planeTexture.loadFromFile("resources/planeboeing.png"))					//Plane Texture
		std::cout << "Cannot load form file" << std::endl;
	if (!missileTexture.loadFromFile("resources/missile.png"))						//Missile Texture
		std::cout << "Cannot load form file" << std::endl;
	if (!tank1Texture.loadFromFile("resources/tan2.png"))							//Tank 1 Texture
		std::cout << "Cannot load form file" << std::endl;
	if (!tank2Texture.loadFromFile("resources/tan.png"))							//Tank 2 Texture
		std::cout << "Cannot load form file" << std::endl;


	//Assigning Textures to Sprites
	sf::Sprite bgsprite, planeSprite, mTruckSprite, fuelicon, planelifeindicator, hudmenu, tankSprite, missileSprite;

	//Indivisualling Giving Textures to Sprites
	mTruckSprite.setTexture(tank1Texture);
	bgsprite.setTexture(bgtexture);
	hudmenu.setTexture(hud);
	planelifeindicator.setTexture(planelife);
	fuelicon.setTexture(fuel);
	planeSprite.setTexture(planeTexture);
	tankSprite.setTexture(tank2Texture);
	missileSprite.setTexture(missileTexture);

	sf::Vector2u sizePlane = planeTexture.getSize();
	sf::Vector2u sizeMissile = missileTexture.getSize();
	sf::Vector2u sizemTruck = tank1Texture.getSize();
	sf::Vector2u sizeTank = tank2Texture.getSize();
	sf::Vector2u sizeHud = hud.getSize();

	//HUD Texts


	
	//Game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		window.clear(sf::Color::Black);
		//background drawn
		window.draw(bgsprite);

		//Initiallizing screenX, screenY values
		float screenX = 0.0f, screenY = 768.0f;

		//Placement for every sprites
		hudmenu.setPosition(sf::Vector2f(hudX, hudY));
		fuelicon.setPosition(sf::Vector2f(fueliconX, fueliconY));
		missileSprite.setPosition(sf::Vector2f(missileX, missileY));
		planeSprite.setPosition(sf::Vector2f(planeX, planeY));
		mTruckSprite.setPosition(sf::Vector2f(truckX, truckY - sizemTruck.y));
		tankSprite.setPosition(sf::Vector2f(tankX, tankY - sizeTank.y + 5.0f));
		planelifeindicator.setPosition(sf::Vector2f(planelifeX, planelifeY));




		//Giving Boundary For Plane, When Boundary Condition Meets Plane will be turned as Y-Mirror
		planeSprite.move(planeOffsetX, planeOffsetY);

		if (planeSprite.getPosition().x < 0.0f)
		{
			r = planeSprite.getRotation();
			a = 180 + ((360 - r));

			planeSprite.setRotation(a);
			missileSprite.setRotation(a);
			planeSprite.setScale(1.0f, 1.0f);
			missileSprite.setScale(1.0f, 1.0f);

		}
		if (planeSprite.getPosition().x > (1366 - sizePlane.x))
		{

			r = planeSprite.getRotation();
			a = 180 + ((360 - r));

			planeSprite.setRotation(a);
			missileSprite.setRotation(a);
			planeSprite.setScale(1.0f, -1.0f);
			missileSprite.setScale(1.0f, 1.0f);
			
		}
		if (planeSprite.getPosition().y > (screenY - 180.0f))
		{
			r = planeSprite.getRotation();
			a = r + ((360 - r) * 2);
			planeSprite.setRotation(a);
			missileSprite.setRotation(a);

		}
		if (planeSprite.getPosition().y < 0)
		{
			r = planeSprite.getRotation();
			a = r + ((360 - r) * 2);
			planeSprite.setRotation(a);
			missileSprite.setRotation(a);
		}
		planeSprite.setOrigin(sizePlane.x / 2, sizePlane.y / 2);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{

			if (planeSprite.getScale(). y> 0)
			{
				planeSprite.rotate(-3.0f);
				missileSprite.rotate(-3.0f);
			}
			else
			{
				planeSprite.rotate(3.0f);
				missileSprite.rotate(3.0f);
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (planeSprite.getScale().y > 0)
			{
				planeSprite.rotate(3.0f);
				missileSprite.rotate(3.0f);

			}
			else
			{
				planeSprite.rotate(-3.0f);
				missileSprite.rotate(-3.0f);

			}

		}

		

		//Controls For Plane Sprite
		// Down 
		// Up
		// Q				Quit Forcely Close all the current window
		// Escapse			Back to mainmenu
		// R				Reload the Current Window
		// Space			Launching Missiles


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			ingamemusic.pause();
			planesound.pause();
			window.close();
			Game::start();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			launching.play();
			missileLaunched = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			planesound.pause();
			window.close();
			Engine::start();
		}

		//Rotations of Plane, Missile and updating the planeX and planeY position
		planeX += planeOffsetX;
		planeY += planeOffsetY;
		//truckX = truckX - truckOffsetX;
		//tankX = tankX + tankOffsetX;
		missileX += missileOffsetX;

		//When Space Button is Pressed
		if (missileLaunched)
		{
			Missile::move(&missileSprite, &missileY, planePosition);
			window.draw(missileSprite);
		}
		if (!missileLaunched)
		{
			missileY += missileOffsetY;
		}
		if (missileY > tankY)
		{
			missileSprite.setRotation(planeSprite.getRotation());
			missileSprite.setPosition(missileX, missileY);

			missileY = planeY;
			missileLaunched = false;
		}


		//Draw all the gameobjects according to z-index
		window.draw(missileSprite);
		window.draw(planeSprite);
		Level1::Start(tank1Texture, tank2Texture, &missileSprite, window);
		

		planeOffsetX = cos(planeSprite.getRotation()*3.14159265 / 180) * 10.0f;
		planeOffsetY = sin(planeSprite.getRotation()*3.14159265 / 180) * 10.0f;
		missileOffsetX = cos(planeSprite.getRotation()*3.14159265 / 180) * 10.0f;
		missileOffsetY = sin(planeSprite.getRotation()*3.14159265 / 180) * 10.0f;
		if (fuelcheck.isFinished())
		{
			Highscore::Save(Level1::sendScore(1));
			window.close();
		}
		//Drawing remaining sprites
		//std::cout << "The value is :" << fuelcheck.getFuel() << "\n";
		LifeFuel LifeFuel(window.getSize().x, window.getSize().y, fuelcheck.getFuel(), fuelcheck.getLife());
		LifeFuel.draw(window);
		window.draw(planelifeindicator);
		window.draw(hudmenu);
		window.draw(fuelicon);
		window.display();


	}

}
