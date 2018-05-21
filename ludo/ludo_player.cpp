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

ludo_player::ludo_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
    std::cout << "q-learning" << std::endl;
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
        int nrows = 7;
        int ncols = 13;
        qLearningTable = Eigen::MatrixXd::Zero(nrows,ncols);
        qLearningTable(6,12) = 70;
        
        std::ofstream current_table("../trainQ.txt");
        current_table << qLearningTable;
    }
    update = false;
    std::cout << "qLearningTable initiliazed!" << std::endl;

}

// int ludo_player::isOccupied(int index){ //returns number of people of another color
//     int number_of_people = 0;

//     if(index != 99){
//         for(size_t i = 0; i < player_positions.size(); ++i){
//             if(i < static_cast<size_t>(color)*4 || i >= static_cast<size_t>(color)*4 + 4){        //Disregard own players
//                 if(player_positions[i] == index){
//                     ++number_of_people;
//                 }
//             }
//         }
//     }
//     return number_of_people;
// }

// bool ludo_player::isGlobe(int index){
//     if(index < 52){     //check only the indexes on the board, not in the home streak
//         if(index % 13 == 0 || (index - 8) % 13 == 0 || isOccupied(index) > 1){  //if more people of the same team stand on the same spot it counts as globe
//             return true;
//         }
//     }
//     return false;
// }

// void ludo_player::saveQTable(std::vector<std::vector<double>> &qTable, std::string filename)
// {

// }

std::vector<int> ludo_player::getActions()
{
    std::vector<int> actions;
    for (int i = 0; i < 4; i++) {
        int action = -1;
        int start_pos = pos_start_of_turn[i];
        int new_pos = start_pos + dice_roll;
        // std::cout << "i: " << i << ", pos: " << new_pos << ", startpos: " << start_pos << ", dice: " << dice_roll << std::endl;
        // Move out of home
        if (start_pos == -1 && dice_roll == 6) {
            action = 0;
        // Check if token is stuck in home or is in goal
        } else if (start_pos != -1 && start_pos != 99) {
            // Move into goal stretch
            if (new_pos > 50 && new_pos < 56)
                action = 1;

            // Form blockade
            for (int j = 0; j < 4; j++) {
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99) {
                    action = 4;
                    break;
                }
            }

            // Move onto same field as an enemy
            bool kill = false;
            bool suicide = false;

            for (int j = 4; j < 16; j++) {
                if (new_pos == pos_start_of_turn[j] && start_pos != -1 && start_pos != 99) {
                    kill = true;
                    // Check if enemy token is on globe (Suicide)
                    if ((pos_start_of_turn[j] - 8) % 13 == 0 || (pos_start_of_turn[j]) % 13 == 0) {
                        suicide = true;
                        kill = false;
                        break;
                    }
                    // Check if enemy has formed a blockade (Suicide)
                    for (int k = 0; k < 4; k++) {
                        int index = int(j/4) * k;
                        if (new_pos == pos_start_of_turn[index] && index != j) {
                            suicide = true;
                            break;
                        }
                    }
                }
            }

            // Protect token (goal stretch or globe)
            if ((new_pos > 50 && start_pos < 50) ||
                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)) {
                action = 6;
            }

            if (suicide) { // Suicide
                action = 4;
            } else if (kill && !suicide) { // Kill
                action = 3;
            }
            // Move token
            if (action == 0 && new_pos > start_pos)
                action = 2;
        }
        actions.push_back(action);
    }
    
    // for(int i = 0; i < actions.size(); i++)
    // {
    //     std::cout << "actions: " << actions[i] << std::endl;
    // }
    
    return actions;
}

int ludo_player::make_decision(){
    getActions();
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
