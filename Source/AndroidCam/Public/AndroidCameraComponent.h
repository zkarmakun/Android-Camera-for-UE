// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Texture2D.h"
#include "AndroidCameraComponent.generated.h"

// Region Data struct
struct FUpdateTextureRegionsData
{
	FTexture2DResource* Texture2DResource;
	int32 MipIndex;
	uint32 NumRegions;
	FUpdateTextureRegion2D* Regions;
	uint32 SrcPitch;
	uint32 SrcBpp;
	uint8* SrcData;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDROIDCAM_API UAndroidCameraComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UTexture2D* androidCameraTexture;
	bool bActive = false;
	TArray<FColor> rawData;
	FUpdateTextureRegion2D *echoUpdateTextureRegion;
	int32 width = 320;
	int32 height = 240;

	int YUVtoRGB(int y, int u, int v);
	const float frameRate = 30.f;
	float timer = 0;

public:	
	// Sets default values for this component's properties
	UAndroidCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void updateTexture(void* data);

	UFUNCTION(BlueprintCallable, Category = Android)
		void updateCamera(float delta);

	UFUNCTION(BlueprintCallable, Category = Android)
		UTexture2D* getAndroidCameraTexture();

	UFUNCTION(BlueprintCallable, Category = Android)
		void shutDownCamera();

		
};
