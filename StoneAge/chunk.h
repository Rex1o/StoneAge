#ifndef CHUNK_H__
#define CHUNK_H__

#include "array3d.h"
#include "define.h"
#include "vertexbuffer.h"
#include <iostream>
#include "blockinfo.h"
#include "perlin.h"

class Chunk
{
public:
	Chunk(int x, int z, Perlin* perlin);
	~Chunk();

	void RemoveBlock(int x, int y, int z);
	void SetBlock(int x, int y, int z, BlockType type);
	BlockType GetBlock(int x, int y, int z);
	void Update(BlockInfo* bi[]);
	void Render() const;
	bool IsDirty() const;
	void Reset(BlockType type);
	void AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType bt, int x, int y, int z, float& u, float& v, float& w, byte side);
	void SetDirty();
	void UnloadChunk();
	int GetChunkPositionX();
	int GetChunkPositionZ();
	void SaveToBinary();
	char* LoadBinary();
	bool SearchBinary();
private:
	Array3d<BlockType> m_blocks;
	VertexBuffer m_vertexBuffer;
	bool m_isDirty = true;
	bool m_modified = false;
	float m_PositionX, m_PositionZ;
	int m_ChunkPositionX, m_ChunkPositionZ;
};

#endif // CHUNK_H__
