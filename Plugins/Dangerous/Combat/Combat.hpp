#pragma once

#include "Services/Hooks/Hooks.hpp"

namespace Dangerous
{

class Dangerous;

class Combat
{
public:
    explicit Combat(NWNXLib::ViewPtr<Dangerous> plugin);
    ~Combat() = default;

private:
    static bool m_overrideEnhancementBehavior;
    static bool m_useEpicDodgeEvent;
    static bool m_useDeathAttackRollEvent;
    static bool m_fixOverwhelmingDamage;
    static bool m_useDevastatingCriticalEvent;
    static bool m_useSneakAttackCheckEvent;
    static bool m_useCriticalCheckEvent;
    static bool m_useCriticalHitDamageEvent;
    static bool m_useSneakDamageEvent;

    //Helper function to calculate sneak attack dice
    static int GetSneakAttackDice(NWNXLib::API::CNWSCreature *thisPtr);
    //Helper function to calculate death attack dice
    static int GetDeathAttackDice(NWNXLib::API::CNWSCreature *thisPtr);
    static inline float GetDistance(NWNXLib::API::Vector &v1, NWNXLib::API::Vector &v2);
    //Helper function to get the offhand weapon
    static NWNXLib::API::CNWSItem *GetDualwieldWeapon(NWNXLib::API::CNWSCreature *pCreature);

    //Engine functions
    static void CNWSCreature__ResolveAttackRoll(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget);
    static int32_t CNWSCreatureStats__GetCriticalHitRoll(NWNXLib::API::CNWSCreatureStats *thisPtr, int32_t bOffhand);
    static int32_t CNWSCreature__ResolveDefensiveEffects(NWNXLib::API::CNWSCreature *thisPtr,
                                                         NWNXLib::API::CNWSObject *pTarget, int32_t bHit);
    static void CNWSCreature__ResolveMeleeAttack(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget,
                                                 int32_t nNumAttacks, int32_t a4);
    static void CNWSCreature__ResolveRangedAttack(NWNXLib::API::CNWSCreature *thisPtr,
                                                  NWNXLib::API::CNWSObject *pTarget,
                                                  int32_t nNumAttacks, int32_t a4);
    static void CNWSCreature__ResolvePostRangedDamage(NWNXLib::API::CNWSCreature *thisPtr,
                                                      NWNXLib::API::CNWSObject *pTarget);
    static void CNWSCreature__ResolveCachedSpecialAttacks(NWNXLib::API::CNWSCreature *thisPtr);
    static void CNWSCreature__ResolveSituationalModifiers(NWNXLib::API::CNWSCreature *thisPtr,
                                                          NWNXLib::API::CNWSObject *pTarget);
    static void CNWSCreature__ResolveDamage(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget);
    static void CNWSCreature__ResolvePostMeleeDamage(NWNXLib::API::CNWSCreature *thisPtr,
                                                     NWNXLib::API::CNWSObject *pTarget);
    static int32_t CNWSCreatureStats__GetDamageRoll(NWNXLib::API::CNWSCreatureStats *thisPtr,
                                                    NWNXLib::API::CNWSObject *pTarget,
                                                    int32_t bOffhand, int32_t bCritical, int32_t bSneak, int32_t bDeath,
                                                    int32_t bKiDamage);
    static int32_t CNWSCreature__GetIsWeaponEffective(NWNXLib::API::CNWSCreature *thisPtr, uint32_t nObjectID,
                                                      int32_t bOffhand);
    static int32_t CNWSCreatureStats__GetDamageBonus(NWNXLib::API::CNWSCreatureStats *thisPtr,
                                                     NWNXLib::API::CNWSCreature *pTarget, int32_t bOffhand);
    static int32_t CNWSObject__DoDamageImmunity(NWNXLib::API::CNWSObject *thisPtr,
                                                NWNXLib::API::CNWSCreature *pAttacker,
                                                int32_t nDamage, uint16_t nFlags, int32_t bSimulate,
                                                int32_t bCombatDamage);
    static int32_t CNWSObject__DoDamageReduction(NWNXLib::API::CNWSObject *thisPtr,
                                                 NWNXLib::API::CNWSCreature *pAttacker,
                                                 int32_t nDamage, unsigned char nDamagePower, int32_t bSimulate,
                                                 int32_t bCombatDamage);
    static unsigned char CNWSCreature__CalculateDamagePower(NWNXLib::API::CNWSCreature *thisPtr,
                                                            NWNXLib::API::CNWSObject *pTarget,
                                                            int32_t bOffhand);
    static int16_t CNWSCreature__GetArmorClass(NWNXLib::API::CNWSCreature *thisPtr);
    static char CNWSCreatureStats__GetACNaturalBase(NWNXLib::API::CNWSCreatureStats *thisPtr, int32_t bTouchAttack);
    static char CNWSCreatureStats__GetTotalACSkillMod(NWNXLib::API::CNWSCreatureStats *thisPtr);
    static char CNWSCreatureStats__GetDEXMod(NWNXLib::API::CNWSCreatureStats *thisPtr, int32_t bArmorDexCap);
    static int32_t CNWSCreature__GetWeaponPower(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget,
                                                int32_t bOffhand);
};

}