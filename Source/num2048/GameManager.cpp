// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Engine.h"


// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // Initialize the 4x4 board with zeros
    Board.SetNum(4);  // Ensure there are 4 rows
    for (int32 i = 0; i < 4; i++) {
        Board[i].Init(0, 4);  // Initialize each row with 4 columns set to zero
    }
}

void AGameManager::BeginPlay()
{
    Super::BeginPlay();
    AddRandomTile();  // Start the game with one tile
}

void AGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AGameManager::IsCellEmpty(int32 X, int32 Y) const
{
    return Board[X][Y] == 0;
}

void AGameManager::AddRandomTile()
{
    TArray<FIntPoint> EmptyPositions;

    for (int32 i = 0; i < 4; ++i)
    {
        for (int32 j = 0; j < 4; ++j)
        {
            if (IsCellEmpty(i, j))
            {
                EmptyPositions.Add(FIntPoint(i, j));
            }
        }
    }

    if (EmptyPositions.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, EmptyPositions.Num() - 1);
        int32 Value = FMath::RandRange(0, 9) < 1 ? 4 : 2;  // 10% chance for a 4
        Board[EmptyPositions[Index].X][EmptyPositions[Index].Y] = Value;
    }
}

void AGameManager::ShiftRight()
{
    TileMovedOrMerged = false;
    for (int32 i = 0; i < 4; ++i)
    {
        int32 CurrentPos = 0;
        for (int32 j = 0; j < 4; ++j)
        {
            if (Board[i][j] != 0)
            {
                if (j != CurrentPos)
                {
                    Board[i][CurrentPos] = Board[i][j];
                    Board[i][j] = 0;
                    TileMovedOrMerged = true;
                }
                CurrentPos++;
            }
        }
    }
}

void AGameManager::CombineTilesRight()
{
    TileMovedOrMerged = false;
    for (int32 i = 0; i < 4; ++i)
    {
        for (int32 j = 0; j < 3; ++j)
        {
            if (Board[i][j] != 0 && Board[i][j] == Board[i][j + 1])
            {
                Board[i][j] *= 2;
                Board[i][j + 1] = 0;
                TileMovedOrMerged = true;
            }
        }
    }
}

bool AGameManager::Move(EMoveDirection Direction)
{
	TileMovedOrMerged = false;
    // Perform the shift and combine based on the direction
    switch (Direction)
    {
    case EMoveDirection::Left:
        ShiftLeft();
        CombineTilesLeft();
        ShiftLeft();
        break;
    case EMoveDirection::Right:
        ShiftRight();
        CombineTilesRight();
        ShiftRight();
        break;
    case EMoveDirection::Up:
        ShiftUp();
        CombineTilesUp();
        ShiftUp();
        break; 
    case EMoveDirection::Down:
        ShiftDown();
        CombineTilesDown();
        ShiftDown();
        break;
    }

    if(TileMovedOrMerged) AddRandomTile();

    // Display debug messages if GEngine is available
    if (GEngine)
    {
        FString Message = TEXT("Updating game state");
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);

        // Show board state
        for (int32 i = 0; i < Board.Num(); ++i)
        {
            FString RowContents;
            for (int32 j = 0; j < Board[i].Num(); ++j)
            {
                RowContents += FString::Printf(TEXT("%d "), Board[i][j]);
            }
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, RowContents);
        }

        // Show if tiles moved or merged
        FString result = TileMovedOrMerged ? "true" : "false";
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, result);
    }
    return TileMovedOrMerged;
}


// Use this to access the board state wherever needed
TArray<int32> AGameManager::GetBoard() const
{
    TArray<int32> result = TArray<int32>();
    for (const TArray<int32>& row : Board)
    {
        // Iterate over each value in the row
        for (int32 value : row)
        {
            result.Add(value);
        }
    }
    return result;
}

