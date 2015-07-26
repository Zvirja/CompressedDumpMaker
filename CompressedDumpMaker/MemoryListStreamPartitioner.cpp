#include "stdafx.h"
#include "MemoryListStreamPartitioner.h"

using namespace std;


MemoryListStreamPartitioner::~MemoryListStreamPartitioner()
{
}

MemoryListStreamPartitioner::DescriptorsQueuePtr MemoryListStreamPartitioner::getMemoryListStreamDescriptors()
{
	DescriptorsQueuePtr result = make_unique<DescriptorsQueue>();
	ULONG64 memory64ListDataOffset = 0;

	EnumerateMemoryInfos([&result, &memory64ListDataOffset](PMINIDUMP_MEMORY_INFO mInfo)
	{
		ULONG64 bytesToCopyLeft = mInfo->RegionSize;
		BYTE* targetProcAddrToCopy = reinterpret_cast<BYTE*>(mInfo->BaseAddress);

		do
		{
			ULONG64 copyThisTime = min<ULONG64>(MEM_COPY_BUFFER_SIZE, bytesToCopyLeft);

			result->push(MemoryListStreamSegmentDescriptor(memory64ListDataOffset, copyThisTime, targetProcAddrToCopy));

			bytesToCopyLeft -= copyThisTime;
			targetProcAddrToCopy += copyThisTime;

		} while (bytesToCopyLeft > 0);

		memory64ListDataOffset += mInfo->RegionSize;
	});


	return move(result);
}

void MemoryListStreamPartitioner::EnumerateMemoryInfos(function<void(PMINIDUMP_MEMORY_INFO memInfo)> const& handler)
{
	PMINIDUMP_MEMORY_INFO_LIST pMemoryInfoList = this->_pMemoryInfoList;

	PMINIDUMP_MEMORY_INFO pMemInfoBase = (PMINIDUMP_MEMORY_INFO)((BYTE*)pMemoryInfoList + pMemoryInfoList->SizeOfHeader);

	for (size_t i = 0; i < pMemoryInfoList->NumberOfEntries; ++i)
	{
		PMINIDUMP_MEMORY_INFO pCurrMemInfo = pMemInfoBase + i;
		if (pCurrMemInfo->State == MEM_COMMIT)
		{
			auto prot = pCurrMemInfo->Protect;
			//Pages with no access and guarded pages should be skipped.
			if ((prot & (PAGE_NOACCESS | PAGE_GUARD)) == 0)
			{
				handler(pCurrMemInfo);
			}
		}
	}
}