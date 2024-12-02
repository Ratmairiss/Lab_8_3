
#include "Person.h"
#include <memory>
using namespace std;

QMap <QString, bool (*)(shared_ptr<Person>, shared_ptr<Person>)> comparicments{
	{"Sort By Name",       [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->name < p2->name; }},
	{"Sort By Surname",    [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->surname < p2->surname; }},
	{"Sort By Patronymic", [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->patronymic < p2->patronymic; }},
	{"Sort By Phone",      [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->phone < p2->phone; }},
	{"Sort By Email",      [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->email < p2->email; }},
	{"Sort By Birth Date", [](shared_ptr<Person> p1, shared_ptr<Person> p2) { return p1->birthDate < p2->birthDate; }}
};