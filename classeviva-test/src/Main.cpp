#include "Classeviva.h"
#include <fstream>

int main() {
	std::array<std::string, 2> credentials;

	std::ifstream file;

	file.open("credentials.txt");

	if (file.is_open()) {
		file >> credentials[0] >> credentials[1];
		file.close();
	}

    Classeviva::ClassevivaClient client(credentials[0], credentials[1]);

	client.Login();
	std::vector<Classeviva::Grade> grades;
	client.GetGrades(grades);

	std::cout << grades.size() << std::endl;
	for (Classeviva::Grade grade : grades) {
		std::cout << grade.gradeType << std::endl;
	}


	std::cin.get();
}