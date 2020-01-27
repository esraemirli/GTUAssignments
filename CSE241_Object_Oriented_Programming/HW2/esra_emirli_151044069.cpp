#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
using namespace std;
struct Course{
  int id,code,credit,total_hours;
  char name[25];
  vector<string> dates;
  vector<string>field;
};
struct Classroom{
  int id,capacity,student_inroom;
  char c_no[25];
};
class Student{
public:
  Student();
  Student(string nameValue,string surnameValue,int number,int levelValue);
  int enrollCourse(struct Course c); //return credit remained
  int disenroll(struct Course c); //return credit remained
  void enterClassroom(struct Classroom c); //student enters classroom
  void quitClassroom(struct Classroom c); //studetn quits classroom
  void output() const;
  string extractSchedule(); // combine dates and field as string.
  int getNo() const{
    return student_no;
  }
private:
  string name,surname;
  int student_no,level;
  int max_credit;
  int using_credit;
  vector<Course> courses;
};
////////
FILE* fp;
int convert(string s);
int findStudentIndex(vector<Student> s,int id);
bool isExist(vector<Student> s,int id);
bool isExist(vector<Course> c,int choise);
void showCourse(vector<Course> c);
////////
Student::Student() : name(NULL),surname(NULL),student_no(0),level(0){ }
Student::Student(string nameValue,string surnameValue,int number,int levelValue)
: name(nameValue),surname(surnameValue),student_no(number),level(levelValue){
  using_credit= 0;
  if(level==1)
    max_credit = 20;
  else if(level==2)
    max_credit= 21;
  else if(level==3)
    max_credit= 22;
  else
    max_credit= 23;
}
string Student::extractSchedule(){
  std::stringstream ss;
  ss<<name<<" "<<surname<<" :[";
  for(int i=0;i<courses.size();i++){
    ss<<courses[i].name<<" "<<courses[i].code<<" ";
    for(int j=0;j<courses[i].dates.size();j++){
      ss<<courses[i].dates[j]<<"_"<<courses[i].field[2*j+0]<<"-"<<courses[i].field[2*j+1]<<" ";
    }
  }
  ss<<"]";
  string schedule = ss.str();
  return schedule;
}
void Student::output() const{
  cout<<name<<" "<<surname<<": [";
  for(int i=0;i<courses.size();i++){
        cout<<courses[i].name<<"("<<courses[i].credit<<") ";
    }
  cout<<"] total credits:"<<using_credit<<" credit right:"<<max_credit<<endl;
}
void Student::enterClassroom(struct Classroom classroom){ //student enters classroom
  if(classroom.student_inroom < classroom.capacity)
    classroom.student_inroom ++;
}
void Student::quitClassroom(struct Classroom classroom){ //studetn quits classroom
  if(classroom.student_inroom > 0)
    classroom.student_inroom--;
}
int Student::enrollCourse(struct Course c){
  int error=1;
  //If you registered this lesson break..
  for(int i=0;i<courses.size();i++){
    if(courses[i].id == c.id){
      cout<<"BLOCK! Because of EXIST!"<<endl;
      error = 0;
      break;
    }
    //Credit limit is enough or not..
    if(max_credit - using_credit < c.credit){
      cout<<"BLOCK! Because of CREDIT!"<<endl;
      error = 0;
      break;
    }
    //Overlap or not..
    for(int j=0;j<courses[i].dates.size();j++){
      for(int k=0;k < c.dates.size(); k++){
        if(courses[i].dates[j] == c.dates[k]){
          int b,e,pb,pe;  //begin/end - parameter begin/end
          stringstream ss(courses[i].field[2*j+0]); //start time
          ss >> b;
          stringstream sas(c.field[2*k+0]); //start time
          sas >> pb;
          stringstream sss(courses[i].field[2*j+1]); //end time
          sss >> e;
          stringstream sass(c.field[2*k+1]); //end time
          sass >> pe;
          if(b > pb){
            if(b-pe <= -2){ // is overlap !!
              error = 0;
              if(e-b < 2 || pe-pb < 2)  //check lessons hour
                error = 1;
            }
          }
          else{
            if(pb - e <= -2){ // is overlap !!
              error = 0;
              if(e-b < 2 || pe-pb < 2)  //check lessons hour
                error = 1;
            }
          }
        }
      }
    }
  }
  if(error==1){
    courses.push_back(c);  // save the structure of course to courses vector..
    using_credit+=c.credit;
  }
  else
    cout<<"BLOCK! Because of OVERLAP!"<<endl;
  return (max_credit - using_credit);
}
int Student::disenroll(struct Course c){
  int i=0;
  //If you registered this lesson, delete..
  for(int i=0;i<courses.size();i++){
    if(courses[i].id == c.id){
      courses.erase(courses.begin() + i); //delete lesson
      using_credit-=c.credit;
      return (max_credit - using_credit);
    }
  }
  //You are not registered..
  return (max_credit - using_credit);
}
vector<string> separate(string const &str, const char mark) {
  int start;
  int end = 0;
  vector<string> output;
  //Basic string functions..
  while( (start = str.find_first_not_of(mark, end)) != string::npos){
    end = str.find(mark, start);
    output.push_back(str.substr(start, end - start));
  }
  return output;
}

