#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <typeinfo>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
	m_dirty = !readFromFile();	
}
Storage::~Storage() {
	sync();
	m_instance = nullptr;
}
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (!m_instance)
		m_instance.reset(new Storage() );
	return m_instance;
}

bool Storage::readFromFile(void) {
	std::ifstream filein;
	filein.open(Path::meetingPath, std::ios::in);
	if (filein.is_open() ) {
		std::string line;
		while (std::getline(filein, line) ) {
			int index1=0, index2;
			Meeting n_meeting;
			for (int i=0; i<5; i++) {
				for (index2=index1+1; line[index2]!='"'; index2++);
				int index = index1 + 1;
				switch (i) {
					case 0: {
						std::string sponsor = line.substr(index, index2-index);
						n_meeting.setSponsor(sponsor);
						break;
					}
					case 1: {
						for (int j=index+1; j<=index2; j++) {
							if (line[j] == '&' || line[j] == '"') {
								std::string participator = line.substr(index, j-index);
								n_meeting.addParticipator(participator);
								index = j+1;
							}
						}
						break;
					}
					case 2: {
						std::string startDate = line.substr(index, index2-index);
						n_meeting.setStartDate(startDate);
						break;
					}
					case 3: {
						std::string endDate = line.substr(index, index2-index);
						n_meeting.setEndDate(endDate);
						break;
					}
					case 4: {
						std::string title = line.substr(index, index2-index);
						n_meeting.setTitle(title);
						break;
					}
				}
				index1 = index2 + 2;
			}
			createMeeting(n_meeting);
		}
		filein.close();
	}
	else return false;
	filein.open(Path::userPath, std::ios::in);
	if (filein.is_open() ) {
		std::string line;
		while (std::getline(filein, line) ) {
			int index1=0, index2;
			User n_user;
			for (int i=0; i<4; i++) {
				for (index2=index1+1; line[index2]!='"'; index2++);
				int index = index1+1;
				switch (i) {
					case 0: {
						std::string username = line.substr(index, index2-index);
						n_user.setName(username);
						break;
					}
					case 1: {
						std::string password = line.substr(index, index2-index);
						n_user.setPassword(password);
						break;
					}
					case 2: {
						std::string email = line.substr(index, index2-index);
						n_user.setEmail(email);
						break;
					}
					case 3: {
						std::string phone = line.substr(index, index2-index);
						n_user.setPhone(phone);
						break;
					}
				}
				index1 = index2 + 2;
			}
			createUser(n_user);
		}
	}
	else return false;
	return true;
}

bool Storage::writeToFile(void) {
	std::ofstream fileout;
	fileout.open(Path::meetingPath, std::ios::out);
	if (fileout.is_open() ) {
		// std::list<Meeting>::const_iterator it;
		for (auto it=m_meetingList.begin(); it!=m_meetingList.end(); it++) {
			fileout << '"' << it->getSponsor() << "\",\"";
			// std::vector<std::string>::const_iterator pIt;

			std::vector<std::string> partis = it->getParticipator();
			for (int i = 0;i < it->getParticipator().size(); i ++){
				if (i == 0) fileout << partis[i];
				else fileout << '&' << partis[i];
			}
			// for (auto pIt=it->getParticipator().begin(); pIt!=it->getParticipator().end(); pIt++) {
			// 	if (pIt == it->getParticipator().begin() ) {
			// 		printf("w\n");
			// 		// std::cout << *pIt << '\n';
			// 		// fileout << *pIt;
			// 	}
			// 	else fileout << '&' << *pIt;
			// }
			fileout << "\",\"" << Date::dateToString(it->getStartDate());
			fileout << "\",\"" << Date::dateToString(it->getEndDate());
			fileout << "\",\"" << it->getTitle() << "\"\n";
		}
		fileout.close();
	}
	else return false;
	fileout.open(Path::userPath, std::ios::out);
	if (fileout.is_open() ) {
		std::list<User>::const_iterator it;
		for (it=m_userList.begin(); it!=m_userList.end(); it++) {
			fileout << '"' << it->getName();
			fileout << "\",\"" << it->getPassword();
			fileout << "\",\"" << it->getEmail();
			fileout << "\",\"" << it->getPhone() << "\"\n";
		}
		fileout.close();
	}
	else return false;
	return true;
}

void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	// m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> user_list;
	std::list<User>::const_iterator it;
	for (it=m_userList.begin(); it!=m_userList.end(); it++) {
		if (filter(*it) ) user_list.push_back(*it);
	}
	return user_list;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 		std::function<void(User &)> switcher) {
	int count = 0;
	std::list<User>::iterator it;
	for (it=m_userList.begin(); it!=m_userList.end(); it++) {
		if (filter(*it) ) {
			switcher(*it);
			count++;
		}
	}
	if (count) m_dirty = true;
	return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int count = 0;
	std::list<User>::iterator it;
	for (it=m_userList.begin(); it!=m_userList.end(); it++) {
		if (filter(*it) ) {
			m_userList.erase(it);
			count++;
		}
	}
	if (count) m_dirty = true;
	return count;
}

void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.push_back(t_meeting);
	// m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting> meeting_list;
	std::list<Meeting>::const_iterator it;
	for (it=m_meetingList.begin(); it!=m_meetingList.end(); it++) {
		if (filter(*it) ) meeting_list.push_back(*it);
	}
	return meeting_list;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    	   std::function<void(Meeting &)> switcher) {
	int count = 0;
	std::list<Meeting>::iterator it;
	for (it=m_meetingList.begin(); it!=m_meetingList.end(); it++) {
		if (filter(*it) ) {
			switcher(*it);
			count++;
		}
	}
	if (count) m_dirty = true;
	return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int count=0;
	std::list<Meeting>::iterator it;
	for (it=m_meetingList.begin(); it!=m_meetingList.end(); it++) {
		if (filter(*it) ) {
			m_meetingList.erase(it);
			count++;
		}
	}
	if (count) m_dirty = true;
	return count;
}

bool Storage::sync(void) {
	if (m_dirty) {
		bool flag = writeToFile();
		if (flag) m_dirty = false;
		return flag;
	}
	return false;
}