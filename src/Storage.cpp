#include "Storage.hpp"

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
	m_dirty = false;
	readFromFile();	
}
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (!m_instance)
		m_instance.reset(new Storage() );
	return m_instance;
}

bool Storage::readFromFile(void) {
	ifstream fs;

}
bool Storage::writeToFile(void) {

}
int Storage::deleteUser(std::function<bool(const User &)> filter) {

}