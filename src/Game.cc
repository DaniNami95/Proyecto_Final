#include "CommonHeaders.hh"
#include "Player.hh"
#include "Animation.hh"
#include "TileGroup.hh"


sf::CircleShape* circle{new sf::CircleShape()};

TextObject* textObj1{new TextObject(ASSETS_FONT_PSYCHOPATHY, 36, sf::Color::White, sf::Text::Italic)};

sf::Clock* gameClock{new sf::Clock()};
float deltaTime{};
Player* player1{};
//GameObject* chest1{};
GameObject* light1{};
GameObject* light2{};
GameObject* enemy1{};
GameObject* enemy2{};
//GameObject* wall1{};
Animation* idleAnimation{new Animation()};
Animation* runAnimation{new Animation()};

TileGroup* tileGroup{};
Tile* tile1{};

uint32 flags{};
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_centerOfMassBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_jointBit;

Animation* lightIdle{};
Animation* chestIdle{};
Animation* enemyIdle{};
Animation* enemyIdle2{};
Animation* lightIdle2{};


Game::Game()
{
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
  event = new sf::Event();
  gravity = new b2Vec2(0.f, 0.f);
  world = new b2World(*gravity);
  drawPhysics = new DrawPhysics(window);
  gameObjects = new std::vector<GameObject*>();
  gameObjectsDeleteList = new std::vector<GameObject*>();

  player1 = new Player(ASSETS_SPRITES, 4.f, 16, 16, 0, 5, 200, 250, 200.f, b2BodyType::b2_dynamicBody, world, window);
  player1->SetTagName("Player");
  /*chest1 = new GameObject(ASSETS_SPRITES, 4.f, 16, 16, 6, 1, 560, 800, b2BodyType::b2_staticBody, world, window);
  chest1->SetTagName("chest");*/
  light1 = new GameObject(ASSETS_SPRITES, 4.f, 16, 16, 6, 3, 200, 175, b2BodyType::b2_staticBody, world, window);
  light1->SetTagName("light1");
  light2 = new GameObject(ASSETS_SPRITES, 4.f, 16, 16, 6, 3, 200, 775, b2BodyType::b2_staticBody, world, window);
  light2->SetTagName("light2");
  enemy1 = new GameObject(ASSETS_SPRITES, 4.f, 16, 16, 0, 3, 275, 250,b2BodyType::b2_staticBody, world, window);
  enemy1->SetTagName("enemy");
  enemy2 = new GameObject(ASSETS_SPRITES, 4.f, 16, 16, 0, 3, 275, 400, b2BodyType::b2_staticBody, world, window);
  enemy2->SetTagName("enemy2");
  /*wall1 = new GameObject(ASSETS_TILES, 4.f, 32, 32, 0, 0, 100, 65, b2BodyType::b2_staticBody, world, window);
  wall1->SetTagName("WallHorUp1");*/
 
  tileGroup = new TileGroup(window, 12, 12, ASSETS_MAPS, 5.6f, 16, 16, ASSETS_TILES);

  contactEventManager = new ContactEventManager(gameObjects, gameObjectsDeleteList);


  lightIdle = new Animation(light1->GetSprite(), 6, 11, 0.1f, 3);
  //chestIdle = new Animation(chest1->GetSprite(), 6, 13, 0.1f, 1);
  enemyIdle = new Animation(enemy1->GetSprite(), 0, 5, 0.1f, 3);
  enemyIdle2 = new Animation(enemy2->GetSprite(), 0, 5, 0.1f, 3);
  lightIdle2 = new Animation(light2->GetSprite(), 6, 11, 0.1f, 3);
}

Game::~Game()
{
}

//Starting things
void Game::Start()
{
  flags += b2Draw::e_shapeBit;
  world->SetDebugDraw(drawPhysics);
  drawPhysics->SetFlags(flags);
  world->SetContactListener(contactEventManager);

  AddGameObject(player1);
  //AddGameObject(chest1);
  AddGameObject(light1);
  AddGameObject(light2);
  AddGameObject(enemy1);
  AddGameObject(enemy2);
  textObj1->SetTextStr("La Mamada no Funciona");
  idleAnimation = new Animation(player1->GetSprite(), 0, 5, 0.05f, 5);
  runAnimation = new Animation(player1->GetSprite(), 0, 5, 0.08f, 6);

}

void Game::Initialize()
{
  Start();
  MainLoop();
}

void Game::UpdatePhysics()
{
  world->ClearForces();
  world->Step(deltaTime, 8, 8);
}

void Game::Update()
{
  deltaTime = gameClock->getElapsedTime().asSeconds();
  gameClock->restart();

  for(auto &gameObject : *gameObjects)
  {
    gameObject->Update(deltaTime);
  }

  lightIdle->Play(deltaTime);
  lightIdle2->Play(deltaTime);
  chestIdle->Play(deltaTime);
  enemyIdle->Play(deltaTime);
  enemyIdle2->Play(deltaTime);

  if(std::abs(InputSystem::Axis().x) > 0 || std::abs(InputSystem::Axis().y) > 0)
  {
    runAnimation->Play(deltaTime);
  }
  else
  {
    idleAnimation->Play(deltaTime);
  }
}

void Game::MainLoop()
{
  while (window->isOpen())
  {
    while(window->pollEvent(*event))
    {
      if(event->type == sf::Event::Closed)
      {
        window->close();
      }
    }

    UpdatePhysics();
    Input();
    Update();
    Render();
  }
  Destroy();
}

void Game::Render()
{
  for(auto& gameobject: *gameObjectsDeleteList)
  {
      gameObjects->erase(std::remove(gameObjects->begin(), gameObjects->end(), gameobject), gameObjects->end());
      delete gameobject;
  }
  gameObjectsDeleteList->clear();

  window->clear(sf::Color::Black);
  Draw();
  window->display();
}

void Game::Draw()
{
  
  tileGroup->Draw();

  for(auto &gameObject : *gameObjects)
  {
    gameObject->Draw();
  }

  window->draw(*textObj1->GetText());
}

void Game::Input()
{
  
}

void Game::Destroy()
{
  delete window;
  delete event;
}

void Game::AddGameObject(GameObject* gameObject)
{
  gameObjects->push_back(gameObject);
}