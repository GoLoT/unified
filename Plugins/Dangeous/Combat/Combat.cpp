#include "Combat.hpp"
#include "API/C2DA.hpp"
#include "API/CAppManager.hpp"
#include "API/Constants.hpp"
#include "API/CNWBaseItem.hpp"
#include "API/CNWBaseItemArray.hpp"
#include "API/CNWCCMessageData.hpp"
#include "API/CNWRules.hpp"
#include "API/CNWSCreature.hpp"
#include "API/CNWSCreatureStats.hpp"
#include "API/CNWSCombatRound.hpp"
#include "API/CNWSInventory.hpp"
#include "API/CNWSItem.hpp"
#include "API/CNWSObject.hpp"
#include "API/CServerExoApp.hpp"
#include "API/CTwoDimArrays.hpp"
#include "API/Functions.hpp"
#include "API/Globals.hpp"
#include "API/Vector.hpp"

namespace Dangerous
{

using namespace NWNXLib;
using namespace NWNXLib::API;

Combat::Combat(ViewPtr<Services::HooksProxy> hooker)
{
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveAttackRoll>
        (&CNWSCreature__ResolveAttackRoll);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetCriticalHitRoll>
        (&CNWSCreatureStats__GetCriticalHitRoll);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveDefensiveEffects>
        (&CNWSCreature__ResolveDefensiveEffects);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveMeleeAttack>
        (&CNWSCreature__ResolveMeleeAttack);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveRangedAttack>
        (&CNWSCreature__ResolveRangedAttack);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolvePostRangedDamage>
        (&CNWSCreature__ResolvePostRangedDamage);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveCachedSpecialAttacks>
        (&CNWSCreature__ResolveCachedSpecialAttacks);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveSituationalModifiers>
        (&CNWSCreature__ResolveSituationalModifiers);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolveDamage>
        (&CNWSCreature__ResolveDamage);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__ResolvePostMeleeDamage>
        (&CNWSCreature__ResolvePostMeleeDamage);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetDamageRoll>
        (&CNWSCreatureStats__GetDamageRoll);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__GetIsWeaponEffective>
        (&CNWSCreature__GetIsWeaponEffective);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetDamageBonus>
        (&CNWSCreatureStats__GetDamageBonus);
    hooker->RequestExclusiveHook<API::Functions::CNWSObject__DoDamageImmunity>
        (&CNWSObject__DoDamageImmunity);
    hooker->RequestExclusiveHook<API::Functions::CNWSObject__DoDamageReduction>
        (&CNWSObject__DoDamageReduction);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__CalculateDamagePower>
        (&CNWSCreature__CalculateDamagePower);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreature__GetArmorClass>
        (&CNWSCreature__GetArmorClass);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetACNaturalBase>
        (&CNWSCreatureStats__GetACNaturalBase);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetTotalACSkillMod>
        (&CNWSCreatureStats__GetTotalACSkillMod);
    hooker->RequestExclusiveHook<API::Functions::CNWSCreatureStats__GetDEXMod>
        (&CNWSCreatureStats__GetDEXMod);
}

int Combat::GetSneakAttackDice(CNWSCreature *thisPtr)
{
    int nDice = 0;

    for (int i = 10; i > 0; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::EpicImprovedSneakAttack1 - 1 + i))
        { //EpicImprovedSneakAttack
            nDice = i;
            break;
        }
    }

    bool nBaseSneakDiceAdded = false;
    for (int i = 20; i > 10 && !nBaseSneakDiceAdded; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::SneakAttack11 - 11 + i))
        { //SneakAttack11-20
            nDice += i;
            nBaseSneakDiceAdded = true;
        }
    }

    for (int i = 10; i > 1 && !nBaseSneakDiceAdded; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::SneakAttack2 - 2 + i))
        { //SneakAttack10-2
            nDice += i;
            nBaseSneakDiceAdded = true;
        }
    }
    if (!nBaseSneakDiceAdded && thisPtr->m_pStats->HasFeat(Constants::Feat::SneakAttack))
    {
        nDice += 1;
    }

    nBaseSneakDiceAdded = false;
    for (int i = 15; i > 3 && !nBaseSneakDiceAdded; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::BlackguardSneakAttack4d6 - 4 + i))
        { //BlackguardSneakAttack4-15
            nDice += i;
            nBaseSneakDiceAdded = true;
        }
    }
    for (int i = 3; i > 0 && !nBaseSneakDiceAdded; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::BlackguardSneakAttack1d6 - 1 + i))
        { //BlackguardSneakAttack1-3
            nDice += i;
            nBaseSneakDiceAdded = true;
        }
    }

    return nDice;
}

int Combat::GetDeathAttackDice(CNWSCreature *thisPtr)
{
    int nDice = 0;

    for (int i = 20; i > 8; i--)
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::PrestigeDeathAttack9 + i))
        { //PrestigeDeathAttack9-20
            nDice = i;
            break;
        }
    }
    if (nDice == 0)
    {
        for (int i = 8; i > 5; i--)
        {
            if (thisPtr->m_pStats->HasFeat(Constants::Feat::PrestigeDeathAttack8 - 8 + i))
            { //PrestigeDeathAttack6-8
                nDice = i;
                break;
            }
        }
    }
    if (nDice == 0)
    {
        for (int i = 5; i > 0; i--)
        {
            if (thisPtr->m_pStats->HasFeat(Constants::Feat::PrestigeDeathAttack5 - 5 + i))
            { //PrestigeDeathAttack1-5
                nDice = i;
                break;
            }
        }
    }

    return nDice;
}

inline float Combat::GetDistance(Vector &v1, Vector &v2)
{
    return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z);
}

CNWSItem *Combat::GetDualwieldWeapon(CNWSCreature *pCreature)
{
    if (!pCreature)
    {
        return nullptr;
    }

    auto pItem = pCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::LeftHand);
    if (!pItem)
    {
        pItem = pCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);
        if (pItem)
        {
            auto pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pItem->m_nBaseItem);
            if (!pBaseItem || pBaseItem->m_nWeaponWield == 8)
            {
                return pItem;
            }
        }
    }
    else
    {
        return pItem;
    }

    return nullptr;
}

void Combat::CNWSCreature__ResolveAttackRoll(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget)
{
    if (!pTarget)
        return;

    auto pAttackData = thisPtr->m_pcCombatRound->GetAttack(thisPtr->m_pcCombatRound->m_nCurrentAttack);
    CNWSCreatureStats *pThisStats = thisPtr->m_pStats;
    CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);

    if (pTargetCreature)
    {
        thisPtr->ResolveSneakAttack(pTargetCreature);
        thisPtr->ResolveDeathAttack(pTargetCreature);
    }

    if (pAttackData->m_bCoupDeGrace)
    {
        pAttackData->m_nToHitRoll = 20;
        pAttackData->m_nAttackResult = 7;
        pAttackData->m_nToHitMod = pThisStats->GetAttackModifierVersus(pTargetCreature);
        return;
    }

    pAttackData->m_nToHitMod = pThisStats->GetAttackModifierVersus(pTargetCreature);
    pAttackData->m_nToHitRoll = Globals::Rules()->RollDice(1, 20);
    auto nModifiedRoll = pAttackData->m_nToHitRoll + pAttackData->m_nToHitMod;
    auto nAC = !pTargetCreature ? 0 : pTargetCreature->m_pStats->GetArmorClassVersus(thisPtr, 0);
    bool bHit = nModifiedRoll >= nAC;

    //Test deflect / epic dodge
    if (thisPtr->ResolveDefensiveEffects(pTarget, bHit))
        return;

    //Parry check
    if (pTargetCreature &&
        pTargetCreature->m_nCombatMode == 1 && pAttackData->m_nToHitRoll != 20 &&
        pTargetCreature->m_pcCombatRound->m_nParryActions != 0 &&
        pTargetCreature->m_pcCombatRound->m_bRoundPaused == 0 &&
        pTargetCreature->m_nState != 6 && !pAttackData->m_bRangedAttack &&
        !(pTargetCreature->GetRangeWeaponEquipped()))
    {

        auto pThisAsNWSObject = Utils::AsNWSObject(thisPtr);
        auto nParrySkill = pTargetCreature->m_pStats->GetSkillRank(Constants::Skill::Parry, pThisAsNWSObject, 0);
        auto nParryCheck = Globals::Rules()->RollDice(1, 20) + nParrySkill - nModifiedRoll;
        if (nParryCheck >= 0)
        {
            pAttackData->m_nAttackResult = 2;
            pTargetCreature->m_pcCombatRound->m_nParryActions--;
            if (nParryCheck >= 10)
            {
                pTargetCreature->m_pcCombatRound->AddParryAttack(thisPtr->m_idSelf);
            }
            return;
        }

        pTargetCreature->m_pcCombatRound->AddParryIndex();
        pTargetCreature->m_pcCombatRound->m_nParryActions--;
    }

    if (pAttackData->m_nToHitRoll == 1)
    {
        //Critical miss
        pAttackData->m_nAttackResult = 4;
        pAttackData->m_nMissedBy = 1;
        return;
    }
    else
    {
        if (pAttackData->m_nToHitRoll != 20 && nModifiedRoll < nAC)
        {
            //Regular miss
            pAttackData->m_nAttackResult = 4;
            pAttackData->m_nMissedBy = static_cast<char>(nModifiedRoll - nAC);
            if (pAttackData->m_nMissedBy < 0)
            {
                pAttackData->m_nMissedBy = static_cast<char>(-pAttackData->m_nMissedBy);
            }
            return;
        }
    }

    auto nCritThreat = pThisStats->GetCriticalHitRoll(thisPtr->m_pcCombatRound->GetOffHandAttack());
    //Weapon threat test
    if (nCritThreat <= pAttackData->m_nToHitRoll)
    {
        pAttackData->m_nThreatRoll = Globals::Rules()->RollDice(1, 20);
        pAttackData->m_bCriticalThreat = 1;
        //Confirm crit
        if (pAttackData->m_nThreatRoll + pAttackData->m_nToHitMod >= nAC)
        {
            if (!pTargetCreature)
            {
                pAttackData->m_nAttackResult = 3;
                return;
            }
            auto nDifficultySetting = Globals::AppManager()->m_pServerExoApp->GetDifficultyOption(0);
            //Checking very easy difficulty, monster attack on PC or controlled familiar
            if (nDifficultySetting == 1 && !thisPtr->m_bPlayerCharacter &&
                (pTargetCreature->m_bPlayerCharacter || pTargetCreature->GetIsPossessedFamiliar())
                && !thisPtr->GetIsPossessedFamiliar())
            {
                pAttackData->m_nAttackResult = 1;
                return;
            }

            if (!pTargetCreature->m_pStats->GetEffectImmunity(Constants::ImmunityType::CriticalHit, thisPtr, 1))
            {
                //Crit immunity
                pAttackData->m_nAttackResult = 3;
                return;
            }
            auto pMessage = new CNWCCMessageData();
            pMessage->SetObjectID(0, pTarget->m_idSelf);
            pMessage->SetInteger(0, 126);
            pAttackData->m_alstPendingFeedback.Add(pMessage);
        }
    }

    //Regular hit
    pAttackData->m_nAttackResult = 1;
}

