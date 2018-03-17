#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <vector>


using namespace std;

struct Person {
  string id;
  string name;
  string spouse;
  string father;
  string mother;
  vector<string> child;
};

// prints struct Person information into output.txt
void print_info(Person *p){
  int i = 0;
  int z = 0;
  ofstream output("output.txt");
  if(output.is_open()){
    while(!p[i].id.empty()){
      output<< "id: " << p[i].id << ", name: " << p[i].name << ", spouse: " << p[i].spouse << ", father: " << p[i].father << ", mother: " << p[i].mother;
      for(z = 0; z<p[i].child.size(); ++z){
        output << ", child: " << p[i].child[z];
        //z++;
      }
      output << "\n";
      i++;
    }
  }
  output.close();
}

// print vector string ancestor. ancestor contains list of ID of ancestors of a given person
void print_ancestor(vector<string> ancestor){
  int i;
  cout << "size: " << ancestor.size() << endl;
  for(i = 0; i < ancestor.size(); i++){
    cout << ancestor[i] << endl;
  }
}

// returns the index of person in struct p with the given id
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

// returns the index of person in struct p with the given name(the first one found)
int search_by_name(Person *p, string str){
  int i = 0;
  while(!p[i].id.empty()){
    if(p[i].name == str){
      //cout << "husb  " << i << endl;
      return i;
    }
    i++;
  }
  return 0;
}

// construct list of ancestors of a given person.(Recursively)
void find_ancestors(Person *p, Person current_person, vector<string> &ancestor){
  int i;
  Person mother, father, spouse;
  if(!current_person.father.empty()){ //check if the person have parents
    mother = p[search_by_id(p, current_person.mother)]; // find parents in struct
    father = p[search_by_id(p, current_person.father)];
    ancestor.push_back(father.id); // add parent's id to ancestor vector
    ancestor.push_back(mother.id);
    if(!father.father.empty()){ // check if parents have parents
      find_ancestors(p, father, ancestor);
    }
    if(!mother.father.empty()){
      find_ancestors(p, mother, ancestor);
    }
  }
  if(!current_person.spouse.empty()){ // check if person have spouse
    spouse = p[search_by_id(p, current_person.spouse)]; // find spouse
    if(!spouse.father.empty()){ // check if spouse has parent
      mother = p[search_by_id(p, spouse.mother)]; // find parents
      father = p[search_by_id(p, spouse.father)];
      ancestor.push_back(father.id); // add parents to ancestor vector
      ancestor.push_back(mother.id);
      if(!father.father.empty()){ // check if parents have parents
        find_ancestors(p, father, ancestor);
      }
      if(!mother.father.empty()){
        find_ancestors(p, mother, ancestor);
      }
    }
  }
}

// find noot node and return. The other root node is this person's spouse.
Person find_root_node(Person *p){
  int i = 0;
  while(!p[i].id.empty()){
    if(p[i].father.empty()){ // has no information about their parents
      if(p[search_by_id(p, p[i].spouse)].father.empty()){ // spouse also has no information about their parents
        return p[i];
        //cout << i << endl;
      }
    }
    //cout << i << "!!!!!!!!"<< endl;
    i++;
  }
  return p[99];
}


// find common ancestor in the two ancestor vectors
void find_common_ancestor(vector<string> ancestor1, vector<string> ancestor2){
  int i, j;
  for(i = 0; i < ancestor1.size(); i++){
    for(j = 0; j < ancestor2.size(); j++){
      if(ancestor1[i] == ancestor2[j]){
        cout << "common ancestor: " << ancestor1[i] << endl;
        return;
      }
    }
  }
}

int main(){
  Person p[100];
  Person root, root_spouse;
  int i, j, husb, wife, chil, z;
  string str;
  vector<string> ancestor1, ancestor2;
  string str2 ("0 @I");
  string str3 ("1 NAME");
  string str4 ("1 HUSB");
  string str5 ("1 WIFE");
  string str6 ("1 CHIL");

  ifstream input("s.ged");

  i = 0;
  j = 0;
  z = 0;

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
        p[husb].spouse = p[wife].id;
        p[wife].spouse = p[husb].id;
        //p[i].name = str;

      }

      if(str.find(str6) != string::npos){
        str.erase(11,2);
        str.erase(0,8);
        chil = search_by_id(p, str);
        p[chil].mother = p[wife].id;
        p[chil].father = p[husb].id;
        p[wife].child.push_back(p[chil].id);
        p[husb].child.push_back(p[chil].id);
      }
    }
  }
  input.close();

  i = 0;
  print_info(p);
  root = find_root_node(p);
  cout << root.id << " name: " <<root.name << endl;
  root_spouse = p[search_by_id(p, root.spouse)];
  cout << root_spouse.id << " name: " <<root_spouse.name << endl;
  find_ancestors(p, p[18], ancestor1);
  find_ancestors(p, p[6], ancestor2);
  print_ancestor(ancestor1);
  print_ancestor(ancestor2);
  find_common_ancestor(ancestor1, ancestor2);
  return 0;
}
