// Fill out your copyright notice in the Description page of Project Settings.


#include "StopWatch.h"

// Sets default values for this component's properties
UStopWatch::UStopWatch()
{
	PrimaryComponentTick.bCanEverTick = false;              // Disable tick for this component

	Milliseconds = 0.f;                                     // Set default variables

	Parent_Actor = GetOwner();                              // Retrieve actor component Parent Actor

}


// Called when the game starts
void UStopWatch::BeginPlay()
{

	Super::BeginPlay();

}


// Called every frame
void UStopWatch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UStopWatch::Start_Timer(bool StopWatch, float Rate)
{
	Current_Seconds = float(Start_Seconds);                 // Convert input values to float for calculations (prevents improper input through blueprints)
	Current_Minutes = float(Start_Minutes);

	if (Rate == 0)                                          // Prevent division by "0" from improper user input
	{
		Rate = 1;
	}

	if (fmod(100, Rate) != 0)                               // Ensure 100 divisible to a whole number by any input rate
	{
		Rate = 5;                                           // Default rate value if impoper value entered
	}

	IsStopWatch = StopWatch;                                // Grab input bool from blueprint function call and set global variable

	if (IsStopWatch == true)                                // Set values for "additive" time
	{
		Current_Seconds = 0;                                // If "Stop Watch" mode is true, set minutes and seconds to "0" in case this was forgotten
		Current_Minutes = 0;
		MillisecondsChange = Rate;                          // Value matches rate at which timer runs, 0.05 sec timer will (+5) from milliseconds each pass
		SecondsChange = 1.f;
		MinutesChange = 1.f;
		MillisecondsReset = 100.f;
		SecondsReset = 60.f;
		MillisecondsResetValue = 0.f;
		SecondsResetValue = 0.f;
	}
	else                                                    // Set values for "subtractive" time
	{
		MillisecondsChange = -Rate;                         // Value matches rate at which timer runs, 0.05 sec timer will (-5) from milliseconds each pass
		SecondsChange = -1.f;
		MinutesChange = -1.f;
		MillisecondsReset = 0 - Rate;                       // Cosmetic reset value, if reset at "0" Time Stamp would read "00:4:100" going from >5 to 4 seconds                           
		SecondsReset = -1.f;                                // Prefer "00:05:05", "00:05:00", "00:04:95" opposed to "00:05:05", "00:04:100", "00:04:95"    
		MillisecondsResetValue = 100 - Rate;                // Although values are equivalent
		SecondsResetValue = 59.f;

		if (Current_Seconds == 0)                           // Only necessary for "Timer-Style" (Subtractive) mode, performs initial subtraction prior to function call
		{
			Current_Minutes += MinutesChange;
			Current_Seconds = SecondsResetValue;
			Milliseconds = MillisecondsResetValue;
		}
		else                                                // If starting value for seconds > 0 simply subtract the first second and add to milliseconds
		{
			Current_Seconds += SecondsChange;
			Milliseconds = MillisecondsResetValue;
		}
	}

	Parent_Actor->GetWorldTimerManager().SetTimer(StopWatchHandle, this, &UStopWatch::StopWatch, Rate / 100, true);     // Create a timer

}

void UStopWatch::StopWatch()                          // Function that actually calculates "time"
{

	Milliseconds += MillisecondsChange;                     // Add or Subtract 5 from the value of "Milliseconds" each call of this function

	if (Milliseconds == MillisecondsReset)                  // If value of "Milliseconds" = the "reset value" add or subtract appropriately from "Seconds"
	{
		Milliseconds = MillisecondsResetValue;
		Current_Seconds += SecondsChange;

		if (Current_Seconds == SecondsReset)                // Check if "Seconds" has reached its reset value and add or subtract from "Minutes" appropriately
		{
			Current_Seconds = SecondsResetValue;
			Current_Minutes += MinutesChange;
		}

	}

	GenerateTimeStamp();                                    // Function call to create Time Stamp format "00:00:00"

	if (IsStopWatch == false && Current_Minutes < 0)        // If using "Timer-Sytle" mode set all values to 0 upon completion and call "Time Is Up" function
	{

		Parent_Actor->GetWorldTimerManager().ClearTimer(StopWatchHandle);       // Clear timer handle (prevents timer from continuing to execute)

		Current_Minutes = 0;                                // Sets Minutes, Seconds and Milliseconds to default 0,0,0
		Current_Seconds = 0;
		Milliseconds = 0;

		GenerateTimeStamp();                                // Create final Time Stamp showing "00:00:00"

		Time_Is_Up();                                       // Call to Blueprint Implemented Function (*Allows for custom events to occur when timer reaches "0")

	}


}

void UStopWatch::GenerateTimeStamp()                          // Actual function to create a "Time Stamp"
{

	MinutesOutput = FString::SanitizeFloat(Current_Minutes, 0);     // Remove trailing 0's from float, only use whole number values
	if (MinutesOutput.Len() < 2)                                    // Add a leading "0" if float is a single digit
	{
		MinutesOutput = "0" + MinutesOutput;
	}
	SecondsOutput = FString::SanitizeFloat(Current_Seconds, 0);
	if (SecondsOutput.Len() < 2)
	{
		SecondsOutput = "0" + SecondsOutput;
	}
	MillisecondsOutput = FString::SanitizeFloat(Milliseconds, 0);
	if (MillisecondsOutput.Len() < 2)
	{
		MillisecondsOutput = "0" + MillisecondsOutput;
	}

	TimeStamp = (MinutesOutput + " : " + SecondsOutput + " : " + MillisecondsOutput);       // Add member strings together with separating colons

}

void UStopWatch::Pause()                                                              // Function to "Pause" timer called from Blueprints
{
	Parent_Actor->GetWorldTimerManager().PauseTimer(StopWatchHandle);
}

void UStopWatch::Unpause()                                                            // Function to "Unpause" timer called from Blueprints
{
	Parent_Actor->GetWorldTimerManager().UnPauseTimer(StopWatchHandle);
}

void UStopWatch::ClearTimer()                                                         // Function to "Clear" timer called from Blueprints
{
	Parent_Actor->GetWorldTimerManager().ClearTimer(StopWatchHandle);
}