int32_t Combat::CNWSCreatureStats__GetCriticalHitRoll(NWNXLib::API::CNWSCreatureStats *thisPtr, int32_t bOffhand)
{
    auto pInventory = thisPtr->m_pBaseCreature->m_pInventory;
    CNWSItem *pItem = nullptr;

    //Get item used to attack
    if (bOffhand)
        pItem = GetDualwieldWeapon(thisPtr->m_pBaseCreature);
    else
        pItem = pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);

    int nBaseThreat = 1;
    int nTotalThreat = 0;

    if (!pItem)
    {
        //If not using weapon, get gloves/bracers with base threat 1
        pItem = pInventory->GetItemInSlot(Constants::EquipmentSlot::Arms);
    }
    else
    {
        //Get weapon threat from base item
        auto pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pItem->m_nBaseItem);
        nBaseThreat = pBaseItem->m_nCritThreat;
    }
    nTotalThreat = nBaseThreat;

    if (pItem)
    {
        //GetKeenProperty
        CNWItemProperty *pItemPropPtr;
        if (pItem->GetPropertyByType(&pItemPropPtr, Constants::ItemProperty::Keen, 0))
            nTotalThreat += nBaseThreat;
    }

    //Get weapon again because reasons...
    auto pWeapon = thisPtr->m_pBaseCreature->m_pcCombatRound->GetCurrentAttackWeapon(bOffhand ? 2 : 1);
    if (pWeapon && thisPtr->GetIsWeaponOfChoice(pWeapon->m_nBaseItem))
    {
        nTotalThreat += 2;
    }

    if (thisPtr->GetWeaponImprovedCritical(pItem))
        nTotalThreat += nBaseThreat;

    return 21 - nTotalThreat;
}

