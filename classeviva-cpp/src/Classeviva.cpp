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
	}
}


const char* Classeviva::ClassevivaClient::GetName() const {
	return m_Name.c_str();
}

const char* Classeviva::ClassevivaClient::GetSurname() const {
	return m_Surname.c_str();
}
