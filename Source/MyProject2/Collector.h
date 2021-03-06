// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Collector.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UCollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT2_API ICollector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Trigger Collection")
	virtual void Collect();
};
