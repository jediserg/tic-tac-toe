//
// Created by serg on 11/15/16.
//

#ifndef TIC_TAC_TOE_PLAYER_H
#define TIC_TAC_TOE_PLAYER_H

#include "PlayerBase.h"
#include "Api.h"

class Player : public PlayerBase {
public:
    Player(Api::Callback response);

protected:
    virtual void _moveRequest(const Board &board) override;

private:
    Api::Callback _send_to_player;
};


#endif //TIC_TAC_TOE_PLAYER_H
