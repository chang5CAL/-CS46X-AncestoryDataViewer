#include <iostream>
#include <vector>
#include <string>


std::vector<std::vector<double>> levelMaxVal;
//This should be [tree][level]


/*Temporary, remove later*/
struct Person {
	std::string id;
	std::string name;
	std::string sprouse;
	std::string father;
	std::string mother;
	std::vector<std::string> children;
    double pos = 0;
    double range = 0;
    int level = 0; //This is the Y level. It goes up as the range
};

    std::vector<std::string> roots;
    std::vector<std::string> findRoots(std::vector<Person> family){
    //We just store the id.
    bool spouseAdded = false;
    for(int i=0;i<family.size();++i){
        if(family[i].father == NULL && family[i].mother == NULL
           && ((family[i].spouse.father == NULL && family[i].spouse.mother == NULL)
               || (family[i].spouse == NULL))){
            //That's a hefty if, but it basically just checks if the node has parents,
            //and if their spouse either exists, or if their spouse also has no parents.
            for(int j=0;j<roots.size();++j){
                if(roots[j] == family[i].spouse){
                    //Will need to come back to solve divorces.
                    spouseAdded = true;
                }
            }
            //This for makes sure the spouse hasn't shown up before.
            if(!spouseAdded){
                //If the spouse has been added already, don't add.
                roots.push_back(family[i].id);
            }

            spouseAdded = false;
            //So the fatal flaw is that we assume the roots married once and had children.
            //If W-X and Y-Z both had children, then X-Y and W-Z got together, we'll forget to
            //push one in.
            //Fortunately, GEDCOM files only allow one spouse, so we just need to keep track of
            //divorces.
        }
    }
    return roots;
}

void levelInsert(int tree,int level, double val){
    //Checks if an inputted value is greater than
    //the current value, and if it doesn't exist,
    //create all levels until it does.
    while(levelMaxVal[tree].size() < level){
        levelMaxVal[tree].push_back(0);
    }
    if(levelMaxVal[tree][level-1] < val){
        levelMaxVal[tree][level-1] = val;
    }
    //Remember, if we get 3, we add 0,1,2 as 0,0,0. Therefore 2 is the third value.

}

//This is similar to the fibonacci sequence.
double spacingChildren(int tree,std::string id,std::vector<Person> p,int level){
    //This should give me the root for the id to start with.
    double maxVal = 0;
    double curVal = 0;

    level++; //Increment level for each. Start level at 0.
    p[id].level = level-1;
    //This should be find id function, but I don't know how that one works.

    if(p[id].children.size() == 0){
        //If you have no children, your size is always 3.
        p[id].range = 3;
        levelInsert(tree,level,3);
        return 3;
        //Base case is 3 because it's one for the person, one for their link and one for their spouse.
        //O-O <- like this
    }

    for(int i=0;i<p[id].children.size();++i){
        //Find the maximum child
        curVal = spacingChildren(tree,p[id].children[i],level);
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
        //    | |-|
        //    o o o
       //The spacing should be 18 for the parent, and 6 for the children EACH.
       //This is because the rightmost child has two children, so the bottom-most's
       //maxVal would return 3 each, and there's two children so below it would
       //set maxVal for the parent as 3*2 to 6.
        if(curVal > maxVal){
            maxVal = curVal;
        }

    }

    maxVal = maxVal * p[id].children.size();
    //Now the max value is set to the amount of children * the largest node

    p[id].range = maxVal;
    levelInsert(tree,level,maxVal);

    return maxVal;
}

void place(int tree,std::string id,std::vector<Person> p,double minPos){
    //The position is always half the range minus 1, to leave room for the
    //connector pipe and the spouse.
    p[id].pos = levelMaxVal[tree][p[id].level] - 1 + minPos;
    //minPos starts at 0.

    //So we know that all of the children will have the same range.
    for(int i=0;i<p[id].children.size();++i){
        //So we'll do each subtree at a time.
        place(tree,p[id].children[i],p,minPos);
        minPos += levelMaxVal[tree][p[id].level+1]; //Increments by the child's max range.
        //We want the range below the current node for the minPos, not the range of the
        //current node. Never worry about the +1 because having a child is a requisite for
        //this loop to run.

        //Remember that the parent is in charge of placing it's children.

    }
}


void startSort(){
    //This is the only public function. It runs all the above functions.
    //We assume the parsed data, f, is given and global.
    std::vector<std::string> p = findRoots(f); //Where f is the parsed data
    for(int i=0;i<p.size();++i){
        spacingChildren(i,p[i],f,0);
        place(i,p[i],f,0);
    }
}

int main()
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}
