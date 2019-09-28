#include "Storage.hpp"

// std::shared_ptr<Storage> Storage::m_instance;

// Storage::Storage() {
// 	m_dirty = false;
// 	readFromFile();
// }
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (!m_instance)
		m_instance.reset(new Storage() );
	return m_instance;
}
