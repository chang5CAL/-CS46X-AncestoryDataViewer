#include <iostream>

using namespace std;

///Note to self; Draw it out, since you seem to not know how to draw positions


/*Temporary, remove later*/
struct Person {
	string id;
	string name;
	string sprouse;
	string father;
	string mother;
	vector<string> children;
};

struct PosNode{
    string id;
    double pos = 0;
    double range = 0;
    int level = 0; //This is the Y level. It goes up as the range
    //finder recurses, so we can go back and set the range of others later.
};

vector<node> findRoots(unordered_map family){
    vector<node> roots;
    for(auto it = family.begin; it != family.end(); ++it){
        if(it->second.parent1 == NULL && it->second.parent2 == NULL
           && ((it->second.spouse.parent1 == NULL && it->second.spouse.parent2 == NULL)
               || (it->second.spouse == NULL))){
            //That's a hefty if, but it basically just checks if the node has parents,
            //and if their spouse either exists, or if their spouse also has no parents.
            roots.push_back(it->second.id);
        }
    }
    return roots;
}

//So I realized this can actually be done like the Fibonacci sequence.
double spacingChildren(PosNode* b,Person p,int level){
    double maxVal = 0;
    double curVal = 0;

    level++; //Increment level for each. Start level at -1 when running this.
    b.level = level;

    if(p.children.size() == 0){
        //If you have no children, your size is always 3.
        b.range = 3;
        return 3;
        //Base case is 3 because it's one for the person, one for their link and one for their spouse.
        //O-O <- like this
    }

    for(int i=0;i<;++i){
        //Find the maximum child
        curVal = spacingChildren(p.children[i]);
        //We're going to recursively call this function, stopping when
        //a person has no children, in which case it goes in to the
        //above base case.

        //For an example, if it's like
        //  o
        //|-|-|
        //o o o
        //The spacing should be 9 for the parent, and 3 for the children each.
        //For this one below:
        //    o
        // |--|--|
        // o  o  o
        //|-|
        //o o
       //The spacing should be 18 for the parent, and 6 for the children EACH.
       //This is because the leftmost child has two children, so the bottom-most's
       //maxVal would return 3 each, and there's two children so below it would
       //set maxVal for the parent as 3*2 to 6.
        if(curVal > maxVal){
            maxVal = curVal;
        }

    }
    for(int i=0;i<;++i){
        //The above for loop got the largest value per child.
        //Apply the maxVal to all direct children of the node.
        b[p.children[i]].range = maxVal;
    }

    maxVal = maxVal * p.children.size();
    //Now the max value is set to the amount of children * the largest node

    b.range = maxVal;
    return maxVal;
}

unordered_map<int,int> levelEqualize(unordered_map<string,PosNode> b,Person p,unordered_map<int,int> maxLevelPos){
    //Start at the 0th level. That one has no siblings, or it wouldn't be a progenitor.
    for(int i=0;i<p.children;++i){
        //Since we start at the progenitor, this one will be 1.
        if(maxLevelPos[b[p.children[i]].level] == NULL){
            //If the level doesn't exist, fill it.
            maxLevelPos[b[p.children[i]].level] = 0;
        }
        else if(b[p.id].pos > maxLevelPos[b[p.children[i]].level]){
            //Check if the current max value is greater than the current max
            maxLevelPos[b.level] = b.pos;
        }
        levelEqualize(b[p.children[i]],p[p.children[i]],maxLevelPos);
        //So go down the tree and figure out the next level, and recurse back up. Need to go through
        //all the values anyways.
    }
    return maxLevelPos;
}

void place(PosNode b,Person p,double minPos,unordered_map<int,int> maxLevelPos){
    //Once ranges are done, place positions
    //First place is half it's range, minus 1.
    if(b.level == 0){
        b.pos = b.range/2 - 1;
    }
    else{
        b.pos = maxLevelPos[b.level]/p.children.size() - 1 + minPos;
    }

    //So we know that all of the children will have the same range.
    for(int i=0;i<p[b.id].children.size();++i){
        //So we'll do each subtree at a time.
        place(b[p.children[i]],p.children[i],b.pos/p[b.id].children.size());
        minPos += b.range/p[b.id].children.size();
        //So run this function again with the new position, starting at 0,
        //or whatever the min is/was. Then it places the node in the middle
        //of it's block - 1.
        //Rinse repeat.

    }
}

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
