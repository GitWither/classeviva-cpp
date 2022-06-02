#include "Classeviva.h"
#include <sstream>
#include "curl/curl.h"
#include <chrono>

#define ENSURE_SUCCESS_CODE() do {if (response->status != 200) return false;} while(0)

constexpr int SUCCESS_CODE = 200;

size_t WriteFunction(void* ptr, size_t size, size_t nmemb, std::string * data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> m_Start;

	Timer() : m_Start(std::chrono::high_resolution_clock::now()) {}
	~Timer() {
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_Start;
		std::cout << "Function took " << duration.count() * 1000 << " ms" << std::endl;
	}
};

namespace Classeviva {

	ClassevivaClient::ClassevivaClient(const std::string& email, const std::string& password) : m_Email(email), m_Password(password) {}

	double GetGradesAverage(const std::vector<Grade>& grades, const std::string& filter) {
		double sum = 0;
		double count = 0;

		for (const Grade& grade : grades) {
			if (grade.decimalValue == -1 || (filter.size() > 0 && grade.subjectDescription != filter)) continue;


			sum += grade.decimalValue;
			count++;
		}

		if (count == 0) return 0;

		return sum / count;
	}


	/// <summary>
	/// Logins to Classeviva and sets up the 
	/// </summary>
	bool ClassevivaClient::Login() {
		CURL* curl = curl_easy_init();
		CURLcode response;

		if (!curl) return false;

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, Constants::LOGIN_PATH);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");


		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "User-Agent: zorro/1.0");
		headers = curl_slist_append(headers, "Z-Dev-Apikey: +zorro+");
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string responseString;

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

		std::string data = "{\"uid\":\"" + m_Email + "\",\"pass\":\"" + m_Password + "\"}";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		response = curl_easy_perform(curl);


		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (responseCode == SUCCESS_CODE) {
			const nlohmann::json response_data = nlohmann::json::parse(responseString);

			m_Token = response_data["token"].get<std::string>();
			m_Name = response_data["firstName"].get<std::string>();
			m_Surname = response_data["lastName"].get<std::string>();

			std::string id = response_data["ident"].get<std::string>();
			id.erase(0, 1).erase(id.end() - 1, id.end());
			m_Id = id;

			return true;
		}

		curl_easy_cleanup(curl);

		return false;
	}

	bool ClassevivaClient::GetStudentInfo(StudentInfo& studentInfo) const {
		CURL* curl = curl_easy_init();
		CURLcode response;

		if (!curl) return false;

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_URL, (Constants::BASE_API_PATH + m_Id + Constants::CARDS_PATH).c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "User-Agent: zorro/1.0");
		headers = curl_slist_append(headers, "Z-Dev-Apikey: +zorro+");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, (std::string("Z-Auth-Token:") + m_Token).c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string responseString;

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

		response = curl_easy_perform(curl);

		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (responseCode == SUCCESS_CODE) {
			const nlohmann::json response_data = nlohmann::json::parse(responseString);

			auto& cardsDataUnparsed = response_data["cards"][0];

			StudentInfo info = {
				cardsDataUnparsed["birthDate"].get<std::string>(),
				cardsDataUnparsed["miurSchoolCode"].get<std::string>(),
				cardsDataUnparsed["miurDivisionCode"].get<std::string>(),
				cardsDataUnparsed["fiscalCode"].get<std::string>(),
				cardsDataUnparsed["schCode"].get<std::string>(),
				cardsDataUnparsed["schDedication"].get<std::string>(),
				cardsDataUnparsed["schName"].get<std::string>(), //schName and schDedication seem to be swapped
				cardsDataUnparsed["schCity"].get<std::string>(),
				cardsDataUnparsed["schProv"].get<std::string>()
			};

			studentInfo = std::move(info);

			return true;
		}

		curl_easy_cleanup(curl);

		return false;
	}

	/// <summary>
	/// Get a list of grades in no particular order
	/// </summary>
	/// <param name="outGrades">The list of grades to fill passed as a reference</param>
	bool ClassevivaClient::GetGrades(std::vector<Classeviva::Grade>& outGrades) const {
		CURL* curl = curl_easy_init();
		CURLcode response;

		if (!curl) return false;

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_URL, (Constants::BASE_API_PATH + m_Id + Constants::GRADES_PATH).c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "User-Agent: zorro/1.0");
		headers = curl_slist_append(headers, "Z-Dev-Apikey: +zorro+");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, (std::string("Z-Auth-Token:") + m_Token).c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string responseString;

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

		response = curl_easy_perform(curl);


		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (responseCode == SUCCESS_CODE) {
			const nlohmann::json response_data = nlohmann::json::parse(responseString);

			auto& gradesDataUnparsed = response_data["grades"];

			const size_t length = gradesDataUnparsed.size();
			auto grades = gradesDataUnparsed.items();

			outGrades.reserve(length);

			for (const auto& grade : grades) {
				const nlohmann::basic_json<>::value_type value = grade.value();

				//Figure out if the decimal value is null
				const nlohmann::basic_json<>::value_type decimalValueUnparsed = value["decimalValue"];
				const double decVal = decimalValueUnparsed.is_null() ? -1 : decimalValueUnparsed.get<double>();

				outGrades.emplace_back(
					value["subjectDesc"].get<std::string>(),
					value["evtDate"].get<std::string>(),
					decVal,
					value["displayValue"].get<std::string>(),
					value["notesForFamily"].get<std::string>(),
					value["periodDesc"].get<std::string>(),
					value["componentDesc"].get<std::string>(),
					value["color"].get<std::string>()
				);
			}

			return true;
		}

		curl_easy_cleanup(curl);

		return false;
	}

}