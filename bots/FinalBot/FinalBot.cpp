/*
* @author Kunal Grover
* @date 2013-12-30
* Othello bot
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <time.h>
#include <cstdlib>
#define DEPTH 9

using namespace std;
using namespace Desdemona;
    
Move bestMove=Move(0,0);
unsigned long start;

int score_gen(const OthelloBoard&,Turn) ;
int alphabeta(const OthelloBoard&,int ,int ,int ,Turn ,Turn);

class MyBot: public OthelloPlayer
{
    public:
        MyBot( Turn turn );
        virtual Move play( const OthelloBoard& board );
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

Move MyBot::play( const OthelloBoard& board )
{
   start = clock();
   cout<<"My turn\n";
   board.print(turn);
   while(double(clock() - start)/CLOCKS_PER_SEC<2);
   start = clock();
   alphabeta(board,DEPTH,-1000,1000,turn,turn);
   return bestMove;
}



int score_gen(const OthelloBoard& board,Turn turn) 
{
  int r_score = board.getRedCount();
  int b_score = board.getBlackCount();
  if(turn == RED) 
    {
      if (b_score == 0) 
        {
	  return 100;
	}
      if(r_score == 0)
	{
	    return -100;
	}
      return r_score-b_score;
    }
  else   
    {
      if (r_score == 0) 
        {
	  return 100;
	}
      if(b_score == 0)
	{
	    return -100;
	}
      return b_score-r_score;
    }
}

int alphabeta(const OthelloBoard&  board,int depth,int alpha,int beta,Turn turn ,Turn max_player)
{
  Turn next_turn = other(turn);
  int k;
  if (depth == 0)
    {
      return score_gen(board,max_player);
    }
  if(board.getValidMoves( turn ).empty() && board.getValidMoves( other( turn ) ).empty())
  {
     return score_gen(board,max_player);
  }
  list<Move> Moves = board.getValidMoves(turn);
  int temp;
  OthelloBoard temp_board ;
  	    
  if (turn == max_player)
    { 
       if(!Moves.empty())
       {
         list<Move>::iterator it = Moves.begin();
         if(depth == DEPTH) 
         {
           bestMove = Move(*it);
         }
         for( ; it!=Moves.end() ; it++)   
	 {
	   temp_board = board;
	   temp_board.makeMove(turn,*it);
     /*      if(distance(it,Moves.end())<DEPTH && depth == DEPTH) 
	   {
         	k = DEPTH - distance(it,Moves.end());
	   }
	   else 
           {
                k=0;
           }*/
           if(double(clock() - start)/CLOCKS_PER_SEC>4.98)
           {
//		cout<<"broke";               
		break;
	   }
           temp = alphabeta(temp_board, depth - 1 , alpha, beta, next_turn, max_player);
           if(alpha < temp)
	    {
              alpha = temp;
              if(depth == DEPTH) {
                  bestMove = Move(*it);
              }
            }           
	    if (beta<=alpha)
	      { 
		 break;
	      }
	}
       return alpha;
       }
   }
    
   else
     {
        if(!Moves.empty())
        {
	for( list<Move>::iterator it = Moves.begin() ; it!=Moves.end() ; it++)   
	  {
	    temp_board = board;
            temp_board.makeMove(turn,*it);
            if(double(clock() - start)/CLOCKS_PER_SEC>4.98)
            {             
//	       cout<<"broke"; 
               break;
	    }
	    temp = alphabeta(temp_board, depth - 1, alpha, beta, next_turn, max_player);
	    if( beta > temp) 
              {
                beta = temp;
              }
            if (beta<=alpha)
                { 
		   break;
		}
          }
	  return beta;
	 }
      }
}


// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}



