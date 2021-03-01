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

	if (!client.Login()) {
		std::cout << "Something went wrong!" << std::endl;
	}
	std::vector<Classeviva::Grade> grades;
	if (!client.GetGrades(grades)) {
		std::cout << "Could not get grades!" << std::endl;
	}

	std::cout << grades.size() << std::endl;
	for (Classeviva::Grade grade : grades) {
		std::cout << grade.gradeType << std::endl;
	}
}