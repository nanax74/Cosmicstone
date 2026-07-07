#include "Cmn/WeaponData.hpp"
#include "Cmn/WeaponSubType.hpp"
#include "Game/BulletBombBase.hpp"
#include "Game/BulletMgr.hpp"
#include "Game/CloneObjMgr.hpp"
#include "Game/Player.hpp"
#include "Game/PlayerInkAction.hpp"
#include "Game/PlayerParam.hpp"
#include "Game/MainMgrBase.hpp"
#include "Game/BulletCloneHandle.hpp"
#include "Game/Bullet.hpp"
#include "Game/BulletBombInstant.hpp"
#include "math.hpp"

namespace Cosmicstone
{
    extern "C" void shotBombInstantEx(Game::PlayerInkAction* t, const sead::Vector3<float>* directionOffset)
    {
        Game::Player* player = t->player;
        Game::Player::PlayerControlType controlType = player->controlType;

        sead::Vector3<float> position;
        sead::Vector3<float> velocity;
        sead::Vector3<float> direction;

        direction.x = 0.0f;
        direction.y = 0.6f;

        Cmn::WeaponData* weaponData = t->getShotVel_BombStd();
        if (weaponData->field_0xf4 == 8)
            direction.z = 2.8f;
        else
            direction.z = t->player->playerParam->field_0x50;

        direction.x += directionOffset->x;
        direction.y += directionOffset->y;
        direction.z += directionOffset->z;

        bool unk1 = false;
        if ((controlType == Game::Player::PlayerControlType::Local) && (player->isRemoteControlled == false))
        {
            unk1 = true;
        }
        Game::PlayerInkAction::WeaponTypeAddMove weaponType = Game::PlayerInkAction::WeaponTypeAddMove::Unknown;
        t->calcShotPosVel(-80.0, 0.0, 50.0, &position, &velocity, nullptr, &direction, &weaponType, unk1, 0, 0);
        if (velocity.y < -1.0)
        {
            float u0 = -1.0 / velocity.y;
            velocity.x *= u0;
            velocity.y *= u0;
            velocity.z *= u0;
        }
        Game::BulletMgr::BulletType type = static_cast<Game::BulletMgr::BulletType>(Game::MainMgrBase::sInstance->bulletMgr->actorTypeTableByIndex[Game::BulletBombInstant::sActorTypeIndex]);
        if (type != Game::BulletMgr::BulletType::Invalid)
        {
            Game::BulletBombBase* bullet = reinterpret_cast<Game::BulletBombBase*>(Game::MainMgrBase::sInstance->bulletMgr->activateOneCancelUnnecessary(type,true,&position,&velocity));
            {
                if(bullet)
                {
                    uint32_t playerIndex = player->playerIndex;
                    uint32_t cloneHandleSize = Game::CloneObjMgr::sInstance->bulletCloneHandleArray.size;
                    Game::BulletCloneHandle* bulletCloneHandle = nullptr;

                    if(playerIndex < cloneHandleSize)
                    {
                        bulletCloneHandle = reinterpret_cast<Game::BulletCloneHandle**>(Game::CloneObjMgr::sInstance->bulletCloneHandleArray.pointer)[playerIndex];
                    }

                    bulletCloneHandle->sendEvent_ShotBombInstant(playerIndex,&position,&velocity);
                    uint32_t seed = t->getRandomSeed();
                    bullet->initializeSender(player, playerIndex, playerIndex, &position, &velocity, seed);
                    player->startMotion_BombThrow(0, 1);
                    t->setInkLock(0x28, true, true);
                }
            }
        }
    }

    extern "C" void shotBombInstantRadialXY(Game::PlayerInkAction* t)
    {
        int n = 8;
        for(int i = 0; i < n; i++)
        {
            float rad = 2.0f * M_PI * i / n;
            sead::Vector3<float> direction;
            direction.x = cosf(rad);
            direction.y = sinf(rad);
            direction.z = 0.0f;
            shotBombInstantEx(t, &direction);
        }
    }
}