	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "StopWatch.generated.h"


	UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
	class MYPROJECT2_API UStopWatch : public UActorComponent
{
	GENERATED_BODY()

public:

	UStopWatch();                                                         // Function to set default values for this component's properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stop Watch", meta = (ClampMin = "0", UIMin = "0"))      // Blueprint exposed variable to set "Minutes"
		int32 Start_Minutes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stop Watch", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))       // Blueprint exposed variable to set "Seconds"
		int32 Start_Seconds;
	UPROPERTY(BlueprintReadOnly, Category = "Stop Watch")                       // Blueprint variable "read only" to access "Time Stamp" data
		FString TimeStamp;
	UPROPERTY(BlueprintReadOnly, Category = "Stop Watch")                       // Blueprint variable "read only" to access current Millisecond value
		float Milliseconds;
	UPROPERTY(BlueprintReadOnly, Category = "Stop Watch")                       // Blueprint variable "read only" to access current Seconds value
		float Current_Seconds;
	UPROPERTY(BlueprintReadOnly, Category = "Stop Watch")                       // Blueprint variable "read only" to access current Minutes value
		float Current_Minutes;

	UFUNCTION(BlueprintCallable)                                                // Blueprint exposed function to start the StopWatch/Timer
		void Start_Timer(bool StopWatch, float Rate);                           // Rate will be modified to a number that 100 is divisible by
	UFUNCTION(BlueprintCallable)                                                // Blueprint exposed function to pause the StopWatch/Timer
		void Pause();
	UFUNCTION(BlueprintCallable)                                                // Blueprint exposed function to unpause the StopWatch/Timer
		void Unpause();
	UFUNCTION(BlueprintCallable)                                                // Blueprint exposed function to clear the StopWatch/Timer
		void ClearTimer();
	UFUNCTION()
		void StopWatch();
	UFUNCTION()
		void GenerateTimeStamp();
	UFUNCTION(BlueprintImplementableEvent)                                      // Event scripted in Blueprints to occur when Timer hits "0"
		void Time_Is_Up();                                                      // Call this event using the function name "Time Is Up"


	bool IsStopWatch;                                                           // Global variable for function access

	float MillisecondsChange;                                                   // Declare necessary variables for StopWatch/Timer functionality
	float MillisecondsReset;
	float MillisecondsResetValue;
	float SecondsChange;
	float SecondsReset;
	float SecondsResetValue;
	float MinutesChange;

	FString MinutesOutput;
	FString SecondsOutput;
	FString MillisecondsOutput;

	FTimerHandle StopWatchHandle;

	AActor* Parent_Actor;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};