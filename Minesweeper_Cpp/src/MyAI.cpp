
// FILE:        MyAI.cpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"
#include <iostream>

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) :
Agent(), rowCount{_rowDimension}, colCount{_colDimension},
board{new tile*[_colDimension]()},
x_location{_agentX},
y_location{_agentY}, mines{_totalMines},
coveredNum{_rowDimension*_colDimension-1}
{
    start = clock();
    //allocates space for game board
    for ( int index = 0; index < _colDimension; ++index )
        board[index] = new tile[_rowDimension]();
    

    //initializes board with -1's and adjMap with 0's
    for (int i =0;i<rowCount;i++){
        for (int j =0;j<colCount;j++){
            adjMap.insert(std::pair<intPair, int>(intPair(j,i), 0));
        }
    }
};

/* prints the game board
*/
void MyAI::printBoard(){
    std::cout<<x_location<<' '<<y_location<<std::endl;
    for (int i = rowCount-1; i>=0; i--){
        for (int j = 0; j<colCount; j++){
            std::cout<<board[j][i].label<<"\t";
        }

    std::cout<<std::endl;
    }
}


/* This is a group of functions that we
   pass to getAdjacentMine. These are
   executed for each tile adjacent to
   our current location
*/
void MyAI::insertDomain(int x, int y){
    //push covered tiles adjacent to a number to domain
    domainVec.push_back(intPair(x, y));
    
}

void MyAI::insertZero(int x, int y){
    //pushes tiles guaranteed to be safe to zeroSet
    zeroSet.insert(intPair(x, y));
}

void MyAI::insertAdjMap(int x, int y){
    //inserts tiles into adjSet and increments adjMap entry
    adjMap[intPair(x, y)]+=board[x_location][y_location].label;
}

void MyAI::countCovered(int x, int y){
    board[x_location][y_location].adjCovered++;
}

void MyAI::decAdjCovered(int x, int y){
    board[x][y].adjCovered--;
}

bool MyAI::isCovered(int x, int y){
    return board[x][y].number < 0;
}

bool MyAI::notMine(int x, int y){
    return board[x][y].number != -2;
}

void MyAI::decLabel(int x, int y){
    board[x][y].label--;
}

void MyAI::computeLabel(int x, int y){
    board[x_location][y_location].label--;
}

/*
    checks adjacent tiles that are in bounds
    if bool covered = true then runs func for
    each adjacent covered tile else if bool
    covered = false then it checks for numbered
    tiles
*/
void MyAI::getAdjacentMine(std::function<void(MyAI*, int, int)> func, bool covered, bool mine){
    int x = x_location, y = y_location;
    
    //checks locations where x and y coord are both non-zero
    if (x>0 && y>0){
        if (notMine(x-1, y-1) != mine && isCovered(x-1, y-1) == covered){
            func(this, x-1,y-1);
        }
    }
    //checks spaces when x coord is non-zero
    if (x > 0){
        if (notMine(x-1, y) != mine && isCovered(x-1, y) == covered){
            func(this, x-1, y);
        }
    }
    //checks locations where x is non-zero and y is within bounds
    if (x>0 && y+1<rowCount){
        if (notMine(x-1, y+1) != mine && isCovered(x-1, y+1) == covered){
            func(this, x-1,y+1);
        }
    }
    //checks spaces where y coord is non-zero
    if (y > 0){
        if (notMine(x, y-1) != mine && isCovered(x, y-1) == covered){
            func(this, x, y-1);
        }
    }
    //checks spaces where y coord is not out of bounds
    if (y+1 < rowCount){
        if (notMine(x, y+1) != mine && isCovered(x, y+1) == covered){
            func(this, x, y+1);
        }
    }
    //checks locations where x is within bounds and y is non-zero
    if (x+1 < colCount && y > 0){
        if (notMine(x+1, y-1) != mine && isCovered(x+1, y-1) == covered){
            func(this, x+1,y-1);
        }
    }
    //checks spaces where x coord is not out of bounds
    if (x+1 < colCount){
        if (notMine(x+1, y) != mine && isCovered(x+1, y) == covered){
            func(this, x+1, y);
        }
    }
    //checks locations where x and y are within bounds
    if (x+1 < colCount && y+1 < rowCount){
        if (notMine(x+1, y+1) != mine && isCovered(x+1, y+1) == covered){
            func(this, x+1,y+1);
        }
    }
}

void MyAI::insertMine(intPair coord){
    adjMap.erase(coord);
    board[coord.first][coord.second].number = -2;
    x_location = coord.first;
    y_location = coord.second;
    getAdjacentMine(&MyAI::decLabel, false);

}

