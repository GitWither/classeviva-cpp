#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"
#include <regex>
#include <array>
//temp
#include <iostream>

namespace Classeviva {

	constexpr const char LOGIN_UID[] = "uid";
	constexpr const char LOGIN_PASS[] = "pass";

	constexpr const char RAW_DATA_TYPE[] = "raw";

	constexpr const char BASE_URL[] = "https://web.spaggiari.eu";

	constexpr const char LOGIN_PATH[] = "/rest/v1/auth/login";
	constexpr const char BASE_API_PATH[] = "/rest/v1/students/";

	constexpr const char GRADES_PATH[] = "/grades";

	struct Grade {
		const char* subjectDescription;
		const char* eventDate;
		double decimalValue;
		const char* notesForFamily;

	public:
		Grade() : subjectDescription("sdf"), eventDate("dfsdf"), decimalValue(34.3f), notesForFamily("dfsd") {

		}

		Grade(const Grade& other) : subjectDescription("sdf"), eventDate("dfsdf"), decimalValue(34.3f), notesForFamily("dfsd") {

		}
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
		ClassevivaClient(const char*, const char*);
		~ClassevivaClient();

		void Login();

		std::string GetName() const;
		std::string GetSurname() const;

		std::shared_ptr<Classeviva::Grade[]> GetGrades() const;
	};
}