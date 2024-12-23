
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm> 
#include "AlgosHeader.h"

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const sf::Color BAR_COLOR = sf::Color(255, 0, 0);
// Function to handle user input and store it in the array
template<typename t>
void handleUserInput(sf::RenderWindow& window, vector<t>& arr, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text prompt("Enter up to 15 numbers separated by spaces\nPress Enter to finish:", font, 20);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(10, 10);

    sf::Text userInput("", font, 20);
    userInput.setFillColor(sf::Color::Yellow);
    userInput.setPosition(10, 100);

    string input = "";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                }
                else if (event.text.unicode == '\r') {
                    // Process input (parse numbers, strings, or doubles as needed)
                    stringstream ss(input);
                    string token;
                    while (ss >> token) {
                        try {
                            double number = stod(token); // Convert token to double if it's numeric
                            arr.push_back(number);      // Push to array
                        }
                        catch (const invalid_argument&) {
                            cout << "Invalid input: " << token << endl;
                            // Optionally, handle non-numeric tokens
                        }
                        if (arr.size() >= 15) break; // Limit to 15 elements
                    }
                    return;
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    input += static_cast<char>(event.text.unicode); // Append printable characters
                }
            }
        }

        userInput.setString(input);

        window.clear();
        window.draw(background);
        window.draw(prompt);
        window.draw(userInput);
        window.display();

    }
}

// Structure to hold sorting algorithm choice and order
struct SortChoice {
    int algorithmIndex;
    bool ascending;
};

// Function to display the sorting menu and get the sorting choice
SortChoice displaySortMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Sorting Algorithm", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    vector<string> sortNames = {
        "Bubble Sort", "Selection Sort", "Insertion Sort",
        "Merge Sort", "Quick Sort", "Radix Sort", "Heap Sort"
    };

    vector<sf::RectangleShape> buttons;
    vector<sf::Text> buttonTexts;

    for (size_t i = 0; i < sortNames.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(200, 40));
        button.setPosition(WINDOW_WIDTH / 2 - 100, 150 + i * 50);
        button.setFillColor(sf::Color(50, 150, 50));
        buttons.push_back(button);

        sf::Text buttonText(sortNames[i], font, 20);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(button.getPosition().x + 10, button.getPosition().y + 5);
        buttonTexts.push_back(buttonText);
    }

    SortChoice choice;
    choice.algorithmIndex = -1;
    choice.ascending = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return choice;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        choice.algorithmIndex = static_cast<int>(i);
                        return choice;
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return choice;
}

// Function to display the order selection menu and get the sorting order
bool displayOrderMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Sorting Order", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    sf::RectangleShape ascendingButton(sf::Vector2f(150, 40));
    ascendingButton.setPosition(WINDOW_WIDTH / 2 - 200, 200);
    ascendingButton.setFillColor(sf::Color(50, 150, 50));

    sf::RectangleShape descendingButton(sf::Vector2f(150, 40));
    descendingButton.setPosition(WINDOW_WIDTH / 2 + 50, 200);
    descendingButton.setFillColor(sf::Color(150, 50, 50));

    sf::Text ascendingText("Ascending", font, 20);
    ascendingText.setFillColor(sf::Color::White);
    ascendingText.setPosition(ascendingButton.getPosition().x + 10, ascendingButton.getPosition().y + 5);

    sf::Text descendingText("Descending", font, 20);
    descendingText.setFillColor(sf::Color::White);
    descendingText.setPosition(descendingButton.getPosition().x + 10, descendingButton.getPosition().y + 5);

    bool ascending = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return ascending;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (ascendingButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return true;
                }
                else if (descendingButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return false;
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(ascendingButton);
        window.draw(descendingButton);
        window.draw(ascendingText);
        window.draw(descendingText);
        window.display();
    }

    return ascending;
}

int displaySpeedMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Animation Speed", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    vector<string> speedLabels = { "Slow", "Normal", "Fast" };
    vector<int> speedValues = { 100, 50, 10 }; // Delays in milliseconds

    vector<sf::RectangleShape> buttons;
    vector<sf::Text> buttonTexts;

    // Create buttons dynamically
    for (size_t i = 0; i < speedLabels.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(200, 50));
        button.setPosition(WINDOW_WIDTH / 2 - 100, 150 + i * 70);
        button.setFillColor(sf::Color(50, 150, 50));
        buttons.push_back(button);

        sf::Text buttonText(speedLabels[i], font, 25);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(button.getPosition().x + 50, button.getPosition().y + 10);
        buttonTexts.push_back(buttonText);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return speedValues[1]; // Default to "Normal" speed
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        return speedValues[i];
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return speedValues[1]; // Default to "Normal" speed
}



