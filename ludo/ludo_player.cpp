#include "ludo_player.h"
#include <random>

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
        int nrows = 8;
        int ncols = 11;
        qLearningTable = Eigen::MatrixXd::Zero(nrows,ncols);
        qLearningTable(0,0) = 70;
        
        std::ofstream current_table("../trainQ.txt");
        current_table << qLearningTable;
    }
    update = false;
    std::cout << "qLearningTable initiliazed!" << std::endl;

}



// void ludo_player::saveQTable(std::vector<std::vector<double>> &qTable, std::string filename)
// {

// }

std::vector<int> ludo_player::currentStates()
{
    std::vector<int> states(4, -1);

    for (int i = 0; i < 4; i++) 
    {
        int state = -1;
        int start_pos = pos_start_of_turn[i];

        // Home
        if (start_pos == -1) 
            state = 0;

        // Goal stretch
        if (start_pos > 50 && start_pos < 56)
            state = 1;
        
        // Goal
        if ((start_pos >= 56 && start_pos < 72) || start_pos == 99)
            state = 2;

        // Star
        if (start_pos == 5 || start_pos == 11 || start_pos == 18 || start_pos == 24 || start_pos == 31 || start_pos == 37 || start_pos == 44)
            state = 3;

        // Safe globe
        if ((start_pos < 50) && ((start_pos - 8) % 13 == 0 || start_pos == 0))
            state = 4;

        // Risky globe
        if (start_pos % 13 == 0 && start_pos > 0)
            state = 5;

        // Neutral
        if (state == -1 && start_pos != -1 && start_pos != 99)
            state = 6;

        // Own block and neutral
        for (int j = 0; j < 4; j++)
        {
            if (state == 6 && start_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99)
            {
                state = 7;
                break;
            }
        }

        states[i] = state;
    }

    return states;
}

std::vector<int> ludo_player::getActions()
{
    std::vector<int> actions;
    for (int i = 0; i < 4; i++) {
        int action = -1;
        int start_pos = pos_start_of_turn[i];
        int new_pos = start_pos + dice_roll;
        // std::cout << "i: " << i << ", pos: " << new_pos << ", startpos: " << start_pos << ", dice: " << dice_roll << std::endl;
        
        // Move out of home
        if (start_pos == -1 && dice_roll == 6) 
        {
            action = 0;
        } 
        else if (start_pos != -1 && start_pos != 99) // Check if token is stuck in home or is in goal
        {
            posIfMovingBack = 0;
            if(new_pos > 56 && new_pos < 72) // Check if token is moving past goal and back
                posIfMovingBack = 56 - (new_pos - 56);

            // Move into goal stretch
            if (new_pos > 50 && new_pos < 56) 
            {
                action = 1;
                // Moving around in goal stretch
                if (start_pos > 50 && start_pos < 56) 
                    action = 10;
            }

            // Move to goal (56) or star -> goal (50)
            if (new_pos == 56 || new_pos == 50) 
                action = 3;   

            // Star
            if(new_pos == 5 || new_pos == 11 || new_pos == 18 || new_pos == 24 || new_pos == 31 || new_pos == 37 || new_pos == 44) 
                action = 5;

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
                        // std::cout << "if " << new_pos << " == " << pos_start_of_turn[index] << " && " << index << " != " << j << std::endl;
                        if (new_pos == pos_start_of_turn[index] && index != j) {
                            // std::cout << "Suicide --------- " << std::endl;
                            suicide = true;
                            kill = false;
                            break;
                        }
                    }
                }
            }

            if (suicide) // Suicide
                action = 9;
            else if (kill && !suicide) // Kill
                action = 2;

            // Form own blockade
            for (int j = 0; j < 4; j++) 
            {
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99)
                {
                    action = 4;
                    break;
                }
            }

            if ((new_pos - 8) % 13 == 0) // Safe globe
                action = 6;

            if (new_pos % 13 == 0) // Risky globe
                action = 7;

            if (action == -1 && new_pos > start_pos) // Neutral
                action = 8;
        }
        actions.push_back(action);
    }
    
    // for(int i = 0; i < actions.size(); i++)
    // {
    //     std::cout << "action: " << actions[i] << ", dice roll: " << dice_roll << std::endl;
    // }
    // std::cout << std::endl;
    
    return actions;
}

int ludo_player::selectAction(Eigen::MatrixXd qTable,
    std::vector<int> states, std::vector<int> possible_actions)
{
    int best_action = 0;
    if (EXPLORE_RATE == 0 || (double)(rand() % 1000) / 1000.0 > EXPLORE_RATE) 
    {
        double max_q = -10000;
        for (int i = 0; i < 4; i++)
        {
            if (pos_start_of_turn[i] > 55 || (pos_start_of_turn[i] == -1 && dice_roll != 6))
                continue;

            if (qTable( possible_actions[i], states[i] ) > max_q && possible_actions[i] != 0) 
            {
                max_q = qTable(possible_actions[i], states[i]);
                best_action = i;
            }
        }
    // Random action
    } 
    else 
    {
        bool token_out_of_home = false;
        for (int i = 0; i < 4; i++) 
        {
            if (pos_start_of_turn[i] != -1 && pos_start_of_turn[i] != 99) 
            {
                token_out_of_home = true;
                break;
            }
        }
        while (true) {
            best_action = rand() % 4;
            if (pos_start_of_turn[best_action] < 56) 
            {
                if (pos_start_of_turn[best_action] != -1 && token_out_of_home) 
                {
                    break;
                } 
                else if (!token_out_of_home) 
                {
                    break;
                }
            }
        }
    }
    // Make sure that best_action is not moving a token in goal
    while(pos_start_of_turn[best_action] == 99)
    {
        best_action++;
        best_action = best_action % 4;
    }

    return best_action;
}

int ludo_player::make_decision(){
    // selectAction();
    std::vector<int> states = currentStates();
    std::vector<int> actions = getActions();

    std::cout << "Dice: " << dice_roll << std::endl;
    for (int i = 0; i < states.size(); i++)
        std::cout << "States: " << states[i] << std::endl;

    std::cout << "\n\n";

    // for (int i = 0; i < actions.size(); i++)
    //     std::cout << "actions: " << actions[i] << std::endl;


    static bool first_turn = true;
    static Eigen::MatrixXd qTable(7, 11);
    qTable.setZero();
    // std::cout << "action: " << selectAction(qTable, states, actions) << std::endl;

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

    std::vector<int> possible_actions = getActions();




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
