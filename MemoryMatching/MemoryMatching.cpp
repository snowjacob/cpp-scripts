#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

// FRUIT MATCH : Author : Jacob Snow
// Specification : C++ Memory Matching Game

// Global variables

// Mouse starting point for game board
int clickedRow = -1;
int clickedCol = -1;

// Specify the number of rows and columns
// HERE is where difficulty can be changed (still, all images will be loaded in, but we may implement
// different difficulties in the future)
int numRows = 4;
int numCols = 6;

// Specify the dimensions
int cardWidth = 100; // Set the width of each card (in pixels)
int cardHeight = 150; // Set the height of each card (in pixels)
int tableWidth = numCols * (cardWidth + 10); // Set the width of the table (including spacing)
int tableHeight = numRows * (cardHeight + 10); // Set the height of the table (including spacing)

struct Card {
    int value;          // Unique identifier for the card
    bool matched;       // Flag to indicate if the card is matched
    bool revealed;      // Flag to indicate if the card is currently revealed
};

// Function to generate a shuffled deck of cards
std::vector<Card> generateDeck(int numPairs) {
    std::vector<Card> deck;
    for (int i = 1; i <= numPairs; ++i) {
        deck.push_back({ i, false, false });  // Add the first card of the pair
        deck.push_back({ i, false, false });  // Add the second card of the pair
    }

    // Shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    return deck;
}

// Function to create the game board
std::vector<std::vector<Card>> createBoard(int numRows, int numCols) {
    int numPairs = (numRows * numCols) / 2;
    std::vector<Card> deck = generateDeck(numPairs);

    std::vector<std::vector<Card>> board(numRows, std::vector<Card>(numCols));

    // Fill the board with cards from the deck
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            board[i][j] = deck[i * numCols + j];
        }
    }

    return board;
}

// Mouse event callback function
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        int row = y / (cardHeight + 10);
        int col = x / (cardWidth + 10);
        clickedRow = row;
        clickedCol = col;
    }
}

