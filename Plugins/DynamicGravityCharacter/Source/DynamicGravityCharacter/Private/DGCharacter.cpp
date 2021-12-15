// Copyright 2019, Caio Felipe de Moura Peixoto, All rights reserved.


#include "DGCharacter.h"
#include "DGCharacterMovementComponent.h"

#include "GameFramework/Controller.h"

#include "Engine/GameEngine.h"


void ADGCharacter::UpdateRawViewRotation(float DeltaTime, UDGCharacterMovementComponent* MovementComponent)
{
	FVector ZVector;
	switch (ViewRotationBaseMode)
	{
	case EViewRotationBaseMode::VRM_Gravity:
		ZVector = -MovementComponent->Gravity();
		break;
	case EViewRotationBaseMode::VRM_WorldGravity:
		ZVector = -MovementComponent->WorldGravity();
		break;
	case EViewRotationBaseMode::VRM_DynamicGravity:
		ZVector = -MovementComponent->DynamicGravity;
		break;
	case EViewRotationBaseMode::VRM_VerticalDirection:
		ZVector = MovementComponent->VerticalDirection;
		break;
	case EViewRotationBaseMode::VRM_CharacterRotation:
		ZVector = GetActorUpVector();
		break;
	case EViewRotationBaseMode::VRM_ControlRotation:
		return;
	default:
		ViewRotationBase = CustomViewRotationBase;
		return;
	}
	ZVector = ZVector.GetSafeNormal();

	FVector CurrentVectorZ = FRotationMatrix(ViewRotationBase.GetEquivalentRotator()).GetScaledAxis(EAxis::Z);
	float dot = FVector::DotProduct(CurrentVectorZ, ZVector);
	if (dot < 0) {
		FVector XAxis = FRotationMatrix(GetViewRotation().GetEquivalentRotator()).GetScaledAxis(EAxis::X);
		XAxis = (XAxis - XAxis.ProjectOnToNormal(ZVector)).GetSafeNormal();
		ZVector = -ZVector.RotateAngleAxis(90 + acos(dot) * 180 / PI, XAxis);
	}

	FVector YVector = FRotationMatrix(ViewRotationBase.GetEquivalentRotator()).GetScaledAxis(EAxis::Y);
	FRotator NewRotation = fabs(FVector::DotProduct(YVector, ZVector)) < .7071068 ? FRotationMatrix::MakeFromZY(ZVector, YVector).Rotator() : FRotationMatrix::MakeFromZX(ZVector, FRotationMatrix(ViewRotationBase.GetEquivalentRotator()).GetScaledAxis(EAxis::X)).Rotator();


	const float AngleTolerance = 1e-3f;
	if (!ViewRotationBase.Equals(NewRotation, AngleTolerance))
	{
		float Alpha = ViewRotationAdjustIntensity < 0 ? 1 : DeltaTime * ViewRotationAdjustIntensity;
		if (Alpha > 1) Alpha = 1;

		FQuat AQuat(ViewRotationBase);
		FQuat BQuat(NewRotation);

		FQuat Result = FQuat::Slerp(AQuat, BQuat, Alpha);
		Result.Normalize();
		ViewRotationBase = Result.Rotator();

		ViewRotationBase.Normalize();
	}
}

void ADGCharacter::UpdateControlRotation(float DeltaTime, UDGCharacterMovementComponent* MovementComponent)
{
	if (ResetingPitchControlRotation || ResetingRollControlRotation || ResetingYawControlRotation)
	{
		if (Controller != nullptr && Controller->IsLocalPlayerController())
		{
			FRotator CurrentControlRotation = Controller->GetControlRotation();
			FRotator DesiredControlRotation = GetForwardControlRotation();
			if (!ResetingPitchControlRotation)
			{
				DesiredControlRotation.Pitch = CurrentControlRotation.Pitch;
			}
			if (!ResetingRollControlRotation)
			{
				DesiredControlRotation.Roll = CurrentControlRotation.Roll;
			}
			if (!ResetingYawControlRotation)
			{
				DesiredControlRotation.Yaw = CurrentControlRotation.Yaw;
			}
			DesiredControlRotation.Normalize();

			CurrentControlRotation = FMath::Lerp(CurrentControlRotation, DesiredControlRotation, DeltaTime * ResetControlRotationAdjustRate);

			const float AngleTolerance = 10;
			if (CurrentControlRotation.Equals(DesiredControlRotation, AngleTolerance))
			{
				Controller->SetControlRotation(DesiredControlRotation);
				ResetingPitchControlRotation = ResetingRollControlRotation = ResetingYawControlRotation = false;
			}
			else {
				Controller->SetControlRotation(CurrentControlRotation);
			}
		}
		else {
			ResetingPitchControlRotation = ResetingRollControlRotation = ResetingYawControlRotation = false;
		}
	}
	else if (ControlRotationAdjustRate > 0)
	{
		AddControllerYawInput(FVector::DotProduct(FRotationMatrix(GetViewRotation()).GetScaledAxis(EAxis::Y), MovementComponent->GetCurrentAcceleration()) * DeltaTime * ControlRotationAdjustRate / MovementComponent->GetMaxAcceleration());
	}
}

// Sets default values
ADGCharacter::ADGCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UDGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	ViewRotationBase = FRotator();

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ViewRotationAdjustIntensity = 15;
	ViewRotationBaseMode = EViewRotationBaseMode::VRM_ControlRotation;
	CustomViewRotationBase = DEFAULT_CUSTOM_VIEW_ROTATION_BASE;

	ControlRotationAdjustRate = 20;
	ResetControlRotationAdjustRate = 50;
}

float ADGCharacter::Speed()
{
	return GetVelocity().Size();
}

