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
    void saveQTable(Eigen::MatrixXd &qTable);
    Eigen::MatrixXd loadQTable();

    bool training = false;
    double epsilon = 0;
    double gamma = 0.3;
    double alpha = 0.7;

    // Eigen::MatrixXd qTable;
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
    bool fileExists(const std::string &filename);
    bool update;
    void receiveReward();
    
    std::vector<int> currentStates();
    void getReward(Eigen::MatrixXd &qTable, int action, int state, int decision);

    std::vector<int> getActions();
    int selectAction(Eigen::MatrixXd qTable, std::vector<int> states, std::vector<int> possibleActions);

    int posIfMovingBack = 0; // For checking if the token moved past goal and therefore back in goal stretch

public:
    ludo_player();

signals:
    void select_piece(int);
    void turn_complete(bool);
public slots: 
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_H