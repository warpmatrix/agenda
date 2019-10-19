#include "AgendaService.hpp"
#include <iostream>

AgendaService::AgendaService() {
    m_storage = Storage::getInstance();
}

AgendaService::~AgendaService() {
    m_storage = nullptr;
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    auto match = [&userName, &password](const User &user) {
        bool userNameEq = userName == user.getName();
        bool passwordEq = password == user.getPassword();
        return userNameEq && passwordEq;
    };
    if (m_storage->queryUser(match).size() == 1)
        return true;
    else return false;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                                 const std::string &email, const std::string &phone) {
    auto userNameEq = [&userName](const User &user) ->bool {
        if (userName == user.getName() ) return true;
    };
    if (m_storage->queryUser(userNameEq).empty() ) {
        m_storage->createUser(User(userName, password, email, phone));
        return true;
    }
    else return false;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto match = [&userName, &password](const User &user) {
        bool userNameEq = userName == user.getName();
        bool passwordEq = password == user.getPassword();
        return userNameEq && passwordEq;
    };
    deleteAllMeetings(userName);
    std::list<Meeting> meeting_list = listAllParticipateMeetings(userName);
    for (auto it=meeting_list.begin(); it!=meeting_list.end(); it++) {
        quitMeeting(userName, it->getTitle());
    }
    if (m_storage->deleteUser(match) > 0) {
        auto noParti = [](const Meeting &meeting) ->bool {
            return meeting.getParticipator().empty();
        };
        m_storage->deleteMeeting(noParti);
        return true;
    }
    else return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
    auto all = [](const User &user) {
        return true;
    };
    return m_storage->queryUser(all);
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

// bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
//                                   const std::string &startDate, const std::string &endDate,
//                                   const std::vector<std::string> &participator) {
//     if (!Date::isValid(startDate) || !Date::isValid(endDate) || startDate >= endDate)
//         return false;

//     auto userNameEq = [&userName](const User &user) ->bool{
//         if (userName == user.getName() ) return true;
//     };
//     if (m_storage->queryUser(userNameEq).empty() ) return false;
//     std::list<Meeting> meeting_list = listAllMeetings(userName);
//     for (auto it=meeting_list.begin(); it!=meeting_list.end(); it++) {
//         if (it->getStartDate() < endDate && it->getEndDate() > startDate)
//             return false;
//     }

//     auto titleEq = [&title](const Meeting &meeting) ->bool {
//         if (meeting.getTitle() == title) return true;
//     };
//     if (m_storage->queryMeeting(titleEq).empty() ) return false;
// //
//     if (participator.empty() ) return false;
//     for (int i=0; i<participator.size(); i++) {
//         std::string partiName = participator[i];
//         auto userNameEq = [partiName](const User &user) ->bool{
//             if (partiName == user.getName() ) return true;
//         };
//         if (m_storage->queryUser(userNameEq).size() != participator.size() )
//             return false;
//         std::list<Meeting> meeting_list = listAllMeetings(participator[i]);
//         for (auto it=meeting_list.begin(); it!=meeting_list.end(); it++) {
//             if (it->getStartDate() < endDate && it->getEndDate() > startDate)
//                 return false;
//         }
//     }
//     m_storage->createMeeting(Meeting(userName, participator, startDate, endDate, title));
//     return true;
// }

bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    auto userNameEq = [&participator](const User &user) ->bool{
        if (participator == user.getName() ) return true;
    };
    if (m_storage->queryUser(userNameEq).empty() ) return false;
    std::list<Meeting> meeting_list = listAllMeetings(participator);
    auto matchedM = [&userName, &title, &meeting_list](const Meeting &meeting) ->bool{
        if (title != meeting.getTitle() || userName != meeting.getSponsor() )
            return false;
        for (auto it=meeting_list.begin(); it!=meeting_list.end(); it++) {
            if (it->getStartDate() < meeting.getEndDate() &&
                it->getEndDate() > meeting.getStartDate() )
                return false;
        }
        return true;
    };
    auto addParti = [&participator](Meeting &meeting) {
        meeting.addParticipator(participator);
    };
    if (m_storage->updateMeeting(matchedM, addParti) > 0)
        return true;
    else return false;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    auto matchedM = [&](const Meeting &meeting) ->bool {
        return title == meeting.getTitle() 
            && userName == meeting.getSponsor()
            && meeting.isParticipator(participator);
    };
    auto rmParti = [&participator](Meeting &meeting) {
        meeting.removeParticipator(participator);
    };
    if (m_storage->updateMeeting(matchedM, rmParti) > 0) {
        auto noParti = [](const Meeting &meeting) ->bool {
            return meeting.getParticipator().empty();
        };
        m_storage->deleteMeeting(noParti);
        return true;
    }
    else return false;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto filter = [&](const Meeting &meeting) ->bool {
        return title == meeting.getTitle() 
            && meeting.isParticipator(userName);
    };
    auto rmParti = [&userName](Meeting &meeting) {
        meeting.removeParticipator(userName);
    };
    if (m_storage->updateMeeting(filter, rmParti) > 0) {
        auto noParti = [](const Meeting &meeting) ->bool {
            return meeting.getParticipator().empty();
        };
        m_storage->deleteMeeting(noParti);
        return true;
    }
    else return false;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const{
    auto filter = [&](const Meeting &meeting) ->bool {
        return title == meeting.getTitle() && (userName==meeting.getSponsor() || meeting.isParticipator(userName));
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const {
    if (!Date::isValid(startDate) || !Date::isValid(endDate) || startDate > endDate)
        return std::list<Meeting>();
    auto filter = [&](const Meeting &meeting) ->bool {
        if (!(userName==meeting.getSponsor() || meeting.isParticipator(userName)) )
            return false;
        if (meeting.getStartDate() <= endDate && meeting.getEndDate() >= startDate)
            return true;
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    auto hasUser = [&userName](const Meeting &meeting) ->bool {
        return meeting.getSponsor() == userName || meeting.isParticipator(userName);
    };
    return m_storage->queryMeeting(hasUser);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    auto userSpon = [&userName](const Meeting &meeting) ->bool {
        return meeting.getSponsor() == userName;
    };
    return m_storage->queryMeeting(userSpon);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
    auto userParti = [&userName](const Meeting &meeting) ->bool {
        return meeting.isParticipator(userName);
    };
    return m_storage->queryMeeting(userParti);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    auto filter = [&](const Meeting &meeting) ->bool {
        return meeting.getSponsor() == userName 
            && meeting.getTitle() == title;
    };
    return m_storage->deleteMeeting(filter);
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    auto filter = [&](const Meeting &meeting) ->bool {
        return meeting.getSponsor() == userName;
    };
    return m_storage->deleteMeeting(filter);
}

void AgendaService::startAgenda(void) {}

void AgendaService::quitAgenda(void) {}