//returns part of longer set
//that is not a subset
void MyAI::checkSubset(std::vector<intPair>& shorter, std::vector<intPair>& longer, std::vector<intPair>& newVec){
    unsigned index = 0;
    if(!shorter.size()){
        return;
    }
    for(auto domain : longer){
        if(domain == shorter[index]){
            index++;
        }
        else{
            newVec.push_back(domain);
        }
    }
    if(shorter.size() == index){
        return;
    }
    newVec = std::vector<intPair>();
}

void MyAI::initIndexVec(){
    indexVec.clear();
    for(unsigned i = 0; i < domainVec.size(); i++){
        indexVec.push_back(i);
    }
}

void MyAI::checkSingleMap(std::vector<std::map<intPair, int>>& mapVec, std::set<intPair>& mineSet){
    for(auto mapping : mapVec[0]){
        if(!mapping.second){
            zeroSet.insert(mapping.first);
        }
        else if(mapping.second){
            mineSet.insert(mapping.first);
        }
    }
}

std::vector<std::map<intPair, int>> MyAI::reduceCSP(std::set<intPair>& tempMineSet, int bigLabel, int smallLabel)
{
    unsigned difference = 0;
    std::vector<std::map<intPair, int>> tempMapVec;
    std::vector<unsigned> temp; //storage for combinations of indices
    
    initIndexVec();
    difference = bigLabel - smallLabel;
    temp = std::vector<unsigned>(difference, 0);
    combineCSP(tempMapVec, temp, 0, 0,difference);
    return tempMapVec;
}

/* adds constraints to graph for us
   to iterate through to find a consistent
   set of constraints
*/
void MyAI::addCSP(){
    int x_temp = x_location, y_temp = y_location;   //stores our current location
    std::vector<unsigned> temp; //storage for combinations of indices
    std::vector<std::vector<intPair>> tempDomain;
    std::set<std::vector<intPair>> setDomain; //copy of tempDomain
    std::set<intPair> tempMineSet;
    std::vector<int> labelVec;
    std::vector<intPair> newDomain;
    
    bool addCSP = true; 
    
    for(auto coord : numSet){
        //changes current location and inserts adjacent tiles into domain
        addCSP = true;
        x_location = coord.first;
        y_location = coord.second;
        getAdjacentMine(&MyAI::insertDomain);

        if(setDomain.find(domainVec) == setDomain.end()){
            tempDomain.push_back(domainVec);
            setDomain.insert(domainVec);
            labelVec.push_back(board[coord.first][coord.second].label);
            for(unsigned index = 0; index < tempDomain.size()-1; index++){
                if(tempDomain[tempDomain.size()-1].size() < tempDomain[index].size()){
                    checkSubset(tempDomain[tempDomain.size()-1], tempDomain[index], newDomain);
                    if(!newDomain.empty()){
                        domainVec = newDomain;
                        csp[index] = reduceCSP(tempMineSet,labelVec[index], labelVec[labelVec.size()-1]);
                        
                        labelVec[index] = labelVec[index] - labelVec[labelVec.size()-1];
                        
                        if(csp[index].size() == 1){
                            checkSingleMap(csp[index], tempMineSet);
                        }
                        
                        tempDomain[index] = newDomain;
                        setDomain.insert(newDomain);
                        newDomain.clear();
                    }
                }
                else if(tempDomain[index].size() < tempDomain[tempDomain.size()-1].size()){
                    checkSubset(tempDomain[index],
                    tempDomain[tempDomain.size()-1], newDomain);
                    if(!newDomain.empty()){
                        addCSP = false;
                        domainVec = newDomain;
                        csp.push_back(reduceCSP(tempMineSet,labelVec[labelVec.size()-1],labelVec[index]));

                        labelVec[labelVec.size()-1] = labelVec[labelVec.size()-1] - labelVec[index];

                        if(csp[csp.size()-1].size() == 1){
                            checkSingleMap(csp[csp.size()-1], tempMineSet);
                        }
                        
                        tempDomain[tempDomain.size()-1] = newDomain;
                        setDomain.insert(newDomain);
                        newDomain.clear();
                    }
                }
            }
            if(addCSP){
                domainVec = tempDomain[tempDomain.size()-1];
                initIndexVec();
                csp.push_back(std::vector<std::map<intPair, int>>());
                temp = std::vector<unsigned>(labelVec[labelVec.size()-1], 0);
                combineCSP(csp.at(csp.size()-1), temp, 0, 0,labelVec[labelVec.size()-1]);
                if(csp[csp.size()-1].size() == 1){
                    checkSingleMap(csp[csp.size()-1], tempMineSet);
                }
                temp.clear();
            }
        }
        domainVec.clear();
        indexVec.clear();
    }
    if(zeroSet.empty() && getTime(start) < 295){
        std::map<intPair, int> finalMap;
        std::sort(csp.begin(), csp.end(), CompareMapVec());
        cspClock = clock();
        recurseCheck(finalMap, 0);
        
        if(getTime(cspClock) > 30 || getTime(start) > 295){
            return;
        }
        for(unsigned i = 0; i < consistentMappings.size(); i++){
            for(auto pair : consistentMappings[i]){
                if(finalMap.find(pair.first) == finalMap.end()){
                    finalMap.insert(pair);
                }
                else if(finalMap[pair.first] && pair.second){
                    finalMap[pair.first]++;
                }
                else if(finalMap[pair.first] || pair.second){
                    finalMap[pair.first] = 1;
                }
                if(finalMap[pair.first] == consistentMappings.size()){
                    tempMineSet.insert(pair.first);
                }
            }
        }
        for(auto entry : finalMap){
            if(!entry.second && board[entry.first.first][entry.first.second].number == -1){
                zeroSet.insert(entry.first);
            }
        }
        consistentMappings.clear();
    }
    
    for(auto entry : tempMineSet){
        insertMine(entry);
    }
    csp.clear();
    x_location = x_temp;
    y_location = y_temp;
}


