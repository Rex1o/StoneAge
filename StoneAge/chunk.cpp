#include "chunk.h"
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

Chunk::Chunk(int x, int z, Perlin* perlin) : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_PositionX(x* CHUNK_SIZE_X),
m_PositionZ(z* CHUNK_SIZE_Z), m_ChunkPositionX(x), m_ChunkPositionZ(z)
{
	/*The first parameter is the number of octaves , this is how noisy or smooth the function is. This is valid between 1 and 16. A value of
	4 to 8 octaves produces fairly conventional noise results.The second parameter is the noise frequency.Values betwen 1 and 8 are
	reasonable here.You can try sampling the data and plotting it to the screen to see what numbers you like.The last parameter is
	the amplitude.Setting this to a value of 1 will return randomized samples between - 1 and +1. The last parameter is the random
	number seed .*/
	if (SearchBinary())
	{
		m_blocks.SetBlocksFromBin((BlockType*)LoadBinary());
		m_isDirty = true;
	}
	else {
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				// La methode Get accepte deux param^etre ( coordonnee en X et Z) et retourne une valeur qui respecte
				// les valeurs utilisees lors de la creation de l'objet Perlin
				// La valeur retournee est entre -1 et 1
				float val = perlin->Get((float)(m_PositionX + x) / 2000.f, (float)(m_PositionZ + z) / 2000.f);

				// Utiliser val pour determiner la hauteur du terrain a la position x,z
				int hauteur;
				if (val < 0) {
					hauteur = (OCEAN_HEIGHT * val) + OCEAN_HEIGHT;
				}
				else
				{
					hauteur = (OCEAN_HEIGHT * val) + OCEAN_HEIGHT;
				}

				if (hauteur > CHUNK_SIZE_Y)
				{
					hauteur = hauteur - (hauteur - CHUNK_SIZE_Y);
				}

				// Vous devez vous assurer que la hauteur ne depasse pas CHUNK_SIZE_Y
				// Remplir les blocs du bas du terrain jusqu 'a la hauteur calculee.
				// N' hesitez pas a jouer avec la valeur retournee pour obtenir un resultat qui vous semble satisfaisant

				for (int y = 0; y <= hauteur; y++)
				{
					
					if (y == 0) {
						SetBlock(x, y, z, BTYPE_BEDROCK);
					}
					else if (y == hauteur)
					{
						SetBlock(x, y, z, BTYPE_GRASS);
					}
					else if (y >= hauteur - 3)
					{
						SetBlock(x, y, z, BTYPE_DIRT);
					}
					else
					{
						SetBlock(x, y, z, BTYPE_STONE);
					}
				}
				for (int y = hauteur + 1; y < CHUNK_SIZE_Y; y++)
				{
					SetBlock(x, y, z, BTYPE_AIR);
				}
			}
		}
	}

	m_modified = false;
#pragma region Parcours
//
//	m_blocks.Reset(BTYPE_AIR);
//	for (int x = 0; x < CHUNK_SIZE_X; ++x)
//	{
//		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
//		{
//			for (int y = 0; y < 3; ++y)
//			{
//				SetBlock(x, y, z, BTYPE_STONE);
//			}
//		}
//	}
//
//	//Escalier
//	m_blocks.Set(7, 3, 1, BTYPE_DIRT);
//	m_blocks.Set(7, 4, 2, BTYPE_DIRT);
//	m_blocks.Set(7, 5, 3, BTYPE_DIRT);
//	m_blocks.Set(7, 6, 4, BTYPE_DIRT);
//
//	for (int i = 0; i <= 3; i++)
//	{
//		for (int j = 0; j <= 4; j++)
//		{
//			m_blocks.Set(10 + j, 3 + i, 10, BTYPE_DIRT);
//		}
//	}
//
	//// forme de base
	//for (int x = 0; x < 5; x++)
	//{
	//	m_blocks.Set(1 + x, 3, 1, BTYPE_WOOD);
	//	m_blocks.Set(1 + x, 3, 8, BTYPE_WOOD);
	//	// hauteur des murs
	//	for (int y = 0; y < 3; y++)
	//	{
	//		m_blocks.Set(1 + x, 3 + y, 1, BTYPE_WOOD);
	//		m_blocks.Set(1 + x, 3 + y, 8, BTYPE_WOOD);
	//	}
	//}

	//for (int z = 0; z < 8; z++)
	//{
	//	m_blocks.Set(1, 3, 1 + z, BTYPE_WOOD);
	//	m_blocks.Set(6, 3, 1 + z, BTYPE_WOOD);
	//	// hauteur des murs
	//	for (int y = 0; y < 3; y++)
	//	{
	//		m_blocks.Set(1, 3 + y, 1 + z, BTYPE_WOOD);
	//		m_blocks.Set(6, 3 + y, 1 + z, BTYPE_WOOD);
	//	}
	//}

	//// toit de la maison
	//for (int x = 0; x < 6; x++)
	//{
	//	for (int z = 0; z < 8; z++)
	//	{
	//		m_blocks.Set(1 + x, 6, 1 + z, BTYPE_DIRT);
	//	}
	//}

	//for (int x = 0; x < 4; x++)
	//{
	//	for (int z = 0; z < 8; z++)
	//	{
	//		m_blocks.Set(2 + x, 7, 1 + z, BTYPE_DIRT);
	//	}
	//}

	//for (int x = 0; x < 2; x++)
	//{
	//	for (int z = 0; z < 8; z++)
	//	{
	//		m_blocks.Set(3 + x, 8, 1 + z, BTYPE_DIRT);
	//	}
	//}

	//// fenetre
	//for (int z = 0; z < 4; z++)
	//{
	//	m_blocks.Set(1, 4, 3 + z, BTYPE_AIR);
	//	m_blocks.Set(6, 4, 3 + z, BTYPE_AIR);
	//}

	//// porte
	//for (int y = 0; y < 2; y++)
	//{
	//	m_blocks.Set(3, 3 + y, 8, BTYPE_AIR);
	//}

