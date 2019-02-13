#include "maze.h"
#include "path.h"
#include<queue>
#include<vector>
#include<list>
#include<tuple>
#include<utility>
#include<iostream>
#include<climits>
#include<sstream>
#include<limits.h>
#include<algorithm>
#include<stdlib.h>

using namespace std;

path solve_dfs(Maze& m, int rows, int cols);
path solve_bfs(Maze& m, int rows, int cols);
path solve_dijkstra(Maze& m, int rows, int cols);
path solve_tour(Maze& m, int rows, int cols);


int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cerr << "usage:\n"
             << "./maze option rows cols\n"
             << " options:\n"
             << "  -dfs: depth first search (backtracking)\n"
             << "  -bfs: breadth first search\n"
             << "  -dij: dijkstra's algorithm\n"
             << "  -tour: all corners tour\n"
             << "  -basic: run dfs, bfs, and dij\n"
             << "  -advanced: run dfs, bfs, dij and tour" << endl;
        return 0;
    }
    string opt(argv[1]);

    int rows, cols;
    stringstream s;
    s << argv[2] << " " << argv[3];
    s >> rows >> cols;

    // construct a new random maze;
    Maze m(rows, cols);

    // print the initial maze out
    cout << "Initial maze" << endl;
    m.print_maze(cout, opt == "-dij" || opt == "-tour");

    if(opt == "-dfs")
    {
        cout << "\nSolved dfs" << endl;
        path p = solve_dfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);
    }


    if(opt == "-bfs")
    {
        cout << "\nSolved bfs" << endl;
        path p = solve_bfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);
    }

    if(opt == "-dij")
    {
        cout << "\nSolved dijkstra" << endl;
        path p = solve_dijkstra(m, rows, cols);
        m.print_maze_with_path(cout, p, true, false);
    }

    if(opt == "-tour")
    {
        cout << "\nSolved all courners tour" << endl;
        path p = solve_tour(m, rows, cols);
        m.print_maze_with_path(cout, p, true, true);
    }
    if(opt == "-basic")
    {
        cout << "\nSolved dfs" << endl;
        path p = solve_dfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);

        cout << "\nSolved bfs" << endl;
        p = solve_bfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);

        cout << "\nSolved dijkstra" << endl;
        p = solve_dijkstra(m, rows, cols);
        m.print_maze_with_path(cout, p, true, false);
    }
    if(opt == "-advanced")
    {
        cout << "\nSolved dfs" << endl;
        path p = solve_dfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);

        cout << "\nSolved bfs" << endl;
        p = solve_bfs(m, rows, cols);
        m.print_maze_with_path(cout, p, false, false);

        cout << "\nSolved dijkstra" << endl;
        p = solve_dijkstra(m, rows, cols);
        m.print_maze_with_path(cout, p, true, false);

        cout << "\nSolved all courners tour" << endl;
        p = solve_tour(m, rows, cols);
        m.print_maze_with_path(cout, p, true, true);
    }
}
//** Function to print the path for trouble shooting
void printPath(path return_path)
{
	int x, y;	
	list<point>::iterator i;
	for(i = return_path.begin(); i != return_path.end(); ++i)	
	{
		x = i->first;
		y = i->second;
	
		cout << x << " x: y " << y << endl;	
	}	
}

//** DFS helper to run the function recursively as the path grows to the next square
//Function takes a point, p, as starting position in maze and continues in a right->down->left->up pattern
//until the exit at (m.columns()-1, m.row()-1) is found
bool dfs_helper(Maze m, point p, path& ret, vector<bool>& visited)
{
	ret.push_back(p);//start the solution path at argument starting point p
	int y = p.first;//put point p in terms of x and y for location parameters
	int x = p.second;
	int r = m.rows();
	int c = m.columns();

	if(visited[y*c+x] == true)//If current point has been visited then remove from path
	{
		ret.pop_back();
		return false;
	}
	else 
	{
		visited[y*c+x] = true;//Mark current point as visited to keep from maze looping 
		
		if(x == (c-1) && y == (r-1))//If finished, stop
		{
			return true;
		}	
			//If direction possible, go in that direction by calling function with next point  
		if( (x<c && m.can_go_right(y,x)) && dfs_helper(m,  p+moveIn(RIGHT), ret, visited))
		{
			return true;
		}
		if( (y<r) && m.can_go_down(y,x) && dfs_helper(m, p+moveIn(DOWN), ret, visited))
		{
			return true;
		}
		if( x>0 && m.can_go_left(y,x) && dfs_helper(m, p+moveIn(LEFT), ret, visited))
		{
			return true;
		}
		if( y>0 && m.can_go_up(y,x) && dfs_helper(m, p+moveIn(UP), ret, visited))
		{
			return true;
		}
		else
		{//If no direction is possible, remove current point from path
			ret.pop_back();
			return false;
		}
	}
}
//Wrapper function for DFS which calls helper function with starting point (0,0)
path solve_dfs(Maze& m, int rows, int cols)
{
	vector<bool> visited(rows*cols, false);//Vector of bools to check if a point has been visited
	path return_path;
	point add_point = make_pair(0,0);

	dfs_helper(m, add_point, return_path, visited);//Recursive helper function
	cout << "Size of path: " << return_path.size() << endl;
	return return_path;
}

