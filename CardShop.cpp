/*
Name: Methila Deb
*/

#include "CardShop.hpp"
#include "Node.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


/**
  @pre  : the input file is in csv format: "name,type,level_rank,race,attribute,atk,def"
  @param: the name of the input file
  @post:  Reads the input file and adds pointers to YGOCard objects 
          instantiated with the information read from the file. 
    */
CardShop::CardShop(std::string input_file_name) 
{
  std::ifstream fin(input_file_name);
  if (fin.fail())
  {
    std::cerr << "File cannot be opened for reading. \n";
    exit(1); // exit if failed to open the file
  }
  //we don't use the first line
  std::string ignore;
  getline(fin, ignore);

  //the columns of the csv associated with the data members
  std::string name, type, race, attribute;
  int level_rank, atk, def;

  std::string temp_string;
  int temp_int;
  int item_counter = 0;
  while (getline(fin, temp_string, ','))
  {
    name = temp_string;

    getline(fin, type, ',');
  
    getline(fin, temp_string, ',');
    std::istringstream(temp_string) >> temp_int;
    level_rank = temp_int;

    getline(fin, race, ',');

    getline(fin, attribute, ',');

    getline(fin, temp_string, ',');
    std::istringstream(temp_string) >> temp_int;
    atk = temp_int;

    getline(fin, temp_string);
    std::istringstream(temp_string) >> temp_int;
    def = temp_int;

    //create new card
    YGOCard *card_ptr = new YGOCard(name, type, level_rank, race, attribute, atk, def);

    //add to linked list and increment counter
    insert(item_counter++, card_ptr);
  }
}

//@post: removes all cards from the shop
//Deallocate and delete items
void CardShop::clear()
{
  for (int i = 0; i < item_count_; i++)
  {
    YGOCard *ptr = getItem(i);
    delete ptr;
    ptr = nullptr;
  }
  DoublyLinkedList::clear();
}

//destructor
CardShop::~CardShop()
{
  clear();
}

//@post: displays information of all cards in the shop, one per line
void CardShop::display()
{
  for (int i = 0; i < item_count_; i++)
  {
    getItem(i)->display();
  }
}

  //@param: the index of the first card to be displayed
  //@param: the index of the last card to be displayed
  //@post: displays the names of all cards in the shop with startRange and endRange, 
  //       inclusive, one per line
void CardShop::displayName(int startRange, int endRange)
{
  for (int i = startRange; i <= endRange; i++)
  {
    getItem(i)->displayName();
    if (i != endRange)
    {
      std::cout << ", ";
    }
    else
    {
      std::cout << std::endl;
    }
  }
}


//@return:  true if all the cards in rhs are equal to the cards in the shop, false otherwise
bool CardShop::operator==(const CardShop &rhs) const
{
  if (getLength() != rhs.getLength())
  {
    return false;
  }
  for (int i = 0; i < getLength(); i++)
  {
    if (*(getPointerTo(i)->getItem()) == *(rhs.getPointerTo(i)->getItem()))
    {
    }
    else
    {
      return false;
    }
  }
  return true;
}

// Define a template function for bubble sorting items in CardShop using a Comparator object and a key
template<typename Comparator>
int CardShop::bubbleSort(Comparator comp, std::string key) {
  // Initialize a counter for the number of swaps made during the sorting process
  int swap_count = 0;

  // Declare variables for comparison
  int x, y;

  // Loop through the items in the CardShop
  for (int i=0; i<getLength(); i++) {

    // Loop through the remaining unsorted items
    for (int j=0; j<getLength()-i-1; j++) {

      // If the key is "atk", compare the attack stats of the two items
      if (key == "atk") {
        x = getPointerTo(j)->getItem()->getAtk();
        y = getPointerTo(j+1)->getItem()->getAtk();
      }

      // If the key is "def", compare the defense stats of the two items
      else if (key == "def") {
        x = getPointerTo(j)->getItem()->getDef();
        y = getPointerTo(j+1)->getItem()->getDef();
      }

      if(comp(y,x)) {
        swap(j,j+1);
        swap_count++;
      }
    }
  }
  // Return the number of swaps made during the sorting process
  return swap_count;
}