int32_t Combat::CNWSCreature__ResolveDefensiveEffects(CNWSCreature *thisPtr, CNWSObject *pTarget, int32_t bHit)
{
    CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
    if (!pTargetCreature)
        return 0;

    auto pAttackData = thisPtr->m_pcCombatRound->GetAttack(thisPtr->m_pcCombatRound->m_nCurrentAttack);

    int nMissChance = 0;
    int nConcealment = 0;

    for (int i = thisPtr->m_pStats->m_nMissChancePtr;
         i < thisPtr->m_appliedEffects.num &&
             thisPtr->m_appliedEffects.element[i]->m_nType <= Constants::EffectTrueType::MissChance;
         i++)
    {
        //Calculate miss chance percent
        auto pEffect = thisPtr->m_appliedEffects.element[i];
        if (pEffect->m_nType == Constants::EffectTrueType::MissChance)
        {
            int nResult = 1;
            auto nParam1 = pEffect->GetInteger(1);

            switch (nParam1)
            {
                case 1:nResult = (pTargetCreature->m_nVisionType & 2u) != 0;
                    break;
                case 2:nResult = pAttackData->m_bRangedAttack;
                    break;
                case 3:nResult = !pAttackData->m_bRangedAttack;
                    break;
            }

            if (nResult != 0 && (nParam1 = pEffect->GetInteger(0)) > nMissChance)
                nMissChance = nParam1;
        }

    }

    for (int i = pTargetCreature->m_pStats->m_nInvisibilityPtr;
         i < pTargetCreature->m_appliedEffects.num &&
             pTargetCreature->m_appliedEffects.element[i]->m_nType <= Constants::EffectTrueType::Concealment;
         i++)
    {
        //Calculate concealment from invisibility
        auto pEffect = pTargetCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == Constants::EffectTrueType::Invisibility)
        {
            auto nParam0 = pEffect->GetInteger(0);
            auto nParam1 = pEffect->GetInteger(1);
            auto nParam2 = pEffect->GetInteger(2);
            auto nParam3 = pEffect->GetInteger(3);
            auto nAlignmentLaw = thisPtr->m_pStats->GetSimpleAlignmentLawChaos();
            auto nAlignmentGood = thisPtr->m_pStats->GetSimpleAlignmentGoodEvil();

            if (nConcealment < 50 &&
                (nParam1 == thisPtr->m_pStats->m_nRace || nParam1 == Constants::RacialType::All) &&
                (nParam2 == 0 || nParam2 == nAlignmentLaw) &&
                (nParam3 == 0 || nParam3 == nAlignmentGood)
                )
            {
                if (nParam0 == 2)
                {
                    if ((thisPtr->m_nVisionType & 6u) == 0)
                        nConcealment = 50;
                }
                else if ((thisPtr->m_nVisionType & 5u) == 0)
                {
                    nConcealment = 50;
                }
            }

        }
        else
        {
            //Calculate concealment from other sources
            if (pEffect->m_nType == Constants::EffectTrueType::Concealment)
            {
                auto nParam0 = pEffect->GetInteger(0);
                auto nParam1 = pEffect->GetInteger(1);
                auto nParam2 = pEffect->GetInteger(2);
                auto nParam3 = pEffect->GetInteger(3);
                auto nAlignmentLaw = thisPtr->m_pStats->GetSimpleAlignmentLawChaos();
                auto nAlignmentGood = thisPtr->m_pStats->GetSimpleAlignmentGoodEvil();

                if (nConcealment < nParam0 &&
                    (nParam1 == thisPtr->m_pStats->m_nRace || nParam1 == Constants::RacialType::All) &&
                    (nParam2 == 0 || nParam2 == nAlignmentLaw) &&
                    (nParam3 == 0 || nParam3 == nAlignmentGood)
                    )
                {
                    auto nParam4 = pEffect->GetInteger(4);

                    if (nParam4 == 0)
                    {
                        nConcealment = nParam0;
                    }
                    else if (!pAttackData->m_bRangedAttack)
                    {
                        if (nParam4 == 3)
                            nConcealment = nParam0;
                    }
                    else if (nParam4 == 2)
                    {
                        nConcealment = nParam0;
                    }
                }
            }
        }
    }

    if (nConcealment < 50 && thisPtr->m_pStats->HasFeat(Constants::Feat::EpicSelfConcealment50))
        nConcealment = 50;
    else if (nConcealment < 40 && thisPtr->m_pStats->HasFeat(Constants::Feat::EpicSelfConcealment40))
        nConcealment = 40;
    else if (nConcealment < 30 && thisPtr->m_pStats->HasFeat(Constants::Feat::EpicSelfConcealment30))
        nConcealment = 30;
    else if (nConcealment < 20 && thisPtr->m_pStats->HasFeat(Constants::Feat::EpicSelfConcealment20))
        nConcealment = 20;
    else if (nConcealment < 10 && thisPtr->m_pStats->HasFeat(Constants::Feat::EpicSelfConcealment10))
        nConcealment = 10;

    //Test missing chance agains the highest of miss chance and concealment
    if (nMissChance > 0 || nConcealment > 0)
    {
        nMissChance = nMissChance > nConcealment ? nMissChance : nConcealment;
        int nRoll = Globals::Rules()->RollDice(1, 100);
        if (nRoll < nMissChance &&
            (!thisPtr->m_pStats->HasFeat(Constants::Feat::BlindFight)
                || Globals::Rules()->RollDice(1, 100) < nMissChance))
        {

            pAttackData->m_nAttackResult = 8;
            pAttackData->m_nConcealment = nMissChance;
            pAttackData->m_nMissedBy = 1;
            return 1;
        }
    }

    if (bHit)
    {
        //Deflect Arrow
        if (pAttackData->m_bRangedAttack &&
            pTargetCreature->m_pcCombatRound->m_bDeflectArrow &&
            !pTargetCreature->GetFlatFooted() &&
            pTargetCreature->m_pStats->HasFeat(Constants::Feat::DeflectArrows))
        {

            auto pItemRight = pTargetCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);
            auto pItemLeft = pTargetCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::LeftHand);

            CNWBaseItem *pBaseItem;

            if ((!pItemLeft && !pItemRight) ||
                (pItemRight && !pItemLeft &&
                    (pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pItemRight->m_nBaseItem)) &&
                    !pBaseItem->m_nWeaponRanged &&
                    pTargetCreature->GetRelativeWeaponSize(pItemRight) < 1)
                )
            {

                pTargetCreature->m_pcCombatRound->SetDeflectArrow(0);

                if (pTargetCreature->SavingThrowRoll(2, 20, 0, thisPtr->m_idSelf, 1, 8, 1))
                {
                    auto pWeapon = thisPtr->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);
                    if (pWeapon)
                    {
                        int nParam;
                        if (pWeapon->m_nBaseItem == 31 || pWeapon->m_nBaseItem == 59 || pWeapon->m_nBaseItem == 63)
                            nParam = 1;
                        else
                            nParam = 0;

                        thisPtr->CalculateProjectileTimeToTarget(pTargetCreature->m_vPosition, nParam);
                        pAttackData->m_nAttackResult = 2;
                        pAttackData->m_bAttackDeflected = 1;
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

void Combat::CNWSCreature__ResolveMeleeAttack(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget,
                                              int32_t nNumAttacks, int32_t a4)
{
    if (!pTarget)
        return;

    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto nCurrentAttack = pCombatRound->m_nCurrentAttack;
    auto pAttackData = pCombatRound->GetAttack(nCurrentAttack);
    pCombatRound->m_nAttackGroup++;
    pAttackData->m_nAttackGroup = pCombatRound->m_nAttackGroup;

    for (int i = 0; i < nNumAttacks; i++)
    {
        pAttackData->m_nAttackGroup = pCombatRound->m_nAttackGroup;
        pAttackData->m_oidReactObject = pTarget->m_idSelf;
        pAttackData->m_nAttackMode = thisPtr->m_nCombatMode;
        pAttackData->m_nWeaponAttackType = pCombatRound->GetWeaponAttackType();

        //Test coup de grace
        thisPtr->ResolveSituationalModifiers(pTarget);
        if (!pAttackData->m_bCoupDeGrace)
            thisPtr->ResolveCachedSpecialAttacks();

        if (pAttackData->m_nAttackType == 0)
        {
            thisPtr->ResolveAttackRoll(pTarget);
            auto nAttackResult = pAttackData->m_nAttackResult;
            if (nAttackResult == 1 || nAttackResult == 3 || nAttackResult == 5 ||
                nAttackResult == 6 || nAttackResult == 7 || nAttackResult == 10)
            {
                thisPtr->ResolveDamage(pTarget);
                thisPtr->ResolvePostMeleeDamage(pTarget);
            }
            thisPtr->ResolveMeleeAnimations(i, nNumAttacks, pTarget, a4);
        }
        else
        {
            thisPtr->ResolveMeleeSpecialAttack(i, nNumAttacks, pTarget, a4);
        }

        pCombatRound->SetCurrentAttack(++nCurrentAttack);
        pAttackData = pCombatRound->GetAttack(nCurrentAttack);

    }

    thisPtr->SignalMeleeDamage(pTarget, nNumAttacks);
}

void Combat::CNWSCreature__ResolveRangedAttack(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget,
                                               int32_t nNumAttacks, int32_t a4)
{
    int nAvailableAttacks = 0;
    if (pTarget && (nAvailableAttacks = thisPtr->GetAmmunitionAvailable(nNumAttacks)))
    {
        auto nCurrentAttack = thisPtr->m_pcCombatRound->m_nCurrentAttack;
        auto pAttackData = thisPtr->m_pcCombatRound->GetAttack(nCurrentAttack);
        thisPtr->m_pcCombatRound->m_nAttackGroup++;
        pAttackData->m_nAttackGroup = thisPtr->m_pcCombatRound->m_nAttackGroup;

        for (int i = 0; i < nAvailableAttacks; i++)
        {
            pAttackData->m_bRangedAttack = 1;
            pAttackData->m_oidReactObject = pTarget->m_idSelf;
            pAttackData->m_nWeaponAttackType = thisPtr->m_pcCombatRound->GetWeaponAttackType();
            thisPtr->ResolveSituationalModifiers(pTarget);
            if (!pAttackData->m_bCoupDeGrace)
            {
                thisPtr->ResolveCachedSpecialAttacks();
            }
            if (pAttackData->m_nAttackType == 0)
            {
                thisPtr->ResolveAttackRoll(pTarget);
                auto nAttackResult = pAttackData->m_nAttackResult;
                if (nAttackResult == 1 || nAttackResult == 3 || nAttackResult == 5 ||
                    nAttackResult == 6 || nAttackResult == 7 || nAttackResult == 10)
                {
                    thisPtr->ResolveDamage(pTarget);
                    thisPtr->ResolvePostRangedDamage(pTarget);
                }
                else
                {
                    thisPtr->ResolveRangedMiss(pTarget);
                }
                thisPtr->ResolveRangedAnimations(pTarget, a4);
            }
            else
            {
                thisPtr->ResolveRangedSpecialAttack(pTarget, a4);
            }
            thisPtr->m_pcCombatRound->SetCurrentAttack(++nCurrentAttack);
            pAttackData = thisPtr->m_pcCombatRound->GetAttack(nCurrentAttack);
        }

        thisPtr->SignalRangedDamage(pTarget, nAvailableAttacks);
    }
    thisPtr->m_pcCombatRound->SetRoundPaused(0, 0x7F000000);
    thisPtr->m_pcCombatRound->SetPauseTimer(0, 0);
    thisPtr->SetAnimation(1);
}

void Combat::CNWSCreature__ResolvePostRangedDamage(CNWSCreature *thisPtr, CNWSObject *pTarget)
{
    if (!pTarget)
        return;

    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto pCurrentAttack = pCombatRound->GetAttack(pCombatRound->m_nCurrentAttack);
    auto nAttackResult = pCurrentAttack->m_nAttackResult;

    CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
    auto nTotalDamage = pCurrentAttack->GetTotalDamage(1);
    if (!pTargetCreature)
    {
        if (pTarget->GetCurrentHitPoints(0) <= nTotalDamage && !pTarget->m_bPlotObject)
            pCurrentAttack->m_bKillingBlow = 1;
    }
    else
    {
        auto nCurrentHitpoints = pTargetCreature->GetCurrentHitPoints(0);
        //Check if killing blow
        if ((nCurrentHitpoints <= nTotalDamage || pCurrentAttack->m_bCoupDeGrace) &&
            !pTargetCreature->m_bIsImmortal && !pTargetCreature->m_bPlotObject)
        {
            pCurrentAttack->m_bKillingBlow = 1;
        }

        if (pCurrentAttack->m_bCoupDeGrace && GetDistance(thisPtr->m_vPosition, pTarget->m_vPosition) <= 100.0f)
        {
            auto pEffect = new CGameEffect(1);
            pEffect->m_nType = 19;
            pEffect->m_nSubType = 0;
            pEffect->SetCreator(thisPtr->m_idSelf);
            pEffect->SetInteger(0, 0);
            pEffect->SetInteger(1, 1);
            pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
        }

        auto pCurrentWeapon = pCombatRound->GetCurrentAttackWeapon(pCurrentAttack->m_nWeaponAttackType);
        if (pCurrentAttack->m_nAttackResult == 3 && thisPtr->m_pStats->GetEpicWeaponDevastatingCritical(pCurrentWeapon)
            && !pTargetCreature->m_pStats->GetEffectImmunity(Constants::ImmunityType::CriticalHit, thisPtr, 1) &&
            !pTargetCreature->m_bIsImmortal && !pCurrentAttack->m_bKillingBlow
            )
        {
            //Devastating critical roll
            auto nStrength = thisPtr->m_pStats->m_nStrengthModifier;
            auto nLevel = thisPtr->m_pStats->GetLevel(0);

            if (!pTargetCreature->SavingThrowRoll(1, nLevel / 2 + nStrength + 10, 0, thisPtr->m_idSelf, 1, 0, 1))
            {
                auto pEffect = new CGameEffect(1);
                pEffect->m_nType = Constants::EffectTrueType::Death;
                pEffect->m_nSubType = 0;
                pEffect->SetCreator(thisPtr->m_idSelf);
                pEffect->SetInteger(0, 1);
                pEffect->SetInteger(1, 1);
                pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
                pCurrentAttack->m_nAttackResult = 10;
            }
            else
            {
                auto pMessage = new CNWCCMessageData();
                pMessage->m_nType = 2;
                pMessage->SetObjectID(0, thisPtr->m_idSelf);
                pMessage->SetInteger(0, 0x101);
                pCurrentAttack->m_alstPendingFeedback.Add(pMessage);
            }
        }

        if (nTotalDamage < 1 && pCombatRound->m_bWeaponSucks == 0 &&
            !thisPtr->GetIsWeaponEffective(pTarget->m_idSelf, pCurrentAttack->m_nAttackType == 2))
        {
            auto pMessage = new CNWCCMessageData();
            pMessage->m_nType = 3;
            pMessage->SetInteger(0, 0x75);
            pCurrentAttack->m_alstPendingFeedback.Add(pMessage);
            pCombatRound->m_bWeaponSucks = 1;
        }
    }
}

void Combat::CNWSCreature__ResolveCachedSpecialAttacks(NWNXLib::API::CNWSCreature *thisPtr)
{
    auto pCombatRound = thisPtr->m_pcCombatRound;
    if (pCombatRound->m_nSpecialAttacks.num < 1)
        return;

    auto nTotalAttacks = pCombatRound->GetTotalAttacks();
    auto nCurrentAttack = pCombatRound->m_nCurrentAttack;

    while (nCurrentAttack < nTotalAttacks && pCombatRound->m_nSpecialAttacks.num > 0)
    {

        auto pAttack = pCombatRound->GetAttack(nCurrentAttack);

        if (pAttack->m_nAttackType == 0)
        {
            auto pSpecialAttack = pCombatRound->m_nSpecialAttacks.element;
            pAttack->m_nAttackType = *pSpecialAttack;
            auto pSpecialAttackID = pCombatRound->m_nSpecialAttackIDs.element;
            pAttack->m_nSpecialAttackID = *pSpecialAttackID;

            pCombatRound->m_nSpecialAttacks.num--;
            for (int i = 0; i < pCombatRound->m_nSpecialAttacks.num; i++)
            {
                pSpecialAttack[i] = pSpecialAttack[i + 1];
            }

            pCombatRound->m_nSpecialAttackIDs.num--;
            for (int i = 0; i < pCombatRound->m_nSpecialAttackIDs.num; i++)
            {
                pSpecialAttackID[i] = pSpecialAttackID[i + 1];
            }

            return;
        }

        nCurrentAttack++;
    }
}

void Combat::CNWSCreature__ResolveSituationalModifiers(NWNXLib::API::CNWSCreature *thisPtr,
                                                       NWNXLib::API::CNWSObject *pTarget)
{
    CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
    if (!pTargetCreature)
        return;

    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto pCurrentAttack = pCombatRound->GetAttack(pCombatRound->m_nCurrentAttack);

    if (pCurrentAttack->m_bRangedAttack &&
        GetDistance(thisPtr->m_vPosition, pTarget->m_vPosition) > 100.0f
        )
    {
        return;
    }

    if ((pTargetCreature->m_nState < 10) &&
        pTargetCreature->m_pStats->GetLevel(0) < 5 &&
        !pTargetCreature->m_bPlayerCharacter && !pTargetCreature->GetIsPossessedFamiliar()
        )
    {
        pCurrentAttack->m_bCoupDeGrace = 1;
    }
}

void Combat::CNWSCreature__ResolveDamage(NWNXLib::API::CNWSCreature *thisPtr, NWNXLib::API::CNWSObject *pTarget)
{
    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto bOffhandAttack = pCombatRound->GetOffHandAttack();
    auto pCurrentAttack = pCombatRound->GetAttack(pCombatRound->m_nCurrentAttack);
    auto bSneakAttack = pCurrentAttack->m_bSneakAttack;
    auto bCriticalAttack = pCurrentAttack->m_nAttackResult == 3;
    auto bDeathAttack = pCurrentAttack->m_bDeathAttack;
    int nDamage = 0;

    if (pCurrentAttack->m_nAttackType == Constants::Feat::KiDamage)
    {
        nDamage = thisPtr->m_pStats->GetDamageRoll(pTarget, bOffhandAttack, bCriticalAttack, bSneakAttack, bDeathAttack,
            1);
        thisPtr->m_pStats->DecrementFeatRemainingUses(Constants::Feat::KiDamage);
    }
    else
    {
        nDamage = thisPtr->m_pStats->GetDamageRoll(pTarget, bOffhandAttack, bCriticalAttack, bSneakAttack, bDeathAttack,
            0);
    }

    CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
    if (pTargetCreature)
    {
        //Defensive Roll test
        int nRemainingHP = pTargetCreature->GetCurrentHitPoints(0) - nDamage;
        if (nRemainingHP <= 0 && pTargetCreature->m_pStats->HasFeat(Constants::Feat::DefensiveRoll))
        {
            if (!pTargetCreature->GetFlatFooted())
            {
                if (pTargetCreature->m_pStats->GetFeatRemainingUses(Constants::Feat::DefensiveRoll) > 0)
                {
                    if (pTargetCreature->SavingThrowRoll(2, nDamage, 0, thisPtr->m_idSelf, 1,
                        Constants::Feat::DefensiveRoll, 1))
                    {
                        nDamage /= 2;
                    }
                    pTargetCreature->m_pStats->DecrementFeatRemainingUses(Constants::Feat::DefensiveRoll);
                }
            }
        }

        //Death Attack test
        if (bDeathAttack && pCombatRound->m_nCurrentAttack == 0 && pTargetCreature->m_bCombatState == 0)
        {
            auto nAssassinLevels = thisPtr->m_pStats->GetNumLevelsOfClass(Constants::ClassType::Assassin);
            nAssassinLevels = nAssassinLevels > 0 ? nAssassinLevels : 1;
            auto nDeathAttackDC = 10 + thisPtr->m_pStats->m_nIntelligenceModifier + nAssassinLevels;
            if (!pTargetCreature->SavingThrowRoll(1, nDeathAttackDC, 20, thisPtr->m_idSelf, 1, 0, 1)
                && pTargetCreature->m_nState != 8)
            {
                thisPtr->ApplyOnHitDeathAttack(pTarget, nAssassinLevels + Globals::Rules()->RollDice(1, 6));
            }
        }

        //Epic dodge
        if (nDamage > 0 && pTargetCreature->m_pStats->HasFeat(Constants::Feat::EpicDodge) &&
            !pTargetCreature->m_pcCombatRound->m_bEpicDodgeUsed)
        {
            auto pMessage = new CNWCCMessageData();
            pMessage->m_nType = 2;
            pMessage->SetObjectID(0, pTarget->m_idSelf);
            pMessage->SetInteger(0, 234);
            pCurrentAttack->m_alstPendingFeedback.Add(pMessage);
            pCurrentAttack->m_nAttackResult = 4;

            if (thisPtr->GetRangeWeaponEquipped())
            {
                pCurrentAttack->m_vRangedTarget.x = pTarget->m_vPosition.x;
                pCurrentAttack->m_vRangedTarget.y = pTarget->m_vPosition.y;
                pCurrentAttack->m_vRangedTarget.z = pTarget->m_vPosition.z;
            }

            pTargetCreature->m_pcCombatRound->m_bEpicDodgeUsed = 1;
            return;
        }
    }

    //Damage effects
    nDamage = pTarget->m_bPlotObject ? 0 : nDamage;
    pCurrentAttack->SetBaseDamage(nDamage);
    if (!pTarget->m_bPlotObject)
    {
        thisPtr->GetTotalEffectBonus(2, pTarget, 1, 0, 0, 0, -1, -1, 0);
        if (nDamage > 0)
            thisPtr->ResolveOnHitEffect(pTarget, bOffhandAttack, bCriticalAttack);

        thisPtr->ResolveItemCastSpell(pTarget);

        if (pCurrentAttack->GetTotalDamage(1) >= 0)
            thisPtr->ResolveOnHitVisuals(pTarget);
    }

}

void Combat::CNWSCreature__ResolvePostMeleeDamage(NWNXLib::API::CNWSCreature *thisPtr,
                                                  NWNXLib::API::CNWSObject *pTarget)
{
    if (!pTarget)
        return;

    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto pCurrentAttack = pCombatRound->GetAttack(pCombatRound->m_nCurrentAttack);
    auto nAttackResult = pCurrentAttack->m_nAttackResult;
    if (nAttackResult == 1 || nAttackResult == 3 || nAttackResult == 5 ||
        nAttackResult == 6 || nAttackResult == 7 || nAttackResult == 10)
    {
        auto nTotalDamage = pCurrentAttack->GetTotalDamage(1);
        CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
        if (!pTargetCreature)
        {
            if (pTarget->GetCurrentHitPoints(0) <= nTotalDamage && !pTarget->m_bPlotObject)
                pCurrentAttack->m_bKillingBlow = 1;
        }
        else
        {
            auto nCurrentHitpoints = pTargetCreature->GetCurrentHitPoints(0);
            //Check if killing blow
            if ((nCurrentHitpoints <= nTotalDamage || pCurrentAttack->m_bCoupDeGrace) &&
                !pTargetCreature->m_bIsImmortal && !pTargetCreature->m_bPlotObject
                )
            {
                pCurrentAttack->m_bKillingBlow = 1;
            }

            auto pCurrentWeapon = pCombatRound->GetCurrentAttackWeapon(pCurrentAttack->m_nWeaponAttackType);
            if (pCurrentAttack->m_nAttackResult == 3
                && thisPtr->m_pStats->GetEpicWeaponDevastatingCritical(pCurrentWeapon) &&
                !pTargetCreature->m_pStats->GetEffectImmunity(Constants::ImmunityType::CriticalHit, thisPtr, 1) &&
                !pTargetCreature->m_bIsImmortal && !pCurrentAttack->m_bKillingBlow
                )
            {
                //Devastating Critical
                auto nStrength = thisPtr->m_pStats->m_nStrengthModifier;
                auto nLevel = thisPtr->m_pStats->GetLevel(0);
                if (!pTargetCreature->SavingThrowRoll(1, nLevel / 2 + nStrength + 10, 0, thisPtr->m_idSelf, 1, 0, 1))
                {
                    auto pEffect = new CGameEffect(1);
                    pEffect->m_nType = Constants::EffectTrueType::Death;
                    pEffect->m_nSubType = 0;
                    pEffect->SetCreator(thisPtr->m_idSelf);
                    pEffect->SetInteger(0, 1);
                    pEffect->SetInteger(1, 1);
                    pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
                    pCurrentAttack->m_nAttackResult = 10;
                }
                else
                {
                    auto pMessage = new CNWCCMessageData();
                    pMessage->m_nType = 2;
                    pMessage->SetObjectID(0, thisPtr->m_idSelf);
                    pMessage->SetInteger(0, 0x101);
                    pCurrentAttack->m_alstPendingFeedback.Add(pMessage);
                }
            }

            //Crippling Strike
            if (nTotalDamage > 0 && pCurrentAttack->m_bSneakAttack &&
                thisPtr->m_pStats->HasFeat(Constants::Feat::CripplingStrike))
            {
                auto pEffect = new CGameEffect(1);
                pEffect->m_nType = 37;
                pEffect->m_nSubType = 2;
                pEffect->SetInteger(0, 0);
                pEffect->SetInteger(1, 2);
                pEffect->SetCreator(thisPtr->m_idSelf);
                pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
            }

            //Stunning fist
            if (pCurrentAttack->m_nAttackType == 296 && (nAttackResult == 1 ||
                nAttackResult == 3 || nAttackResult == 7 || nAttackResult == 10) &&
                nTotalDamage > 0
                && !pTargetCreature->m_pStats->GetEffectImmunity(Constants::ImmunityType::CriticalHit, thisPtr, 1)
                )
            {

                auto nAttackerLevel = thisPtr->m_pStats->GetLevel(0);
                auto nDefenderLevel = pTargetCreature->m_pStats->GetLevel(0);

                if (nAttackerLevel > nDefenderLevel)
                { //TODO: Check if this is wrong
                    auto nWisdom = thisPtr->m_pStats->m_nWisdomModifier;
                    if (!pTargetCreature->SavingThrowRoll(1, nAttackerLevel / 2 + nWisdom + 10, 0, thisPtr->m_idSelf, 1,
                        0, 1))
                    {
                        auto pEffect = new CGameEffect(1);
                        pEffect->m_nType = 19;
                        pEffect->m_nSubType = 0;
                        pEffect->SetCreator(thisPtr->m_idSelf);
                        pEffect->SetInteger(0, 1);
                        pEffect->SetInteger(1, 1);
                        pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
                    }
                }
            }

            //Coup de grace
            if (pCurrentAttack->m_bCoupDeGrace)
            {
                auto pEffect = new CGameEffect(1);
                pEffect->m_nType = 19;
                pEffect->m_nSubType = 0;
                pEffect->SetCreator(thisPtr->m_idSelf);
                pEffect->SetInteger(0, 0);
                pEffect->SetInteger(1, 1);
                pCurrentAttack->m_alstOnHitGameEffects.Add(pEffect);
            }

            //Circle kick
            if (pCurrentAttack->m_nAttackType != 867 && pCurrentAttack->m_nAttackType != 868 &&
                thisPtr->GetAttackResultHit(pCurrentAttack) && thisPtr->m_pStats->HasFeat(Constants::Feat::CircleKick)
                &&
                    pCombatRound->m_nCircleKickAttacks > 0 && thisPtr->GetUnarmed() &&
                pCombatRound->GetTotalAttacks() < 50)
            {

                auto fAttackRange = thisPtr->MaxAttackRange(0x7f000000, 0, 1);
                auto nNearestEnemyID = thisPtr->GetNearestEnemy(fAttackRange, pTargetCreature->m_idSelf, 1, 1);
                auto pNearestCreature = Globals::AppManager()->m_pServerExoApp->GetCreatureByGameObjectID(
                    nNearestEnemyID);

                if (pNearestCreature)
                {
                    pCombatRound->m_oidNewAttackTarget = nNearestEnemyID;
                    pCombatRound->AddCircleKickAttack(nNearestEnemyID);
                    thisPtr->m_bPassiveAttackBehaviour = 1;
                    pCombatRound->m_nCircleKickAttacks--;
                }
            }

            //Cleave
            if (pCurrentAttack->m_bKillingBlow && pCurrentAttack->m_nAttackType != 867 &&
                pCurrentAttack->m_nAttackType != 868 && pCombatRound->GetTotalAttacks() < 50
                )
            {

                auto bHasGreatCleave = thisPtr->m_pStats->HasFeat(Constants::Feat::GreatCleave);
                if (bHasGreatCleave || (thisPtr->m_pStats->HasFeat(Constants::Feat::Cleave) &&
                    pCombatRound->m_nCleaveAttacks > 0))
                {

                    auto fAttackRange = thisPtr->MaxAttackRange(0x7f000000, 0, 1);
                    auto nNearestEnemyID = thisPtr->GetNearestEnemy(fAttackRange, pTargetCreature->m_idSelf, 1, 0);
                    auto pNearestCreature = Globals::AppManager()->m_pServerExoApp->GetCreatureByGameObjectID(
                        nNearestEnemyID);

                    if (pNearestCreature)
                    {
                        pCombatRound->m_oidNewAttackTarget = nNearestEnemyID;
                        pCombatRound->AddCleaveAttack(nNearestEnemyID, 0);
                        thisPtr->m_bPassiveAttackBehaviour = 1;
                        if (!bHasGreatCleave)
                        {
                            pCombatRound->m_nCleaveAttacks--;
                        }
                    }
                }
            }
        }

        if (nTotalDamage < 1 && pCombatRound->m_bWeaponSucks == 0 &&
            !thisPtr->GetIsWeaponEffective(pTarget->m_idSelf, pCurrentAttack->m_nAttackType == 2)
            )
        {
            auto pMessage = new CNWCCMessageData();
            pMessage->m_nType = 3;
            pMessage->SetInteger(0, 117);
            pCurrentAttack->m_alstPendingFeedback.Add(pMessage);
            pCombatRound->m_bWeaponSucks = 1;
        }
    }
}

int32_t Combat::CNWSCreatureStats__GetDamageRoll(NWNXLib::API::CNWSCreatureStats *thisPtr, CNWSObject *pTarget,
                                                 int32_t bOffhand, int32_t bCritical, int32_t bSneak, int32_t bDeath,
                                                 int32_t bKiDamage)
{
    auto pCombatRound = thisPtr->m_pBaseCreature->m_pcCombatRound;
    CNWSItem *pAttackWeapon = nullptr;
    int nDifficultyOption = 100;
    int nWeightParam1 = 0, nWeightParam2 = 0;
    int nDamage = 0;

    //Get current weapon
    if (bOffhand)
        pAttackWeapon = GetDualwieldWeapon(thisPtr->m_pBaseCreature);
    else
        pAttackWeapon = pCombatRound->GetCurrentAttackWeapon(0);

    //Get weight parameters
    if (pTarget)
    {
        CNWSCreature *pTargetCreature = Utils::AsNWSCreature(pTarget);
        if (pTargetCreature)
        {
            if (thisPtr->m_bIsPC)
            {
                if (!pTargetCreature->m_bPlayerCharacter && !pTargetCreature->GetIsPossessedFamiliar())
                {
                    nWeightParam1 = 1;
                    nDifficultyOption = Globals::AppManager()->m_pServerExoApp->GetDifficultyOption(3);
                }
            }
            else
            {
                if (pTargetCreature->m_bPlayerCharacter || pTargetCreature->GetIsPossessedFamiliar())
                {
                    nWeightParam2 = 1;
                    nDifficultyOption = Globals::AppManager()->m_pServerExoApp->GetDifficultyOption(4);
                }
            }
        }
    }

    //If unarmed attack
    if (!pAttackWeapon || pAttackWeapon->m_nBaseItem == Constants::BaseItem::Gloves
        || pAttackWeapon->m_nBaseItem == Constants::BaseItem::Bracer)
    {
        if (!bCritical)
        {
            if (!bKiDamage)
                nDamage = thisPtr->GetUnarmedDamageRoll(pTarget);
            else
                nDamage = thisPtr->GetUnarmedDamageDice() * thisPtr->GetUnarmedDamageDie();
        }
        else
        {
            int bOverwhelming = thisPtr->GetEpicWeaponOverwhelmingCritical(pAttackWeapon);
            int nMultiplier = thisPtr->GetCriticalHitMultiplier(bOffhand);
            for (int i = 0; i < nMultiplier; i++)
            {
                nDamage += thisPtr->GetUnarmedDamageRoll(pTarget);
            }
            //TODO: Add variable to fix extra 1d6 damage
            if (bOverwhelming)
                nDamage += Globals::Rules()->RollDice(nMultiplier, 6); //Should be nMultiplier - 1
        }

    }
    else
    {
        //Not unarmed attack
        auto nWeaponType = pCombatRound->GetWeaponAttackType();
        if (!bKiDamage)
        {
            //Creature weapons
            if (nWeaponType == 3 || nWeaponType == 4 || nWeaponType == 5)
            {
                if (pAttackWeapon->GetPropertyByTypeExists(Constants::ItemProperty::MonsterDamage, 0))
                {
                    CNWItemProperty *pItemProperty;
                    int nCost = 0;
                    if (pAttackWeapon->GetPropertyByType(&pItemProperty, Constants::ItemProperty::MonsterDamage, 0) &&
                        pItemProperty && (nCost = pItemProperty->m_nCostTableValue) != 0)
                    {
                        int nDice, nDie;
                        Globals::Rules()->m_p2DArrays->m_pIPRPMonsterDamageTable->GetINTEntry(nCost,
                            CExoString("NumDice"), &nDice);
                        Globals::Rules()->m_p2DArrays->m_pIPRPMonsterDamageTable->GetINTEntry(nCost, CExoString("Die"),
                            &nDie);
                        if (bCritical)
                        {
                            int bOverwhelming = thisPtr->GetEpicWeaponOverwhelmingCritical(pAttackWeapon);
                            int nMultiplier = thisPtr->GetCriticalHitMultiplier(bOffhand);
                            for (int i = 0; i < nMultiplier; i++)
                            {
                                int nRoll = Globals::Rules()->RollDice(nDice, nDie);
                                nDamage += Globals::Rules()->GetWeightedDamageAmount(nRoll, nDice * nDie, nWeightParam1,
                                    nWeightParam2, nDifficultyOption);
                            }
                            //TODO: Add variable to fix extra 1d6 damage
                            if (bOverwhelming)
                                nDamage += Globals::Rules()->RollDice(nMultiplier, 6); //Should be nMultiplier - 1
                        }
                        else
                        {
                            int nRoll = Globals::Rules()->RollDice(nDice, nDie);
                            nDamage = Globals::Rules()->GetWeightedDamageAmount(nRoll, nDice * nDie, nWeightParam1,
                                nWeightParam2, nDifficultyOption);
                        }
                    }
                }
            }
            else
            { //Regular weapons
                auto pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pAttackWeapon->m_nBaseItem);
                int nDice = pBaseItem->m_nNumDice;
                int nDie = pBaseItem->m_nDieToRoll;
                if (bCritical)
                {
                    int bOverwhelming = thisPtr->GetEpicWeaponOverwhelmingCritical(pAttackWeapon);
                    int nMultiplier = thisPtr->GetCriticalHitMultiplier(bOffhand);
                    for (int i = 0; i < nMultiplier; i++)
                    {
                        int nRoll = Globals::Rules()->RollDice(nDice, nDie);
                        nDamage += Globals::Rules()->GetWeightedDamageAmount(nRoll, nDice * nDie, nWeightParam1,
                            nWeightParam2, nDifficultyOption);
                    }
                    //TODO: Add variable to fix extra 1d6 damage
                    if (bOverwhelming)
                        nDamage += Globals::Rules()->RollDice(nMultiplier, 6); //Should be nMultiplier - 1
                }
                else
                {
                    int nRoll = Globals::Rules()->RollDice(nDice, nDie);
                    nDamage = Globals::Rules()->GetWeightedDamageAmount(nRoll, nDice * nDie, nWeightParam1,
                        nWeightParam2, nDifficultyOption);
                }
            }
        }
        else
        { //Ki damage
            int nDice = 0, nDie = 0;
            //Creature weapons
            if (nWeaponType == 3 || nWeaponType == 4 || nWeaponType == 5)
            {
                if (pAttackWeapon->GetPropertyByTypeExists(Constants::ItemProperty::MonsterDamage, 0))
                { //Monster Damage
                    CNWItemProperty *pItemProperty;
                    int nCost = 0;
                    if (pAttackWeapon->GetPropertyByType(&pItemProperty, Constants::ItemProperty::MonsterDamage, 0) &&
                        pItemProperty && (nCost = pItemProperty->m_nCostTableValue) != 0)
                    {
                        Globals::Rules()->m_p2DArrays->m_pIPRPMonsterDamageTable->GetINTEntry(nCost,
                            CExoString("NumDice"), &nDice);
                        Globals::Rules()->m_p2DArrays->m_pIPRPMonsterDamageTable->GetINTEntry(nCost, CExoString("Die"),
                            &nDie);
                    }
                }
            }
            else
            { //Regular weapons
                if (nWeaponType == 7 || nWeaponType == 8)
                {
                    nDice = thisPtr->GetUnarmedDamageDice();
                    nDie = thisPtr->GetUnarmedDamageDie();
                }
                else
                {
                    auto pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pAttackWeapon->m_nBaseItem);
                    nDice = pBaseItem->m_nNumDice;
                    nDie = pBaseItem->m_nDieToRoll;
                }
            }
            nDamage = nDice * nDie;

            if (bCritical)
            {
                int bOverwhelming = thisPtr->GetEpicWeaponOverwhelmingCritical(pAttackWeapon);
                int nMultiplier = thisPtr->GetCriticalHitMultiplier(bOffhand);
                for (int i = 1; i < nMultiplier; i++)
                {
                    nDamage += Globals::Rules()->RollDice(nDice, nDie);
                }
                //TODO: Fix overwhelming attack only adding 1d6 damage on crits
                if (bOverwhelming)
                    nDamage += Globals::Rules()->RollDice(1, 6); //should be nMultiplier d6
            }
        }

        //Adjust damage from NoDamage property
        if (pAttackWeapon->GetPropertyByTypeExists(Constants::ItemProperty::NoDamage, 0))
        { //NoDamage
            nDamage = 0;
        }
    }

    if (bSneak)
    {
        int nSneakAttackDice = GetSneakAttackDice(thisPtr->m_pBaseCreature);
        nDamage += Globals::Rules()->RollDice(nSneakAttackDice, 6);
    }

    if (bDeath)
    {
        int nDeathAttackDice = GetDeathAttackDice(thisPtr->m_pBaseCreature);
        nDamage += Globals::Rules()->RollDice(nDeathAttackDice, 6);
    }

    CNWSCreature *pTargetCreature = nullptr;
    if (pTarget)
    {
        pTargetCreature = Utils::AsNWSCreature(pTarget);
    }
    auto nBonusDamage = thisPtr->GetDamageBonus(pTargetCreature, bOffhand);

    if (bCritical)
    {
        int nMultiplier = thisPtr->GetCriticalHitMultiplier(bOffhand);
        for (int i = 0; i < nMultiplier; i++)
        {
            nDamage += nBonusDamage;
        }

        if (pAttackWeapon && pAttackWeapon->GetPropertyByTypeExists(Constants::ItemProperty::MassiveCriticals, 0))
        { //Massive critical
            CNWItemProperty *pItemProperty;
            int nCost = 0;
            if (pAttackWeapon->GetPropertyByType(&pItemProperty, Constants::ItemProperty::MassiveCriticals, 0) &&
                pItemProperty && (nCost = pItemProperty->m_nCostTableValue) != 0
                )
            {

                int nDice, nDie;
                Globals::Rules()->m_p2DArrays->m_pIPRPDamageTable->GetINTEntry(nCost, CExoString("NumDice"), &nDice);
                Globals::Rules()->m_p2DArrays->m_pIPRPDamageTable->GetINTEntry(nCost, CExoString("Die"), &nDie);
                if (nDice > 0)
                {
                    nDamage += Globals::Rules()->RollDice(nDice, nDie);
                }
            }
        }
    }
    else
    {
        nDamage += nBonusDamage;
    }

    nBonusDamage = thisPtr->m_pBaseCreature->GetTotalEffectBonus(2, pTarget, 0, bKiDamage, 0, 0, -1, -1, 0);
    nDamage += nBonusDamage;

    //Wut? No clue what this is supposed to do. Overflow check?
    if (nDamage < nBonusDamage)
        nDamage = nBonusDamage;
    if (nDamage < 1)
        nDamage = 1;

    if (pTarget)
    {
        auto nDamageFlags = thisPtr->m_pBaseCreature->GetDamageFlags();
        auto nDamagePower = thisPtr->m_pBaseCreature->CalculateDamagePower(pTarget, bOffhand);
        nDamage = pTarget->DoDamageImmunity(thisPtr->m_pBaseCreature, nDamage, nDamageFlags, bKiDamage, 1);
        nDamage = pTarget->DoDamageResistance(thisPtr->m_pBaseCreature, nDamage, nDamageFlags, bKiDamage, 1, 1);
        nDamage = pTarget->DoDamageReduction(thisPtr->m_pBaseCreature, nDamage, nDamagePower, bKiDamage, 1);
        if (nDamage < 0)
        {
            nDamage = 0;
        }
    }

    return nDamage;
}

