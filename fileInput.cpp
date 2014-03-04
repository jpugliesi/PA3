#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Room {
  string building_code;
  int room_number;
  int max_students;
};
struct Course {
  string prefix;
  int course_number;
  int num_minutes;
  int num_lectures;
};
struct Section_Constraints{
  int days[7]; // see explanation below
  int earliest_start_time;
  int latest_end_time;
};
struct Section {
  int section_id;
  int section_number;
  int num_students;
  Course course;
  Section_Constraints constraints;
};

struct InputFile{

	string theFile;
	int section1Index;
	int section2Index;
	int section3Index;

};

struct ScheduledRooms{

	Room *rooms;
	int size;

};

struct ScheduledCourses{

	Course *courses;
	int size;

};

struct ScheduledSectionConstraints{

	Section_Constraints *sec_cons;
	int size;

};

struct ScheduledSections{

	Section *sections;
	int size;

};

ScheduledRooms scheduledRooms;
ScheduledCourses scheduledCourses;
ScheduledSectionConstraints scheduledSectionConstraints;
ScheduledSections scheduledSections;

int getScheduleInformation();
int calculateNumberOfInputValues(string inputfile);
int isValidConstraint(string dayConstraint, int startTimeConstraint, int endTimeConstraint);
int findAndSetSectionAndConstraints(int id, string dayConstraint, int startTimeConstraint, int endTimeConstraint);
void setDayConstraints(int *dayArray, string availableDays);
void printRoomInfo();
void printCourseInfo();
void printSectionInfo();



int main(){

	if(getScheduleInformation()){

	}

	return 0;

}

/*******************************************************************************************************
********************************************************************************************************
********************************************************************************************************
************************************    Other Functions!    ********************************************
********************************************************************************************************
********************************************************************************************************
********************************************************************************************************/

int getScheduleInformation(){

	ifstream infile;
	stringstream ss;
	string line;
	string inputfile;

	//Get input file from the User - loop until existing file is found
	cout << "Enter the input file: ";

	do{

	cin >> inputfile;
	infile.open(inputfile); // open the file

	if (infile.fail()){

		cin.clear();
		cin.ignore(10000, '\n'); //clear inputs up to 10,000 characters, or first newline
		cout << "That file is non-existent. Enter another input file name: "; //prompt user again if the user screwed up
		continue; //jump to while statement
	}
	//a good character entered

	cin.ignore(100000, '\n'); //clear the stream
	break; //exit the loop

	} while(true);

	cout << "Initial File Calculations" << endl;
	if(!calculateNumberOfInputValues(inputfile)){
		return 0;
	}

	if(infile.is_open()){

		int i = 0;
		do{

			getline(infile, line);
			if(infile.eof()){
				cout << "Bad File. Incorrect Sections. Exiting." << endl;
			}
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			cout << "The Line: " << line << endl;
			ss >> scheduledRooms.rooms[i].building_code
			>> scheduledRooms.rooms[i].room_number
			>> scheduledRooms.rooms[i].max_students;

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return 0;
			}
			i++;
			ss.str("");
			ss.clear();

		}
		while(true); //repeat this do while loop for each section

		printRoomInfo();
		i = 0;
		do{

			getline(infile, line);
			if(infile.eof()){
				cout << "Bad File. Incorrect Sections. Exiting." << endl;
			}
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			ss >> scheduledSections.sections[i].section_id
			>> scheduledCourses.courses[i].prefix
			>> scheduledCourses.courses[i].course_number
			>> scheduledSections.sections[i].section_number
			>> scheduledCourses.courses[i].num_minutes
			>> scheduledCourses.courses[i].num_lectures
			>> scheduledSections.sections[i].num_students;

			scheduledSections.sections[i].course = scheduledCourses.courses[i];

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return 0;
			}
			i++;
			ss.str("");
			ss.clear();

		}
		while(true); //repeat this do while loop for each section

		i = 0;
		do{
			int id;
			string dayConstraint;
			int startTimeConstraint;
			int endTimeConstraint;

			getline(infile, line);
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			ss >> id
			>> dayConstraint
			>> startTimeConstraint
			>> endTimeConstraint;

			if(!findAndSetSectionAndConstraints(id, dayConstraint, startTimeConstraint, endTimeConstraint)){
				cout << "Mismatch of sections and section constraints. Exiting." << endl;
				return 0;
			}

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return 0;
			}
			i++;
			ss.str("");
			ss.clear();

		}
		while(!infile.eof()); //repeat this do while loop for each section

		printSectionInfo();

	}

	return 1;
}

