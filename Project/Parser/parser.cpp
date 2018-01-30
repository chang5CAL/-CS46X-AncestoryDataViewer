#include <iostream>
#include <string>
#include <unordered_set>
//Really, this should all be in a class.

using namespace std;


unordered_set<node> nodes = ;
unordered_set<families> families = ;

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
    string* children; //FAM
    string parent1; //FAMC
    string parent2; //FAMC
    int numChildren;
    //This should store their IDs, the nodes store their location.
};

//So GEDCOM files work as basically "subtrees" and "nodes", for our purposes.
//Soooooooooooo,
//Let's start by finding every GEDCOM file that does NOT have a FAMC.


void createFamily(){
    //Set the parents and the children of a given GEDCOM.
}

void createNode(){

}
//Run these two in more or less equal order.

void linkFamily(family f,unordered_set<node> n){
    for(int i=0;i<f->numChildren;++i){
        n[f->parent1]->child[i] = n[f->children[i]];
        n[f->parent2]->child[i] = n[f->children[i]];
        //Take the parent node's child parameter and attach the
        //child's node to it.
        n[f->children[i]]->parent1 = n[f->parent1];
        n[f->children[i]]->parent2 = n[f->parent2];
        //And attach the parents as the child's parents.
    }
    //This should work fine. Now to just read the file.
}

void linkNode(){
    //Given a node, find the nodes it is attached to.
}

int main()
{
    //Maybe a vector of pointers to keep track? Or a map?
    //Hash set seems best, so we can have the ID of the person/family,
    //So it points to them and so we can access them directly in O(1) time.
    node* n = NULL;
    families = new family[100]; //Temp, just set it to 100
    while( != NULL){
        //Read the file
        if(){
            //If it's a node
            n = new node;
            node->id = ;
            nodes[/** id **/] = n;
            //Keep going for a name...
            while(WORD != NAME){

            }
        }
        else if (){
            //If it's a family, make the family.

        }
    }
    return 0;
}
