#pragma once
#include <DbgHelp.h>
#include <queue>
#include <functional>


struct MemoryListStreamSegmentDescriptor
{
	MemoryListStreamSegmentDescriptor(ULONG64 mem_stream_dest_offset, ULONG64 region_size, void* target_process_mem_offset)
		: MemStreamDestOffset(mem_stream_dest_offset),
		  RegionSize(region_size),
		  TargetProcessMemOffset(target_process_mem_offset)
	{
	}

	MemoryListStreamSegmentDescriptor() {}

	ULONG64 MemStreamDestOffset;
	ULONG64 RegionSize;

	void* TargetProcessMemOffset;
};

class MemoryListStreamPartitioner
{
public:
	typedef std::queue<MemoryListStreamSegmentDescriptor> DescriptorsQueue;
	typedef std::unique_ptr<DescriptorsQueue> DescriptorsQueuePtr;

	MemoryListStreamPartitioner(PMINIDUMP_MEMORY_INFO_LIST pMemoryInfoList) :
		_pMemoryInfoList(pMemoryInfoList)
	{
		
	}

	MemoryListStreamPartitioner(MemoryListStreamPartitioner&) = delete;
	MemoryListStreamPartitioner& operator=(MemoryListStreamPartitioner&) = delete;

	~MemoryListStreamPartitioner();

	DescriptorsQueuePtr getMemoryListStreamDescriptors();
	



private:
	PMINIDUMP_MEMORY_INFO_LIST _pMemoryInfoList;

	void EnumerateMemoryInfos(std::function<void(PMINIDUMP_MEMORY_INFO memInfo)> const& handler);
};

