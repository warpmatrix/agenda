#include "Date.hpp"

int strToInt(const std::string numStr);
int getMaxDay(int year, int month);
bool isLeap(int year);
bool isNum(const std::string &str);
bool isFormat(const std::string &t_dateString);

bool isFormat(const std::string &t_dateString) {
	if (t_dateString.length() != 16)
		return false;
 	if (!isNum(t_dateString.substr(0, 4)) )
 		return false;
 	if (t_dateString[4] != '-')
 		return false;
 	if (!isNum(t_dateString.substr(5, 2)) )
 		return false;
 	if (t_dateString[7] != '-')
 		return false;
 	if (!isNum(t_dateString.substr(8, 2)) )
 		return false;
 	if (t_dateString[10] != '/')
 		return false;
 	if (!isNum(t_dateString.substr(11, 2)) )
 		return false;
 	if (t_dateString[13] != ':')
 		return false;
 	if (!isNum(t_dateString.substr(14, 2)) )
 		return false;
 	return true;
}
int strToInt(const std::string numStr) {
  int num = 0;
  for (int i=0; i<numStr.length(); i++)
    num = num*10 + numStr[i]-'0';
  return num;
}
int getMaxDay(int year, int month) {
	int maxDay;
	switch(month) {
	case 2:
		if (isLeap(year) ) maxDay = 29;
		else maxDay = 28;
		break;
	case 4: case 6: case 9: case 11:
		maxDay = 30;
		break;
	default:
		maxDay = 31;
	}
	return maxDay;
}
bool isLeap(int year) {
	if (year%400 == 0) return true;
	if (year%100 != 0 && year%4 == 0)
		return true;
	return false;
}
bool isNum(const std::string &str) {
	for (int i=0; i<str.length(); i++)
		if (!(str[i]>='0' && str[i]<='9') )
			return false;
	return true;
}

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
 	m_year = t_year;
 	m_month = t_month;
 	m_day = t_day;
 	m_hour = t_hour;
 	m_minute = t_minute;
}
Date::Date(const std::string &dateString) {
	if (!isFormat(dateString) )
		m_year = m_month = m_day = m_hour = m_minute = 0;
	else {
	 	m_year = strToInt(dateString.substr(0, 4) );
	 	m_month = strToInt(dateString.substr(5, 2) );
	 	m_day = strToInt(dateString.substr(8, 2) );
	 	m_hour = strToInt(dateString.substr(11, 2) );
	 	m_minute = strToInt(dateString.substr(14, 2) );
	}
}
int Date::getYear(void) const {
 	return m_year;
}
void Date::setYear(const int t_year) {
 	m_year = t_year;
}
int Date::getMonth(void) const {
 	return m_month;
}
void Date::setMonth(const int t_month) {
 	m_month = t_month;
}
int Date::getDay(void) const {
 	return m_day;
}
void Date::setDay(const int t_day) {
 	m_day = t_day;
}
int Date::getHour(void) const {
 	return m_hour;
}
void Date::setHour(const int t_hour) {
 	m_hour = t_hour;
}
int Date::getMinute(void) const {
 	return m_minute;
}
void Date::setMinute(const int t_minute) {
 	m_minute = t_minute;
}
bool Date::isValid(const Date &t_date) {
 	bool yearIsValid = (t_date.m_year >= 1000) && (t_date.m_year <= 9999);
 	bool monthIsValid = (t_date.m_month >= 1) && (t_date.m_month <= 12);
 	int maxDay = getMaxDay(t_date.m_year, t_date.m_month);
 	bool dayIsValid = (t_date.m_day >= 1) && (t_date.m_day <= maxDay);
 	bool hourIsValid = (t_date.m_hour >= 1) && (t_date.m_hour <= 23);
 	bool minuteIsValid = (t_date.m_minute >= 1) && (t_date.m_minute <= 59);
 	bool isValid = yearIsValid && monthIsValid && dayIsValid && hourIsValid && minuteIsValid;
 	return isValid;
}

Date Date::stringToDate(const std::string &t_dateString) {
 	if (!isFormat(t_dateString) )
 		return Date();
 	return Date(t_dateString);
}
std::string Date::dateToString(const Date &t_date) {
	if (!isValid(t_date) )
		return "0000-00-00/00:00";
	std::string str = std::to_string(t_date.m_year) + "-";
	if (t_date.m_month < 10) str += "0";
	str += std::to_string(t_date.m_month) + "-";
	if (t_date.m_day < 10) str += "0";
	str += std::to_string(t_date.m_day) + "/";
	if (t_date.m_hour < 10) str += "0";
	str += std::to_string(t_date.m_hour) + ":";
	if (t_date.m_minute < 10) str += "0";
	str += std::to_string(t_date.m_minute);
	return str;
}
Date &Date::operator=(const Date &t_date) {
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}
bool Date::operator==(const Date &t_date) const {
	bool yearEq = m_year == t_date.m_year;
	bool monthEq = m_month == t_date.m_month;
	bool dayEq = m_day == t_date.m_day;
	bool hourEq = m_hour == t_date.m_hour;
	bool minEq = m_minute == t_date.m_minute;
	return yearEq && monthEq && dayEq && hourEq && minEq;
}
bool Date::operator>(const Date &t_date) const {
	if (m_year > t_date.m_year) return true;
	if (m_year < t_date.m_year) return false;
	if (m_month > t_date.m_month) return true;
	if (m_month < t_date.m_month) return false;
	if (m_day > t_date.m_day) return true;
	if (m_day < t_date.m_day) return false;
	if (m_hour > t_date.m_hour) return true;
	if (m_hour < t_date.m_hour) return false;
	if (m_minute > t_date.m_minute) return true;
	return false;
}
bool Date::operator<(const Date &t_date) const {
	return !(*this > t_date) && !(*this == t_date);
}
bool Date::operator>=(const	Date &t_date) const {
	return (*this>t_date) || (*this==t_date);
}
bool Date::operator<=(const Date &t_date) const {
	return !(*this>t_date);
}