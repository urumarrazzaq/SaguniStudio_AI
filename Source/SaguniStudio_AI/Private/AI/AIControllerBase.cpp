// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerBase.h"
#include "AI/EnemyBase.h" 
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Character.h"
#include <Actions/PawnAction.h>
#include "NavigationSystem.h" 
#include "NavigationPath.h"

AAIControllerBase::AAIControllerBase()
{
   
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->ConfigureSense(*SightConfig);
        AIPerceptionComponent->ConfigureSense(*HearingConfig);
        AIPerceptionComponent->ConfigureSense(*DamageConfig);
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
        SetPerceptionComponent(*AIPerceptionComponent);
    }
   
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    SetupPerceptionSystem();
    InitializeBlackBoardKeys();
}

void AAIControllerBase::BeginPlay()
{
    Super::BeginPlay(); 
}


void AAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    Enemy = Cast<AEnemyBase>(InPawn);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
        {
            SetAIState(Enemy->AIState);
            SetAIAttackingState(Enemy->AIAttackingState);
            UE_LOG(LogTemp, Warning, TEXT("AI State set and AI started after delay!"));
        }), 0.5f, false);
    RunAI();
}

void AAIControllerBase::SetupPerceptionSystem()
{
    if (!AIPerceptionComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AIPerceptionComponent Setup Failed"));
        return;
    }

    if (SightConfig)
    {
        SightConfig->SightRadius = 1200.0f;
        SightConfig->LoseSightRadius = 1500.0f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        AIPerceptionComponent->ConfigureSense(*SightConfig);
    }

    if (HearingConfig)
    {
        HearingConfig->HearingRange = 1800.0f;
        HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
        HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
        HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
        AIPerceptionComponent->ConfigureSense(*HearingConfig);
    }

    if (DamageConfig)
    {
        AIPerceptionComponent->ConfigureSense(*DamageConfig);
    }

    AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerceptionUpdated);

    UE_LOG(LogTemp, Warning, TEXT("AIPerceptionComponent Setup Successfully"));
}

void AAIControllerBase::RunAI()
{
    if (!Enemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIControllerBase: No Pawn found!"));
        return;
    }

    if (Enemy->BehaviorTree)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIControllerBase: Running Behavior Tree - %s"), *Enemy->BehaviorTree->GetName());

        if (Enemy->BehaviorTree->BlackboardAsset)
        {
            if (!UseBlackboard(Enemy->BehaviorTree->BlackboardAsset, BlackboardComp))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to initialize Blackboard!"));
                return;
            }
            RunBehaviorTree(Enemy->BehaviorTree);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AIControllerBase: No Blackboard found in BehaviorTree!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AIControllerBase: No Behavior Tree found in EnemyBase!"));
    }
}


void AAIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        if (!Actor || Cast<AEnemyBase>(Actor)) continue;  // Ignore invalid actors & enemies

        TargetActor = Actor;

        UE_LOG(LogTemp, Warning, TEXT("Perceived Actor: %s"), *Actor->GetName());

        FActorPerceptionBlueprintInfo PerceptionInfo;
        if (AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo))
        {
            for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)  // Loop through all sensed stimuli
            {
                if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI Perception:: Sight detected: %s"), *Actor->GetName());
                    HandleSenseSight(Actor);
                }
                else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI Perception:: Heard sound: %s"), *Stimulus.Tag.ToString());
                    HandleSenseHearing(Actor, Stimulus.StimulusLocation);
                }
                else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
                {
                    UE_LOG(LogTemp, Warning, TEXT("AI Perception:: Damage detected: %s"), *Actor->GetName());
                    HandleSenseDamage(Actor);
                }
            }
        }
    }
}



void AAIControllerBase::StopAI()
{
    if (BrainComponent)
    {
        BrainComponent->StopLogic(TEXT("Enemy Died"));
        UE_LOG(LogTemp, Warning, TEXT("AI Behavior Tree Stopped: Enemy Died"));
    }

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->ForgetAll();
        AIPerceptionComponent->Deactivate();
    }

    StopMovement();
    ClearFocus(EAIFocusPriority::Gameplay);

    SetActorTickEnabled(false);

    UE_LOG(LogTemp, Warning, TEXT("AI Fully Stopped!"));
}

void AAIControllerBase::OnUnPossess()
{
    Super::OnUnPossess();

	StopAI();
}



#pragma region Handle_Perceived_Senses


void AAIControllerBase::HandleSenseSight(AActor* InActor)
{
    UE_LOG(LogTemp, Warning, TEXT("Handling sight sense of: %s"), *InActor->GetName());
    switch (GetCurrentState())
    {
        case EAIStates::Idle:
        case EAIStates::Patrolling:
        case EAIStates::Chasing:
        case EAIStates::Attacking:
        case EAIStates::Investigating:
            SetStateAsAttacking(InActor);
            break;

        case EAIStates::Dead:
            SetStateAsDead();
            break;
    }
}


