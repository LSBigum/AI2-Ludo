#include "ludo_player.h"
#include <random>

// ludo_player::ludo_player():
//     pos_start_of_turn(16),
//     pos_end_of_turn(16),
//     dice_roll(0)
// {
// }

bool ludo_player::fileExists(const std::string &filename)
{
    return access( filename.c_str(), 0 ) == 0;
}

ludo_player::ludo_player()
{
    std::cout << "q-learning" << std::endl;
    // acc_reward_player1 = 0;
    // acc_reward_player2 = 0;
    // acc_reward_player3 = 0;
    // acc_reward_player4 = 0;
    if(fileExists("../trainQ.txt")) // Continue from previous
    {
        int nrows = 7;
        int ncols = 13;
        qLearningTable = Eigen::MatrixXd(nrows,ncols);
        std::ifstream fin("../trainQ.txt");
        if (fin.is_open())
        {
            for (int row = 0; row < nrows; row++)
                for (int col = 0; col < ncols; col++)
                {
                    float item = 0.0;
                    fin >> item;
                    qLearningTable(row, col) = item;
                }
            fin.close();
        }
    }
    else
    {
        qLearningTable = Eigen::MatrixXd::Zero(7,13);
        qLearningTable(6,12) = 70;
        
        std::ofstream current_table("../trainQ.txt");
        current_table << qLearningTable << "\n";
    }
    update = false;
    std::cout << "qLearningTable initiliazed!" << std::endl;

}

// void ludo_player::saveQTable(std::vector<std::vector<double>> &qTable, std::string filename)
// {

// }

int ludo_player::make_decision(){
    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

void ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
