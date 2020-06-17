// ======================================================================
// FILE:        MyAI.hpp
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

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <functional>


//using namespace std::chrono; 
using namespace std;
#define intPair std::pair<int, int>


class MyAI : public Agent
{
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;
	~MyAI();
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
private: 
    struct CompareValue{
        bool operator()(const std::pair<intPair, int>& left, const std::pair<intPair, int>& right) const
        {
            if(!left.second){
                return false;
            }
            else if(!right.second){
                return true;
            }
            return left.second < right.second;
        }
    };

    struct CompareCoord{
        bool operator()(const intPair& left, const intPair& right) const
        {
            if(left.first == right.first){
                return left.second < right.second;
            }

            return left.first < right.first;
        }
    };

    struct CompareMapVec{
        bool operator()(const std::vector<std::map<intPair, int>>& left, const std::vector<std::map<intPair, int>>& right) const
        {
            if(left.size() == right.size()){
				return left[0].size() < right[0].size();
			}
			return left.size() < right.size();
        }
    };

    struct tile{
        int number;
        int label;
        int adjCovered;
        tile() : number{-1}, label{-1}, adjCovered{0}{}
    };
    
    tile** board;
    clock_t start, cspClock;
	int x_location, y_location;
    int mines, coveredNum;
    int rowCount, colCount;
   
    
    std::map<intPair,int> adjMap; //key is coord and value is number of tiles with number adjacent to this
    std::vector<intPair> domainVec; //temp containment for domain for constraints
    void addCSP();
    std::set<intPair> zeroSet;
    
    std::set<intPair> numSet; //holds all the coordinates that are non-zero
    std::vector<unsigned> indexVec;
    std::vector<std::map<intPair, int>> consistentMappings;
    std::vector<std::vector<std::map<intPair, int>>> csp;

    void insertMine(intPair coord);
    void printBoard();
    void getAdjacentMine(std::function<void(MyAI*, int, int)> func, bool covered=true, bool mine=false);
    intPair getMin(std::map<intPair, int> adjMap);
    void createMap(std::vector<std::map<intPair, int>>& mapVec, std::vector<unsigned>& data);
    void combineCSP(std::vector<std::map<intPair, int>>& mapVec, std::vector<unsigned> data, unsigned start, unsigned index, unsigned size);

    void checkSubset(std::vector<intPair>& shorter, std::vector<intPair>& longer, std::vector<intPair>& newVec);
    void checkSingleMap(std::vector<std::map<intPair, int>>& mapVec, std::set<intPair>& mineSet);
    
    std::vector<std::map<intPair, int>> reduceCSP(std::set<intPair>& tempMineSet, int bigLabel, int smallLabel);
    
    
    float getTime(clock_t startTime);
    void runCSP(std::vector<std::vector<std::map<intPair, int>>>& csp,std::set<intPair>& tempMineSet);
    //when a tile is fulfilled then adjacent covered tiles decremented
    void computeLabel(int x, int y);
    void insertDomain(int x, int y);
    void insertZero(int x, int y);
    void insertAdjMap(int x, int y);
    void countCovered(int x, int y);
    void decAdjCovered(int x, int y);
    bool isCovered(int x, int y);
    void recurseCheck(std::map<intPair, int> data, int outer);
    bool notMine(int x, int y);
    void decLabel(int x, int y);
    void initIndexVec();
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
