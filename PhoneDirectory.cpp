#include "PhoneDirectory.h"
#include "MainHeader.h"

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

	connect(closeButton,		        &QPushButton::clicked,     this,       [=]() { this->setCurrentWidget(mainPage); });
	connect(personData->nameEdit,       &QLineEdit::  textChanged, personData, &InputTab::checkInput);
	connect(personData->surnameEdit,    &QLineEdit::  textChanged, personData, &InputTab::checkInput);
	connect(personData->patronymicEdit, &QLineEdit::  textChanged, personData, &InputTab::checkInput);
	connect(personData->phoneEdit,      &QLineEdit::  textChanged, personData, &InputTab::checkInput);
	connect(personData->emailEdit,      &QLineEdit::  textChanged, personData, &InputTab::checkInput);
	connect(applyButton,                &QPushButton::clicked,     this,       &PhoneDirectory::changeCurPerson);
	connect(changePhotoButton,          &QPushButton::clicked,     this,       &PhoneDirectory::changeCurPersonPhoto);
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

map <char, string> PhoneDirectory::getCodesForHuffmanAlgorithm(const string& str)
{
	vector< pair<string, int> >    mapForAlgorithm;  // Структура, которая содержит 
													 // каждый символ и количество его 
													 // вхождений в строку. Будет использована 
													 // как дерево для составления кодов для 
													 // каждого символа отталкиваясь от количества.

	map   < char, string >         codes;            // Хранение кодов символов, 
												     // которые будут использоваться 
  	                                                 // при кодировании данных.
    // Заполнение mapForAlgorithm и codes.
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		if (codes.find(str[i]) == codes.end()) // Проверяем что мы ещё
			// не ввели данный символ
			// в codes.
		{
			mapForAlgorithm.emplace_back(pair<string, int>(string(1, str[i]),
				std::count(str.begin(), str.end(), str[i])));
			codes[str[i]];
		}
	}

	while (mapForAlgorithm.size() != 1)
	{
		sort(mapForAlgorithm.begin(), mapForAlgorithm.end(),   // Сортируем элементы, 
			[](pair<string, int> pr1, pair<string, int> pr2)   // чтобы они распологались 
			{                                                  // в порядке убывания 
				return (pr1.second > pr2.second);              // количества вхождений.
			});

		// Обновляем коды для символов, которые находятся в двух элементах 
		// с наименьшим количеством вхождений.
		for (int i = 1; i < 3; i++)
		{
			for (int j = 0; j < (mapForAlgorithm.end() - i)->first.size(); j++)
			{
				char symbol = (mapForAlgorithm.end() - i)->first[j];
				codes[symbol].insert(codes[symbol].begin(), '0' + (i - 1));
			}
		}

		// Объединяем элементы с наименьшим количеством вхождений.
		(mapForAlgorithm.end() - 2)->first += (mapForAlgorithm.end() - 1)->first;
		(mapForAlgorithm.end() - 2)->second += (mapForAlgorithm.end() - 1)->second;
		mapForAlgorithm.pop_back();
	}

	return codes;
}

void PhoneDirectory::encodeStr(string& str, map <char, string>& codes)
{
	// Кодируем строку и она становится набором 0 и 1, 
	// которые представляют из себя последовательностей битов.
	for (int i = 0; i < str.size();)
	{
		string code = codes[str[i]];
		str.replace(i, 1, code);
		i += code.size();
	}

	// Преобразуем строку из набора 0 и 1 в строку в которой символы
	// имеют код представленный двоичным кодом из закодировонной строки
	string res;
	res.resize((str.size() + 7) / 8, '\0');
	for (int i = 0; i < str.size(); i++)
	{
		res[i / 8] |= ((str[i] - '0') << (7 - (i % 8)));
	}

	str = res;
}

void PhoneDirectory::decodeStr(string& str, int initialSize, map<string, char> codes)
{
	for (int i = 0; i < str.size(); i += 8)
	{
		str.replace(i, 1, bitset<8>(str[i]).to_string());
	}

	for (int i = 0; i < str.size(); i++)
	{
		string code;
		int size = 0;
		do {
			if ((i + size) > str.size())
			{
				goto END;
			}
			code += str[i + size];
			size++;
		} while (codes.find(code) == codes.end());

		str.replace(i, size, string(1, codes[code]));
	}

END:

	str.resize(initialSize);
}

void PhoneDirectory::fillContacts()
{
	/*
	// Чтение не закодированнной информации для дальнейшего сравнения.
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
	*/

	// Чтение заокдированной информации.
	int initialSize;
	map<string, char> codes;

	// Чтение кодов символов и количества символов в изначальной строке
	ifstream fileIn("CodeTablet.txt");

	fileIn >> initialSize;
	fileIn.ignore();

	while (true)
	{
		char   symbol;
		string code = "";
		if (!fileIn.get(symbol))
			break;
		fileIn.ignore();
		fileIn >> code;
		fileIn.ignore();
		codes[code] = symbol;
	}
	fileIn.close();

	// Чтение закодированной строки
	fileIn.open("PersonsDataHuffman.txt", std::ios::binary);
	string str((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
	fileIn.close();

	decodeStr(str, initialSize, codes);

	QString     qStr = QString::fromStdString(str);
	QStringList qStrList = qStr.split("\n");
	for (int i = 0; i < qStrList.size() - 1; i += 8)
	{
		persons.emplaceBack(new Person(qStrList[i],
			qStrList[i + 1],
			qStrList[i + 2],
			qStrList[i + 3],
			qStrList[i + 4],
			QDate::fromString(qStrList[i + 5], "dd.MM.yyyy"),
			qStrList[i + 6],
			qStrList[i + 7].replace("SpEcIaLSyMbOl", "\n")));
	}
}

void PhoneDirectory::rewriteDataFile()
{
	// Сохранение не закодированных данных.
	QFile qFile("PersonsData.txt");
	qFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&qFile);
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
	qFile.close();

	// Сохранение закодированных данных.
	string str;
	for (int i = 0; i < persons.size(); i++)
		str += persons[i]->getAllData().toStdString();
	int    initialSize = str.size();

	map<char, string> codes = getCodesForHuffmanAlgorithm(str);

	encodeStr(str,codes);
	
	// Сохранение кодов символов и количества символов в изначальной строке
	ofstream file("CodeTablet.txt");
	file << initialSize << "\n";
	for (auto it = codes.begin(); it != codes.end(); it++)
	{
		file << it->first << " " << it->second << "\n";
	}
	file.close();

	// Сохранение закодированной строки
	file.open("PersonsDataHuffman.txt", std::ios::binary);
	file.write(str.data(), str.size());
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



