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
		std::cout << "Could not open credentials file!" << std::endl;
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


	std::cout << "Grades count: " << grades.size() << std::endl;
	for (Classeviva::Grade& grade : grades) {
		std::cout << grade.color <<  " ";
	}
	std::cout << std::endl;

	std::vector<Classeviva::Document> documents;
	if (!client.GetDocuments(documents)) {
		std::cout << "Could not get documents!" << std::endl;
	}

	std::cout << "Document count: " << documents.size() << std::endl;
	for (Classeviva::Document& doc : documents) {
		std::cout << doc.category << " ";
	}
	std::cout << std::endl;


	Classeviva::StudentInfo studentInfo;

	if (!client.GetStudentInfo(studentInfo)) {
		std::cout << "Could not get school name!" << std::endl;
	}

	std::cout << "Birth date: " << studentInfo.birthDate << std::endl;
	std::cout << "Fiscal code: " << studentInfo.fiscalCode << std::endl;
	std::cout << "MIUR division code: " << studentInfo.miurDivisionCode << std::endl;
	std::cout << "MIUR school code: " << studentInfo.miurSchoolCode << std::endl;
	std::cout << "School city: " << studentInfo.schoolCity << std::endl;
	std::cout << "School code: " << studentInfo.schoolCode << std::endl;
	std::cout << "School dedication: " << studentInfo.schoolDedication << std::endl;
	std::cout << "School name: " << studentInfo.schoolName << std::endl;
	std::cout << "School province: " << studentInfo.schoolProvince << std::endl;

	std::cout << "Grade total average is: " << Classeviva::GetGradesAverage(grades, "LINGUA E CULTURA LATINA") << std::endl;
	//std::cout << "\a" << std::endl;

}