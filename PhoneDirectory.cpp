#include "PhoneDirectory.h"

PhoneDirectory::PhoneDirectory(QStackedWidget* pt) : QStackedWidget(pt)
{
	this->setMinimumSize(350, 350);

	fillContacts();
	formMainPage();
	formSecondPage();
	this->setCurrentWidget(mainPage);
}

void PhoneDirectory::formMainPage()
{
	mainPage = new QWidget;

	mainScrollArea = new QScrollArea;
	mainInformation = new QLabel;
	mainTabWidget = new QTabWidget;
	mainInputTab = new InputTab;

	mainSortComboBox = new QComboBox;
	mainSortComboBox->setFixedHeight(50);
	mainSortComboBox->addItem("No Sort");
	mainSortComboBox->addItem("Sort By Name");
	mainSortComboBox->addItem("Sort By Surname");
	mainSortComboBox->addItem("Sort By Patronymic");
	mainSortComboBox->addItem("Sort By Phone");
	mainSortComboBox->addItem("Sort By Email");
	mainSortComboBox->addItem("Sort By Birth Date");

	mainSortButton = new QPushButton;
	mainSortButton->setFixedSize(50, 50);
	mainSortButton->setIcon(QPixmap("Pointer1.png").scaled(mainSortButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QWidget* sortTab = new QWidget;
	sortTab->setObjectName("CustomTab");
	QGridLayout* sortTabLayout = new QGridLayout;
	sortTabLayout->addWidget(mainSortComboBox, 0, 0);
	sortTabLayout->addWidget(mainSortButton, 0, 1);
	sortTabLayout->addWidget(new QWidget, 1, 1);
	sortTabLayout->setRowStretch(1, 1);
	sortTab->setLayout(sortTabLayout);

	mainTabWidget->addTab(mainInputTab, "Find or Add");
	mainTabWidget->addTab(sortTab, "Sort");

	mainInformation->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	mainInformation->setStyleSheet("QLabel { background-color: rgb(250,250,250); color: red }");

	mainInformation->setMinimumSize(250, 120);
	mainTabWidget->setMaximumSize(500, 250);
	mainTabWidget->setMinimumSize(150, 250);
	mainScrollArea->setMaximumWidth(500);
	mainScrollArea->setMinimumSize(150, 120);

	mainInputTab->button->setEnabled(false);

	updateScrollArea();

	QGridLayout* mainLayout = new QGridLayout;

	QWidget* tmp = new QWidget;
	tmp->setMaximumWidth(150);
	mainLayout->addWidget(mainScrollArea, 1, 1);
	mainLayout->addWidget(new QWidget, 0, 0, 2, 1);
	mainLayout->addWidget(tmp, 0, 3, 2, 1);
	mainLayout->addWidget(mainTabWidget, 0, 1);
	mainLayout->addWidget(mainInformation, 0, 2, 2, 1);

	mainPage->setLayout(mainLayout);
	this->addWidget(mainPage);

	connect(mainInputTab->nameEdit, &QLineEdit::textChanged, this, &PhoneDirectory::checkMainInput);
	connect(mainInputTab->surnameEdit, &QLineEdit::textChanged, this, &PhoneDirectory::checkMainInput);
	connect(mainInputTab->patronymicEdit, &QLineEdit::textChanged, this, &PhoneDirectory::checkMainInput);
	connect(mainInputTab->phoneEdit, &QLineEdit::textChanged, this, &PhoneDirectory::checkMainInput);
	connect(mainInputTab->emailEdit, &QLineEdit::textChanged, this, &PhoneDirectory::checkMainInput);
	connect(mainInputTab->nameEdit, &QLineEdit::textChanged, this, &PhoneDirectory::updateScrollArea);
	connect(mainInputTab->surnameEdit, &QLineEdit::textChanged, this, &PhoneDirectory::updateScrollArea);
	connect(mainInputTab->patronymicEdit, &QLineEdit::textChanged, this, &PhoneDirectory::updateScrollArea);
	connect(mainInputTab->phoneEdit, &QLineEdit::textChanged, this, &PhoneDirectory::updateScrollArea);
	connect(mainInputTab->emailEdit, &QLineEdit::textChanged, this, &PhoneDirectory::updateScrollArea);
	connect(mainInputTab->button, &QPushButton::clicked, this, &PhoneDirectory::addPerson);

	connect(mainSortButton, &QPushButton::clicked, this,
		[=]() {
			isUpSorting = !isUpSorting;
			mainSortButton->setIcon(QIcon(QString("Pointer") +
				(isUpSorting ? "1" : "0") +
				QString(".png")));
			updateScrollArea(); });

	connect(mainSortComboBox, &QComboBox::currentTextChanged, this, &PhoneDirectory::updateScrollArea);
}

void PhoneDirectory::formSecondPage()
{
	secondPage = new QWidget;

	personPhoto = new QLabel;
	changePhotoButton = new QPushButton("Choose different\n    photo");
	applyButton = new QPushButton("Apply");
	closeButton = new QPushButton;
	personText = new QTextEdit("Information about ");
	personData = new InputTab(0);

	closeButton->setFixedSize(30, 30);
	applyButton->setFixedSize(70, 30);
	personPhoto->setFixedSize(130, 130);
	changePhotoButton->setFixedWidth(130);
	personData->setMaximumSize(500, 180);
	personData->setMinimumSize(150, 180);

	QPixmap pix("delete.png");
	closeButton->setIcon(pix);

	QGridLayout* secondLayout = new QGridLayout;

	secondLayout->addWidget(personPhoto, 0, 0, 2, 2);
	secondLayout->addWidget(changePhotoButton, 3, 0, 1, 2);
	secondLayout->addWidget(new QWidget, 0, 5);
	secondLayout->addWidget(applyButton, 0, 6);
	secondLayout->addWidget(closeButton, 0, 7);
	secondLayout->addWidget(personText, 4, 0, 3, 8);
	secondLayout->addWidget(personData, 0, 2, 4, 3);

	secondLayout->setColumnStretch(5, 1);
	secondLayout->setRowStretch(4, 1);

	secondPage->setLayout(secondLayout);
	this->addWidget(secondPage);

	connect(closeButton, &QPushButton::clicked, this, [=]() { this->setCurrentWidget(mainPage); });
	connect(personData->nameEdit, &QLineEdit::textChanged, this, [=]() { personData->checkInput(); });
	connect(personData->surnameEdit, &QLineEdit::textChanged, this, [=]() { personData->checkInput(); });
	connect(personData->patronymicEdit, &QLineEdit::textChanged, this, [=]() { personData->checkInput(); });
	connect(personData->phoneEdit, &QLineEdit::textChanged, this, [=]() { personData->checkInput(); });
	connect(personData->emailEdit, &QLineEdit::textChanged, this, [=]() { personData->checkInput(); });
	connect(applyButton, &QPushButton::clicked, this, &PhoneDirectory::changeCurPerson);
	connect(changePhotoButton, &QPushButton::clicked, this, &PhoneDirectory::changeCurPersonPhoto);
}

void PhoneDirectory::changeCurPersonPhoto()
{
	QFileDialog dialog(this);
	dialog.setNameFilter("Images (*.png *.jpg *.jpeg)");

	QString newPict;
	if (dialog.exec() == QDialog::Accepted)
		newPict = dialog.selectedFiles().first();
	if (newPict != "")
	{
		personPhoto->setPixmap(QPixmap(newPict).scaled(personPhoto->size()));
		personPhoto->setProperty("fileName", newPict);
	}
}

void PhoneDirectory::openSecondView()
{
	personData->nameEdit->setText(currentPerson->name);
	personData->surnameEdit->setText(currentPerson->surname);
	personData->patronymicEdit->setText(currentPerson->patronymic);
	personData->phoneEdit->setText(currentPerson->phone);
	personData->emailEdit->setText(currentPerson->email);
	personData->dateEdit->setDate(currentPerson->birthDate);

	personPhoto->setPixmap(QPixmap(currentPerson->profilePict).scaled(personPhoto->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	personPhoto->setProperty("fileName", currentPerson->profilePict);
	personText->setText(currentPerson->info);

	this->setCurrentWidget(secondPage);
}

void PhoneDirectory::updateScrollArea()
{
	if (mainScrollArea->widget())
		delete mainScrollArea->widget();

	QWidget* container = new QWidget;
	QVBoxLayout* layout = new QVBoxLayout;
	container->setLayout(layout);

	int sortedSize = sortPersons();
	for (int i = 0; i < sortedSize; i++)
	{
		QPushButton* mainButton = new QPushButton(persons[i]->name + " " + persons[i]->surname + " " + persons[i]->patronymic + "\n" +
			persons[i]->phone + "\n" +
			persons[i]->email);
		QPushButton* deleteButton = new QPushButton;
		deleteButton->setIcon(QPixmap("delete.png"));

		mainButton->setStyleSheet("text-align: left; padding-left: 20px;");

		mainButton->setMinimumSize(90, 60);
		deleteButton->setFixedSize(60, 60);

		QHBoxLayout* hLayout = new QHBoxLayout;
		hLayout->addWidget(mainButton);
		hLayout->addWidget(deleteButton);
		layout->addLayout(hLayout);

		connect(mainButton, &QPushButton::clicked, this, [=]() { currentPerson = persons[i];
		openSecondView(); });
		connect(deleteButton, &QPushButton::clicked, this,
			[=]() { for (int j = 0; j != persons.size(); j++)
		{
			if (persons[i] == persons[j])
			{
				persons.remove(j);
				break;
			}
		}
		updateScrollArea();
			});
	}

	container->setLayout(layout);
	layout->setSpacing(0);
	mainScrollArea->setWidget(container);
	mainScrollArea->setWidgetResizable(true);
}

void PhoneDirectory::fillContacts()
{
	QFile file("PersonsData.txt");
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QTextStream in(&file);

	while (!in.atEnd()) {
		QString name = in.readLine();
		QString surname = in.readLine();
		QString patronymic = in.readLine();
		QString phone = in.readLine();
		QString email = in.readLine();
		QDate birthDate = QDate::fromString(in.readLine(), "dd.MM.yyyy");
		QString profilePict = in.readLine();
		QString info = in.readLine().replace("SpEcIaLSyMbOl", "\n");
		in.readLine();

		persons.emplace_back(new Person(name, surname, patronymic, phone, email,
			birthDate, profilePict, info));
	}

	file.close();
}

void PhoneDirectory::rewriteDataFile()
{
	QFile file("PersonsData.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "Не удалось открыть файл",
			"Не удалось открыть файл для сохранения данных");
		return;
	}

	QTextStream out(&file);
	for (int i = 0; i < persons.size(); i++) {
		out << persons[i]->name << "\n";
		out << persons[i]->surname << "\n";
		out << persons[i]->patronymic << "\n";
		out << persons[i]->phone << "\n";
		out << persons[i]->email << "\n";
		out << persons[i]->birthDate.toString("dd.MM.yyyy") << "\n";
		out << persons[i]->profilePict << "\n";
		out << persons[i]->info.replace("\n", "SpEcIaLSyMbOl") << "\n";
		out << "\n";
	}
	file.close();
}

PhoneDirectory::~PhoneDirectory()
{
	rewriteDataFile();
}

int PhoneDirectory::sortPersons()
{
	int result = 0;
	if ((!mainInputTab->allFieldsAreEmpty()) || mainInputTab->dateEdit->date() != QDate(2000, 1, 1))
	{
		Person* requestedPerson = new Person(mainInputTab->nameEdit->text(),
			mainInputTab->surnameEdit->text(),
			mainInputTab->patronymicEdit->text(),
			mainInputTab->phoneEdit->text(),
			mainInputTab->emailEdit->text(),
			mainInputTab->dateEdit->date());

		//Всех людей, которые совпадают с поиском переносим в начало вектора
		for (int i = 0; i < persons.size(); i++)
		{
			if ((*persons[i]).contains(*requestedPerson))
			{
				shared_ptr<Person> tmp = persons[result];
				persons[result] = persons[i];
				persons[i] = tmp;
				result++;
			}
		}
	}
	else
		result = persons.size();

	//Сортируем
	if (mainSortComboBox->currentText() != "No Sort")
	{
		sort(persons.begin(), persons.begin() + result,
			comparicments[mainSortComboBox->currentText()]);
		if (!isUpSorting)
			reverse(persons.begin(), persons.begin() + result);
	}

	return result;
}



