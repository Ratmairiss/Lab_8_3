#pragma once

#include <QtWidgets>
#include "InputTab.h"
#include "Person.h"
#include "SpecialButton.h"

#include <iostream>

#include <memory>
using namespace std;

class PhoneDirectory : public QStackedWidget
{
	QWidget* mainPage;
	QWidget* secondPage;

	QScrollArea*         mainScrollArea;
	QLabel*              mainInformation;
	QTabWidget*          mainTabWidget;
	InputTab*            mainInputTab;
	QComboBox*           mainSortComboBox;
	QPushButton*         mainSortButton;

	QLabel*      personPhoto;
	QPushButton* changePhotoButton;
	QPushButton* applyButton;
	QPushButton* closeButton;
	QTextEdit*   personText;
	InputTab*    personData;

	bool isUpSorting = 1;

	QVector <shared_ptr<Person>> persons;
	shared_ptr<Person> currentPerson = 0;

public:

	PhoneDirectory(QStackedWidget* pt = 0);

	void formMainPage();
	void formSecondPage();

	void changeCurPersonPhoto();

	void openSecondView();

	void updateScrollArea();

	void fillContacts();

	void rewriteDataFile();

	~PhoneDirectory();

	int sortPersons();

public slots:

	void checkMainInput()
	{
		QString res = mainInputTab->checkInput();
		if (res.isEmpty() && !mainInputTab->oneFieldIsEmpty())
			mainInputTab->button->setEnabled(true);
		else
			mainInputTab->button->setEnabled(false);

		mainInformation->setText(res);
	}

	void changeCurPerson()
	{
		personData->deleteTrashSymbols();
		QString res = personData->checkInput();
		if (!res.isEmpty())
		{
			QMessageBox::warning(this, "Incorrect input.",res);
			return;
		}
		else if (personData->oneFieldIsEmpty())
		{
			 QMessageBox::warning(this, "Incorrect input.", "Some of the LineEdits are empty.");
			 return;
		}
		else
		{
			for (int i = 0; i < persons.size(); i++)
			{
				if (persons[i] == currentPerson)
					continue;
				if (persons[i]->phone == personData->phoneEdit->text())
				{
					QMessageBox::warning(this, "Failed to add contact.",
										"Contact named " + persons[i]->name + 
										" already have that phone number.");
					return;
				}
			}
			currentPerson->name        = personData->nameEdit->text();
			currentPerson->surname     = personData->surnameEdit->text();
			currentPerson->patronymic  = personData->patronymicEdit->text();
			currentPerson->phone       = personData->phoneEdit->text();
			currentPerson->email       = personData->emailEdit->text();
			currentPerson->birthDate   = personData->dateEdit->date();
			currentPerson->info        = personText->toPlainText();
			currentPerson->profilePict = personPhoto->property("fileName").toString();
		}

		updateScrollArea();
	}

	void addPerson()
	{
		mainInputTab->deleteTrashSymbols();

		Person* newPerson = new Person(mainInputTab->nameEdit->text(),
									   mainInputTab->surnameEdit->text(),
									   mainInputTab->patronymicEdit->text(),
			                           mainInputTab->phoneEdit->text(),
			                           mainInputTab->emailEdit->text(),
									   mainInputTab->dateEdit->date());

		for (int i = 0; i < persons.size(); i++)
		{
			if (persons[i]->phone == newPerson->phone)
			{
				QMessageBox::warning(this, "Failed to add contact.",
					                 "Contact named " + persons[i]->name + " already have that phone number.");
				return;
			}
		}
		persons.emplaceBack(newPerson);
		updateScrollArea();
	}
};