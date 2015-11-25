// g++ -g tests.cpp -o tests
// valgrind --leak-check=full -v ./tests

#include <algorithm>
#include "../../mini_test.h"
#include "diccRapido.h"

using namespace std;
using namespace aed2;

template<typename T>
string to_s(const T& m) {
 	ostringstream os;
	os << m;
	return os.str();
}

string remove_spaces(const string& s) {
  string out(s);
  out.erase(remove(out.begin(), out.end(), ' '), out.end());
  out.erase(remove(out.begin(), out.end(), '\n'), out.end());
  return out;
}

void diccRapido_constructor() {
	DiccRapido<int, int> d();
}

void diccRapido_definir() {
	DiccRapido<int, int> d;
	d.Definir(1, 1);
}

int main() {
	//test interfáz pública de campus
	RUN_TEST(diccRapido_constructor);
	RUN_TEST(diccRapido_definir);

	return 0;
}