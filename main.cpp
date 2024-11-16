/*
 * ADDITIONAL FEATURES:
 *
 * 1. Admin Dashboard (PASSWORD IS "Coder"):
 *    - Secure login/logout system.
 *    - Showcases commissions, total profit, and operating costs.
 *    - Identifies the most popular resort based on guest bookings.
 *
 * 2. User Satisfaction Survey:
 *    - Quick, easy-to-use survey allowing ratings out of 5.
 *    - Results displayed in the admin dashboard for quality assessment.
 *
 * 3. Activity Booking:
 *    - Guests can book distinctive activities unique to each resort.
 *
 * 4. Promo Codes:
 *    - Feature to apply promo codes for a 15% discount on bookings. Code is
 *    "SAVE"
 *
 * 5. Enhanced User Interface:
 *    - Clear console function for streamlined experience upon exit.
 *    - Engaging ASCII art enhancing visual appeal.
 *    - Bold and vivid headers for an intuitive and user-friendly interface.
 *
 * 6. Customized Booking Receipts:
 *    - Personalized receipts generated for each booking, provided as a txt
 *      file.
 *    - Comprehensive details included: resort, room type, price, activities,
 *      and more.
 *    - Works per session.
 *
 * 7. Booking Record:
 *    - A complete record of all bookings in a session, stored in a txt file.
 *    - Accessible for admins for efficient tracking and management of bookings.
 *
 * 8. Resort Descriptions:
 *    - Engaging, personalized descriptions for each resort.
 *    - Adds a our unique touch to the booking experience, highlighting resort
 *     features.
 *
 * 9. Unique Real-World Pricing:
 *    - Pricing for rooms is highest at the start of the break.
 *    - Pricing for rooms is lowest at the end of the break.
 */

#include <algorithm>
#include <fstream> // This is needed for reading and writing data to our .txt files (receipt and record)
#include <iostream>
#include <map>
#include <stdlib.h> //For clearing console when exiting
#include <string>   // Enables use of the string class (resort names)
#include <vector> // Allows us to use vector container to store Room objects in Resort

using namespace std;

// Room class (one of the OOP concepts on the assignment sheet)
class Room {
public:
  int capacity, price, available;
  // Room capacity, price of room, number of those rooms available

  Room(int cap, int pr, int avail)
      : capacity(cap), price(pr), available(avail) {}

  bool bookRoom() { return available > 0 ? (--available, true) : false; }
};

class Resort {
public:
  string name, description;
  vector<Room> rooms;
  vector<string> activities;
  vector<int> activityPrices;
  int bookings;

  // Constructor to initialize a room (another OOP concept)
  Resort(const string &n, const vector<Room> &r, const vector<string> &acts,
         const vector<int> &actPrices, const string &desc)
      : name(n), description(desc), rooms(r), activities(acts),
        activityPrices(actPrices), bookings(0) {}

  // Calculate the price based on the first letter of the resort
  int calculatePrice(int week) const {
    int initialPrice = (name[0] == 'A')   ? 300
                       : (name[0] == 'S') ? 325
                       : (name[0] == 'N') ? 290
                       : (name[0] == 'M') ? 400
                                          : 360;
    return max(0, initialPrice - (week - 1) * getPriceReduction());
    // Adjust price based on the week of stay
  }

  // Calculate the price reduction based on the week of stay
  int getPriceReduction() const {
    return (name[0] == 'A')   ? 15
           : (name[0] == 'S') ? 10
           : (name[0] == 'N') ? 12
           : (name[0] == 'M') ? 5
                              : 8;
  }

