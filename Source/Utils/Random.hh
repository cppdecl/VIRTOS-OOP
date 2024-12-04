#ifndef UTILS_RANDOM_HH
#define UTILS_RANDOM_HH

#include <chrono>
#include <random>
#include <thread>

namespace Random
{
	static bool g_IsDeviceInitialized = false;
	static std::random_device g_RandomDevice;
	static std::mt19937_64 g_Generator;

	inline void EnsureInit()
	{
		if (!g_IsDeviceInitialized)
		{
            uint64_t entropy = 0;

            // 1. Time-based entropy (system time and high-res clock)
            entropy ^= std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            entropy ^= std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();

            // 2. Hardware entropy (std::random_device)
            entropy ^= g_RandomDevice();

            // 3. Process/thread-specific entropy
            entropy ^= std::hash<std::thread::id>{}(std::this_thread::get_id()); // Thread ID
            entropy ^= std::hash<std::thread::id>{}(std::this_thread::get_id()) + 0x5a1b2c3d; 

            // 4. System-specific entropy (e.g., process ID)
            entropy ^= std::hash<std::uintptr_t>{}(reinterpret_cast<std::uintptr_t>(&entropy)); // Memory address, acts as a unique identifier
            
            // 5. Random noise generation for further entropy
            std::uniform_int_distribution<uint64_t> noise_dist(0, 255);
            for (uint64_t i = 0; i < 10; ++i)
            {
                entropy ^= noise_dist(g_RandomDevice) << (i * 8);
            }

            entropy ^= (entropy >> 33);
            entropy *= 0xff51afd7ed558ccd;
            entropy ^= (entropy >> 33);
            entropy *= 0xc4ceb9fe1a85ec53;
            entropy ^= (entropy >> 33);

            g_Generator.seed(static_cast<std::mt19937_64::result_type>(entropy));
			g_IsDeviceInitialized = true;
		}
	}

	inline int32_t Gen(int32_t min, int32_t max)
	{
		EnsureInit();

		std::uniform_int_distribution<int32_t> distrib(min, max);
		return distrib(g_Generator);
	}

	inline bool Gen(double chanceOfTrue) 
	{
		int random = Gen(0, 100);
		return random < chanceOfTrue;
	}

	inline std::string Gen(const std::vector<std::string>& vec) 
	{
		EnsureInit();
		
		if (vec.empty()) 
		{
			return "";
		}

		std::uniform_int_distribution<> distr(0, vec.size() - 1);

		return vec[distr(g_Generator)];
	}

	inline int Gen(const std::vector<int>& vec) 
	{
		EnsureInit();

		if (vec.empty()) 
		{
			return 0;
		}

		std::uniform_int_distribution<> distr(0, vec.size() - 1);

		return vec[distr(g_Generator)];
	}

} 

#endif	// UTILS_RANDOM_HH