//Helper function to contain main BFS function 
int bfs_helper(Maze m, point p, path& casa, vector<bool>& visited)
{
	int y = p.first;//Starting point p broken into x,y coordinates
	int x = p.second;
	int r = m.rows();//Rows and columns set as variables for quicker access
	int c = m.columns();
	path ret;//Path to cycle thru while gathering path points
	ret.emplace_front(p);//Seed cycle path with starting point p

	list<point>::iterator i;

	//Continue adding points to the cycle path until end is found
	//mark each point visited when it is added to the return path, casa, 
	//and pop the current point when it has already been visited 
	for( i = ret.begin(); i != ret.end(); ++i)
	{
		y = i->first;
		x = i->second;
		
		if(x == c-1 && y == r-1)//if current point is the exit, add point to path and break loop
		{
			casa.push_back(*i);
			break;
		}

		//If current point has not been visited, mark visited true and add to path
		//then check every direction and add possibles to cycle path
		if(!visited[y*c + x])
		{
				
			casa.push_back(*i);			
			visited[y*c + x] = true;
			
			if(x<c && m.can_go_right(y,x) && !visited[y*c+x+1])
			{
				ret.push_back(*i+moveIn(RIGHT));
			}
			
			if(y<r && m.can_go_down(y,x) && !visited[(y+1)*c+x])
			{	
				ret.push_back(*i+moveIn(DOWN));
			}
			
			if(x>0 && m.can_go_left(y,x) && !visited[y*c+x-1])
			{	
				ret.push_back(*i+moveIn(LEFT));
			}
			
			if(y>0 && m.can_go_up(y,x) && !visited[(y-1)*c+x])			
			{	
				ret.push_back(*i+moveIn(UP));
			}
		}
		else
		{
			ret.pop_front();
		}
	//	ret.pop_front();
	}	
}
//Attempted to backtrace the path from the BFS to eleminate unnecessary points on path
//Unable to determine source of fault
//
/*path directPath(path& casa)
{
	point temp;
	path retCasa;
	list<point>::iterator i; 
	
	for(i = casa.end(); i != casa.begin(); --i)
	{
		temp = *i;
		casa.pop_back();
		if( ( temp.first == i->first && abs(temp.second-(i->second)) == 1 ) || (abs(temp.first-(i->first)) == 1 && temp.second==i->second) )
			retCasa.push_front(temp);
		else
		{
			casa.pop_back();
			casa.push_back(temp);
		}
		
			
	}
	return retCasa;

}*/


