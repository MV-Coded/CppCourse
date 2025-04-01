#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <queue>
#include <vector>
#include <random>
#include <iomanip>
#include <locale>

constexpr double P_global = 0.5; // вероятность успешного решения кв.уравнения
constexpr int kontrolnaya_size = 5; // количество кв.уравнений в контрольной

using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::queue;
using std::vector;
using std::string;
using std::setw;
using std::left;

struct Solution { // структура решения - письма, присылаемого студентом преподу
	string name;
	vector<pair<double, double>> answers; 
	vector<vector<double>> tasks;
	vector<bool> correctness;
};

enum StudentType {POOR, OK, EXCELLENT}; // двоечник, окей и отличник
// структура студент. методы: может прочитать задание (из текстового файла), решить уравнения, составить и отправить письмо
class Student {
public:
	string student_name;
	StudentType skill_level;
	vector<vector<double>> polynoms;
	vector<pair<double, double>> answers;

	pair<double, double> answerforpolynom(vector<double> poly) {
		double secondcoeff = poly[0];
		double onecoeff = poly[1];
		double zerocoeff = poly[2];
		if (secondcoeff != 0) {
			double D = (onecoeff * onecoeff) - 4 * secondcoeff * zerocoeff;
			if (D < 0) {
				return { double(INFINITY), double(INFINITY) };
			}
			if (D == 0) {
				return { onecoeff / -2 / secondcoeff, onecoeff / -2 / secondcoeff };
			}
			return { (-onecoeff - sqrt(D)) / 2 / secondcoeff, (-onecoeff + sqrt(D)) / 2 / secondcoeff };
		}
		else cout << "Error: non-quadratic equation detected. Fix your polynoms. " << endl;
		return { double(INFINITY), double(INFINITY) };
	}
public:
	Student() : Student("AndreyVano", OK) {}
	Student(string input_name, StudentType input_level) : student_name(input_name), skill_level(input_level) {}

	void acceptwork(const string& filenametxt) {
		double twocoeff, onecoeff, zerocoeff = 0;
		std::ifstream read(filenametxt, std::ios::in);
		if (read.is_open()) {
			do {
				read >> twocoeff >> onecoeff >> zerocoeff;
				polynoms.push_back({ twocoeff, onecoeff, zerocoeff });
			} while (!read.eof());
		}
		else {
			cout << "Error: cannot read from file. Values remain unchanged" << endl;
		}
		read.close();
	}
	void solvework() {
		for (auto& polynom : polynoms) {
			if (skill_level == POOR) {
				for (int _ = 0; _ < polynoms.size(); _++) {
					answers.push_back({ 0, 0 });
				}
			}
			else if (skill_level == EXCELLENT) {
				answers.push_back(answerforpolynom(polynom));
			}
			else {
				double P_current = double(std::rand())/RAND_MAX;
				if (P_current >= P_global) {
					answers.push_back(answerforpolynom(polynom));
				}
				else answers.push_back({ 0, 0 });
			}
		}
	}
	Solution createletter() {
		Solution letter = { student_name, answers, polynoms };
		return letter;
	}
};
// класс учитель. методы: генерирует условия контрольных, принимает письмо студента, проверяет первое присланное на почту письмо, отсылает (печатает) таблицу
class Teacher {
private:
	queue<Solution> mail;
	vector<Solution> results;
	pair<double, double> currentanswer;

	pair<double, double> answerforpolynom(vector<double> poly) {
		double secondcoeff = poly[0];
		double onecoeff = poly[1];
		double zerocoeff = poly[2];
		if (secondcoeff != 0) {
			double D = (onecoeff * onecoeff) - 4 * secondcoeff * zerocoeff;
			if (D < 0) {
				return { double(INFINITY), double(INFINITY) };
			}
			if (D == 0) {
				return { onecoeff / -2 / secondcoeff, onecoeff / -2 / secondcoeff };
			}
			return { (-onecoeff - sqrt(D)) / 2 / secondcoeff, (-onecoeff + sqrt(D)) / 2 / secondcoeff };
		}
		else cout << "Error: non-quadratic equation detected. Fix your polynoms. " << endl;
		return { double(INFINITY), double(INFINITY) };
	}
public:
	void generate_kontrolnaya(const string& filenametxt) {
		std::ofstream write(filenametxt, std::ios::out);
		if (write.is_open()) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(-10.0, 10.0);
			for (int _ = 0; _ < kontrolnaya_size; _++) {
				write << dis(gen) << " " << dis(gen) << " " << dis(gen) << endl;
			}
		}
		else {
			cout << "Error: cannot write in file" << endl;
		}
		write.close();
	}
	void accept_letter(Solution letter) {
		mail.push(letter);
	}
	void review_oldest_letter() {
		auto letter = mail.front();
		auto polynoms = letter.tasks;
		for (int polynom_index = 0; polynom_index < kontrolnaya_size; polynom_index++) {
			letter.correctness.push_back((answerforpolynom(polynoms[polynom_index]) == letter.answers[polynom_index] ? true : false));
		}
		results.push_back(letter);
		mail.pop();
	}
	void print_table() {
		for (Solution& letter : results) {
			cout << setw(10) << left << "Имя"
				<< " ### "
				<< setw(7) << left << "Задача"
				<< " ### "
				<< setw(7) << left << "Ответы"
				<< " ### "
				<< setw(4) << left << "Правильность"
				<< endl;
			for (int polynom_index = 0; polynom_index < kontrolnaya_size; polynom_index++) {
				if (polynom_index == 0) {
					cout << setw(10) << left << letter.name;
				}
				else {
					cout << setw(10) << left << "";
				}

				cout << " ### ";
				cout << "[";
				for (int polynom_size = 0; polynom_size < 3; polynom_size++) {
					cout << letter.tasks[polynom_index][polynom_size];
					if (polynom_size != letter.tasks[polynom_index].size() - 1) cout << ", ";
				}
				cout << "]";

				cout << " ### ";
				cout << "(" << answerforpolynom(letter.tasks[polynom_index]).first << ", " << answerforpolynom(letter.tasks[polynom_index]).second << ")";
				
				// Правильность
				cout << " ### ";
				cout << (letter.correctness[polynom_index] ? "Правильно" : "Неправильно");

				cout << endl;
			}
		}
	}
};
int main(void) {
	setlocale(LC_ALL, "Russian");
	std::srand(0);

	int number_of_students = 60;
	int excellence_barrier = 17;
	int poor_barrier = 37;
	vector<Student> student_list = {};
	Teacher iskander = {};
	for (int id = 0; id < number_of_students; id++) { // на месте айди может быть любое имя
		if (id < excellence_barrier) {
			student_list.push_back({ ("id" + std::to_string(id)), EXCELLENT }); 
		}
		else if (id < poor_barrier) {
			student_list.push_back({ ("id" + std::to_string(id)), POOR }); 
		}
		else {
			student_list.push_back({ ("id" + std::to_string(id)), OK }); 
		}
	}
	iskander.generate_kontrolnaya("krmatan.txt");
	for (Student& someguy : student_list) {
		someguy.acceptwork("krmatan.txt");
		someguy.solvework();
		iskander.accept_letter(someguy.createletter()); // вообще тут добавляются ученики ровно по айдишникам, но программа устойчива к добавлению вразнобой и будет в порядке очереди всё проверять
	}
	for (int student_iterator = 0; student_iterator < number_of_students; student_iterator++) {
		iskander.review_oldest_letter();
	}
	iskander.print_table();
	return 0;
}