#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//глобальные переменные
const int rows = 8;
const int cols = 7;
const int numCoord = 3;
//...\_*_*_/.\_*_*_/.\_*_*_/.\_*_*_/.....

string stringFromFile(const char* direct)
{
	char buff[150]; // буфер промежуточного хранения считываемого из файла текста
	ifstream fin("input.txt"); // открыли файл для чтения
	string a;
	while (fin.getline(buff, 150))
	{
		a += buff;
		a += '\n';
	}
	fin.close();
	return a;
}

struct Point
{
	float x;
	float y;
};

struct Sheet
{
	int num;
	string color;
	Point points[3];
};

/**
* возращает массив из номера и трех координат
*/
float** strToFloatMatrix(string text)
{
	bool isDigitValue = true;
	//flag который нужен чтоб понять что запятая относится к числу
	//или что после равно идет точно число, а относится,
	//если до этого была целая часть числа с ',' или '='
	string num;
	//просто выделение памяти
	float** m = new float* [rows];
	for (int i = 0; i < rows; i++)
	{
		m[i] = new float[cols];
		for (int j = 0; j < cols; j++)
			m[i][j] = NULL;
	}
	int r = 0, c = 0;

	//идем по символам string
	//ниже распишу условия все
	for (int i = 0; i < text.size(); i++)
	{
		//создание слова в виде string
		//если любая запятая,даже которая не является мантиссой числа ,
		//сконкатинируется с num(добавится в конец)
		if (isdigit(text[i]) && isDigitValue == true || text[i] == ',')
		{
			num += text[i];
		}
		//убираем флаг если до этого не было признака того что может быть число(',' или '=')
		if (isDigitValue == true && !isdigit(text[i]))
		{
			isDigitValue = false;
		}
		//ставим флаг, дальше идет число
		if (text[i] == '=' || text[i] == ',')
		{
			isDigitValue = true;
		}
		//если не число и не запятая нашли нужное число. То есть когда он пройдет число
		// и будет уже на буквенном сиволе каком-то то мы нашли число
		if (text[i] != ',' && num != "" && !isdigit(text[i])) {
			m[r][c] = atof(num.c_str());
			c++;
			num = "";
		}
		//просто переход на новую строчку
		if (text[i] == '\n') {
			isDigitValue = true;
			r++;
			c = 0;
		}
	}
	return m;
}

string* colors(string a)
{
	bool isColor = false;
	string* colors = new string[rows];
	string color = "";
	int r = 0;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] == ':')
		{
			isColor = true;
		}
		//коды символов
		if (isColor == true && a[i] > -65 && a[i] < 0)
		{
			color += a[i];
		}
		if (a[i] == ';')
		{
			isColor = false;
		}
		if (a[i] == '\n')
		{
			colors[r++] = color;
			color = "";
		}
	}
	return colors;
}

Sheet* sheets(string text)
{
	float** m = strToFloatMatrix(text);
	string* c = colors(text);
	Sheet* lists = new Sheet[rows];
	for (size_t i = 0; i < rows; i++)
	{
		lists[i].color = c[i];
		lists[i].num = m[i][0];
		for (size_t j = 0, col = 1; j < numCoord; j++, col += 2)
		{
			lists[i].points[j].x = m[i][col];
			lists[i].points[j].y = m[i][col + 1];
		}
	}
	return lists;
}

void printSheets(Sheet* sheets, int rows)
{
	for (size_t i = 0; i < rows; i++)
	{
		cout << " Номер листа: " << sheets[i].num << " Цвет: " << sheets[i].color;
		for (size_t j = 0; j < numCoord; j++)
		{
			cout << " " << sheets[i].points[j].x << " " << sheets[i].points[j].y << " ";
		}
		cout << endl;
	}
}
//проверка что равнобедренный
bool isIsoscelesTriangle(Point* points)
{
	float distance1 = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2));
	float distance2 = sqrt(pow(points[1].x - points[2].x, 2) + pow(points[1].y - points[2].y, 2));
	float distance3 = sqrt(pow(points[0].x - points[2].x, 2) + pow(points[0].y - points[2].y, 2));
	if (abs(distance1 - distance2) < 0.01 || abs(distance1 - distance3) < 0.01 || abs(distance2 - distance3) < 0.01)
		return true;
	else
		return false;
}

int sizeResult(Sheet* sheets)
{
	int sizeResult = 0;
	for (size_t i = 0; i < rows; i++)
	{
		if (sheets[i].color == "синий" && isIsoscelesTriangle(sheets[i].points))
			sizeResult++;
	}
	return sizeResult;
}

Sheet* solution(Sheet* sheets)
{
	//посмотрим сколько листов в итоге будет
	int resSize = sizeResult(sheets);
	//добавим в res
	Sheet* res = new Sheet[resSize];
	int count = 0;
	for (size_t i = 0; i < rows; i++)
	{
		if (sheets[i].color == "синий" && isIsoscelesTriangle(sheets[i].points))
		{
			res[count++] = sheets[i];
		}
	}
	return res;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
	//string text = stringFromFile("input.txt");
	string text = "42)цвет: зеленый; х1=5, у1=20, х2=12, у2=20,8; х3=40,6, у3=32;\n" 
		"43)цвет: синий; х1=5, у1=20, х2=15, у2=20; х3=10, у3=10;\n"
		"44)цвет: зеленый; х1=5,3, у1=20, х2=15, у2=20; х3=10, у3=10,9;\n"
		"45)цвет: желтый; х1=5,3, у1=20, х2=15, у2=20; х3=10, у3=10,9;\n"
		"46)цвет: зеленый; х1=5, у1=20, х2=15, у2=20; х3=10, у3=10;\n"
		"47)цвет: синий; х1=5, у1=20, х2=15, у2=20; х3=10, у3=10;\n"
		"48)цвет: синий; х1=5,3, у1=20, х2=15, у2=20; х3=10, у3=10,9;\n"
		"49)цвет: зеленый; х1=5,3, у1=20, х2=15, у2=20; х3=10, у3=10,9;\n" ;

	cout << "Исходные листы: " << endl;
	Sheet* lists = sheets(text);
	printSheets(lists, rows);
	cout << "Полученные листы синего цвета с равнобедренными треугольниками: " << endl;
	Sheet* result = solution(lists);
	printSheets(result, sizeResult(lists));

	system("pause");
	return 0;
}