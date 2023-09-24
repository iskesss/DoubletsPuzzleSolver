#include <iostream>
#include <stack>
#include <fstream>
#include <vector>
#include "graph.h"

using namespace std;

int main(){

	string fileName = "fiveletterwords.txt"; // feel free to change the input file name!
	ifstream infile(fileName);
	string currWord;
	vector<string> allWords;
	vector< pair< pair<string,size_t>, pair<string,size_t> > > start_and_end_locations;
	//starting word & location^^                ^^ ending word & location
	graph edges;

	// here we load the doublets we want to find into a vertex of pairs of string/size_t pairs, intended to store both words and their respective index numbers
	start_and_end_locations.push_back(make_pair( make_pair("black",size_t()), make_pair("white",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("tears",size_t()), make_pair("smile",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("small",size_t()), make_pair("giant",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("stone",size_t()), make_pair("money",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("angel",size_t()), make_pair("devil",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("amino",size_t()), make_pair("right",size_t()) ));
	start_and_end_locations.push_back(make_pair( make_pair("amigo",size_t()), make_pair("signs",size_t()) ));

	size_t vecPos = 0;
	while(infile >> currWord){

		allWords.push_back(currWord);
		edges.add_vertex(); // adds vertex who's number corresponds to the index number of 'currWord', which was just added to the vector
		
		for(size_t i = 0; i < start_and_end_locations.size(); ++i){ // if the word we're loading into the vector/graph is one of the double start/end points, we should make note of its coordinates (for later)
		    if(currWord == start_and_end_locations[i].first.first){
		        start_and_end_locations[i].first.second = vecPos;
		    }
		    if(currWord == start_and_end_locations[i].second.first) {
		        start_and_end_locations[i].second.second = vecPos;
		    }
		}
		++vecPos;
	}
	infile.close();

	size_t ePos = 0;
	for( auto e : allWords){ // here we connect similiar words (words sharing all but one letter) to each other via graph edges
		size_t cPos = 0;
		for(auto c : allWords){
			if(c == e){ break; }
			bool isDoublet = true; // innocent until proven guilty!
			bool alreadyDifferent = false;
			size_t charPos = 0;

			while(isDoublet && charPos < 5){
				if(e[charPos] != c[charPos]){
					if(!alreadyDifferent){ alreadyDifferent = true; }
					else{ isDoublet = false; }
				}
				++charPos;
			}

			if(isDoublet){ 
				edges.add_edge(ePos,cPos);
				cout << allWords[ePos] << " <---> " << allWords[cPos] << endl;
			}

			++cPos;
		}
		++ePos;
	}

	cout << "---------------------------------------" << endl;
	for(size_t i = 0; i < start_and_end_locations.size(); ++i){
		cout << "Finding path from '" << allWords[start_and_end_locations[i].first.second] << "' to '" << allWords[start_and_end_locations[i].second.second] << "'" << endl;
		vector<int> tempPath = edges.shortest_path(start_and_end_locations[i].first.second, start_and_end_locations[i].second.second);
		if(tempPath.size() > 0){
			for(size_t i = tempPath.size(); i > 0; --i){
				cout << allWords[tempPath[i-1]] << " -> ";
			}
			cout << "DONE!" << endl;
		}
		else{
			cout << "NO SOLUTION!" << endl;
		}
		cout << "---------------------------------------" << endl;
	}
	
	return 0;
}