int32_t Combat::CNWSCreature__GetIsWeaponEffective(NWNXLib::API::CNWSCreature *thisPtr, uint32_t nObjectID,
                                                   int32_t bOffhand)
{

    auto pGameObject = Globals::AppManager()->m_pServerExoApp->GetGameObject(nObjectID);
    CNWSObject *pObject = nullptr;

    if (pGameObject)
        pObject = Utils::AsNWSObject(pGameObject);

    if (pObject && pObject->m_bPlotObject)
        return 0;

    auto nDamage = thisPtr->m_pStats->GetDamageRoll(pObject, bOffhand, 0, 0, 0, 1);
    auto nElementalDamage = thisPtr->CalculateMaxElementalDamage(pObject, bOffhand);

    if (nDamage + nElementalDamage > 0)
        return 1;

    CNWSItem *pItem = nullptr;
    if (!bOffhand)
        pItem = thisPtr->m_pcCombatRound->GetCurrentAttackWeapon(0);
    else
        pItem = GetDualwieldWeapon(thisPtr);

    if (!pItem)
        return 0;

    if (!pItem->GetPropertyByTypeExists(Constants::ItemProperty::OnHitCastSpell, 0))
        return 0;

    return 1;
}

int32_t Combat::CNWSCreatureStats__GetDamageBonus(NWNXLib::API::CNWSCreatureStats *thisPtr,
                                                  NWNXLib::API::CNWSCreature *pTarget, int32_t bOffhand)
{

    auto pCombatData = thisPtr->m_pBaseCreature->m_pcCombatRound;
    auto pAttackData = pCombatData->GetAttack(pCombatData->m_nCurrentAttack);

    CNWSItem *pItem = nullptr;
    int nStrengthMod = thisPtr->m_nStrengthModifier;
    if (!bOffhand)
    {
        auto pItemRight = thisPtr->m_pBaseCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);
        if (pItemRight)
        {
            //TODO: Monkey Grip?
            auto pBaseItem = Globals::Rules()->m_pBaseItemArray->GetBaseItem(pItemRight->m_nBaseItem);
            if ((pBaseItem->m_nWeaponSize - thisPtr->m_pBaseCreature->m_nCreatureSize) != 1
                || pAttackData->m_bRangedAttack)
            {
                nStrengthMod = 0;
                if (!pAttackData->m_bRangedAttack || (pBaseItem->m_nWeaponWield == 11 && pItemRight->m_nBaseItem != 59))
                {
                    nStrengthMod = thisPtr->m_nStrengthModifier;
                }
                else
                {
                    if (pAttackData->m_bRangedAttack)
                    {
                        if (thisPtr->m_nStrengthModifier < 0 && pItemRight->m_nBaseItem != 7
                            && pItemRight->m_nBaseItem != 6 && pItemRight->m_nBaseItem != 59)
                        {
                            nStrengthMod = thisPtr->m_nStrengthModifier;
                        }
                        CNWItemProperty *pItemProp;
                        if (pItemRight->GetPropertyByType(&pItemProp, 0x2d, 0))
                        {
                            nStrengthMod = thisPtr->m_nStrengthModifier < pItemProp->m_nCostTableValue ?
                                           thisPtr->m_nStrengthModifier : pItemProp->m_nCostTableValue;
                        }
                    }
                }
            }
            else
            {
                if (nStrengthMod > 0 && pBaseItem->m_nWeaponWield != 8)
                    nStrengthMod *= 1.5f;
            }
        }
    }
    else
    {
        if (nStrengthMod > 0)
            nStrengthMod *= 0.5f;
    }

    if (pAttackData->m_bRangedAttack)
        if (thisPtr->HasFeat(Constants::Feat::PointBlankShot))
            if (pTarget && GetDistance(thisPtr->m_pBaseCreature->m_vPosition, pTarget->m_vPosition) < 25.0f)
                nStrengthMod++;

    auto pWeapon = pCombatData->GetCurrentAttackWeapon(pAttackData->m_nWeaponAttackType);
    if (pWeapon && (pWeapon->m_nBaseItem == 8 || pWeapon->m_nBaseItem == 11))
    {
        //Get Enhanced arrows
        auto nBaseFeat = Constants::Feat::PrestigeEnchantArrow6 - 6;
        int i;
        for (i = 20; i > 5; i--)
        {
            if (thisPtr->HasFeat(nBaseFeat + i))
            {
                nStrengthMod += i;
                break;
            }
        }
        if (i < 6)
        {
            for (nBaseFeat = Constants::Feat::PrestigeEnchantArrow1 - 1; i > 0; i--)
            {
                if (thisPtr->HasFeat(nBaseFeat + i))
                {
                    nStrengthMod += i;
                    break;
                }
            }
        }
    }

    if (pTarget)
    {
        auto nFavoredEnemyBonus = thisPtr->GetFavoredEnemyBonus(pTarget);
        if (nFavoredEnemyBonus < 1)
        {
            nFavoredEnemyBonus = 0;
        }
        else
        {
            if (thisPtr->HasFeat(Constants::Feat::EpicBaneOfEnemies))
            {
                nFavoredEnemyBonus += Globals::Rules()->RollDice(2, 6);
            }
        }
        nStrengthMod += nFavoredEnemyBonus;
    }

    //TODO: Rework/hook power attack?
    if (thisPtr->m_pBaseCreature->m_nCombatMode == Constants::CombatMode::PowerAttack)
    {
        nStrengthMod += 5;
    }
    else
    {
        if (thisPtr->m_pBaseCreature->m_nCombatMode == Constants::CombatMode::ImprovedPowerAttack)
        {
            nStrengthMod += 10;
        }
    }

    if (thisPtr->m_pBaseCreature->m_nCombatMode == Constants::CombatMode::DirtyFighting)
        nStrengthMod += Globals::Rules()->RollDice(1, 4);

    if (thisPtr->GetWeaponSpecialization(pWeapon))
        nStrengthMod += 2;

    if (thisPtr->GetEpicWeaponSpecialization(pWeapon))
        nStrengthMod += 4;

    nStrengthMod += thisPtr->ResolveSpecialAttackDamageBonus(pTarget);

    return nStrengthMod;
}

