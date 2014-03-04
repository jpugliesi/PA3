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

	string *foundRooms;
	string *foundCourses;
	string *foundSectionConstraints;
	string *foundSections;

	int foundRoomsLength;
	int foundCoursesLength;
	int foundSectionConstraintsLength;
	int foundSectionsLength;

	int blankLines;
	string *theFile;
	int section1Index;
	int section2Index;
	int section3Index;
	int endline;

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

InputFile theInputFile;

ScheduledRooms scheduledRooms;
ScheduledCourses scheduledCourses;
ScheduledSectionConstraints scheduledSectionConstraints;
ScheduledSections scheduledSections;

int getScheduleInformation();
int getFile(ifstream& infile);
int readSectionOne(ifstream& infile, int line_num, int file_len);
int readSectionTwo(ifstream& infile, int line_num, int file_len);
int readSectionThree(ifstream& infile, int line_num, int file_len);
void printStringArray(string *stringArray, int len);
int searchStringArray(string *stringArray, int len, string searchValue);
int isValidConstraint(string dayConstraint, int startTimeConstraint, int endTimeConstraint);
void printRoomInfo();
void printSectionInfo();
void printCourseInfo();
void printFile(int len);
int parseDataSectionOne();
int parseDataSectionTwo();
int parseDataSectionThree();
void setDayConstraints(int *dayArray, string availableDays);
int findAndSetSectionAndConstraints(int id, string dayConstraint, int startTimeConstraint, int endTimeConstraint);

// int calculateNumberOfInputValues(string inputfile);
// int isValidConstraint(string dayConstraint, int startTimeConstraint, int endTimeConstraint);
// int findAndSetSectionAndConstraints(int id, string dayConstraint, int startTimeConstraint, int endTimeConstraint);
// void setDayConstraints(int *dayArray, string availableDays);
// void printRoomInfo();
// void printCourseInfo();
// void printSectionInfo();



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
	string inputFileName;

	//Get input file from the User - loop until existing file is found
	cout << "Enter the input file: ";

	do{

	cin >> inputFileName;
	infile.open(inputFileName); // open the file

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

	if(!getFile(infile)){
		return 0;
	}

	return 1;
}

int getFile(ifstream& infile){

	theInputFile.foundRooms = new string[100];
	theInputFile.foundRoomsLength = 100;
	theInputFile.foundCourses = new string[100];
	theInputFile.foundCoursesLength = 100;
	theInputFile.foundSectionConstraints = new string[100];
	theInputFile.foundSectionConstraintsLength = 100;
	theInputFile.foundSections = new string[100];
	theInputFile.foundSectionsLength = 100;

	int valid = true;
	stringstream ss;
	theInputFile.theFile = new string[100];
	int line_num = 0;
	int file_len = 100;

	theInputFile.section1Index = line_num;
	line_num = readSectionOne(infile, line_num, file_len);
	if(line_num == -1){
		cout << "The Error is in Section One. Exiting" << endl;
		return 0;
	}
	theInputFile.section2Index = line_num;
	line_num = readSectionTwo(infile, line_num, file_len);
	if(line_num == -1){
		cout << "The Error is in Section Two. Exiting" << endl;
		return 0;
	}
	theInputFile.section3Index = line_num;
	line_num = readSectionThree(infile, line_num, file_len);
	if(line_num == -1){
		cout << "The Error is in Section Three. Exiting" << endl;
		return 0;
	}
	theInputFile.endline = line_num;

	if(!parseDataSectionOne()){
		cout << "The Error is in Data Section 1" << endl;
		return 0;
	}
	if(!parseDataSectionTwo()){
		cout << "The Error is in Data Section 2" << endl;
		return 0;
	}
	if(!parseDataSectionThree()){
		cout << "The Error is in Data Section 3" << endl;
		return 0;
	}

	return valid;
}

int readSectionOne(ifstream& infile, int line_num, int file_len){

	stringstream ss;
	string line;
	int i = 0;

	int numRooms = 0;

	if(infile.is_open()){
		do{
			string building_code;
			string building_number;
			string building_full;

			getline(infile, line);
			theInputFile.theFile[line_num] = "";
			// cout << "The Line: " << line << endl;
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return -1;
			}
			if(line.size()==0){
				theInputFile.blankLines++;
				if(theInputFile.blankLines > 1){
					cout << "Too Many Blank lines." << endl;
					return -1;
				}
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			theInputFile.theFile[line_num] = line;
			ss >> building_code >> building_number;
			// cout << "Building Code: " << building_code << endl;

			building_full = building_code + building_number;

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return -1;
			}

			if(searchStringArray(theInputFile.foundRooms, numRooms, building_full) != -1){
				cout << "Repeat Room Definitions. Exiting." << endl;
				return -1;
			}
			theInputFile.foundRooms[i] = building_full;
			numRooms++;
			i++;

			line_num++;
			ss.str("");
			ss.clear();
		}
		while(true); //repeat this do while loop for each section
	}

	// cout << "Found Rooms: "  << endl;
	// printStringArray(theInputFile.foundRooms, numRooms);

	// cout << "Num Rooms: " << numRooms << endl;
	scheduledRooms.rooms = new Room[numRooms];
	scheduledRooms.size = numRooms;

	return line_num;

}

