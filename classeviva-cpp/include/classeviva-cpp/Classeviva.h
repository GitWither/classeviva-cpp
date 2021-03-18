#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <array>
#include <vector>
#include <any>
//temp
#include <iostream>

namespace Classeviva {

	namespace Constants {
		constexpr const char LOGIN_UID[] = "uid";
		constexpr const char LOGIN_PASS[] = "pass";

		constexpr const char RAW_DATA_TYPE[] = "raw";
		constexpr const char APPLICATION_JSON_DATA_TYPE[] = "application/json";

		constexpr const char BASE_URL[] = "https://web.spaggiari.eu";

		constexpr const char LOGIN_PATH[] = "/rest/v1/auth/login";
		constexpr const char BASE_API_PATH[] = "/rest/v1/students/";

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

	class ClassevivaClient {
	private:
		std::string m_Password;
		std::string m_Email;

		std::string m_Name;
		std::string m_Surname;

		std::string m_Token;
		std::string m_Id;

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