/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_new_delete_override.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_new_delete_override.h"

#pragma push_macro("new")
#undef new
void* operator new(std::size_t size, const char* file, const char* func, int lineno) noexcept {
	std::cout << "[Allocate Size]:" << size << " byte " << std::endl;
	std::cout << "[file]:" << file << " " << std::endl;
	std::cout << "[func]:" << func << " " << std::endl;
	std::cout << "[lineno]:" << lineno << std::endl;

	while (true) {
		void* block = ::malloc(size);
		if (block != nullptr) {
			return block;
		}
#if defined(_MSC_VER)
		std::new_handler globalHandler = std::set_new_handler(0);
		std::set_new_handler(globalHandler);

		if (globalHandler) (*globalHandler)();
#endif // !_MSC_VER
	}
}
#pragma pop_macro("new")