  // Function to book a room
  bool bookRoom() {
    int week;
    cout
        << "\nEnter the week you want to stay during the Spring Break (1-12): ";
    cin >> week;

    // Validate the week input
    while (cin.fail() || week < 1 || week > 12) {
      cout << "Invalid week. Please enter a week from 1 to 12: ";
      cin.clear();             // Reset cin so user can input again
      cin.ignore(10000, '\n'); // Ignore the rest of the line
      cin >> week;             // Ask for input again
    }

    // Display available room types for booking
    cout << "Select a room type to book:\n";
    for (size_t i = 0; i < rooms.size(); ++i) {
      cout << (i + 1) << ": " << rooms[i].capacity
           << "-person room, Available: " << rooms[i].available << endl;
    }

    int choice; // Get users choice of room
    cout << "\033[1mEnter the room type number: \033[0m";
    cin >> choice;

    // Validate the room choice input from the user
    while (cin.fail() || choice < 1 ||
           choice > static_cast<int>(rooms.size())) {
      cout << "Invalid choice. Please enter a valid room type number: ";
      cin.clear();
      cin.ignore(10000, '\n');
      cin >> choice;
    }

    // Calculating the price of the room
    int totalPrice = calculatePrice(week) + rooms[choice - 1].price;
    vector<string> bookedActivities; //String to store the activites booked

    // Activities booking starts here
    bool bookMoreActivities = true;

    // Booking activites 
    while (bookMoreActivities) {
      // Prompt the user to decide if they want to book an activity (1: Yes, 2:
      // No).
      cout << "Do you want to book an activity? (1: Yes, 2: No): ";
      int activityChoice;
      cin >> activityChoice;

      // Check the user's choice.
      if (activityChoice == 1) {
        // Display a list of available activities.
        cout << "Select an activity to book:\n";
        for (size_t i = 0; i < activities.size(); ++i) {
          cout << (i + 1) << ": " << activities[i] << ", Price: $"
               << activityPrices[i] << endl;
        }

        int activityNumber;
        cout << "Enter the activity number: ";
        cin >> activityNumber;

        // Validate the user's input for the activity number.
        while (cin.fail() || activityNumber < 1 ||
               activityNumber > static_cast<int>(activities.size())) {
          cout << "Invalid choice. Please enter a valid activity number: ";
          cin.clear();
          cin.ignore(10000, '\n');
          cin >> activityNumber;
        }

        // Update the total price and record the booked activity.
        totalPrice += activityPrices[activityNumber - 1];
        bookedActivities.push_back(activities[activityNumber - 1]);
        cout << "Activity booked successfully! Total Price: $" << totalPrice
             << endl;

        // Set the flag to exit the loop, so user can only book activity once.
        bookMoreActivities = false;

      } else {
        // If the user chooses not to book more activities, exit the loop.
        bookMoreActivities = false;
      }
    }

    // Promo code section
    string promoCode;
    cout << "Enter a promo code for a discount (or type 'none' to skip): ";
    cin >> promoCode;
    if (promoCode == "SAVE") {
      totalPrice *= 0.85; // Applying a 15% discount
      cout << "Promo code applied! New Total Price: $" << totalPrice << "\n";
    }

    if (rooms[choice - 1].bookRoom()) {
      bookings++;

      // Writing to the receipt file
      ofstream receiptFile("booking_receipt.txt");
      receiptFile << "Booking Receipt for You! (Sanchith & Armaan Resort "
                     "Bookings Inc.)\n";
      receiptFile << "----------------------\n";
      receiptFile << "Resort: " << name << "\n";
      receiptFile << "Room Type: " << rooms[choice - 1].capacity
                  << "-person room\n";
      receiptFile << "Price: $" << totalPrice << "\n";
      receiptFile << "Booking Week: " << week << "\n";
      if (!bookedActivities.empty()) { //If activies is chosen (not empty)
        receiptFile << "Activity: ";
        for (const auto &activity : bookedActivities) {
          receiptFile << activity; //Insert activity in the receipt
        }
        receiptFile << "\n";
      }
      receiptFile << "----------------------\n";
      receiptFile << "Thank you for your booking!\n";
      receiptFile.close();

      // Apending to the record booking file txt file
      ofstream recordFile("booking_record.txt", ios::app);

      recordFile << "Resort: " << name
                 << ", Room Type: " << rooms[choice - 1].capacity
                 << "-person room, Price: $" << totalPrice
                 << ", Booking Week: " << week;
      if (!bookedActivities.empty()) { //If activies is chosen (not empty)
        recordFile << ", Activity: ";
        for (const auto &activity : bookedActivities) {
          recordFile << activity;
        }
      }
      recordFile << "\n";
      recordFile.close();

      cout << "\nYour booking has been stored in our records! Please view your "
              "receipt.\n";
      return true;
    } else {
      cout << "Room not available." << endl;
      return false;
    }
  }

