//
// Created by serg on 11/15/16.
//

#ifndef TIC_TAC_TOE_GAMEMANAGER_H
#define TIC_TAC_TOE_GAMEMANAGER_H


class GameManager {
public:
    GameManager();

    GameManager &getInstance();

private:
    void _onShowGameStatus();
};


#endif //TIC_TAC_TOE_GAMEMANAGER_H