void AAIControllerBase::HandleSenseHearing(AActor* PerceiInActorvedActor, const FVector InLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("Handling hearing at location: %s"), *InLocation.ToString());
    SetStateAsInvestigating(InLocation);
}


void AAIControllerBase::HandleSenseDamage(AActor* InActor)
{
    UE_LOG(LogTemp, Warning, TEXT("Handling damage sense: AI is taking damage! %s"), *InActor->GetName());
    SetStateAsAttacking(InActor);
}


#pragma endregion 



#pragma region AI_State

EAIStates AAIControllerBase::GetCurrentState() const
{
    const EAIStates CurrentState = static_cast<EAIStates>(GetBlackboardComponent()->GetValueAsEnum(AIStateKeyName));
    return CurrentState;
}

EAttackingStates AAIControllerBase::GetCurrentAttackingState() const
{
    
    const EAttackingStates CurrentState = static_cast<EAttackingStates>(GetBlackboardComponent()->GetValueAsEnum(AIAttackingStateKeyName));
    return CurrentState;
}

void AAIControllerBase::SetAIState(EAIStates AIState)
{
    GetBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(AIState));

    UE_LOG(LogTemp, Warning, TEXT("SetAIState: %s"), *UEnum::GetValueAsString(AIState));
    UE_LOG(LogTemp, Warning, TEXT("Current AIState from Blackboard: %s"), *UEnum::GetValueAsString(GetCurrentState()));
}

void AAIControllerBase::SetAIAttackingState(EAttackingStates  AIAttackingState)
{
    GetBlackboardComponent()->SetValueAsEnum(FName("AIAttackingState"), static_cast<uint8>(AIAttackingState));

    UE_LOG(LogTemp, Warning, TEXT("SetAIAttackingState: %s"), *UEnum::GetValueAsString(AIAttackingState));
    UE_LOG(LogTemp, Warning, TEXT("Current AIAttackingState from Blackboard: %s"), *UEnum::GetValueAsString(GetCurrentAttackingState()));
}

void AAIControllerBase::SetStateAsIdle()
{
    GetBlackboardComponent()->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Idle));
}

void AAIControllerBase::SetStateAsPatrolling()
{
    GetBlackboardComponent()->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Patrolling));
}

void AAIControllerBase::SetStateAsAttacking(AActor* AttackTarget)
{
    GetBlackboardComponent()->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Attacking));
    GetBlackboardComponent()->SetValueAsObject(AttackTargetKeyName, AttackTarget);

}

void AAIControllerBase::SetStateAsInvestigating(FVector Location)
{
    GetBlackboardComponent()->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Investigating));
    GetBlackboardComponent()->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AAIControllerBase::SetStateAsDead()
{
    GetBlackboardComponent()->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Dead));
}

#pragma endregion 

#pragma region Handle_Blackboard

void AAIControllerBase::InitializeBlackBoardKeys()
{
    AttackTargetKeyName = FName("AttackTarget");
    AIStateKeyName = FName("AIState");
    AIAttackingStateKeyName  = FName("AIAttackingState");
    PointOfInterestKeyName = FName("PointOfInterest");
}

#pragma endregion

#pragma region AI_Utilities

