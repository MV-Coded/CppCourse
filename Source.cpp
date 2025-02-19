#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::pair;

// чтение значений из файла и помещение их по ссылкам
void readfile(double& a, double& b, double& c, std::string filenametxt) {
	cout << "Reading data from file " << filenametxt << endl;
	std::ifstream read("values.txt", std::ios::in);
	if (read.is_open()) {
		read >> a >> b >> c;
		cout << "Data from " << filenametxt << " read successfully" << endl;
	}
	else {
		cout << "Error: cannot read from file. Values remain unchanged" << endl;
	}
	read.close();
}
// нахождение корней полинома. при дискриминанте < 0 выкидывает пару НАНов (они по логике вычислительного алгоритма никогда не должны выдаться иначе)
pair<double, double> solvequadratic(double squarecoeff, double linearcoeff, double zerocoeff) {
	if (squarecoeff != 0) {
		double D = (linearcoeff * linearcoeff) - 4 * squarecoeff * zerocoeff;
		if (D < 0) {
			return { NAN, NAN };
		}
		if (D == 0) {
			return { NAN, linearcoeff / -2 / squarecoeff };
		}
		return { (-linearcoeff - sqrt(D)) / 2 / squarecoeff, (-linearcoeff + sqrt(D)) / 2 / squarecoeff };
	}
	else cout << "Error: non-quadratic equations are not supported" << endl;
	return { NAN, NAN };
}
// запись в файл значений из ссылки на константную пару чисел
void writefile(pair<double, double>const &doublepair, std::string filenametxt) {
	cout << "Writing in " << filenametxt << endl;
	std::ofstream write(filenametxt, std::ios::app);
	if (write.is_open()) {
		write << doublepair.first << " " << doublepair.second << endl;
		cout << "Written answer in " << filenametxt << endl;
	}
	else {
		cout << "Error: cannot write in file" << endl;
	}
	write.close();
}
int main(void) {
	double a = {}, b = {}, c = {};
	readfile(a, b, c, "values.txt");
	auto roots = solvequadratic(a, b, c);
	writefile(roots, "answer.txt");
	return 0;
}