float MyAI::getTime(clock_t startTime){
    return float(clock()-startTime)/CLOCKS_PER_SEC;
}

void MyAI::recurseCheck(std::map<intPair, int> mapping, int outer)
{
    if(getTime(cspClock) > 30 || getTime(start) > 295){
        return;
    }
    std::map<intPair, int> temp = mapping;
    bool consistent = true;
    if(outer == csp.size()){
        consistentMappings.push_back(mapping);
    }
    else{
        for(int inner = 0; inner < csp[outer].size(); inner++){
            consistent = true;
            for(auto pair : csp[outer][inner]){
                if(temp.find(pair.first) == temp.end()){
                    temp.insert(pair);
                }
                else if(temp[pair.first] != pair.second){
                    consistent = false;
                    break;
                }
            }
            if(consistent){
                recurseCheck(temp, outer+1);
           }
           temp = mapping;
        }
    }
}

/* create a mapping for mines
   to spaces in the domain
*/
void MyAI::createMap(std::vector<std::map<intPair, int>>& mapVec, std::vector<unsigned>& data){
    std::map<intPair, int> mapping;
    unsigned domainIndex = 0, dataIndex = 0;

    for(auto domain : domainVec){
        if(dataIndex < data.size() && data[dataIndex] == domainIndex){
            mapping.insert({domain, 1});
            dataIndex++;
        }
        else{
            mapping.insert({domain, 0});
        }
        domainIndex++;
    }

    mapVec.push_back(mapping);
}


/* generates all combinations of indices
   that are a certain size then maps these
   to the order of tiles in the domain as mines
*/
void MyAI::combineCSP(std::vector<std::map<intPair, int>>& mapVec, std::vector<unsigned> data, unsigned start,unsigned index, unsigned size){
    
    //combination is of desired size
    if(index == size){
        createMap(mapVec, data);
        return;
    }
    
    //adds next item in combination
    for(unsigned i = start; i <= indexVec.size()-1 && indexVec.size()-i >= size-index; i++){
        data[index] = indexVec[i];
        combineCSP(mapVec, data, i+1, index+1, size);
    }
}

Agent::Action MyAI::getAction( int number )
{
    if(getTime(start) > 299.5){
        return {LEAVE, -1, -1};
    }
    std::set<intPair>::iterator item;
    board[x_location][y_location].number = number;
    board[x_location][y_location].label = number;
    getAdjacentMine(&MyAI::computeLabel, true, true);
    getAdjacentMine(&MyAI::countCovered);
    
    adjMap.erase(intPair(x_location, y_location));
    std::set<intPair> tempCoord;
     
    if(number == 0){
        getAdjacentMine(&MyAI::insertZero);
    }
    else{
        numSet.insert(intPair(x_location, y_location));
        getAdjacentMine(&MyAI::insertAdjMap);
            
    }
    if(zeroSet.empty() && coveredNum > mines){
        addCSP();
    }
    if(!zeroSet.empty()){
        item = zeroSet.begin();
        x_location = item->first;
        y_location = item->second;
        zeroSet.erase(item);
        coveredNum--;
        getAdjacentMine(&MyAI::decAdjCovered, false);
        return {UNCOVER, x_location, y_location};
    }
    else if(coveredNum>mines){
        intPair coord = getMin(adjMap);
        x_location = coord.first;
        y_location = coord.second;
        coveredNum--;
        getAdjacentMine(&MyAI::decAdjCovered,false);
        return {UNCOVER, x_location, y_location};
    }
    
    return {LEAVE,-1,-1};
}

intPair MyAI::getMin(std::map<intPair,int> adjMap){
    std::pair<intPair, int> min =
        *min_element(adjMap.begin(), adjMap.end(), CompareValue());
    return min.first;
}
MyAI::~MyAI(){
    for(int i=0;i<colCount;i++){
        delete[] board[i];
    }
    delete[] board;
}