int32_t Combat::CNWSObject__DoDamageImmunity(NWNXLib::API::CNWSObject *thisPtr, NWNXLib::API::CNWSCreature *pAttacker,
                                             int32_t nDamage, uint16_t nFlags, int32_t bSimulate, int32_t bCombatDamage)
{

    auto nImmunityPercent = thisPtr->GetDamageImmunityByFlags(nFlags);
    if (nImmunityPercent < 1)
        return nDamage;

    auto nReducedDamage = nDamage * nImmunityPercent / 100;
    nReducedDamage = nReducedDamage > 0 ? nReducedDamage : 1;
    nDamage -= nReducedDamage;

    if (bSimulate)
        return nDamage;

    if (bCombatDamage && pAttacker)
    {
        auto pMessage = new CNWCCMessageData();
        pMessage->SetObjectID(0, thisPtr->m_idSelf);
        pMessage->SetInteger(0, 0x3e);
        pMessage->SetInteger(1, nReducedDamage);
        pMessage->SetInteger(2, nFlags);
        auto pAttack = pAttacker->m_pcCombatRound->GetAttack(pAttacker->m_pcCombatRound->m_nCurrentAttack);
        pAttack->m_alstPendingFeedback.Add(pMessage);
        return nDamage;
    }

    CNWCCMessageData *pMessage = new CNWCCMessageData();
    CNWCCMessageData *pMessage2 = new CNWCCMessageData();
    pMessage->SetObjectID(0, thisPtr->m_idSelf);
    pMessage->SetInteger(0, 0x3e);
    pMessage->SetInteger(1, nReducedDamage);
    pMessage->SetInteger(2, nFlags);
    pMessage->CopyTo(pMessage2);

    auto pCreature = Utils::AsNWSCreature(thisPtr);
    if (pCreature)
        pCreature->SendFeedbackMessage(0x3e, pMessage, nullptr);
    else
        delete pMessage;

    if (pAttacker)
    {
        pAttacker->SendFeedbackMessage(0x3e, pMessage2, nullptr);
    }
    else
    {
        delete pMessage2;
    }

    return nDamage;
}

