
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
	char buff[150]; // буфер промежуточного хранения считываемого из файла текста
	ifstream fin("cppstudio.txt"); // открыли файл для чтения
	fin.getline(buff, 150);

	fin.close();
	system("pause");
	return 0;
}