int main() {
    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting Algorithm Visualization");

    // Load sounds
    sf::SoundBuffer swapBuffer, finishBuffer;
    if (!swapBuffer.loadFromFile("E:/VS projects/GUI trial/maro-jump-sound-effect_1.wav") || !finishBuffer.loadFromFile("E:/VS projects/GUI trial/untitled_3.wav")) {
        cout << "Error loading sound files!" << endl;
        return -1;
    }
    sf::Sound swapSound(swapBuffer), finishSound(finishBuffer);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("E:/VS projects/GUI trial/SuperMario256.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Load background texture
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("E:/VS projects/GUI trial/brown-brick-wall-texture-wall-bricks_146714-3037.png")) {
        cout << "Error loading background image!" << endl;
        return -1;
    }
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setTexture(&bgTexture);

    // Handle user input to get the array of numbers
    vector<int> arr;
    finishSound.play();
    handleUserInput(window, arr, font, background);

    SortingAlgorithms<int> sorting;

    if (!arr.empty()) {
        // Display sorting menu and get the sorting choice
        int animationSpeed = displaySpeedMenu(window, font, background);
        SortChoice sortChoice = displaySortMenu(window, font, background);

        int maxValue = *max_element(arr.begin(), arr.end());

        if (sortChoice.algorithmIndex != -1) { // Check if an algorithm was selected
            // Display order selection menu and get the sorting order
            bool ascending = displayOrderMenu(window, font, background);

            switch (sortChoice.algorithmIndex) {
            case 0:
                sorting.bubbleSort(arr, arr.size(), maxValue, window, font,  swapSound, animationSpeed);
                break;
            case 1:
                sorting.selectionSort(arr, arr.size(), maxValue, window, font, swapSound, animationSpeed);
                break;
            case 2:
                sorting.insertionSort(arr, maxValue, window, font, swapSound, animationSpeed);
                break;
            case 3: 
                sorting.mergeSort(window, arr, font, maxValue, 0, arr.size() - 1, swapSound, animationSpeed);
                break;
            case 4:
                sorting.QuickSort_recursion(arr,0,arr.size()-1,maxValue,window,font,swapSound, animationSpeed);
                break;
            case 5:
                sorting.radix_sort(arr, maxValue, window, font, swapSound, animationSpeed);
                break;
            case 6:
                sorting.HeapSort(arr, arr.size(), window, font, maxValue, swapSound, animationSpeed);
                break;
            default:
                cout << "Invalid algorithm selection." << endl;
                break;
            }
        }
        else {
            cout << "No algorithm selected or window closed." << endl;
        }
    }

    cout << "Press Enter to exit..." << endl;

    return 0;
}

//marker


