#include<bits/stdc++.h>
using namespace std;

class Cell{
    int weight;
    public:
    int x,y;
    bool isBlocked;
    Cell(int x, int y,int weight = 0){
        this->x = x;
        this->y = y;
        assert(weight>=0 && weight<=9);
        this->weight = weight;
        isBlocked = false;
    }
    void setWeight(int val){
        if(val<=0 || val>9){
            cout<<"val can only be between 1 and 9"<<endl;
            return;
        }
        if(isBlocked){
            cout<<"The cell is blocked"<<endl;
            return;
        }
        weight = val;
    }
    int getWeight(){
        return weight;
    }
    char getCharCellValue(){
        return isBlocked?'#':weight+'0';
    }
};

class Board{
    int rows,cols;
    vector<Cell> grid;
    int getRandomValue(int low, int high){
        int diff = abs(high-low)+1;
        return rand()%diff + min(low,high);
    }
    void recusriveDivision(int rs, int re, int cs, int ce){
        if( (abs(rs-re)<2) ||  (abs(cs - ce)<2) )
            return;
        // if( (rs>=re) ||  (cs >=ce) )
        //     return;
        int row = getRandomValue(rs,re);
        int col = getRandomValue(cs,ce);
        for(int i = rs;i<=re;i++){
            blockCell(i,col);
            assert(getCharCellValue(i,col) == '#');
        }
        for(int j = cs; j<=ce; j++){
            blockCell(row,j);
            assert(getCharCellValue(row,j) == '#');
        }
        unblockCell(getRandomValue(rs,re),col);
        unblockCell(getRandomValue(rs,re),col);
        unblockCell(row,getRandomValue(cs,ce));
        unblockCell(row,getRandomValue(cs,ce));
        recusriveDivision(rs,row-1,cs,col-1);
        recusriveDivision(row+1,re,cs,col-1);
        recusriveDivision(rs,row-1,col+1,ce);
        recusriveDivision(row+1,re,col+1,ce);
    }
    public:
    Board(int rows, int cols){
        srand( (unsigned)time(NULL) );
        this->rows = rows;
        this->cols = cols;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                int val = getRandomValue(1,9);
                Cell thisCell(i,j,val);
                grid.push_back(thisCell);
            }
        }
    }
    bool isValidCoordinate(int x, int y){
        if(x<0 || x>= rows || y<0 || y>=cols)
            return false;
        return true;
    }
    string getStringVal(int point){
        int x = point/cols, y = point%cols;
        return "("+to_string(x+1)+","+to_string(y+1)+")";
    }
    bool isValidCell(int x, int y){
            return isValidCoordinate(x,y) && (!grid.at(x*cols+y).isBlocked);
        }
    char getCharCellValue(int x, int y){
        return grid.at(x*cols + y).getCharCellValue();
    }
    void blockCell(int x, int y){
        grid.at(x*cols + y).isBlocked = true;
    }
    void unblockCell(int x, int y){
        grid.at(x*cols + y).isBlocked = false;
    }
    void changeWeight(int x, int y, int val){
        grid.at(x*cols + y).setWeight(val);
    }
    void print(){
        for(int i=0;i<rows;i++){
            for(int j =0;j<cols;j++){
                cout<<getCharCellValue(i,j);
            }
            cout<<"\n";
        }
        cout<<"\n";
    }
    void recusriveDivision(){
        recusriveDivision(0,rows-1,0,cols-1);
    }
    vector<pair<int,int> > getNeighbours(int p){
        pair<int,int> point({p/cols, p%cols});
        vector<pair<int,int> > neighbours;
        vector<pair<int,int> > directions;
        directions.push_back({-1 ,0});
        directions.push_back({1,0});
        directions.push_back({0,1});
        directions.push_back({0,-1});
        for(pair<int,int> direction : directions){
            if(isValidCell(point.first + direction.first, point.second + direction.second)){
                neighbours.push_back({point.first + direction.first, point.second + direction.second});
            }
        }
        return neighbours;
    }

    pair<int,string> getShortestDistance(pair<int,int> src, pair<int,int> destn){
        int srcPoint = src.first * cols +src.second;
        int destnPoint = destn.first * cols + destn.second;
        if(grid.at(srcPoint).isBlocked || grid.at(destnPoint).isBlocked){
            return {INT_MAX,"PATH NOT FOUND"};
        }
        if(srcPoint == destnPoint){
            return {0,getStringVal(destnPoint)};
        }
        //point to key mapping
        unordered_map<int,int> distance;
        unordered_map<int,int> parent;
        for(int i = 0;i<rows*cols;i++){
            distance[i] = INT_MAX;
            parent[i] = i;
        }
        distance[srcPoint] = 0;
        // pair{distance,point}
        set<pair<int,int> > pq;
        pq.insert({0,srcPoint});
        while(!pq.empty()){
            pair<int,int> smallestPoint  = *pq.begin();
            pq.erase(pq.begin());
            vector<pair<int,int> > neighbhours = getNeighbours(smallestPoint.second);
            for(const pair<int,int> &neighbhour : neighbhours){
                int thisPoint = neighbhour.first *cols + neighbhour.second;
                if(smallestPoint.first + grid.at(thisPoint).getWeight() < distance[thisPoint]){
                    // delete the point from pq
                    auto f = pq.find({distance[thisPoint],thisPoint});
                    if(f!=pq.end()){
                        pq.erase(f);
                    }
                    distance[thisPoint] = smallestPoint.first + grid.at(thisPoint).getWeight();
                    parent[thisPoint] = smallestPoint.second;
                    pq.insert({smallestPoint.first + grid.at(thisPoint).getWeight(),thisPoint});
                }
            }
        }
        stack<int> pathPoints;
        pathPoints.push(destnPoint);
        int shortestPathDistance = distance[destnPoint];
        while(parent[destnPoint]!=destnPoint){
            destnPoint = parent[destnPoint];
            pathPoints.push(destnPoint);
        }
        if(pathPoints.size()<2){
            return {INT_MAX,"PATH NOT FOUND"};
        }
        string path;
        while(!pathPoints.empty()){
            path.append(getStringVal(pathPoints.top()));
            path.append(" ");
            pathPoints.pop();
        }
        return {shortestPathDistance,path};
    }
};
class Play{
    Board *board;
    int x,y;
    void Printinstructions(){
        cout<<"WELCOME TO THE MAZE!!"<<endl;
        cout<<"YOU can calculate the shortest distance from a cell to another"<<endl;
        cout<<"YOU can check via different algorithms"<<endl;
        cout<<"YOU can change the contents of maze as well"<<endl;
        cout<<"# represents blocked cell, numeric value means weight of that cell"<<endl;
        cout<<"weight of a cell can only be between 0 -9"<<endl;
        cout<<"start by entering dimensions of maze"<<endl;
    }
    bool isValidDimension(int x, int y){
        if(x<=0 || y<=0 ||x>this->x || y>this->y)
            return false;
        return true;
    }
    bool isValidWeight(int weight){
        return weight>=1 && weight<=9;
    }
    void printMenu(){
        cout<<"press 1 to find distances"<<endl;
        cout<<"press 2 to unblock a cell"<<endl;
        cout<<"press 3 to block a cell"<<endl;
        cout<<"press 4 to change cell weight"<<endl;
        cout<<"press 5 to reprint the matrix"<<endl;
        cout<<"press 6 to exit"<<endl;
    }
    bool isValidChoice(int choice){
        // bool val = ((choice>=1) && (choice<=5));
        // cout<<"You entered "<<choice<<" "<<val<<endl;
        return ((choice>=1) && (choice<=6));
    }
    pair<int,int> takeValidInputCoordinates(){
        int x,y;
        do{
            cout<<"enter Co-ordinates"<<endl;
            cin>>x>>y;
        }while(!isValidDimension(x,y));
        return {x-1,y-1};
    }
    bool cellWeight(){
        pair<int,int> point = takeValidInputCoordinates();
        int weight;
        while(true){
            cin>>weight;
            if(isValidWeight(weight)) break;
        }
        board->changeWeight(point.first,point.second,weight);
        return true;
    }
    bool exit(){
        cout<<"THANK YOU FOR PLAYING"<<endl;
        return false;
    }
    bool unblock(){
        pair<int,int> point = takeValidInputCoordinates();
        board->unblockCell(point.first,point.second);
        board->print();
        return true;
    }
    bool block(){
        pair<int,int> point = takeValidInputCoordinates();
        board->blockCell(point.first,point.second);
        board->print();
        return true;
    }
    
