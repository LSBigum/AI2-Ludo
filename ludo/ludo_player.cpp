// Some code segments are inspired by ztaal

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
    if(fileExists("../qTable.txt")) // Continue from previous
    {
        int nrows = 8;
        int ncols = 11;
        // qTable = Eigen::MatrixXd(nrows,ncols);
        Eigen::MatrixXd qTable(nrows,ncols);
        std::ifstream fin("../qTable.txt");
        if (fin.is_open())
        {
            for (int row = 0; row < nrows; row++)
                for (int col = 0; col < ncols; col++)
                {
                    float item = 0.0;
                    fin >> item;
                    qTable(row, col) = item;
                }
            fin.close();
        }
    }
    else
    {
        int nrows = 8;
        int ncols = 11;
        // qTable = Eigen::MatrixXd::Zero(nrows,ncols);
        Eigen::MatrixXd qTable(nrows,ncols);
        qTable(0,0) = 70;
        qTable(1,1) = 5;
        qTable(1,3) = 10;
        qTable(1,10) = 5;
        qTable(3,1) = 90;
        qTable(3,2) = 80;
        qTable(3,3) = 100;
        qTable(3,4) = 50;
        qTable(3,5) = 60;
        qTable(3,6) = 50;
        qTable(3,7) = 20;
        qTable(3,8) = 5;
        qTable(3,9) = -150;
        qTable(4,1) = 90;
        qTable(4,2) = 80;
        qTable(4,4) = 50;
        qTable(4,5) = 60;
        qTable(4,7) = 20;
        qTable(4,8) = 5;
        qTable(4,9) = -150;
        qTable(5,2) = 80;
        qTable(5,4) = 60;
        qTable(5,5) = 60;
        qTable(5,8) = 60;
        qTable(5,9) = -150;
        qTable(6,1) = 100;
        qTable(6,2) = 80;
        qTable(6,4) = 50;
        qTable(6,5) = 60;
        qTable(6,6) = 50;
        qTable(6,7) = 20;
        qTable(6,8) = 5;
        qTable(6,9) = -150;
        qTable(7,1) = 90;
        qTable(7,2) = 80;
        qTable(7,3) = 100;
        qTable(7,4) = 50;
        qTable(7,5) = 60;
        qTable(7,6) = 50;
        qTable(7,7) = 20;
        qTable(7,8) = 5;
        qTable(7,9) = -150;


        
        
        std::ofstream current_table("../qTable.txt");
        current_table << qTable;
    }
    update = false;
}

Eigen::MatrixXd ludo_player::loadQTable()
{

    int nrows = 8;
    int ncols = 11;
    // qTable = Eigen::MatrixXd(nrows,ncols);
    Eigen::MatrixXd qTable(nrows, ncols);
    std::ifstream fin("../qTable.txt");
    if (fin.is_open())
    {
        for (int row = 0; row < nrows; row++)
            for (int col = 0; col < ncols; col++)
            {
                float item = 0.0;
                fin >> item;
                qTable(row, col) = item;
            }
        fin.close();
    }
    return qTable;
}

void ludo_player::saveQTable(Eigen::MatrixXd &qTable)
{
    std::ofstream current_table("qTable.txt");
    current_table << qTable << std::endl;
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
                    // if ((pos_start_of_turn[j] - 8) % 13 == 0 || (pos_start_of_turn[j]) % 13 == 0) { 
                    if ((pos_start_of_turn[j] - 8) % 13 == 0 || (pos_start_of_turn[j] - 8) % 18 == 0) {
                        suicide = true;
                        kill = false;
                        break;
                    }
                    // Check if enemy has formed a blockade (Suicide)
                    for (int k = 0; k < 4; k++) {
                        int index = int(j/4) * k;
                        if (new_pos == pos_start_of_turn[index] && index != j) {
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

int ludo_player::selectAction(Eigen::MatrixXd qTable, std::vector<int> states, std::vector<int> actions)
{
    int best_action = -1;
    if (epsilon == 0 || (double)(rand() % 1000) / 1000.0 > epsilon) 
    {
        double max_q = -10000;
        for (int i = 0; i < 4; i++)
        {
            if (pos_start_of_turn[i] > 55 || (pos_start_of_turn[i] == -1 && dice_roll != 6))
                continue;

            if (qTable( states[i], actions[i] ) > max_q && actions[i] != -1) 
            {
                max_q = qTable(states[i], actions[i]);
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

    while(states[best_action] == 2)
    {
        best_action++;
        best_action = best_action % 4;
    }

    return best_action;
}

void ludo_player::getReward(Eigen::MatrixXd &qTable, int action, int state, int decision)
{
    static int previousState = -1;
    static int previousAction = -1;
    
    double reward = 0;

    // Move out of home
    if (previousAction == 0)
        reward += 70;

    // Move piece closest to home
    if (previousAction != 4) {
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
    if (previousAction == 2)
        reward += 80;

    // Goal
    if (previousAction == 3)
        reward += 100;

    // Own blockade
    if (previousAction == 4)
        reward += 50;

    // Star
    if (previousAction == 5)
        reward += 60;

    // Safe globe
    if (previousAction == 6)
        reward += 50;

    // Risky globe
    if (previousAction == 7)
        reward += 20;

    // Neutral spot
    if (previousAction == 8)
        reward += 5;

    // Suicide
    if (previousAction == 9)
        reward -= 150;

    // Move around in goal stretch
    if (previousAction == 10)
        reward += 5;

    bool wonGame = true;
    for (int i = 0; i < 4; i++) 
    {
        // Winning the game
        if(pos_end_of_turn[i] != 99)
            wonGame = false;

    }

    // Needs further work 

    // Update qTable
    if (reward != 0) {
        qTable(previousAction, previousState) += alpha *
            (reward + gamma * qTable(action, state)
             - qTable(previousAction, previousState));
    }
    previousState = state;
    previousAction = action;
    saveQTable(qTable);
}

int ludo_player::make_decision()
{
    static Eigen::MatrixXd qTable(8, 11);
    qTable.setZero();

    qTable = loadQTable();

    std::vector<int> states = currentStates();
    std::vector<int> actions = getActions();
    int decision = selectAction(qTable, states, actions);

    if (this->training) 
        getReward(qTable, actions[decision], states[decision], decision);

    return decision;

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