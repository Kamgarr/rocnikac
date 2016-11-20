#include "EventReceiver.h"

bool gg::EventReceiver::OnEvent(const SEvent &TEvent)
{

    if(TEvent.EventType == EET_KEY_INPUT_EVENT)
    {
        btVector3 torqueVec(0,0,0);

        switch(TEvent.KeyInput.Key)
        {
            case KEY_ESCAPE:
                game->Done = true;
            break;
            case KEY_KEY_C:
                game->CreateStartScene();
            break;
            case KEY_KEY_W:
            {
                torqueVec = btVector3(-torque,0,0);
            }
            break;
            case KEY_KEY_S:
            {
                torqueVec = btVector3(torque,0,0);
            }
            break;
            case KEY_KEY_A:
            {
                torqueVec = btVector3(0,-torque/5,0);
            }
            break;
            case KEY_KEY_D:
            {
                torqueVec = btVector3(0,torque/5,0);
            }
            break;
            case KEY_KEY_Q:
            {
                torqueVec = btVector3(0,0,-torque);
            }
            break;
            case KEY_KEY_E:
            {
                torqueVec = btVector3(0,0,torque);
            }
            break;
            case KEY_LSHIFT:
            {
                game->velocity -= 20;
                if(game->velocity < -200) game->velocity = -200;
            }
            break;
            case KEY_LCONTROL:
            {
                game->velocity += 20;
                if(game->velocity > 0) game->velocity = 0;
            }
            break;
            case KEY_SPACE:
            {
                game->Shoot();
            }
            break;
            default:
                return false;
            break;
        }
        //apply the effect of WASDQE
        if(torqueVec != empty)
        {
            game->btShip->setDamping(0,0.9);
            game->btShip->setAngularVelocity(game->btShip->getWorldTransform().getBasis() * torqueVec + game->btShip->getAngularVelocity());
        }
        return true;
    }
    return false;
}