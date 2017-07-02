#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

template <class TObject, class TOperation>
class AggregatingQueue {
 public:
  AggregatingQueue(const TOperation& operation) : operation_(operation) {}

  void Push(const TObject& object) {
    Push(push_stack_, object);
  }

  void Pop() {
    Flush();
    pop_stack_.pop_back();
  }

  const TObject& Front() {
    Flush();
    return pop_stack_.back().first;
  }

  size_t Size() const {
    return push_stack_.size() + pop_stack_.size();
  }

  bool Empty() const {
    return push_stack_.empty() && pop_stack_.empty();
  }

  const TObject Aggregate() const {
    if (push_stack_.empty()) {
      return pop_stack_.back().second;
    }
    if (pop_stack_.empty()) {
      return push_stack_.back().second;
    }
    return operation_(push_stack_.back().second, pop_stack_.back().second);
  }

 private:
  typedef std::vector<std::pair<TObject, TObject>> Stack;

  void Push(Stack& stack, const TObject& object) {
    stack.emplace_back(
      object,
      stack.empty() ? object : operation_(stack.back().second, object)
    );
  }

  void Flush() {
    if (!pop_stack_.empty()) {
      return;
    }
    while (!push_stack_.empty()) {
      Push(pop_stack_, push_stack_.back().first);
      push_stack_.pop_back();
    }
  }

  Stack push_stack_;
  Stack pop_stack_;
  TOperation operation_;
};

void PlusQueueTest() {
  const auto plus = std::plus<int>();
  AggregatingQueue<int, std::plus<int>> queue(plus);
  queue.Push(5);
  queue.Push(10);
  queue.Push(15);
  assert(queue.Aggregate() == 30);
  queue.Pop();
  assert(queue.Aggregate() == 25);
  queue.Push(100);
  assert(queue.Aggregate() == 125);
  std::cout << "PlusQueueTest passed: OK" << std::endl;
}

void MaxQueueTest() {
  typedef const int& (*MaxFunc)(const int&, const int&);
  AggregatingQueue<int, MaxFunc> queue(std::max<int>);
  queue.Push(15);
  queue.Push(10);
  queue.Push(5);
  assert(queue.Aggregate() == 15);
  queue.Pop();
  assert(queue.Aggregate() == 10);
  queue.Pop();
  assert(queue.Aggregate() == 5);
  queue.Push(100);
  assert(queue.Aggregate() == 100);
  std::cout << "MaxQueueTest passted: OK" << std::endl;
}

int main() {
  PlusQueueTest();
  MaxQueueTest();
  std::cout << "All tests passed: OK" << std::endl;
  return 0;
}