void drawText(cv::Mat& image, const std::string& text, const cv::Point& position) {
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.7;
    int thickness = 2;
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    baseline += thickness;

    cv::Point textOrg(position.x - textSize.width / 2, position.y + textSize.height / 2);
    cv::putText(image, text, textOrg, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
}

bool isValidCardClick(int row, int col, const std::vector<std::vector<Card>>& board) {
    // Check if the clicked coordinates are within the bounds of the board
    if (row >= 0 && row < board.size() && col >= 0 && col < board[0].size()) {
        // Check if the clicked card is face-down (not revealed or matched)
        const Card& clickedCard = board[row][col];
        if (!clickedCard.revealed && !clickedCard.matched) {
            return true;  // Valid card click
        }
    }
    return false;  // Invalid card click
}

bool isRevealed(int row, int col, const std::vector<std::vector<Card>>& board) {
    return board[row][col].revealed;
}

int main() {
    std::cout << "Welcome to Fruit Match!" << std::endl;
    // Grab images from bitfuul-images, representing the cards and card-backs
    cv::Mat card1 = cv::imread("bitfuul-images/apple.jpg");
    cv::Mat card2 = cv::imread("bitfuul-images/banana.jpg");
    cv::Mat card3 = cv::imread("bitfuul-images/cherry.jpg");
    cv::Mat card4 = cv::imread("bitfuul-images/grapes.jpg");
    cv::Mat card5 = cv::imread("bitfuul-images/lemon.jpg");
    cv::Mat card6 = cv::imread("bitfuul-images/orange.jpg");
    cv::Mat card7 = cv::imread("bitfuul-images/pear.jpg");
    cv::Mat card8 = cv::imread("bitfuul-images/pineapple.jpg");
    cv::Mat card9 = cv::imread("bitfuul-images/strawberry.jpg");
    cv::Mat card10 = cv::imread("bitfuul-images/watermelon.jpg");
    cv::Mat card11 = cv::imread("bitfuul-images/kiwi-fruit.jpg");
    cv::Mat card12 = cv::imread("bitfuul-images/mango.jpg");
    cv::Mat cardBack = cv::imread("bitfuul-images/card-back.jpg");

    // Resize the images
    cv::resize(card1, card1, cv::Size(cardWidth, cardHeight));
    cv::resize(card2, card2, cv::Size(cardWidth, cardHeight));
    cv::resize(card3, card3, cv::Size(cardWidth, cardHeight));
    cv::resize(card4, card4, cv::Size(cardWidth, cardHeight));
    cv::resize(card5, card5, cv::Size(cardWidth, cardHeight));
    cv::resize(card6, card6, cv::Size(cardWidth, cardHeight));
    cv::resize(card7, card7, cv::Size(cardWidth, cardHeight));
    cv::resize(card8, card8, cv::Size(cardWidth, cardHeight));
    cv::resize(card9, card9, cv::Size(cardWidth, cardHeight));
    cv::resize(card10, card10, cv::Size(cardWidth, cardHeight));
    cv::resize(card11, card11, cv::Size(cardWidth, cardHeight));
    cv::resize(card12, card12, cv::Size(cardWidth, cardHeight));
    cv::resize(cardBack, cardBack, cv::Size(cardWidth, cardHeight));

    std::vector<std::vector<Card>> board = createBoard(numRows, numCols);

    // Create a window to display the game
    cv::namedWindow("Memory Matching Game", cv::WINDOW_NORMAL);
    cv::resizeWindow("Memory Matching Game", tableWidth, tableHeight);

    // Set the mouse callback function
    cv::setMouseCallback("Memory Matching Game", onMouse);

    // Game loop
    bool gameOver = false;
    int score = 0;
    int matches = 0;
    bool click1 = false;
    bool click2 = false;
    bool click3 = false;
    Card* clickedCard1 = nullptr;
    Card* clickedCard2 = nullptr;

    while (!gameOver) {
        // Display the game board
        cv::Mat gameBoard(tableHeight, tableWidth, CV_8UC3, cv::Scalar(0, 0, 0));
        int x = 0;
        int y = 0;

        for (const auto& row : board) {
            for (const auto& card : row) {
                cv::Mat cardImage;

                if (card.revealed || card.matched) {
                    if (card.matched) {
                        cardImage = cv::Mat(cardHeight, cardWidth, CV_8UC3, cv::Scalar(0, 255, 0));
                    }
                    else {
                        switch (card.value) {
                        case 1:
                            cardImage = card1;
                            break;
                        case 2:
                            cardImage = card2;
                            break;
                        case 3:
                            cardImage = card3;
                            break;
                        case 4:
                            cardImage = card4;
                            break;
                        case 5:
                            cardImage = card5;
                            break;
                        case 6:
                            cardImage = card6;
                            break;
                        case 7:
                            cardImage = card7;
                            break;
                        case 8:
                            cardImage = card8;
                            break;
                        case 9:
                            cardImage = card9;
                            break;
                        case 10:
                            cardImage = card10;
                            break;
                        case 11:
                            cardImage = card11;
                            break;
                        case 12:
                            cardImage = card12;
                            break;
                        }
                    }
                }
                else {
                    cardImage = cardBack;
                }

                cv::Rect roi(x, y, cardWidth, cardHeight);
                cardImage.copyTo(gameBoard(roi));
                x += cardWidth + 10;
            }

            x = 0;
            y += cardHeight + 10;
        }

        // Display the score
        std::string scoreText = "Score: " + std::to_string(score);
        cv::Point scorePosition(tableWidth - 120, tableHeight - 20);
        drawText(gameBoard, scoreText, scorePosition);

        // Display the game board
        cv::imshow("Memory Matching Game", gameBoard);

        // Check for user input
        int key = cv::waitKey(1);

        // Handle user input
        if (key == 27) {
            // Exit the game if the user presses the Esc key
            break;
        }

        // Check if the game is over
        if (matches == (numRows * numCols) / 2) {
            break;
        }

        // If we're starting a new round, hide the previous two cards
        if (click3 && (clickedRow != -1 || clickedCol != -1) && isValidCardClick(clickedRow, clickedCol, board)) {
            clickedCard1->revealed = false;
            clickedCard2->revealed = false;
            click3 = false;
        }

        // Beginning of game ~ select first card
        if ((clickedRow != -1 || clickedCol != -1) && !click1 && ! click3 && isValidCardClick(clickedRow, clickedCol, board)) {
            clickedCard1 = &board[clickedRow][clickedCol];
            clickedCard1->revealed = true;
            clickedRow = -1;
            clickedCol = -1;
            click1 = true;
        }

        // Select second card
        else if ((clickedRow != -1 || clickedCol != -1) && click1 && isValidCardClick(clickedRow, clickedCol, board)) {
            clickedCard2 = &board[clickedRow][clickedCol];
            clickedCard2->revealed = true;
            clickedRow = -1;
            clickedCol = -1;
            click2 = true;
        }

        // Check if the two revealed cards match ~ 5 points for a match, -1 point for a miss
        else if (click1 && click2) {
            if (clickedCard1->value == clickedCard2->value) {
                clickedCard1->matched = true;
                clickedCard2->matched = true;
                score += 5;
                matches++;
            }
            else {
                score -= 1;
            }
            click1 = false;
            click2 = false;
            click3 = true;
        }

        // Reset the clicked card coordinates
        clickedRow = -1;
        clickedCol = -1;
    }

    std::cout << "Final Score: " << score << std::endl;

    // Clean up and exit the program
    cv::destroyAllWindows();

    return 0;
}