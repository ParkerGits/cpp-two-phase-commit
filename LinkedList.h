#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <vector>
#include <unordered_set>

class LinkedList {
  private:
  struct Node {
    const char* _element;
    Node* _next;
  };

  enum CommandType {
    REMOVE,
    ADD
  };

  struct Command {
    Node* _node;
    Node* _prev;
    int _index;
    CommandType _command;
  };


  Node* _head;
  unsigned int _length;
  std::vector<Command> _commandBuffer;
  int _addLock; // max add index for current transaction
  int _removeLock; // max remove index for current transaction

  void commitAdd(const Command& addCmd);
  void commitRemove(const Command& removeCmd);
  void setLocksAdd(int index);
  void setLocksRemove(int index);
  void resetLocks();

  public:
  LinkedList();
  ~LinkedList();
  void prepareAdd(int index, const char* string);
  void prepareRemove(int index);
  void printPreparedTransaction();
  void printList();
  void commit();
  void rollback();
  void clear();
};

#endif