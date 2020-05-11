/// @addtogroup combat Combat
/// @brief Functions that expose extra functionality related to the combat engine
/// @{
/// @file nwnx_combat.nss
#include "nwnx"

const string NWNX_Combat = "NWNX_Combat"; ///< @private

/// @brief Set custom modifiers and scripts for a special attack feat.
/// @param nFeat Feat ID of the special attack.
void NWNX_Combat_SetSpecialAttackInfo(int nFeat, int nAttackRangeType, int nDamageModifier = 0, int nAttackModifier = 0, string sTestRequirementsScript = "", string sAttackModScript = "", string sDamageModScript = "", string sPostDamageScript = "");

/// @brief Sets a new return value for the current function.
/// @param nRet returned value
void NWNX_Combat_SetReturnValue(int nRet);

/// @}

void NWNX_Combat_SetSpecialAttackInfo(int nFeat, int nAttackRangeType, int nDamageModifier = 0, int nAttackModifier = 0, string sTestRequirementsScript = "", string sAttackModScript = "", string sDamageModScript = "", string sPostDamageScript = "")
{
    string sFunc = "SetSpecialAttackInfo";

    NWNX_PushArgumentString(NWNX_Combat, sFunc, sPostDamageScript);
    NWNX_PushArgumentString(NWNX_Combat, sFunc, sDamageModScript);
    NWNX_PushArgumentString(NWNX_Combat, sFunc, sAttackModScript);
    NWNX_PushArgumentString(NWNX_Combat, sFunc, sTestRequirementsScript);
    NWNX_PushArgumentInt(NWNX_Combat, sFunc, nAttackModifier);
    NWNX_PushArgumentInt(NWNX_Combat, sFunc, nDamageModifier);
    NWNX_PushArgumentInt(NWNX_Combat, sFunc, nAttackRangeType);
    NWNX_PushArgumentInt(NWNX_Combat, sFunc, nFeat);

    NWNX_CallFunction(NWNX_Combat, sFunc);
}

void NWNX_Combat_SetReturnValue(int nRet)
{
    string sFunc = "SetReturnValue";

    NWNX_PushArgumentInt(NWNX_Combat, sFunc, nRet);

    NWNX_CallFunction(NWNX_Combat, sFunc);
}