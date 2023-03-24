#include "engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "transformation.h"
#include "player.h"

Engine::Engine() : m_textureAtlas(BTYPE_LAST), m_activeChunks((VIEW_DISTANCE * 2) / CHUNK_SIZE_X, (VIEW_DISTANCE * 2) / CHUNK_SIZE_Z)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
	{
		std::cerr << " ERREUR GLEW : " << glewGetErrorString(glewErr) << std::endl;
		abort();
	}

	glEnable(GL_CULL_FACE); //Ne pas oublier de décommenter avant la remise

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);

	// Light
	GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
	GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	int positionChunkx;
	int positionChunkz;
	Vector3f playerPostion = m_player.GetPosition();
	//Initialisation des Chunk relatif à la position du joueur
	for (int i = 0; i < (VIEW_DISTANCE * 2) / CHUNK_SIZE_X; i++)
	{
		positionChunkx = (int)(playerPostion.x / CHUNK_SIZE_X) + ((i)-((VIEW_DISTANCE * 2) / CHUNK_SIZE_X / 2));

		for (int j = 0; j < (VIEW_DISTANCE * 2) / CHUNK_SIZE_Z; j++)
		{
			positionChunkz = (int)(playerPostion.z / CHUNK_SIZE_Z) - ((j)-((VIEW_DISTANCE * 2) / CHUNK_SIZE_Z / 2) + 1);
			m_activeChunks.Set(i, j, new Chunk(positionChunkx, positionChunkz, m_perlin));
			if (i == 0 && j == 0) {
				m_ChunkRelatifx = positionChunkx;
				m_ChunkRelatifz = positionChunkz;
			}
		}
	}

	//Permet de load les shaders
	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << " Failed to load shader " << std::endl;
		exit(1);
	}

	float u, v, w;

	//TextureHUD

	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");

#pragma region TextureBlock

	// Texture Dirt
	TextureAtlas::TextureIndex texDirtIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.png");
	m_textureAtlas.TextureIndexToCoord(texDirtIndex, u, v, w, w);
	m_bi[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Dirt");
	m_bi[BTYPE_DIRT]->SetUVW(u, v, w);

	// Texture Sand
	TextureAtlas::TextureIndex texSandIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "sand.png");
	m_textureAtlas.TextureIndexToCoord(texSandIndex, u, v, w, w);
	m_bi[BTYPE_SAND] = new BlockInfo(BTYPE_SAND, "Sand");
	m_bi[BTYPE_SAND]->SetUVW(u, v, w);

	//Texture Stone
	TextureAtlas::TextureIndex texStoneIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "stone.png ");
	m_textureAtlas.TextureIndexToCoord(texStoneIndex, u, v, w, w);
	m_bi[BTYPE_STONE] = new BlockInfo(BTYPE_STONE, "Stone");
	m_bi[BTYPE_STONE]->SetUVW(u, v, w);

	//Texture Wood
	TextureAtlas::TextureIndex texWoodIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "wood.png");
	m_textureAtlas.TextureIndexToCoord(texWoodIndex, u, v, w, w);
	m_bi[BTYPE_WOOD] = new BlockInfo(BTYPE_WOOD, "Wood");
	m_bi[BTYPE_WOOD]->SetUVW(u, v, w);

	//Texture BedRock
	TextureAtlas::TextureIndex texBedRockIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "bedrock.png");
	m_textureAtlas.TextureIndexToCoord(texBedRockIndex, u, v, w, w);
	m_bi[BTYPE_BEDROCK] = new BlockInfo(BTYPE_BEDROCK, "Bedrock");
	m_bi[BTYPE_BEDROCK]->SetUVW(u, v, w);

	//Texture Snow
	TextureAtlas::TextureIndex texSnowIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "snow.png");
	m_textureAtlas.TextureIndexToCoord(texSnowIndex, u, v, w, w);
	m_bi[BTYPE_SNOW] = new BlockInfo(BTYPE_SNOW, "Snow");
	m_bi[BTYPE_SNOW]->SetUVW(u, v, w);

	//Texture Grass
	TextureAtlas::TextureIndex texGrassIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "grass.png");
	m_textureAtlas.TextureIndexToCoord(texGrassIndex, u, v, w, w);
	m_bi[BTYPE_GRASS] = new BlockInfo(BTYPE_SNOW, "Grass");
	m_bi[BTYPE_GRASS]->SetUVW(u, v, w);

	if (!m_textureAtlas.Generate(128, false))
	{
		std::cout << " Unable to generate texture atlas ..." << std::endl;
		abort();
	}

