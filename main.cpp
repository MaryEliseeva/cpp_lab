#include "lab2.h"

using namespace std;

int main()
{
	setlocale(LC_CTYPE, "Russian");
	string equation = "task.txt";
	string outputFilename = "output.txt";
    string line;

    Teacher teacher;

    Excellent_Student Masha("Masha");
    Bad_Student Leonid("Leonid");
    Good_Student Gleb("Gleb", 0.8);
    Good_Student Max("Maxim", 0.5);
    Good_Student Kate("Kate", 0.3);


    std::vector<std::shared_ptr<Student>> group_pmi3;

    group_pmi3.push_back(std::make_shared<Excellent_Student>(Masha));
    group_pmi3.push_back(std::make_shared<Good_Student>(Gleb));
    group_pmi3.push_back(std::make_shared<Good_Student>(Max));
    group_pmi3.push_back(std::make_shared<Good_Student>(Kate));
    group_pmi3.push_back(std::make_shared<Bad_Student>(Leonid)); 


    teacher.solve(equation);

    teacher.give_task(group_pmi3,equation);

    teacher.check(group_pmi3);
    ofstream outputFile("output.txt");
    streambuf* coutBuffer = cout.rdbuf(); // Сохраняем текущий буфер cout
    cout.rdbuf(outputFile.rdbuf()); // Перенаправляем вывод в outputFile
    teacher.show_result(group_pmi3);

	return 0;
}
