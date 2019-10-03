# Dangerous Plugin Reference

## Description

Rewritten engine functions that aren't properly tested yet and can potentially break the game, exposing some hardcoded mechanics. Use at your own risk.

## Environment Variables

| Variable Name                |  Type                   | Default Value                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| `NWNX_DANGEROUS_PREVENT_AB_BYPASSING_SOAK` | bool                  | false                  |
| `NWNX_DANGEROUS_USE_EPIC_DODGE_EVENT` | bool                  | false                  |
| `NWNX_DANGEROUS_USE_DEATH_ATTACK_ROLL_EVENT` | bool                  | false                |
| `NWNX_DANGEROUS_FIX_OVERWHELMING_DAMAGE` | bool                  | false                    |
| `NWNX_DANGEROUS_USE_DEVASTATING_CRITICAL_EVENT` | bool                  | false             |
| `NWNX_DANGEROUS_USE_SNEAK_ATTACK_CHECK_EVENT` | bool                  | false               |
| `NWNX_DANGEROUS_USE_CRITICAL_CHECK_EVENT` | bool                  | false                   |
| `NWNX_DANGEROUS_USE_CRITICAL_HIT_DAMAGE_EVENT` | bool                  | false              |
| `NWNX_DANGEROUS_USE_SNEAK_DAMAGE_EVENT` | bool                  | false                     |

## Fixes
### NWNX_DANGEROUS_PREVENT_AB_BYPASSING_SOAK
Weapons with attack bonus will not bypass Damage Reduction from creatures (a.k.a. soak). Only enhancement bonuses will.

### NWNX_DANGEROUS_FIX_OVERWHELMING_DAMAGE
Overwhelming critical will behave as expected based on the description. Originally an extra 1d6 damage was added.
It will also fix the KiDamage bug where overwhelming critical only added 1d6 damage instead of (multiplier-1)d6.

## New events

### NWNX_DANGEROUS_ON_CRITICAL_HIT_CHECK

Requires env var NWNX_DANGEROUS_USE_CRITICAL_CHECK_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| TARGET_ID | int                  | Target creature ObjectID                  |
| CRIT_IMMUNE | bool                  | Target critical hit immunity                  |

Use NWNX_Events_SetEventResult() to set wether the attack should be a crit (1) or not (0).

### NWNX_DANGEROUS_ON_DEVASTATING_CRITICAL_ROLL

Requires env var NWNX_DANGEROUS_USE_DEVASTATING_CRITICAL_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| TARGET_ID | int                  | Target creature ObjectID                  |
| DEVASTATING_DC | int                  | Devastating critical DC calculated by the engine                  |
| CRIT_IMMUNE | bool                  | Target critical hit immunity                  |

Use NWNX_Events_SetEventResult() to set the new devastating critical DC.
Values lower than 1 or using NWNX_Events_SkipEvent() will skip the devastating critical roll.

### NWNX_DANGEROUS_ON_DEATH_ATTACK_ROLL

Requires env var NWNX_DANGEROUS_USE_DEATH_ATTACK_ROLL_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| TARGET_ID | int                  | Target creature ObjectID                  |
| DEATH_ATTACK_DC | int                  | Death attack DC calculated by the engine                  |

Use NWNX_Events_SetEventResult() to set the new death attack DC.
Values lower than 1 or using NWNX_Events_SkipEvent() will skip the death attack roll.

### NWNX_DANGEROUS_ON_EPIC_DODGE

Requires env var NWNX_DANGEROUS_USE_EPIC_DODGE_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| TARGET_ID | int                  | Target creature ObjectID                  |

Using NWNX_Events_SkipEvent() will prevent epic dodge from triggering.

### NWNX_DANGEROUS_ON_GET_SNEAK_DAMAGE

Requires env var NWNX_DANGEROUS_USE_SNEAK_DAMAGE_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| SNEAK_DICE | int                  | Number of dice used to roll sneak attack damage                 |
| DEATH_DICE | int                  | Number of dice used to roll sneak attack damage                 |
| SNEAK_DAMAGE | int                  | Sneak damage calculated as (SNEAK_DICE+DEATH_DICE)d6                 |

Use NWNX_Events_SetEventResult() to set the new sneak bonus damage. Negative values will be set to 0.

### NWNX_DANGEROUS_ON_GET_CRITICAL_DAMAGE

Requires env var NWNX_DANGEROUS_USE_CRITICAL_HIT_DAMAGE_EVENT=1

| Variable Name                |  Type                   | Description                      |
| ---------------------------- | :---------------------: | ---------------------------------- |
| OBJECT_SELF | int                  | Attacker creature ObjectID                  |
| BASE_DAMAGE | int                  | Regular damage applied by this attack                |
| EXTRA_DAMAGE | int                  | Bonus critical damage added on top of the base damage                 |

Use NWNX_Events_SetEventResult() to set the new critical bonus damage. Negative values will be set to 0.

### NWNX_DANGEROUS_ON_SNEAK_ATTACK_CHECK

Requires env var NWNX_DANGEROUS_USE_SNEAK_ATTACK_CHECK_EVENT=1

Not implemented yet.

