// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    PrintLine(TEXT("Welcome to the Bull Cow game!"));

    TArray<FString> ValidWords = GetValidWords(Words);
    PrintLine(TEXT("Number of valid words is: %i"), ValidWords.Num());
    SetupGame();
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

    const TCHAR HW[] = TEXT("cakes");
}

void UBullCowCartridge::EndGame(const FString &Input)
{
    bGameOver = true;
    PrintLine(TEXT("Do you want to play again? \nPress Enter to continue"));
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
        PrintLine(TEXT("Guess needs to be %i letters long. \nPlease try again"), HiddenWord.Len());
        return;
    };
    if (!IsIsogram(Input))
    {
        PrintLine(TEXT("Guess must be an isogram, which \nmeans it has no repeating letters. \nPlease try again"));
        return;
    }
    if (Lives > 1)
    {
        --Lives;
        PrintLine(TEXT("Wrong word, please try again. \nYou have %i lives left."), Lives);
        return;
    };
    PrintLine(TEXT("You Lose."));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    EndGame(Input);
}

bool UBullCowCartridge::IsIsogram(const FString &Input) const
{
    for (int i = 0; i < Input.Len(); i++)
    {
        for (int j = i + 1; j < Input.Len() - j; j++)
        {
            if (Input[i] == Input[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() > 4 && Word.Len() < 10 && IsIsogram(Word))
            ValidWords.Emplace(Word);
    }
    return ValidWords;
}