#pragma endregion

}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
	if (elapsedTime > .2f) {
		return;
	}

	static float gameTime = elapsedTime;

	gameTime += elapsedTime;

	m_fps = 1 / elapsedTime;

	if (m_leftCTRL)
	{
		if (m_currentFieldOfView < FIELD_OF_VIEW + SPRINT_FIELD_OF_VIEW)
		{
			if (m_currentFieldOfView > FIELD_OF_VIEW + SPRINT_FIELD_OF_VIEW) {
				m_currentFieldOfView = FIELD_OF_VIEW + SPRINT_FIELD_OF_VIEW;
			}
			else
			{
				m_currentFieldOfView += 0.05 / elapsedTime;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_currentFieldOfView, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
		}
	}
	else if (m_currentFieldOfView != FIELD_OF_VIEW)
	{
		if (m_currentFieldOfView > FIELD_OF_VIEW) {
			if (m_currentFieldOfView < FIELD_OF_VIEW) {
				m_currentFieldOfView = FIELD_OF_VIEW;
			}
			else
			{
				m_currentFieldOfView -= 0.05 / elapsedTime;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(m_currentFieldOfView, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Transformation m_playerTransformation;

#pragma region Déplacements du joueur

	Vector3f pos = m_player.GetPosition();
	Vector3f delta = m_player.SimulateMove(m_keyW, m_keyS, m_keyA, m_keyD, m_leftCTRL, elapsedTime);

	BlockType bt_top, bt_middle, bt_bottom;

	// Collision par rapport au déplacement en x:
	bt_top = BlockAt(pos.x + delta.x, pos.y, pos.z, BTYPE_AIR);
	bt_middle = BlockAt(pos.x + delta.x, pos.y - (m_player.GetHeight() / 2), pos.z, BTYPE_AIR);
	bt_bottom = BlockAt(pos.x + delta.x, pos.y - m_player.GetHeight(), pos.z, BTYPE_AIR);
	if (bt_top != BTYPE_AIR || bt_middle != BTYPE_AIR || bt_bottom != BTYPE_AIR)
		delta.x = 0;

	//Faire tomber le jouer si aucun block n'est en dessous de lui
	bt_bottom = BlockAt(pos.x, pos.y + delta.y - m_player.GetHeight(), pos.z, BTYPE_AIR);
	if ((bt_bottom == BTYPE_AIR || m_spaceBar) && !m_player.m_godmode)
	{
		if (m_player.IsJumping())
		{
			delta = delta + m_player.GetVelocity() * elapsedTime;
			m_player.SetVelocity(m_player.GetVelocity() + m_player.GetGravity() * elapsedTime);
			if (m_player.GetVelocity().y <= 0)
			{
				m_player.Falling(true);
			}
		}
		else if (!m_player.IsJumping())
		{
			m_player.Falling(true);
			delta = delta + m_player.GetVelocity() * elapsedTime;
			m_player.SetVelocity(m_player.GetVelocity() + m_player.GetGravity() * elapsedTime);
		}
	}

	// Collision par rapport au déplacement en y:
	bt_top = BlockAt(pos.x, pos.y + delta.y, pos.z, BTYPE_AIR);
	bt_middle = BlockAt(pos.x, pos.y + delta.y - (m_player.GetHeight() / 2), pos.z, BTYPE_AIR);
	bt_bottom = BlockAt(pos.x, pos.y + delta.y - m_player.GetHeight(), pos.z, BTYPE_AIR);
	if (bt_top != BTYPE_AIR || bt_middle != BTYPE_AIR || bt_bottom != BTYPE_AIR)
	{
		m_player.Falling(false);
		m_player.SetJumping(false);
		m_player.SetVelocity(Vector3f(0, 0, 0));
		delta.y = 0;
	}

	// Collision par rapport au déplacement en z:
	bt_top = BlockAt(pos.x, pos.y, pos.z + delta.z, BTYPE_AIR);
	bt_middle = BlockAt(pos.x, pos.y - (m_player.GetHeight() / 2), pos.z + delta.z, BTYPE_AIR);
	bt_bottom = BlockAt(pos.x, pos.y - m_player.GetHeight(), pos.z + delta.z, BTYPE_AIR);
	if (bt_top != BTYPE_AIR || bt_middle != BTYPE_AIR || bt_bottom != BTYPE_AIR)
		delta.z = 0;

	// glissement angle face au bloc / ou diminuer deplacement x ou z

	pos += delta;
	m_player.SetPosition(pos);
#pragma endregion

#pragma region Loading de chunk dynamiques
	//Calculer limite de la render distance du joueur
	int coordLimitLeft = m_ChunkRelatifx * 16;
	int coordLimitTop = ((m_ChunkRelatifz + 1) * 16);
	int coordLimitRight = (m_ChunkRelatifx + m_activeChunks.GetSizeX()) * 16;
	int coordLimitBottom = (m_ChunkRelatifz - m_activeChunks.GetSizeY() + 1) * 16;
	//Comparer la position du joueur avec la RenderDistance

	if (pos.x + m_activeChunks.GetSizeX() / 2 * CHUNK_SIZE_X > coordLimitRight + CHUNK_SIZE_X || m_IsLoadingXPositive) {//Est

		m_IsLoadingXPositive = true;
		if (m_LoadingX == 0) {
			m_ChunkRelatifx += 1;
		}

		//Ajouter les chunks à l'est
		if (m_LoadingX == 0) {

			//Offset tout les chunks de 1 vers l'ouest
			for (int i = 1; i < m_activeChunks.GetSizeX(); i++) {
				for (int j = 0; j < m_activeChunks.GetSizeY(); j++)
				{
					m_activeChunks.Set(i - 1, j, m_activeChunks.Get(i, j));
				}
			}
		}
		if (m_LoadingX % 10 == 0) {

			m_activeChunks.Set(m_activeChunks.GetSizeX() - 1, m_LoadingX / 10, new Chunk(m_ChunkRelatifx + m_activeChunks.GetSizeX() - 1, (m_ChunkRelatifz - m_LoadingX / 10), m_perlin));

		}
		if (m_LoadingX >= 10 * (m_activeChunks.GetSizeX() - 1)) {
			m_IsLoadingXPositive = false;
			m_LoadingX = 0;
		}
		else
		{
			m_LoadingX++;
		}

	}
	else if (pos.x - CHUNK_SIZE_X * m_activeChunks.GetSizeX() / 2 < coordLimitLeft - CHUNK_SIZE_X || m_IsLoadingXNegative)//Ouest
	{
		m_IsLoadingXNegative = true;
		if (m_LoadingX == 0) {
			m_ChunkRelatifx -= 1;
		}
		//Offset tout les chunks de 1 vers l'est
		if (m_LoadingX == 0) {
			for (int i = m_activeChunks.GetSizeX() - 2; i >= 0; i--) {
				for (int j = 0; j < m_activeChunks.GetSizeY(); j++)
				{
					m_activeChunks.Set(i + 1, j, m_activeChunks.Get(i, j));
				}
			}
		}
		//Ajouter les chunks à l'ouest

		if (m_LoadingX % 10 == 0) {
			m_activeChunks.Set(0, m_LoadingX / 10, new Chunk(m_ChunkRelatifx, m_ChunkRelatifz - m_LoadingX / 10, m_perlin));
		}
		if (m_LoadingX >= 10 * (m_activeChunks.GetSizeX() - 1)) {
			m_IsLoadingXNegative = false;
			m_LoadingX = 0;
		}
		else
		{
			m_LoadingX++;
		}
	}
	else if (pos.z + CHUNK_SIZE_Z * m_activeChunks.GetSizeY() / 2 > coordLimitTop + CHUNK_SIZE_Z || m_IsLoadingYPositive) {//nord
		m_IsLoadingYPositive = true;
		if (m_LoadingZ == 0) {
			m_ChunkRelatifz += 1;
		}


		//offset le chunk de 1 vers le sud
		if (m_LoadingZ == 0) {
			for (int i = m_activeChunks.GetSizeY() - 2; i >= 0; i--) {
				for (int j = 0; j < m_activeChunks.GetSizeX(); j++)
				{
					m_activeChunks.Set(j, i + 1, m_activeChunks.Get(j, i));
				}
			}
		}

		//Ajouter les chunks au nord

		if (m_LoadingZ % 10 == 0) {
			m_activeChunks.Set(m_LoadingZ / 10, 0, new Chunk(m_ChunkRelatifx + m_LoadingZ / 10, m_ChunkRelatifz, m_perlin));
		}
		if (m_LoadingZ >= 10 * (m_activeChunks.GetSizeX() - 1)) {
			m_IsLoadingYPositive = false;
			m_LoadingZ = 0;
		}
		else
		{
			m_LoadingZ++;
		}



	}
	else if (pos.z - CHUNK_SIZE_Z * m_activeChunks.GetSizeY() / 2 < coordLimitBottom - CHUNK_SIZE_Z || m_IsLoadingYNegative) {//sud
		//Offset tout les chunks de 1 ver le nord
		m_IsLoadingYNegative = true;
		if (m_LoadingZ == 0) {
			m_ChunkRelatifz -= 1;
		}

		if (m_LoadingZ == 0) {
			for (int i = 1; i < m_activeChunks.GetSizeY(); i++) {
				for (int j = 0; j < m_activeChunks.GetSizeX(); j++)
				{
					m_activeChunks.Set(j, i - 1, m_activeChunks.Get(j, i));
				}
			}
		}


		//Ajouter les chunks au sud
		if (m_LoadingZ % 10 == 0) {
			m_activeChunks.Set(m_LoadingZ / 10, m_activeChunks.GetSizeY() - 1, new Chunk(m_ChunkRelatifx + m_LoadingZ / 10, m_ChunkRelatifz - m_activeChunks.GetSizeY() + 1, m_perlin));
		}
		if (m_LoadingZ >= 10 * (m_activeChunks.GetSizeX() - 1)) {
			m_IsLoadingYNegative = false;
			m_LoadingZ = 0;
		}
		else
		{
			m_LoadingZ++;
		}

	}
#pragma endregion

	m_player.ApplyTransformation(m_playerTransformation);
	m_playerTransformation.Use();

	//vérifie si le chunk a été modifier, si oui, changer l'affichage de celui-ci
	for (int i = 0; i < (VIEW_DISTANCE * 2) / CHUNK_SIZE_X; i++)
	{
		for (int j = 0; j < (VIEW_DISTANCE * 2) / CHUNK_SIZE_Z; j++)
		{
			if (m_activeChunks.Get(i, j)->IsDirty()) {
				m_activeChunks.Get(i, j)->Update(m_bi);
			}
		}
	}

	m_shader01.Use();

	//Moves the entire world
	m_playerTransformation.ApplyTranslation(.5f, .5f, .5f);
	m_playerTransformation.Use();
	m_textureAtlas.Bind();

	for (int i = 0; i < (VIEW_DISTANCE * 2) / CHUNK_SIZE_X; i++)
	{
		for (int j = 0; j < (VIEW_DISTANCE * 2) / CHUNK_SIZE_Z; j++)
		{
			m_activeChunks.Get(i, j)->Render();
		}
	}

	Shader::Disable();

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DrawHud();
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Searches the bloc at which the mouse points
	GetBlocAtCursor();
	std::cout << "The player is looking at the block x y z " << m_currentBlock.x << " / " << m_currentBlock.y << " / " << m_currentBlock.z << " Side: " << m_currentFaceNormal << std::endl;
}

void Engine::KeyPressEvent(unsigned char key)
{
	switch (key)
	{
	case 36: // ESC
		Stop();
		break;
	case 94: // F10
		SetFullscreen(!IsFullscreen());
		break;
	case 0:
		m_keyA = true;
		//std::cout << "A keypress" << std::endl;
		break;
	case 18:
		m_keyS = true;
		//std::cout << "S keypress" << std::endl;
		break;
	case 3:
		m_keyD = true;
		//std::cout << "D keypress" << std::endl;
		break;
	case 22:
		m_keyW = true;
		//std::cout << "W keypress" << std::endl;
		break;
	case 6:
		break;
	case 37:
		m_leftCTRL = true;
		break;
	case 57:
		m_player.Jump();
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
	}
}

void Engine::KeyReleaseEvent(unsigned char key)
{
	switch (key)
	{
	case 24: // Y
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 0:
		m_keyA = false;
		break;
	case 18:
		m_keyS = false;
		break;
	case 3:
		m_keyD = false;
		break;
	case 22:
		m_keyW = false;
		break;
	case 6:
		m_player.SetGodmode();
		std::cout << "God mode was changed" << std::endl;
		break;
	case 37:
		m_leftCTRL = false;
		break;
	case 57:
		m_spaceBar = false;
		break;
	}
}

void Engine::MouseMoveEvent(int x, int y)
{
	if (x == (Width() / 2) && y == (Height() / 2))
	{
		return;
	}
	else
	{
		MakeRelativeToCenter(x, y);
		m_player.TurnLeftRight((float)y);
		m_player.TurnTopBottom((float)x);
		CenterMouse();
	}
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y)
{
	switch (button)
	{
	case 1: // left click
		PlaceBlock(BTYPE_AIR, true);
		break;
	case 4: // right click
		PlaceBlock(blockPlace);
		break;
	case 2:
		SetBlockType();
		break;
	default:
		break;
	}
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
	texture.Load(filename);
	if (!texture.IsValid())
	{
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if (stopOnError)
			Stop();

		return false;
	}

	return true;
}

void Engine::DrawHud()
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// Bind de la texture pour le font
	m_textureFont.Bind();
	std::ostringstream ss;
	ss << " Fps : " << GetFps();
	PrintText(10, Height() - 25, ss.str());
	ss.str("");
	if (m_player.m_godmode) {
		ss << " GODMODE : ON";
		PrintText(10, Height() - 50, ss.str());
		ss.str("");
	}
	else {
		ss << " GODMODE : OFF";
		PrintText(10, Height() - 50, ss.str());
		ss.str("");
	}
	ss << " Position : " << m_player.Position(); // IMPORTANT : on utilise l' operateur << pour afficher la position
	PrintText(10, 10, ss.str());
	// Affichage du crosshair
	m_textureCrosshair.Bind();
	static const int crossSize = 32;
	glLoadIdentity();
	glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(crossSize, 0);
	glTexCoord2f(1, 1);
	glVertex2i(crossSize, crossSize);
	glTexCoord2f(0, 1);
	glVertex2i(0, crossSize);
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
	glLoadIdentity();
	glTranslated(x, y, 0);
	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 32) / 16) / 16.0f;
		top += 0.5f;
		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2f(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2f(12, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2f(12, 12);
		glTexCoord2f(left, 1.0f - top);
		glVertex2f(0, 12);
		glEnd();
		glTranslated(8, 0, 0);
	}
}

std::string Engine::GetFps()
{
	return std::to_string(m_fps);
}

Chunk* Engine::ChunkAt(const Vector3f& pos) const
{
	return ChunkAt(pos.x, pos.y, pos.z);
}

Chunk* Engine::ChunkAt(float x, float y, float z) const
{
	int Chunkx = (int)(x / CHUNK_SIZE_X);
	int Chunkz = (int)(z / CHUNK_SIZE_Z);
	int activeChunkx = 0;
	int activeChunkz = 0;

	if (x >= 0) {
		if (z >= 0) {//Quadrant 1 //PARFAIT
			if (m_ChunkRelatifx < 0 && Chunkx >= 0) { //Fonctione
				activeChunkx = -m_ChunkRelatifx + Chunkx;
			}
			else if (m_ChunkRelatifx >= 0) {//Si le x est dans le quadrant 1
				activeChunkx = Chunkx - m_ChunkRelatifx;
			}


			if (m_ChunkRelatifz >= 0 && Chunkz >= 0) {//Fonctionne pour le quadrant1
				activeChunkz = m_ChunkRelatifz - Chunkz;
			}
		}
		else if (z < 0) {//Quadrant 4 //PARFAIT
			Chunkz -= 1;
			if (m_ChunkRelatifx < 0 && Chunkx >= 0) {
				activeChunkx = -m_ChunkRelatifx + Chunkx;
			}
			if (Chunkz < 0 && m_ChunkRelatifz >= 0) {
				activeChunkz = -Chunkz + m_ChunkRelatifz;
			}
			if (Chunkx > 0 && m_ChunkRelatifz >= 0) {
				activeChunkx = Chunkx - m_ChunkRelatifx;
			}
		}
	}
	else if (x < 0)
	{
		Chunkx -= 1;
		if (z >= 0) {//Quadrant 2
			if (m_ChunkRelatifx < 0) {
				activeChunkx = -m_ChunkRelatifx + Chunkx;
			}
			if (m_ChunkRelatifz >= 0 && Chunkz >= 0) {//Fonctionne pour le quadrant1
				activeChunkz = m_ChunkRelatifz - Chunkz;
			}
		}
		else if (z < 0) {//Quadrant 3
			Chunkz -= 1;
			if (Chunkz < 0 && m_ChunkRelatifz >= 0) {
				activeChunkz = -Chunkz + m_ChunkRelatifz;
			}

			if (m_ChunkRelatifx < 0) {
				activeChunkx = -m_ChunkRelatifx + Chunkx;
			}
		}
	}

	if (activeChunkx >= 0 && activeChunkx < CHUNK_SIZE_X && activeChunkz >= 0 && activeChunkz < CHUNK_SIZE_Z)
	{
		//std::cout << "Your are in the chunk: " << m_activeChunks.Get(activeChunkx, activeChunkz)->GetChunkPositionX() << "|" << m_activeChunks.Get(activeChunkx, activeChunkz)->GetChunkPositionZ() <<
		//	" In the array 2D: " << activeChunkx << "|" << activeChunkz << std::endl;

		//std::cout << m_activeChunks.Get(activeChunkx, activeChunkz) << std::endl;

		return m_activeChunks.Get(activeChunkx, activeChunkz);
	}
	else
	{
		return nullptr;
	}
}

BlockType Engine::BlockAt(float x, float y, float z, BlockType defaultBlockType) const
{
	Chunk* c = ChunkAt(x, y, z);

	if (!c)
		return defaultBlockType;

	//std::cout << x << " " << y << " " << z << std::endl;

	int bx = (int)x % CHUNK_SIZE_X;
	int by = (int)y % CHUNK_SIZE_Y;
	int bz = (int)z % CHUNK_SIZE_Z;
	//Flip on X
	if (x < 0) {
		bx = (CHUNK_SIZE_X - 1) - abs(bx);
	}
	//Flip on Z
	if (z < 0) {
		bz = (CHUNK_SIZE_Z - 1) - abs(bz);
	}

	return c->GetBlock(bx, by, bz);
}

Vector3f Engine::BlockAt(float x, float y, float z) const
{
	Chunk* c = ChunkAt(x, y, z);

	//std::cout << x << " " << y << " " << z << std::endl;

	int bx = (int)x % CHUNK_SIZE_X;
	int by = (int)y % CHUNK_SIZE_Y;
	int bz = (int)z % CHUNK_SIZE_Z;
	//Flip on X
	if (x < 0) {
		bx = (CHUNK_SIZE_X - 1) - abs(bx);
	}
	//Flip on Z
	if (z < 0) {
		bz = (CHUNK_SIZE_Z - 1) - abs(bz);
	}

	return Vector3f(bx, by, bz);
}

void Engine::GetBlocAtCursor()
{
	int x = Width() / 2;
	int y = Height() / 2;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


	posX += .5f;
	posZ += .5f;
	posY += .5f;
	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	std::cout << px << " " << py << " " << pz << std::endl;
	bool found = false;

	int yo = (m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length();

	if ((m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length() < MAX_SELECTION_DISTANCE)
	{



		if (px > 0 && pz >0) {
			for (int x = px - 1; !found && x <= px + 1; ++x)
			{
				for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
				{
					for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
					{
						if (z >= 0)
						{
							BlockType bt = BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);
							if (bt == BTYPE_AIR)
								continue;


							m_currentBlock.x = x;
							m_currentBlock.y = y;
							m_currentBlock.z = z;

							if (Epsilon<float>::InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.05f))
							{
								found = true;
							}
						}
					}
				}
			}
		}
		else if (px < 0 && pz > 0)//Quadrant 2
		{
			for (int x = px + 1; !found && x >= px - 1; --x)
			{
				for (int y = py - 1; !found && x <= 0 && y <= py + 1; ++y)
				{
					for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
					{
						if (z >= 0)
						{
							BlockType bt = BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);
							if (bt == BTYPE_AIR)
								continue;


							m_currentBlock.x = x;
							m_currentBlock.y = y;
							m_currentBlock.z = z;

							if (Epsilon<float>::InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.05f))
							{
								found = true;
							}
						}
					}
				}
			}
		}
		else if (px <0 && pz <0) {//Quadrant 3
			for (int x = px + 1; !found && x >= px - 1; --x)
			{
				for (int y = py - 1; !found && x <= 0 && y <= py + 1; ++y)
				{
					for (int z = pz + 1; !found && y >= 0 && z >= pz - 1; --z)
					{
						if (z <= 0)
						{
							BlockType bt = BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);
							if (bt == BTYPE_AIR)
								continue;


							m_currentBlock.x = x;
							m_currentBlock.y = y;
							m_currentBlock.z = z;

							if (Epsilon<float>::InRangeWithEpsilon((float)posX, (float)x, (float)x  +1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posZ, (float)z, (float)z  +1.f, 0.05f))
							{
								found = true;
							}
						}
					}
				}
			}
		}
		else if (px> 0 && pz < 0) { //Quadrant4
			for (int x = px - 1; !found && x <= px + 1; ++x)
			{
				for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
				{
					for (int z = pz + 1; !found && y >= 0 && z >= pz - 1; --z)
					{
						if (z <= 0)
						{
							BlockType bt = BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);
							if (bt == BTYPE_AIR)
								continue;


							m_currentBlock.x = x;
							m_currentBlock.y = y;
							m_currentBlock.z = z;

							if (Epsilon<float>::InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.05f)
								&& Epsilon<float>::InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.05f))
							{
								found = true;
							}
						}
					}
				}
			}
		}


		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouvé pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		


	}


	

	if (!found)
	{
		m_currentBlock.x = -1;
	}
	else
	{
		// Find on which face of the bloc we got a hit
		m_currentFaceNormal.Zero();

		const float epsilon = 0.020f;

		// Front et back:
		if (Epsilon<float>::EqualWithEpsilon((float)posZ, (float)m_currentBlock.z, epsilon))
			m_currentFaceNormal.z = -1;
		else if (Epsilon<float>::EqualWithEpsilon((float)posZ, (float)m_currentBlock.z + 1.f, epsilon))
			m_currentFaceNormal.z = 1;
		else if (Epsilon<float>::EqualWithEpsilon((float)posX, (float)m_currentBlock.x, epsilon))
			m_currentFaceNormal.x = -1;
		else if (Epsilon<float>::EqualWithEpsilon((float)posX, (float)m_currentBlock.x + 1.f, epsilon))
			m_currentFaceNormal.x = 1;
		else if (Epsilon<float>::EqualWithEpsilon((float)posY, (float)m_currentBlock.y, epsilon))
			m_currentFaceNormal.y = -1;
		else if (Epsilon<float>::EqualWithEpsilon((float)posY, (float)m_currentBlock.y + 1.f, epsilon))
			m_currentFaceNormal.y = 1;
	}
}