float ADGCharacter::VerticalSpeed()
{
	return VerticalVelocity().Size();
}

float ADGCharacter::HorizontalSpeed()
{
	return HorizontalVelocity().Size();
}


void ADGCharacter::AddForwardPlanarMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce)
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	FVector ForwardViewRotationDirection = FRotationMatrix(WorldRotation).GetScaledAxis(EAxis::X);
	FRotator ForwardInputRotation = FRotationMatrix::MakeFromZX(MovementComponent->VerticalDirection, ForwardViewRotationDirection).Rotator();
	FVector ForwardInputDirection = FRotationMatrix(ForwardInputRotation).GetScaledAxis(EAxis::X);
	AddMovementInput(ForwardInputDirection, ScaleValue, bForce);
}

void ADGCharacter::AddForwardPlanarMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce)
{
	AddForwardPlanarMovementInput(GetViewRotation(), ScaleValue, bForce);
}


void ADGCharacter::AddRightPlanarMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce)
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	FVector RightViewRotationDirection = FRotationMatrix(WorldRotation).GetScaledAxis(EAxis::Y);
	FRotator RightInputRotation = FRotationMatrix::MakeFromYZ(RightViewRotationDirection, MovementComponent->VerticalDirection).Rotator();
	FVector RightInputDirection = FRotationMatrix(RightInputRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightInputDirection, ScaleValue, bForce);
}

void ADGCharacter::AddRightPlanarMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce)
{
	AddRightPlanarMovementInput(GetViewRotation(), ScaleValue, bForce);
}


void ADGCharacter::AddForwardRadialMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce)
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	FVector RightViewRotationDirection = FRotationMatrix(WorldRotation).GetScaledAxis(EAxis::Y);
	FRotator RightInputRotation = FRotationMatrix::MakeFromYZ(RightViewRotationDirection, MovementComponent->VerticalDirection).Rotator();
	FVector ForwardInputDirection = FRotationMatrix(RightInputRotation).GetScaledAxis(EAxis::X);
	AddMovementInput(ForwardInputDirection, ScaleValue, bForce);
}

void ADGCharacter::AddForwardRadialMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce)
{
	AddForwardRadialMovementInput(GetViewRotation(), ScaleValue, bForce);
}


void ADGCharacter::AddRightRadialMovementInput(FRotator WorldRotation, float ScaleValue, bool bForce)
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	FVector ForwardViewRotationDirection = FRotationMatrix(WorldRotation).GetScaledAxis(EAxis::X);
	FRotator ForwardInputRotation = FRotationMatrix::MakeFromZX(MovementComponent->VerticalDirection, ForwardViewRotationDirection).Rotator();
	FVector RightInputDirection = FRotationMatrix(ForwardInputRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightInputDirection, ScaleValue, bForce);
}

void ADGCharacter::AddRightRadialMovementInputWithViewRotationAsWorldRotation(float ScaleValue, bool bForce)
{
	AddRightRadialMovementInput(GetViewRotation(), ScaleValue, bForce);
}

FVector ADGCharacter::VerticalVelocity()
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	return GetVelocity().ProjectOnToNormal(MovementComponent->VerticalDirection);
}

FVector ADGCharacter::HorizontalVelocity()
{
	return GetVelocity() - VerticalVelocity();
}

FHorizontalAndVerticalVelocities ADGCharacter::HorizontalAndVerticalVelocities()
{
	const UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	const FVector Velocity = GetVelocity();
	const FVector VerticalVelocity = Velocity.ProjectOnToNormal(MovementComponent->VerticalDirection);
	const FVector HorizontalVelocity = Velocity - VerticalVelocity;
	return FHorizontalAndVerticalVelocities(HorizontalVelocity, VerticalVelocity);
}

FRotator ADGCharacter::GetViewRotation() const
{
	if (Controller != nullptr && ViewRotationBaseMode != EViewRotationBaseMode::VRM_ControlRotation)
	{
		return FRotator(FQuat(ViewRotationBase) * FQuat(Controller->GetControlRotation())).GetNormalized();
	}

	return ACharacter::GetViewRotation();
}

void ADGCharacter::ResetControlRotation()
{
	ResetingPitchControlRotation = ResetingYawControlRotation = ResetingRollControlRotation = true;
}

void ADGCharacter::ResetPitchControlRotation()
{
	ResetingPitchControlRotation = true;
}

void ADGCharacter::ResetYawControlRotation()
{
	ResetingYawControlRotation = true;
}

void ADGCharacter::ResetRollControlRotation()
{
	ResetingRollControlRotation = true;
}

void ADGCharacter::ResetControlRotationHorizontally()
{
	ResetYawControlRotation();
}

void ADGCharacter::ResetControlRotationVertically()
{
	ResetPitchControlRotation();
}

void ADGCharacter::AddControllerPitchInput(float Val)
{
	if (!ResetingPitchControlRotation)
	{
		APawn::AddControllerPitchInput(Val);
	}
}

void ADGCharacter::AddControllerYawInput(float Val)
{
	if (!ResetingYawControlRotation)
	{
		APawn::AddControllerYawInput(Val);
	}
}

void ADGCharacter::AddControllerRollInput(float Val)
{
	if (!ResetingRollControlRotation)
	{
		APawn::AddControllerRollInput(Val);
	}
}

void ADGCharacter::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	UDGCharacterMovementComponent* MovementComponent = Cast<UDGCharacterMovementComponent>(GetMovementComponent());
	UpdateRawViewRotation(DeltaTime, MovementComponent);
	UpdateControlRotation(DeltaTime, MovementComponent);

	AActor::TickActor(DeltaTime, TickType, ThisTickFunction);
}