// lab2c.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <cmath>
#include <locale.h>
#include <fstream>
#include<string>
#include <sstream>
#include <cctype>
#include <vector>
#include <cstdlib>  
#include <ctime>    
#define count_equation 6//кол-во уравнений 


// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
class Student {
public:
    int result;
    std::string name;
    std::vector<std::vector<double>> solution;
    virtual void solve(const std::string& equation_filename)=0;
    Student(std::string name) : name(name), solution(count_equation+1, std::vector<double>(2)) {}
    virtual ~Student() { }
};

class Excellent_Student : public Student {
public:
    Excellent_Student(std::string name) : Student(name) {}
    void solve(const std::string& equation) override;
    ~Excellent_Student() override {};
};

class Bad_Student : public Student {
public:
    Bad_Student(std::string name) : Student(name) {}
    void solve(const std::string& equation)  override;
    ~Bad_Student() override {};
};

class Good_Student : public Student {
public:
    double probability;
    Good_Student(std::string name, double probability) : Student(name) {
        this->probability = probability;
    }
    void solve(const std::string& equation) override;
    ~Good_Student() override {};
};

class Teacher {
public:
    std::vector<std::vector<double>> solution;

    Teacher() : solution(count_equation + 1, std::vector<double>(2)) {}

    void solve(const std::string& equation_filename);
    void give_task(std::vector<std::shared_ptr<Student>> group, std::string equation_filename);
    void check(std::vector<std::shared_ptr<Student>> group);
    void show_result(std::vector<std::shared_ptr<Student>> group);

    ~Teacher() {};
}; 

double Diskriminant(double a, double b, double c);

double cleanZero(double value);

void Error_A_Zero(double b, double c);

void Roots(double a, double b, double c);

void Print(double a, double b, double c);

