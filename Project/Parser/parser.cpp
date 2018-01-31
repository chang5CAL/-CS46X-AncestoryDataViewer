#include <iostream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <fstream>
//Really, this should all be in a class.

using namespace std;


unordered_set<node> nodes;
unordered_set<families> families;

struct node{
    //So we're just gonna treat the node as a name and
    //their direct link. No siblings.
    string name;
    string id;
    //Let's start simply:
    node* spouse; //FAMS
    node** children; //FAM
    node* parent1; //FAMC
    node* parent2; //FAMC
};

struct family{
    //So we're just gonna treat the node as a name and
    //their direct link. No siblings.
    string id;
    string* children; //FAM
    string parent1; //FAMC
    string parent2; //FAMC
    int numChildren;
    //This should store their IDs, the nodes store their location.
};

//So GEDCOM files work as basically "subtrees" and "nodes", for our purposes.
//Soooooooooooo,
//Let's start by finding every GEDCOM file that does NOT have a FAMC.



void linkFamily(family f){
    for(int i=0;i<f->numChildren;++i){
        nodes[f->parent1]->child[i] = nodes[f->children[i]];
        nodes[f->parent2]->child[i] = nodes[f->children[i]];
        //Take the parent node's child parameter and attach the
        //child's node to it.
        nodes[f->children[i]]->parent1 = nodes[f->parent1];
        nodes[f->children[i]]->parent2 = nodes[f->parent2];
        //And attach the parents as the child's parents.
    }
    //This should work fine. Now to just read the file.
}


int main()
{
    string line;
    //This should actually be a constructor.
    node* n = NULL;
    families = new family[100]; //Temp, just set it to 100

    //This should go in a different function, like read or something. But then it should wipe it all.
    ifstream file("temp.ged"); //Should allow manual input but this will suffice for now.
    if(file.is_open()){
        while(file >> line){
                //Note to self: This should grab a word I think
            //Read the file
            if(line == "NODE"){
                //If it's a node
                //Keep going for a name...
                n = new node;

                bool nameFound = false;
                bool idFound = false;

                while(file >> line){
                    if(line == "ID"){
                        file >> line;
                        n->id = line;

                        idFound = true;
                    }
                    else if (line == "NAME"){
                        file >> line;
                        n->name = line;

                        nameFound = true;
                    }

                    if(idFound && nameFound){
                        break;
                        //n has a name and id, so we're good!
                    }
                }
                nodes[idTemp] = n;
                n = NULL;
            }
            else if (line == "FAM"){
                //If it's a family, make the family.
                family* fTemp = new family;
                fTemp->numChildren = 0;
                bool idFound = false;
                bool parent1Found = false;
                bool parent2Found = false;
                bool childFound = false;

                while(file >> line && line != "WHATEVER_SIGNALS_FAMILY_DONE"){
                    if(line == "ID"){
                        //Grab the family's id.
                        file >> line;
                        fTemp->id = line;

                        idFound = true;
                    }
                    else if (line == "CHILD"){
                        //I think having at least one child is mandatory for a GEDCOM,
                        //but I don't know. Parents ARE mandatory. Probably.
                        file >> line;

                        fTemp->children[fTemp->numChildren] = line;
                        fTemp->numChildren++;

                        childFound = true;
                    }
                    else if (line == "PARENT"){
                        file >> line;

                        if(!parent1Found){
                            parent1Found = true;
                            fTemp->parent1 = line;
                        }
                        else if (!parent2Found){
                            parent2Found = true;
                            fTemp->parent2 = line;
                        }
                        else{
                            cout << "WHOOPS." << endl;
                            //Throw an error you have three parents!
                        }
                    }
                }
                if(!parent1Found || !parent2Found || !childFound || !idFound){
                    //You're missing something
                    cout << "Whoops." << endl;
                    //So it should error.
                }
                families[fTemp->id] = ;

            }
        }
    }
    else{
        cout << "Error, file could not be opened." << endl;
    }

    //Now make an iterator and have it run through the families unordered set.
    for(auto it = families.begin();it != families.end();++it){
        linkFamily(*it);
    }

    return 0;
}
