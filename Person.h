#pragma once

#include <QtWidgets>
#include <memory>
using namespace std;

struct Person {

	QString name;
	QString surname;
	QString patronymic;
	QString phone;
	QString email;
	QDate   birthDate;
	QString profilePict;
	QString info;

	Person(QString name0, QString surname0, QString patronymic0,
		   QString phone0, QString email0, QDate birthDate0, 
		   QString profilePict0 = "defaultImage.jpg", 
		   QString info0 = "No Info") : name(name0),
		   surname(surname0), patronymic(patronymic0), phone(phone0),
		   email(email0), birthDate(birthDate0),
		   profilePict(profilePict0), info(info0) {}


	bool contains(const Person& person0)
	{
		bool result = 0;

		result = name.contains(person0.name) && 
				 surname.contains(person0.surname) && 
				 patronymic.contains(person0.patronymic) && 
				 email.contains(person0.email) &&
			     phone.contains(person0.phone) && 
				 ((person0.birthDate == QDate(2000, 1, 1) || (birthDate == person0.birthDate)));

		return result;
	}

};

extern QMap <QString, bool (*)(shared_ptr<Person>, shared_ptr<Person>)> comparicments;
