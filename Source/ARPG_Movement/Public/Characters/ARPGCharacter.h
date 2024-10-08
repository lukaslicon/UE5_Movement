// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"

#include "ARPGCharacter.generated.h"



class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;



UCLASS()
class ARPG_MOVEMENT_API AARPGCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AARPGCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* CharacterContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EActionMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipActionMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HeavyAttackAction;

	/**
	* Callback for Input
	*/


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void EquipKeyPressed();


	virtual void LightAttack() override;
	virtual void HeavyAttack() override;
	/**
	* Play Montage functions
	*/

	virtual void PlayLightAttackMontage() override;
	virtual void PlayHeavyAttackMontage() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;

	void PlayEquipMontage(FName SectionName);
	bool CanUnEquip();
	bool CanEquip();

	UFUNCTION(BlueprintCallable)
	void Unequip();

	UFUNCTION(BlueprintCallable)
	void Equip();

	UFUNCTION(BlueprintCallable)
	void FinishEquiping();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Damage")
	float HeavyAttackMultiplier = 2.0f;


private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public: 
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }


};
