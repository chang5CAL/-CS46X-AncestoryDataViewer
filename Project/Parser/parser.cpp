
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <algorithm>


using namespace std;

struct Person {
	string id;
	string name;
	string sprouse;
	string father;
	string mother;
};


void print_info(Person *p){
	int i = 0;
	ofstream output("output.txt");
	if(output.is_open()){
		while(!p[i].id.empty()){
			output << p[i].id << ", " << p[i].name << ", " << p[i].sprouse << ", " << p[i].father << ", " << p[i].mother << "\n";
			i++;
		}
	}
	output.close();
}

int search_by_id(Person *p, string str){
	int i = 0;
	while(!p[i].id.empty()){
		if(p[i].id == str){
			//cout << "husb  " << i << endl;
			return i;
		}
		i++;
	}
	return 0;
}


int main(){
	Person p[100];
	int i, j, husb, wife, chil;
	string str;
	string str2 ("0 @I");
	string str3 ("1 NAME");
	string str4 ("1 HUSB");
	string str5 ("1 WIFE");
	string str6 ("1 CHIL");

	ifstream input("s.ged");

	i = 0;
	j = 0;

	if(!input.fail()){

		while(getline(input, str)){
			if(str.find(str2) != string::npos){
				str.erase(6,7);
				str.erase(0,3);
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				p[j].id = str;
				//cout << j << endl;
				j++;
			}

			if(str.find(str3) != string::npos){
				str.erase(0,7);
				str.erase(remove(str.begin(), str.end(), '/'), str.end());
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				str.erase(str.length()-1);
				p[i].name = str;
				//cout << i << endl;
				i++;
			}

			if(str.find(str4) != string::npos){
				str.erase(11,2);
				str.erase(0,8);
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				husb = search_by_id(p, str);
				//p[i].name = str;

			}

			if(str.find(str5) != string::npos){
				str.erase(11,2);
				str.erase(0,8);
				wife = search_by_id(p, str);
				p[husb].sprouse = p[wife].name;
				p[wife].sprouse = p[husb].name;
				//p[i].name = str;

			}

			if(str.find(str6) != string::npos){
				str.erase(11,2);
				str.erase(0,8);
				chil = search_by_id(p, str);
				p[chil].mother = p[wife].name;
				p[chil].father = p[husb].name;
				//p[i].name = str;
			}
		}
	}
	input.close();

	i = 0;
	print_info(p);
	return 0;
}