  // Function to view bookings
  void viewBookings() const {
    cout << "\033[1m\033[35mResort: " << name << "\033[0m - " << description
         << ", Bookings: " << bookings << "\n";
    for (const auto &room : rooms) {
      cout << "Room Capacity: " << room.capacity
           << ", Available: " << room.available << endl;
    }
    cout << endl;
  }
};

class HotelManagementSystem {
  map<string, int> surveyResults; //The rating received on that session
  bool surveyCompleted = false; // is the survey completed? Boolean
  double totalCommissions = 0, operatingCost = 100; // This is needed to calculate the total profit

public:
  void conductSurvey() {
    if (surveyCompleted) {
      cout << "Survey already completed for this session.\n";
      return;
    }

    cout << "Please rate our service (1-5): ";
    cin >> surveyResults["Service Rating"];
    surveyCompleted = true;
  }

  void adminAccess(const vector<Resort> &resorts) {
    string password; //Initiallize password
    cout << "Enter admin password: "; //Ask for it
    cin >> password; //Store user password attempt
 
    if (password == "Coder") { //Checks if the password is correct
      int adminOption;
      do {
        //Admin Menu
        cout << "\nAdmin Panel Options:\n"
             << "1. View Reports\n"
             << "2. Logout\n"
             << "Enter your choice: ";
        cin >> adminOption;

        //Displaying statisitics for admin panel menu option number 1
        if (adminOption == 1) {
          cout << "\033[1m\033[34m\nReports:\033[0m\n";
          cout << "Commissions Earned: $" << totalCommissions << endl;
          cout << "Operating Cost: $" << operatingCost << endl;
          cout << "Total Profit: $" << (totalCommissions - operatingCost) //calculating total profit
               << endl;
          //Display rating user has given
          if (surveyCompleted) {
            cout << "Service Rating: " << surveyResults["Service Rating"]
                 << "/5\n";
          } else {
            cout << "No survey data available.\n"; //If user does not do the survey
          }

          // Most popular resort calculation
          auto mostPopularResort =
              max_element(resorts.begin(), resorts.end(),
                          [](const Resort &a, const Resort &b) {
                            return a.bookings < b.bookings;
                          });
          //Display most the selected resort in the session (mode)
          if (mostPopularResort != resorts.end()) {
            cout << "Most Popular Destination: " << mostPopularResort->name
                 << " with " << mostPopularResort->bookings << " bookings.\n";
          }
        } else if (adminOption == 2) {
          cout << "Logging out...\n";   //Logs out when user presses 2
        } else {
          cout << "Invalid option, please try again.\n"; //When user presses something other than 1 or 2
        }
      } while (adminOption != 2); 
    } else {
      cout << "Incorrect password.\n"; //Wrong password, leads back to general menu.
    }
  }

  void addCommission(double amount) { totalCommissions += amount; } //Update total commisions
};

