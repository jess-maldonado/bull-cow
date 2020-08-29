// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();

    PrintLine(TEXT("Welcome to the Bull Cow game!"));
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    ClearScreen();

    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        CheckInput(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("cakes");
    Lives = HiddenWord.Len() + 1;
    bGameOver = false;
    PrintLine((TEXT("Guess the %i letter isogram to continue")), HiddenWord.Len()); // Remove hardcoded
}

void UBullCowCartridge::EndGame(const FString &Input)
{
    bGameOver = true;
    PrintLine(TEXT("Do you want to play again? Press Enter to continue"));
}

void UBullCowCartridge::CheckInput(const FString &Input)
{
    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You Win!"));
        EndGame(Input);
        return;
    };
    if (Input.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Guess needs to be %i letters long. Please try again"), HiddenWord.Len());
        return;
    };
    // Also want to check for isogram
    // if not isogram, give warning.
    if (Lives > 1)
    {
        --Lives;
        PrintLine(TEXT("Wrong word, please try again. You have %i lives left."), Lives);
        return;
    };
    PrintLine(TEXT("You Lose."));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    EndGame(Input);
}