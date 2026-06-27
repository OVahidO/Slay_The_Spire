#include "skillcards.h"

SkillCard::SkillCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Skill, energyCost, path, isRare, parent) {}

SkillCard::~SkillCard() {}