#include <cassert>
#include <atomic>
#include <future>
#include <mutex>
#include <vector>

#include "lz4mt_xxh32.h"

namespace {
	typedef std::unique_lock<std::mutex> Lock;
} // anonymous namespace


namespace Lz4Mt {

	Xxh32::Xxh32(uint32_t seed)
		: mut()
		, st()
	{
		Lock lock(mut);
		XXH32_reset(&this->st, seed);
	}


	Xxh32::Xxh32(const void* input, int len, uint32_t seed)
		: mut()
		, st()
	{
		Lock lock(mut);
		XXH32_reset(&this->st, seed);
		XXH32_update(&this->st, input, len);
	}


	Xxh32::~Xxh32() {
		Lock lock(mut); // wait for release
	}


	bool Xxh32::update(const void* input, int len) {
		Lock lock(mut);
		return XXH_OK == XXH32_update(&this->st, input, len);
	}


	uint32_t Xxh32::digest() {
		Lock lock(mut);
		return XXH32_digest(&this->st);
	}

} // namespace Lz4Mt
