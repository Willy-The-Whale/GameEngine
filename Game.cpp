#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "ContactListener.hpp"
#include "GUI.hpp"
#include <string>
#include "KeyboardInput.hpp"
#include <array>


GameObject* player;
GameObject* enemy;
Map* map;


b2World* world;
SDL_Event Game::event;

int ScreenX;
int ScreenY;

//adjust how fast the player moves;
float speed = 1.0f;

//Previous speed of the last frame.
b2Vec2 PastSpeed;

int jumpSteps = 0;
int jumpStepsEnemy = 0;
int blockSteps = 0;

//unique number for tagging sensors and bodies
int Tag;

ContactListener listenerInstance;

//Keeps track of the last body used so respawning is more randomized.
b2Body* bd;

//Player Variables
Game::Game() {
	b2Vec2 gravity(0, 7.5f);
	world = new b2World(gravity);
	Tag = 0;
	maxHealth = 100;
}
Game::~Game(){}
Game* Game::mInstance = nullptr;

SDL_Renderer* Game::renderer = nullptr;
Gui gui;
bool showGUI = false;

//IMGUI MAP VARIABLES
const int mapWidth = 20;
const int mapHeight = 25;
//std::array<std::array<int, mapHeight>, mapWidth> mapData;
int selectedTileX = -1;
int selectedTileY = -1;
const char* tileLabels[] = { "Water", "Grass", "Dirt", "Brick", "Lava", "Wood", "Ice", "Rock", "Marsh"};
int mapData[mapWidth][mapHeight] = {};
//-----------------------------------------------

b2World* Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	ScreenX = width;
	ScreenY = height;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "Subsystems Initialised!..." << endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			cout << "WINDOW CREATED" << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "RENDERER CREATED" << endl;
		}

		isRunning = true;		
	} else {
		isRunning = false;
	}

	gui.Create(window, renderer);
	
	player = new GameObject("assets/guyIdle.png", 20, 20,world,Tag);
	Tag += 3;
	enemy = new GameObject("assets/Meat.png", 700, 50, world,Tag);
	Tag += 3;
	map = new Map(world);
	map->readFromFile(4);
	//ensure the map loaded in Map.cpp also corelates to the map in gui
	map->GuiMap(mapData);
	listenerInstance.Player = player;
	listenerInstance.Enemy = enemy;
	world->SetContactListener(&listenerInstance);
	CreateCeiling();
	return world;
	
}

void Game::handleEvents() {
	KeyboardInput keyboardInput(*player, *enemy);
	SDL_PollEvent(&event);
	gui.HandleSDLEvent(event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		keyboardInput.update(event);
		break;
	}
}

void Game::CreateCeiling()
{
	float end = (float)ScreenX / 100;
	float physX = -.64;
	float physY = (float)ScreenY / 100;
	physY += .3;
	end += .64;
	for (float i = -.64; i < end; i += .32)
	{
		b2BodyDef groundDef;
		b2PolygonShape pshape2;
		b2FixtureDef def;
		groundDef.type = b2_staticBody;
		groundDef.position.Set(physX + .160f, physY + .160f);
		b2Body* ground = world->CreateBody(&groundDef);
		pshape2.SetAsBox(.160f, .160f);
		def.shape = &pshape2;
		b2Fixture* fix = ground->CreateFixture(&def);
	}
}

void Game::update(){
	player->Update();
	enemy->Update();
	enemyAI();
	//cout << player->body->GetPosition().x << " ";
	//cout << player->body->GetPosition().y << endl;
	DeathCheck();
	b2Vec2 tempPos;
	if (enemy->body->GetPosition().y *100 > ScreenY + 30 || enemy->body->GetPosition().y*100 < -100)
	{
		
		b2Body* temp;
		temp = bd;
		if (temp == NULL)
		{
			temp = world->GetBodyList();
		}
		temp->GetNext();
		while (temp->GetType() != b2_staticBody)
		{
			if (temp != NULL)
			{
				temp = temp->GetNext();
			}
			else if (temp == NULL)
			{
				temp = world->GetBodyList();
			}
			
		}
		bd = temp;
		tempPos = temp->GetPosition();
		tempPos.y -= .33;
		tempPos.x -= .32;
		enemy->body->SetTransform(tempPos, 0);
	}

	if (player->body->GetPosition().y * 100 > ScreenY + 30 || player->body->GetPosition().y * 100 < -100)
	{

		b2Body* temp;
		temp = bd;
		if (temp == NULL)
		{
			temp = world->GetBodyList();
		}
		temp->GetNext();
		while (temp->GetType() != b2_staticBody)
		{
			if (temp != NULL)
			{
				temp = temp->GetNext();
			}
			else if (temp == NULL)
			{
				temp = world->GetBodyList();
			}

		}
		bd = temp;
		tempPos = temp->GetPosition();
		tempPos.y -= .33;
		tempPos.x -= .32;
		player->body->SetTransform(tempPos, 0);
	}
}
//global variables for imgui