int32_t Combat::CNWSObject__DoDamageReduction(NWNXLib::API::CNWSObject *thisPtr, NWNXLib::API::CNWSCreature *pAttacker,
                                              int32_t nDamage, unsigned char nDamagePower, int32_t bSimulate,
                                              int32_t bCombatDamage)
{

    auto pCreature = Utils::AsNWSCreature(thisPtr);
    int nHighestReductionPower = 0;
    uint16_t nReductionEffectPos = (uint16_t) -1u;
    int nPerfectSelfPower = 0;
    int nFeatReduction = 0;
    int nReduction = 0;

    for (uint16_t i = pCreature ? pCreature->m_pStats->m_nDamageReductionPtr : 0;
         i < thisPtr->m_appliedEffects.num &&
             thisPtr->m_appliedEffects.element[i]->m_nType <= Constants::EffectTrueType::DamageReduction;
         i++)
    {
        if (thisPtr->m_appliedEffects.element[i]->m_nType == Constants::EffectTrueType::DamageReduction)
        {
            auto pEffect = thisPtr->m_appliedEffects.element[i];
            auto nReductionAmount = pEffect->GetInteger(0);
            auto nReductionPower = pEffect->GetInteger(1);
            if (nReductionPower > nDamagePower && nReduction < nReductionAmount)
            {
                nReduction = nReductionAmount;
                nHighestReductionPower = nReductionPower;
                nReductionEffectPos = i;
            }
        }
    }

    if (pCreature)
    {
        if (pCreature->m_pStats->HasFeat(Constants::Feat::EpicBarbarianDamageReduction))
        {
            int nBarbarianLevel = pCreature->m_pStats->GetNumLevelsOfClass(Constants::ClassType::Barbarian);
            if (nBarbarianLevel < 26)
                nFeatReduction = 5;
            else
            {
                int nBarbReduction = (nBarbarianLevel - 23) / 3.0f + 5; //TODO: Check if std::round() needed
                if (nBarbReduction > 0)
                    nFeatReduction = nBarbReduction;
            }
        }
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::DamageReduction4))
            nFeatReduction = 4;
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::DamageReduction3))
            nFeatReduction = 3;
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::DamageReduction2))
            nFeatReduction = 2;
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::DamageReduction))
            nFeatReduction = 1;

        if (pCreature->m_pStats->HasFeat(Constants::Feat::DamageReduction6))
        {
            int nDefenderLevels = pCreature->m_pStats->GetNumLevelsOfClass(Constants::ClassType::DwarvenDefender);
            //TODO: Make sure it is the right formula
            if (nDefenderLevels > 9)
                nFeatReduction += ((nDefenderLevels - 6) >> 2) * 3 + 3; //(Level-6)/4*3+3
            else
                nFeatReduction += 3;
        }

        if (pCreature->m_pStats->HasFeat(Constants::Feat::PerfectSelf) && nDamagePower == 0 && nFeatReduction < 20)
        {
            nFeatReduction = 20;
            nPerfectSelfPower = 1;
        }

        if (pCreature->m_pStats->HasFeat(Constants::Feat::EpicDamageReduction9))
            nFeatReduction += 9;
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::EpicDamageReduction6))
            nFeatReduction += 6;
        else if (pCreature->m_pStats->HasFeat(Constants::Feat::EpicDamageReduction3))
            nFeatReduction += 3;

        if (nFeatReduction
            && (nHighestReductionPower <= nDamagePower || nReduction <= nFeatReduction ||
                (nPerfectSelfPower && nPerfectSelfPower <= nDamagePower && nFeatReduction < 1)))
        { //If feat reduction is stronger than effect reduction
            int nReducedAmount = nFeatReduction < nDamage ? nFeatReduction : nDamage;
            nDamage -= nFeatReduction;

            if (!bSimulate)
            {
                if (bCombatDamage && pAttacker)
                {
                    auto pMessage = new CNWCCMessageData();
                    pMessage->SetObjectID(0, thisPtr->m_idSelf);
                    pMessage->SetInteger(0, 0x40);
                    pMessage->SetInteger(1, nReducedAmount);
                    auto pAttack = pAttacker->m_pcCombatRound->GetAttack(pAttacker->m_pcCombatRound->m_nCurrentAttack);
                    pAttack->m_alstPendingFeedback.Add(pMessage);

                    return nDamage > 0 ? nDamage : 0;
                }

                auto pMessage = new CNWCCMessageData();
                auto pMessage2 = new CNWCCMessageData();
                pMessage->SetObjectID(0, thisPtr->m_idSelf);
                pMessage->SetInteger(1, nReducedAmount);
                pMessage->CopyTo(pMessage2);

                if (pCreature)
                    pCreature->SendFeedbackMessage(0x40, pMessage, nullptr);
                else
                    delete pMessage;
                if (pAttacker)
                    pAttacker->SendFeedbackMessage(0x40, pMessage2, nullptr);
                else
                    delete pMessage2;

                return nDamage > 0 ? nDamage : 0;
            }
            return nDamage > 0 ? nDamage : 0;
        }
    }

    if (!nReduction || nHighestReductionPower < nDamagePower)
        return nDamage;

    int nRemainingEffectReduction = 0;
    bool bApplyReduction = false;
    if (nReductionEffectPos == (uint16_t) -1u
        || !(nRemainingEffectReduction = pCreature->m_appliedEffects.element[nReductionEffectPos]->GetInteger(2)))
    {
        if (!bSimulate)
            bApplyReduction = true;
    }
    else
    {
        nReduction = nDamage < nReduction ? nDamage : nReduction;
        if (nRemainingEffectReduction <= nReduction)
        {
            nReduction = nRemainingEffectReduction;
            if (!bSimulate)
            {
                auto pEffect = pCreature->m_appliedEffects.element[nReductionEffectPos];
                pCreature->RemoveEffectById(pEffect->m_nID);
                bApplyReduction = true;
            }
        }
        else
        {
            if (!bSimulate)
            {
                auto pEffect = pCreature->m_appliedEffects.element[nReductionEffectPos];
                pEffect->SetInteger(2, nRemainingEffectReduction - nReduction);
                bApplyReduction = true;
            }
        }
    }

    if (bApplyReduction)
    {
        if (bCombatDamage && pAttacker)
        {
            auto pMessage = new CNWCCMessageData();
            pMessage->SetObjectID(0, thisPtr->m_idSelf);
            nReduction = nDamage < nReduction ? nDamage : nReduction;
            if (nRemainingEffectReduction == 0)
            {
                pMessage->SetInteger(0, 0x40);
                pMessage->SetInteger(1, nReduction);
            }
            else
            {
                pMessage->SetInteger(0, 0x43);

                if (nRemainingEffectReduction <= nReduction)
                {
                    pMessage->SetInteger(1, nRemainingEffectReduction);
                    pMessage->SetInteger(2, 0);
                }
                else
                {
                    pMessage->SetInteger(1, nReduction);
                    pMessage->SetInteger(2, nRemainingEffectReduction - nReduction);
                }
            }
            auto pAttack = pAttacker->m_pcCombatRound->GetAttack(pAttacker->m_pcCombatRound->m_nCurrentAttack);
            pAttack->m_alstPendingFeedback.Add(pMessage);
        }
        else
        {
            CNWCCMessageData *pMessage = new CNWCCMessageData();
            CNWCCMessageData *pMessage2 = new CNWCCMessageData();
            pMessage->SetObjectID(0, thisPtr->m_idSelf);
            int nReduced = nReduction < nDamage ? nReduction : nDamage;

            if (nRemainingEffectReduction == 0)
            {
                pMessage->SetInteger(1, nReduced);
                pMessage->CopyTo(pMessage2);

                if (pCreature)
                    pCreature->SendFeedbackMessage(0x40, pMessage, nullptr);
                else
                    delete pMessage;

                if (pAttacker)
                    pAttacker->SendFeedbackMessage(0x40, pMessage2, nullptr);
                else
                    delete pMessage2;
            }
            else
            {
                if (nRemainingEffectReduction <= nReduction)
                {
                    pMessage->SetInteger(1, nRemainingEffectReduction);
                    pMessage->SetInteger(2, 0);
                }
                else
                {
                    pMessage->SetInteger(1, nReduction);
                    pMessage->SetInteger(2, nRemainingEffectReduction - nReduction);
                }
                pMessage->CopyTo(pMessage2);

                if (pCreature)
                    pCreature->SendFeedbackMessage(0x43, pMessage, nullptr);
                else
                    delete pMessage;

                if (pAttacker)
                    pAttacker->SendFeedbackMessage(0x43, pMessage2, nullptr);
                else
                    delete pMessage2;
            }
        }
    }

    nDamage = nReduction > nDamage ? 0 : nDamage - nReduction;
    return nDamage;
}

