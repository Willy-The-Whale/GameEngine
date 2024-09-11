#include "Map.hpp"
#include "TextureManager.hpp"

int lvl1[20][25] = { 
	{0,3,3,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0}, 
	{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,1,1,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,1,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} 
};

Map::Map(b2World* worldIn) {
	
	dirt = TextureManager::LoadTexture("assets/dirt.png");
	grass = TextureManager::LoadTexture("assets/grass.png");
	water = TextureManager::LoadTexture("assets/water.png");
	brick = TextureManager::LoadTexture("assets/brick.png");
	lava = TextureManager::LoadTexture("assets/lava.png");
	wood = TextureManager::LoadTexture("assets/wood.png");
	ice = TextureManager::LoadTexture("assets/ice.png");
	rock = TextureManager::LoadTexture("assets/rock.png");
	marsh = TextureManager::LoadTexture("assets/marsh.png");

	LoadMap(lvl1);

	src.x = src.y = 0;
	src.w = dest.w = 32;
	src.h = dest.h = 32;

	dest.x = dest.y = 0;
	world = worldIn;
	bodyType = -1;

	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {

			bodies[row][col] = NULL;
		}
	}
}

void Map::LoadMap(int arr[20][25]) {

	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {
			
			map[row][col] = arr[row][col];
		}
	}

}

void Map::GuiMap(int arr[20][25]) {
	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {
			arr[row][col] = map[row][col];
		}
	}
}

// Writing to file
void Map::writeToFile(int level[20][25], const char* levelName) {
	ifstream fileIn;
	fileIn.open("map.txt"); // open the existing file
	ofstream fileOut("map.txt", ios::app); // open file in append mode
	string line;
	int mapCount = 0;
	while (getline(fileIn, line)) {
		if (line.size() > 0 && line[0] == 'm') {
			mapCount++;
		}
	}
	fileOut << "map" << mapCount +1 << "\n";
	fileOut << levelName << "\n";
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 25; j++) {
			fileOut << level[i][j] << " ";
		}
		fileOut << "\n";
	}
	fileOut.close();
}

// Reading from file
void Map::readFromFile( int levelNumber) {
	ifstream file("map.txt");
	string line;
	int currentMap = 0;
	bool foundMap = false;
	while (getline(file, line)) {
		if (line.size() > 0 && line[0] == 'm') {
			currentMap++;
			if (currentMap == levelNumber) {
				foundMap = true;
			}
			else {
				foundMap = false;
			}
		}
		else if (foundMap) {
			stringstream ss(line);
			for (int i = 0; i < 20; i++) {
				for(int j = 0;j<25;j++)
				ss >> map[i][j];
			}
		}
	}
	file.close();
	if (!foundMap) {
		cout << "Map not found!\n";
	}
	
}


