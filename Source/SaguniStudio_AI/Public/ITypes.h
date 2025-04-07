#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIDifficulty : uint8
{
    Recruit  UMETA(DisplayName = "Recruit"),
    Regular  UMETA(DisplayName = "Regular"),
    Veteran  UMETA(DisplayName = "Veteran")
};

UENUM(BlueprintType)
enum class EAIMovementMode : uint8
{
    Idle    UMETA(DisplayName = "Idle"),
    Walk    UMETA(DisplayName = "Walk"),
    Jog     UMETA(DisplayName = "Jog"),
    Sprint  UMETA(DisplayName = "Sprint"),
    Faster  UMETA(DisplayName = "Faster"),
};

UENUM(BlueprintType)
enum class EAISenses : uint8
{
    None     UMETA(DisplayName = "None", ToolTip = "No sense activated."),
    Sight    UMETA(DisplayName = "Sight", ToolTip = "Sense that allows AI to see targets."),
    Hearing  UMETA(DisplayName = "Hearing", ToolTip = "Sense that allows AI to hear sounds."),
    Damage   UMETA(DisplayName = "Damage", ToolTip = "Sense that detects damage taken.")
};


UENUM(BlueprintType)
enum class EAIStates : uint8
{
    None                UMETA(DisplayName = "None"),
    Idle                UMETA(DisplayName = "Idle"),
    Patrolling          UMETA(DisplayName = "Patrolling"),
    Chasing             UMETA(DisplayName = "Chasing"),
    Investigating       UMETA(DisplayName = "Investigating"),
    Attacking           UMETA(DisplayName = "Attacking"),
    Dead                UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EAttackingStates : uint8
{
    None        UMETA(DisplayName = "None"),

    Simple      UMETA(DisplayName = "Simple Attack", ToolTip = "A basic attack with no special strategy."),

    Combo       UMETA(DisplayName = "Combo Attack", ToolTip = "A sequence of attacks chained together."),

    Flanking    UMETA(DisplayName = "Flanking", ToolTip = "AI moves to the side or behind the enemy before attacking."),

    Retreating  UMETA(DisplayName = "Retreating", ToolTip = "AI moves away from the enemy to reposition or avoid damage."),

    Supporting  UMETA(DisplayName = "Supporting", ToolTip = "AI helps allies with buffs, healing, or distractions."),

    Staggered   UMETA(DisplayName = "Staggered", ToolTip = "AI is stunned or knocked back after taking a strong hit."),

    Cooldown    UMETA(DisplayName = "Cooldown", ToolTip = "AI is recovering and cannot attack temporarily.")
};


UENUM(BlueprintType)
enum class ESupportingStates : uint8
{
    None        UMETA(DisplayName = "None"),        
    Buffing     UMETA(DisplayName = "Buffing"),     
    Healing     UMETA(DisplayName = "Healing"),     
    Supporting    UMETA(DisplayName = "Supporting")  
};



UENUM(BlueprintType)
enum class EAIPositionRelation : uint8
{
    Unknown    UMETA(DisplayName = "Unknown"),
    Front      UMETA(DisplayName = "Front"),
    Side       UMETA(DisplayName = "Side"),
    Behind     UMETA(DisplayName = "Behind")
};
