// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"



UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Left,
	Right,
	Up,
	Down
};


UCLASS()
class NUM2048_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Game logic functions

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Move"))
	bool Move(EMoveDirection direction);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetBoard"))
	TArray<int32> GetBoard() const;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsMovePossible"))
	bool IsMovePossible(const TArray<int32>& _Board, int32 BoardSize);


private:
	TArray<TArray<int32>> Board;
	UPROPERTY(VisibleAnywhere)
	bool TileMovedOrMerged;

	bool IsCellEmpty(int32 X, int32 Y) const;
	void AddRandomTile();

	void ShiftLeft();
	void CombineTilesLeft();

	void ShiftRight();
	void CombineTilesRight();

	void ShiftUp();
	void CombineTilesUp();

	void ShiftDown();
	void CombineTilesDown();
};

