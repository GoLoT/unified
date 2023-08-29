#pragma once

#include "nwnx.hpp"
#include <list>
#include <map>
#include <set>
using namespace std;
using namespace NWNXLib::API;
using namespace NWNXLib::Services;

using ArgumentStack = NWNXLib::ArgumentStack;

namespace Feat {

class Feat : public NWNXLib::Plugin
{
public:
    Feat(NWNXLib::Services::ProxyServiceList* services);
    virtual ~Feat();

private:
    bool m_ShowEffectIcon;
    uint32_t m_nCustomSpellID;

    ArgumentStack SetFeatModifier(ArgumentStack&& args);

    enum FeatModifier
    {
        INVALID              = 0,
        AB                   = 1,
        ABILITY              = 2,
        ABVSRACE             = 3,
        AC                   = 4,
        ACVSRACE             = 5,
        ARCANESPELLFAILURE   = 6,
        CONCEALMENT          = 7,
        DAMAGE               = 8,
        DMGIMMUNITY          = 9,
        DMGREDUCTION         = 10,
        DMGRESIST            = 11,
        IMMUNITY             = 12,
        MOVEMENTSPEED        = 13,
        REGENERATION         = 14,
        SAVE                 = 15,
        SAVEVSRACE           = 16,
        SAVEVSTYPE           = 17,
        SAVEVSTYPERACE       = 18,
        SPELLIMMUNITY        = 19,
        SRCHARGEN            = 20,
        SRINCLEVEL           = 21,
        SPELLSAVEDC          = 22,
        BONUSSPELL           = 23,
        TRUESEEING           = 24,
        SEEINVISIBLE         = 25,
        ULTRAVISION          = 26,
        HASTE                = 27,
        VISUALEFFECT         = 28,
        SPELLSAVEDCFORSCHOOL = 29,
        SPELLSAVEDCFORSPELL  = 30,
    };

    set<uint16_t> m_Feats;
    unordered_map<uint16_t, int32_t>                                                  m_FeatAB;
    unordered_map<uint16_t, unordered_map<uint8_t, int32_t>>                          m_FeatAbility;
    unordered_map<uint16_t, unordered_map<uint16_t, int32_t>>                         m_FeatABVsRace;
    unordered_map<uint16_t, int32_t>                                                  m_FeatAC;
    unordered_map<uint16_t, unordered_map<uint16_t, int32_t>>                         m_FeatACVsRace;
    unordered_map<uint16_t, int8_t>                                                   m_FeatArcaneSpellFailure;
    unordered_map<uint16_t, unordered_map<uint8_t, unordered_map<uint8_t, uint8_t>>>  m_FeatBonusSpell;
    unordered_map<uint16_t, uint8_t>                                                  m_FeatConcealment;
    unordered_map<uint16_t, unordered_map<uint32_t, int32_t>>                         m_FeatDamage;
    unordered_map<uint16_t, unordered_map<uint32_t, int32_t>>                         m_FeatDmgImmunity;
    unordered_map<uint16_t, unordered_map<uint8_t, int32_t>>                          m_FeatDmgReduction;
    unordered_map<uint16_t, unordered_map<uint32_t, int32_t>>                         m_FeatDmgResist;
    set<uint16_t>                                                                     m_FeatHaste;
    unordered_map<uint16_t, set<uint32_t>>                                            m_FeatImmunities;
    unordered_map<uint16_t, int32_t>                                                  m_FeatMovementSpeed;
    unordered_map<uint16_t, pair<uint8_t, uint16_t>>                                  m_FeatRegeneration;
    unordered_map<uint16_t, unordered_map<uint8_t, int32_t>>                          m_FeatSave;
    unordered_map<uint16_t, unordered_map<uint8_t, unordered_map<uint16_t, int16_t>>> m_FeatSaveVsRace;
    unordered_map<uint16_t, unordered_map<uint8_t, unordered_map<uint16_t, int16_t>>> m_FeatSaveVsType;
    unordered_map<uint16_t, unordered_map<uint8_t, unordered_map<uint16_t, unordered_map<uint16_t, int16_t>>>> m_FeatSaveVsTypeRace;
    set<uint16_t>                                                                     m_FeatSeeInvisible;
    unordered_map<uint16_t, int32_t>                                                  m_FeatSpellSaveDC;
    unordered_map<uint16_t, set<uint32_t>>                                            m_FeatSpellImmunities;
    unordered_map<uint16_t, pair<uint8_t, uint8_t>>                                   m_FeatSRCharGen;
    unordered_map<uint16_t, tuple<uint8_t, uint8_t, uint8_t>>                         m_FeatSR;
    set<uint16_t>                                                                     m_FeatTrueSeeing;
    set<uint16_t>                                                                     m_FeatUltravision;
    unordered_map<uint16_t, set<uint16_t>>                                            m_FeatVFX;
    unordered_map<uint16_t, pair<uint8_t, int32_t>>                                   m_FeatSpellSaveDCForSpellSchool;
    unordered_map<uint16_t, pair<uint16_t, int32_t>>                                  m_FeatSpellSaveDCForSpell;

    static void DoEffect(CNWSCreature*, uint16_t, uint16_t, int32_t = 0, int32_t = 0, int32_t = 0, int32_t = 0, int32_t = 0, int32_t = 0);
    static void ApplyFeatEffects(CNWSCreature*, uint16_t);
    static void AddFeatEffects(CNWSCreatureStats*, uint16_t);
    static void RemoveFeatEffects(CNWSCreatureStats*, uint16_t);
    static void AddRemoveBonusSpell(CNWSCreatureStats*, uint16_t, bool bAdd = true);
    static bool DoFeatModifier(int32_t, FeatModifier, int32_t, int32_t, int32_t, int32_t);

    static void AddFeatHook(CNWSCreatureStats*, uint16_t);
    static void RemoveFeatHook(CNWSCreatureStats*, uint16_t);
    static int32_t  OnApplyBonusFeatHook(CNWSEffectListHandler*, CNWSObject*, CGameEffect*, int32_t);
    static int32_t  OnRemoveBonusFeatHook(CNWSEffectListHandler*, CNWSObject*, CGameEffect*);
    static uint8_t SavingThrowRollHook(CNWSCreature*, uint8_t, uint16_t, uint8_t, ObjectID, int32_t, uint16_t, int32_t);
    static int32_t GetWeaponPowerHook(CNWSCreature*, CNWSObject*, int32_t);
    static int32_t GetTotalEffectBonusHook(CNWSCreature*, uint8_t, CNWSObject*, int32_t, int32_t, uint8_t, uint8_t, uint8_t, uint8_t, int32_t);
    static void EatTURDHook(CNWSPlayer*, CNWSPlayerTURD*);
};

}
