#include <iostream>
#include "LinkedList.h"

void test1();
void test2();

int main() {
  test1();
  test2();
}

void test1() {
  std::cout << "-=-=-=-= BEGIN TEST 1 -=-=-=-=" << std::endl;
  // Example transactions
  LinkedList list;
  list.printList();

  try {
    std::cout << "BEGIN TRANSACTION 1" << std::endl;
    list.prepareAdd(0, "e1");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  try {
    std::cout << "BEGIN TRANSACTION 2" << std::endl;
    list.prepareAdd(0, "e2");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  try {
    std::cout << "BEGIN TRANSACTION 3" << std::endl;
    list.prepareAdd(1, "e3");
    list.prepareAdd(0, "e4");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  try {
    std::cout << "BEGIN TRANSACTION 4" << std::endl;
    list.prepareRemove(3);
    list.prepareAdd(0, "e5");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  try {
    std::cout << "BEGIN TRANSACTION 5" << std::endl;
    list.prepareRemove(1);
    list.prepareAdd(0, "e6");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Violate Remove Lock
  try {
    std::cout << "BEGIN TRANSACTION 6" << std::endl;
    list.prepareRemove(1);
    list.prepareRemove(1);
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Double Remove
  try {
    std::cout << "BEGIN TRANSACTION 7" << std::endl;
    list.prepareRemove(1);
    list.prepareRemove(0);
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Double Add
  try {
    std::cout << "BEGIN TRANSACTION 8" << std::endl;
    list.prepareAdd(0, "e7");
    list.prepareAdd(0, "e8");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();
  
  // Violate Add Lock
  try {
    std::cout << "BEGIN TRANSACTION 9" << std::endl;
    list.prepareAdd(0, "e9");
    list.prepareAdd(1, "e10");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();
  list.clear();
  list.printList();
  std::cout << "-=-=-=-= END TEST 1 -=-=-=-=" << std::endl << std::endl;
}

void test2() {
  std::cout << "-=-=-=-= BEGIN TEST 2 -=-=-=-=" << std::endl;
  // Example transactions
  LinkedList list;
  list.printList();

  // Fill list
  try {
    std::cout << "BEGIN TRANSACTION 1" << std::endl;
    list.prepareAdd(0, "e1");
    list.prepareAdd(0, "e2");
    list.prepareAdd(0, "e3");
    list.prepareAdd(0, "e4");
    list.prepareAdd(0, "e5");
    list.prepareAdd(0, "e6");
    list.prepareAdd(0, "e7");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Add to end of list
  try {
    std::cout << "BEGIN TRANSACTION 2" << std::endl;
    list.prepareAdd(7, "e8");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Add to list._length+1, expect fail
  try {
    std::cout << "BEGIN TRANSACTION 2" << std::endl;
    list.prepareAdd(9, "e9");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Add to -1, expect fail
  try {
    std::cout << "BEGIN TRANSACTION 3" << std::endl;
    list.prepareAdd(9, "e10");
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Remove from list._length and beyond, expect fail
  try {
    std::cout << "BEGIN TRANSACTION 4" << std::endl;
    list.prepareRemove(8);
    list.prepareRemove(9);
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Remove from -1, expect fail
  try {
    std::cout << "BEGIN TRANSACTION 5" << std::endl;
    list.prepareRemove(8);
    list.prepareRemove(9);
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();

  // Remove from 0
  try {
    std::cout << "BEGIN TRANSACTION 6" << std::endl;
    list.prepareRemove(0);
    list.commit();
  } catch (std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    list.rollback();
  }
  list.printList();
  list.clear();
  list.printList();
  std::cout << "-=-=-=-= END TEST 2 -=-=-=-=" << std::endl << std::endl;
}