#include "Classeviva.h"

Classeviva::ClassevivaClient::ClassevivaClient(const char* email, const char* password) : m_Email(email), m_Password(password) {

}

Classeviva::ClassevivaClient::~ClassevivaClient() {

}

void Classeviva::ClassevivaClient::Login() {
	httplib::Client client(Classeviva::BASE_URL);
	httplib::Headers headers = {
		{"User-Agent", "zorro/1.0"},
		{"Z-Dev-Apikey", "+zorro+"},
		{"Content-Type", "application/json"}
	};
	client.set_default_headers(headers);

	nlohmann::json data;
	data[Classeviva::LOGIN_UID] = m_Email;
	data[Classeviva::LOGIN_PASS] = m_Password;

	httplib::Result response = client.Post(Classeviva::LOGIN_PATH, data.dump(), Classeviva::RAW_DATA_TYPE);

	if (response->status == 200) {
		nlohmann::json response_data = nlohmann::json::parse(response->body);

		m_Token = response_data["token"].get<std::string>();
		m_Name = response_data["firstName"].get<std::string>();
		m_Surname = response_data["lastName"].get<std::string>();

		std::string id = response_data["ident"].get<std::string>();
		id.erase(0, 1).erase(id.end() - 1, id.end());
		m_Id = id;
	}
}

std::shared_ptr<Classeviva::Grade[]> Classeviva::ClassevivaClient::GetGrades() const {
	httplib::Client client(Classeviva::BASE_URL);
	httplib::Headers headers = {
		{"User-Agent", "zorro/1.0"},
		{"Z-Dev-Apikey", "+zorro+"},
		{"Z-Auth-Token", m_Token},
		{"Content-Type", "application/json"}
	};
	client.set_default_headers(headers);

	std::string url = std::string(Classeviva::BASE_API_PATH) + m_Id + std::string(Classeviva::GRADES_PATH);
	httplib::Result response = client.Get(url.c_str());

	if (response->status == 200) {
		const nlohmann::json response_data = nlohmann::json::parse(response->body);

		const int length = response_data["grades"].size();

		std::unique_ptr<Grade[]> grades(new Grade[length]);

		grades[0] = Grade();

		return grades;
	}
	return nullptr;
}

std::string Classeviva::ClassevivaClient::GetName() const {
	return m_Name;
}

std::string Classeviva::ClassevivaClient::GetSurname() const {
	return m_Surname;
}
