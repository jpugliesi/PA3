/*

	General Advice:
		Schedule Big sections first in big rooms,
		then schedule the small rooms

*/

struct Room {
  string building_code[4];
  int room_number;
  int max_students;
};
struct Course {
  char prefix;
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

Room rooms[10];

string line;
int i = 0;
do{

	ifile.getline(line);
	if(ifile.fail()){
		//bad times... no data
	}
	if(line.size()==0){
		break;//what to do when there is a blank line
	}
	stringstream ss;
	ss << line;
	ss >> rooms[i].building_code
		>> rooms[i].room_number
		>> rooms[i].max_students;
	if(ss.fail()){
		//bad times...
	}
	i++;

}
while(true); //repeat this do while loop for each section