#include "LinkedList.h"
#include "PhaseOneException.h"
#include <iostream>
 
LinkedList::LinkedList(): _length(0), _head(nullptr), _addLock(INT_MAX), _removeLock(INT_MAX) {}

LinkedList::~LinkedList() {
  clear();
}

// Allocate the node to be added
// Push command into command buffer
// Create lock
// Throw any errors if they occur
void LinkedList::prepareAdd(int index, const char* string) {
  if (index > _length || index < 0) {
    throw PhaseOneException("Add Index Out of Bounds");
  }

  if (index >= _addLock) {
    throw PhaseOneException("Add Lock Violation");
  }
  
  // Allocate element to add
  Node* toAdd = new Node();
  toAdd->_next = nullptr;
  toAdd->_element = string;

  // Determine prev node
  Node* tmp = nullptr;
  if (index != 0) {
    tmp = _head;
    for (int i = 0; i < index-1; i++) {
      tmp = tmp->_next;
    } 
  }

  Command newAddCommand;
  newAddCommand._command = LinkedList::ADD;
  newAddCommand._node = toAdd;
  newAddCommand._prev = tmp;
  newAddCommand._index = index;

  _commandBuffer.push_back(newAddCommand);

  setLocksAdd(index);

  std::cout << "PREPARE ADD " << index << " " << string << std::endl;
}

// Find node to remove in list
// Push command into command buffer
// Create lock
// Throw any errors if they occur
void LinkedList::prepareRemove(int index) {
  if (index >= _length || index < 0) {
    throw PhaseOneException("Remove Index Out of Bounds");
  };

  if (index >= _removeLock) {
    throw PhaseOneException("Remove Lock Violation");
  }

  // iterate to target index
  Node* tmp = _head;
  Node* prev = nullptr;
  for (int i = 0; i < index; i++) {
    prev = tmp;
    tmp = tmp->_next;
  }

  Command newRemoveCommand;
  newRemoveCommand._command = LinkedList::REMOVE;
  newRemoveCommand._node = tmp;
  newRemoveCommand._prev = prev;
  newRemoveCommand._index = index;

  _commandBuffer.push_back(newRemoveCommand);

  setLocksRemove(index);

  std::cout << "PREPARE REMOVE " << index << std::endl;
}

void LinkedList::printPreparedTransaction() {
  for(const auto& command : _commandBuffer) {
    std::cout << (command._command == LinkedList::ADD ? "ADD " : "DELETE ");
    std::cout << command._index << std::endl;
  }
}

void LinkedList::printList() {
  std::cout << "[ ";
  for (Node* tmp = _head; tmp != nullptr; tmp = tmp->_next) {
    std::cout << tmp->_element << " ";
  }
  std::cout << "]" << std::endl;
}

void LinkedList::commit(){
  for(const auto& command : _commandBuffer) {
    if (command._command == LinkedList::ADD) {
      commitAdd(command);
    } else {
      commitRemove(command);
    }
  }

  resetLocks();

  // Transaction committed, clear command buffer
  _commandBuffer.clear();

  std::cout << "COMMIT." << std::endl;
}

void LinkedList::rollback()
{
  // Deallocate all nodes allocated for add
  for(const auto& command : _commandBuffer) {
    if (command._command == LinkedList::ADD) {
      delete command._node;
    }
  }

  // Clear command buffer
  _commandBuffer.clear();

  // Reset locks
  resetLocks();

  std::cout << "ROLLBACK." << std::endl;
}

void LinkedList::clear() {
  Node* next = nullptr;
  while(_head != nullptr) {
    next = _head->_next;
    _head->_next = nullptr;
    delete _head;
    _head = next;
  }
  _length = 0;
  resetLocks();
  _commandBuffer.clear();
}

void LinkedList::commitAdd(const Command& cmd) {
  Node* newNode = cmd._node;
  if(cmd._prev == nullptr) {
    // inserting at head
    newNode->_next = _head;
    _head = newNode;
  }
  else {
    // inserting at middle or end of list
    Node* prevNode = cmd._prev;
    Node* next = prevNode->_next;
    prevNode->_next = newNode;
    newNode->_next = next;
  }
  _length++;
}

void LinkedList::commitRemove(const Command& cmd) {
  Node* toRemove = cmd._node;
  if(cmd._prev == nullptr) {
    // removing head
    _head = toRemove->_next;
    toRemove->_next = nullptr;
  } else {
    // removing from middle or end of list
    Node* prevNode = cmd._prev;
    Node* next = toRemove->_next;
    prevNode->_next = next;
    toRemove->_next = nullptr;
  }
  delete toRemove;
  _length--;
}

// Sets the locks for a given add command
void LinkedList::setLocksAdd(int index) {
  // Can no longer add to indices greater than or equal to index+1 for this transaction
  _addLock = index+1;
  // Can no longer add to indices greater than equal to index+1 for this transaction.
  _removeLock = index+1;
}

// Sets the locks for a given remove command
void LinkedList::setLocksRemove(int index) {
  // Can no longer add to indices greater than or equal to index+1 for this transaction.
  _addLock = index+1;
  // Can no longer remove from indices greater than equal to index for this transaction.
  _removeLock = index;
}

void LinkedList::resetLocks() {
  _addLock = INT_MAX;
  _removeLock = INT_MAX;
}
