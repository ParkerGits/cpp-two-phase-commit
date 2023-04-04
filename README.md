# LinkedList Two-Phase Commit Interface

Two-Phase Commit interface for Linked List operations, implemented in C++. See main.cpp for example usage.

- `void prepareAdd(int index, const char* string);`
  - Includes an "add" command in the current transaction. For use during phase 1.
- `void prepareRemove(int index);`
  - Includes a "remove" command in the current transaction. For use during phase 1.
- `void commit();`
  - Commits all commands in current transaction, mutating the linked list. Has high probability of success. Represents phase 2 commit.
- `void rollback();`
  - Rollback all commands in current transaction, clearing all prepared statements. Has high probability of success.