static bool showMap = false;
int currentSection = 0;
int sectionWidth = 10;
int sectionHeight = 12;
string sectionNames[] = { "Top Left", "Bottom Left", "Top Right", "Bottom Right" };

void Game::render() {
	SDL_RenderClear(renderer);
	map->DrawMap();
	player->Render();
	enemy->Render();

	if (true) {		
		gui.BeginRender();
		
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Edit Map")) {
				// Toggle the visibility of the map grid				
				if (ImGui::Checkbox("Show/Hide Map Grid", &showMap)) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (showMap)
			showGUIsystem();

		gui.EndRender();

	}
	renderHealthBar();
	renderScoreSystem();
	SDL_RenderPresent(renderer);

}

void Game::showGUIsystem() {
	// Create a window
	ImGui::Begin("##Map Editor");
	// Create a child window to hold the map
	ImGui::BeginChild("##Map", ImVec2(0, 0), true);
	
	// Calculate the starting and ending x and y values for the current section
	int startX = (currentSection % (mapWidth / sectionWidth)) * sectionWidth;
	int endX = startX + sectionWidth;
	int startY = (currentSection / (mapWidth / sectionWidth)) * sectionHeight;
	int endY = startY + sectionHeight;

	// Iterate over the mapData array for the current section
	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			// Get the current tile value
			int tileValue = mapData[x][y];

			switch (tileValue) {
			case 0: // Water
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // dark blue
				break;
			case 1: // Grass
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.565f, 0.933f, 0.565f, 1.0f)); // light green
				break;
			case 2: // Dirt
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.647f, 0.165f, 0.165f, 1.0f)); // brown
				break;
			case 3: // Brick
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.271f, 0.0f, 1.0f)); // red
				break;
			case 4: // Lava
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.549f, 0.0f, 1.0f)); // orange
				break;
			case 5: // Wood
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.545f, 0.271f, 0.075f, 1.0f)); // dark brown
				break;
			case 6: // Ice
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.529f, 0.808f, 0.922f, 1.0f)); // whitish blue
				break;
			case 7: // Rock
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.663f, 0.663f, 0.663f, 1.0f)); // grey
				break;
			case 8: // Marsh
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.392f, 0.f, 1.f)); // dark green
				break;
			default:
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f)); // default color (white)
				break;
			}


			// draws the button with the label of the current tile value (water, dirt, grass, etc)
			ImGui::Button(tileLabels[tileValue], ImVec2(32, 32));

			// pops the button color
			ImGui::PopStyleColor();

			// Check if the button was clicked
			if (ImGui::IsItemClicked()) {
				// Update the selected tile coordinates
				selectedTileX = x;
				selectedTileY = y;

				// Open the context menu
				ImGui::OpenPopup("TileContextMenu");
				//loading the current grid onto the map that will display on screen.

			}

			// Push a unique ID for this tile onto the ID stack
			ImGui::PushID(x * mapHeight + y);

			// Pop the ID off the stack
			ImGui::PopID();

			if (y < endY - 1) {
				ImGui::SameLine();
			}
		}
	}

	// Add buttons to switch between sections
	for (int i = 0; i < (mapWidth / sectionWidth) * (mapHeight / sectionHeight); i++) {
		if (ImGui::Button(sectionNames[i].c_str())) {
			currentSection = i;
		}
		if (i < ((mapWidth / sectionWidth) * (mapHeight / sectionHeight)) - 1) {
			ImGui::SameLine();
		}
	}
	// Push a unique ID for this tile onto the ID stack
	if (ImGui::BeginPopup("TileContextMenu")) {
		if (ImGui::MenuItem("Water")) {
			mapData[selectedTileX][selectedTileY] = 0;
		}
		if (ImGui::MenuItem("Grass")) {
			mapData[selectedTileX][selectedTileY] = 1;
		}
		if (ImGui::MenuItem("Dirt")) {
			mapData[selectedTileX][selectedTileY] = 2;
		}
		if (ImGui::MenuItem("Brick")) {
			mapData[selectedTileX][selectedTileY] = 3;
		}
		if (ImGui::MenuItem("Lava")) {
			mapData[selectedTileX][selectedTileY] = 4;
		}
		if (ImGui::MenuItem("Wood")) {
			mapData[selectedTileX][selectedTileY] = 5;
		}
		if (ImGui::MenuItem("Ice")) {
			mapData[selectedTileX][selectedTileY] = 6;
		}
		if (ImGui::MenuItem("Rock")) {
			mapData[selectedTileX][selectedTileY] = 7;
		}
		if (ImGui::MenuItem("Marsh")) {
			mapData[selectedTileX][selectedTileY] = 8;
		}
		map->LoadMap(mapData);
		ImGui::EndPopup();
	}

	if (ImGui::Button("Save Map")) {
		map->writeToFile(mapData, "Level");
	}

	ImGui::EndChild();

	ImGui::End();
}