void Map::DrawMap() {

	int type = 0;

	for (int row = 0; row < 20; row++) {
		for (int col = 0; col < 25; col++) {

			type = map[row][col];

			//when our row and col are 0 we are going to draw at the top left 0,0
			//when row is 0 and col is 1 we are giong to move 32 pixel apart from our destination
			dest.x = col * 32;
			dest.y = row * 32;

			switch (type) {

			case 0:
				if (bodies[row][col] != NULL)
				{
					world->DestroyBody(bodies[row][col]);
					bodies[row][col] = NULL;
					cout << "Body Destroyed" << endl;
				}
				TextureManager::Draw(water, src, dest);
				break;

			case 1:
				if (bodies[row][col] != NULL)
				{
					world->DestroyBody(bodies[row][col]);
					bodies[row][col] = NULL;
					cout << "Body Destroyed" << endl;
				}
				TextureManager::Draw(grass, src, dest);
				break;

			case 2:
			{
				TextureManager::Draw(dirt, src, dest);
				if (bodies[row][col] ==NULL)
				{
					b2BodyDef groundDef;
					b2PolygonShape pshape2;
					b2FixtureDef def;
					groundDef.type = b2_staticBody;
					float PhysX = (float)dest.x / 100;
					float PhysY = (float)dest.y / 100;
					groundDef.position.Set(PhysX + .160f, PhysY + .160f);
					groundDef.userData.pointer = bodyType;
					b2Body* ground = world->CreateBody(&groundDef);
					pshape2.SetAsBox(.160f, .160f);
					def.shape = &pshape2;
					def.userData.pointer = bodyType; // -1
					b2Fixture* fix = ground->CreateFixture(&def);
					bodies[row][col] = ground;
				}
				
				break;
			}
			case 3:
			{
				TextureManager::Draw(brick, src, dest);
				if (bodies[row][col] == NULL)
				{
					b2BodyDef groundDef;
					b2PolygonShape pshape2;
					b2FixtureDef def;
					groundDef.type = b2_staticBody;
					float PhysX = (float)dest.x / 100;
					float PhysY = (float)dest.y / 100;
					groundDef.position.Set(PhysX + .160f, PhysY + .160f);
					groundDef.userData.pointer = bodyType;
					b2Body* ground = world->CreateBody(&groundDef);
					pshape2.SetAsBox(.160f, .160f);
					def.shape = &pshape2;
					def.userData.pointer = bodyType; // -1
					b2Fixture* fix = ground->CreateFixture(&def);
					bodies[row][col] = ground;
				}
				break;
			}
			case 4:
			{
				if (bodies[row][col] != NULL)
				{
					world->DestroyBody(bodies[row][col]);
					bodies[row][col] = NULL;
				}
				TextureManager::Draw(lava, src, dest);
				break;
			}
			case 5:
			{
				TextureManager::Draw(wood, src, dest);
				if (bodies[row][col] == NULL)
				{
					b2BodyDef groundDef;
					b2PolygonShape pshape2;
					b2FixtureDef def;
					groundDef.type = b2_staticBody;
					float PhysX = (float)dest.x / 100;
					float PhysY = (float)dest.y / 100;
					groundDef.position.Set(PhysX + .160f, PhysY + .160f);
					groundDef.userData.pointer = bodyType;
					b2Body* ground = world->CreateBody(&groundDef);
					pshape2.SetAsBox(.160f, .160f);
					def.shape = &pshape2;
					def.userData.pointer = bodyType; // -1
					b2Fixture* fix = ground->CreateFixture(&def);
					bodies[row][col] = ground;
				}
				break;
			}
			case 6:
			{
				TextureManager::Draw(ice, src, dest);
				if (bodies[row][col] == NULL)
				{
					b2BodyDef groundDef;
					b2PolygonShape pshape2;
					b2FixtureDef def;
					groundDef.type = b2_staticBody;
					float PhysX = (float)dest.x / 100;
					float PhysY = (float)dest.y / 100;
					groundDef.position.Set(PhysX + .160f, PhysY + .160f);
					groundDef.userData.pointer = bodyType;
					b2Body* ground = world->CreateBody(&groundDef);
					pshape2.SetAsBox(.160f, .160f);
					def.shape = &pshape2;
					def.userData.pointer = bodyType; // -1
					b2Fixture* fix = ground->CreateFixture(&def);
					bodies[row][col] = ground;
				}
				break;
			}
			case 7:
			{
				TextureManager::Draw(rock, src, dest);
				if (bodies[row][col] == NULL)
				{
					b2BodyDef groundDef;
					b2PolygonShape pshape2;
					b2FixtureDef def;
					groundDef.type = b2_staticBody;
					float PhysX = (float)dest.x / 100;
					float PhysY = (float)dest.y / 100;
					groundDef.position.Set(PhysX + .160f, PhysY + .160f);
					groundDef.userData.pointer = bodyType;
					b2Body* ground = world->CreateBody(&groundDef);
					pshape2.SetAsBox(.160f, .160f);
					def.shape = &pshape2;
					def.userData.pointer = bodyType; // -1
					b2Fixture* fix = ground->CreateFixture(&def);
					bodies[row][col] = ground;
				}
				break;
			}
			case 8:
			{
				if (bodies[row][col] != NULL)
				{
					world->DestroyBody(bodies[row][col]);
					bodies[row][col] = NULL;
				}
				TextureManager::Draw(marsh, src, dest);
				break;
			}
			default:
				break;
			}

		}
	}

}