int main(int argc, char const *argv[]) {
  char title[250];
  int i,num,rv;
  vector<Classroom> classroom;
  // Test Scenarios 1.
  vector<Course> courses;
  //Read files and save to structure vector..
  fp = fopen("courses_classrooms.txt","r");
  fscanf(fp,"%s",&title[0]);
  if(strcmp(title,"COURSES")==0){
    i=0;
    while( (rv = fscanf(fp,"%d",&num)) != EOF && rv==1 ){
      Course tempCourse;
      vector<string> temp,date,hour;
      tempCourse.id = num;
      fscanf(fp,"%s",&tempCourse.name[0]);
      fscanf(fp,"%d",&tempCourse.code);
      fscanf(fp,"%d",&tempCourse.credit);
      fscanf(fp,"%d",&tempCourse.total_hours);
      fscanf(fp,"%s",&title[0]);
      temp = separate(title,',');
      for(int j=0;j<temp.size();j++){
        date = separate(temp[j],'_');
        tempCourse.dates.push_back(date[0]);
        hour=separate(date[1],'-');
        tempCourse.field.push_back(hour[0]);
        tempCourse.field.push_back(hour[1]);
      }
      courses.push_back(tempCourse);
    }

  }
  fscanf(fp,"%s",&title[0]);
  if(strcmp(title,"CLASSROOMS")==0){
    i=0;
    while( (rv = fscanf(fp,"%d",&num)) != EOF && rv==1 ){
      Classroom tempClassroom;
      tempClassroom.id=num;
      fscanf(fp,"%s",&tempClassroom.c_no[0]);
      fscanf(fp,"%d",&tempClassroom.capacity);
      classroom.push_back(tempClassroom);
    }
  }
  fclose(fp);
  vector<Student> student;
  string name,surname;
  int no,id,level;

  string input;
  vector<string> temp;
  getline(cin,input);
  temp = separate(input,' '); //seperate schedule part by part days and fields..
  while( temp[0]!="exit"){
    //Enter student info and check all situation..
    if(temp[0] == "-ns"){
      no=convert(temp[3]);  //convert to int
      level=convert(temp[4]);
      name= temp[1];
      surname =temp[2];
      if(!isExist(student,no) && (level<5 && level>0))
        student.push_back( Student(name,surname,no,level) );
      else
        cout<<"BLOCK! Check INFO!"<<endl;
      getline(cin,input);
      temp = separate(input,' ');
    }
    //Print course names and ids on screen..
    else if(temp[0] == "-list"){
      showCourse(courses);
      getline(cin,input);
      temp = separate(input,' ');
    }
    // If conditions are appropriate, enroll lesson..
    else if(temp[0] == "-e"){
      no=convert(temp[1]);  // string to int
      id=convert(temp[2]);
      // check number of student and lesson id and level..
      if(isExist(student,no) && isExist(courses,id)){
        i=findStudentIndex(student,no);
        student[i].enrollCourse(courses[id-1]);  // return remained credit..
        cout<<"DONE!"<<endl;
      }
      else
        cout<<"BLOCK! Because of NO STUDENT OR LESSON!"<<endl;
      getline(cin,input);
      temp = separate(input,' ');
    }
    // Delete the record..
    else if(temp[0] == "-de"){
      no=convert(temp[1]);
      id=convert(temp[2]);
      if(isExist(student,no) && isExist(courses,id) ){
        i=findStudentIndex(student,no);
        student[i].disenroll(courses[id-1]);  // return remained credit..
        cout<<"DONE!"<<endl;
      }
      else
        cout<<"BLOCK! Because of NO STUDENT OR LESSON!"<<endl;
      getline(cin,input);
      temp = separate(input,' ');
    }
    //Write credit and using credit of all students..
    else if(temp[0] == "-o"){
      if(temp.size()==1){
        for(int i=0;i<student.size();i++)
        student[i].output();
      }
      //Write credit and using credit of given person..
      else if(temp.size()==2){
        no=convert(temp[1]);
        if(isExist(student,no)){
          i=findStudentIndex(student,no);
          student[i].output();
        }
      }
      //Write credit and using credit of all students in file..
      else if(temp.size()==3){
        string schedule;
        char str[temp[2].size()+1];
        strcpy(str,&temp[2][0]);  //change string to char*
        fp = fopen(str,"w");
        if(fp==NULL)
          exit(-1);
        for(int i=0;i<student.size();i++){
          schedule = student[i].extractSchedule();
          char str1[schedule.size()+1];
          strcpy(str1,&schedule[0]);  //change string to char*
          fprintf(fp,"%s\n",str1);
        }
        fclose(fp);
        cout<<"DONE! "<<str<<endl;
      }
      //Write credit and using credit of given person in file..
      else if(temp.size()==4){
        no=convert(temp[2]);
        if(isExist(student,no)){
          char str[temp[3].size()+1];
          strcpy(str,&temp[3][0]);  //change string to char*
          fp = fopen(str,"w");
          i=findStudentIndex(student,no);
          string schedule;
          schedule = student[i].extractSchedule();
          char str1[schedule.size()+1];
          strcpy(str1,&schedule[0]);  //change string to char*
          fprintf(fp,"%s\n",str1);

          fclose(fp);
          cout<<"DONE! "<<str<<endl;
        }
      }
      getline(cin,input);
      temp = separate(input,' ');
    }
    else{
      getline(cin,input);
      temp = separate(input,' ');
    }
  }
  return 0;
}
// string to int..
int convert(string s){
  int id;
  stringstream stoi(s);
  stoi >> id;
  return id;
}
// Find index who has this id..
int findStudentIndex(vector<Student> s,int id){
  for(int i=0;i<s.size();i++){
    if(s[i].getNo() == id)
    return i;
  }
}
//Check the person is student in school or not..
bool isExist(vector<Student> s,int id){
  for(int i=0;i<s.size();i++){
    if(s[i].getNo() == id)
    return true;
  }
  return false;
}
//Check the student has already been registered in lesson or not..
bool isExist(vector<Course> c,int choise){
  for(int i=0;i<c.size();i++){
    if(c[i].id == choise)
      return true;
  }
  return false;
}
//Print screen lessons and its id..
void showCourse(vector<Course> c){
  for(int i=0;i<c.size();i++){
    if(c[i].id !=0)
      cout<<"("<<c[i].id<<")"<<" "<<c[i].name<<" ";
  }
  cout<<endl;
}
