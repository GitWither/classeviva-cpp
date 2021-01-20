#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"
//temp
#include <iostream>

namespace Classeviva {

	constexpr const char* LOGIN_UID = "uid";
	constexpr const char* LOGIN_PASS = "pass";

	constexpr const char* RAW_DATA_TYPE = "raw";

	constexpr const char* BASE_URL = "https://web.spaggiari.eu";

	constexpr const char* LOGIN_PATH = "/rest/v1/auth/login";
	constexpr const char* GRADES_PATH = "/rest/v1/grades";

	class ClassevivaClient {
	private:
		const char* m_Password;
		const char* m_Email;

		std::string m_Name;
		std::string m_Surname;

		std::string m_Token;
		uint32_t m_Id = 0;

	public:
		ClassevivaClient(const char*, const char*);
		~ClassevivaClient();

		void Login();

		const char* GetName() const;
		const char* GetSurname() const;

		void GetGrades() const;
	};
}