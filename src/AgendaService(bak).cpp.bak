#include "AgendaService.hpp"

AgendaService::AgendaService() {
    m_storage = Storage::getInstance();
}

AgendaService::~AgendaService() {
    m_storage = nullptr;
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    std::function<bool(const User &)> filter = [&userName, &password](const User& t_user) {
        return t_user.getName() == userName && t_user.getPassword() == password;
    };

    return !m_storage->queryUser(filter).empty();
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone) {
    
    std::function<bool(const User &)> filter = [&userName](const User& t_user) {
        return t_user.getName() == userName;
    };

    bool exist = !(m_storage->queryUser(filter).empty());
    if(exist)
        return false;

    User user(userName, password, email, phone);

    m_storage->createUser(user);
    return true;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    
    deleteAllMeetings(userName);

    std::list<Meeting> participating = listAllParticipateMeetings(userName);
    std::list<Meeting>::const_iterator i;
    for(i = participating.begin(); i != participating.end(); i++) {
        quitMeeting(userName, i->getTitle());
    }

    std::function<bool(const User &)> user_filter = [&userName, &password](const User & t_user) {
       return t_user.getName() == userName && t_user.getPassword() == password;
    };

    return m_storage->deleteUser(user_filter);

}

std::list<User> AgendaService::listAllUsers(void) const {
    std::function<bool(const User &)> filter = [](const User & t_user) {
        return true;
    };
    return m_storage->queryUser(filter);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator) {
    Date start = Date::stringToDate(startDate);
    Date end = Date::stringToDate(endDate);
    if(!Date::isValid(start) || !Date::isValid(end) || start >= end)
        return false;
    
    if(participator.empty())
        return false;

    bool have = false;
    std::list<User> user = listAllUsers();
    std::list<User>::const_iterator k;
    for(k = user.begin(); k != user.end(); k++) {
        if(userName == k->getName()) {
            have = true;
            break;
        }
    }
	if(!have)
		return false;


    std::function<bool(const Meeting &)> filter = [&title](const Meeting& t_meeting) {
        return t_meeting.getTitle() == title;
    };
    bool unique = m_storage->queryMeeting(filter).empty();
    if(!unique)
        return false;

    std::list<Meeting> allMeetings = listAllMeetings(userName);
    std::list<Meeting>::const_iterator i;
    for(i = allMeetings.begin(); i != allMeetings.end(); i++) {
        if(!(start >= i->getEndDate() || end <= i->getStartDate()))
            return false;
    }

    std::vector<std::string> tmp_participator;
    Meeting meeting(userName, tmp_participator, start, end, title);
    m_storage->createMeeting(meeting);

    std::vector<std::string>::const_iterator j;
    for(j = participator.begin(); j != participator.end(); j++){
		bool success;
		success = addMeetingParticipator(userName, title, *j);
    	if(!success) {
			m_storage->deleteMeeting(filter);
			return false;
		}
	}
	return true;
}


bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
    std::list<User> user = listAllUsers();
    int user_size = user.size();
    bool in = false;
    std::list<User>::const_iterator k;
    for(k = user.begin(); k != user.end(); k++) {
        if(participator == k->getName()) {
            in = true;
            break;
        }
    }
    if(!in)
        return false;

    std::list<Meeting> userMeetings = listAllSponsorMeetings(userName);
    std::list<Meeting>::iterator i;
    for(i = userMeetings.begin(); i != userMeetings.end(); i++) {
        if(i->getTitle() == title) {

            std::list<Meeting> participactorMeetings = listAllMeetings(participator);
            std::list<Meeting>::const_iterator k;
            for(k = participactorMeetings.begin(); k != participactorMeetings.end(); k++) {
                if(!(i->getStartDate() >= k->getEndDate() || i->getEndDate() <= k->getStartDate()))
                    return false;
            }

            std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
                return t_meeting.getSponsor() == userName && t_meeting.getTitle() == title;
            };
            std::function<void(Meeting &)> switcher = [&participator](Meeting & t_meeting) {
                t_meeting.addParticipator(participator);
            };
            return m_storage->updateMeeting(filter, switcher);
        }
    }
    return false;
}


bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator) {
    
    std::function<bool(const Meeting &)> filter = [&userName, &title, &participator](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName && t_meeting.getTitle() == title && t_meeting.isParticipator(participator);
    };
    std::function<void(Meeting &)> switcher = [&participator](Meeting & t_meeting) {
        t_meeting.removeParticipator(participator);
    };
    bool success = m_storage->updateMeeting(filter, switcher);
    if(success) {
        std::function<bool(const Meeting &)> empty_filter = [&userName](const Meeting& t_meeting) {
            return t_meeting.getParticipator().empty();
        };
        m_storage->deleteMeeting(empty_filter);
        return true;
    }
    return false;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
        return t_meeting.getTitle() == title && t_meeting.isParticipator(userName);
    };
    std::function<void(Meeting &)> switcher = [&userName](Meeting & t_meeting) {
        t_meeting.removeParticipator(userName);
    };
    bool success = m_storage->updateMeeting(filter, switcher);

    if(success) {
        std::function<bool(const Meeting &)> empty_filter = [&userName](const Meeting& t_meeting) {
            return t_meeting.getParticipator().empty();
        };
        m_storage->deleteMeeting(empty_filter);
        return true;
    }
    return false;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const {
    std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {

        return t_meeting.getTitle() == title && (t_meeting.getSponsor() == userName || t_meeting.isParticipator(userName));
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const {
    Date start = Date::stringToDate(startDate);
    Date end = Date::stringToDate(endDate);
    if(!Date::isValid(start) || !Date::isValid(end) || start > end)
        return std::list<Meeting>();

    std::function<bool(const Meeting &)> filter = [&userName, &start, &end](const Meeting& t_meeting) {
        
        if(t_meeting.getSponsor() != userName && !t_meeting.isParticipator(userName))
            return false;

        if(start > t_meeting.getEndDate() || end < t_meeting.getStartDate())
            return false;

        return true;
    };
    return m_storage->queryMeeting(filter);
}


std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {

        return t_meeting.getSponsor() == userName || t_meeting.isParticipator(userName);
    };
    return m_storage->queryMeeting(filter);
}

 
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName;
    };
    return m_storage->queryMeeting(filter);
}


std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.isParticipator(userName);
    };
    return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName && t_meeting.getTitle() == title;
    };
    return m_storage->deleteMeeting(filter);
}


bool AgendaService::deleteAllMeetings(const std::string &userName) {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName;
    };
    return m_storage->deleteMeeting(filter);
}

void AgendaService::startAgenda(void) {
    
}

void AgendaService::quitAgenda(void) {
    m_storage->~Storage();
}