path solve_bfs(Maze& m, int rows, int cols)
{
 	vector<bool> visited(rows*cols, false);
	path return_path;
	path p;
	point add_point = make_pair(0,0);

	bfs_helper(m, add_point, return_path, visited);
	cout << "Size of path: " << return_path.size() << endl;
//	printPath(return_path);
//	return_path = directPath(return_path);
//	printPath(return_path);

	return return_path;
}
//Attempted to determine with neighbor point had the min cost from current point
/*
int min(int x, int y, Maze m, vector<bool> visited)
{
	int r = m.rows();
	int c = m.columns();
	int costR = 11, costL = 11, costU = 11, costD = 11;
	vector<int> order;
	priority_queue <int, vector<int>, greater<int> > minHeap;

	

	if( (x < c) && m.can_go_right(y,x)) //&& dijkstra_helper(m, p+moveIn(RIGHT), casa, visited) )
		minHeap.push( m.cost(y, x, RIGHT));
	
	if( (y < r) && m.can_go_down(y,x))
		minHeap.push( m.cost(y, x, DOWN));
	
	if( (x > 0) && m.can_go_left(y,x))
		minHeap.push( m.cost(y, x, LEFT));
		
	if( (y > 0) && m.can_go_up(y,x))
		minHeap.push( m.cost(y, x, UP));

//	cout << costR << " R " << costL << " L " << costU << " U " << costD << " D " << endl;

	int cst = -1; 
	cst = minHead.top();
 
	if(cst == costL && x>0 && !visited[y*c+x-1])  
		return 1;
	else if(cst == costU && y>0 && !visited[(y-1)*c+x] )
		return 0;
	else if(cst == costD && y<r && !visited[(y+1)*c+x] )
		return 2;
	else if(cst == costR && x<c && !visited[y*c+x+1] )
		return 3;	    
	else
		return 5;
}

int dijkstra_helper(Maze m, point p, path& casa, vector<bool>& visited)
{
	int x = p.first;
	int y = p.second;
	int r = m.rows();
	int c = m.columns();
	path ;
	ret.push_back(p);

	list<point>::iterator i;

	for( i = ret.begin(); i != ret.end(); ++i)
	{
		x = i->first;
		y = i->second;
		
		if(x == c-1 && y == r-1)
		{
			casa.push_back(*i);
			break;
		}

		if(!visited[y*c + x])
		{	
			casa.push_back(*i);			
			visited[y*c + x] = true;
			
			if(x< c && m.can_go_right(x,y))// && !visited[(y)*c+x+1])
			{
				ret.push_back(*i+moveIn(RIGHT));
			}
			
			if(y < r && m.can_go_down(x,y))// && !visited[(y+1)*c+x])
			{
				ret.push_back(*i+moveIn(DOWN));
			}
			
			if(x>0 && m.can_go_left(x,y))// && !visited[y*c+x-1])
			{
				ret.push_back(*i+moveIn(LEFT));
			}
			
			if(y>0 && m.can_go_up(x,y))// && !visited[(y-1)*c+x])
			{
				ret.push_back(*i+moveIn(UP));
			}
		}
		else
			ret.pop_front();
	}	
}
*/

//** Dijkstra helper is same as DFS to run the function recursively as the path grows to the next square
//Function takes a point, p, as starting position in maze and continues in a right->down->left->up pattern
//until the exit at (m.columns()-1, m.row()-1) is found
bool dijkstra_helper(Maze m, point p, path& ret, vector<bool>& visited)
{
	ret.push_back(p);//start the solution path at argument starting point p
	int y = p.first;//put point p in terms of x and y for location parameters
	int x = p.second;
	int r = m.rows();
	int c = m.columns();

	if(visited[y*c+x] == true)//If current point has been visited then remove from path
	{
		ret.pop_back();
		return false;
	}
	else 
	{
		visited[y*c+x] = true;//Mark current point as visited to keep from maze looping 
		
		if(x == (c-1) && y == (r-1))//If finished, stop
		{
			return true;
		}	
			//If direction possible, go in that direction by calling function with next point  
		if( (x<c && m.can_go_right(y,x)) && dfs_helper(m,  p+moveIn(RIGHT), ret, visited))
		{
			return true;
		}
		if( (y<r) && m.can_go_down(y,x) && dfs_helper(m, p+moveIn(DOWN), ret, visited))
		{
			return true;
		}
		if( x>0 && m.can_go_left(y,x) && dfs_helper(m, p+moveIn(LEFT), ret, visited))
		{
			return true;
		}
		if( y>0 && m.can_go_up(y,x) && dfs_helper(m, p+moveIn(UP), ret, visited))
		{
			return true;
		}
		else
		{//If no direction is possible, remove current point from path
			ret.pop_back();
			return false;
		}
	}
}

path solve_dijkstra(Maze& m, int rows, int cols)
{
	vector<bool> visited(rows*cols, false);
	path return_path;
	point add_point = make_pair(0,0);

	dijkstra_helper(m, add_point, return_path, visited);
	cout << "Size of path: " << return_path.size() << endl;

	return return_path;
}
//Did not have enough time to implement the tour
path solve_tour(Maze& m, int rows, int cols)
{
    return list<point>();
}
