#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cMain.h"

gameState update(gameState theCurrentGameState, gameState newGameState);
int astro = 0;
bool menu = false;
string finalScore = "";
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
	
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;
	

	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	// Cube

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));
	LPCSTR gameSounds[] = { "Audio\\menu.wav", "Audio\\gameSound.wav", "Audio\\endSound.wav" };
	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}
	
		
		cTexture textureBkgd;
		textureBkgd.createTexture("Images\\starscape1024x768.png");
		cBkGround spriteBkgd;
		spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
		spriteBkgd.setTexture(textureBkgd.getTexture());
		spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

		cTexture rocketTxt;
		rocketTxt.createTexture("Images\\rocketSprite.png");
		cRocket rocketSprite;
		rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
		rocketSprite.setSpritePos(glm::vec2(512.0f, 380.0f));
		rocketSprite.setTexture(rocketTxt.getTexture());
		rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
		rocketSprite.setSpriteCentre();
		rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));
		
		cTexture menuTxt;
		menuTxt.createTexture("Images\\menuBackground.png");
		cBkGround menuSprite;
		spriteBkgd.attachInputMgr(theInputMgr);
		spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
		spriteBkgd.setTexture(textureBkgd.getTexture());
		spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

		cTexture endTxt;
		endTxt.createTexture("Images\\endBackground.png");
		cBkGround endSprite;
		spriteBkgd.attachInputMgr(theInputMgr);
		spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
		spriteBkgd.setTexture(textureBkgd.getTexture());
		spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

		gameState theGameState = GAME;

		//This is the mainloop, we render frames until isRunning returns false
		while (pgmWNDMgr->isWNDRunning())
		{
			pgmWNDMgr->processWNDEvents(); //Process any window events

			//We get the time that passed since the last frame
			float elapsedTime = pgmWNDMgr->getElapsedSeconds();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			switch (theGameState)
			{
			case MENU:
				menuSprite.render();
				
				if (m_InputMgr->isKeyDown(VK_RETURN))
				{
					gameState theGameState = GAME;
				}

				break;
			case GAME:
				spriteBkgd.render();
				// Create vector array of textures
				rocketSprite.render();
				for (int astro = 0; astro < 2; astro++)
				{
					theAsteroids.push_back(new cAsteroid);

					theAsteroids[astro]->setSpritePos(glm::vec2(1024 / (rand() % 5 + 1), 0));
					theAsteroids[astro]->setSpriteTranslation(glm::vec2(0, 20));
					theAsteroids[astro]->setTexture(theGameTextures[rand() % 4]->getTexture());
					theAsteroids[astro]->setTextureDimensions(theGameTextures[rand() % 1]->getTWidth() / 2, theGameTextures[rand() % 1]->getTHeight() / 2);
					theAsteroids[astro]->setSpriteCentre();
					theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
					theAsteroids[astro]->setActive(true);
					theAsteroids[astro]->setMdlRadius();
				}
			case END:
				endSprite.render();
				if (m_InputMgr->isKeyDown(VK_RETURN))
				{
					gameState theGameState = MENU;
				}

				

				vector<cAsteroid*>::iterator index = theAsteroids.begin();
				while (index != theAsteroids.end())
				{
					if ((*index)->isActive() == false)
					{
						index = theAsteroids.erase(index);
					}
					else
					{
						(*index)->update(elapsedTime);
						(*index)->render();
						++index;
					}
				}

				rocketSprite.update(elapsedTime);
				rocketSprite.render();

				pgmWNDMgr->swapBuffers();
				theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
			}

			theOGLWnd.shutdown(); //Free any resources
			pgmWNDMgr->destroyWND(); //Destroy the program window
			

			return 0; //Return success
		}
		
}