int calculateNumberOfInputValues(string inputfile){

	stringstream ss;
	ifstream infile;
	infile.open(inputfile);

	string line;

	string foundRooms;
	string foundCourses;
	string foundIDs;
	string foundSectionConstraints;

	//calculate # of rooms provided in the input file
	int numRooms = 0;
	int numCourses = 0;
	int numIDs = 0;
	int numSectionConstraints = 0;

	if(infile.is_open()){
		do{
			int i = 0;
			string building_code;
			string building_number;
			string building_full;

			getline(infile, line);

			cout << "The Line: " << line << endl;
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			ss >> building_code >> building_number;
			cout << "Building Code: " << building_code << endl;

			building_full = building_code + building_number;

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return 0;
			}

			cout << "Found Rooms: " << foundRooms << endl;
			if(foundRooms.find(building_full) != string::npos){
				cout << "Multiple Room Definitions. Exiting." << endl;
				return 0;
			}
			foundRooms += building_full + " ";
			numRooms++;

			ss.str("");
			ss.clear();
		}
		while(true); //repeat this do while loop for each section

		cout << "Num Rooms: " << numRooms << endl;

		//scan section 2 for # of sections and courses

		do{
			string id;
			string section;
			string name;
			string prefix;
			string nameAndPrefix;

			getline(infile, line);
			cout << "The Line: " << line << endl;
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			ss >> id >> name >> prefix >> section;
			nameAndPrefix = name + prefix;

			cout << "The ID: " << id << endl;

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return 0;
			}

			cout << "Found IDs: " << foundIDs << endl;
			if(foundIDs.find(id, 0) != string::npos){
				cout << "Duplicate Sections. Exiting." << endl;
				return 0;
			}

			foundIDs += id + " ";
			numIDs++;

			if(foundCourses.find(nameAndPrefix) == string::npos){
				foundCourses += nameAndPrefix + " ";
				numCourses++;
			}

			ss.str("");
			ss.clear();
		}
		while(true); //repeat this do while loop for each section

		cout << "Num Section IDs: " << numIDs << endl;
		cout << "Num Courses: " << numCourses << endl;

		//section 3 for # of constraints

		do{
			string id;
			string dayConstraint;
			int startTimeConstraint;
			int endTimeConstraint;

			getline(infile, line);
			cout << "The Line: " << line << endl;

			if(infile.fail() && !infile.eof()){
				//bad times... no data
				cout << "***Bad File. Exiting.";
				return 0;
			}
			if(line.size()==0){
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			ss >> id >> dayConstraint >> startTimeConstraint >> endTimeConstraint;

			if(!isValidConstraint(dayConstraint, startTimeConstraint, endTimeConstraint)){
				//bad times...
				cout << "Bad Constraints. Exiting.";
				return 0;
			}

			cout << "Found Constraints: " << foundSectionConstraints << endl;
			if(foundSectionConstraints.find(id) != string::npos){
				cout << "Invalid Constraint Definitions. Exiting.";
				return 0;
			}
			foundSectionConstraints += id + " ";
			numSectionConstraints++;

			ss.str("");
			ss.clear();

			// if(infile.eof()){
			// 	break;
			// }
		}
		while(!infile.eof()); //repeat this do while loop for each section

		cout << "Num Section Constraints!: " << numSectionConstraints << endl;
	}

	if(numSectionConstraints != numIDs){
		cout << "Unequal number of courses and course time constraints. Exiting." << endl;
		return 0;
	}

	scheduledRooms.rooms = new Room[numRooms];
	scheduledRooms.size = numRooms;
	scheduledCourses.courses = new Course[numCourses];
	scheduledCourses.size = numCourses;
	scheduledSectionConstraints.sec_cons = new Section_Constraints[numSectionConstraints];
	scheduledSectionConstraints.size = numSectionConstraints;
	scheduledSections.sections = new Section[numIDs];
	scheduledSections.size = numIDs;

	infile.close();

	return 1;
}

