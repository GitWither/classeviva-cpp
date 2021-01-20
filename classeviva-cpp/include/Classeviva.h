#pragma once
#include "httplib.h"

namespace Classeviva {
	class ClassevivaClient {
	private:
		const char* m_Password;
		const char* m_Email;

		char* m_Name = nullptr;
		char* m_Surname = nullptr;
	public:
		ClassevivaClient(const char*, const char*);
		~ClassevivaClient();

		void Login() const;

		char* GetName() const;
		char* GetSurname() const;
	};
}