void Engine::PlaceBlock(BlockType block, bool destroy)
{
	if (!(m_currentBlock.x == -1)) {
		Chunk* blockToPlace = ChunkAt(m_currentBlock.x + m_currentFaceNormal.x, m_currentBlock.y, m_currentBlock.z + m_currentFaceNormal.z);

		int placePosX = ((int)m_currentBlock.x % CHUNK_SIZE_X) + m_currentFaceNormal.x;
		int placePosY = (int)m_currentBlock.y + m_currentFaceNormal.y;
		int placePosZ = ((int)m_currentBlock.z % CHUNK_SIZE_Z) + m_currentFaceNormal.z;

		int posX = (int)m_player.GetPosition().x % CHUNK_SIZE_X;
		int posY = (int)m_player.GetPosition().y % CHUNK_SIZE_Y;
		int posZ = (int)m_player.GetPosition().z % CHUNK_SIZE_Z;

		if (!destroy)
		{
			if (posX != placePosX || posZ != placePosZ)
			{
				if (blockToPlace->GetBlock(placePosX, placePosY, placePosZ) == BTYPE_AIR)
				{
					blockToPlace->SetBlock(placePosX, placePosY, placePosZ, block);
					blockToPlace->SetDirty();
				}
			}
		}
		else
		{
			if (blockToPlace->GetBlock((int)m_currentBlock.x % CHUNK_SIZE_X, (int)m_currentBlock.y, (int)m_currentBlock.z % CHUNK_SIZE_Z) != BTYPE_AIR
				&& blockToPlace->GetBlock((int)m_currentBlock.x % CHUNK_SIZE_X, (int)m_currentBlock.y, (int)m_currentBlock.z % CHUNK_SIZE_Z) != BTYPE_BEDROCK)
			{
				blockToPlace->SetBlock((int)m_currentBlock.x % CHUNK_SIZE_X, (int)m_currentBlock.y, (int)m_currentBlock.z % CHUNK_SIZE_Z, block);
				blockToPlace->SetDirty();
			}
		}
	}
}

void Engine::SetBlockType() {
	Chunk* blockToGet = ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z);

	int placePosX = (int)m_currentBlock.x % CHUNK_SIZE_X;
	int placePosY = (int)m_currentBlock.y;
	int placePosZ = (int)m_currentBlock.z % CHUNK_SIZE_Z;

	BlockType type = blockToGet->GetBlock(placePosX, placePosY, placePosZ);
	if (type != BTYPE_AIR && type != BTYPE_BEDROCK)
	{
		blockPlace = type;
	}
}