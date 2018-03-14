// Fill out your copyright notice in the Description page of Project Settings.
#include "GenerateTree.h"


//Note that there are a lot of comments. I'm keeping them so if someone (Including me) needs to reevaluate something,
//they know what they're looking at -Justin

//This class is created and runs once the file name is acquired.
GenerateTree::GenerateTree()
{
	//
	static ConstructorHelpers::FObjectFinder<UClass> NodeFinder(TEXT("Blueprint'/ADVR/Content/Blueprints/Node.Node_C'"));
	Node = Node.Object;

	//When this is run, it should take the location from some (as of writing) unknown other class
	//that came from the UI that has the file location from the user.
	std::string file = "s.ged"; //Currently a default value that assumes the file's s.ged in the same folder.
	parsedData = parse(file);

	roots = findRoots(parsedData);
	
	for (int i = 0; i < roots.size(); ++i) {
		spacingChildren(i, roots[i],parsedData,0);
		place(i,roots[i],parsedData,0);

		//Places the nodes for the tree
		placeNodes(roots[i],parsedData,NULL); //Note to self: if NULL bugs, just send roots[i].
		//In the first run, level == 0, so we won't search for the parent.
	}
	//I need to link the same node to itself if it shows up elsewhere (Ex: spouses).
}

GenerateTree::~GenerateTree()
{
}

//Parser Functionality
int GenerateTree::search_by_id(std::vector<Person> p, std::string str) {
	int i = 0;
	while (i < p.size()) {
		if (p[i].id == str) {
			//cout << "husb  " << i << endl;
			return i;
		}
		i++;
	}
	return -1; //Return an impossible value if the id doesn't exist.
}

void GenerateTree::parse(std::string fileLocation) {
	std::vector<Person> p;
	int i, j, husb, wife, chil;
	string str;
	string str2("0 @I");
	string str3("1 NAME");
	string str4("1 HUSB");
	string str5("1 WIFE");
	string str6("1 CHIL");

	ifstream input(fileLocation);

	i = 0;
	j = 0;

	if (!input.fail()) {

		while (getline(input, str)) {
			if (str.find(str2) != string::npos) {
				str.erase(6, 7);
				str.erase(0, 3);
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				p[j].id = str;
				//cout << j << endl;
				j++;
			}

			if (str.find(str3) != string::npos) {
				str.erase(0, 7);
				str.erase(remove(str.begin(), str.end(), '/'), str.end());
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				str.erase(str.length() - 1);
				p[i].name = str;
				//cout << i << endl;
				i++;
			}

			if (str.find(str4) != string::npos) {
				str.erase(11, 2);
				str.erase(0, 8);
				//cout << "str  :" << str << " length: " << str.length() <<endl;
				husb = search_by_id(p, str);
				//p[i].name = str;

			}

			if (str.find(str5) != string::npos) {
				str.erase(11, 2);
				str.erase(0, 8);
				wife = search_by_id(p, str);
				p[husb].spouse = p[wife].name;
				p[wife].spouse = p[husb].name;
				//p[i].name = str;

			}

			if (str.find(str6) != string::npos) {
				str.erase(11, 2);
				str.erase(0, 8);
				chil = search_by_id(p, str);
				p[chil].mother = p[wife].name;
				p[chil].father = p[husb].name;
				//p[i].name = str;
			}
		}
	}
	input.close();

	i = 0;

	return p;
}

//Sorting algorithm functionality
void GenerateTree::levelInsert(int tree, int level, float val) {
	//Checks if an inputted value is greater than
	//the current value, and if it doesn't exist,
	//create all levels until it does.
	std::vector<float> f;
	while (levelMaxVal.size() < tree) {
		//If the outer vector doesn't have a vector, then add it in until it does.
		levelMaxVal.push_back(f);
	}
	while (levelMaxVal[tree].size() < level) {
		levelMaxVal[tree].push_back(0);
	}
	if (levelMaxVal[tree][level - 1] < val) {
		levelMaxVal[tree][level - 1] = val;
	}
	//Remember, if we get 3, we add 0,1,2 as 0,0,0. Therefore 2 is the third value.
}

