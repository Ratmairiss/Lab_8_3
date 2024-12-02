#pragma once

#include <QtWidgets>
#include "Person.h"

class SpecialButton : public QWidget
{
public:

	QPushButton* mainButton;
	QPushButton* deleteButton;

	SpecialButton(Person* person, QWidget* pt = 0) : QWidget(pt)
	{
		QHBoxLayout* layout = new QHBoxLayout(this);
 		mainButton = new QPushButton(person->name + " " + person->surname + " " + person->patronymic + "\n" +
			                         person->phone + "\n" +
			                         person->email);
		deleteButton = new QPushButton();

		mainButton->setMinimumSize(120, 30);
		deleteButton->setFixedSize(30, 30);

		layout->addWidget(mainButton);
		layout->addWidget(deleteButton);
	}
};