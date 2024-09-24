// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_MOVEMENT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();


protected:
	virtual void BeginPlay() override;

private:
	//current
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	//max
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;
		

};
