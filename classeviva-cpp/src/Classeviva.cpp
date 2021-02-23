#include "Classeviva.h"


Classeviva::Grade::Grade(
	std::string& subjectDescription, 
	std::string& eventDate, 
	double& decimalValue, 
	std::string& notesForFamily, 
	std::string& periodDesc, 
	std::string& gradeType) :

	subjectDescription(subjectDescription), 
	eventDate(eventDate), 
	decimalValue(decimalValue), 
	notesForFamily(notesForFamily),
	periodDescription(periodDesc), 
	gradeType(gradeType) {
}

Classeviva::Grade::~Grade() {

}

Classeviva::ClassevivaClient::ClassevivaClient(const char* email, const char* password) : m_Email(email), m_Password(password) {

}

Classeviva::ClassevivaClient::~ClassevivaClient() {

}

/// <summary>
/// Logins to Classeviva and sets up the 
/// </summary>
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

/// <summary>
/// Get a list of grades in no particular order
/// </summary>
/// <param name="outGrades">The list of grades to fill passed as a reference</param>
void Classeviva::ClassevivaClient::GetGrades(std::vector<Classeviva::Grade>& outGrades) const {
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
		
		auto& gradesDataUnparsed = response_data["grades"];

		const int length = gradesDataUnparsed.size();
		auto grades = gradesDataUnparsed.items();

		outGrades.reserve(length);

		for (const auto& grade : grades) {
			nlohmann::basic_json<>::value_type value = grade.value();

			//Figure out if the decimal value is null
			nlohmann::basic_json<>::value_type decimalValueUnparsed = value["decimalValue"];
			double decVal = decimalValueUnparsed.is_null() ? -1 : decimalValueUnparsed.get<double>();

			outGrades.emplace_back(
				value["subjectDesc"].get<std::string>(),
				value["evtDate"].get<std::string>(),
				decVal,
				value["notesForFamily"].get<std::string>(),
				value["periodDesc"].get<std::string>(),
				value["componentDesc"].get<std::string>()
			);
		}

	}
}

/// <summary>
/// A function that returns the student's name
/// </summary>
/// <returns>A string containing the student's name</returns>
std::string Classeviva::ClassevivaClient::GetName() const {
	return m_Name;
}

/// <summary>
/// A function that returns the student's surname
/// </summary>
/// <returns>A string containing the student's surname</returns>
std::string Classeviva::ClassevivaClient::GetSurname() const {
	return m_Surname;
}
