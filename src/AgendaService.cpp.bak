#include "AgendaService.hpp"
#include "iostream"

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
    /*
    int participator_size = participator.size();
    for(int j = 0; j < participator_size; j++) {
        std::list<Meeting> participactorMeetings = listAllMeetings(participator[j]);
        std::list<Meeting>::const_iterator k;
        for(k = participactorMeetings.begin(); k != participactorMeetings.end(); k++) {
            if(!(start >= i->getEndDate() || end <= i->getStartDate()))
                return false;
        }
    }
    */

    /*
    std::list<Meeting> userMeetings = listAllSponsorMeetings(userName);
    std::list<Meeting>::iterator j;
    for(j = userMeetings.begin(); j != userMeetings.end(); j++) {
        if(!(start <= j->getEndDate() || end >= j->getStartDate()))
            return false;
    }
    */
}


bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
    //maybe
    //check  participator
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

    //maybe
    std::list<Meeting> userMeetings = listAllSponsorMeetings(userName);
    std::list<Meeting>::iterator i;
    for(i = userMeetings.begin(); i != userMeetings.end(); i++) {
        if(i->getTitle() == title) {

            std::list<Meeting> participactorMeetings = listAllMeetings(participator);
            std::list<Meeting>::const_iterator k;
            for(k = participactorMeetings.begin(); k != participactorMeetings.end(); k++) {
                //here
                if(!(i->getStartDate() >= k->getEndDate() || i->getEndDate() <= k->getStartDate()))
                    return false;
            }
            //i->addParticipator(participator);

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

/*
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator) {
    std::list<Meeting> userMeetings = listAllSponsorMeetings(userName);
    std::list<Meeting>::iterator i;
    for(i = userMeetings.begin(); i != userMeetings.end(); i++) {
        if(i->getTitle() == title && i->isParticipator(participator)) {
            i->removeParticipator(participator);
            /*
            std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
                if(t_meeting.getSponsor() == userName && t_meeting.getTitle() == title)
                    return true;
                return false;
            };
            
            std::function<bool(const Meeting &)> filter = [&title](const Meeting& t_meeting) {
                return t_meeting.getTitle() == title;
            };

            if(i->getParticipator().empty()) {
                return m_storage->deleteMeeting(filter);
            }
            
            std::function<void(Meeting &)> switcher = [&participator](Meeting & t_meeting) {
                t_meeting.removeParticipator(participator);
            };
            return m_storage->updateMeeting(filter, switcher);
        }
    }
    return false;
}
*/

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

/*
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    std::list<Meeting> meetings = listAllParticipateMeetings(userName);
    std::list<Meeting>::iterator i;

    for(i = meetings.begin(); i != meetings.end(); i++) {
        if(i->getTitle() == title) {
            i->removeParticipator(userName);

            //maybe
            std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
                if(t_meeting.getTitle() == title && t_meeting.isParticipator(userName))
                    return true;
                return false;
            };
            
            std::function<bool(const Meeting &)> filter = [&title](const Meeting& t_meeting) {
                return t_meeting.getTitle() == title;
            };
            
            if(i->getParticipator().empty()) {
                return m_storage->deleteMeeting(filter);
            }

            std::function<void(Meeting &)> switcher = [i](Meeting & t_meeting) {
                t_meeting = *i;
            };
            return m_storage->updateMeeting(filter, switcher);
        }
    }
    return false;
}
*/
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
        /*
        if(t_meeting.getTitle() != title)
            return false;

        if(t_meeting.getSponsor() == userName)
            return  true;
        
        return t_meeting.isParticipator(userName);
        */
    };
    return m_storage->queryMeeting(filter);
}

//maybe
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
        /*
        return (meeting.getSponsor() == userName || meeting.isParticipator(userName))
            && meeting.getEndDate() >= start && meeting.getStartDate() <= end;
        */
    };
    return m_storage->queryMeeting(filter);
}


std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {

        return t_meeting.getSponsor() == userName || t_meeting.isParticipator(userName);
        /*
        if()
            return  true;

        return ;
        */
    };
    return m_storage->queryMeeting(filter);
}

 
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName;
        /*
        if()
            return true;
        return false;
        */
    };
    return m_storage->queryMeeting(filter);
}


std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.isParticipator(userName);
        //return t_meeting.getSponsor() != userName && t_meeting.isParticipator(userName);
        /*
        if()
            return false;

        return ;
        */
    };
    return m_storage->queryMeeting(filter);
}

//
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    std::function<bool(const Meeting &)> filter = [&userName, &title](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName && t_meeting.getTitle() == title;
        /*
        if()
            return true;
        return false;
        */
    };
    return m_storage->deleteMeeting(filter);
}


bool AgendaService::deleteAllMeetings(const std::string &userName) {
    std::function<bool(const Meeting &)> filter = [&userName](const Meeting& t_meeting) {
        return t_meeting.getSponsor() == userName;
        /*
        if()
            return true;
        return false;
        */
    };
    return m_storage->deleteMeeting(filter);
}

  /**
   * start Agenda service and connect to storage
   */
void AgendaService::startAgenda(void) {
    
}

  /**
   * quit Agenda service
   */
void AgendaService::quitAgenda(void) {
    m_storage->~Storage();
}