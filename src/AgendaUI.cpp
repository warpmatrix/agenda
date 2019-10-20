#include "AgendaUI.hpp"


void AgendaUI::printAction() {
    int num = 30;
    for (int i=0; i<num; i++)
      std::cout << '-';
    std::cout << "Agenda";
    for (int i=0; i<num; i++)
      std::cout << '-';
    std::cout << '\n';

    std::cout << "Action :" << '\n';
    if (m_userName == "") printUnLogAct();
    else printLogInAct();

    for (int i=0; i<2*num+6; i++)
      std::cout << '-';
    std::cout << '\n';
}
void AgendaUI::printUnLogAct() {
    std::cout << "l\t- log in Agenda by user name and password" << '\n';
    std::cout << "r\t- register an Agenda account" << '\n';
    std::cout << "q\t- quit Agenda" << '\n';
}
void AgendaUI::printLogInAct() {
    std::cout << "o\t- log out Agenda" << '\n';
    std::cout << "dc\t- delete Agenda account" << '\n';
    std::cout << "lu\t- list all Agenda user" << '\n';
    std::cout << "cm\t- create a meeting" << '\n';
    std::cout << "amp\t- add meeting participator" << '\n';
    std::cout << "rmp\t- remove meeting participator" << '\n';
    std::cout << "rqm\t- request to quit meeting" << '\n';
    std::cout << "la\t- list all meetings" << '\n';
    std::cout << "las\t- list all sponsor meetings" << '\n';
    std::cout << "lap\t- list all participator meetings" << '\n';
    std::cout << "qm\t- query meeting by title" << '\n';
    std::cout << "qt\t- query meeting by time interval" << '\n';
    std::cout << "dm\t- delete meeting by title" << '\n';
    std::cout << "da\t- delete all meetings" << '\n';
}

