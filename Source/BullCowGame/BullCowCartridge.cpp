// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString &Word) {
        return Word.Len() > 4 && Word.Len() < 8 && IsIsogram(Word);
    });

    PrintLine(TEXT("Welcome to the Bull Cow game!"));
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{

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
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() + 1;
    bGameOver = false;
    PrintLine((TEXT("Guess the %i letter isogram to continue")), HiddenWord.Len());
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
        PrintLine(TEXT("Guess must have no repeating letters. \nPlease try again"));
        return;
    }
    if (Lives > 1)
    {
        --Lives;
        FBullCowCount Count = GetBullCows(Input);

        PrintLine(TEXT("You Have %i bulls and %i cows. \nYou have %i lives left.\nPlease guess again."), Count.Bulls, Count.Cows, Lives);
        return;
    };
    PrintLine(TEXT("You Lose."));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    EndGame(Input);
}

bool UBullCowCartridge::IsIsogram(const FString &Input)
{
    for (int i = 0; i < Input.Len(); i++)
    {
        for (int j = i + 1; j < Input.Len(); j++)
        {
            if (Input[i] == Input[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() > 4 && Word.Len() < 10 && IsIsogram(Word))
            ValidWords.Emplace(Word);
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Input) const
{
    FBullCowCount Count;

    for (int32 i = 0; i < Input.Len(); i++)
    {
        if (Input[i] == HiddenWord[i])
        {
            ++Count.Bulls;
            continue;
        }
        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Input[i] == HiddenWord[j])
            {
                ++Count.Cows;
                break;
            }
        }
    }
    return Count;
}