bool AGameManager::IsMovePossible(const TArray<int32>& _Board, int32 BoardSize)
{
    // Check for empty tile
    for (int32 Index = 0; Index < BoardSize * BoardSize; Index++)
    {
        if (_Board[Index] == 0)
        {
            return true;
        }
    }

    // Check for possible merges horizontally
    for (int32 Row = 0; Row < BoardSize; Row++)
    {
        for (int32 Col = 0; Col < BoardSize - 1; Col++)
        {
            int32 CurrentIndex = Row * BoardSize + Col;
            int32 NextIndex = CurrentIndex + 1;
            if (_Board[CurrentIndex] == _Board[NextIndex])
            {
                return true;
            }
        }
    }

    // Check for possible merges vertically
    for (int32 Col = 0; Col < BoardSize; ++Col)
    {
        for (int32 Row = 0; Row < BoardSize - 1; ++Row)
        {
            int32 CurrentIndex = Row * BoardSize + Col;
            int32 NextIndex = CurrentIndex + BoardSize;
            if (_Board[CurrentIndex] == _Board[NextIndex])
            {
                return true;
            }
        }
    }

    // No moves possible
    return false;
}



void AGameManager::ShiftLeft()
{
    for (int32 i = 0; i < 4; ++i)
    {
        int32 CurrentPos = 3;
        for (int32 j = 3; j >= 0; --j)
        {
            if (Board[i][j] != 0)
            {
                if (j != CurrentPos)
                {
                    Board[i][CurrentPos] = Board[i][j];
                    Board[i][j] = 0;
                    TileMovedOrMerged = true;
                }
                CurrentPos--;
            }
        }
    }
}

void AGameManager::CombineTilesLeft()
{
    for (int32 i = 0; i < 4; ++i)
    {
        for (int32 j = 3; j > 0; --j)
        {
            if (Board[i][j] != 0 && Board[i][j] == Board[i][j - 1])
            {
                Board[i][j] *= 2;
                Board[i][j - 1] = 0;
                TileMovedOrMerged = true;
                j--;  // Skip next element to avoid double combining
            }
        }
    }
}
void AGameManager::ShiftDown()
{
    for (int32 j = 0; j < 4; ++j)
    {
        int32 CurrentPos = 0;
        for (int32 i = 0; i < 4; ++i)
        {
            if (Board[i][j] != 0)
            {
                if (i != CurrentPos)
                {
                    Board[CurrentPos][j] = Board[i][j];
                    Board[i][j] = 0;
                    TileMovedOrMerged = true;
                }
                CurrentPos++;
            }
        }
    }
}

void AGameManager::CombineTilesDown()
{
    for (int32 j = 0; j < 4; ++j)
    {
        for (int32 i = 0; i < 3; ++i)
        {
            if (Board[i][j] != 0 && Board[i][j] == Board[i + 1][j])
            {
                Board[i][j] *= 2;
                Board[i + 1][j] = 0;
                TileMovedOrMerged = true;
                i++;  // Skip next element to avoid double combining
            }
        }
    }
}

void AGameManager::ShiftUp()
{
    for (int32 j = 0; j < 4; ++j)
    {
        int32 CurrentPos = 3;
        for (int32 i = 3; i >= 0; --i)
        {
            if (Board[i][j] != 0)
            {
                if (i != CurrentPos)
                {
                    Board[CurrentPos][j] = Board[i][j];
                    Board[i][j] = 0;
                    TileMovedOrMerged = true;
                }
                CurrentPos--;
            }
        }
    }
}

void AGameManager::CombineTilesUp()
{
    for (int32 j = 0; j < 4; ++j)
    {
        for (int32 i = 3; i > 0; --i)
        {
            if (Board[i][j] != 0 && Board[i][j] == Board[i - 1][j])
            {
                Board[i][j] *= 2;
                Board[i - 1][j] = 0;
                TileMovedOrMerged = true;
                i--;  // Skip next element to avoid double combining
            }
        }
    }
}