/*
#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// Constants for window dimensions and bar color
const int WINDOW_WIDTH = 800; // Width of the window
const int WINDOW_HEIGHT = 600; // Height of the window
const sf::Color BAR_COLOR = sf::Color(255, 0, 0); // Red color for bars

// Function to draw the bars with dynamic scaling and labels
void drawBars(sf::RenderWindow& window, const vector<int>& arr, sf::Font& font, int maxValue,
    int current = -1, int minIndex = -1, float offsetX1 = 0, float offsetY1 = 0,
    float offsetX2 = 0, float offsetY2 = 0) // Default parameters for animation
{

    window.clear(); // Clear the window

    int numBars = arr.size(); // Number of bars to draw
    if (numBars == 0) { // Return if the array is empty
        window.display(); // Display the window
        return; // Exit the function
    }

    // Calculate bar width and scaling factor
    int barWidth = (WINDOW_WIDTH / numBars) - 10; // Width of each bar
    float scaleFactor = static_cast<float>(WINDOW_HEIGHT - 50) / maxValue; // Scale factor for bar height

    for (size_t i = 0; i < arr.size(); i++) { // Loop through the array
        float barHeight = arr[i] * scaleFactor; // Calculate bar height based on value
        sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight)); // Create a bar shape

        float xPosition = i * (barWidth + 10); // Calculate x position of the bar
        float yPosition = WINDOW_HEIGHT - barHeight - 25; // Calculate y position of the bar

        // Adjust position for animated bars
        if (i == current) // If the current bar is being animated
        {
            bar.setPosition(xPosition + offsetX1, yPosition + offsetY1); // Adjust position
            bar.setFillColor(sf::Color::Blue); // Highlight current bar in blue
        }
        else if (i == minIndex) // If the minIndex bar is being animated
        {
            bar.setPosition(xPosition + offsetX2, yPosition + offsetY2); // Adjust position
            bar.setFillColor(sf::Color::Blue); // Highlight minIndex bar in blue
        }
        else // Otherwise, set the default position and color
        {
            bar.setPosition(xPosition, yPosition); // Set the position of the bar
            bar.setFillColor(BAR_COLOR); // Default bar color
        }

        window.draw(bar); // Draw the bar on the window

        // Draw the value of the bar as a label
        sf::Text label(to_string(arr[i]), font, 15); // Create a text label
        float labelX = bar.getPosition().x + barWidth / 2 - label.getGlobalBounds().width / 2; // Center the label
        float labelY = bar.getPosition().y - label.getGlobalBounds().height - 5; // Position above the bar
        label.setPosition(labelX, labelY); // Set the position of the label
        label.setFillColor(sf::Color::White); // Set the color of the label
        window.draw(label); // Draw the label on the window
    }
    window.display(); // Display the window
}

// Function to animate the swapping of two bars
void animateSwap(sf::RenderWindow& window, vector<int>& arr, sf::Font& font, int maxValue, int index1, int index2, sf::Sound& sound, float speedMultiplier)
{
    float totalFrames = 50.0; // Total number of frames for the animation
    int barWidth = (WINDOW_WIDTH / arr.size()) - 10; // Width of each bar

    for (float frame = 0; frame <= totalFrames; frame++) // Loop through the frames
    {
        float progress = frame / totalFrames; // Calculate the progress of the animation
        float offsetX1 = (index2 - index1) * barWidth * progress; // Calculate the offset for bar 1
        float offsetX2 = (index1 - index2) * barWidth * progress; // Calculate the offset for bar 2

        float offsetY = -4 * 100 * progress * (1 - progress); // Parabolic movement

        drawBars(window, arr, font, maxValue, index1, index2, offsetX1, offsetY, offsetX2, offsetY); // Draw the bars with animation

        if (frame == 0) // If it's the first frame of the animation
        {
            sound.play(); // Play sound when the swap animation starts
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(speedMultiplier)));

    }

    swap(arr[index1], arr[index2]); // Swap the elements in the array
    drawBars(window, arr, font, maxValue); // Draw the final state after the swap
}

// Selection sort implementation
void selectionSort(sf::RenderWindow& window, vector<int>& arr, sf::Font& font, sf::Sound& swapSound, sf::Sound& finishSound, bool ascending, float speedMultiplier) {
    int maxValue = *max_element(arr.begin(), arr.end()); // Find the maximum value in the array

    for (size_t i = 0; i < arr.size() - 1; i++)
    {
        size_t targetIndex = i;

        for (size_t j = i + 1; j < arr.size(); j++) {
            drawBars(window, arr, font, maxValue);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Brief pause for comparison

            if (ascending && arr[j] < arr[targetIndex]) {
                targetIndex = j;
            }
            else if (!ascending && arr[j] > arr[targetIndex]) {
                targetIndex = j;
            }
        }

        if (targetIndex != i) {
            animateSwap(window, arr, font, maxValue, i, targetIndex, swapSound, speedMultiplier);
        }
    }

    drawBars(window, arr, font, maxValue); // Draw the final sorted array
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause to show the sorted array
    finishSound.play(); // Play finish sound
}

// Function to handle user input and store it in the array
void handleUserInput(sf::RenderWindow& window, vector<int>& arr, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text prompt("Enter up to 15 numbers separated by spaces\nPress Enter to finish:", font, 20);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(10, 10);

    sf::Text userInput("", font, 20);
    userInput.setFillColor(sf::Color::Yellow);
    userInput.setPosition(10, 100);

    string input = "";
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) { // Backspace
                    input.pop_back();
                }
                else if (event.text.unicode == '\r') { // Enter key
                    stringstream ss(input);
                    int number;
                    while (ss >> number && arr.size() < 15) {
                        arr.push_back(number);
                    }
                    return;
                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126) { // Printable characters
                    input += static_cast<char>(event.text.unicode);
                }
            }
        }

        userInput.setString(input);

        window.clear();
        window.draw(background);
        window.draw(prompt);
        window.draw(userInput);
        window.display();
    }
}

// Structure to hold sorting algorithm choice and order
struct SortChoice {
    int algorithmIndex;
    bool ascending;
};

// Function to display the sorting menu and get the sorting choice
SortChoice displaySortMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Sorting Algorithm", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    vector<string> sortNames = {
        "Bubble Sort", "Selection Sort", "Insertion Sort",
        "Merge Sort", "Quick Sort", "Radix Sort", "Heap Sort"
    };

    vector<sf::RectangleShape> buttons;
    vector<sf::Text> buttonTexts;

    // Create buttons dynamically based on sortNames
    for (size_t i = 0; i < sortNames.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(200, 40));
        button.setPosition(WINDOW_WIDTH / 2 - 100, 150 + i * 50);
        button.setFillColor(sf::Color(50, 150, 50));
        buttons.push_back(button);

        sf::Text buttonText(sortNames[i], font, 20);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(button.getPosition().x + 10, button.getPosition().y + 5);
        buttonTexts.push_back(buttonText);
    }

    SortChoice choice;
    choice.algorithmIndex = -1; // Initialize to an invalid value
    choice.ascending = true; // Default to ascending

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return choice; // Return default choice if window is closed
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        choice.algorithmIndex = static_cast<int>(i);
                        return choice; // Return choice immediately after an algorithm is selected
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return choice; // Return default choice if loop ends unexpectedly
}

// Function to display the order selection menu and get the sorting order
bool displayOrderMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Sorting Order", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    // Add buttons for ascending/descending order
    sf::RectangleShape ascendingButton(sf::Vector2f(150, 40));
    ascendingButton.setPosition(WINDOW_WIDTH / 2 - 200, 200);
    ascendingButton.setFillColor(sf::Color(50, 150, 50));

    sf::RectangleShape descendingButton(sf::Vector2f(150, 40));
    descendingButton.setPosition(WINDOW_WIDTH / 2 + 50, 200);
    descendingButton.setFillColor(sf::Color(150, 50, 50));

    sf::Text ascendingText("Ascending", font, 20);
    ascendingText.setFillColor(sf::Color::White);
    ascendingText.setPosition(ascendingButton.getPosition().x + 10, ascendingButton.getPosition().y + 5);

    sf::Text descendingText("Descending", font, 20);
    descendingText.setFillColor(sf::Color::White);
    descendingText.setPosition(descendingButton.getPosition().x + 10, descendingButton.getPosition().y + 5);

    bool ascending = true; // Default to ascending

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return ascending; // Return default order if window is closed
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (ascendingButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return true; // Return true for ascending
                }
                else if (descendingButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return false; // Return false for descending
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(ascendingButton);
        window.draw(descendingButton);
        window.draw(ascendingText);
        window.draw(descendingText);
        window.display();
    }

    return ascending; // Return default order if loop ends unexpectedly
}




// Function to display the speed selection menu
int displaySpeedMenu(sf::RenderWindow& window, sf::Font& font, const sf::RectangleShape& background) {
    sf::Text title("Choose Animation Speed", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);

    vector<string> speedLabels = { "Slow", "Normal", "Fast" };
    vector<int> speedValues = { 100, 50, 10 }; // Delays in milliseconds

    vector<sf::RectangleShape> buttons;
    vector<sf::Text> buttonTexts;

    // Create buttons dynamically
    for (size_t i = 0; i < speedLabels.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(200, 50));
        button.setPosition(WINDOW_WIDTH / 2 - 100, 150 + i * 70);
        button.setFillColor(sf::Color(50, 150, 50));
        buttons.push_back(button);

        sf::Text buttonText(speedLabels[i], font, 25);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(button.getPosition().x + 50, button.getPosition().y + 10);
        buttonTexts.push_back(buttonText);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return speedValues[1]; // Default to "Normal" speed
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        return speedValues[i];
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return speedValues[1]; // Default to "Normal" speed
}




int main() {
    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting Algorithm Visualization");

    // Load sounds
    sf::SoundBuffer swapBuffer, finishBuffer;
    if (!swapBuffer.loadFromFile("E:/VS projects/GUI trial/maro-jump-sound-effect_1.wav") || !finishBuffer.loadFromFile("E:/VS projects/GUI trial/untitled_3.wav")) {
        cout << "Error loading sound files!" << endl;
        return -1;
    }
    sf::Sound swapSound(swapBuffer), finishSound(finishBuffer);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("E:/VS projects/GUI trial/SuperMario256.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Load background texture
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("E:/VS projects/GUI trial/brown-brick-wall-texture-wall-bricks_146714-3037.png")) {
        cout << "Error loading background image!" << endl;
        return -1;
    }
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setTexture(&bgTexture);

    // Handle user input to get the array of numbers
    vector<int> arr;
    handleUserInput(window, arr, font, background);

    if (!arr.empty()) {
        int animationSpeed = displaySpeedMenu(window, font, background);
        // Display sorting menu and get the sorting choice
        SortChoice sortChoice = displaySortMenu(window, font, background);

        if (sortChoice.algorithmIndex != -1) { // Check if an algorithm was selected
            // Display order selection menu and get the sorting order
            bool ascending = displayOrderMenu(window, font, background);

            switch (sortChoice.algorithmIndex) {
            case 1: // Selection Sort
                selectionSort(window, arr, font, swapSound, finishSound, ascending, animationSpeed / 50.0f);
                break;
                // Add cases for other sorting algorithms here
            default:
                cout << "Invalid algorithm selection." << endl;
                break;
            }
        }
        else {
            cout << "No algorithm selected or window closed." << endl;
        }
    }

    cout << "Press Enter to exit..." << endl;
    cin.get();

    return 0;
}
*/