#pragma endregion

	/*std::cout << "A new chunk was built" << std::endl;*/
}

Chunk::~Chunk()
{
}

void Chunk::RemoveBlock(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
	m_modified = true;
	m_blocks.Set(x, y, z, type);
}

BlockType Chunk::GetBlock(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}

void Chunk::Update(BlockInfo* bi[])
{
	// Update mesh
	if (m_isDirty)
	{
		if (m_modified)
		{
			SaveToBinary();
			m_modified = false;
		}
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
		VertexBuffer::VertexData* vd = new VertexBuffer::VertexData[maxVertexCount];
		int count = 0;
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count > USHRT_MAX)
						break;
					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR)
					{
						//Si le block n'est pas un block d'air, aller chercher les textures du block dans l'atlass
						BlockInfo* biCourant = bi[bt];
						float u = biCourant->GetU();
						float v = biCourant->GetV();
						float w = biCourant->GetW();

						//Regarde si un des blocks à coté est de l'aire, si ce ne l'est pas, ne pas render
						//Ne fonctionne pas correctement encore
						if (z == CHUNK_SIZE_Z - 1) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 1);
						}
						else if (GetBlock(x, y, z + 1) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 1);
						}

						if (z == 0) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 2);
						}
						else if (GetBlock(x, y, z - 1) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 2);
						}

						if (x == CHUNK_SIZE_X - 1) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 3);
						}
						else if (GetBlock(x + 1, y, z) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 3);
						}

						if (x == 0) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 4);
						}
						else if (GetBlock(x - 1, y, z) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 4);
						}

						if (y == CHUNK_SIZE_Y - 1) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 5);
						}
						else if (GetBlock(x, y + 1, z) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 5);
						}

						if (y == 0) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 6);
						}
						else if (GetBlock(x, y - 1, z) == BTYPE_AIR) {
							AddBlockToMesh(vd, count, bt, x + m_PositionX, y, z + m_PositionZ, u, v, w, 6);
						}
					}
				}
			}
		}
		if (count > USHRT_MAX)
		{
			count = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}
		m_vertexBuffer.SetMeshData(vd, count);
		delete[] vd;
	}
	m_isDirty = false;
}

void Chunk::Render() const
{
	m_vertexBuffer.Render();
}

bool Chunk::IsDirty() const
{
	return m_isDirty;
}

void Chunk::Reset(BlockType type)
{
	m_blocks.Reset(type);
}

void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType bt, int x, int y, int z, float& u, float& v, float& w, byte side)
{
	// front #1
	if (side == 1) {
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v + w);
	}
	//back #2
	if (side == 2)
	{
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u, v + w);
	}
	//right #3
	if (side == 3) {
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .9f, .9f, .9f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .9f, .9f, .9f, u, v + w);
	}
	//left #4
	if (side == 4) {
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .9f, .9f, .9f, u, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .9f, .9f, .9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .9f, .9f, .9f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .9f, .9f, .9f, u, v + w);
	}
	//top #5
	if (side == 5) {
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .8f, .8f, .8f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .8f, .8f, .8f, u, v + w);
	}
	//buttom #6
	if (side == 6)
	{
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, u, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .8f, .8f, .8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .8f, .8f, .8f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, u, v + w);
	}
}

void Chunk::SetDirty()
{
	m_modified = true;
	m_isDirty = true;
}

void Chunk::UnloadChunk()
{
	// Update mesh
	int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
	VertexBuffer::VertexData* vd = new VertexBuffer::VertexData[maxVertexCount];
	m_vertexBuffer.SetMeshData(vd, 0);
	delete[] vd;
}

int Chunk::GetChunkPositionX()
{
	return m_ChunkPositionX;
}

int Chunk::GetChunkPositionZ()
{
	return m_ChunkPositionZ;
}

void Chunk::SaveToBinary() {
	char* data = new char[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	std::ofstream sortie(std::to_string(m_ChunkPositionX) + "_" + std::to_string(m_ChunkPositionZ) + ".bin", std::fstream::binary);
	sortie.write((char*)m_blocks.GetTValue(), CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
	sortie.close();
	delete[] data;
}

char* Chunk::LoadBinary() {
	// Relire le fichier 
	std::ifstream entree(std::to_string(m_ChunkPositionX) + "_" + std::to_string(m_ChunkPositionZ) + ".bin", std::fstream::binary);

	char* data = new char[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	entree.read(data, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
	entree.close();

	return data;
	delete[] data;
}

bool Chunk::SearchBinary() {
	std::ifstream search(std::to_string(m_ChunkPositionX) + "_" + std::to_string(m_ChunkPositionZ) + ".bin", std::fstream::binary);

	return search.is_open();
}

