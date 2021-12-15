// Copyright 2019, Caio Felipe de Moura Peixoto, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DGCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
enum class EWalkableFloorNormalMode : uint8
{
	WFN_Gravity					UMETA(DisplayName = "Gravity"),
	WFN_WorldGravity 			UMETA(DisplayName = "World Gravity"),
	WFN_DynamicGravity			UMETA(DisplayName = "Dynamic Gravity"),
	WFN_CharacterRotation		UMETA(DisplayName = "Character Rotation"),
	WFN_FloorImpactNormal		UMETA(DisplayName = "Floor Impact Normal"),
	WFN_NoFloor					UMETA(DisplayName = "No Floor"),
	WFN_Custom					UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EJumpDirectionMode : uint8
{
	JDM_Gravity 			 	UMETA(DisplayName = "Gravity"),
	JDM_WorldGravity 		 	UMETA(DisplayName = "World Gravity"),
	JDM_DynamicGravity 			UMETA(DisplayName = "Dynamic Gravity"),
	JDM_VerticalDirection 		UMETA(DisplayName = "Vertical Direction"),
	JDM_Custom					UMETA(DisplayName = "Custom")
};



UENUM(BlueprintType)
enum class EPhysicsRotationVerticalDirectionMode : uint8
{
	PRVDM_Gravity 			 	    UMETA(DisplayName = "Gravity"),
	PRVDM_WorldGravity		 	    UMETA(DisplayName = "World Gravity"),
	PRVDM_DynamicGravity	 	    UMETA(DisplayName = "Dynamic Gravity"),
	PRVDM_VerticalDirection 		UMETA(DisplayName = "Vertical Direction"),
	PRVDM_Custom					UMETA(DisplayName = "Custom")
};


/**
 *
 */
UCLASS()
class DYNAMICGRAVITYCHARACTER_API UDGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()



		void UpdateVerticalDirection();


	/**
	 * The walkable floor normal mode.
	 *    - Gravity:  Uses inverse of gravity normal as walkable floor normal.
	 *    - World Gravity:  Uses inverse of world gravity normal as walkable floor normal.
	 *    - Dynamic Gravity:  Uses inverse of dynamic gravity normal as walkable floor normal.
	 *    - Character Rotation:  Uses character up vector as walkable floor normal.
	 *    - Floor Impact Normal:  Uses current floor impact normal as walkable floor normal.
	 *    - No Floor:  Character wont find floor.
	 *    - Custom:  Uses the CustomWalkableFloorNormal as walkable floor normal.
	 * @see CustomWalkableFloorNormal
	 */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EWalkableFloorNormalMode WalkableFloorNormalMode;

	/**
	 * The direction of the floor if 'WalkableFloorNormalMode' is 'Custom'.
	 * @see WalkableFloorNormalMode
	*/
	UPROPERTY(Category = "Character Movement: Walking", VisibleAnywhere, BlueprintGetter = GetCustomWalkableFloorNormal, BlueprintSetter = SetCustomWalkableFloorNormal, meta = (AllowPrivateAccess = "true"))
		FVector CustomWalkableFloorNormal;

