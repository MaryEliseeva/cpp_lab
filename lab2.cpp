// lab1c.cpp: определяет точку входа для приложения.
//

#include "lab2.h"

using namespace std;
double Diskriminant(double a, double b, double c) {
	double result = b * b - 4.0f * a * c;
	return result;
}

double cleanZero(double value) {
	if (value == 0.0) {
		return 0.0;
	}
	return value;
}

void Error_A_Zero(double b, double c) {

	if (c == 0 && b != 0) {
		cout << "Корень уравнения: " << endl << "x1 = " << 0.0 << endl;
		return;
	}

	if (c == 0 && b == 0) {
		cout << "Корни уравнения - множество вещественных чисел" << endl;
		return;
	}

	if (b == 0 && c != 0) {
		cout << "Уравнение не имеет смысла" << endl;
		return;
	}

	double root = (-1.0) * c / b;
	cout << "Корень уравнения: " << endl << "x1 = " << root << endl;


}

void Roots(double a, double b, double c, double& root1, double& root2) {

	if (a == 0.0) {
		Error_A_Zero(b, c);
		return;
	}
	double diskriminant = Diskriminant(a, b, c);

	if (diskriminant < 0.0) root1 = root2 = -1;

	else if (diskriminant == 0.0) {

		root1 = b / (-2.0 * a);
	}
	else {

		root1 = ((-1.0) * b - sqrt(diskriminant)) / (2.0 * a);
		root2 = ((-1.0) * b + sqrt(diskriminant)) / (2.0 * a);
	}
}

void Print(double a, double b, double c) {
	cout << "Уравнение: " << a << "x^2";
	if (b < 0) {
		cout << b << "x";
	}
	else {
		cout<<"+" << b << "x";
	}
	if (c < 0) {
		cout << c << "=0"<<endl;
	}
	else {
		cout << "+" << c << "=0" << endl;
	}
}

void Excellent_Student::solve(const std::string& equation_filename) {
	ifstream equation(equation_filename);
	int current_n_equation = 0;
	std::string line;
	while (std::getline(equation, line) ) {
		double a, b, c;
		double root1, root2;
		std::istringstream iss(line);
		if (!(iss >> a >> b >> c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		if (std::isnan(a) || std::isnan(b) || std::isnan(c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		Roots(a, b, c, root1, root2);
		solution[current_n_equation][0] = root1;
		solution[current_n_equation][1] = root2;
		current_n_equation++;
	}
	equation.close();
}

void Bad_Student::solve(const std::string& equation_filename) {
	ifstream equation(equation_filename);
	int current_n_equation = 0;
	std::string line;
	while (std::getline(equation, line)) {
		solution[current_n_equation][0] = 0;
		solution[current_n_equation][1] = 0;
		current_n_equation++;
	}
	equation.close();
}

void Good_Student::solve(const std::string& equation_filename) {
	ifstream equation(equation_filename);
	int number_current_equation = 0;
	std::string line;
	std::srand(static_cast<unsigned int>(std::time(0)));
	while (std::getline(equation, line)) {
		double current_probability = static_cast<double>(std::rand()) / RAND_MAX;// если вероятность меньше,
		//чем шанс на успешное выполнение - то неправильный ответ
		double a, b, c;
		double root1, root2;
		std::istringstream iss(line);
		if (!(iss >> a >> b >> c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		if (std::isnan(a) || std::isnan(b) || std::isnan(c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		if (current_probability <= probability) {
			Roots(a, b, c, root1, root2);
			solution[number_current_equation][0] = root1;
			solution[number_current_equation][1] = root2;
			number_current_equation++;
		}
		else {
			solution[number_current_equation][0] = 0;
			solution[number_current_equation][1] = 0;
			number_current_equation++;
		}
	}
	equation.close();
};

void Teacher::check(std::vector<std::shared_ptr<Student>> group) {

	for (auto& student : group) {

		double count_right_answer = 0;

		for (int i = 0; i < count_equation;i++) {
			if (solution[i][0] == student->solution[i][0] && solution[i][1] == student->solution[i][1])  count_right_answer++;
		}
		student->result = count_right_answer;
	}
};
void Teacher::give_task(std::vector<std::shared_ptr<Student>> group, std::string equation_filename){
	for (auto& student : group) {
		student->solve(equation_filename);
	}
};

void Teacher::show_result(std::vector<std::shared_ptr<Student>> group) {
	std::cout << "  Table of Progress  " << std::endl;
	std::cout << "Name\t      Result" << std::endl;
	for (auto& student : group) {
		std::cout << student->name << "\t\t" << student->result <<"/"<<count_equation<< std::endl;
	}
};

void Teacher::solve(const std::string& equation_filename) {
	ifstream equation(equation_filename);
	int current_n_equation = 0;
	std::string line;
	while (std::getline(equation, line)) {
		double a, b, c;
		double root1, root2;
		std::istringstream iss(line);
		if (!(iss >> a >> b >> c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		if (std::isnan(a) || std::isnan(b) || std::isnan(c)) {
			std::cout << "Ошибка при чтении." << std::endl;
			continue;
		}
		Roots(a, b, c, root1, root2);
		solution[current_n_equation][0] = root1;
		solution[current_n_equation][1] = root2;
		current_n_equation++;
	}
	equation.close();
}