// Define a template function for insertion sorting items in CardShop using a Comparator object and a key
template<typename Comparator>
int CardShop::insertionSort(Comparator comp, std::string key){
  // Size of CardShop
  int size = getLength();

  // Initialize counter for number of swaps that will be performed during sorting
  int swap_count = 0;

  // Loop through items in CardShop starting from index 1
  for(int i=1;i<size;i++){

    // Set current item to be sorted to i
    int current = i;

    // If key is atk, attack values will be compared for current item & item to left until current is in correct position 
    if(key== "atk") {
      while(current>0 && comp(getItem(current)->getAtk(),getItem(current-1)->getAtk())){
        // Swap current item w/ item to its left
        swap(current,current-1);
        // Move to left to continue comparison
        current--;
        // Increment swap counter
        swap_count++;
      }
    }

    // If key is def, defense values will be compared for current item & item to left until current is in correct position
    else if(key=="def"){
      while(current>0 && comp(getItem(current)->getDef(),getItem(current-1)->getDef())){
        // Swap current ite, w/ item to its left
        swap(current,current-1);
        // Move to left to continue comparison
        current--;
        // Increment swap counter
        swap_count++;
      }
    }
  }
  // Return the number of swaps performed during the sort
  return swap_count;
}

// Define a template function for quick sorting items in CardShop using a Comparator object and a key
template <typename Comparator>
int CardShop::quickSort(Comparator comp, std::string key) {
  // Initialize counter for number of swaps that will be performed during sorting
  int swap_count = 0;

  // Call the quicksort helper function with the Comparator object, the range of the array to be sorted, the key, and the swap counter
  quickSortHelper(comp, 0, getLength() - 1, key, swap_count);

  // Return the number of swaps performed during the sort
  return swap_count;
}

// This is a helper function for quicksort that takes a Comparator object, a lower index, a higher index, a key, and a swap counter
template <typename Comparator>
void CardShop::quickSortHelper(Comparator comp, int lower, int higher, std::string key, int& swap_count) {
  // If the lower index is less than the higher index, perform quicksort on the array
  if (lower < higher) {
    // Determine the pivot index by partitioning the array using the Comparator object and the key
    int pivot_index = part(comp, lower, higher, key, swap_count);

    // Recursively call quickSortHelper on the left and right subarrays using the pivot index
    quickSortHelper(comp, lower, pivot_index - 1, key, swap_count);
    quickSortHelper(comp, pivot_index + 1, higher, key, swap_count);
  }
}

// This is a template helper function that partitions the array for quicksort based on the provided Comparator object and key
template <typename Comparator>
int CardShop::part(Comparator comp, int lower, int higher, std::string key, int& swap_count) {
  // Set the pivot to be the YGOCard object at the higher index
  YGOCard *pivot = getItem(higher);
  // Initialize a variable to keep track of the boundary between the partitioned and unpartitioned elements
  int i = lower - 1;

  // Iterate over the array from the lower index to the second-to-last index
  for (int j = lower; j < higher; j++) {
    // Get the current YGOCard object from the array
    YGOCard *card = getItem(j);
    // If the key is "atk", compare the attack values of the card and the pivot using the Comparator object
    if (key == "atk") {
      // If the attack value of the card is less than the attack value of the pivot, swap the card with the element at the boundary and increment the boundary
      if (comp(card->getAtk(), pivot->getAtk())) {
        i++;
        swap(i, j);
        swap_count++;
      }
    }

    // If the key is "def", compare the defense values of the card and the pivot using the Comparator object
    else if (key == "def") {
      // If the defense value of the card is less than the defense value of the pivot, swap the card with the element at the boundary and increment the boundary
      if (comp(card->getDef(), pivot->getDef())) {
        i++;
        swap(i, j);
        swap_count++;
      }
    }
  }
  // Swap the pivot with the element at the boundary and increment the swap counter
  swap(i + 1, higher);
  swap_count++;
  // Return the index of the pivot
  return i + 1;
}


