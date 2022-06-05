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
		constexpr const char CARDS_PATH[] = "/cards";
		constexpr const char NOTICEBOARD_PATH[] = "/noticeboard";
	}

	/// <summary>
	/// Struct that represents a student's grade
	/// </summary>
	struct Grade {
		/// <summary>
		/// Name of the subject of the grade
		/// </summary>
		const std::string subjectDescription;
		/// <summary>
		/// The date on which the grade has been assigned
		/// </summary>
		const std::string eventDate;
		/// <summary>
		/// Value of the grade represented in numerical form. -1 if grade is not a number
		/// </summary>
		const double decimalValue;
		/// <summary>
		/// The way the grade is displayed in the register
		/// </summary>
		const std::string displayValue;
		/// <summary>
		/// Additional notes that describe the grade
		/// </summary>
		const std::string notes;
		/// <summary>
		/// School period of when the grade was set (semester, pentamester, etc.)
		/// </summary>
		const std::string periodDescription;
		/// <summary>
		/// Type of the grade (Written, Oral, Etc.)
		/// </summary>
		const std::string gradeType;
		/// <summary>
		/// Color of the grade
		/// </summary>
		const std::string color;

	public:

		Grade(
			const std::string& subjectDescription,
			const std::string& eventDate,
			const double decimalValue,
			const std::string& displayValue,
			const std::string& notes,
			const std::string& periodDesc,
			const std::string& gradeType,
			const std::string& color
		) :
			subjectDescription(subjectDescription),
			eventDate(eventDate),
			decimalValue(decimalValue),
			displayValue(displayValue),
			notes(notes),
			periodDescription(periodDesc),
			gradeType(gradeType),
			color(color)
		{
		}

	};

	struct Document {
		const int publicationId, contentId;
		const bool isDateValid, wasRead, hasChanged, hasAttachments, needsJoining, needsReply, needsSignature;

		const std::string publicationDate, status, title, category;

		Document(
			const int publicationId,
			const int contentId,
			const bool isDateValid,
			const bool wasRead,
			const bool hasChanged,
			const bool hasAttachments,
			const bool needsJoining,
			const bool needsReply,
			const bool needsSignature,

			const std::string& publicationDate,
			const std::string& status,
			const std::string& title,
			const std::string& category
		) 
			: 
			publicationId(publicationId),
			contentId(contentId),
			isDateValid(isDateValid),
			wasRead(wasRead),
			hasChanged(hasChanged),
			hasAttachments(hasAttachments),
			needsJoining(needsJoining),
			needsReply(needsReply),
			needsSignature(needsSignature),
			publicationDate(publicationDate),
			status(status),
			title(title),
			category(category)
		{}
	};

	struct StudentInfo {
		std::string birthDate;
		std::string miurSchoolCode;
		std::string miurDivisionCode;
		std::string fiscalCode;
		std::string schoolCode;
		std::string schoolName;
		std::string schoolDedication;
		std::string schoolCity;
		std::string schoolProvince;

	public:
		StudentInfo() {}
	private:
		StudentInfo(
			std::string& birthDate,
			std::string& miurSchoolCode,
			std::string& miurDivisionCode,
			std::string& fiscalCode,
			std::string& schoolCode,
			std::string& schoolName,
			std::string& schoolDedication,
			std::string& schoolCity,
			std::string& schoolProvince
		) : 
			birthDate(birthDate),
			miurSchoolCode(miurSchoolCode),
			miurDivisionCode(miurDivisionCode),
			fiscalCode(fiscalCode),
			schoolCode(schoolCode),
			schoolName(schoolName),
			schoolDedication(schoolDedication),
			schoolCity(schoolCity),
			schoolProvince(schoolProvince)
		{
		}

		StudentInfo(StudentInfo&& other) noexcept {
		}

		StudentInfo& operator=(StudentInfo&& other) noexcept {

			if (this != &other) {
				birthDate = std::move(other.birthDate);
				miurSchoolCode = std::move(other.miurSchoolCode);
				miurDivisionCode = std::move(other.miurDivisionCode);
				fiscalCode = std::move(other.fiscalCode);
				schoolCode = std::move(other.schoolCode);
				schoolName = std::move(other.schoolName);
				schoolDedication = std::move(other.schoolDedication);
				schoolCity = std::move(other.schoolCity);
				schoolProvince = std::move(other.schoolProvince);
			}

			return *this;
		}

		friend class ClassevivaClient;
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

		/// <summary>
		/// Get a list of grades in no particular order
		/// </summary>
		/// <param name="outGrades">The resulting vector of Grades that has to be filled</param>
		/// <returns>True if the operation succeeded</returns>
		bool GetGrades(std::vector<Grade>& outGrades) const;

		/// <summary>
		/// Get all kinds of information about a student
		/// </summary>
		/// <param name="outStudentInfo">The resulting StudentInfo struct</param>
		/// <returns>True if the operation succeeded</returns>
		bool GetStudentInfo(StudentInfo& outStudentInfo) const;

		bool GetDocuments(std::vector<Document>& outDocuments) const;
	};
}