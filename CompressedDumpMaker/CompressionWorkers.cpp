#include "stdafx.h"
#include "CompressionWorkers.h"

#include "lz4mtLib/lz4mt.h"
#include "lz4mtLib/lz4/lz4hc.h"

using namespace std;

typedef pair<MemoryListStreamPartitioner::DescriptorsQueue*, HANDLE> CompressReadCtx;

LZ4CompressionWorker::LZ4CompressionWorker()
{
}


LZ4CompressionWorker::~LZ4CompressionWorker()
{
}

bool LZ4CompressionWorker::compressMemoryChunks(MemoryListStreamPartitioner::DescriptorsQueue* pCompressionChunks, HANDLE hOutputFile, HANDLE hTargetProcess)
{
	CompressReadCtx readCtx(pCompressionChunks, hTargetProcess);


	Lz4MtContext ctx = lz4mtInitContext();
	ctx.mode = Lz4MtMode::LZ4MT_MODE_PARALLEL;
	ctx.read = LZ4CompressionWorker::readFromChunksQueue;
	ctx.readCtx = &readCtx;
	ctx.write = LZ4CompressionWorker::writeToCompressedFile;
	ctx.writeCtx = hOutputFile;
	ctx.compressionLevel = 1; //max compression level
	ctx.compress = LZ4_compressHC2_limitedOutput;

	Lz4MtStreamDescriptor streamDesc = lz4mtInitStreamDescriptor();
	streamDesc.bd.blockMaximumSize = 7; //actual buffer size is 1 << 8 * (2 ^ val). For 2 I get 2^24 

	auto result = lz4mtCompress(&ctx, &streamDesc);

	return true;
}

int LZ4CompressionWorker::readFromChunksQueue(Lz4MtContext* ctx, void* dst, int dstSize)
{
	assert(ctx->readCtx != nullptr);

	auto pReadCtx = static_cast<CompressReadCtx*>(ctx->readCtx);

	auto pCompressionChunks = pReadCtx->first;
	auto hTargetProc = pReadCtx->second;

	const size_t SERVICE_DATA_SIZE = sizeof(ULONG64) * 2;

	//If buffer is not enough to fit data.
	if (dstSize < MEM_COPY_BUFFER_SIZE + SERVICE_DATA_SIZE)
	{
		return 0;
	}

	if (pCompressionChunks->empty())
	{
		return 0;
	}

	auto& chunk = pCompressionChunks->front();

	PULONG64 dstBuff = static_cast<PULONG64>(dst);
	PULONG64 pDstMemStreamDestOffset = dstBuff;
	PULONG64 pDstSize = dstBuff + 1;
	PULONG64 pDstData = dstBuff + 2;

	*pDstMemStreamDestOffset = chunk.MemStreamDestOffset;

	SIZE_T actualCopied;
	if (::ReadProcessMemory(hTargetProc, chunk.TargetProcessMemOffset, pDstData, static_cast<SIZE_T>(chunk.RegionSize), &actualCopied) == 0)
	{
		*pDstSize = 0;
		return SERVICE_DATA_SIZE;
	}

	*pDstSize = actualCopied;

	pCompressionChunks->pop();

	return static_cast<int>(actualCopied + chunk.RegionSize);
}

int LZ4CompressionWorker::writeToCompressedFile(const Lz4MtContext* ctx, const void* source, int sourceSize)
{
	assert(ctx->writeCtx != nullptr);

	HANDLE hFileOut = reinterpret_cast<HANDLE>(ctx->writeCtx);

	DWORD bytesWritten = 0;
	::WriteFile(hFileOut, source, sourceSize, &bytesWritten, nullptr);

	return bytesWritten;
}