unsigned char Combat::CNWSCreature__CalculateDamagePower(NWNXLib::API::CNWSCreature *thisPtr, CNWSObject *pTarget,
                                                         int32_t bOffhand)
{
    auto nWeaponPower = thisPtr->GetWeaponPower(pTarget, bOffhand);
    if (nWeaponPower > 6)
        nWeaponPower++;

    if (thisPtr->m_pStats->GetUseMonkAttackTables(1))
    {
        if (thisPtr->m_pStats->HasFeat(Constants::Feat::EpicImprovedKiStrike5))
        {
            if (nWeaponPower < 5)
                nWeaponPower = 5;
        }
        else if (thisPtr->m_pStats->HasFeat(Constants::Feat::EpicImprovedKiStrike4))
        {
            if (nWeaponPower < 4)
                nWeaponPower = 4;
        }
        else if (thisPtr->m_pStats->HasFeat(Constants::Feat::KiStrike3))
        {
            if (nWeaponPower < 3)
                nWeaponPower = 3;
        }
        else if (thisPtr->m_pStats->HasFeat(Constants::Feat::KiStrike2))
        {
            if (nWeaponPower < 2)
                nWeaponPower = 2;
        }
        else if (thisPtr->m_pStats->HasFeat(Constants::Feat::KiStrike))
        {
            if (nWeaponPower < 1)
                nWeaponPower = 1;
        }
    }

    auto pCombatRound = thisPtr->m_pcCombatRound;
    auto pCurrentAttack = pCombatRound->GetAttack(pCombatRound->m_nCurrentAttack);

    if (pCurrentAttack->m_bRangedAttack)
    {
        auto pItem = thisPtr->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::RightHand);
        if (pItem && (pItem->m_nBaseItem == 8 || pItem->m_nBaseItem == 11))
        {
            for (int i = 20; i > 5; i--)
            {
                if (nWeaponPower < i)
                {
                    if (thisPtr->m_pStats->HasFeat(Constants::Feat::PrestigeEnchantArrow6 + i - 1))
                        return i;
                }
                else
                {
                    break;
                }
            }
            for (int i = 5; i > 0; i--)
            {
                if (nWeaponPower < i)
                {
                    if (thisPtr->m_pStats->HasFeat(Constants::Feat::PrestigeEnchantArrow1 + i - 1))
                        return i;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return nWeaponPower;
}

int16_t Combat::CNWSCreature__GetArmorClass(CNWSCreature *thisPtr)
{
    auto pStats = thisPtr->m_pStats;
    char nDodgeAC = pStats->m_nACDodgeMod - pStats->m_nACDodgeNeg;
    if (nDodgeAC > 20)
    {
        nDodgeAC = 20;
    }
    char nNaturalAC = pStats->GetACNaturalBase(0);
    char nSkillAC = pStats->GetTotalACSkillMod();
    char nDexAC = pStats->GetDEXMod(1);

    return 10 + nNaturalAC + nDexAC + nDodgeAC + nSkillAC + pStats->m_nACNaturalMod - pStats->m_nACNaturalNeg +
        pStats->m_nACShieldBase + pStats->m_nACShieldMod - pStats->m_nACShieldNeg +
        pStats->m_nACArmorBase + pStats->m_nACArmorMod - pStats->m_nACArmorNeg +
        pStats->m_nACDeflectionMod - pStats->m_nACDeflectionNeg;
}

char Combat::CNWSCreatureStats__GetACNaturalBase(CNWSCreatureStats *thisPtr, int32_t bTouchAttack)
{
    char nNaturalAC = 0;

    if (thisPtr->m_pBaseCreature->GetUseMonkAbilities())
    {
        if (thisPtr->HasFeat(Constants::Feat::MonkAcBonus) && thisPtr->m_nWisdomModifier > 0)
            nNaturalAC = thisPtr->m_nWisdomModifier;

        auto nMonkIndex = thisPtr->GetIsClass(Constants::ClassType::Monk);
        if (nMonkIndex < thisPtr->m_nNumMultiClasses)
            nNaturalAC += thisPtr->m_ClassInfo[nMonkIndex].m_nLevel / 5;
    }

    if (bTouchAttack)
        return nNaturalAC;

    int nClassIndex = thisPtr->GetIsClass(Constants::ClassType::EyeOfGruumsh);
    if (nClassIndex < thisPtr->m_nNumMultiClasses)
        nNaturalAC += thisPtr->m_ClassInfo[nClassIndex].m_nLevel / 3;

    if (thisPtr->HasFeat(Constants::Feat::EpicArmorSkin))
        nNaturalAC += 2;

    nClassIndex = thisPtr->GetIsClass(Constants::ClassType::Palemaster);
    if (nClassIndex < thisPtr->m_nNumMultiClasses && thisPtr->HasFeat(Constants::Feat::BoneSkin2))
        nNaturalAC += thisPtr->m_ClassInfo[nClassIndex].m_nLevel / 2 + 2;

    nClassIndex = thisPtr->GetIsClass(Constants::ClassType::DragonDisciple);
    if (nClassIndex < thisPtr->m_nNumMultiClasses && thisPtr->HasFeat(Constants::Feat::DragonArmor))
    {
        int nDiscipleLevel = thisPtr->m_ClassInfo[nClassIndex].m_nLevel;
        if (nDiscipleLevel < 5)
            nNaturalAC += 1;
        else if (nDiscipleLevel < 8)
            nNaturalAC += 2;
        else if (nDiscipleLevel < 10)
            nNaturalAC += 3;
        else
            nNaturalAC += 4 + (nDiscipleLevel - 10) / 5;
    }

    return nNaturalAC;
}

char Combat::CNWSCreatureStats__GetTotalACSkillMod(CNWSCreatureStats *thisPtr)
{
    char nSkillAC = 0;

    //TODO: Use new rulesets variable in preparation for x64
    char nTumbleRanks = thisPtr->GetSkillRank(Constants::Skill::Tumble, 0, 1);
    if (nTumbleRanks > 5)
        nSkillAC += nTumbleRanks / 5;

    return nSkillAC;
}

char Combat::CNWSCreatureStats__GetDEXMod(CNWSCreatureStats *thisPtr, int32_t bArmorDexCap)
{
    char nDexAC = thisPtr->m_nDexterityModifier;

    if (bArmorDexCap)
    {
        auto pArmor = thisPtr->m_pBaseCreature->m_pInventory->GetItemInSlot(Constants::EquipmentSlot::Chest);
        int nTempValue = 0;
        if (pArmor && (nTempValue = pArmor->ComputeArmorClass()) > 0)
        {
            Globals::Rules()->m_p2DArrays->m_pArmorTable->GetINTEntry(nTempValue, CExoString("DEXBONUS"), &nTempValue);
            if (nTempValue > 0 && nTempValue < nDexAC)
                nDexAC = (char) nTempValue;
        }
    }

    return nDexAC;
}
}