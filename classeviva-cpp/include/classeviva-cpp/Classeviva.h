#pragma once
#include <nlohmann/json.hpp>
#include <regex>
#include <array>
#include <vector>
#include <any>
//temp
#include <iostream>

namespace Classeviva {

	namespace Constants {

		constexpr const char LOGIN_PATH[] = "https://web.spaggiari.eu/rest/v1/auth/login";
		constexpr const char BASE_API_PATH[] = "https://web.spaggiari.eu/rest/v1/students/";

		constexpr const char GRADES_PATH[] = "/grades";
	}

	struct Grade {
		const std::string subjectDescription;
		const std::string eventDate;
		const double decimalValue;
		const std::string displayValue;
		const std::string notes;
		const std::string periodDescription;
		const std::string gradeType;
		const std::string color;

	public:

		Grade(const std::string&, const std::string&, const double, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	};

	double GetGradesAverage(const std::vector<Grade>&, const std::string& filter = "");

	class ClassevivaClient {
	private:
		std::string m_Password;
		std::string m_Email;

		std::string m_Name;
		std::string m_Surname;

		std::string m_Token;
		std::string m_Id;

		std::vector<Grade> m_CachedGrades;

	public:
		ClassevivaClient(const std::string&, const std::string&);

		bool Login();

		inline const std::string& GetName() const {
			return m_Name;
		}
		inline const std::string& GetSurname() const {
			return m_Surname;
		}

		bool GetGrades(std::vector<Classeviva::Grade>&) const;
	};
}