#include "Classeviva.h"

#define ENSURE_SUCCESS_CODE() do {if (response->status != 200) return false;} while(0)


Classeviva::Grade::Grade(
	const std::string& subjectDescription, 
	const std::string& eventDate, 
	const double decimalValue, 
	const std::string& notesForFamily, 
	const std::string& periodDesc, 
	const std::string& gradeType) :

	subjectDescription(subjectDescription), 
	eventDate(eventDate), 
	decimalValue(decimalValue), 
	notesForFamily(notesForFamily),
	periodDescription(periodDesc), 
	gradeType(gradeType) {
}

Classeviva::Grade::~Grade() {

}

Classeviva::ClassevivaClient::ClassevivaClient(std::string& email, std::string& password) : m_Email(email), m_Password(password) {

}

Classeviva::ClassevivaClient::~ClassevivaClient() {

}

/// <summary>
/// Logins to Classeviva and sets up the 
/// </summary>
bool Classeviva::ClassevivaClient::Login() {
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

	if (httplib::Result response = client.Post(Classeviva::LOGIN_PATH, data.dump(), "application/json")) {
		ENSURE_SUCCESS_CODE();

		const nlohmann::json response_data = nlohmann::json::parse(response->body);

		m_Token = response_data["token"].get<std::string>();
		m_Name = response_data["firstName"].get<std::string>();
		m_Surname = response_data["lastName"].get<std::string>();

		std::string id = response_data["ident"].get<std::string>();
		id.erase(0, 1).erase(id.end() - 1, id.end());
		m_Id = id;

		return true;
	}

	return false;
}

/// <summary>
/// Get a list of grades in no particular order
/// </summary>
/// <param name="outGrades">The list of grades to fill passed as a reference</param>
bool Classeviva::ClassevivaClient::GetGrades(std::vector<Classeviva::Grade>& outGrades) const {
	httplib::Client client(Classeviva::BASE_URL);
	httplib::Headers headers = {
		{"User-Agent", "zorro/1.0"},
		{"Z-Dev-Apikey", "+zorro+"},
		{"Z-Auth-Token", m_Token},
		{"Content-Type", "application/json"}
	};
	client.set_default_headers(headers);

	std::string url = std::string(Classeviva::BASE_API_PATH) + m_Id + std::string(Classeviva::GRADES_PATH);

	if (httplib::Result response = client.Get(url.c_str())) {
		ENSURE_SUCCESS_CODE();

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

		return true;
	}

	return false;
}

/// <summary>
/// A function that returns the student's name
/// </summary>
/// <returns>A string containing the student's name</returns>
inline std::string Classeviva::ClassevivaClient::GetName() const {
	return m_Name;
}

/// <summary>
/// A function that returns the student's surname
/// </summary>
/// <returns>A string containing the student's surname</returns>
inline std::string Classeviva::ClassevivaClient::GetSurname() const {
	return m_Surname;
}
