#pragma once
#include "LeapData_Structs.generated.h"

USTRUCT(BlueprintType)
struct FFrameData {
	GENERATED_USTRUCT_BODY()

	FFrameData() {

	}

	FFrameData(TArray<float> Data, FString GestureTag) {
		data = Data;
		gestureTag = GestureTag;
		gestureIndex = 0;
	}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		TArray<float> data;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FString gestureTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int gestureIndex;

};

UENUM(BlueprintType)
enum NetworkUse {
	Train UMETA(DisplayName = "Train"),
	Test UMETA(DisplayName="Test"),
	Runtime UMETA(DisplayName = "Runtime")
};