	/**
	 * The jump direction.
	 *    - Gravity:  Uses inverse of gravity normal as walkable jump direction.
	 *    - World Gravity:  Uses inverse of World gravity normal as walkable jump direction.
	 *    - Dynamic Gravity:  Uses inverse of Dynamic gravity normal as walkable jump direction.
	 *    - Vertical Direction:  Uses character vertical direction as jump direction.
	 *    - Custom:  Uses the CustomJumpDirection as walkable floor normal.
	 * @see CustomWalkableFloorNormal
	 */
	UPROPERTY(Category = "Character Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EJumpDirectionMode JumpDirectionMode;

	/**
	 * The direction of the floor if 'JumpDirectionMode' is 'Custom'.
	 * @see JumpDirectionMode
	*/
	UPROPERTY(Category = "Character Movement: Jumping / Falling", VisibleAnywhere, BlueprintGetter = GetCustomJumpDirection, BlueprintSetter = SetCustomJumpDirection, meta = (AllowPrivateAccess = "true"))
		FVector CustomJumpDirection;


public:

	UDGCharacterMovementComponent();


	const FVector DEFAULT_GRAVITY_DIRECTION = FVector::UpVector;

	const EWalkableFloorNormalMode DEFAULT_WALKABLE_FLOOR_NORMAL_MODE = EWalkableFloorNormalMode::WFN_CharacterRotation;
	const FVector DEFAULT_CUSTOM_WALKABLE_FLOOR_NORMAL = FVector::UpVector;

	const EJumpDirectionMode DEFAULT_JUMP_DIRECTION_MODE = EJumpDirectionMode::JDM_Gravity;
	const FVector DEFAULT_CUSTOM_JUMP_DIRECTION = FVector::UpVector;

	const EPhysicsRotationVerticalDirectionMode DEFAULT_PHYSICS_ROTATION_VERTICAL_DIRECTION_MODE = EPhysicsRotationVerticalDirectionMode::PRVDM_VerticalDirection;

	const FVector DEFAULT_VERTICAL_DIRECTION = FVector::UpVector;

	const float DEFAULT_LERP_ROTATION_RATE = 10.0f;

	const FRotator DEFAULT_CUSTOM_VIEW_ROTATION_VERTICAL_DIRECTION = FRotator::ZeroRotator;

	const float VERTICAL_SLOPE_NORMAL_Z = 0.001f;



	/** Intensity of the adjust when UseControllerDesiredRotation or OrientRotationToMovement are true. If the values is less than zero, it will adjust immediately.*/
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
		float RotationAdjustIntensity;

	/**
	 * The physics rotation vertical direction mode.
	 *    - Gravity:  Uses inverse of gravity normal as vertical direction of physics rotation.
	 *    - World Gravity:  Uses inverse of world gravity normal as vertical direction of physics rotation.
	 *    - Dynamic Gravity:  Uses inverse of dynamic gravity normal as vertical direction of physics rotation.
	 *    - Vertical Direction:  Uses character vertical direction as vertical direction of physics rotation.
	 *    - Custom:  Uses the RotationRate's up vector as vertical direction of physics rotation. OBS: RotationRate is being used as CustomPhysicsRotationVerticalDirection because RotationAdjustIntensity is doing the it´s role. So, to not make RotationRate a useless variable, it´s being used for a different purpuse.
	 */
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
		EPhysicsRotationVerticalDirectionMode PhysicsRotationVerticalDirectionMode;

	/** The vertical direction of the character.*/
	UPROPERTY(Category = "Character Movement (General Settings)", BlueprintReadOnly)
		FVector VerticalDirection;

	/** Despises World Gravity if Dynamic Gravity Is Differernt of zero. */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
		bool bIgnoreWorldGravityIfDynamicGravityIsNotZero;

	/** The vector that represents Dynamic Gravity. */
	UPROPERTY(Category = "Dynamic Gravity", EditAnywhere, BlueprintReadWrite)
		FVector DynamicGravity;


	/**
	 * The walkable floor normal is the direction that the character finds the ground.
	 * @return The current walkable floor normal.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector WalkableFloorNormal() const;

	UFUNCTION(Category = "Dynamic Gravity", BlueprintGetter)
		FVector GetCustomWalkableFloorNormal() const { return CustomWalkableFloorNormal; }

	UFUNCTION(Category = "Dynamic Gravity", BlueprintSetter)
		void SetCustomWalkableFloorNormal(FVector NewFloorDirection) { CustomWalkableFloorNormal = NewFloorDirection.GetSafeNormal(); }

	/**
	 * The direction of jump applied.
	 * @return The current walkable floor normal.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector JumpDirection() const;

	UFUNCTION(Category = "Dynamic Gravity", BlueprintGetter)
		FVector GetCustomJumpDirection() const { return CustomJumpDirection; }

	UFUNCTION(Category = "Dynamic Gravity", BlueprintSetter)
		void SetCustomJumpDirection(FVector NewJumpDirection) { CustomJumpDirection = NewJumpDirection.GetSafeNormal(); }


	/** Calculate the vector that represents gravity. It is the multiplication of GravityZ and GravityDirection.*/
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector WorldGravity() const { return  GetGravityZ() * DEFAULT_GRAVITY_DIRECTION; }

	/** Calculate the vector that represents gravity. The combination of World Gravity and Dynamic Gravity. If bIgnoreWorldGravityIfDynamicGravityIsNotZero is true and DynamicGravity is not zero, then the value will be only DynamicGravity.*/
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector Gravity() const { return (bIgnoreWorldGravityIfDynamicGravityIsNotZero && !DynamicGravity.Equals(FVector::ZeroVector)) ? DynamicGravity : WorldGravity() + DynamicGravity; }

	/**
	 * The vector that represents World Gravity nomalized. If GravityZ is negative, it's direction will be oposite of gravity direction.
	 * @see Gravity()
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector WorldGravityNormal() const { return  GetGravityZ() >= 0 ? DEFAULT_GRAVITY_DIRECTION : -DEFAULT_GRAVITY_DIRECTION; }

	/**
	 * The vector that represents Dynamic Gravity nomalized.
	 * @see Gravity()
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector DynamicGravityNormal() const { return  DynamicGravity.GetSafeNormal(); }

	/**
	 * The vector that represents Gravity nomalized.
	 * @see Gravity()
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector GravityNormal() const { return  Gravity().GetSafeNormal(); }


	/**
	* Sweeps a vertical trace to find the floor for the capsule at the given location. Will attempt to perch if ShouldComputePerchResult() returns true for the downward sweep result.
	* No floor will be found if collision is disabled on the capsule!
	*
	* @param WalkableFloorNormal	The floor normal.
	* @param CapsuleRotation		Location where the capsule sweep should originate.
	* @param CapsuleLocation		Location where the capsule sweep should originate.
	* @param FloorResult			Result of the floor check.
	*/
	UFUNCTION(Category = "Pawn|Components|CharacterMovement", BlueprintCallable, meta = (DisplayName = "FindFloor", ScriptName = "FindFloor"))
		virtual void FindFloor(const FVector WalkableNormal, const FRotator CapsuleRotation, FVector CapsuleLocation, FFindFloorResult& FloorResult) const;


	/**
	* Compute distance to the floor from bottom sphere of capsule and store the result in FloorResult.
	* This distance is the swept distance of the capsule to the first point impacted by the lower hemisphere, or distance from the bottom of the capsule in the case of a line trace.
	* This function does not care if collision is disabled on the capsule (unlike FindFloor).
	*
	* @param WalkableFloorNormal	The floor normal.
	* @param CapsuleLocation		Location where the capsule sweep should originate.
	* @param CapsuleLocation		Location where the capsule sweep should originate.
	* @param LineDistance			If non-zero, max distance to test for a simple line check from the capsule base. Used only if the sweep test fails to find a walkable floor, and only returns a valid result if the impact normal is a walkable normal.
	* @param SweepDistance			If non-zero, max distance to use when sweeping a capsule downwards for the test. MUST be greater than or equal to the line distance.
	* @param SweepRadius			The radius to use for sweep tests. Should be <= capsule radius.
	* @param FloorResult			Result of the floor check
	*/
	UFUNCTION(Category = "Pawn|Components|CharacterMovement", BlueprintCallable, meta = (DisplayName = "ComputeFloorDistance", ScriptName = "ComputeFloorDistance"))
		virtual void ComputeFloorDist(const FVector WalkableNormal, const FRotator CapsuleRotation, FVector CapsuleLocation, float LineDistance, float SweepDistance, float SweepRadius, FFindFloorResult& FloorResult) const;


protected:

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual FVector ConstrainInputAcceleration(const FVector& InputAcceleration) const override;

	virtual void AdjustFloorHeight();
	virtual void TwoWallAdjust(FVector& Delta, const FHitResult& Hit, const FVector& OldHitNormal) const override;
	virtual bool StepUp(const FVector& FloorDirection, const FVector& Delta, const FHitResult& Hit, struct UCharacterMovementComponent::FStepDownResult* OutStepDownResult = NULL) override;
	virtual FVector HandleSlopeBoosting(const FVector& SlideResult, const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	virtual void RequestPathMove(const FVector& MoveInput) override;
	virtual FVector GetLedgeMove(const FVector& OldLocation, const FVector& Delta, const FVector& GravDir) const override;
	virtual void ApplyAccumulatedForces(float DeltaSeconds) override;
	void ApplyRootMotionToVelocity(float deltaTime);
	virtual void SetDefaultMovementMode() override;
	virtual void MoveSmooth(const FVector& InVelocity, const float DeltaSeconds, FStepDownResult* OutStepDownResult = NULL) override;
	virtual void SimulateMovement(float DeltaTime) override;

	void PhysWalking(float deltaTime, int32 Iterations) override;

	virtual FVector ComputeGroundMovementDelta(const FVector& Delta, const FHitResult& RampHit, const bool bHitFromLineTrace) const override;
	virtual float SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact) override;
	virtual void MoveAlongFloor(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult = NULL) override;
	virtual void MaintainHorizontalGroundVelocity() override;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;


	virtual void PhysFalling(float DeltaTime, int32 Iterations) override;

	virtual FVector GetFallingLateralAcceleration(float DeltaTime) override;
	virtual bool DoJump(bool bReplayingMoves) override;
	virtual void JumpOff(AActor* MovementBaseActor) override;
	float BoostAirControl(float DeltaTime, float TickAirControl, const FVector& FallAcceleration) override;


public:

	virtual bool IsValidLandingSpot(const FVector& CapsuleLocation, const FHitResult& Hit) const override;


	virtual bool IsWalkable(const FHitResult& Hit) const override;
	virtual bool IsWalkable(const FVector WalkableFloorNormal, const FHitResult& Hit) const;

	virtual bool IsWithinEdgeTolerance(const FVector& CapsuleLocation, const FVector& TestImpactPoint, const float CapsuleRadius) const override;
	virtual bool IsWithinEdgeTolerance(const FVector WalkableFloorNormal, const FVector& CapsuleLocation, const FVector& TestImpactPoint, const float CapsuleRadius) const;

	virtual bool ShouldComputePerchResult(const FHitResult& InHit, bool bCheckRadius = true) const override;
	virtual bool ShouldComputePerchResult(const FVector WalkableFloorNormal, const FHitResult& InHit, bool bCheckRadius = true) const;

	virtual bool ComputePerchResult(const float TestRadius, const FHitResult& InHit, const float InMaxFloorDist, FFindFloorResult& OutPerchFloorResult) const override;
	virtual bool ComputePerchResult(const FVector WalkableFloorNormal, const float TestRadius, const FHitResult& InHit, const float InMaxFloorDist, FFindFloorResult& OutPerchFloorResult) const;

	virtual void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bZeroDelta, const FHitResult* DownwardSweepResult = NULL) const override;
	virtual void FindFloor(const FVector WalkableFloorNormal, const FRotator Rot, const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bZeroDelta, const FHitResult* DownwardSweepResult = NULL) const;

	virtual void ComputeFloorDist(const FVector& CapsuleLocation, float LineDistance, float SweepDistance, FFindFloorResult& OutFloorResult, float SweepRadius, const FHitResult* DownwardSweepResult = NULL) const override;
	virtual void ComputeFloorDist(const FVector WalkableFloorNormal, const FRotator Rot, const FVector& CapsuleLocation, float LineDistance, float SweepDistance, FFindFloorResult& OutFloorResult, float SweepRadius, const FHitResult* DownwardSweepResult = NULL) const;

	virtual bool FloorSweepTest(struct FHitResult& OutHit, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const struct FCollisionShape& CollisionShape, const struct FCollisionQueryParams& Params, const struct FCollisionResponseParams& ResponseParam) const override;
	virtual bool FloorSweepTest(struct FHitResult& OutHit, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const FRotator Rot, const struct FCollisionShape& CollisionShape, const struct FCollisionQueryParams& Params, const struct FCollisionResponseParams& ResponseParam) const;


	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;
	virtual void PhysicsRotation(float DeltaTime) override;



	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};