void Game::clean()
{
		for (int x = 0; x < mapWidth; x++) {
	for (int y = 0; y < mapHeight; y++) {
			cout << mapData[x][y] << " ";
		}
		cout << endl;
	}
	delete world;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_QUIT;
	cout << "GAME CLEANED" << endl;

}

void Game::enemyAI() 
{
	if (!enemy->dead)
	{
		b2Vec2 velocity = enemy->body->GetLinearVelocity();
		float vel = 0;
		float velChange = 0;
		float impulse = 0;

		// Calculate the distance between the enemy and the player
		b2Vec2 playerPos = player->body->GetPosition();
		b2Vec2 enemyPos = enemy->body->GetPosition();
		double distance = playerPos.x - enemyPos.x;


		// Move towards the player's position
		if (distance > .7)
		{
			vel = speed;
		}
		else if (distance < -.7)
		{
			vel =  -speed;
		}
		else
		{
			vel = 0;
		}
		distance = playerPos.y - enemyPos.y;
		
		// Check if the enemy is about to fall off an edge
			bool aboutToFall = listenerInstance.EnemyAboutToFall;
		if (aboutToFall) {
			jumpStepsEnemy = 3;
			listenerInstance.EnemyAboutToFall = false;
		}

		velChange = vel - velocity.x;
		impulse = enemy->body->GetMass() * velChange / (1 / 60.0f);
		enemy->body->ApplyForceToCenter(b2Vec2(impulse, 0), false);
		b2Vec2 temp = enemy->body->GetPosition();
		if (impulse < 0)
		{
			enemy->flip = true;
		}
		else
		{
			enemy->flip = false;
		}
		//cout << "Enemy pos" << temp.x << endl;

		if (jumpStepsEnemy)
		{
			
			vel = speed;
			velChange = vel - velocity.x;
			impulse = enemy->body->GetMass() * velChange / (1 / 60.0f);
			enemy->body->ApplyForceToCenter(b2Vec2(0, -impulse /2), false);
			jumpStepsEnemy--;
		}

		// Attack the player
		//cout << "Distance:" << distance << endl;
		if (abs(distance) < .1 && rand() % 10 == 0)
		{
			//cout << "Enemy Attack:" << endl;
			enemy->attack(player->Guarding, player);
		}
	}
}

