#include "classeviva-cpp/Classeviva.h"
#include <fstream>
#include <chrono>


struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> m_Start;
	
	Timer() : m_Start(std::chrono::high_resolution_clock::now()) {}
	~Timer() {
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_Start;
		std::cout << "Function took " << duration.count() << " ms";
	}
};

int main() {
	std::array<std::string, 2> credentials;

	std::ifstream file;

	file.open("credentials.txt");

	if (file.is_open()) {
		file >> credentials[0] >> credentials[1];
		file.close();
	}
	else {
		std::cout << "Could not open file!" << std::endl;
		return -1;
	}

    Classeviva::ClassevivaClient client(credentials[0], credentials[1]);

	if (!client.Login()) {
		std::cout << "Something went wrong!" << std::endl;
		return -1;
	}

	std::cout << "Successfully logged in as " << client.GetName() << " " << client.GetSurname() << std::endl;

	std::vector<Classeviva::Grade> grades;
	if (!client.GetGrades(grades)) {
		std::cout << "Could not get grades!" << std::endl;
		return -1;
	}


	std::cout << grades.size() << std::endl;
	for (Classeviva::Grade& grade : grades) {
		std::cout << grade.subjectDescription << std::endl;
	}

	std::cout << "Grade total average is: " << Classeviva::GetGradesAverage(grades, "LINGUA E CULTURA LATINA") << std::endl;
	//std::cout << "\a" << std::endl;
}