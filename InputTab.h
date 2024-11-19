#pragma once

#include <QtWidgets>


class InputTab : public QWidget
{
	QGridLayout* layout;

public:

	QLineEdit* nameEdit;
	QLineEdit* surnameEdit;
	QLineEdit* patronymicEdit;
	QLineEdit* phoneEdit;
	QLineEdit* emailEdit;
	QDateEdit* dateEdit;

	QPushButton* button;

	InputTab(bool addButton = 1,QWidget * pt = 0) : QWidget(pt)
	{
		this->setObjectName("CustomTab");

		layout = new QGridLayout(this);
		layout->setObjectName("NeedsToBeGray");

		if (addButton)
		{
			button = new QPushButton("Add");
			layout->addWidget(button, 3, 1);
		}

		nameEdit = new QLineEdit;
		surnameEdit = new QLineEdit;
		patronymicEdit = new QLineEdit;
		phoneEdit = new QLineEdit;
		emailEdit = new QLineEdit;
		dateEdit = new QDateEdit;

		nameEdit->setMinimumHeight(30);
		surnameEdit->setMinimumHeight(30);
		patronymicEdit->setMinimumHeight(30);
		phoneEdit->setMinimumHeight(30);
		emailEdit->setMinimumHeight(30);
		dateEdit->setMinimumHeight(30);

		dateEdit->setMaximumDate(QDate::currentDate());

		dateEdit->setCalendarPopup(true);

		QWidget* edits[]{ nameEdit, surnameEdit, patronymicEdit,
							phoneEdit, emailEdit, dateEdit };
		QString buttons[]{ "Name:", "Surname:", "Patronymic:",
						   "Phone:","Email:",   "Date:"};

		QVBoxLayout* tmp;
		for (int i = 0; i < 6; i++)
		{
			tmp = new QVBoxLayout;
			tmp->addWidget(new QLabel(buttons[i]));
			tmp->addWidget(edits[i]);
			if (i < 3)
				layout->addLayout(tmp, i, 0);
			else
				layout->addLayout(tmp, i - 3, 1);
		}
	}

	void deleteSymbols(QString& str, QVector<QChar>& symbols)
	{
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < symbols.size(); j++)
				if (str[i] == symbols[j])
				{
					str.removeAt(i);
					i--;
					break;
				}
		}
	}

	void deleteTrashSymbols()
	{
		QString name = nameEdit->text();
		QString surname = surnameEdit->text();
		QString patronymic = patronymicEdit->text();
		QString phone = phoneEdit->text();
		QString email = emailEdit->text();

		QVector<QChar> symbolsToDelete{ ' ' };
		deleteSymbols(name, symbolsToDelete);
		deleteSymbols(surname, symbolsToDelete);
		deleteSymbols(patronymic, symbolsToDelete);
		deleteSymbols(email, symbolsToDelete);

		symbolsToDelete.push_back('(');
		symbolsToDelete.push_back(')');
		symbolsToDelete.push_back('-');
		deleteSymbols(phone, symbolsToDelete);

		nameEdit->setText(name);
		surnameEdit->setText(surname);
		patronymicEdit->setText(patronymic);
		phoneEdit->setText(phone);
		emailEdit->setText(email);
	}

	bool allFieldsAreEmpty()
	{
		return (nameEdit->text().isEmpty()	     && surnameEdit->text().isEmpty() &&
				patronymicEdit->text().isEmpty() && phoneEdit->text().isEmpty() &&
				emailEdit->text().isEmpty());
	}

	bool oneFieldIsEmpty()
	{
		return (nameEdit->text().isEmpty()		 || surnameEdit->text().isEmpty() ||
				patronymicEdit->text().isEmpty() || phoneEdit->text().isEmpty() ||
				emailEdit->text().isEmpty());
	}

	public slots:

		QString checkInput()
		{
			QString res = "";

			QLineEdit* widgetToCheck[]{ nameEdit, surnameEdit, patronymicEdit };
			QString text[]{ "Имя", "Фамилия", "Отчество" };
			QRegularExpression reg("[ ]*[A-ZА-Я][A-ZА-Яa-za-z- 0-9]*[^-][ ]*");

			for (int i = 0; i < sizeof(text) / sizeof(text[0]); i++)
			{
				if (!widgetToCheck[i]->text().isEmpty())
				{
					if (!reg.match(widgetToCheck[i]->text()).hasMatch())
					{
						res += text[i] + " должно(а) начинаться на заглавуню\nбукву и не заканчиваться на дефис,\nвключать в себя только\nбуквы, цифры, знак пробела и дефиса.\n";
						widgetToCheck[i]->setStyleSheet("QLineEdit { color: red; border: 2px solid red; }");
					}
					else
						widgetToCheck[i]->setStyleSheet("");
				}
				else
					widgetToCheck[i]->setStyleSheet("");
			}

			if (!phoneEdit->text().isEmpty())
			{
				QRegularExpression reg2("\\+?\\s*\\d{1,2}\\s*[-(]?\\s*\\d{3}\\s*[)-]?\\s*\\d{3}\\s*-?\\d{2}\\s*-?\\d{2}\\s*");
				if (!reg2.match(phoneEdit->text()).hasMatch() || (phoneEdit->text().count(QRegularExpression("\\d")) != 11))
				{
					res += "Телефон должен быть записан в\nмеждународном формате \n";
					phoneEdit->setStyleSheet("QLineEdit { color: red; border: 2px solid red; }");
				}
				else
					phoneEdit->setStyleSheet("");
			}
			else
				phoneEdit->setStyleSheet("");

			if (!emailEdit->text().isEmpty())
			{
				QRegularExpression reg3("[\\dA-Za-zА-Яа-я ]+@[\dA-Za-zА-Яа-я ]+\\.[A-Za-zА-Яа-я ]+");
				if (!reg3.match(emailEdit->text()).hasMatch())
				{
					res += "Почта внесена некорректно. \n";
					emailEdit->setStyleSheet("QLineEdit { color: red; border: 2px solid red; }");
				}
				else
					emailEdit->setStyleSheet("");
			}
			else
				emailEdit->setStyleSheet("");

			return res;
		}

};