int readSectionTwo(ifstream& infile, int line_num, int file_len){
	
	stringstream ss;
	string line;
	int numCourses = 0;
	int numIDs = 0;
	int start = 0;

	//scan section 2 for # of sections and courses
	if(infile.is_open()){
		do{
			string id;
			string section;
			string name;
			string prefix;
			string nameAndPrefix;

			getline(infile, line);
			theInputFile.theFile[line_num] = "";
			// cout << "The Line: " << line << endl;
			if(infile.fail()){
				//bad times... no data
				cout << "Bad File. Exiting.";
				return -1;
			}
			if(line.size()==0){
				theInputFile.blankLines++;
				if(theInputFile.blankLines > 2){
					cout << "Too Many Blank Lines." << endl;
					return -1;
				}
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			theInputFile.theFile[line_num] = line;
			ss >> id >> name >> prefix >> section;
			nameAndPrefix = name + prefix;

			// cout << "The ID: " << id << endl;

			if(ss.fail()){
				//bad times...
				cout << "Bad File. Exiting.";
				return -1;
			}

			if(searchStringArray(theInputFile.foundSections, numIDs, id) != -1){
				cout << "Duplicate Section IDs." << endl;
				return -1;
			}

			theInputFile.foundSections[numIDs] = id;
			numIDs++;

			if(searchStringArray(theInputFile.foundCourses, numCourses, nameAndPrefix) == -1){
				theInputFile.foundCourses[numCourses] = nameAndPrefix;
				numCourses++;

			}

			line_num++;
			ss.str("");
			ss.clear();
		}
		while(true); //repeat this do while loop for each section
	}

	// cout << "Found IDs: " << endl;
	// printStringArray(theInputFile.foundSections, numIDs);

	// cout << "Num Section IDs: " << numIDs << endl;
	// cout << "Num Courses: " << numCourses << endl;
	scheduledCourses.courses = new Course[numCourses];
	scheduledCourses.size = numCourses;
	scheduledSections.sections = new Section[numIDs];
	scheduledSections.size = numIDs;

	return line_num;

}

int readSectionThree(ifstream& infile, int line_num, int file_len){

	stringstream ss;
	string line;
	int numSectionConstraints = 0;

	do{
			string id;
			string dayConstraint;
			int startTimeConstraint;
			int endTimeConstraint;

			getline(infile, line);
			theInputFile.theFile[line_num] = "";
			// cout << "The Line: " << line << endl;

			if(infile.fail() && !infile.eof()){
				//bad times... no data
				cout << "***Bad File. Exiting.";
				return -1;
			}
			if(line.size()==0){
				theInputFile.blankLines++;
				if(theInputFile.blankLines > 2){
					cout << "Too Many Blank Lines." << endl;
					return -1;
				}
				break;//what to do when there is a blank line
			}
			if(line.find("--",0,2) != string::npos){
				// cout << "Skipped!";
				continue;
			}

			ss << line;
			theInputFile.theFile[line_num] = line;
			ss >> id >> dayConstraint >> startTimeConstraint >> endTimeConstraint;

			if(searchStringArray(theInputFile.foundSections, theInputFile.foundSectionsLength, id) == -1){
				cout << "Mismatch of Sections and Constraints" << endl;
				return -1;
			}

			if(isValidConstraint(dayConstraint, startTimeConstraint, endTimeConstraint) == false){
				//bad times...
				cout << "Bad Constraints. Exiting." << endl;
				return -1;
			}

			if(searchStringArray(theInputFile.foundSectionConstraints, numSectionConstraints, id) != -1){
				cout << "Duplicate Constraint Definitions. Exiting." << endl;
				return -1;
			}
			theInputFile.foundSectionConstraints[numSectionConstraints] = id;
			numSectionConstraints++;
			line_num++;
			ss.str("");
			ss.clear();

			// if(infile.eof()){
			// 	break;
			// }
		}
		while(!infile.eof()); //repeat this do while loop for each section

	// cout << "Found Constraints: " << endl;
	// printStringArray(theInputFile.foundSectionConstraints, numSectionConstraints);

	// cout << "Num Section Constraints!: " << numSectionConstraints << endl;
	scheduledSectionConstraints.sec_cons = new Section_Constraints[numSectionConstraints];
	scheduledSectionConstraints.size = numSectionConstraints;

	return line_num;		
	
}

void printStringArray(string *stringArray, int len){

	for(int i = 0; i < len; i++){

		cout << stringArray[i] << endl;

	}

}

int searchStringArray(string *stringArray, int len, string searchValue){

	int foundIndex = -1;

	for(int i = 0; i < len; i++){

		if(stringArray[i] == searchValue){
			foundIndex = i;
			return foundIndex;
		}
	}

	return foundIndex;

}

int isValidConstraint(string dayConstraint, int startTimeConstraint, int endTimeConstraint){

	int valid = false;
	if(dayConstraint.find('M') || dayConstraint.find('T') ||
		dayConstraint.find('W') || dayConstraint.find('R'))
	{
		valid = true;
	}
	if((startTimeConstraint < 800 || startTimeConstraint > 1700) || (endTimeConstraint < 800 || endTimeConstraint > 1700)){
		valid = false;
	}
	if(dayConstraint.find('F')){
		cout << "Cannot set Friday classes. Change cosntains." << endl;
	}

	return valid;
}

int parseDataSectionOne(){

	cout << endl << "Parsing Section One" << endl;

	int i = theInputFile.section1Index;
	string line;
	stringstream ss;

	do{
		line = theInputFile.theFile[i];

		ss << line;
		ss >> scheduledRooms.rooms[i].building_code
		>> scheduledRooms.rooms[i].room_number
		>> scheduledRooms.rooms[i].max_students;

		if(ss.fail()){
			//bad times...
			cout << "Bad File. Exiting." << endl;
			return 0;
		}
		i++;
		ss.str("");
		ss.clear();

	}
	while(i < theInputFile.section2Index); //repeat this do while loop for each section

	printRoomInfo();
	return 1;

}

int parseDataSectionTwo(){

	cout << endl << "Parsing Section Two" << endl;

	int i = theInputFile.section2Index;
	int counter = 0;
	string line;
	stringstream ss;

	do{
		line = theInputFile.theFile[i];

		ss << line;
			ss >> scheduledSections.sections[counter].section_id
			>> scheduledCourses.courses[counter].prefix
			>> scheduledCourses.courses[counter].course_number
			>> scheduledSections.sections[counter].section_number
			>> scheduledCourses.courses[counter].num_minutes
			>> scheduledCourses.courses[counter].num_lectures
			>> scheduledSections.sections[counter].num_students;

			scheduledSections.sections[counter].course = scheduledCourses.courses[counter];

		if(ss.fail()){
			//bad times...
			cout << "Bad File. Exiting.";
			return 0;
		}
		i++;
		counter++;
		ss.str("");
		ss.clear();

	}
	while(i < theInputFile.section3Index); //repeat this do while loop for each section

	printCourseInfo();
	return 1;

}

int parseDataSectionThree(){

	cout << endl << "Parsing Section Three" << endl;

	int i = theInputFile.section3Index;
	string line;
	int id;
	string dayConstraint;
	int startTimeConstraint;
	int endTimeConstraint;
	stringstream ss;

	do{
		line = theInputFile.theFile[i];

		ss << line;
		ss >> id
		>> dayConstraint
		>> startTimeConstraint
		>> endTimeConstraint;

		if(!findAndSetSectionAndConstraints(id, dayConstraint, startTimeConstraint, endTimeConstraint)){
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
	while(i < theInputFile.endline); //repeat this do while loop for each section

	printSectionInfo();
	return 1;

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
	if(found == false){
		cout << "Mismatch of sections and section constraints. Exiting." << endl;
		return false;
	} else if(startTimeConstraint + foundSection->course.num_minutes > 1700){
		found = false;
		cout << "Classes must end by 1700. A class is exceeding this time constraitnt. Check the file and make changes." << endl;
		return found;
	} else if(startTimeConstraint + foundSection->course.num_minutes > endTimeConstraint){
		found = false;
		cout << "A class is too long for its given constraints. Check the file and make changes." << endl;
		return found;
	}
	Section_Constraints newConstraint;
	newConstraint.earliest_start_time = startTimeConstraint;
	newConstraint.latest_end_time = endTimeConstraint;
	setDayConstraints(newConstraint.days, dayConstraint);

	foundSection->constraints = newConstraint;

	return found;

}

void setDayConstraints(int *dayArray, string availableDays){

	for(int j = 0; j < 7; j++){
		dayArray[j] = 0;
	}
	// cout << "Starting to set Day Constraints:" << endl;
	// cout << "Available Days: " << availableDays << endl;

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
		cout << "Cannot set Friday classes. Ignoring." << endl;
	}

	// cout << "Printing final day array: " << endl;
	// for(int i = 0; i < 7; i++){

	// 	cout << dayArray[i] << " ";
	// }
	// cout << endl;

}

void printRoomInfo(){


	cout << "Printing Rooms: " << endl;
	for(int i = 0; i < scheduledRooms.size; i++){

		cout << "Building: " << scheduledRooms.rooms[i].building_code << " " << scheduledRooms.rooms[i].room_number << " " << scheduledRooms.rooms[i].max_students << endl;

	}

}

void printCourseInfo(){


	cout << "Printing Courses: " << endl;
	for(int i = 0; i < scheduledSections.size; i++){

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

void printFile(int len){
	cout << endl;
	for(int i = 0; i < len; i++){
		cout << theInputFile.theFile[i] << endl;
	}
}