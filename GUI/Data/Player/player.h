#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QObject>
#include <QPainter>

#include "playerInfo.h"

class Player
{
public:
    Player(unsigned long id, QString name);

    void setPlayerSide(QString playerSide);
    void incrementKills();
    void incrementAssists();
    void incrementDeaths();
    void incrementEnemyFlashes();
    void incrementTeamFlahes();
    void incrementUtilityDMG(unsigned short dmg);

    ulong getID();
    QString getName();
    QString getPlayerSide();
    ushort getKills();
    ushort getAssists();
    ushort getDeaths();
    ushort getEnemyFlashes();
    ushort getTeamFlahes();
    ushort getUtilityDMG();

    void addPlayerInfo(int tick, QPointF playerPosition, float playerXView, unsigned short money, unsigned char health, unsigned char armor, bool helmet, bool kit, QString activeWeapon);
    QList<PlayerInfo> getListPlayerInfo();

private:
    ulong mID;
    QString mName;
    QString mPlayerSide;
    ushort mKills;
    ushort mAssists;
    ushort mDeaths;
    ushort mEnemyFlashes;
    ushort mTeamFlashes;
    ushort mUtilityDMG;

    QList<PlayerInfo> mListPlayerInfo;
};


#endif // PLAYER_H