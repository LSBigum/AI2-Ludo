#include "ludo_player.h"
#include <random>

#define MAXBUFSIZE  ((int) 1e6)

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
        int nrows = 8;
        int ncols = 11;
        std::vector<std::vector<double>> qLearningTable(nrows,std::vector<double>(ncols));
        std::ifstream fin("../trainQ.txt");
        if (fin.is_open())
        {
            for (int row = 0; row < nrows; row++)
                for (int col = 0; col < ncols; col++)
                {
                    float item = 0.0;
                    fin >> item;
                    qLearningTable[row][col] = item;
                }
            fin.close();
        }
    }
    else
    {
        int nrows = 8;
        int ncols = 11;
        std::vector<std::vector<double>> qLearningTable(nrows,std::vector<double>(ncols, 0));
        qLearningTable[0][0] = 70;
        qLearningTable[7][0] = 70;
        
        std::ofstream current_table("../trainQ.txt");
        // current_table << qLearningTable;

        for (int i = 0; i < qLearningTable.size(); i++)
        {
            std::vector<double> tmp = qLearningTable[i];
            
            for(int j = 0; j < tmp.size(); j++)
            {
                current_table << tmp[j] << " ";
            }
            current_table << std::endl;
        }
    }
    update = false;
    std::cout << "qLearningTable initiliazed!" << std::endl;

}

// std::vector<std::vector<double>> ludo_player::loadQTable()
// {
//     std::vector<std::vector<double>> qTable;
//     std::ifstream input_file("../trainQ.txt");
//     input_file >> qTable;
//     input_file.close();
//     return qTable;
// }

std::vector<std::vector<double>> ludo_player::loadQTable(const char *filename)
{
    // int cols = 0, rows = 0;
    // double buff[MAXBUFSIZE];

    // // Read numbers from file into buffer.
    // std::ifstream infile;
    // infile.open(filename);
    // while (! infile.eof())
    //     {
    //     std::string line;
    //     getline(infile, line);

    //     int temp_cols = 0;
    //     std::stringstream stream(line);
    //     while(! stream.eof())
    //         stream >> buff[cols*rows+temp_cols++];

    //     if (temp_cols == 0)
    //         continue;

    //     if (cols == 0)
    //         cols = temp_cols;

    //     rows++;
    //     }

    // infile.close();

    // // rows--;
    
    // // Populate matrix with numbers.
    // std::vector<std::vector<double>> result[rows][cols];
    // for (int i = 0; i < rows; i++)
    //     for (int j = 0; j < cols; j++)
    //     {
    //         result[i][j] = buff[ cols*i+j ];
    //         std::cout << "i: " << i << ", j: " << j << std::endl;
    //     }

    // std::cout << result << std::endl;
    // return result;
}

void ludo_player::saveQTable(std::vector<std::vector<double>> &qTable)
{
    // std::stringstream ss;
    // std::ofstream outputFile("../trainQ.txt");
    // outputFile << qTable;
}

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
    
    return actions;
}

