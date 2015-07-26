#pragma once
#include "MemoryListStreamPartitioner.h"

struct Lz4MtContext;

class CompressionWorkerBase
{
public:

	virtual ~CompressionWorkerBase()
	{
	}

	virtual bool compressMemoryChunks(MemoryListStreamPartitioner::DescriptorsQueue* pCompressionChunks, HANDLE hOutputFile, HANDLE hTargetProcess) = 0;
	virtual const wchar_t* getFileExtension() = 0;

};




class LZ4CompressionWorker :
	public CompressionWorkerBase
{
public:
	LZ4CompressionWorker();
	virtual ~LZ4CompressionWorker();

	bool compressMemoryChunks(MemoryListStreamPartitioner::DescriptorsQueue* pCompressionChunks, HANDLE hOutputFile, HANDLE hTargetProcess) override;
	const wchar_t* getFileExtension() override
	{
		return L".dmemLZ4";
	}

private:
	static int readFromChunksQueue(Lz4MtContext* ctx, void* dst, int dstSize);
	static int writeToCompressedFile(const Lz4MtContext* ctx, const void* source, int sourceSize);

};
