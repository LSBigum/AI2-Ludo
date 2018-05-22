#ifndef LUDO_PLAYER_H
#define LUDO_PLAYER_H
#include <QObject>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "positions_and_dice.h"
#include <eigen3/Eigen/Dense>

class ludo_player : public QObject {
    Q_OBJECT
private:
    std::vector<std::vector<double>> loadQTable(const char *filename);
    void saveQTable(std::vector<std::vector<double>> &qTable);
    double EXPLORE_RATE = 0;
    double EXPLORE_RATE_DECAY;
    double DISCOUNT_FACTOR = 0.4;
    double LEARNING_RATE = 0.7;
    // bool training = true;
    // double EXPLORE_RATE = 0.9;
    bool training = false;

    std::vector<std::vector<double>> qLearningTable;
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
    bool fileExists(const std::string &filename);
    bool update;
    void receiveReward();

    std::vector<int> currentStates();

    std::vector<int> getActions();
    void getReward(std::vector<std::vector<double>> &qTable, int action, int state, int decision);
    int selectAction(std::vector<std::vector<double>> qTable, std::vector<int> states, std::vector<int> possible_actions);

    int posIfMovingBack = 0; // For checking if the token moved past goal and therefore back in goal stretch

public:
    ludo_player();
    // double acc_reward_player1;
    // int acc_reward_player2;
    // int acc_reward_player3;
    // int acc_reward_player4;
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots: 
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_H
