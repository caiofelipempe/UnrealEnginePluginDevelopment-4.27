// Copyright 2019, Caio Felipe de Moura Peixoto, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DGCharacter.generated.h"

class UDGCharacterMovementComponent;

UENUM(BlueprintType)
enum class EViewRotationBaseMode : uint8
{
	VRM_Gravity 				UMETA(DisplayName = "Gravity"),
	VRM_WorldGravity 			UMETA(DisplayName = "World Gravity"),
	VRM_DynamicGravity 			UMETA(DisplayName = "Dynamic Gravity"),
	VRM_VerticalDirection 		UMETA(DisplayName = "Vertical Direction"),
	VRM_CharacterRotation		UMETA(DisplayName = "Character Rotation"),
	VRM_ControlRotation			UMETA(DisplayName = "Control Rotation"),
	VRM_Custom					UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FHorizontalAndVerticalVelocities
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(Category = "Horizontal", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Horizontal"))
		FVector HorizontalVelocity;

	UPROPERTY(Category = "Vertical", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Vertical"))
		FVector VerticalVelocity;

	FHorizontalAndVerticalVelocities()
	{
		HorizontalVelocity = FVector();
		VerticalVelocity = FVector();
	}

	FHorizontalAndVerticalVelocities(FVector Horizontal, FVector Vertical)
	{
		HorizontalVelocity = Horizontal;
		VerticalVelocity = Vertical;
	}
};

UCLASS()
class DYNAMICGRAVITYCHARACTER_API ADGCharacter : public ACharacter
{
	GENERATED_BODY()

		const FRotator DEFAULT_CUSTOM_VIEW_ROTATION_BASE = FRotator(0, 0, 0);

	FORCEINLINE void UpdateRawViewRotation(float DeltaTime, UDGCharacterMovementComponent* MovementComponent);
	FORCEINLINE void UpdateControlRotation(float DeltaTime, UDGCharacterMovementComponent* MovementComponent);

	/**
	 * The view rotation base mode. view rotation base is the view rotation without control rotation.
	 *    - Gravity: The view rotation base is related to the negative direction of the gravity.
	 *    - World Gravity: The view rotation base is related to the negative direction of the world gravity.
	 *    - Dynamic Gravity: The view rotation base is related to the negative direction of the dynamic gravity.
	 *    - Vertical Direction:  The view rotation base is related to the vertical direction of the CharacterMovementComponent.
	 *    - Character Rotation:  The view rotation base is related to the up vector of the character.
	 *    - Control:  Uses only control rotation to rotate camera. So the control rotation base is always zero.
	 *    - Custom:  Uses the CustomViewRotationBase variable as view rotation base.
	 * @see CustomViewRotationBase
	 */
	UPROPERTY(Category = "View Rotation", EditAnywhere, BlueprintGetter = GetViewRotationBaseMode, BlueprintSetter = SetViewRotationBaseMode)
		EViewRotationBaseMode ViewRotationBaseMode;

	/** This is the ViewRotationBase if ViewRotationBaseMode == Custom.*/
	UPROPERTY(Category = "View Rotation", EditAnywhere)
		FRotator CustomViewRotationBase;

	bool ResetingPitchControlRotation;
	bool ResetingYawControlRotation;
	bool ResetingRollControlRotation;

	FRotator GetForwardControlRotation()
	{
		return FRotator(FQuat(ViewRotationBase.GetInverse()) * FQuat(GetActorRotation()));
	}


public:

	virtual void AddControllerPitchInput(float Val) override;
	virtual void AddControllerYawInput(float Val);
	virtual void AddControllerRollInput(float Val);



	/**
	 * Add forward input rotated for the plane made by X and Y vectors of view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddForwardPlanarMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce = false);

	/**
	 * Add right input rotated for the plane made by X and Y vectors of view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddRightPlanarMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce = false);

	/**
	 * Add forward input rotated for the sphere around view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddForwardRadialMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce = false);

	/**
	 * Add right input rotated for the sphere around view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddRightRadialMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce = false);


	/**
	 * Add forward input rotated for the plane made by X and Y vectors of view rotation using view rotation as world rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddForwardPlanarMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce = false);

	/**
	 * Add right input rotated for the plane made by X and Y vectors of view rotation using view rotation as world rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddRightPlanarMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce = false);

	/**
	 * Add forward input rotated for the sphere around view rotation using view rotation as world rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddForwardRadialMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce = false);

	/**
	 * Add right input rotated for the sphere around view rotation using view rotation as world rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity (Movement Input)", BlueprintCallable)
		void AddRightRadialMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce = false);


	/**
	 * Calculate the vertical velocity of the character. The vertical velocity is a projection of the velocity on the vertical direction.
	 * @return The vertical velocity of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector VerticalVelocity();

	/**
	 * Calculate the horizontal velocity of the character. The horizontal velocity is the velocity of the character subtracted by the vertical direction.
	 * @return The horizontal velocity of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FVector HorizontalVelocity();

	/**
	 * Calculate the horizontal velocity of the character. The horizontal velocity is the velocity of the character subtracted by the vertical direction.
	 * @return The horizontal velocity of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		FHorizontalAndVerticalVelocities HorizontalAndVerticalVelocities();

	/**
	 * Calculate the speed of the character.
	 * @return The speed of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		float Speed();

	/**
	 * Calculate the vertical speed of the character. The vertical speed is the scalar velocity of the velocity projected in the vertical direction.
	 * @return The vertical speed of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		float VerticalSpeed();

	/**
	 * Calculate the speed of the character. The horizontal speed is the lenght of velocity subtract by the vertical velocity.
	 * @return The horizontal speed of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		float HorizontalSpeed();

	/** The view rotation without control rotation.*/
	UPROPERTY(Category = "View Rotation", BlueprintReadOnly)
		FRotator ViewRotationBase;

	/** Intensity of the view adjustment. If the value negative, the adjustment is imediate.*/
	UPROPERTY(Category = "View Rotation", EditAnywhere, BlueprintReadWrite)
		float ViewRotationAdjustIntensity;

	/** The yaw adjustment rate of control rotation when character is moving. */
	UPROPERTY(Category = "View Rotation (Control)", EditAnywhere, BlueprintReadWrite)
		float ControlRotationAdjustRate;

	/** The yaw adjustment rate of control rotation when character is moving. */
	UPROPERTY(Category = "View Rotation (Control)", EditAnywhere, BlueprintReadWrite)
		float ResetControlRotationAdjustRate;

	UFUNCTION(Category = "Dynamic Gravity", BlueprintGetter)
		EViewRotationBaseMode GetViewRotationBaseMode() const { return ViewRotationBaseMode; }

	UFUNCTION(Category = "Dynamic Gravity", BlueprintSetter)
		void SetViewRotationBaseMode(EViewRotationBaseMode NewViewRotationBaseMode) {
		ViewRotationBaseMode = NewViewRotationBaseMode;

		if (NewViewRotationBaseMode == EViewRotationBaseMode::VRM_ControlRotation)
		{
			ViewRotationBase = FRotator();
		}
	}

	/**
	 * Get the view rotation of the Character (direction they are looking, normally Controller->ControlRotation combined with ViewRotationBase).
	 * @return The view rotation of the Character.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintPure)
		virtual FRotator GetViewRotation() const override;


	/**
	 * Centralize view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetControlRotation();

	/**
	 * Centralize view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetPitchControlRotation();

	/**
	 * Centralize view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetYawControlRotation();

	/**
	 * Centralize view rotation.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetRollControlRotation();

	/**
	 * Centralize view rotation horizontally.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetControlRotationHorizontally();

	/**
	 * Centralize view rotation vertically.
	 */
	UFUNCTION(Category = "Dynamic Gravity", BlueprintCallable)
		void ResetControlRotationVertically();


	// Sets default values for this character's properties
	ADGCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
};