void Game::renderHealthBar() {
/*
	TTF_Font* font = TTF_OpenFont("Font.ttf", 0);
	if (font == NULL)
	{
		//cout << "Font unable to load." << endl;
		//cout << TTF_GetError << endl;
	}*/
	// Define health bar position and size
	int barWidth = 200;
	int barHeight = 20;
	int barPadding = 10;
	int barXPlayer = barPadding;
	int barYPlayer = ScreenY - barHeight - barPadding;
	int barXEnemy = ScreenX - barWidth - barPadding;
	int barYEnemy = ScreenY - barHeight - barPadding;
	
	// Calculate the percentage of health remaining
	float playerHealthPercent = (float)player->health / (float)maxHealth;
	float enemyHealthPercent = (float)enemy->health / (float)maxHealth;

	// Render player health bar
	//cout << "barXPlayer" << barXPlayer << endl; 
	//cout << "barYPlayer" << barYPlayer << endl;
	SDL_Rect playerBarBg = { barXPlayer, barYPlayer, barWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &playerBarBg);
	SDL_Rect playerBar = { barXPlayer, barYPlayer, (int)(barWidth * playerHealthPercent), barHeight };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &playerBar);
	std::string playerHealthText = "Player Health: " + std::to_string((int)player->health);
	SDL_Color textColor = { 255, 255, 255 };
	//SDL_Surface* playerHealthSurface = TTF_RenderText_Solid(font, playerHealthText.c_str(), textColor);
	//SDL_Texture* playerHealthTexture = SDL_CreateTextureFromSurface(renderer, playerHealthSurface);
	//SDL_Rect playerHealthRect = { barX + barPadding, barY - playerHealthSurface->h - barPadding, playerHealthSurface->w, playerHealthSurface->h };
	//SDL_RenderCopy(renderer, playerHealthTexture, NULL, &playerHealthRect);
	//SDL_FreeSurface(playerHealthSurface);
	//SDL_DestroyTexture(playerHealthTexture);

	// Render enemy health bar
	SDL_Rect enemyBarBg = { barXEnemy, barYEnemy, barWidth, barHeight };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &enemyBarBg);
	SDL_Rect enemyBar = { barXEnemy + barWidth - (int)(barWidth * enemyHealthPercent), barYEnemy, (int)(barWidth * enemyHealthPercent), barHeight };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &enemyBar);
	std::string enemyHealthText = "Enemy Health: " + std::to_string((int)enemy->health);
	//SDL_Surface* enemyHealthSurface = TTF_RenderText_Solid(font, enemyHealthText.c_str(), textColor);
	//SDL_Texture* enemyHealthTexture = SDL_CreateTextureFromSurface(renderer, enemyHealthSurface);
	//SDL_Rect enemyHealthRect = { ScreenX - barX - enemyHealthSurface->w - barPadding, barY - enemyHealthSurface->h - barPadding, enemyHealthSurface->w, enemyHealthSurface->h };
	//SDL_RenderCopy(renderer, enemyHealthTexture, NULL, &enemyHealthRect);
	//SDL_FreeSurface(enemyHealthSurface);
	//SDL_DestroyTexture(enemyHealthTexture);

	// Check if player or enemy is dead
	
}

void Game::DeathCheck()
{
	if (player->health <= 0) //destory the player and create a new one.
	{
		player->body->SetAwake(true);
		std::cout << "Player died!" << std::endl;
		world->DestroyBody(player->body);
		delete player;
		player = new GameObject("assets/guyIdle.png", 20, 20, world, 0);
		listenerInstance.Player = player;
		enemy->score++;

	}
	if (enemy->health <= 0) //Destroy the game object and create a new one.
	{
		enemy->body->SetAwake(true);
		std::cout << "Enemy died!" << std::endl;
		world->DestroyBody(enemy->body);
		enemy = new GameObject("assets/Meat.png", 700, 50, world, 3);
		listenerInstance.Enemy = enemy;
		player->score++;
	}
}

void Game::renderScoreSystem() 
{

	int barWidth = 200;
	int barHeight = 20;
	int barPadding = 10;
	int barXPlayer = barPadding;
	int barYPlayer = ScreenY - barHeight - barPadding;
	int barXEnemy = ScreenX - barWidth - barPadding;
	int barYEnemy = ScreenY - barHeight - barPadding;

	// Define score position and size
	int circleRadius = 30;
	int circlePadding = 10;
	int circleXPlayer = barXPlayer;
	int circleXEnemy = barXEnemy;
	int circleOffset = 60;
	int circleYPlayer = barYPlayer - circleOffset;
	int circleYEnemy = barYEnemy - circleOffset;

	//Get the score of game objects
	int playerScore = player->score;
	int enemyScore = enemy->score;

	//Score colors
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Color yellow = { 255, 255, 0, 255 };
	SDL_Color orange = { 255, 165, 0, 255 };
	SDL_Color white = { 255, 255, 255, 255 };

	// Render player square based on their score
	SDL_Rect playerCircle = { circleXPlayer, circleYPlayer, circleRadius * 2, circleRadius * 2 };
	if (playerScore == 0) 
	{
		SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
	}
	else if (playerScore >= 3) 
	{
		SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);
	}
	else if (playerScore >= 2) 
	{
		SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
	}
	else if (playerScore >= 1)
		SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);


	//Render player score square
	SDL_RenderFillRect(renderer, &playerCircle);

	// Render enemy score square based on how much score they have
	SDL_Rect enemyCircle = { circleXEnemy, circleYEnemy, circleRadius * 2, circleRadius * 2 };
	if (enemyScore <= 0) 
	{
		SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
	}
	else if (enemyScore >= 3) 
	{
		SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);
	}
	else if (enemyScore >= 2) 
	{
		SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
	}
	else if (enemyScore >= 1)
		SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);

	SDL_RenderFillRect(renderer, &enemyCircle);

	// Check if player or enemy has won
	if (playerScore >= 3) 
	{
		std::cout << "Player wins!" << std::endl;
	}
	else if (enemyScore >= 3) 
	{
		std::cout << "Enemy wins!" << std::endl;
	}
}