int isValidConstraint(string dayConstraint, int startTimeConstraint, int endTimeConstraint){

	int valid = false;
	if(dayConstraint.find('M') || dayConstraint.find('T') ||
		dayConstraint.find('W') || dayConstraint.find('R') ||
		dayConstraint.find('F'))
	{
		valid = true;
	}
	// if(dayConstraint.find("MM") || dayConstraint.find("TT") ||
	// 	dayConstraint.find("WW") || dayConstraint.find("RR") ||
	// 	dayConstraint.find("FF"))
	// {
	// 	valid = false;
	// }
	if((startTimeConstraint < 0 || startTimeConstraint > 2400) || (endTimeConstraint < 0 || endTimeConstraint > 2400)){
		valid = false;
	}

	return valid;
}

void setDayConstraints(int *dayArray, string availableDays){

	for(int j = 0; j < 7; j++){
		dayArray[j] = 0;
	}
	cout << "Starting to set Day Constraints:" << endl;
	cout << "Available Days: " << availableDays << endl;

	if(availableDays.find('M') != string::npos){
		dayArray[0] = 1;
	}
	if(availableDays.find('T') != string::npos){
		dayArray[1] = 1;
	}
	if(availableDays.find('W') != string::npos){
		dayArray[2] = 1;
	}
	if(availableDays.find('R') != string::npos){
		dayArray[3] = 1;
	}
	if(availableDays.find('F') != string::npos){
		dayArray[4] = 1;
	}

	cout << "Printing final day array: " << endl;
	for(int i = 0; i < 7; i++){

		cout << dayArray[i] << " ";
	}

}

int findAndSetSectionAndConstraints(int id, string dayConstraint, int startTimeConstraint, int endTimeConstraint){

	Section* foundSection;
	int found = false;

	for(int i = 0; i < scheduledSections.size; i++){

		if(scheduledSections.sections[i].section_id == id){
			foundSection = &scheduledSections.sections[i];
			found = true;
		}
	}

	Section_Constraints newConstraint;
	newConstraint.earliest_start_time = startTimeConstraint;
	newConstraint.latest_end_time = endTimeConstraint;
	setDayConstraints(newConstraint.days, dayConstraint);

	foundSection->constraints = newConstraint;

	return found;

}

void printRoomInfo(){


	cout << "Printing Rooms: " << endl;
	for(int i = 0; i < scheduledRooms.size; i++){

		cout << "Building: " << scheduledRooms.rooms[i].building_code << " " << scheduledRooms.rooms[i].room_number << " " << scheduledRooms.rooms[i].max_students << endl;

	}

}

void printCourseInfo(){


	cout << "Printing Courses: " << endl;
	for(int i = 0; i < scheduledCourses.size; i++){

		cout << "Course: " << scheduledCourses.courses[i].prefix << " " << scheduledCourses.courses[i].course_number << " " << scheduledCourses.courses[i].num_minutes << " " << scheduledCourses.courses[i].num_lectures << endl;

	}

}

void printSectionInfo(){


	cout << "Printing Sections: " << endl;
	for(int i = 0; i < scheduledSections.size; i++){

		cout << "Section: " << scheduledSections.sections[i].section_id << " " << scheduledSections.sections[i].section_number << " " << scheduledSections.sections[i].num_students << endl;
		cout << "\t Course: " << scheduledSections.sections[i].course.prefix << " " << scheduledSections.sections[i].course.course_number << " " << scheduledSections.sections[i].course.num_minutes << " " << scheduledCourses.courses[i].num_lectures << endl;

		cout << "\t Constraints: ";
		for(int j = 0; j < 7; j++){
			cout << scheduledSections.sections[i].constraints.days[j] << " ";
		}
		cout << "\t\t" << scheduledSections.sections[i].constraints.earliest_start_time << " " << scheduledSections.sections[i].constraints.latest_end_time << endl;

	}

}