AgendaUI::AgendaUI() {
    m_userName = "";
    m_userPassword = "";
}
void AgendaUI::OperationLoop(void) {
    while (1) {
        std::cout << '\n';
        std::string operation = getOperation();
        if (!executeOperation(operation) ) {
            std::cout << '\n';
            break;
        }
    }
}
void AgendaUI::startAgenda(void) {
    printAction();
    std::cout << '\n';
    while (1) {
        std::string operation = getOperation();
        if (operation == "l") userLogIn();
        else if (operation == "r") userRegister();
        else if (operation == "q") {
            quitAgenda();
            break;
        }
    }
}
std::string AgendaUI::getOperation() {
    if (m_userName == "")
        std::cout << "Agenda :~$ ";
    else std::cout << "Agenda@" << m_userName << " :~# ";
    std::string operation;
    std::cin >> operation;
    std::cout << '\n';
    return operation;
}
bool AgendaUI::executeOperation(std::string t_operation) {
    if (t_operation == "o") {
        userLogOut();
        return false;
    }
    else if (t_operation == "dc") {
        deleteUser();
        return false;
    }
    else if (t_operation == "lu") {
        listAllUsers();
        return true;
    }
    else if (t_operation == "cm") {
        createMeeting();
        return true;
    }
    else if (t_operation == "amp") {
        addMeetingParti();
        return true;
    }
    else if (t_operation == "rmp") {
        removeMeetingParti();
        return true;
    }
    else if (t_operation == "rqm") {
        quitMeeting();
        return true;
    }
    else if (t_operation == "la") {
        listAllMeetings();
        return true;
    }
    else if (t_operation == "las") {
        listAllSponsorMeetings();
        return true;
    }
    else if (t_operation == "lap") {
        listAllParticipateMeetings();
        return true;
    }
    else if (t_operation == "qm") {
        queryMeetingByTitle();
        return true;
    }
    else if (t_operation == "qt") {
        queryMeetingByTimeInterval();
        return true;
    }
    else if (t_operation == "dm") {
        deleteMeetingByTitle();
        return true;
    }
    else if (t_operation == "da") {
        deleteAllMeetings();
        return true;
    }
}
void AgendaUI::userLogIn(void) {
    std::cout << "[log in] [username] [password]" << '\n';
    std::cout << "[log in] ";
    std::string username, password;
    std::cin >> username >> password;
    if (m_agendaService.userLogIn(username, password) ) {
        std::cout << "[log in] succeed!" << '\n';
        std::cout << '\n';
        m_userName = username;
        m_userPassword = password;
        printAction();
        OperationLoop();
    }
    else {
        std::cout << "[log in] Password error or user doesn't exist" << '\n';
        std::cout << '\n';
    }
}
void AgendaUI::userRegister(void) {
    std::cout << "[register] [username] [password] [email] [phone]" << '\n';
    std::cout << "[register] ";
    std::string username, password, email, phone;
    std::cin >> username >> password >> email >> phone;
    if (m_agendaService.userRegister(username, password, email, phone) ) {
        std::cout << "[register] succeed! (You will log in Agenda automatically.)" << '\n';
        std::cout << '\n';
        m_userName = username;
        m_userPassword = password;
        printAction();
        OperationLoop();
    }
    else {
        std::cout << "[register] This username has been registered" << '\n';
        std::cout << '\n';
    }
}
void AgendaUI::userLogOut(void) {
    m_userName = "";
    m_userPassword = "";
    std::cout << '\n';
    printAction();
}
void AgendaUI::quitAgenda(void) {

}
void AgendaUI::deleteUser(void) {
    if (m_agendaService.deleteUser(m_userName, m_userPassword) ) {
        std::cout << "[delete agenda account] succeed!" << '\n';
        m_userName = "";
        m_userPassword = "";
        printAction();
    }
    else {
        std::cout << "[delete agenda account] failed" << '\n';
    }
}
void AgendaUI::listAllUsers(void) {
    std::cout << "[list all user]" << '\n';
    std::cout << '\n';
    std::cout << std::setw(20) << std::left << "name"
              << std::setw(20) << std::left << "email"
              << std::setw(20) << std::left << "phone" << '\n';
    std::list<User> user_list = m_agendaService.listAllUsers();
    for (auto it=user_list.begin(); it!=user_list.end(); it++) {
        std::cout << std::setw(20) << std::left << it->getName()
              << std::setw(20) << std::left << it->getEmail()
              << std::setw(20) << std::left << it->getPhone() << '\n';
    }
}
void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [the number of participators]" << '\n';
    std::cout << "[create meeting] ";
    int partiNum;
    std::cin >> partiNum;
    std::vector<std::string> parti_vector;
    for (int i=0; i<partiNum; i++) {
        std::cout << "[create meeting] [please enter the participator " << i+1 << " ]" << '\n';
        std::cout << "[create meeting] ";
        std::string participator;
        std::cin >> participator;
        parti_vector.push_back(participator);
    }
    std::cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << '\n';
    std::cout << "[create meeting] ";
    std::string title, startDate, endDate;
    std::cin >> title >> startDate >> endDate;
    if (m_agendaService.createMeeting(m_userName, title, startDate, endDate, parti_vector) ) {
        std::cout << "[create meeting] succeed!" << '\n';
    }
    else {
        std::cout << "[create meeting] error!" << '\n';
    }
}
void AgendaUI::addMeetingParti(void) {
    std::cout << "[add participator] [meeting title] [participator username]" << '\n';
    std::cout << "[add participator] ";
    std::string title, participator;
    std::cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator) ) {
        std::cout << "[add participator] succeed!" << '\n';
    }
    else std::cout << "[add participator] error!" << '\n';
}
void AgendaUI::removeMeetingParti(void) {
    std::cout << "[remove participator] [meeting title] [participator username]" << '\n';
    std::cout << "[remove participator] ";
    std::string title, participator;
    std::cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator) ) {
        std::cout << "[remove participator] succeed!" << '\n';
    }
    else std::cout << "[remove participator] error!" << '\n';
}
void AgendaUI::quitMeeting(void) {
    std::cout << "[quit meeting] [meeting title]" << '\n';
    std::cout << "[quit meeting] ";
    std::string title;
    std::cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title) ) {
        std::cout << "[quit meeting] succeed!" << '\n';
    }
    else std::cout << "[quit meeting] error!" << '\n';
}
void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << '\n';
    std::cout << '\n';
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}
void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]" << '\n';
    std::cout << '\n';
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}
void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participator meetings]" << '\n';
    std::cout << '\n';
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}
void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meeting] [title]" << '\n';
    std::cout << "[query meeting] ";
    std::string title;
    std::cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << '\n';
    std::cout << "[query meeting] ";
    std::string startDate, endDate;
    std::cin >> startDate >> endDate;
    printMeetings(m_agendaService.meetingQuery(m_userName, startDate, endDate));
}
void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]" << '\n';
    std::cout << "[delete meeting] ";
    std::string title;
    std::cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title) ) {
        std::cout << "[delete meeting] succeed!" << '\n';
    }
    else std::cout << "[error] delete meeting fail!" << '\n';
}
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        std::cout << "[delete all meeting] succeed!" << '\n';
    }
    else std::cout << "[delete all meeting] failed!" << '\n';
}
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    std::cout << std::setw(20) << std::left << "title"
              << std::setw(20) << std::left << "sponsor"
              << std::setw(20) << std::left << "start time"
              << std::setw(20) << std::left << "end time"
              << std::setw(20) << std::left << "participators" << '\n';
    for (auto it=t_meetings.begin(); it!=t_meetings.end(); it++) {
        std::cout << std::setw(20) << std::left << it->getTitle()
              << std::setw(20) << std::left << it->getSponsor()
              << std::setw(20) << std::left << Date::dateToString(it->getStartDate())
              << std::setw(20) << std::left << Date::dateToString(it->getEndDate());
        std::vector<std::string> participators = it->getParticipator();
        for (int i=0; i<participators.size(); i++) {
            if (i == 0) std::cout << participators[i];
            else std::cout << ',' << participators[i];
        }
        std::cout << '\n';
    }
}