int ludo_player::selectAction(std::vector<std::vector<double>> qTable,
    std::vector<int> states, std::vector<int> possible_actions)
{
    int best_action = 0;
    if (EXPLORE_RATE == 0 || (double)(rand() % 1000) / 1000.0 > EXPLORE_RATE) 
    {
        double max_q = -10000;
        for (int i = 0; i < 4; i++)
        {
            // If token can't do anything, check next token
            if (pos_start_of_turn[i] > 55 || (pos_start_of_turn[i] == -1 && dice_roll != 6))
                continue;

            if (qTable[possible_actions[i]][states[i]] > max_q && possible_actions[i] != -1) 
            {
                max_q = qTable[possible_actions[i]][states[i]];
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
        // Choose a token to do a random action with
        while (true) 
        {
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

void ludo_player::getReward(std::vector<std::vector<double>> &qTable,
    int action, int state, int decision)
{
    double total_diff = 0;
    static int previous_state = 0;
    static int previous_action = 0;
    static int games_played = 0;

    double reward = 0;

    // Move out of home
    if (previous_action == 0)
        reward += 70;

    // Move piece closest to home
    if (previous_action != 0 && previous_action != 1 && previous_action != 4) {
        bool closest = true;
        for (int i = 0; i < 4; i++) {
            if (pos_end_of_turn[decision] < pos_end_of_turn[i] &&
                decision != i && pos_end_of_turn[i] != 99) {
                    closest = false;
            }
        }
        if (closest && pos_end_of_turn[decision] < 51)
            reward += 0.1;
    }
    // Kill
    if (previous_action == 3)
        reward += 0.15;

    // Form Blockade
    if (previous_action == 5)
        reward += 0.05;

    // Protect token
    if (previous_action == 6)
        reward += 0.2;

    // Move into goal
    if (previous_action == 7)
        reward += 0.25;

    // Suicide
    if (previous_action == 4)
        reward -= 0.8;

    bool game_won = true;
    for (int i = 0; i < 4; i++) {
        // Winning the game
        if(pos_end_of_turn[i] != 99)
            game_won = false;

        // Getting a token knocked home
        if (pos_start_of_turn[i] == -1 && pos_end_of_turn[i] != -1)
            reward -= 0.25;
    }

    if (game_won) {
        if (EXPLORE_RATE > 0)
            EXPLORE_RATE -= EXPLORE_RATE_DECAY;
        games_played++;
        reward += 1;
    } else {
        // Losing the game
        for (int i = 4; i < 16; i++) {
            if(pos_end_of_turn[i] == 99 && pos_start_of_turn[i] == -1) {
                if (EXPLORE_RATE > 0)
                    EXPLORE_RATE -= EXPLORE_RATE_DECAY;
                games_played++;
                reward -= 1;
                break;
            }
        }
    }

    if (EXPLORE_RATE < 0)
        EXPLORE_RATE = 0;


    // Update q-table
    if (reward != 0) {
        qTable[previous_action][previous_state] += LEARNING_RATE *
            (reward + DISCOUNT_FACTOR * qTable[action][state]
             - qTable[previous_action][previous_state]);
    }
    previous_state = state;
    previous_action = action;
    // static bool game_saved = false;
    // if (games_played == iterations - 1 && !game_saved) 
    {
        saveQTable(qTable);
        // game_saved = true;
    }
}

int ludo_player::make_decision(){
    // std::cout << "Dice: " << dice_roll << std::endl;
    // for (int i = 0; i < states.size(); i++)
    //     std::cout << "States: " << states[i] << std::endl;

    // std::cout << "\n\n";

    // for (int i = 0; i < actions.size(); i++)
    //     std::cout << "actions: " << actions[i] << std::endl;

    int nrows = 8;
    int ncols = 11;
    static bool first_turn = true;
    static std::vector<std::vector<double>> qTable(nrows, std::vector<double>(ncols, 0));
    // std::cout << "action: " << selectAction(qTable, states, actions) << std::endl;

    if (first_turn) {
        for (int i = 0; i < 4; i++)
            pos_end_of_turn[i] = -1;

        if (!this->training) {
            qTable = loadQTable("../trainQ.txt");
        }
        first_turn = false;
    }

    std::vector<int> states = currentStates();
    std::vector<int> possible_actions = getActions();
    int decision = selectAction(qTable, states, possible_actions);

    if (this->training) {
        getReward(qTable, possible_actions[decision], states[decision], decision);
    }

    return decision;

    // if(dice_roll == 6){
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    // } else {
    //     for(int i = 0; i < 4; ++i){
    //         if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
    //             return i;
    //         }
    //     }
    //     for(int i = 0; i < 4; ++i){ //maybe they are all locked in
    //         if(pos_start_of_turn[i]<0){
    //             return i;
    //         }
    //     }
    // }
    // return -1;

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
