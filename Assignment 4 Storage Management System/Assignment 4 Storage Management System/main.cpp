// Ash Nguyen
// CIS-17B-S25-33477
// 4/16/25
// Assignment 4 Storage Management System

#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>
#include <limits>
#include <string>

// ======================== //
// Custom Exception Classes //
// ======================== //
class DuplicateItemException : public std::runtime_error 
{
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error 
{
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

// ================= //
// Stored Item Class //
// ================= //
class StoredItem 
{
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(std::move(id)), description(std::move(desc)), location(std::move(loc)) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

// ===================== //
// Storage Manager Class //
// ===================== //
class StorageManager 
{
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    // adds an item to both maps
    void addItem(const std::shared_ptr<StoredItem>& item) 
    {
        // throws exception if the item already exists with that id when user attempts to add a new item
        if (itemById.count(item->getId())) 
        {
            throw DuplicateItemException("Item with ID " + item->getId() + " already exists!");
        }
        itemById[item->getId()] = item;
        itemByDescription[item->getDescription()] = item;
    }

    // finds and returns the item by id
    std::shared_ptr<StoredItem> findById(const std::string& id) const 
    {
        auto it = itemById.find(id);
        // throws exception if the item with the ID the user typed is not found in the system so it cannot be found
        if (it == itemById.end()) 
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        return it->second;
    }

    // removes the item from both maps
    void removeItem(const std::string& id) 
    {
        auto it = itemById.find(id);
        // throws exception if the item with the ID the user typed is not found in the system so it cannot be removed
        if (it == itemById.end()) 
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        std::string desc = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(desc);
    }

    // lists items by the order of their description
    void listItemsByDescription() const 
    {
        std::cout << "\nItems in Description Order:\n";
        for (const auto& pair : itemByDescription) 
        {
            std::cout << "- " << pair.second->getDescription() << ": " << pair.second->getLocation() << "\n";
        }
        std::cout << std::endl;
    }
};

// =================== //
// Function Prototypes //
// =================== //
void displayMenu();                             // displays user menu for Congo Storage Management System
void clearInput();                              // clears input buffer
void addItem(StorageManager& manager);          // handles adding an item
void findItem(const StorageManager& manager);   // handles finding an item by ID
void removeItem(StorageManager& manager);       // handles removing an item by ID
void testDuplicateAddition();                   // test function for duplicate addition of an item
void testItemNotFound();                        // test function for item not found


// ============= //
// Main Function //
// ============= //
int main()
{
    StorageManager manager;
    int choice;

    // Constants for menu choices
    const int ADD_ITEM_CHOICE = 1;
    const int FIND_ITEM_CHOICE = 2;
    const int REMOVE_ITEM_CHOICE = 3;
    const int LIST_ITEMS_CHOICE = 4;
    const int TEST_DUPLICATE_CHOICE = 5;
    const int TEST_NOT_FOUND_CHOICE = 6;
    const int QUIT_CHOICE = 7;

    do {
        displayMenu();
        std::cin >> choice;
        clearInput();

        switch (choice)
        {
        case ADD_ITEM_CHOICE:       // choice for adding an item
            addItem(manager);
            break;

        case FIND_ITEM_CHOICE:      // choice for finding an item
            findItem(manager);
            break;

        case REMOVE_ITEM_CHOICE:    // choice for removing an item
            removeItem(manager);
            break;

        case LIST_ITEMS_CHOICE:     // choice for listing items by description
            manager.listItemsByDescription();
            break;

        case TEST_DUPLICATE_CHOICE: // choice for testing when attempting to add duplicate items
            testDuplicateAddition();
            break;

        case TEST_NOT_FOUND_CHOICE: // choice for testing finding an item that doesn't exist
            testItemNotFound();
            break;

        case QUIT_CHOICE:           // choice for exiting the congo storage management system
            std::cout << "Thank you for using the storage management system!\n";
            break;

        default:                    // if the user selects a choice not listed
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != QUIT_CHOICE);

    return 0;
}
//******************************************//
//				   Functions				//
//******************************************//

// ====================== //
// Menu Function for User //
// ====================== //
void displayMenu()
{
    std::cout << "\n        Congo Storage Management System\n";
    std::cout << "===============================================\n";
    std::cout << "1. Add Item\n";
    std::cout << "2. Find Item by ID\n";
    std::cout << "3. Remove Item\n";
    std::cout << "4. List Items by Description\n";
    std::cout << "5. Run Duplicate Addition Test\n";
    std::cout << "6. Run Item Not Found Test\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter choice: ";
}
// ************************************************************* //

// =========================== //
// Clear Input Buffer Function //
// =========================== //
void clearInput()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
// ************************************************************* //

// ================= //
// Add item Function //
// ================= //
void addItem(StorageManager& manager)
{
    std::string id, description, location;
    std::cout << "Enter item ID: ";
    std::getline(std::cin, id);
    std::cout << "Enter item description: ";
    std::getline(std::cin, description);
    std::cout << "Enter item location: ";
    std::getline(std::cin, location);

    auto item = std::make_shared<StoredItem>(id, description, location);
    try
    {
        manager.addItem(item);
        std::cout << "Item added successfully.\n";
    }
    // throws exception when detected user attempted to add a duplicate item
    catch (const DuplicateItemException& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
// ************************************************************* //

// ================== //
// Find item Function //
// ================== //
void findItem(const StorageManager& manager)
{
    std::string id;
    std::cout << "Enter item ID to find: ";
    std::getline(std::cin, id);
    try
    {
        auto found = manager.findById(id);
        std::cout << "Found: " << found->getDescription() << " at " << found->getLocation() << "\n";
    }
    // throws exception if item is not found
    catch (const ItemNotFoundException& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
// ************************************************************* //

// ==================== //
// Remove item Function //
// ==================== //
void removeItem(StorageManager& manager)
{
    std::string id;
    std::cout << "Enter item ID to remove: ";
    std::getline(std::cin, id);
    try
    {
        manager.removeItem(id);
        std::cout << "Item removed successfully.\n";
    }
    // throws exception if item is not found
    catch (const ItemNotFoundException& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
// ************************************************************* //

// ======================================== //
// Test for adding duplicate items Function //
// ======================================== //

void testDuplicateAddition()
{
    std::cout << "\nRunning testDuplicateAddition...\n";
    StorageManager testManager;

    auto item1 = std::make_shared<StoredItem>("ID1", "Test Item", "Shelf A");
    auto item2 = std::make_shared<StoredItem>("ID1", "Duplicate Test Item", "Shelf B");

    try
    {
        testManager.addItem(item1);
        testManager.addItem(item2);  // Should trigger the exception
        std::cout << "Test Failed: DuplicateItemException not thrown.\n";
    }
    // successfully throws exception when attempting to add a duplicate item
    catch (const DuplicateItemException& e)
    {
        std::cout << "Passed: " << e.what() << "\n";
    }
}
// ************************************************************* //

// =========================================== //
// Test for finding an item that doesn't exist //
// =========================================== //
void testItemNotFound()
{
    std::cout << "\nRunning testItemNotFound...\n";
    StorageManager testManager;

    try
    {
        auto item = testManager.findById("NON_EXISTENT_ID");  // Should trigger exception
        std::cout << "Test Failed: ItemNotFoundException not thrown.\n";
    }
    // successfully throws exception when attempting to find an item that doesn't exist
    catch (const ItemNotFoundException& e)
    {
        std::cout << "Passed: " << e.what() << "\n";
    }
}
// ************************************************************* //