int main() {
  // Clear previous booking records/receipts 
  ofstream receiptFile("booking_receipt.txt", ios::trunc);
  ofstream recordFile("booking_record.txt", ios::trunc);

  vector<Resort> resorts = { //Vector to store resorts with their information.
      {"ARMAAN'S PARADISE",
       {{4, 100, 20}, {2, 80, 15}},
       {"Sightseeing Tour", "Wine Club"},
       {100, 80},
       "  A serene getaway with stunning views and creative designs, owned by Armaan."},
      {"SANCHITH'S URBAN OASIS",
       {{3, 150, 10}, {4, 120, 20}},
       {"City Tour", "Rent a Exotic Car"},
       {90, 200},
       "  Sanchith's personalized grand resort, made with the most expensive materials."},
      {"ALGORITHM ACRES",
       {{5, 200, 15}, {3, 120, 10}},
       {"  Arcade Gaming Session", "Code4Acres"},
       {40, 65},
       "  A tech-savvy resort for the modern traveler, equipped with the latest "
       "coding gadgets."},
      {"NORTH PARK HAVEN",
       {{6, 250, 10}, {4, 180, 20}},
       {"Gardening Session", "Plant Trees 4 Lyfe"},
       {10, 20},
       "  North Park's own futuristic resort, with rooftop greenary."},
      {"MARRIOT (MS. BAILEY'S FAVOURITE RESORT)",
       {{3, 120, 15}, {2, 90, 10}},
       {"  Japanese Massage Session", "Ms. Bailey's Exclusive Hack Class"},
       {150, 100},
       "  Elegant and sophisticated, offering a first-class experience. Recommended by Ms.Bailey."}};

  cout << "Welcome to the Spring Break R Us Booking System (Sanchith & Armaan "
          "Resort Bookings Inc.)!\n";

  int menuOption;
  HotelManagementSystem managementSystem;

  do {
    cout << "\033[36m"; // Set text color to cyan
    cout << "     _______.        ___                                          "
            "\n";
    cout << "    /       |       /   \\                                        "
            " \n";
    cout << "   |   (----`      /  ^  \\                                       "
            "\n";
    cout << "    \\   \\         /  /_\\  \\                                   "
            "   \n";
    cout << ".----)   |    __ /  _____  \\                                     "
            "\n";
    cout << "|_______/    (__)__/     \\__\\                                   "
            " \n";
    cout << "                                                                 "
            "\n";
    cout << ".______     ______     ______    __  ___  __  .__   __.   "
            "_______\n";
    cout << "|   _  \\   /  __  \\   /  __  \\  |  |/  / |  | |  \\ |  |  /  "
            "_____|\n";
    cout << "|  |_)  | |  |  |  | |  |  |  | |  '  /  |  | |   \\|  | |  |  __ "
            " \n";
    cout << "|   _  <  |  |  |  | |  |  |  | |    <   |  | |  . `  | |  | |_ | "
            "\n";
    cout << "|  |_)  | |  `--'  | |  `--'  | |  .  \\  |  | |  |\\   | |  |__| "
            "| \n";
    cout << "|______/   \\______/   \\______/  |__|\\__\\ |__| |__| \\__|  "
            "\\______| \n";
    cout << "\033[0m"; // Reset text color to default

    cout << "\033[1m\033[34m\nGeneral Menu Options:\033[0m\n"
         << "1. Book a room\n"
         << "2. View bookings\n"
         << "3. Conduct a survey\n"
         << "4. Admin access\n"
         << "5. Exit\n"
         << "Please enter your choice: ";

    cin >> menuOption; //Get menu option

    // Process the user's menu choice using a switch statement.
    switch (menuOption) {
      case 1: {
        int resortChoice;
        cout << "Enter the resort number to book (1-" << resorts.size() << "):\n";

        // Display a list of available resorts for booking.
        for (size_t i = 0; i < resorts.size(); ++i) {
          cout << "\n";
          cout << (i + 1) << ": " << resorts[i].name << " - "
               << resorts[i].description << endl;
        }
        cin >> resortChoice;

        // Validate the user's input for the resort choice.
        if (cin.fail() || resortChoice < 1 ||
            resortChoice > static_cast<int>(resorts.size())) {
          cout << "Invalid resort choice." << endl;
          cin.clear();
          cin.ignore(10000, '\n');
        } else {
          // Book a room at the selected resort and add commission to the management system.
          resorts[resortChoice - 1].bookRoom();
          managementSystem.addCommission(
              resorts[resortChoice - 1].calculatePrice(0) * 0.30);
        }
        break;
      }
      case 2:
        // Display bookings for all resorts.
        for (const auto &resort : resorts) {
          resort.viewBookings();
        }
        break;
      case 3:
        // Conduct a user satisfaction survey.
        managementSystem.conductSurvey();
        break;
      case 4:
        // Access the admin functions.
        managementSystem.adminAccess(resorts);
        break;
      case 5:
        // Exit the program with a goodbye message.
        cout << "Thank you for using the booking system. Goodbye!" << endl;
        break;
      default:
        // Handle invalid menu options.
        cout << "Invalid option, please try again." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        break;
    }
    // Continue displaying the menu until the user chooses to exit (menuOption 5).
    } while (menuOption != 5);
    // Clear the console before exiting.
    (void)system("clear");
    return 0;
}