std::vector<std::string> GenerateTree::findRoots(std::vector<Person> family) {
	//We just store the id.
	bool spouseAdded = false;
	for (int i = 0; i<family.size(); ++i) {
		//I can do this since I know the family is stored in a vector so I can just iterate through it.
		if (family[i].father.empty() && family[i].mother.empty()
			&& ((family[i].spouse.father.empty() && family[i].spouse.mother.empty())
				|| (family[i].spouse.empty()))) {
			//Check if 
			if (!family[i].spouse.empty()) {
				//If a spouse exists, check if they're in the roots already have them
				for (int j = 0; j < roots.size(); ++j) {
					if (roots[j] == family[i].spouse) {
						//Will need to come back to solve divorces.
						spouseAdded = true;
					}
				}
			}
			//If either the spouse doesn't exist, or they haven't been added already, add it
			if (!spouseAdded) {
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

float GenerateTree::spacingChildren(int tree, std::string sid, std::vector<Person> p, int level) {
	//This should give me the root for the id to start with.
	double maxVal = 0;
	double curVal = 0;
	int id = search_by_id(p,sid);

	level++; //Increment level for each. Start level at 0.
	p[id].level = level - 1;
	//This should be find id function, but I don't know how that one works.

	if (p[id].children.size() == 0) {
		//If you have no children, your size is always 3.
		p[id].range = 3;
		levelInsert(tree, level, 3);
		return 3;
		//Base case is 3 because it's one for the person, one for their link and one for their spouse.
		//O-O <- like this
	}

	for (int i = 0; i<p[id].children.size(); ++i) {
		//Find the maximum child
		curVal = spacingChildren(tree, p[id].children[i], level);
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
		if (curVal > maxVal) {
			maxVal = curVal;
		}

	}

	maxVal = maxVal * p[id].children.size();
	//Now the max value is set to the amount of children * the largest node

	p[id].range = maxVal;
	levelInsert(tree, level, maxVal);

	return maxVal;
}

void GenerateTree::place(int tree, std::string sid, std::vector<Person> p, float minPos) {
	int id = search_by_id(p, sid);

	//The position is always half the range minus 1, to leave room for the
	//connector pipe and the spouse.
	p[id].pos = levelMaxVal[tree][p[id].level] - 1 + minPos;
	//minPos starts at 0.

	//So we know that all of the children will have the same range.
	for (int i = 0; i<p[id].children.size(); ++i) {
		//So we'll do each subtree at a time.
		place(tree, p[id].children[i], p, minPos);
		minPos += levelMaxVal[tree][p[id].level + 1]; //Increments by the child's max range.
		//We want the range below the current node for the minPos, not the range of the
		//current node. Never worry about the +1 because having a child is a requisite for
		//this loop to run.
		
		//Remember that the parent is in charge of placing it's children.
	}
}

void GenerateTree::placeNodes(Person p, std::vector<Person> family, Person parent) {
	//By the sheer fact you're in here, we know the person node exists, so create it with no checks.
	float offset = 0;
	for (int i = 0; i < p.treeLevel;++i) {
		offset += levelMaxVal[i][0]; //The 0th node in each levelMaxVal is the max!
		//Keep adding until you hit the tree level, so you move over enough.
	}
	FVector loc = FVector(p.pos + offset, p.level, 0.f); //Note to self: Look up coords later. I'm almost certain this is X-Y-Z, but still.
	FName name = FName(p.name);
	//Create text(In this case, the name)
	
	//Append the text to the node

	//Place the node
	ANode* personNode = GetWorld()->SpawnActor(ANode, name, loc, FRotator::ZeroRotator);
	//Or perhaps the text should go down here. Then use setText.

	p.placed = true;

	if (p.level > 0) {
		//Place a pipe from the node to their parent's IF they are not level 0.
		//Level 0 means the parents don't actually exist.


		FVector pLoc = FVector(parent.pos + offset + 1, parent.level, 0.f);
		//Generate the parent's location.

		if (p.mother.empty() || p.father.empty()) {
			//If one of the parents are missing
			pLoc = FVector(parent.pos + offset, parent.level, 0.f);
		}
		else {
			pLoc = FVector(parent.pos + offset + 1, parent.level, 0.f);
		}
		GetWorld()->LineBatcher->DrawLine(loc, pLoc, Color.ToFColor(false), SDPG_World, Thickness, LifeTime);
	}

	if (p.spouse != std::string.empty()) {
		//If the spouse exists, add them.
		FVector sLoc = FVector(p.pos + offset + 2, p.level, 0.f);
		//Create text(In this case, the spouse'sname)
		FName sName = FName(s.name);

		ANode* spouseNode = GetWorld()->SpawnActor(GetClass(ANode), sName, sloc, FRotator::ZeroRotator);

		//Draw a line from the person to their spouse
		GetWorld()->LineBatcher->DrawLine(loc, sLoc, Color.ToFColor(false), SDPG_World, Thickness, LifeTime);

		int spouseID = search_by_id(family, p.spouse);
		if (family[spouseID].placed) {
			Person s = family[spouseID];
			float sOffset = 0;
			for (int j = 0; j < s.treeLevel; ++j) {
				sOffset += levelMaxVal[j][0];
			}
			//The spouse has been placed, so make a pipe that goes to it
			FVector spouseLoc = FVector(s.pos + sOffset,s.level,0.f);
			//So now we know the two locations that we need to point to and from, loc and spouseLoc
			//Make sure it's transparent, though.
			GetWorld()->LineBatcher->DrawLine(spouseLoc, loc, Color.ToFColor(false), SDPG_World, Thickness, LifeTime);
		}


		
	}

	for (int i = 0; i < p.children.size(); ++i) {
		placeNodes(parsedData[p.children[i]], family, p);
		//Recursion! Since we run this per tree and once the tree is done, we can safely assume everything's done.
	}
}