    bool findDistance(){
        cout<<"Enter source coordinates"<<endl;
        pair<int,int> src = takeValidInputCoordinates();
        cout<<"Enter destination coordinates"<<endl;
        pair<int,int> dstn = takeValidInputCoordinates();
        pair<int,string> path = board->getShortestDistance(src,dstn);
        cout<<"path is:-"<<endl;
        cout<<path.second<<"distance: "<<path.first<<endl;
        return true;
    }
    bool choiceFxn(int choice){
        if(choice == 0){
            return findDistance();
        }
        else if(choice == 1){
            return unblock();
        }
        else if(choice == 2){
            return block();
        }
        else if(choice == 3){
            return cellWeight();
        }
        else if(choice == 4){
            board->print();
            return true;
        }
        return exit();
    }
    public:
    Play(){
        Printinstructions();
        int x,y;
        do{
            cout<<"Enter valid dimensions"<<endl;
            cin>>x>>y;
            this->x = x;
            this->y = y;
        }while(!isValidDimension(x,y));
        board = new Board(x,y);
        board->recusriveDivision();
        board->print();
    }
    void play(){
        bool flag = true;
        while(flag){
            printMenu();
            int choice;
            while(true){
                cout<<"Enter valid choice"<<endl;
                cin>>choice;
                if(isValidChoice(choice)) break;
            }
            flag = choiceFxn(choice-1);
        }
    }
};
void play(){
    Play p;
    p.play();
}
int main(){
    play();
    return 0;
}