EAIPositionRelation AAIControllerBase::GetRelativePositionToPlayer(AActor* Player)
{
    if (!Player) return EAIPositionRelation::Unknown;

    FVector PlayerForward = Player->GetActorForwardVector();

    // Get direction from player to AI
    FVector ToAI = (Enemy->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();

    float DotProduct = FVector::DotProduct(PlayerForward, ToAI);

    UE_LOG(LogTemp, Warning, TEXT("Dot Product: %f"), DotProduct);

    // Determine AI's position relative to the player
    if (DotProduct < -0.3f) return EAIPositionRelation::Behind;
    if (DotProduct > 0.3f) return EAIPositionRelation::Front; 
    return EAIPositionRelation::Side; 
}

FVector AAIControllerBase::GetTargetPositionAroundPlayer(AActor* Player, float DistanceOffset)
{
    if (!Player) return FVector::ZeroVector;

    // Get player's movement direction (if moving)
    FVector PlayerVelocity = Player->GetVelocity().GetSafeNormal();

    // If the player is standing still, use their forward vector
    if (PlayerVelocity.IsNearlyZero())
    {
        PlayerVelocity = Player->GetActorForwardVector();
    }

    // Get AI's current position relation (Front, Behind, Side)
    EAIPositionRelation Position = GetRelativePositionToPlayer(Player);

    FVector TargetLocation;

    switch (Position)
    {
    case EAIPositionRelation::Behind:
        TargetLocation = Player->GetActorLocation() - PlayerVelocity * DistanceOffset;
        break;

    case EAIPositionRelation::Front:
        TargetLocation = Player->GetActorLocation() + PlayerVelocity * DistanceOffset;
        break;

    case EAIPositionRelation::Side:
        TargetLocation = Player->GetActorLocation() + Player->GetActorRightVector() * DistanceOffset;
        break;

    default:
        TargetLocation = Player->GetActorLocation(); // Default to current player position
        break;
    }

    UE_LOG(LogTemp, Warning, TEXT("AI Target Position: %s"), *TargetLocation.ToString());

    return TargetLocation;
}


FVector AAIControllerBase::RetreatFromPlayer(AActor* Player, float RetreatDistance)
{
    if (!Player || !Enemy) return FVector::ZeroVector;

    FVector AI_Location = Enemy->GetActorLocation();
    FVector Player_Location = Player->GetActorLocation();

    // Calculate direction away from the player
    FVector RetreatDirection = (AI_Location - Player_Location).GetSafeNormal();
    FVector DesiredRetreatLocation = AI_Location + (RetreatDirection * RetreatDistance);

    // Get NavMesh system
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("NavSystem is NULL! Retreating without path check."));
        return DesiredRetreatLocation;
    }

    // Try retreating directly backwards
    FNavLocation NavLocation;
    if (NavSystem->ProjectPointToNavigation(DesiredRetreatLocation, NavLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Retreating Backwards to: %s"), *NavLocation.Location.ToString());
        return NavLocation.Location;
    }

    // If backward retreat is blocked, try moving sideways (left or right)
    FVector RightVector = Player->GetActorRightVector();
    FVector LeftRetreatLocation = AI_Location - (RightVector * RetreatDistance);
    FVector RightRetreatLocation = AI_Location + (RightVector * RetreatDistance);

    // Try left side first
    if (NavSystem->ProjectPointToNavigation(LeftRetreatLocation, NavLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Retreating Left to: %s"), *NavLocation.Location.ToString());
        return NavLocation.Location;
    }

    // Try right side if left is blocked
    if (NavSystem->ProjectPointToNavigation(RightRetreatLocation, NavLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Retreating Right to: %s"), *NavLocation.Location.ToString());
        return NavLocation.Location;
    }

    // If no retreat path found, stay in place
    UE_LOG(LogTemp, Error, TEXT("No valid retreat path found! AI is stuck."));
    return AI_Location;
}


FVector AAIControllerBase::MoveBehindPlayer(AActor* Player, float Distance, float SideOffset)
{
    if (!Player || !Enemy) return FVector::ZeroVector;

    FVector PlayerLocation = Player->GetActorLocation();
    FVector PlayerForward = Player->GetActorForwardVector();
    FVector PlayerRight = Player->GetActorRightVector(); 

    FVector DesiredLocation = PlayerLocation - (PlayerForward * Distance);

   
     
    FVector LeftOffset = DesiredLocation - (PlayerRight * SideOffset);
    FVector RightOffset = DesiredLocation + (PlayerRight * SideOffset);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("NavSystem is NULL!"));
        return FVector::ZeroVector;
    }

    FNavLocation ValidLocation;
    if (!NavSystem->ProjectPointToNavigation(DesiredLocation, ValidLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Direct Back Location is blocked! Trying Side Offsets."));

        if (NavSystem->ProjectPointToNavigation(LeftOffset, ValidLocation))
        {
            UE_LOG(LogTemp, Warning, TEXT("AI Moving to Left Side Behind Player: %s"), *ValidLocation.Location.ToString());
            return ValidLocation.Location;
        }

        if (NavSystem->ProjectPointToNavigation(RightOffset, ValidLocation))
        {
            UE_LOG(LogTemp, Warning, TEXT("AI Moving to Right Side Behind Player: %s"), *ValidLocation.Location.ToString());
            return ValidLocation.Location;
        }

        
        UE_LOG(LogTemp, Error, TEXT("No valid retreat location found! AI staying in place."));
        return Enemy->GetActorLocation();
    }

    
    UNavigationPath* Path = NavSystem->FindPathToLocationSynchronously(GetWorld(), Enemy->GetActorLocation(), ValidLocation.Location);

    if (Path && Path->IsValid() && Path->PathPoints.Num() > 1)
    {
        FVector FirstPathPoint = Path->PathPoints[1];

        UE_LOG(LogTemp, Warning, TEXT("MoveBehindPlayer:: AI Moving to: %s"), *FirstPathPoint.ToString());
        return FirstPathPoint;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No valid path found! AI staying in place."));
        return FVector::ZeroVector;
    }
}




#pragma endregion