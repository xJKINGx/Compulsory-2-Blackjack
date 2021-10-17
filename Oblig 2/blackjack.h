#pragma once
#include <iostream>
#include <string>

std::string username{};
std::string password{};
std::string hitOrStand{};
std::string exitQuestion{};
int tries = 1;
int playerBal = 100;
int AIBal = 100;
int minBet = 10; // minBet is the minimum amount allowed to bet, here it's $10
int playerCardValue{};
int AICardValue{};
int betAmount{};
bool loginLoop = true;
bool gameLoop = true;
bool playerStand = false;
bool betLoop = true;


void playerTurn();
void AITurn();
int drawCard();
void endGame();
void refreshVariables();
void clearCin();