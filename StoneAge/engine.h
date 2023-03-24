#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"
#include "shader.h"
#include "chunk.h"
#include "textureatlas.h"
#include "blockinfo.h"
#include "array2d.h"
#include "epsilon.h"

class Engine : public OpenglContext
{
public:
	Engine();
	virtual ~Engine();
	virtual void Init();
	virtual void DeInit();
	virtual void LoadResource();
	virtual void UnloadResource();
	virtual void Render(float elapsedTime);
	virtual void KeyPressEvent(unsigned char key);
	virtual void KeyReleaseEvent(unsigned char key);
	virtual void MouseMoveEvent(int x, int y);
	virtual void MousePressEvent(const MOUSE_BUTTON& button, int x, int y);
	virtual void MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y);
	virtual void DrawHud();
	virtual void PrintText(unsigned int x, unsigned int y, const std::string& t);
	std::string GetFps();
	Chunk* ChunkAt(float x, float y, float z) const;
	Chunk* ChunkAt(const Vector3f& pos) const;
	BlockType BlockAt(float x, float y, float z, BlockType defaultBlockType) const;
	void GetBlocAtCursor();
	void PlaceBlock(BlockType block, bool destroy = false);
	void SetBlockType();
	Vector3f Engine::BlockAt(float x, float y, float z) const;
private:
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
	bool m_wireframe = false;
	//51561651561981320
	//Génération de terrain
	Perlin* m_perlin = new Perlin(8, 7, 1, 69);
	int m_ChunkRelatifx, m_ChunkRelatifz;

	int m_PlayerChunkx = 0;
	int m_PlayerChunkz = 0;

	int m_LoadingFrames = NULL;
	int m_LoadingX = 0;
	int m_LoadingZ= 0;

	int m_IsLoadingXPositive = false;
	int m_IsLoadingXNegative = false;
	int m_IsLoadingYPositive = false;
	int m_IsLoadingYNegative = false;
	//Texture HUD
	Texture m_textureCrosshair;
	Texture m_textureFont;

	//Texture block
	TextureAtlas m_textureAtlas;
	BlockInfo* m_bi[BTYPE_LAST];
	BlockType blockPlace = BTYPE_STONE;

	Player m_player;
	Shader m_shader01;

	//Gestions des Chunks
	Array2d<Chunk*> m_activeChunks;

	//Le bloc auquel la souris pointe
	Vector3f m_currentBlock;

	//La face du bloc sur laquelle la souris pointe
	Vector3f m_currentFaceNormal;

	bool m_keyW = false;
	bool m_keyA = false;
	bool m_keyS = false;
	bool m_keyD = false;
	bool m_keyG = false;
	bool m_leftCTRL = false;
	bool m_spaceBar = false;
	float m_currentFieldOfView = FIELD_OF_VIEW;

	int m_fps;
};

#endif // ENGINE_H__
